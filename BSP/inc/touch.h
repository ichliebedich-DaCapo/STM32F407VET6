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
static void  ft6336_rest( void );




#ifdef __cplusplus
}
#endif

#endif //FURINA_TOUCH_H