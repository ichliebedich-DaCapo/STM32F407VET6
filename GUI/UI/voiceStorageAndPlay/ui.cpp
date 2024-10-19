//
// Created by fairy on 2024/10/18 18:34.
//
#include <iostream>
#include "JYZQ_Conf.h"
#include "component.hpp"
#include "events.hpp"
#include "custom.hpp"
#include "ui.hpp"

#include "spectrum_1.h"

#if 1
// 头文件



static uint32_t time;
static uint32_t track_id;
static lv_timer_t *sec_counter_timer;
static lv_obj_t *spectrum_area;

static void spectrum_draw_event_cb(lv_event_t *e);

static void spectrum_update_timer_cb(lv_timer_t *timer);

static void update_spectrum();

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
static float bar_spacing = (float) ((SPECTRUM_WIDTH - SPECTRUM_NUM * BAR_WIDTH) / (SPECTRUM_NUM - 1.0)); // 频谱条之间的间距




lv_timer_t *spectrum_update_timer = NULL;

/********************************界面初始化******************************/

auto Screen::init() -> void
{
    gui->main.screen = lv_obj_create(nullptr);
    lv_obj_set_size(gui->main.screen, 480, 320);


//    ComponentInitializer::set_parent(guider_ui.screen);
    /******************************************文本框***************************************/
    TextInitializer text{};

    //Write codes screen_label_speed
    text.init(gui->main.screen, gui->main.label_speed, "快进×1.75");
    text.set_pos_size(392, 26, 74, 13);
    text.add_flag(LV_OBJ_FLAG_HIDDEN);
    text.set_text_font_space(&lv_font_SourceHanSerifSC_Regular_12, 2);

    //Write codes screen_label_title_music
    text.init(gui->main.screen, gui->main.label_title_music, "Waiting for true love");
    text.set_pos_size(150, 5, 180, 21);
    text.set_text_font_space(&lv_font_arial_14);
    text.set_text_color(lv_color_hex(0x504d6d));
    text.set_text_align(LV_TEXT_ALIGN_CENTER);// 设置文本居中

    //Write codes screen_label_slider_time
    text.init(gui->main.screen, gui->main.label_slider_time, "0:00");
    text.set_pos_size(420, 288, 36, 12);
    text.set_text_font_space(&lv_font_arial_12);
    text.set_text_color(lv_color_hex(0x8a86b8));
    text.set_text_align(LV_TEXT_ALIGN_CENTER);

    /******************************************滑条***************************************/
    //Write codes screen_slider_1
    SliderInitializer slider{};
    slider.init(gui->main.screen, gui->main.slider);
    slider.set_range(0, 100);
    slider.set_pos_size(42, 294, 342, 1);
    slider.set_bg_color(lv_color_hex(0x2195f6), 100);


    /*************************************图片*************************************/
    ImageInitializer img{};

    //Write codes screen_img_11
    img.init(gui->main.screen, gui->main.img_11);
    img.set_src(&_icn_slider_alpha_15x15);
    img.set_pos_size(235, 152, 15, 15);
    img.add_flag(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_HIDDEN);
    img.set_style_clip_corner(LV_PART_MAIN | LV_STATE_DEFAULT);// 设置为圆角


    /************************************图片按钮************************************/
    ImageButtonInitializer imgbtn{};

    //Write codes screen_imgbtn_play
    imgbtn.init(gui->main.screen, gui->main.imgbtn_play);
    imgbtn.set_pos_size(216, 234, 48, 48);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
    imgbtn.set_src(LV_IMGBTN_STATE_RELEASED, &_btn_play_alpha_48x48);// 正常状态.播放
    imgbtn.set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_btn_pause_alpha_48x48);// 按下按钮.暂停

    //Write codes screen_imgbtn_acc
    imgbtn.init(gui->main.screen, gui->main.imgbtn_acc);
    imgbtn.set_pos_size(318, 240, 37, 37);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
    imgbtn.set_src(LV_IMGBTN_STATE_RELEASED, &_btn_next_alpha_37x37);
    imgbtn.set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_icn_slider_alpha_37x37);

    //Write codes screen_imgbtn_slow
    imgbtn.init(gui->main.screen, gui->main.imgbtn_slow);
    imgbtn.set_pos_size(122, 240, 37, 37);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
    imgbtn.set_src(LV_IMGBTN_STATE_RELEASED, &_btn_prev_alpha_37x37);
    imgbtn.set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_icn_slider_alpha_37x37);

    /***自定义组件***/
    LV_IMG_DECLARE(_icn_slider_alpha_37x37);

    sec_counter_timer = lv_timer_create(timer_cb, 1000, nullptr);
    lv_timer_pause(sec_counter_timer);
    lv_obj_set_style_bg_img_src(gui->main.slider, &_icn_slider_alpha_37x37, LV_PART_KNOB);

    spectrum_update_timer = lv_timer_create(spectrum_update_timer_cb, 30, gui);
    lv_timer_pause(spectrum_update_timer);

    spectrum_area = lv_obj_create(gui->main.screen);
    lv_obj_remove_style_all(spectrum_area);
    lv_obj_set_pos(spectrum_area, SPECTRUM_START_X, SPECTRUM_START_Y);
    lv_obj_set_size(spectrum_area, SPECTRUM_WIDTH, SPECTRUM_HEIGHT);
    lv_obj_move_background(spectrum_area);

}

/******************************************事件实现*************************************************/

class Event_Handler
{
public:
    static auto playspeed_print(enum PlaySpeed speed)->void ;
};


// 快进处理
static void acc_event(bool is_checked)
{
//    lv_playspeed_print(is_checked ? LV_PLAYSPEED_ACC : LV_PLAYSPEED_NORMAL);// 显示播放速度信息
    lv_playspeed_set(is_checked ? LV_PLAYSPEED_ACC : LV_PLAYSPEED_NORMAL);// 快进
}

// 播放暂停处理
static void play_event(bool is_checked)
{
    if (is_checked)
    {
        lv_demo_music_resume();
    } else
    {
        lv_demo_music_pause();
    }
}


auto Events::init() -> void
{
    Events::bond(gui->main.imgbtn_acc, [](event e) { Events::handler(e, acc_event); });// 快进按钮
    Events::bond(gui->main.imgbtn_play, [](event e) { Events::handler(e, play_event); });// 播放暂停按钮
    Events::bond(gui->main.screen, spectrum_draw_event_cb);
}

/*****************************自定义接口*********************************/
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
//    lv_slider_set_range(guider_ui.screen_slider_1, 0, time_list[track_id]);
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
auto Event_Handler::playspeed_print(enum PlaySpeed speed) -> void
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
            std::cout << "acc_1.75\r\n";
            break;
        case LV_PLAYSPEED_SLOW:
            std::cout << "slow_0.25\r\n";
            break;
        case LV_PLAYSPEED_NORMAL:
            std::cout << "speed_normal\r\n";
        default:
            break;
    }
}


#endif