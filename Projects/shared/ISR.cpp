//
// Created by fairy on 2024/10/10.
//
/**
* @brief 中断服务例程
 * @note 用于处理各种中断回调函数，之所以不用stm32f4xx_it.c,是因为本工程是由C/C++混编，
 *      中断服务例程可能会调用到C++文件相关的代码。
 * @不详 这里发生了奇怪的事，有时候在这里定义甚至包括声明中断处理函数，但是链接时会把函数忽略，即使使用了
 *      extern "C"包含。但map文件确实没有任何相关函数符号，即使是-O0优化。编译也看了，会编译这个文件
 *      真是奇怪。难道是没有头文件的原因？但为什么EXTI0中断可以
 * @解决不详 好吧EXTI0也不可以，原因很简单，不能直接把该文件编译成静态库再取链接启动文件(除非该文件有头文件)。
 *      而是和启动文件一样，作为资源文件一起链接其他静态库
*/
#include "App_Conf.h"
#include "stm32f4xx_hal.h"
#include "timer.h"
#include "adc.h"
#include "key.hpp"
#include "usart.h"

#ifndef GUI_DISABLE

#include "GUI.hpp"

#endif

#include "fsmc.h"
#ifdef FreeRTOS_ENABLE
#include "cmsis_os2.h"
extern osSemaphoreId_t keySemHandle;
#endif

#define KEY_RAM (*((volatile unsigned short *)0x6006000C)) // 键盘接口地址
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;





/** TIM中断回调函数
 * @note 关于TIM6已经转移至timer.c里了
 * */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//    if (htim->Instance == TIM6)
//    {
////        timer6_isr();
//    }
}


/*ADC中断回调函数*/
extern void adc1_isr();
__weak void adc1_isr() {}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc1_isr();
    }
}

void HAL_PWR_PVDCallback(void)
{
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_PVDO); // 清除 PVD 标志
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO)) // 检查是否是电压下降
    {
        // 断电时执行的代码
//        Execute_Shutdown_Code();

//        // 保存关键数据到后备寄存器
//        HAL_PWR_EnableBkUpAccess(); // 允许访问后备寄存器
//        __HAL_RCC_BKP_CLK_ENABLE(); // 启用后备寄存器时钟
//        HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR0, 0x12345678); // 示例：保存数据
    }
    else
    {
        // 电压恢复时执行的代码（可选）

    }

}

/*******************************中断服务例程**************************************/

extern "C" {
void EXTI0_IRQHandler()
{
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_0);
    Key::setCode(KEY_RAM & 0xF);//获取键值
#ifdef FreeRTOS_ENABLE
    /*释放信号量*/
    osSemaphoreRelease(keySemHandle);
#else
    Key::setSign();
#endif
}


/**
 * @brief DMA中断
*/
// 主打一个狂野
#define DMA2_S6CR (*((volatile uint32_t *)0x400264A0))
// DMA中断,里面他宝贝的真啰嗦
void DMA2_Stream6_IRQHandler(void)
{
    /*  检查传输完成标志（TCIF）是否被设置，即是否传输完成标志位*/
    if (DMA2->HISR & 0x1 << 21)
    {
        DMA2->HISR &= ~(0x1 << 21);// 清除标志位
        // 检查中断使能标志位
        if (DMA2_S6CR & (DMA_IT_TC))
        {
            DMA2_S6CR &= ~(DMA_IT_TC);// 清除标志位
            /* Change the DMA state */
            hdma_memtomem_dma2_stream6.State = HAL_DMA_STATE_READY;// 不能少,因为Start_IT里需要靠它来开启
            /* Process Unlocked */
            __HAL_UNLOCK(&hdma_memtomem_dma2_stream6);// 不能少
        }
#ifndef GUI_DISABLE
      GUI::display_flush_ready();
#endif
    }
}


}



/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"


/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
    while (1)
    {
        __BKPT();
    }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
}

__attribute__((weak)) void RTC_Alarm_IRQHandler(void) { while (1) { /* 没有找到RTC_Alarm的定义 */ }}

__attribute__((weak)) void HASH_RNG_IRQHandler(void) { while (1) { /* 没有找到HASH_RNG的定义 */ }}

__attribute__((weak)) void EXTI2_IRQHandler(void) { while (1) { /* 没有找到EXTI2的定义 */ }}

__attribute__((weak)) void TIM8_CC_IRQHandler(void) { while (1) { /* 没有找到TIM8_CC的定义 */ }}

__attribute__((weak)) void TIM1_CC_IRQHandler(void) { while (1) { /* 没有找到TIM1_CC的定义 */ }}

__attribute__((weak)) void DMA2_Stream5_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream5的定义 */ }}

__attribute__((weak)) void DMA1_Stream5_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream5的定义 */ }}

__attribute__((weak)) void SysTick_Handler(void) { while (1) { /* 没有找到SysTick的定义 */ }}

__attribute__((weak)) void PVD_IRQHandler(void) { while (1) { /* 没有找到PVD的定义 */ }}

__attribute__((weak)) void SDIO_IRQHandler(void) { while (1) { /* 没有找到SDIO的定义 */ }}

__attribute__((weak)) void TAMP_STAMP_IRQHandler(void) { while (1) { /* 没有找到TAMP_STAMP的定义 */ }}

__attribute__((weak)) void PendSV_Handler(void) { while (1) { /* 没有找到PendSV的定义 */ }}

__attribute__((weak)) void CAN2_RX1_IRQHandler(void) { while (1) { /* 没有找到CAN2_RX1的定义 */ }}

__attribute__((weak)) void EXTI3_IRQHandler(void) { while (1) { /* 没有找到EXTI3的定义 */ }}

__attribute__((weak)) void TIM8_TRG_COM_TIM14_IRQHandler(void) { while (1) { /* 没有找到TIM8_TRG_COM_TIM14的定义 */ }}

__attribute__((weak)) void TIM1_UP_TIM10_IRQHandler(void) { while (1) { /* 没有找到TIM1_UP_TIM10的定义 */ }}

__attribute__((weak)) void I2C3_ER_IRQHandler(void) { while (1) { /* 没有找到I2C3_ER的定义 */ }}

__attribute__((weak)) void EXTI0_IRQHandler(void) { while (1) { /* 没有找到EXTI0的定义 */ }}

__attribute__((weak)) void I2C2_EV_IRQHandler(void) { while (1) { /* 没有找到I2C2_EV的定义 */ }}

__attribute__((weak)) void DMA1_Stream2_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream2的定义 */ }}

__attribute__((weak)) void CAN1_RX0_IRQHandler(void) { while (1) { /* 没有找到CAN1_RX0的定义 */ }}

__attribute__((weak)) void FPU_IRQHandler(void) { while (1) { /* 没有找到FPU的定义 */ }}

__attribute__((weak)) void OTG_HS_WKUP_IRQHandler(void) { while (1) { /* 没有找到OTG_HS_WKUP的定义 */ }}

__attribute__((weak)) void CAN2_SCE_IRQHandler(void) { while (1) { /* 没有找到CAN2_SCE的定义 */ }}

__attribute__((weak)) void DMA2_Stream2_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream2的定义 */ }}

__attribute__((weak)) void SPI1_IRQHandler(void) { while (1) { /* 没有找到SPI1的定义 */ }}

__attribute__((weak)) void TIM1_BRK_TIM9_IRQHandler(void) { while (1) { /* 没有找到TIM1_BRK_TIM9的定义 */ }}

__attribute__((weak)) void DCMI_IRQHandler(void) { while (1) { /* 没有找到DCMI的定义 */ }}

__attribute__((weak)) void CAN2_RX0_IRQHandler(void) { while (1) { /* 没有找到CAN2_RX0的定义 */ }}

__attribute__((weak)) void DMA2_Stream3_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream3的定义 */ }}

__attribute__((weak)) void USART6_IRQHandler(void) { while (1) { /* 没有找到USART6的定义 */ }}

__attribute__((weak)) void USART3_IRQHandler(void) { while (1) { /* 没有找到USART3的定义 */ }}

__attribute__((weak)) void CAN1_RX1_IRQHandler(void) { while (1) { /* 没有找到CAN1_RX1的定义 */ }}

__attribute__((weak)) void UART5_IRQHandler(void) { while (1) { /* 没有找到UART5的定义 */ }}

__attribute__((weak)) void DMA2_Stream0_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream0的定义 */ }}

__attribute__((weak)) void TIM4_IRQHandler(void) { while (1) { /* 没有找到TIM4的定义 */ }}

__attribute__((weak)) void I2C1_EV_IRQHandler(void) { while (1) { /* 没有找到I2C1_EV的定义 */ }}

__attribute__((weak)) void DMA1_Stream6_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream6的定义 */ }}

__attribute__((weak)) void DMA1_Stream1_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream1的定义 */ }}

__attribute__((weak)) void UART4_IRQHandler(void) { while (1) { /* 没有找到UART4的定义 */ }}

__attribute__((weak)) void TIM3_IRQHandler(void) { while (1) { /* 没有找到TIM3的定义 */ }}

__attribute__((weak)) void RCC_IRQHandler(void) { while (1) { /* 没有找到RCC的定义 */ }}

__attribute__((weak)) void TIM8_BRK_TIM12_IRQHandler(void) { while (1) { /* 没有找到TIM8_BRK_TIM12的定义 */ }}

__attribute__((weak)) void Default_Handler(void) { while (1) { /* 没有找到Default_Handler的定义 */ }}

__attribute__((weak)) void EXTI15_10_IRQHandler(void) { while (1) { /* 没有找到EXTI15_10的定义 */ }}

__attribute__((weak)) void ADC_IRQHandler(void) { while (1) { /* 没有找到ADC的定义 */ }}

__attribute__((weak)) void DMA1_Stream7_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream7的定义 */ }}

__attribute__((weak)) void CAN2_TX_IRQHandler(void) { while (1) { /* 没有找到CAN2_TX的定义 */ }}

__attribute__((weak)) void TIM5_IRQHandler(void) { while (1) { /* 没有找到TIM5的定义 */ }}

__attribute__((weak)) void DMA2_Stream7_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream7的定义 */ }}

__attribute__((weak)) void I2C3_EV_IRQHandler(void) { while (1) { /* 没有找到I2C3_EV的定义 */ }}

__attribute__((weak)) void EXTI9_5_IRQHandler(void) { while (1) { /* 没有找到EXTI9_5的定义 */ }}

__attribute__((weak)) void RTC_WKUP_IRQHandler(void) { while (1) { /* 没有找到RTC_WKUP的定义 */ }}

__attribute__((weak)) void ETH_WKUP_IRQHandler(void) { while (1) { /* 没有找到ETH_WKUP的定义 */ }}

__attribute__((weak)) void SPI2_IRQHandler(void) { while (1) { /* 没有找到SPI2的定义 */ }}

__attribute__((weak)) void OTG_HS_EP1_IN_IRQHandler(void) { while (1) { /* 没有找到OTG_HS_EP1_IN的定义 */ }}

__attribute__((weak)) void DMA1_Stream0_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream0的定义 */ }}

__attribute__((weak)) void CAN1_TX_IRQHandler(void) { while (1) { /* 没有找到CAN1_TX的定义 */ }}

__attribute__((weak)) void SVC_Handler(void) { while (1) { /* 没有找到SVC的定义 */ }}

__attribute__((weak)) void EXTI4_IRQHandler(void) { while (1) { /* 没有找到EXTI4的定义 */ }}

__attribute__((weak)) void FSMC_IRQHandler(void) { while (1) { /* 没有找到FSMC的定义 */ }}

__attribute__((weak)) void ETH_IRQHandler(void) { while (1) { /* 没有找到ETH的定义 */ }}

__attribute__((weak)) void OTG_HS_EP1_OUT_IRQHandler(void) { while (1) { /* 没有找到OTG_HS_EP1_OUT的定义 */ }}

__attribute__((weak)) void WWDG_IRQHandler(void) { while (1) { /* 没有找到WWDG的定义 */ }}

__attribute__((weak)) void TIM2_IRQHandler(void) { while (1) { /* 没有找到TIM2的定义 */ }}

__attribute__((weak)) void OTG_FS_WKUP_IRQHandler(void) { while (1) { /* 没有找到OTG_FS_WKUP的定义 */ }}

__attribute__((weak)) void TIM1_TRG_COM_TIM11_IRQHandler(void) { while (1) { /* 没有找到TIM1_TRG_COM_TIM11的定义 */ }}

__attribute__((weak)) void OTG_HS_IRQHandler(void) { while (1) { /* 没有找到OTG_HS的定义 */ }}

__attribute__((weak)) void EXTI1_IRQHandler(void) { while (1) { /* 没有找到EXTI1的定义 */ }}

__attribute__((weak)) void USART2_IRQHandler(void) { while (1) { /* 没有找到USART2的定义 */ }}

__attribute__((weak)) void I2C2_ER_IRQHandler(void) { while (1) { /* 没有找到I2C2_ER的定义 */ }}

__attribute__((weak)) void DMA2_Stream1_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream1的定义 */ }}

__attribute__((weak)) void CAN1_SCE_IRQHandler(void) { while (1) { /* 没有找到CAN1_SCE的定义 */ }}

__attribute__((weak)) void FLASH_IRQHandler(void) { while (1) { /* 没有找到FLASH的定义 */ }}

__attribute__((weak)) void DMA2_Stream4_IRQHandler(void) { while (1) { /* 没有找到DMA2_Stream4的定义 */ }}

__attribute__((weak)) void USART1_IRQHandler(void) { while (1) { /* 没有找到USART1的定义 */ }}

__attribute__((weak)) void OTG_FS_IRQHandler(void) { while (1) { /* 没有找到OTG_FS的定义 */ }}

__attribute__((weak)) void SPI3_IRQHandler(void) { while (1) { /* 没有找到SPI3的定义 */ }}

__attribute__((weak)) void DMA1_Stream4_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream4的定义 */ }}

__attribute__((weak)) void I2C1_ER_IRQHandler(void) { while (1) { /* 没有找到I2C1_ER的定义 */ }}

__attribute__((weak)) void DMA1_Stream3_IRQHandler(void) { while (1) { /* 没有找到DMA1_Stream3的定义 */ }}