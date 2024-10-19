//
// Created by fairy on 2024/10/18 18:34.
//
#include "component.hpp"
#include "events.hpp"
#include "spectrum_1.h"


#if 1
/********变量声明********/
enum PlaySpeed
{
    LV_PLAYSPEED_NORMAL,// 正常播放
    LV_PLAYSPEED_ACC,// 快进1.75
    LV_PLAYSPEED_SLOW,//慢放0.25
};
// 头文件

static uint32_t time;
static uint32_t track_id;


static const uint32_t time_list[] = {
        1 * 60 + 14,
        2 * 60 + 26,
        1 * 60 + 54,
};


#define FFT_NUM 256
#define SPECTRUM_START_X 50
#define SPECTRUM_START_Y 50
#define SPECTRUM_WIDTH 380
#define SPECTRUM_HEIGHT 170
#define SPECTRUM_NUM 64
#define BAR_WIDTH 2  // 每个频谱条的宽度
static float bar_spacing = (float) ((SPECTRUM_WIDTH - SPECTRUM_NUM * BAR_WIDTH) / (SPECTRUM_NUM - 1.0)); // 频谱条之间的间距

static auto slider_timer_cb(lv_timer_t *timer) -> void;

static auto spectrum_timer_cb(lv_timer_t *timer) -> void;

// 计时器
LV_Timer slider_timer;
LV_Timer spectrum_timer;


/********************************界面初始化******************************/

auto Screen::init() -> void
{

    Component::set_parent(gui->main.screen);
    /******************************************文本框***************************************/

    //Write codes screen_label_speed
    Text::init(gui->main.label_speed, "");
    Text::set_pos_size(392, 26, 74, 13);
    Text::add_flag(LV_OBJ_FLAG_HIDDEN);
    Text::set_text_font_space(&lv_customer_font_SourceHanSerifSC_Regular_12, 2);

    //Write codes screen_label_title_music
    Text::init(gui->main.label_title_music, "语音存储与回放");
    Text::set_pos_size(150, 5, 180, 21);
    Text::set_text_font_space(&lv_customer_font_SourceHanSerifSC_Regular_14);
    Text::set_text_color(lv_color_hex(0x504d6d));
    Text::set_text_align(LV_TEXT_ALIGN_CENTER);// 设置文本居中

    //Write codes screen_label_slider_time
    Text::init(gui->main.label_slider_time, "0:00");
    Text::set_pos_size(420, 288, 36, 12);
    Text::set_text_font_space(&lv_customer_font_SourceHanSerifSC_Regular_12);
    Text::set_text_color(lv_color_hex(0x8a86b8));
    Text::set_text_align(LV_TEXT_ALIGN_CENTER);


    /*************************************图片*************************************/

    //Write codes screen_img_11
    Image::init(gui->main.img_11);
    Image::set_src(&_icn_slider_alpha_15x15);
    Image::set_pos_size(235, 152, 15, 15);
    Image::add_flag(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_HIDDEN);

    /************************************图片按钮************************************/


    //Write codes screen_imgbtn_play
    ImageButton::init(gui->main.imgbtn_play);
    ImageButton::set_pos_size(216, 234, 48, 48);
    ImageButton::add_flag(LV_OBJ_FLAG_CHECKABLE);
    ImageButton::set_src(LV_IMGBTN_STATE_RELEASED, &_btn_list_play_alpha_48x48);// 正常状态.播放
    ImageButton::set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_btn_list_pause_alpha_48x48);// 按下按钮.暂停

    //Write codes screen_imgbtn_acc
    ImageButton::init(gui->main.imgbtn_acc);
    ImageButton::set_pos_size(318, 240, 37, 37);
    ImageButton::add_flag(LV_OBJ_FLAG_CHECKABLE);
    ImageButton::set_src(LV_IMGBTN_STATE_RELEASED, &_btn_next_alpha_37x37);

    //Write codes screen_imgbtn_slow
    ImageButton::init(gui->main.imgbtn_slow);
    ImageButton::set_pos_size(122, 240, 37, 37);
    ImageButton::add_flag(LV_OBJ_FLAG_CHECKABLE);
    ImageButton::set_src(LV_IMGBTN_STATE_RELEASED, &_btn_prev_alpha_37x37);

    /******************************************滑条***************************************/
    //Write codes screen_slider_1
    Slider::init(gui->main.slider);
    Slider::set_range(0, 100);
    Slider::set_pos_size(42, 294, 342, 1);
    Slider::set_bg_color(lv_color_hex(0x2195f6), 100);
    Slider::set_bg_src(gui->main.slider, &_icn_slider_alpha_15x15);

    /***自定义组件***/
    slider_timer.create(slider_timer_cb, 1000);
    spectrum_timer.create(spectrum_timer_cb, 30);

    Component::init(gui->main.spectrum);
    Component::remove_all_style();
    Component::set_pos_size(SPECTRUM_START_X, SPECTRUM_START_Y, SPECTRUM_WIDTH, SPECTRUM_HEIGHT);
    Component::move_to_background();
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
    static auto draw(lv_event_t *e) -> void;

    static auto update() -> void;
};


//// 事件定义
class Event : public GUI_Base
{

public: // 通用事件
    static auto acc(bool is_checked) -> void;

    static auto play(bool is_checked) -> void;

public:// 自定义事件
    static inline auto spectrum_draw(lv_event_t *e) -> void { Spectrum::draw(e); }
};


auto Events::init() -> void
{
    Events::bond(gui->main.imgbtn_acc, [](event e) { Events::handler(e, Event::acc); });// 快进按钮
    Events::bond(gui->main.imgbtn_play, [](event e) { Events::handler(e, Event::play); });// 播放暂停按钮
    Events::bond(gui->main.screen, Event::spectrum_draw);
}

//
///*****************************自定义接口*********************************/
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

auto Spectrum::update() -> void
{
    // 更新FFT频谱数据
    uint8_t temp = spectrum[0];
    spectrum[FFT_NUM - 1] = temp;
    for (uint32_t i = 0; i < FFT_NUM - 1; ++i)
    {
        temp = spectrum[i + 1];
        spectrum[i] = temp;
    }
    lv_obj_invalidate(gui->main.spectrum);  // 使频谱区域无效，触发重绘
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
            lv_label_set_text(gui->main.label_speed, "慢放0.25");
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
    lv_slider_set_range(gui->main.slider, 0, time_list[track_id]);
}

auto Play::pause() -> void
{
    slider_timer.pause();
    spectrum_timer.pause();
}

/*****************************************定时器*************************************/
static auto slider_timer_cb(lv_timer_t *timer) -> void
{
    time++;
    Text::set_object(GUI_Base::get_ui()->main.label_slider_time);
    Text::set_text_fmt("%d:%02d", time / 60, time % 60);

    Slider::set_object(GUI_Base::get_ui()->main.slider);
    Slider::set_value(time, LV_ANIM_ON);
}


static auto spectrum_timer_cb(lv_timer_t *timer) -> void
{
    // 更新FFT频谱数据
    uint8_t temp = spectrum[0];
    spectrum[FFT_NUM - 1] = temp;
    for (uint32_t i = 0; i < FFT_NUM - 1; ++i)
    {
        temp = spectrum[i + 1];
        spectrum[i] = temp;
    }
    Component::set_object(GUI_Base::get_ui()->main.spectrum);
    Component::invalidate();// 使频谱区域无效，触发重绘
}

#endif