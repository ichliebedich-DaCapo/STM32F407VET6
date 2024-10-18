/*
* Copyright 2023 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

/*********************
 *      INCLUDES
 *********************/
#include <cstdio>
#include "lvgl.h"
#include "custom.hpp"
#include "spectrum_1.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**
 * Create a demo application
 */
#define ACTIVE_TRACK_CNT    3
#define BAR_CNT             20
#define BAND_CNT            4


static uint32_t time;
static uint32_t track_id;
static lv_timer_t *sec_counter_timer;
static bool playing;
static uint32_t spectrum_i = 0;
static uint32_t spectrum_len;
static lv_obj_t *spectrum_area;
static uint32_t spectrum_i_pause = 0;
static const uint16_t rnd_array[30] = {994, 285, 553, 11, 792, 707, 966, 641, 852, 827, 44, 352, 146, 581, 490, 80, 729,
                                       58, 695, 940, 724, 561, 124, 653, 27, 292, 557, 506, 382, 199};

static void album_gesture_event_cb(lv_event_t *e);

static void spectrum_draw_event_cb(lv_event_t *e);

static void spectrum_update_timer_cb(lv_timer_t *timer);

static void update_spectrum();


static const char *title_list[] = {
        "Waiting for true love",
        "Need a Better Future",
        "Vibrations",
};

static const char *artist_list[] = {
        "The John Smith Band",
        "My True Name",
        "Robotics",
};

static const uint32_t time_list[] = {
        1 * 60 + 14,
        2 * 60 + 26,
        1 * 60 + 54,
};

static void timer_cb(lv_timer_t *t)
{
    time++;
    lv_label_set_text_fmt(guider_ui.screen_label_slider_time, "%d:%02d", time / 60, time % 60);
    lv_slider_set_value(guider_ui.screen_slider_1, time, LV_ANIM_ON);
}

#define FFT_NUM 256

#define SPECTRUM_START_X 50
#define SPECTRUM_START_Y 50
#define SPECTRUM_WIDTH 380
#define SPECTRUM_HEIGHT 170
#define SPECTRUM_NUM 64
#define BAR_WIDTH 2  // 每个频谱条的宽度
static float bar_spacing = (float)((SPECTRUM_WIDTH - SPECTRUM_NUM * BAR_WIDTH) / (SPECTRUM_NUM - 1.0)); // 频谱条之间的间距




lv_timer_t *spectrum_update_timer = NULL;

void custom_init(lv_ui *ui)
{
    LV_IMG_DECLARE(_icn_slider_alpha_37x37);

    sec_counter_timer = lv_timer_create(timer_cb, 1000, NULL);
    lv_timer_pause(sec_counter_timer);
    lv_obj_set_style_bg_img_src(guider_ui.screen_slider_1, &_icn_slider_alpha_37x37, LV_PART_KNOB);

    spectrum_update_timer = lv_timer_create(spectrum_update_timer_cb, 30, ui);
    lv_timer_pause(spectrum_update_timer);

    spectrum_area = lv_obj_create(guider_ui.screen);
    lv_obj_remove_style_all(spectrum_area);
    lv_obj_set_pos(spectrum_area, SPECTRUM_START_X, SPECTRUM_START_Y);
    lv_obj_set_size(spectrum_area, SPECTRUM_WIDTH, SPECTRUM_HEIGHT);
    lv_obj_move_background(spectrum_area);
    lv_obj_add_event_cb(guider_ui.screen, spectrum_draw_event_cb, LV_EVENT_ALL, nullptr);
}


static void spectrum_draw_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_DRAW_POST)
    {
        lv_draw_ctx_t *draw_ctx = lv_event_get_draw_ctx(e);

        // 定义渐变色
        lv_grad_dsc_t grad;
        memset(&grad, 0, sizeof(lv_grad_dsc_t));  // 清零结构体
        grad.stops_count = 2;  // 设置渐变色的数量
        grad.stops[0].color = lv_color_make(239, 221, 121);  // 结束颜色（流萤_金色）
        grad.stops[0].frac = 0;  // 起始位置
        grad.stops[1].color = lv_color_make(133, 238, 223);  // 起始颜色（流萤_浅绿色）
        grad.stops[1].frac = 255;  // 结束位置
        grad.dir = LV_GRAD_DIR_VER;  // 垂直渐变

        const uint32_t start_x = SPECTRUM_START_X;
        const uint32_t start_y = SPECTRUM_START_Y + SPECTRUM_HEIGHT;
        const uint32_t bar_width_plus_spacing = BAR_WIDTH + bar_spacing;

        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.width = BAR_WIDTH;
        line_dsc.opa = LV_OPA_COVER;

        for (uint32_t i = 0; i < SPECTRUM_NUM; i++)
        {
            // 计算当前频谱条的位置
            uint32_t x1 = start_x + i * bar_width_plus_spacing;
            uint32_t y1 = start_y - spectrum[(i << 2)] * SPECTRUM_HEIGHT / 255;  // 从底部开始，但考虑最大高度

            // 计算当前频谱条的高度比例，用于渐变色
            uint8_t height_ratio = (start_y - y1) * 255 / SPECTRUM_HEIGHT;

            // 根据高度比例调整渐变色
            lv_color_t color = lv_color_mix(grad.stops[0].color, grad.stops[1].color, height_ratio);
            line_dsc.color = color;

            // 定义两个点
            lv_point_t point1 = {static_cast<lv_coord_t>(x1), static_cast<lv_coord_t>(y1)};
            lv_point_t point2 = {static_cast<lv_coord_t>(x1), start_y};

            // 绘制竖直线
            lv_draw_line(draw_ctx, &line_dsc, &point1, &point2);
        }
    }
}

void start_spectrum_update()
{
    lv_timer_resume(spectrum_update_timer);
}

void stop_spectrum_update()
{

    lv_timer_pause(spectrum_update_timer);

}

static void spectrum_update_timer_cb(lv_timer_t *timer)
{
    update_spectrum();  // 更新FFT频谱数据
    lv_obj_invalidate(spectrum_area);  // 使频谱区域无效，触发重绘
}

void lv_demo_music_pause()
{
    stop_spectrum_update();
    lv_timer_pause(sec_counter_timer);
}

void lv_demo_music_resume()
{
    start_spectrum_update();
    lv_timer_resume(sec_counter_timer);
    lv_slider_set_range(guider_ui.screen_slider_1, 0, time_list[track_id]);
}

/*******更新FFT*******/
static void update_spectrum()
{
    uint8_t temp = spectrum[0];
    spectrum[FFT_NUM - 1] = temp;
    for (uint32_t i = 0; i < FFT_NUM - 1; ++i)
    {
        temp = spectrum[i + 1];
        spectrum[i] = temp;
    }
}


/**
 * @brief 播放速度显示
 * @param speed 播放速度
 */
void lv_playspeed_print(enum PlaySpeed speed)
{
    switch (speed)
    {
        case LV_PLAYSPEED_ACC:
            lv_obj_clear_flag(guider_ui.screen_label_speed, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(guider_ui.screen_label_speed, "快进×1.75");
            break;
        case LV_PLAYSPEED_SLOW:
            lv_obj_clear_flag(guider_ui.screen_label_speed, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(guider_ui.screen_label_speed, "慢放0.25");
            break;
        case LV_PLAYSPEED_NORMAL:
        default:
            lv_obj_add_flag(guider_ui.screen_label_speed, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}


/**
 * @brief 设置播放速度
 * @param speed 播放速度
 */
void lv_playspeed_set(enum PlaySpeed speed)
{
    switch (speed)
    {
        case LV_PLAYSPEED_ACC:
#warning "设置速度"
            print("acc_1.75\r\n");
            break;
        case LV_PLAYSPEED_SLOW:
            print("slow_0.25\r\n");
            break;
        case LV_PLAYSPEED_NORMAL:
            print("speed_normal\r\n");
        default:
            break;
    }
}



