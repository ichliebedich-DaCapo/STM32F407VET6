//
// Created by 34753 on 2024/10/10.
//
module;
// ================== 包含头文件 ====================
#include <project_config.h>
#include <type_traits>
// ================== 包含头文件 ====================
export module timer;

// ================== 硬件抽象层 ==================
export namespace bsp::timer
{
    // 定时器编号标签
    struct TIM2_tag {};

    struct TIM6_tag {};

    // 编译时类型特征萃取
    template<typename T>
    struct timer_traits;

    template<>
    struct timer_traits<TIM2_tag>
    {
        static constexpr auto instance = TIM2_BASE;
        static constexpr auto clock_enable = [] { __HAL_RCC_TIM2_CLK_ENABLE(); };
        static constexpr IRQn_Type irq = TIM2_IRQn;
        static constexpr uint32_t apb_check_mask = RCC_CFGR_PPRE1_2;
    };

    template<>
    struct timer_traits<TIM6_tag>
    {
        static constexpr auto instance = TIM6_BASE;
        static constexpr auto clock_enable = [] { __HAL_RCC_TIM6_CLK_ENABLE(); };
        static constexpr IRQn_Type irq = TIM6_DAC_IRQn;
        static constexpr uint32_t apb_check_mask = RCC_CFGR_PPRE2_2;
    };

    // 定时器策略模板
    template<typename TimerTag, typename CallbackType = void(*)()>
    class Timer final
    {
        // 编译期确定的硬件特性
        using traits = timer_traits<TimerTag>;

        // 静态句柄（零存储开销）
        inline static TIM_HandleTypeDef handle{
            .Instance = reinterpret_cast<TIM_TypeDef *>(traits::instance),
            .Init = TIM_Base_InitTypeDef{
                .Prescaler = 0,
                .CounterMode = TIM_COUNTERMODE_UP,
                .Period = 0,
                .ClockDivision = TIM_CLOCKDIVISION_DIV1,
                .AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
            }
        };

        // 编译期绑定的回调函数
        inline static CallbackType user_callback = nullptr;

    public:
        // 初始化（编译期计算时钟）
        template<uint32_t PSC, uint32_t ARR>
        static void init() noexcept
        {
            traits::clock_enable();

            handle.Init.Prescaler = PSC;
            handle.Init.Period = ARR;
            HAL_TIM_Base_Init(&handle);

            if constexpr (std::is_same_v<TimerTag, TIM2_tag>)
            {
                constexpr TIM_MasterConfigTypeDef master = {
                    .MasterOutputTrigger = TIM_TRGO_UPDATE,
                    .MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE
                };
                HAL_TIMEx_MasterConfigSynchronization(&handle, &master);
            }
            else
            {
                constexpr TIM_MasterConfigTypeDef master = {
                    .MasterOutputTrigger = TIM_TRGO_RESET,
                    .MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE
                };
                HAL_TIMEx_MasterConfigSynchronization(&handle, &master);

                HAL_NVIC_SetPriority(traits::irq, 5, 0);
                HAL_NVIC_EnableIRQ(traits::irq);
                __HAL_TIM_CLEAR_IT(&handle, TIM_IT_UPDATE);
            }
        }

        // 运行时频率设置（强制内联）
        static inline void set_freq(const uint32_t freq) noexcept
        {
            constexpr uint32_t timer_clock = []
            {
                if constexpr (std::is_same_v<TimerTag, TIM2_tag>)
                {
                    return HAL_RCC_GetPCLK1Freq() * ((RCC->CFGR & traits::apb_check_mask) ? 2 : 1);
                }
                else
                {
                    return HAL_RCC_GetPCLK2Freq() * ((RCC->CFGR & traits::apb_check_mask) ? 2 : 1);
                }
            }();

            if (freq == 0 || freq > timer_clock) return;
            const uint32_t psc = (timer_clock / freq) - 1;
            if (psc > 0xFFFF) return;

            __HAL_TIM_SET_PRESCALER(&handle, psc);
        }

        // 中断回调注册（类型安全）
        static void register_callback(CallbackType cb) noexcept
        {
            user_callback = cb;
        }

        // 中断处理（编译期绑定）
        template<typename = void> // SFINAE控制
        static void irq_handler()
        {
            __HAL_TIM_CLEAR_FLAG(&handle, TIM_FLAG_UPDATE);
            if constexpr (!std::is_same_v<CallbackType, nullptr_t>)
            {
                if (user_callback) user_callback();
            }
        }

        static inline void start() noexcept
        {
            HAL_TIM_Base_Start_IT(&handle);
        }

        static inline void stop() noexcept
        {
            HAL_TIM_Base_Stop_IT(&handle);
        }
    };
}

// ================== 中断处理连接 ==================
// TIM6中断处理（编译期静态绑定）
extern "C" void TIM6_DAC_IRQHandler()
{
    bsp::timer::Timer<bsp::timer::TIM6_tag>::irq_handler();
}

// ================== 用户接口别名 ==================
export using timer2 = bsp::timer::Timer<bsp::timer::TIM2_tag>;
export using timer6 = bsp::timer::Timer<bsp::timer::TIM6_tag>;
