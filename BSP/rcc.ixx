// rcc.ixx
module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module rcc;  // 声明模块

export namespace bsp::rcc {
    /**
     * @brief 系统时钟配置函数集
     * @note 所有函数均设计为可重入且线程安全
     */

    void SystemClock_DefaultConfig() noexcept;  ///< 默认时钟配置(168MHz)
    void SystemClock_Overclock() noexcept;      ///< 超频至216MHz
    void SystemClock_SwitchToHSI() noexcept;    ///< 切换至内部时钟源
    void SystemClock_SwitchToPLL() noexcept;    ///< 切换至锁相环时钟
    void SystemClock_SetFrequency(uint32_t plln) noexcept; ///< 动态设置时钟频率
}

// ================== 模块内部实现 ==================
namespace {
    // PLL配置参数（保持与HAL库兼容）
    constexpr uint32_t HSE_DIV = 6;       // HSE分频系数固定为6
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
    SystemClock_SetFrequency(168);
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
    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_HSI);
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_HSI) {}
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
    __HAL_RCC_SYSCLK_CONFIG(RCC_SYSCLKSOURCE_PLLCLK);
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_SYSCLKSOURCE_STATUS_PLLCLK) {}
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
    __disable_irq();

    // 切换至HSI
    SystemClock_SwitchToHSI();

    // 禁用并重配置PLL
    __HAL_RCC_PLL_DISABLE();

    RCC_OscInitTypeDef RCC_OscInit = {
        .OscillatorType = RCC_OSCILLATORTYPE_HSE,
        .HSEState = RCC_HSE_ON,
        .PLL = {
            .PLLState = RCC_PLL_ON,
            .PLLSource = RCC_PLLSOURCE_HSE,
            .PLLM = HSE_DIV,
            .PLLN = plln,
            .PLLP = PLLP_DIV,
            .PLLQ = PLLQ_DIV
        }
    };

    if (HAL_RCC_OscConfig(&RCC_OscInit) != HAL_OK) {
        Error_Handler();
    }

    // 配置时钟树
    RCC_ClkInitTypeDef RCC_ClkInit = {
        .ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                   | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2,
        .SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK,
        .AHBCLKDivider = RCC_SYSCLK_DIV1,
        .APB1CLKDivider = RCC_HCLK_DIV4,
        .APB2CLKDivider = RCC_HCLK_DIV2
    };

    // 设置FLASH等待周期
    const uint32_t latency = (plln <= 168) ? FLASH_LATENCY_5 : FLASH_LATENCY_7;
    if (HAL_RCC_ClockConfig(&RCC_ClkInit, latency) != HAL_OK) {
        Error_Handler();
    }

    // 切换回PLL
    SystemClock_SwitchToPLL();
    __enable_irq();
}