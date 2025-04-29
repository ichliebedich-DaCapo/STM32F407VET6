//
// Created by fairy on 2025/4/24 18:03.
//
module;
#include <stm32f4xx_hal.h>
#include "stm32f4xx_ll_rcc.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
export module gpio;


namespace bsp
{
    // 前置声明GPIO端口特性模板
    template<uint32_t PortBase>
    struct gpio_port_traits;

    // 端口特性特化模板
#define DEFINE_GPIO_TRAITS(PORT_BASE,RCC_ENR) \
template<> \
struct gpio_port_traits<PORT_BASE> { \
static void enable_clock() { RCC_ENR; } \
};

    // 定义所有GPIO端口的特性（根据具体型号调整）
    DEFINE_GPIO_TRAITS(GPIOA_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA))
    DEFINE_GPIO_TRAITS(GPIOB_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB))
    DEFINE_GPIO_TRAITS(GPIOC_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC))
    DEFINE_GPIO_TRAITS(GPIOD_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD))
    DEFINE_GPIO_TRAITS(GPIOE_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOE))
    DEFINE_GPIO_TRAITS(GPIOF_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF))
    DEFINE_GPIO_TRAITS(GPIOG_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOG))
    DEFINE_GPIO_TRAITS(GPIOH_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOH))
    DEFINE_GPIO_TRAITS(GPIOI_BASE, LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOI))

}



export namespace bsp
{
    template<uint32_t PortBase, uint16_t Pin>
    struct gpio
    {
        // 引脚属性
        constexpr static GPIO_TypeDef* port(){return reinterpret_cast<GPIO_TypeDef*>(PortBase);}
        constexpr static uint16_t pin(){return Pin;}

        // 编译时获取端口特性
        using traits = gpio_port_traits<PortBase>;

        // 初始化/重新配置 仅需配置outPut、pull和speed即可
        static inline void init(LL_GPIO_InitTypeDef& config)
        {
            traits::enable_clock();// 启用时钟
            config.Pin = Pin;
            LL_GPIO_Init(port(), &config);
        }

        // 模式切换API ---------------------------------------------------

        // 设置为输出模式
        static inline void setOutput(const uint32_t Pull = GPIO_NOPULL, const uint32_t Speed = GPIO_SPEED_FREQ_LOW)
        {
            LL_GPIO_InitTypeDef config{};
            config.Mode = GPIO_MODE_OUTPUT_PP;
            config.Pull = Pull;
            config.Speed = Speed;
            init(config);
        }

        // 设置为输入模式
        static void setInput(const uint32_t Pull = GPIO_NOPULL, const uint32_t Speed = GPIO_SPEED_FREQ_LOW)
        {
            LL_GPIO_InitTypeDef config{};
            config.Mode = GPIO_MODE_INPUT;
            config.Pull = Pull;
            config.Speed = Speed;
            init(config);
        }


        // 基础操作 ------------------------------------------------------
        inline static void high() { port()->BSRR = Pin; }
        inline static void low() { port()->BSRR = static_cast<uint32_t>(Pin) << 16; }
        inline static void toggle() { port()->ODR ^= Pin; }
        inline static bool read() { return port()->IDR & Pin; }

        // 兼容HAL的API
        static void write(const GPIO_PinState state)
        {
            HAL_GPIO_WritePin(port(), Pin, state);
        }
    };
}
