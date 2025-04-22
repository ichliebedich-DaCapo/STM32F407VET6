module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module key_exit; // 声明模块

export namespace bsp::key_exit
{
    void init() noexcept; // 初始化按键中断
}


// ================== 模块内部实现 ==================
namespace
{
    // 中断优先级配置（直接映射原始数值）
    constexpr uint32_t EXTI0_IRQ_PRIORITY = 0xF;
    constexpr uint32_t EXTI0_IRQ_SUBPRIORITY = 0x00;
}

// ================== 实现导出的函数 ==================
/**
 * @brief 外部中断初始化函数
 * @details 初始化PB0引脚为上升沿触发中断，默认启用EXTI0
 * @note 优先级设为最低(0xF)以便RTOS管理，保留EXTI1条件编译扩展点
 */
void bsp::key_exit::init() noexcept
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef exti_gpio_config = {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = 0 // 保持与原始代码相同的隐式初始化
    };
    HAL_GPIO_Init(GPIOB, &exti_gpio_config);

    HAL_NVIC_SetPriority(EXTI0_IRQn, EXTI0_IRQ_PRIORITY, EXTI0_IRQ_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);

    /* 保留EXTI1条件编译扩展点（需在project_config.h定义EXTI1_OPEN） */
    // #if EXTI1_OPEN
    //     GPIO_InitStructure.Pin = GPIO_PIN_1;
    //     HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    //     HAL_NVIC_SetPriority(EXTI1_IRQn, 0x05, 0x00);
    //     HAL_NVIC_EnableIRQ(EXTI1_IRQn);
    // #endif
}

// ================== 中断处理连接 ==================
