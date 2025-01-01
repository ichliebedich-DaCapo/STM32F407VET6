//
// Created by fairy on 2024/10/17 18:53.
//
#ifndef FURINA_GUI_HPP
#define FURINA_GUI_HPP

#include <cstdio>

/* 预编译命令 */
#if 1
// 头文件
#include "LV_Timer.hpp"
#include "events.hpp"
#include "chart.hpp"
#include "slider.hpp"
#include "image.hpp"
#include "text.hpp"
#include "button.hpp"
#include "imageButton.hpp"
#include "FPS.hpp"
// 宏定义
#define SIMPLE_FPS 1 // 启用简单FPS，减少一点点FPS显示对实际帧率的影响。不严谨地对于某个简单的测试来说，从30ms减少到了21ms

/*匿名命名空间，专治各种函数变量暴露狂*/
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
class GUI : public GUI_Base
{
public:
    template<void (*disp_flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)>
    static auto init() -> void;

    static inline auto handler() -> void;

    // 刷新回调
    static inline auto display_flush_ready() -> void;

#ifdef  GUI_ENABLE
    private:
#else
public:
#endif

    static auto resource_init() -> void;// 初始化界面

    template<void (*flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)>
    static inline auto disp_drv_init() -> void;

private:
    static inline lv_display_t *disp;
};

static inline auto LVGL_LCD_FSMC_DMA_pCallback() -> void
{
    GUI::display_flush_ready();
}

/**
 * @brief GUI事件循环
 */
auto GUI::handler() -> void
{
    lv_task_handler();
}


/**
 * @brief 回调函数
 * @note 用于DMA等刷新完成后通知LVGL
 */
auto GUI::display_flush_ready() -> void
{
    lv_display_flush_ready(disp);
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
        lv_display_flush_ready(disp_drv);
    });

    // 缓冲区  双缓冲明显优于单缓冲
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_1[DISP_HOR_RES * DISP_BUF_SIZE * BYTE_PER_PIXEL];
    LV_ATTRIBUTE_MEM_ALIGN
    static uint8_t buf_2_2[DISP_HOR_RES * DISP_BUF_SIZE * BYTE_PER_PIXEL];
    lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
}


template<void (*disp_flush)(uint16_t, uint16_t, uint16_t, uint16_t, const uint16_t *)>
auto GUI::init() -> void
{
    /********初始化LVGL*******/
    lv_init();

    /******初始化显示设备******/
    disp_drv_init<disp_flush>();

    /*****初始化GUI组件*****/
    GUI::resource_init();
}


#endif
#endif //FURINA_GUI_HPP