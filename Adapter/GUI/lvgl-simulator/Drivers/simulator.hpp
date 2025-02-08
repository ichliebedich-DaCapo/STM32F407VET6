//
// Created by fairy on 2025/1/1 15:52.
//
#ifndef LVGL_SIMULATOR_SIMULATOR_HPP
#define LVGL_SIMULATOR_SIMULATOR_HPP
#ifdef __cplusplus
#include <cstdint>
#endif

void simulator_init();
void simulator_quit();
void simulator_event_Handler();
bool simulator_is_running();

/*****************/
#ifdef __cplusplus
extern "C"
{
#endif

    /********************LCD驱动接口***********************/
void LCD_Set_Pixel(uint16_t x, uint16_t y, uint16_t color);

void LCD_Color_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, const uint16_t *color);

void LCD_Color_Clean(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,  uint16_t color);

[[maybe_unused]] void LCD_Clear(uint16_t color);

/**********************触摸屏驱动接口**********************/
int32_t touchpad_read_xy(int32_t *x, int32_t *y);


#ifdef __cplusplus
}
#endif



#endif //LVGL_SIMULATOR_SIMULATOR_HPP