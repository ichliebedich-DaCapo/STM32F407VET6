#ifndef FURINA_BLUE_TOOTH_H
#define FURINA_BLUE_TOOTH_H
#include <bsp_config.h>
#ifdef __cplusplus
extern "C" {
#endif

#include <stm32f4xx_hal.h>

#define HD05_EN_Pin                   GPIO_PIN_12    //复位管脚
#define HD05_EN_Pin_Port              GPIOA    //复位

void HC05_Init(void);
void HC05_connect(void);
void send_bluetooth_data(float thd,float freq,float sampling_rate,float gain, int32_t* ADC_Data_lvgl,float* normalized);

#ifdef __cplusplus
}
#endif
#endif //FURINA_BLUE_TOOTH_H
