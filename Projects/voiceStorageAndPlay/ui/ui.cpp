//
// Created by fairy on 2024/10/18 18:34.
//
#include <cstring>
#include "ui.hpp"
#include "spectrum_1.h"

/********变量声明********/

static int32_t time = 0;

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
Timer slider_timer;
Timer spectrum_timer;
Timer record_blink_timer;//用于让录音状态闪烁

namespace gui::widgets::main
{
    Image img_slider_flag;
    ImageButton imgbtn_slow;
    ImageButton imgbtn_acc;
    Label label_title_music;
    Slider slider;
    Label label_play_speed;
    Label label_slider_time;
    ImageButton imgbtn_play;
    Component spectrum;// 频谱
    Label label_record_state;// 录音状态
    Label label_save_state;// 保存状态
    Label label_record_sample_rate;// 录音采样率
}

/******************************************事件实现*************************************************/
// 播放组件
class Play
{
public:
    static auto resume() -> void;// 恢复播放
    static auto pause() -> void;// 暂停播放
};

// 频谱组件
class Spectrum
{
public:
    static auto draw(lv_event_t *e) -> void;// 重绘频谱
    static auto update() -> void;// 更新频谱数据
};


namespace ui_main = gui::widgets::main;
using namespace ui_main;
namespace gui::init
{
    void screen()
    {
        /*************************文本框**********************/

        Label::Font(lv_customer_font_SourceHanSerifSC_Regular_12);

        // 倍速文本框
        label_play_speed.init(28, 254, 70, 20, "")
                .hidden();

        // 进度条时间
        label_slider_time.init(393, 288, 40, 20, "0:00")
                .text_color(lv_color_hex(0x8a86b8));

        // 保存状态
        label_save_state.init(10, 5, 50, 20, "未保存")
                .text_color(lv_color_hex(0x8a86b8));


        // 录音状态 需要闪烁
        label_record_state.init(10, 5, 50, 20, "录音中")
                .text_color(lv_color_hex(0xcd0303))
                .hidden();

        // 采样率
        label_record_sample_rate.init(410, 5, 70, 20, "采样率:8K")
                .text_color(lv_color_hex(0x504d6d));;

        // 设置全局字体
        Label::Font(lv_customer_font_SourceHanSerifSC_Regular_14);

        // 项目标题
        label_title_music.init(150, 10, 180, 21, "语音存储与回放")
                .text_color(lv_color_hex(0x504d6d))
                .center();

        // 进度条图片
        img_slider_flag.init(235, 152, 15, 15, &_icn_slider_alpha_15x15);


        // 播放按钮
        imgbtn_play.init(216, 234, 48, 48, &_btn_list_play_alpha_48x48, &_btn_list_play_alpha_48x48);

        // 加速按钮
        imgbtn_acc.init(318, 234, 37, 37, &_btn_next_alpha_37x37);;

        // 减速按钮
        imgbtn_slow.init(122, 240, 37, 37, &_btn_prev_alpha_37x37);

        // 进度条
        slider.init(42, 294, 342, 1, 0, 263, lv_color_hex(0x2195f6), 100, &_icn_slider_alpha_15x15);

        // 频谱
        spectrum.init()
                .pos_size(spectrum_start_x - 4, spectrum_start_y, spectrum_width + 6, spectrum_height + 1);

    }


    void events()
    {
        imgbtn_play.OnPressedReleased<Play::resume, Play::pause>();
        scr.bind_event<Spectrum::draw>();


        // 进度条
        slider_timer.create([](Timer_t)
                            {
                                time++;
                                label_slider_time.text("%d:%02d", time / 60, time % 60);
                                slider.value(time, LV_ANIM_ON);
                            }, 1000);

        // 定时更新频谱数据
        spectrum_timer.create([](Timer_t)
                              {               // 更新FFT频谱数据
                                  uint8_t temp = spectrum_1[0];
                                  spectrum_1[fft_num - 1] = temp;
                                  for (uint32_t i = 0; i < fft_num - 1; ++i)
                                  {
                                      temp = spectrum_1[i + 1];
                                      spectrum_1[i] = temp;
                                  }
                                  spectrum.invalidate();// 使频谱区域无效，触发重绘
                              }, 30);

        record_blink_timer.create([](Timer_t)
                                  { // 闪烁录音状态
                                      static bool is_show = false;
                                      is_show = !is_show;
                                      if (is_show)
                                      {
                                          label_record_state.appear();
                                      }
                                      else
                                      {
                                          label_record_state.hidden();
                                      }
                                  }, 500);

    }


}

///*****************************实现接口*********************************/
auto Spectrum::draw(Event_t e) -> void
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_DRAW_POST)
    {
        lv_layer_t *layer = lv_event_get_layer(e); // 替换为获取 layer

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
            uint32_t y1 = start_y - spectrum_1[(i << 2)] * spectrum_height / 255;  // 从底部开始，但考虑最大高度

            // 计算当前频谱条的高度比例，用于渐变色
            uint8_t height_ratio = (start_y - y1) * 255 / spectrum_height;

            // 根据高度比例调整渐变色
            lv_color_t color = lv_color_mix(grad.stops[0].color, grad.stops[1].color, height_ratio);
            line_dsc.color = color;

            // 定义两个点
            line_dsc.color = color;

            lv_point_precise_t p1 = {
                    .x = static_cast<lv_value_precise_t>(x1),
                    .y = static_cast<lv_value_precise_t>(y1)
            };
            lv_point_precise_t p2 = {
                    .x = static_cast<lv_value_precise_t>(x1),
                    .y = static_cast<lv_value_precise_t>(start_y)
            };


            line_dsc.p1 = p1;
            line_dsc.p2 = p2;

            // 调用新 API
            lv_draw_line(layer, &line_dsc);
        }
    }
}

auto Spectrum::update() -> void
{
    // 更新FFT频谱数据
    uint8_t temp = spectrum_1[0];
    spectrum_1[fft_num - 1] = temp;
    for (uint32_t i = 0; i < fft_num - 1; ++i)
    {
        temp = spectrum_1[i + 1];
        spectrum_1[i] = temp;
    }
    spectrum.invalidate();// 使频谱区域无效，触发重绘
}


auto Play::resume() -> void
{
    slider_timer.resume();
    spectrum_timer.resume();
    slider.range(0, 255);
}

auto Play::pause() -> void
{
    slider_timer.pause();
    spectrum_timer.pause();
}

namespace gui::interface
{
/***********************对外接口*************************/
    auto reset_time() -> void
    {
        time = 0;
    }

    auto resume_record() -> void
    {
        slider_timer.resume();
        record_blink_timer.resume();
        label_record_state.appear();
    }

    auto pause_record() -> void
    {
        slider_timer.pause();
        record_blink_timer.pause();
        label_record_state.hidden();
    }

// 设置录音采样率
    auto set_record_state(RecordSampleRate state) -> void
    {
        switch (state)
        {
            case RecordSampleRate::SAMPLE_RATE_8K:
                label_record_sample_rate.text("采样率:8K");
                // 2*2^20/8K = 262.144s
                slider.range(0, 263);
                break;

            case RecordSampleRate::SAMPLE_RATE_16K:
                label_record_sample_rate.text("采样率:16K");
                // 2*2^20/16K = 131.072s
                slider.range(0, 132);
                break;

            default:
                break;
        }
    }

    auto set_play_speed(PlaySpeed speed) -> void
    {
        switch (speed)
        {
            case PlaySpeed::SPEED_NORMAL:
                label_play_speed.hidden();
                break;
            case PlaySpeed::SPEED_0_75:
                label_play_speed.appear().text("慢放×0.75");
                break;
            case PlaySpeed::SPEED_1_5:
                label_play_speed.appear().text("快进×1.5");
                break;
            default:
                break;
        }

    }

    auto saveInfo(bool state) -> void
    {
        if (state)
        {
            label_save_state.text("已保存").text_color(lv_color_hex(0x11a0f5));
        }
        else
        {
            label_save_state.text("未保存").text_color(lv_color_hex(0x8a86b8));
        }

    }


/**
 * @brief 擦除
 * @note 会自动关闭录音的动画显示
 */
    auto erasing() -> void
    {
        label_record_state.text("擦除中").appear();
        record_blink_timer.resume();
    }

    auto erase_done() -> void
    {
        label_record_state.text("录音中").hidden();
        record_blink_timer.pause();
    }

    auto get_time() -> uint32_t
    {
        return time;
    }

    auto set_time(uint32_t t) -> void
    {
        time = t;
    }
}



