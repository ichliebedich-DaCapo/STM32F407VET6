//
// Created by DV on 2024/11/17.
//
#include "usart.h"



// 头文件
#include "stm32f4xx_hal.h"
#include <stdio.h>

// 变量
UART_HandleTypeDef huart1;


// 函数
void usart1_init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();



    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);

}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{

    HAL_UART_IRQHandler(&huart1);

}


//int fputc(int ch, FILE *f)
//
//{
//    while(1){}
//      HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
////    while( __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)==RESET);
////    huart1.Instance->DR=ch;
//
//  return ch;
//
//}

// gcc专用重定向
//#ifdef __GNUC__
//
//int _write(int fd, char *ptr, int len)
//{
//  HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 0xFFFF);
//    return len;
//}
//
//#endif
