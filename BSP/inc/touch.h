//
// Created by DV on 2025/2/5.
// 触摸屏驱动IC为FT6336U

#ifndef FURINA_TOUCH_H
#define FURINA_TOUCH_H
#include "App_Conf.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t TOUCH_Init( void );

void TOUCH_Read(uint8_t dir);//读取触摸屏当前状态啊，有触发则改变TOUCH_STA,读出5个触发点坐标在TOUCH_X,TOUCH_Y数组中

static void touch_delay_us(uint32_t us);
static void  ft6336_rest( void );
int ft6336_rd_register(uint16_t reg, uint8_t *buff, uint8_t len);
int ft6336_rd_sigleReg(uint16_t reg, uint8_t *val );

uint8_t ft6336_RdReg( uint16_t regAdd, uint8_t *pData, uint16_t Size );
uint8_t ft6336_WeReg( uint16_t regAdd, uint8_t *pData, uint16_t Size );

#ifdef __cplusplus
}
#endif

#endif //FURINA_TOUCH_H