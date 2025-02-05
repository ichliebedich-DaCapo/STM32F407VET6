//
// Created by DV on 2025/2/4.
//

#ifndef FURINA_I2C_H
#define FURINA_I2C_H
#include "App_Conf.h"
#include "stm32f4xx_hal.h"



#ifdef __cplusplus
extern "C" {
#endif

void i2c1_Init(); // I2C1��ʼ��
void I2C_Write(unsigned char DevAddress, unsigned char *pData, unsigned int Size); // д���ݵ�I2C�豸
void I2C_Read(unsigned char DevAddress, unsigned char *pData, unsigned int Size);  // ��I2C�豸��ȡ����
HAL_StatusTypeDef I2C_ReadRegister(unsigned char DevAddress, unsigned char RegAddress, unsigned char *pData, unsigned int Size);
HAL_StatusTypeDef I2C_WriteRegister(unsigned char DevAddress, unsigned char RegAddress, unsigned char *pData, unsigned int Size);
#ifdef __cplusplus
}
#endif

#endif //FURINA_I2C_H