module; // 全局模块片段用于包含C头文件
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================
export module delay; // 声明模块

// ================== 导出函数 ====================
export namespace bsp::delay
{
    bool init() noexcept;

    void us(uint16_t us) noexcept;

    void ms(uint32_t ms) noexcept;

    uint32_t tick() noexcept;
}

namespace bsp::delay::detail
{
    // 硬件句柄封装为inline变量（C++17）
    inline TIM_HandleTypeDef htim14 = {
        .Instance = TIM14,
        .Init = {
            .Prescaler = 83, // 84MHz -> 1MHz计数
            .CounterMode = TIM_COUNTERMODE_UP,
            .Period = 0xFFFF,
            .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
        }
    };

    // 替代宏的常量
    static constexpr uint32_t CPU_FREQ_MHZ = 84;

    // 内部函数带错误检查
    static bool timer_start() noexcept { return (HAL_TIM_Base_Start(&htim14) == HAL_OK); }

    static void timer_stop() noexcept { HAL_TIM_Base_Stop(&htim14); }

    // 硬件初始化（返回成功状态）
    static bool configure() noexcept
    {
        __HAL_RCC_TIM14_CLK_ENABLE();

        if (HAL_TIM_Base_Init(&htim14) != HAL_OK) { return false; }

        TIM_MasterConfigTypeDef master_config = {
            .MasterOutputTrigger = TIM_TRGO_RESET,
            .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
        };

        return (HAL_TIMEx_MasterConfigSynchronization(&htim14, &master_config) == HAL_OK);
    }
}

// 实现导出的函数
bool bsp::delay::init() noexcept
{
    static bool initialized = false;

    if (!initialized)
    {
        if (!detail::configure())
        {
            Error_Handler(); // 初始化失败触发处理
            return false;
        }
        if (!detail::timer_start())
        {
            Error_Handler();
            return false;
        }
        initialized = true;
    }
    return true;
}

void bsp::delay::us(const uint16_t us) noexcept
{
    // 启动定时器

    // 频率为84MHz，84M/1000 = 84000
    const volatile uint16_t final_count = TIM14->CNT + us;

    // 针对溢出情况
    while (TIM14->CNT > final_count);

    // 非溢出
    while (TIM14->CNT < final_count);

    // 停止定时器
}

void bsp::delay::ms(uint32_t ms) noexcept
{
    while (ms--)
    {
        us(1000); // 简单实现，实际可优化误差
    }
}

uint32_t bsp::delay::tick() noexcept { return detail::htim14.Instance->CNT; }
