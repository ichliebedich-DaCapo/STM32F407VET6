//
// Created by DV on 2025/2/4.
//

#ifndef FURINA_I2C_H
#define FURINA_I2C_H
#include <bsp_config.h>
#include "debug.h"


#ifdef __cplusplus
extern "C" {
#endif

void i2c1_Init(); // I2C1初始化
void I2C_Write(uint8_t DevAddress, uint8_t *pData, uint16_t Size); // 写数据到I2C设备
void I2C_Read(uint8_t DevAddress, uint8_t *pData, uint16_t Size);  // 从I2C设备读取数据
HAL_StatusTypeDef I2C_ReadRegister(uint16_t DevAddress, uint16_t RegAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef I2C_WriteRegister(uint16_t DevAddress, uint16_t RegAddress, uint8_t *pData, uint16_t Size);
#ifdef __cplusplus
}
#endif

#endif //FURINA_I2C_H