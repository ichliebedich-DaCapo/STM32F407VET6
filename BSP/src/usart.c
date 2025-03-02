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
//DMA_HandleTypeDef hdma_usart1_rx;
//DMA_HandleTypeDef hdma_usart1_tx;
//DMA_HandleTypeDef hdma_usart2_rx;
//DMA_HandleTypeDef hdma_usart2_tx;

// 函数
void usart1_init()
{
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
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    __HAL_RCC_USART1_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//
//    /**USART1 GPIO Configuration
//    PA9     ------> USART1_TX
//    PA10     ------> USART1_RX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_9;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_10;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    huart1.Instance = USART1;
//    huart1.Init.BaudRate = 115200;
//    huart1.Init.WordLength = UART_WORDLENGTH_8B;
//    huart1.Init.StopBits = UART_STOPBITS_1;
//    huart1.Init.Parity = UART_PARITY_NONE;
//    huart1.Init.Mode = UART_MODE_TX_RX;
//    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
//    HAL_UART_Init(&huart1);
//
//    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART1_IRQn);
//
//#ifdef DMA_USART_ENABLE
//    hdma_usart1_rx.Instance = DMA2_Stream2;
//    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
//    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
//    HAL_DMA_Init(&hdma_usart1_rx);
//    __HAL_LINKDMA(&huart1,hdmarx,hdma_usart1_rx);
//
//    hdma_usart1_tx.Instance = DMA2_Stream7;
//    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
//    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
//    HAL_DMA_Init(&hdma_usart1_tx);
//    __HAL_LINKDMA(&huart1,hdmatx,hdma_usart1_tx);
//#endif

}
//
//void usart2_init()
//{
//    GPIO_InitTypeDef GPIO_InitStruct = {0};
//    __HAL_RCC_USART2_CLK_ENABLE();
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//
//    /**USART2 GPIO Configuration
//    PA2     ------> USART2_TX
//    PA3     ------> USART2_RX
//    */
//    GPIO_InitStruct.Pin = GPIO_PIN_2;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    GPIO_InitStruct.Pin = GPIO_PIN_3;
//    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//    huart2.Instance = USART2;
//    huart2.Init.BaudRate = 115200;
//    huart2.Init.WordLength = UART_WORDLENGTH_8B;
//    huart2.Init.StopBits = UART_STOPBITS_1;
//    huart2.Init.Parity = UART_PARITY_NONE;
//    huart2.Init.Mode = UART_MODE_TX_RX;
//    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
//    HAL_UART_Init(&huart2);
//
//    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(USART2_IRQn);
//
//#ifdef DMA_USART_ENABLE
//    hdma_usart2_rx.Instance = DMA1_Stream5;
//    hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//    hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_usart2_rx.Init.Mode = DMA_NORMAL;
//    hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
//    HAL_DMA_Init(&hdma_usart2_rx);
//
//    __HAL_LINKDMA(&huart2,hdmarx,hdma_usart2_rx);
//
//    hdma_usart2_tx.Instance = DMA1_Stream6;
//    hdma_usart2_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//    hdma_usart2_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//    hdma_usart2_tx.Init.MemInc = DMA_MINC_ENABLE;
//    hdma_usart2_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//    hdma_usart2_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//    hdma_usart2_tx.Init.Mode = DMA_NORMAL;
//    hdma_usart2_tx.Init.Priority = DMA_PRIORITY_LOW;
//
//    HAL_DMA_Init(&hdma_usart2_tx);
//    __HAL_LINKDMA(&huart2,hdmatx,hdma_usart2_tx);
//#endif
//}
/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{

    HAL_UART_IRQHandler(&huart1);

}
//void USART2_IRQHandler(void)
//{
//
//    HAL_UART_IRQHandler(&huart2);
//
//}
//
//void uart2_printf(char *fmt, ...)
//{
//    uint32_t length;
//    va_list args;
//
//    va_start(args, fmt);
//    length = vsnprintf((char*)uart2_tx_buf,sizeof(uart2_tx_buf)+1, (char*)fmt, args);
//    va_end(args);
//
//    HAL_UART_Transmit(&huart2,uart2_tx_buf,length,0xFFFF);
//
//}
//
////int fputc(int ch, FILE *f)
////
////{
////    while(1){}
////      HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
//////    while( __HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)==RESET);
//////    huart1.Instance->DR=ch;
////
////  return ch;
////
////}
//
//// gcc专用重定向
////#ifdef __GNUC__
////
////int _write(int fd, char *ptr, int len)
////{
////  HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 0xFFFF);
////    return len;
////}
////
////#endif
//
//
//#define USB_Huart huart1			//修改为所用串口
//
//#define UART_RX_BUF_SIZE 516
//#define UART_RXTX_Switch 1			//串口回显开关
//
//#define TempDoubleLen 18
//#define TempIntLen 11
//
//uint8_t RxBuffer[UART_RX_BUF_SIZE] = {0};
//uint8_t TxBuffer[UART_RX_BUF_SIZE] = {0};
//uint8_t sendCompleteSign = 1;
//uint8_t flag,flag_wifi,flag_mqtt,flag_reset;
//uint8_t TxLen = 0;
//int32_t a,b,c;
//double  d,e,f;
//extern uint8_t UART2_RX_Buffer[r3_length];
//extern uint8_t  UART2_temp[REC_LENGTH];
//extern unsigned int  UART2_Rx_cnt ;
//
///* 主数据处理函数
// * 从接收缓冲区解析6个数据字段
// * 包含3个整型、3个浮点型数据 */
//void DataProcess(void)
//{
//    //在这里加入数据处理的函数
//    a = str2int(RxBuffer, ' ', 1);
//    b = str2int(RxBuffer, ' ', 2);
//    c = str2int(RxBuffer, ' ', 3);
//    str2double(RxBuffer, ' ', 4, &d);
//    str2double(RxBuffer, ' ', 5, &e);
//    str2double(RxBuffer, ' ', 6, &f);
//
//}
//
////到USARTx_IRQHandler中添加，如:
////void USART1_IRQHandler(void)
////{
////  /* USER CODE BEGIN USART1_IRQn 0 */
////  if(__HAL_UART_GET_FLAG(&USB_Huart,UART_FLAG_IDLE))
////  {
////	  HAL_UART_IdleCallback(&USB_Huart);
////  }
////
////  /* USER CODE END USART1_IRQn 0 */
////  HAL_UART_IRQHandler(&huartx);
////}
//void HAL_UART_IdleCallback(UART_HandleTypeDef *huart)
//{
//    __HAL_UART_CLEAR_IDLEFLAG(huart);
//    {
//        HAL_UART_DMAStop(huart);
//
//        ProcessData();
//
//        StartUartRxDMA();
//    }
//}
//
//void ProcessData()
//{
//    uint32_t len = 0;
//
//    //得到已经接收了多少个字节 = 总共要接收的字节数 - >NDTR F1为CNDTR F4为NDTR
//
//
//#ifdef  __STM32F4xx_HAL_H
//    len = UART_RX_BUF_SIZE - USB_Huart.hdmarx->Instance->NDTR;
//#define ProcessDataOK
//#endif
//
//#ifndef ProcessDataOK
//    增加所用芯片的版本
//#endif
//
//    if(len > 0)
//    {
//        if(sendCompleteSign == 1)
//        {
//#if UART_RXTX_Switch
//            memset((void *)TxBuffer, 0, sizeof(TxBuffer));//将TxBuffer数组全部赋值为0
//					memcpy(TxBuffer, RxBuffer, len);//将RxBuffer的内容全部复制到TxBuffer
//            TxLen = len;
//            StartUartTxDMA();	//串口回显
//#endif
//            {
//                //在这里面加入数据处理的函数
//                DataProcess();
//            }
//        }
//    }
//}
//
//void USB_DMA_printf(const char *format,...)
//{
//    uint32_t length;
//    va_list args;
//
//    va_start(args, format);
//    length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
//    va_end(args);
//
//    HAL_UART_Transmit_DMA(&USB_Huart,TxBuffer,length);
//}
//
//void USB_printf(const char *format,...)
//{
//    uint32_t length;
//    va_list args;
//
//    va_start(args, format);
//    length = vsnprintf((char*)TxBuffer, sizeof(TxBuffer)+1, (char*)format, args);
//    va_end(args);
//
//    HAL_UART_Transmit(&USB_Huart,TxBuffer,length,0xFFFF);
//}
//
///**
//  * @brief  Tx Transfer completed callbacks.
//  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
//  *                the configuration information for the specified UART module.
//  * @retval None
//  */
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//    /* Prevent unused argument(s) compilation warning */
////  UNUSED(huart);
//    if(huart == &USB_Huart)
//    {
//        sendCompleteSign = 1;
//    }
//
//}
//
///**
//  * @brief  Rx Transfer completed callbacks.
//  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
//  *                the configuration information for the specified UART module.
//  * @retval None
//  */
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//
////  UNUSED(huart;);
//    if(huart == &huart2)
//    {
//        UART2_RX_Buffer[UART2_Rx_cnt]=UART2_temp[0];
//        UART2_Rx_cnt++;
////      USB_printf("%s",UART2_RX_Buffer);
//        HAL_UART_Receive_IT(&huart2, (uint8_t *)UART2_temp, REC_LENGTH);
//
//    }
//    if(huart == &USB_Huart)
//    {
//        ProcessData();
//        StartUartRxDMA();
//
//
//
//    }
//
//}
//
//uint8_t UartTxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len)
//{
//    HAL_StatusTypeDef status;
//    uint8_t ret = 1;
//
//    if(sendCompleteSign == 0 || len == 0)
//    {
//        return 0;
//    }
//
//    sendCompleteSign = 0;
//
//    status = HAL_UART_Transmit_DMA(huart, (uint8_t*)buf, len);
//
//    if(HAL_OK != status)
//    {
//        ret = 0;
//    }
//
//    return ret;
//}
//
////启动DMA发送
//uint8_t StartUartTxDMA()
//{
//    return UartTxData(&USB_Huart, TxBuffer, TxLen);
//}
//
//uint8_t UartRxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len)
//{
//
//    HAL_StatusTypeDef status;
//    uint8_t ret = 1;
//
//    status = HAL_UART_Receive_DMA(huart, (uint8_t*)buf, len);
//    {
//        if(strstr(buf,"OK")!=NULL)
//        {
//            flag=1;
//        }
//        if(strstr(buf,"WIFI GOT IP")!=NULL)
//            flag_wifi=2;
//        if(strstr(buf,"MQTTCONNECTED")!=NULL)
//            flag_mqtt=1;
//        if(strstr(buf,"MQTTDISCONNECTED")!=NULL)
//            flag_reset=1;
//        uart2_printf("%s",buf);
//
//    }
////pulseWide=buf[0]*10+buf[1];
//    if(HAL_OK != status)
//    {
//        ret = 0;
//    }
//    else
//    {
//        /* 开启空闲接收中断 */
//        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
//    }
//
//    return ret;
//}
//
////启动DMA接收
//uint8_t StartUartRxDMA()
//{
//    return UartRxData(&USB_Huart, RxBuffer, UART_RX_BUF_SIZE);
//}
//
///* 字符串转整型工具函数
// * @param str 源字符串
// * @param flag 分隔符
// * @param no 第n个数据项
// * @return 解析后的整型值 */
//int32_t str2int(uint8_t * str, uint8_t flag, uint8_t no)
//{
//    uint8_t No = 1;
//    uint8_t * Str = str;
//    uint8_t NumTemp[TempIntLen];
//    while(No!=no)
//    {
//        if(*Str == flag)
//            No++;
//        Str++;
//    }
//    No = 0;
//    while(*Str != flag && *Str != '\r' && *Str != '\n' && *Str != '\0' && No < (TempIntLen - 1))
//    {
//        NumTemp[No] = *Str;
//        Str++;
//        No++;
//    }
//    NumTemp[No] = '\0';
//    return atoi(NumTemp);
//}
//
//void str2double(uint8_t * str, uint8_t flag, uint8_t no, double * Output)
//{
//    uint8_t No = 1;
//    uint8_t * Str = str;
//    uint8_t NumTemp[TempDoubleLen];
//    uint8_t NumTemp_int[TempDoubleLen];
//    double OutputNum;
//    while(No!=no)
//    {
//        if(*Str == flag)
//            No++;
//        Str++;
//    }
//    No = 0;
//    while(*Str != flag && *Str != '\r' && *Str != '\n' && *Str != '\0' && No < (TempDoubleLen - 1))
//    {
//        NumTemp[No] = *Str;
//        Str++;
//        No++;
//    }
//    NumTemp[No] = '\0';
//    NumTemp[(TempDoubleLen - 1)] = 0;
//    No = 0;
//    while(NumTemp[NumTemp[(TempDoubleLen - 1)]] != '\0' && NumTemp[(TempDoubleLen - 1)] < (TempDoubleLen - 1))
//    {
//        if(NumTemp[NumTemp[(TempDoubleLen - 1)]] == '.')
//        {
//            NumTemp[(TempDoubleLen - 1)]++;
//            NumTemp_int[(TempDoubleLen - 1)] = NumTemp[(TempDoubleLen - 1)];
//        }
//        NumTemp_int[No] = NumTemp[NumTemp[(TempDoubleLen - 1)]];
//        No++;
//        NumTemp[(TempDoubleLen - 1)]++;
//    }
//    NumTemp_int[No]='\0';
//    NumTemp[(TempDoubleLen - 1)] = NumTemp_int[(TempDoubleLen - 1)]++;
//    OutputNum = (double)atoi(NumTemp_int);
//    while(NumTemp[NumTemp[(TempDoubleLen - 1)]] != '\0')
//    {
//        OutputNum /= 10;
//        NumTemp[(TempDoubleLen - 1)] ++;
//    }
//    *Output = OutputNum;
//}
