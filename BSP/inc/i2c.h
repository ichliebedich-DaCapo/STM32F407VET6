//
// Created by DV on 2025/2/4.
//

#ifndef FURINA_I2C_H
#define FURINA_I2C_H
#include "App_Conf.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

void i2c1_Init(); // I2C1��ʼ��
void I2C_Write(uint8_t DevAddress, uint8_t *pData, uint16_t Size); // д���ݵ�I2C�豸
void I2C_Read(uint8_t DevAddress, uint8_t *pData, uint16_t Size);  // ��I2C�豸��ȡ����
HAL_StatusTypeDef I2C_ReadRegister(uint16_t DevAddress, uint16_t RegAddress, uint8_t *pData, uint16_t Size);
HAL_StatusTypeDef I2C_WriteRegister(uint16_t DevAddress, uint16_t RegAddress, uint8_t *pData, uint16_t Size);
#ifdef __cplusplus
}
#endif

#endif //FURINA_I2C_H