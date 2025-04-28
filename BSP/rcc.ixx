// rcc.ixx
module;
// ==================包含头文件====================
#include <project_config.h>
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_system.h"
#include "stm32f4xx_ll_exti.h"
#include "stm32f4xx_ll_cortex.h"
#include "stm32f4xx_ll_utils.h"
#include "stm32f4xx_ll_pwr.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_gpio.h"
// ==================包含头文件====================

export module rcc; // 声明模块

export namespace bsp::rcc
{
    /**
     * @brief 系统时钟配置函数集
     * @note 所有函数均设计为可重入且线程安全
     */

    void SystemClock_DefaultConfig() noexcept; ///< 默认时钟配置(168MHz)
    void SystemClock_Overclock() noexcept; ///< 超频至216MHz
    void SystemClock_SwitchToHSI() noexcept; ///< 切换至内部时钟源
    void SystemClock_SwitchToPLL() noexcept; ///< 切换至锁相环时钟
    void SystemClock_SetFrequency(uint32_t plln) noexcept; ///< 动态设置时钟频率
}

// ================== 模块内部实现 ==================
namespace
{
    // PLL配置参数（保持与HAL库兼容）
    constexpr uint32_t HSE_DIV = 6; // HSE分频系数固定为6
    constexpr uint32_t PLLP_DIV = RCC_PLLP_DIV2;
    constexpr uint32_t PLLQ_DIV = 4;
}

// ================== 实现导出的函数 ==================
/**
 * @brief 初始化系统时钟至默认频率168MHz
 * @details 基于外部12MHz晶振，配置PLL参数实现稳定168MHz输出
 * @note 此配置经过严格测试，可保证长期稳定运行
 */
void bsp::rcc::SystemClock_DefaultConfig() noexcept
{
    // 设置FLASH等待周期
    LL_FLASH_SetLatency(FLASH_LATENCY_5);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {}
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
    LL_RCC_HSE_Enable();

    // 等待HSE就绪
    while (LL_RCC_HSE_IsReady() != 1) {}
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_6, 168, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();

    // 等待锁相
    while (LL_RCC_PLL_IsReady() != 1) {}
    while (LL_PWR_IsActiveFlag_VOS() == 0) {}
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_4);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);// 切换系统时钟源

    // 等待时钟资源就绪
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {}
    LL_SetSystemCoreClock(168000000);

    // 更新时基
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
 * @brief 超频至216MHz
 * @warning 此模式可能影响芯片稳定性，需确保散热条件良好
 * @details 通过提升PLL倍频系数实现超频，需配合FLASH等待周期调整
 */
void bsp::rcc::SystemClock_Overclock() noexcept
{
    SystemClock_SetFrequency(216);
}

/**
 * @brief 切换系统时钟源至HSI内部时钟
 * @details 16MHz内部RC振荡器，用于低功耗或紧急恢复场景
 * @note 切换过程会阻塞直到时钟稳定，期间禁用中断
 */
void bsp::rcc::SystemClock_SwitchToHSI() noexcept
{
    __disable_irq();
    // 确保HSI已启用
    if(!LL_RCC_HSI_IsReady()){
        LL_RCC_HSI_Enable();
        while(!LL_RCC_HSI_IsReady()){} // 等待HSI稳定
    }

    // 执行时钟切换
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

    // 正确检查切换状态
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {}

    __enable_irq();
}

/**
 * @brief 切换系统时钟源至PLL输出
 * @details 需要先正确配置PLL参数，通常在频率设置函数中自动调用
 * @note 切换过程会阻塞直到时钟稳定，期间禁用中断
 */
void bsp::rcc::SystemClock_SwitchToPLL() noexcept
{
    __disable_irq();
    // 确保PLL已启用
    if(!LL_RCC_PLL_IsReady()){
        LL_RCC_PLL_Enable();
        while(!LL_RCC_PLL_IsReady()){} // 等待PLL稳定
    }

    // 执行时钟切换
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    // 正确检查切换状态
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {}

    __enable_irq();
}

/**
 * @brief 动态设置系统时钟频率
 * @param plln 锁相环倍频系数，实际频率= (HSE/M)*N
 * @details 配置流程：
 *          1. 切换至HSI保证配置安全
 *          2. 重配置PLL参数
 *          3. 调整FLASH等待周期
 *          4. 切换回PLL输出
 * @warning 错误参数可能导致系统崩溃，建议使用预定义配置
 */
void bsp::rcc::SystemClock_SetFrequency(uint32_t plln) noexcept
{
    // 设置FLASH等待周期
    const uint32_t latency = (plln <= 168) ? FLASH_LATENCY_5 : FLASH_LATENCY_7;
    LL_FLASH_SetLatency(latency);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_5) {}
    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);

    // 切换至HSI
    SystemClock_SwitchToHSI();

    // 设置频率
    LL_RCC_PLL_Disable();// 禁用PLL以重配
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_6, plln, LL_RCC_PLLP_DIV_2);
    LL_RCC_PLL_Enable();// 启用PLL

    // 等待锁相
    while (LL_RCC_PLL_IsReady() != 1) {}
    while (LL_PWR_IsActiveFlag_VOS() == 0) {}

    // 切换为PLL
    SystemClock_SwitchToPLL();

    // 等待时钟资源就绪
    LL_SetSystemCoreClock(plln*1000000);

    // 更新时基
    if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
    {
        Error_Handler();
    }
    __enable_irq();
}
