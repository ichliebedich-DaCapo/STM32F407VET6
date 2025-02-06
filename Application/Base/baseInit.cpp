//
// Created by fairy on 2024/9/22.
//
#include "baseInit.hpp"
#include "stm32f4xx_hal.h"
#include "lcd.h"
#include "touch.h"
#include "fsmc.h"
#include "key_exit.h"
#include "App_Conf.h"
#include "CPU_RunTime.h"
#include "key.hpp"
#include "RCC.h"
#ifndef GUI_DISABLE
#include "lvgl.h"
#include "lv_port_disp.h"
#include "GUI.hpp"
#endif



TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;



void BaseInit()
{
    HAL_Init();
    SystemClock_DefaultConfig();// 系统时钟初始化

    // 开启FreeRTOS的运行时统计信息
#if FreeRTOS_DEBUG && defined(FreeRTOS_ENABLE)
    ConfigureTimerForRunTimeStats();
#endif

    fsmc_init();
    lcd_init();// 初始化LCD
    key_exti_init();
//    touch_init();// 初始化触摸屏

#ifdef USE_FSMC_DMA
    fsmc_dma_init();// 初始化FSMC+DMA
#endif
}




// 用于初始化硬件资源
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // 启用基础GPIO时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
}


/*用于配置供HAL使用基础时钟，频率为1KHz*/
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    RCC_ClkInitTypeDef clkconfig;
    uint32_t uwTimclock, uwAPB1Prescaler = 0U;

    uint32_t uwPrescalerValue = 0U;
    uint32_t pFLatency;
    HAL_StatusTypeDef status;

    /* Enable TIM7 clock */
    __HAL_RCC_TIM7_CLK_ENABLE();
    /* Get clock configuration */
    HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);
    /* Get APB1 prescaler */
    uwAPB1Prescaler = clkconfig.APB1CLKDivider;
    /* Compute TIM7 clock */
    if (uwAPB1Prescaler == RCC_HCLK_DIV1)
    {
        uwTimclock = HAL_RCC_GetPCLK1Freq();
    } else
    {
        uwTimclock = 2UL * HAL_RCC_GetPCLK1Freq();
    }

    /* Compute the prescaler value to have TIM7 counter clock equal to 1MHz */
    uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

    /* Initialize TIM7 */
    htim7.Instance = TIM7;

    /* Initialize TIMx peripheral as follow:

    + Period = [(TIM7CLK/1000) - 1]. to have a (1/1000) s time base.
    + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
    + ClockDivision = 0
    + Counter direction = Up
    */
    htim7.Init.Period = (1000000U / 1000U) - 1U;
    htim7.Init.Prescaler = uwPrescalerValue;
    htim7.Init.ClockDivision = 0;
    htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
// 考虑到LVGL初始化会死循环

    status = HAL_TIM_Base_Init(&htim7);
    if (status == HAL_OK)
    {
        /* Start the TIM time Base generation in interrupt mode */
        status = HAL_TIM_Base_Start_IT(&htim7);
        if (status == HAL_OK)
        {
            /* Enable the TIM7 global Interrupt */
            HAL_NVIC_EnableIRQ(TIM7_IRQn);
            /* Configure the SysTick IRQ priority */
            if (TickPriority < (1UL << __NVIC_PRIO_BITS))
            {
                /* Configure the TIM IRQ priority */
                HAL_NVIC_SetPriority(TIM7_IRQn, TickPriority, 0U);
                uwTickPrio = TickPriority;
            } else
            {
                status = HAL_ERROR;
            }
        }
    }

    /* Return function status */
    return status;
}


inline void HAL_SuspendTick(void)
{
    /* Disable TIM7 update Interrupt */
    __HAL_TIM_DISABLE_IT(&htim7, TIM_IT_UPDATE);
}

inline void HAL_ResumeTick(void)
{
    /* Enable TIM7 Update interrupt */
    __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
}

// TIM7中断处理函数
extern "C"
{
void TIM7_IRQHandler()
{
//    if (__HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) != RESET)
//    {
//        __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
//        HAL_IncTick();
//        lv_tick_inc(1);
//    }

    // 我把TIM7当做系统时钟，只用到了基础定时器的更新计时功能，所以并不需要判断中断源
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    HAL_IncTick();

#ifndef GUI_DISABLE
    lv_tick_inc(1);
#endif
}
}


#undef DMA2_S6CR

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    for (;;)
    {

    }
}
#endif /* USE_FULL_ASSERT */