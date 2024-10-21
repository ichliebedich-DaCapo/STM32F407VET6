//
// Created by fairy on 2024/10/8.
//

#ifndef FURINA_W25QXX_H
#define FURINA_W25QXX_H

#include "Module_Conf.h"
#ifdef USE_SPI_FLASH
#include "spi.h"
#ifdef __cplusplus
extern "C" {
#endif

// 声明接口
void w25qxx_init(void);// 初始化Flash,包含了初始化
void w25qxx_page_write(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); // 页写入
void w25qxx_buffer_read(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);  // 读数据流 // flash芯片地址初始化——解锁保护
uint8_t w25qxx_read_byte(uint32_t ReadAddr);// 读一个字节
void w25qxx_write_byte(uint32_t WriteAddr, uint8_t byte);// 写一个字节
void w25qxx_sector_erase(uint32_t SectorAddr);                                         // 擦除扇区
void w25qxx_chip_erase();                                                              // 整片擦除

void w25qxx_async_chip_erase();// 非阻塞式（异步）整片擦除
void w25qxx_async_sector_erase(uint32_t SectorAddr);
uint8_t w25qxx_async_is_busy();// 判断是否正在忙（用于前面擦除）

#ifdef __cplusplus
}
#endif

#endif
#endif //FURINA_W25QXX_H
