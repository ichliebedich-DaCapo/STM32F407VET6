
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_AI_H
#define __APP_AI_H
#ifdef __cplusplus
extern "C" {
#endif
/**
  ******************************************************************************
  * @file    app_x-cube-ai.h
  * @author  X-CUBE-AI C code generator
  * @brief   AI entry function definitions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "ai_platform.h"

void MX_X_CUBE_AI_Init(void);
void MX_X_CUBE_AI_Process(void);
int8_t  process_float_data(float input_data);
int8_t  process_float_data2(int8_t input_data);
#ifdef __cplusplus
}
#endif
#endif /*__STMicroelectronics_X-CUBE-AI_9_1_0_H */