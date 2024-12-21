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
        __BKPT(0);
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