//
// Created by fairy on 2024/10/17 18:53.
//
#ifndef FURINA_GUI_HPP
#define FURINA_GUI_HPP

#include "GUI_Base.hpp"


#ifdef GUI_ENABLE
#include "lcd.h"
#endif


// 宏定义
#define SIMPLE_FPS 1 // 启用简单FPS，减少一点点FPS显示对实际帧率的影响。不严谨地对于某个简单的测试来说，从30ms减少到了21ms
#define LCD_INTERFACE_TYPE 1 // 0:8080接口 1:SPI接口  lcd.c中的同时修改

/*匿名命名空间，治不了各种函数变量暴露狂*/
namespace
{
    constexpr uint16_t DISP_HOR_RES = 480;
    constexpr uint16_t DISP_VER_RES = 320;
    constexpr uint16_t DISP_BUF_SIZE = 20;
    constexpr uint8_t BYTE_PER_PIXEL = (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565));
}


/**
 * @brief GUI类
 */
class GUI
{
public:

    template<void (*disp_flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                                   const uint16_t *color_p),
            int32_t (*touchpad_read_xy)(int32_t *last_x, int32_t *last_y) = nullptr>
    static auto init() -> void;

    // GUI处理函数
    static inline auto handler() -> void { lv_task_handler(); }

    // 刷新回调,可供DMA回调函数使用
    static inline auto display_flush_ready() -> void { lv_display_flush_ready(disp); }

    // 获取设备
    static inline auto get_display() -> lv_display_t * { return disp; }

    static inline auto get_indev() -> lv_indev_t * { return indev_touchpad; }

private:
    static auto resource_init() -> void;// 初始化界面

    template<void (*flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)>
    static inline auto disp_drv_init() -> void;

    template<int32_t (*touchpad_read_xy)(int32_t *last_x, int32_t *last_y)>
    static inline auto touchpad_init() -> void;

private:
    static inline lv_display_t *disp;
    static inline lv_indev_t *indev_touchpad;
};


template<void (*disp_flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                               const uint16_t *color_p),
        int32_t (*touchpad_read_xy)(int32_t *last_x, int32_t *last_y)>
auto GUI::init() -> void
{

    /********初始化LVGL*******/
    lv_init();

    /******初始化显示设备******/
    if constexpr (disp_flush != nullptr) { disp_drv_init<disp_flush>(); }

    /*****初始化触摸屏******/
    if constexpr (touchpad_read_xy != nullptr) { touchpad_init<touchpad_read_xy>(); }


    /*****初始化GUI组件*****/
    GUI::resource_init();
}

/**
 * @brief 初始化显示驱动
 * @tparam flush 涂色函数，有LCD驱动提供
 * @note 为了让lambda表达式可以不用捕获外部函数，只能使用函数模板。如果使用函数指针来传递就必须要显示捕获
 */
template<void (*flush)(uint16_t, uint16_t, uint16_t, uint16_t, const uint16_t *)>
auto GUI::disp_drv_init() -> void
{
    disp = lv_display_create(DISP_HOR_RES, DISP_VER_RES);

    // 刷新回调
    lv_display_set_flush_cb(disp, [](lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
    {
        flush(area->x1, area->y1, area->x2, area->y2, (const uint16_t *) px_map);

        // 只有定义了DMA中断回调才不需要这个函数，那么这就要求DMA中断启用时需要设置相关宏定义
        display_flush_ready();
    });

    // 缓冲区  双缓冲明显优于单缓冲
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_1[DISP_HOR_RES * DISP_BUF_SIZE * BYTE_PER_PIXEL];
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_2[DISP_HOR_RES * DISP_BUF_SIZE * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}


/**
 * @brief 初始化触摸屏驱动
 * @tparam touchpad_read_xy
 */
template<int32_t (*touchpad_read_xy)(int32_t *, int32_t *)>
auto GUI::touchpad_init() -> void
{
    indev_touchpad = lv_indev_create();
    lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev_touchpad, [](lv_indev_t *indev_drv, lv_indev_data_t *data)
    {
        if (touchpad_read_xy(&(data->point.x), &(data->point.y)))
        {
            data->state = LV_INDEV_STATE_PRESSED;
        }
        else
        {
            data->state = LV_INDEV_STATE_RELEASED;
        }
    });

}

#endif //FURINA_GUI_HPP