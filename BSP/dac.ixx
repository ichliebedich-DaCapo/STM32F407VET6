// dac.ixx
module;
// ==================包含头文件====================
#include <project_config.h>
// ==================包含头文件====================

export module dac;  // 声明模块

#include <cstdint>

export namespace bsp::dac {
    inline DAC_HandleTypeDef hdac = {
        .Instance = DAC,
        .State = HAL_DAC_STATE_RESET
    };

    // 初始化配置参数
    struct config {
        uint32_t trigger = DAC_TRIGGER_NONE;
        uint32_t output_buffer = DAC_OUTPUTBUFFER_DISABLE;
    };

    // 操作接口
    void init(const config& cfg = {}) noexcept;
    void start() noexcept;
    void stop() noexcept;
    void set_value(uint16_t value) noexcept;
}

namespace bsp::dac::detail {
    // 硬件常量配置
    #define gpio_port GPIOA
    static constexpr uint16_t gpio_pin = GPIO_PIN_4;

    // 初始化GPIO（内部使用）
    static void init_gpio() noexcept {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitTypeDef init = {
            .Pin = gpio_pin,
            .Mode = GPIO_MODE_ANALOG,
            .Pull = GPIO_NOPULL,
            .Speed = GPIO_SPEED_FREQ_LOW,
            .Alternate = 0
        };
        HAL_GPIO_Init(gpio_port, &init);
    }
}

// ================== 实现导出的函数 ==================
void bsp::dac::init(const config& cfg) noexcept {
    using namespace detail;

    __HAL_RCC_DAC_CLK_ENABLE();
    init_gpio();

    HAL_DAC_Init(&hdac);

    const DAC_ChannelConfTypeDef ch_cfg = {
        .DAC_Trigger = cfg.trigger,
        .DAC_OutputBuffer = cfg.output_buffer
    };
    HAL_DAC_ConfigChannel(&hdac, &ch_cfg, DAC_CHANNEL_1);
}

void bsp::dac::start() noexcept {
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
}

void bsp::dac::stop() noexcept {
    HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
}

void bsp::dac::set_value(const uint16_t value) noexcept {
    HAL_DAC_SetValue(&hdac,
                    DAC_CHANNEL_1,
                    DAC_ALIGN_12B_R,
                    value);
}