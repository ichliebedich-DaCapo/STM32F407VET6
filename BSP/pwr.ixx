// pwr.ixx
module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module pwr;  // 声明模块

export namespace bsp::pwr {
    // 电源管理初始化
    void pvd_init() noexcept;         // 初始化可编程电压检测
    void PowerModeInit() noexcept;    // 使能电源控制时钟

    // 低功耗模式控制
    void EnterSleepMode() noexcept;   // 进入睡眠模式
    void EnterStopMode() noexcept;    // 进入停止模式
    void EnterStandbyMode() noexcept; // 进入待机模式
}

// ================== 中断处理连接 ==================
extern "C" {
    /**
     * @brief PVD电压检测中断服务函数
     * @warning 必须保持C链接规范
     */
    void PVD_IRQHandler() {
        HAL_PWR_PVD_IRQHandler();  // 调用HAL库中断处理
    }
}

// ================== 实现导出的函数 ==================
namespace {
    // PVD配置参数（保持与原始代码一致）
    constexpr PWR_PVDTypeDef pvd_config = {
        .PVDLevel = PWR_PVDLEVEL_7,  // 约2.9V触发
        .Mode = PWR_PVD_MODE_IT_RISING_FALLING
    };
}

/**
 * @brief 初始化可编程电压检测器(PVD)
 * @details 配置电压阈值为2.9V，上升沿和下降沿触发中断
 * @note 中断优先级设为0（最高），需在project_config.h配置相关时钟
 */
void bsp::pwr::pvd_init() noexcept {
    __HAL_RCC_PWR_CLK_ENABLE();
    HAL_PWR_ConfigPVD(&pvd_config);
    HAL_PWR_EnablePVD();
    HAL_NVIC_SetPriority(PVD_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(PVD_IRQn);
}

/**
 * @brief 电源模式初始化
 * @details 使能PWR模块时钟，为低功耗模式配置做准备
 */
void bsp::pwr::PowerModeInit() noexcept {
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
}

/**
 * @brief 进入睡眠模式
 * @details 通过WFI指令进入，任何中断均可唤醒
 * @note 内核停止，外设保持运行，唤醒后程序继续执行
 */
void bsp::pwr::EnterSleepMode() noexcept {
    __WFI();  // 等待中断唤醒
}

/**
 * @brief 进入停止模式
 * @details 配置PDDS位并清除唤醒标志后进入
 * @note 所有时钟停止，SRAM和寄存器保留，需外部中断或RTC唤醒
 */
void bsp::pwr::EnterStopMode() noexcept {
    PWR->CR |= PWR_CR_PDDS;    // 设置深度睡眠模式
    PWR->CR |= PWR_CR_CWUF;    // 清除唤醒标志
    __WFI();                   // 进入停止模式
}

/**
 * @brief 进入待机模式
 * @details 配置PDDS位并清除唤醒标志后进入
 * @note 仅备份寄存器和待机电路保持供电，需复位或WKUP引脚唤醒
 */
void bsp::pwr::EnterStandbyMode() noexcept {
    PWR->CR |= PWR_CR_PDDS;    // 设置深度睡眠模式
    PWR->CR |= PWR_CR_CWUF;    // 清除唤醒标志
    __WFI();                   // 进入待机模式
}