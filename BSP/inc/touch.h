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

uint8_t TOUCH_Init( void );
void TOUCH_Read(uint8_t dir);//��ȡ��������ǰ״̬�����д�����ı�TOUCH_STA,����5��������������TOUCH_X,TOUCH_Y������
static void  ft6336_rest( void );




#ifdef __cplusplus
}
#endif

#endif //FURINA_TOUCH_H