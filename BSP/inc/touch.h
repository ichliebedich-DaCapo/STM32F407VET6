//
// Created by DV on 2025/2/5.
// ����������ICΪFT6336U

#ifndef FURINA_TOUCH_H
#define FURINA_TOUCH_H
#include "App_Conf.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************�궨��******************/
#define FT6336_OK                 1
#define FT6336_ERROR              0

#define FT_FALSE                  0
#define FT_TRUE                   1
#define FT_FALSE1                 2
#define FT_FALSE2                 3

#define FT6336_ADDR              0x71

//FT5426 ���ּĴ�������
#define FT_DEVIDE_MODE           0x00         //FT6336ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER        0x02         //����״̬�Ĵ���

#define FT_TP1_REG               0X03         //��һ�����������ݵ�ַ
#define FT_TP2_REG               0X09         //�ڶ������������ݵ�ַ

#define FT_ID_G_CIPHER_MID       0x9F         //оƬ���ţ����ֽڣ� Ĭ��ֵ0x26
#define FT_ID_G_CIPHER_LOW       0xA0         //оƬ���ţ����ֽڣ� 0x01: Ft6336G  0x02: Ft6336U
#define FT_ID_G_LIB_VERSION      0xA1         //�汾
#define FT_ID_G_CIPHER_HIGH      0xA3         //оƬ���ţ����ֽڣ� Ĭ��0x64
#define FT_ID_G_MODE             0xA4         //FT6636�ж�ģʽ���ƼĴ���
#define FT_ID_G_FOCALTECH_ID     0xA8         //VENDOR ID Ĭ��ֵΪ0x11
#define FT_ID_G_THGROUP          0x80         //������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE     0x88         //����״̬�������üĴ���

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