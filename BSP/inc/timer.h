//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_TIMER_H
#define FURINA_TIMER_H

#include "JYZQ_Conf.h"
#include "stm32f4xx_hal.h"
#ifdef USE_TIMER
#ifdef __cplusplus
extern "C" {
#endif
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;

#define timer2_start() HAL_TIM_Base_Start(&htim2)   // 启用定时器2
#define timer2_stop() HAL_TIM_Base_Stop(&htim2) // 停止定时器2
#define timer6_start_it() HAL_TIM_Base_Start_IT(&htim6) // 启用定时器6中断
#define timer6_stop_it() HAL_TIM_Base_Stop_IT(&htim6)   // 停止定时器6中断


void timer2_init(uint32_t arr, uint32_t psc);

void timer6_init(uint32_t arr, uint32_t psc);

/**
 * @brief 设置定时器6的时钟
 * @param psc 预分频
 * @param arr 自动重载值，不能为0
 */
static inline void timer6_set_freq(uint32_t psc,uint32_t arr )
{
    __HAL_TIM_SET_PRESCALER(&htim6, psc);
    // arr不能为0，否则CNT寄存器不变
    __HAL_TIM_SET_AUTORELOAD(&htim6, arr);
}

#ifdef __cplusplus
}
#endif
#endif

#endif //FURINA_TIMER_H
