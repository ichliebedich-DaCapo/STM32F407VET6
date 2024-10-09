//
// Created by 34753 on 2024/10/8.
//

#ifndef FURINA_W25QXX_H
#define FURINA_W25QXX_H

#include "JYZQ_Conf.h"
#if USE_SPI_FLASH
#include "spi.h"
#ifdef __cplusplus
extern "C" {
#endif

// 声明接口
void w25qxx_init(void);// 初始化Flash
void SPI_FLASH_SectorErase(uint32_t SectorAddr);                                         // 擦除扇区
void SPI_FLASH_ChipErase();                                                              // 整片擦除
void SPI_FLASH_PageWrite(uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite); // 页写入
void SPI_FLASH_BufferRead(uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);  // 读数据流
void SPI_FLASH_Init();                                                                   // flash芯片地址初始化——解锁保护
uint8_t SPI_FLASH_ReadByte(uint32_t ReadAddr);
void SPI_FLASH_WriteByte(uint32_t WriteAddr, uint8_t byte);


#ifdef __cplusplus
}
#endif

#endif
#endif //FURINA_W25QXX_H
