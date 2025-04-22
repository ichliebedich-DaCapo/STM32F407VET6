//
// Created by DV on 2024/11/17.
//
#include "usart.h"



// 头文件
#include "stm32f4xx_hal.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// 变量
/* 全局句柄声明 */
UART_HandleTypeDef huart1;
//UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
//DMA_HandleTypeDef hdma_usart2_rx;
//DMA_HandleTypeDef hdma_usart2_tx;

uint8_t UartIntRxbuf[500]={};
uint16_t UartRxIndex=0;
uint8_t UartRxData;

// 函数
void usart1_init()
{
    __HAL_RCC_DMA2_CLK_ENABLE();
    HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);
    /* DMA2_Stream7_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);

////    位移传感器的配置，以防万一先留着
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

#ifdef DMA_USART_ENABLE

    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    HAL_DMA_Init(&hdma_usart1_rx);
    __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);

    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    HAL_DMA_Init(&hdma_usart1_tx);
    __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);

#endif


    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
//    huart1.Init.BaudRate = 9600;
//    huart1.Init.BaudRate = 74880;
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
void DMA2_Stream7_IRQHandler(void)
{

    HAL_DMA_IRQHandler(&hdma_usart1_tx);

}
void DMA2_Stream2_IRQHandler(void)
{

    HAL_DMA_IRQHandler(&hdma_usart1_rx);

}


// gcc专用重定向
#ifdef __GNUC__

int _write(int fd, char *ptr, int len)
{
  HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 0xFFFF);
    return len;
}
#else
int fputc(int ch, FILE *f)

{
    while(1){}
      HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//    while( __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)==RESET);
//    huart1.Instance->DR=ch;

  return ch;

}

#endif


