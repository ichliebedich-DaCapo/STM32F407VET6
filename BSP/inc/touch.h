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

/**************宏定义******************/
#define FT6336_OK                 1
#define FT6336_ERROR              0

#define FT_FALSE                  0
#define FT_TRUE                   1
#define FT_FALSE1                 2
#define FT_FALSE2                 3

#define FT6336_ADDR              0x71

//FT5426 部分寄存器定义
#define FT_DEVIDE_MODE           0x00         //FT6336模式控制寄存器
#define FT_REG_NUM_FINGER        0x02         //触摸状态寄存器

#define FT_TP1_REG               0X03         //第一个触摸点数据地址
#define FT_TP2_REG               0X09         //第二个触摸点数据地址

#define FT_ID_G_CIPHER_MID       0x9F         //芯片代号（中字节） 默认值0x26
#define FT_ID_G_CIPHER_LOW       0xA0         //芯片代号（低字节） 0x01: Ft6336G  0x02: Ft6336U
#define FT_ID_G_LIB_VERSION      0xA1         //版本
#define FT_ID_G_CIPHER_HIGH      0xA3         //芯片代号（高字节） 默认0x64
#define FT_ID_G_MODE             0xA4         //FT6636中断模式控制寄存器
#define FT_ID_G_FOCALTECH_ID     0xA8         //VENDOR ID 默认值为0x11
#define FT_ID_G_THGROUP          0x80         //触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE     0x88         //激活状态周期设置寄存器

#define PANNEL_ID                0x11
// MATCH VALUE LIST


uint8_t touch_init( void );
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