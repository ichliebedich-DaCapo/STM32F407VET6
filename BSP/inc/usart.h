//
// Created by DV on 2024/11/17.
//

#ifndef FURINA_USART_H
#define FURINA_USART_H
#include <bsp_config.h>
#ifdef __cplusplus
extern "C" {
#endif

//#define REC_LENGTH 1
//#define r3_length 256

//static uint8_t uart2_tx_buf[256];
void usart1_init() ;
void usart2_init() ;
void uart2_printf(char *fmt, ...);



///*
// *  辅助函数
// */
//void USB_DMA_printf(const char *format,...);			//printf DMA方式
//void USB_printf(const char *format,...);				//printf 普通方式
//uint8_t UartTxData(UART_HandleTypeDef *huart, uint8_t *buf, const uint32_t len);
//uint8_t StartUartRxDMA();								//接收DMA初始化
//uint8_t StartUartTxDMA();								//不需要自己调用
//void ProcessData();										//在里面添加数据处理函数
//void HAL_UART_IdleCallback(UART_HandleTypeDef *huart);	//到USARTx_IRQHandler中添加
//uint8_t Uartjudge(uint8_t *RxBuffer,uint8_t number,uint8_t Judgenumber);
//
///*
//str:数字字符串首地址
//flag:分隔符
//no:第no个数字 从1开始计
//Output: 小数存放地址
// */
//extern int32_t str2int(uint8_t * str, uint8_t flag, uint8_t no);
//extern void str2double(uint8_t * str, uint8_t flag, uint8_t no, double * Output);


#ifdef __cplusplus
}
#endif




#endif //FURINA_USART_H