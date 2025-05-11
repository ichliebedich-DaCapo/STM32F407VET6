//
// Created by fairy on 2024/9/22.
//
module;
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
#ifdef GUI_ENABLE
#include "lvgl.h"
#include "lv_port_disp.h"
#endif

export module baseInit;

import fsmc;
import rcc;
import spi;
import usart;
import delay;
// GUI
#ifdef GUI_ENABLE
import lcd;
import touch;
#endif

// cpu运行时
#if defined(FREERTOS_DEBUG) && defined(FREERTOS_ENABLE)
import cpu_runtime;
#endif


export namespace bsp::baseInit
{
    void init();
}

TIM_HandleTypeDef htim7;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;

void bsp::baseInit::init()
{
    HAL_Init();
    rcc::SystemClock_DefaultConfig(); // 系统时钟初始化

    // 开启FreeRTOS的运行时统计信息
#if defined(FREERTOS_DEBUG) && defined(FREERTOS_ENABLE)
    ConfigureTimerForRunTimeStats();
#endif

    //按键和lcd都可能用fsmc，这里不做更改
    fsmc::init();

#ifdef DMA_FSMC_ENABLE
    fsmc_dma_init();// 初始化FSMC+DMA
#endif

#ifdef LCD_SPI_PORT_ENABLE
    spi::init_spi2(); //硬件SPI初始化
#endif

#ifdef GUI_ENABLE
    lcd::init();
    touch::init();
#endif

#ifdef SD_SPI_ENABLE
    spi_sd_init();
#endif

#ifdef FREERTOS_ENABLE
    osKernelInitialize();// FreeRTOS内核初始化
#endif// FREERTOS_ENABLE
}





/*用于配置供HAL使用基础时钟，频率为1KHz*/
export extern "C"
{
    // 用于初始化硬件资源
    void HAL_MspInit()
    {
        __HAL_RCC_SYSCFG_CLK_ENABLE();
        __HAL_RCC_PWR_CLK_ENABLE();

        // 启用基础GPIO时钟
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOH_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
    }

HAL_StatusTypeDef HAL_InitTick(const uint32_t TickPriority)
{
        __HAL_RCC_TIM7_CLK_ENABLE();
        htim7.Instance = TIM7;
        htim7.Init.Prescaler = 20;
        htim7.Init.Period = 3999;
        htim7.Init.ClockDivision = 0;
        htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
        htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

        HAL_StatusTypeDef status = HAL_TIM_Base_Init(&htim7);
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
                }
                else
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
    __HAL_TIM_DISABLE_IT(&htim7, TIM_IT_UPDATE);
}

inline void HAL_ResumeTick(void)
{
    __HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE);
}
}

// TIM7中断处理函数
extern "C"
{
void TIM7_IRQHandler()
{
#if 0
    if (__HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
        HAL_IncTick();
        lv_tick_inc(1);
    }
#endif

    // 我把TIM7当做系统时钟，只用到了基础定时器的更新计时功能，所以并不需要判断中断源
    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);

    HAL_IncTick();

#ifdef GUI_ENABLE
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
