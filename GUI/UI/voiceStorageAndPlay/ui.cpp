//
// Created by fairy on 2024/10/18 18:34.
//
#include "cstdio"
#include "component.hpp"
#include "events.hpp"
#include "spectrum_1.h"
#include "LV_Timer.hpp"
#include "text.hpp"
#include "imageButton.hpp"
#include "image.hpp"
#include "slider.hpp"


#if 1
/********变量声明********/
enum PlaySpeed
{
    LV_PLAYSPEED_NORMAL,// 正常播放
    LV_PLAYSPEED_ACC,// 快进1.75
    LV_PLAYSPEED_SLOW,//慢放0.25
};
static uint32_t time=0;

static constexpr uint16_t fft_num = 256;
static constexpr uint16_t spectrum_start_x = 50;
static constexpr uint16_t spectrum_start_y = 50;
static constexpr uint16_t spectrum_width = 380;
static constexpr uint16_t spectrum_height = 170;
static constexpr uint16_t spectrum_num = 64;
static constexpr uint16_t bar_width = 2; // 每个频谱条的宽度
static constexpr float bar_spacing = (float) ((spectrum_width - spectrum_num * bar_width) /
                                              (spectrum_num - 1.0)); // 频谱条之间的间距


// 计时器
LV_Timer slider_timer;
LV_Timer spectrum_timer;


/********************************界面初始化******************************/

auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);
    /******************************************文本框***************************************/
    Text label;
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_12);

    //Write codes screen_label_speed
    label.init(gui->main.label_speed,392, 26,74,13);
    Text::add_flag(LV_OBJ_FLAG_HIDDEN);
    Text::set_space(2);

    //Write codes screen_label_slider_time
    label.init(gui->main.label_slider_time, 420, 240, 40, 50,"0:00",lv_color_hex(0x8a86b8));

    //Write codes screen_label_title_music
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_14);
    label.init(gui->main.label_title_music,150, 10, 180, 21,"语音存储与回放",lv_color_hex(0x504d6d));
    Text::set_text_align(LV_TEXT_ALIGN_CENTER);// 文本居中

    /*************************************图片*************************************/
    //Write codes screen_img_slider_flag
    Image::init(gui->main.img_slider_flag, 235, 152, 15, 15,&_icn_slider_alpha_15x15);

    /************************************图片按钮************************************/
    //Write codes screen_imgbtn_play
    ImageButton::init(gui->main.imgbtn_play,216, 234, 48, 48,&_btn_list_play_alpha_48x48, &_btn_list_pause_alpha_48x48);

    //Write codes screen_imgbtn_acc
    ImageButton::init(gui->main.imgbtn_acc,318, 240, 37, 37,&_btn_next_alpha_37x37);

    //Write codes screen_imgbtn_slow
    ImageButton::init(gui->main.imgbtn_slow, 122, 240, 37, 37, &_btn_prev_alpha_37x37);

    /******************************************滑条***************************************/
    //Write codes screen_slider
    Slider::init(gui->main.slider,42, 294, 342, 1,0,100,lv_color_hex(0x2195f6), 100,&_icn_slider_alpha_15x15);

    /***自定义组件***/
    Component::init(gui->main.spectrum);
    Component::set_pos_size(spectrum_start_x - 4, spectrum_start_y, spectrum_width + 6, spectrum_height + 1);
}

/******************************************事件实现*************************************************/
// 播放组件
class Play : public GUI_Base
{
public:
    static auto resume() -> void;// 恢复播放
    static auto pause() -> void;// 暂停播放
    static auto print_speed(enum PlaySpeed speed) -> void;// 显示播放速度信息
    static auto set_speed(enum PlaySpeed speed) -> void;// 设置播放速度
};

// 频谱组件
class Spectrum : public GUI_Base
{
public:
    static auto draw(lv_event_t *e) -> void;// 重绘频谱
    static auto update() -> void;// 更新频谱数据
};

// 事件定义
class Event : public GUI_Base
{
public: // 通用事件
    static auto acc(bool is_checked) -> void;// 快进按钮
    static auto slow(bool is_checked) -> void;

    static auto play(bool is_checked) -> void;// 播放暂停按钮
public:// 自定义事件
    static inline auto spectrum_draw(lv_event_t *e) -> void { Spectrum::draw(e); }
};


auto Events::init() -> void
{
    Events::bond(gui->main.imgbtn_acc, imgbtn_fun(Event::acc));// 快进按钮
    Events::bond(gui->main.imgbtn_slow, imgbtn_fun(Event::slow));// 慢放按钮
    Events::bond(gui->main.imgbtn_play, imgbtn_fun(Event::play));// 播放暂停按钮
    Events::bond(gui->main.screen, Event::spectrum_draw);

    // 进度条
    slider_timer.create(timer_fun(
                                time++;
                                char buf[8];
                                sprintf(buf, "%d:%02d", time / 60, time % 60);
                                Text::set_text(buf,GUI_Base::get_ui()->main.label_slider_time);
                                Slider::set_value(time, LV_ANIM_ON, GUI_Base::get_ui()->main.slider);
                        ), 1000);

    // 定时更新频谱数据
    spectrum_timer.create(timer_fun(
                          // 更新FFT频谱数据
                                  uint8_t temp = spectrum[0];
                                  spectrum[fft_num - 1] = temp;
                                  for (uint32_t i = 0; i < fft_num - 1; ++i)
                                  {
                                      temp = spectrum[i + 1];
                                      spectrum[i] = temp;
                                  }
                                  Component::invalidate(GUI_Base::get_ui()->main.spectrum);// 使频谱区域无效，触发重绘
                          ), 30);
}


///*****************************实现接口*********************************/
auto Event::acc(bool is_checked) -> void
{
    Play::print_speed(is_checked ? LV_PLAYSPEED_ACC : LV_PLAYSPEED_NORMAL);// 显示播放速度信息
    Play::set_speed(is_checked ? LV_PLAYSPEED_ACC : LV_PLAYSPEED_NORMAL);// 快进
}

auto Event::play(bool is_checked) -> void
{
    if (is_checked)
    {
        Play::resume();
    } else
    {
        Play::pause();
    }
}

auto Event::slow(bool is_checked) -> void
{
    Play::print_speed(is_checked ? LV_PLAYSPEED_SLOW : LV_PLAYSPEED_NORMAL);// 显示播放速度信息
    Play::set_speed(is_checked ? LV_PLAYSPEED_SLOW : LV_PLAYSPEED_NORMAL);// 快进
}

auto Spectrum::draw(lv_event_t *e) -> void
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

        const uint32_t start_x = spectrum_start_x;
        const uint32_t start_y = spectrum_start_y + spectrum_height;
        const uint32_t bar_width_plus_spacing = bar_width + bar_spacing;

        lv_draw_line_dsc_t line_dsc;
        lv_draw_line_dsc_init(&line_dsc);
        line_dsc.width = bar_width;
        line_dsc.opa = LV_OPA_COVER;

        for (uint32_t i = 0; i < spectrum_num; i++)
        {
            // 计算当前频谱条的位置
            uint32_t x1 = start_x + i * bar_width_plus_spacing;
            uint32_t y1 = start_y - spectrum[(i << 2)] * spectrum_height / 255;  // 从底部开始，但考虑最大高度

            // 计算当前频谱条的高度比例，用于渐变色
            uint8_t height_ratio = (start_y - y1) * 255 / spectrum_height;

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

auto Spectrum::update() -> void
{
    // 更新FFT频谱数据
    uint8_t temp = spectrum[0];
    spectrum[fft_num - 1] = temp;
    for (uint32_t i = 0; i < fft_num - 1; ++i)
    {
        temp = spectrum[i + 1];
        spectrum[i] = temp;
    }
    Component::invalidate(gui->main.spectrum); // 使频谱区域无效，触发重绘
}


/**
 * @brief 播放速度显示
 * @param speed 播放速度
 */
auto Play::print_speed(enum PlaySpeed speed) -> void
{
    switch (speed)
    {
        case LV_PLAYSPEED_ACC:
            lv_obj_clear_flag(gui->main.label_speed, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(gui->main.label_speed, "快进×1.75");
            break;
        case LV_PLAYSPEED_SLOW:
            lv_obj_clear_flag(gui->main.label_speed, LV_OBJ_FLAG_HIDDEN);
            lv_label_set_text(gui->main.label_speed, "慢放×0.25");
            break;
        case LV_PLAYSPEED_NORMAL:
        default:
            lv_obj_add_flag(gui->main.label_speed, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}


/**
 * @brief 设置播放速度
 * @param speed 播放速度
 */
auto Play::set_speed(enum PlaySpeed speed) -> void
{
    switch (speed)
    {
        case LV_PLAYSPEED_ACC:
#warning "设置速度"

            break;
        case LV_PLAYSPEED_SLOW:

            break;
        case LV_PLAYSPEED_NORMAL:

        default:
            break;
    }
}

auto Play::resume() -> void
{
    slider_timer.resume();
    spectrum_timer.resume();
    lv_slider_set_range(gui->main.slider, 0, 255);
}

auto Play::pause() -> void
{
    slider_timer.pause();
    spectrum_timer.pause();
}


#endif