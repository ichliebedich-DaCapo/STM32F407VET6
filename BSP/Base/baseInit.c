//
// Created by 34753 on 2024/9/22.
//

#include "baseInit.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "lvgl.h"
#include "lcd.h"
#include "fsmc.h"

#if FreeRTOS_DebugMode
#include "CPU_RunTime.h"
#endif

TIM_HandleTypeDef htim7;

static inline void SystemClock_Config();

void BaseInit()
{
    HAL_Init();
    SystemClock_Config();// 系统时钟初始化

    // 开启FreeRTOS的运行时统计信息
#if FreeRTOS_DebugMode
  ConfigureTimerForRunTimeStats();
#endif

    // 启用基础GPIO时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    fsmc_dma_init();// 初始化FSMC+DMA
    lcd_init();// 初始化LCD
}


// 系统时钟初始化
void SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
    */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 6;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

// 用于初始化硬件资源
void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

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
void TIM7_IRQHandler()
{
    if(__HAL_TIM_GET_IT_SOURCE(&htim7, TIM_IT_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
        HAL_IncTick();
        lv_tick_inc(1);
    }

    // 下面的也许不安全
/*    __HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
    HAL_IncTick();
    lv_tick_inc(1);*/
}


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