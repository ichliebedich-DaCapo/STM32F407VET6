//
// Created by DV on 2025/2/5.
// 触摸屏驱动IC为FT6336U
#ifndef FURINA_TOUCH_H
#define FURINA_TOUCH_H

#include <bsp_config.h>

#ifdef __cplusplus
extern "C" {
#endif


uint8_t touch_init(void);

int32_t touch_read_single_point(int32_t *last_x, int32_t *last_y);

void ft6336_rest(void);


uint8_t ft6336_RdReg(uint16_t regAdd, uint8_t *pData, uint16_t Size);

uint8_t ft6336_WeReg(uint16_t regAdd, uint8_t *pData, uint16_t Size);

#ifdef __cplusplus
}
#endif

#endif //FURINA_TOUCH_H