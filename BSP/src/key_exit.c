//
// Created by fairy on 2024/10/7.
//

#include "key_exit.h"

#include "stm32f4xx_hal.h"

/*外部中断初始化*/
void key_exti_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStructure.Mode = MODE_INPUT;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;//调低可能是为了防抖
    GPIO_InitStructure.Pin = GPIO_PIN_0;

    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_NVIC_SetPriority(EXTI0_IRQn, 0xF, 0x00);// 设为最低，以便FreeRTOS能够调控
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);


//#if EXTI1_OPEN
//    GPIO_InitStructure.Pin = GPIO_PIN_1;
//    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
//    HAL_NVIC_SetPriority(EXTI1_IRQn, 0x05, 0x00);
//    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
//#endif
}
