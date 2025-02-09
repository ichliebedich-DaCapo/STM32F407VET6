//
// Created by fairy on 2025/1/9 13:31.
//
#ifndef FURINA_APP_HPP
#define FURINA_APP_HPP

// 不使用GUI


#include "touch.h"
#include <stdint.h>
#define CTP_MAX_TOUCH               2

typedef struct
{
    uint8_t ptNum                    : 4;
    uint8_t tpDown                   : 1;
    uint8_t tpPress                  : 1;
    uint8_t res                      : 2;
} Status_bit;

typedef struct
{
    uint16_t xpox[CTP_MAX_TOUCH];
    uint16_t ypox[CTP_MAX_TOUCH];
    union
    {
        uint8_t status;
        Status_bit status_bit;
    };
}stru_pos;

void usr_touchInit();
uint8_t usr_ScanTouchProcess( stru_pos *pPos);
#endif //FURINA_APP_HPP