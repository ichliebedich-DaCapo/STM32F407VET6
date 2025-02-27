//
// Created by 34753 on 2024/10/10.
//

#ifndef FURINA_TIMER_H
#define FURINA_TIMER_H
#include <bsp_config.h>

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

// 设置定时器6的时钟频率
void timer6_set_freq(uint32_t psc,uint32_t arr );

void timer6_set_arr(uint32_t arr);

uint32_t timer6_get_arr();

// 设置定时器2的时钟频率
void timer2_set_freq(uint32_t psc,uint32_t arr );


#ifdef __cplusplus
}
#endif


#endif //FURINA_TIMER_H
