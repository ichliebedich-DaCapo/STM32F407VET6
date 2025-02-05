//
// Created by fairy on 2025/1/11 15:03.
//
#ifndef FURINA_RNG_H
#define FURINA_RNG_H
#include <bsp_config.h>
#ifdef __cplusplus
extern "C" {
#endif
#include "stdint.h"
void RNG_Init(void);

uint32_t Get_Random_Number(void);

#ifdef __cplusplus
}
#endif
#endif //FURINA_RNG_H