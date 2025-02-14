//
// Created by 34753 on 2024/10/8.
//

#ifndef FURINA_SPI_H
#define FURINA_SPI_H
#include <bsp_config.h>


#ifdef __cplusplus
extern "C" {
#endif



void spi3_init();// 初始化SPI3
void spi2_init();// 初始化SPI2
void spi2_dma_Init();//初始化SPI2的DMA(TX)
void spi2_cs_low();// 拉低SPI2的片选
void spi2_cs_high();// 拉高SPI2的片选
void spi2_sendByte(unsigned char data);// 发送一个字节数据
void spi2_sendByteArray(unsigned char *data, unsigned int len);// 发送一个字节数组


#ifdef __cplusplus
}
#endif

#endif //FURINA_SPI_H
