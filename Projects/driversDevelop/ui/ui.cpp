//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
#include "GUI.hpp"

// 默认使用main屏幕里的组件（暂时用不到其他界面）
using namespace gui::widgets::main;


// 颜色宏定义
#define COLOR_PRIMARY lv_color_hex(0x34e6ff)
#define COLOR_SECONDARY lv_color_hex(0x606060)
#define COLOR_BG lv_color_hex(0xF5F5F5)
#define COLOR_TEXT_MAIN lv_color_hex(0x333333)

namespace gui::widgets::main {
    // 状态显示组件
    Label lbl_status;
    Label lbl_time;

    // 数据仪表组件
    Label lbl_temp_value;
    Label lbl_humi_value;
    Label lbl_press_value;

    // 控制按钮
    Button btn_fan;
    Button btn_light;
    Button btn_alarm;

    // 设置按钮
    Button btn_settings;
}

namespace gui::init {
    // 事件处理函数示例
    void toggle_fan() {
        static bool state = false;
        widgets::main::btn_fan.text(state  ? "OFF" : "ON")
                .bg_color(state ? COLOR_SECONDARY : COLOR_PRIMARY);
        state = !state;
    }

    void screen() {
        // 设置全局字体
        Label::Font(lv_customer_font_SourceHanSerifSC_Regular_15);
        Button::Font(lv_customer_font_SourceHanSerifSC_Regular_15);

        // 状态栏容器
        auto status_bar = lv_obj_create(scr);
        lv_obj_set_size(status_bar, 480, 30);
        lv_obj_set_style_bg_color(status_bar, COLOR_PRIMARY, 0);

        // 状态显示
        widgets::main::lbl_status.init(status_bar)
                .text("STATUS: NORMAL").pos(10, 0)
                .text_color(lv_color_white());

        widgets::main::lbl_time.init(status_bar)
                .text("12:00").align(LV_ALIGN_RIGHT_MID, -10, 0)
                .text_color(lv_color_white());

        // 主数据面板
        auto data_panel = lv_obj_create(scr);
        lv_obj_set_size(data_panel, 300, 250);
        lv_obj_align(data_panel, LV_ALIGN_TOP_LEFT, 10, 40);
        lv_obj_set_style_radius(data_panel, 10, 0);

        // 温度显示
        widgets::main::lbl_temp_value.init(data_panel)
                .pos_size(20, 20, 260, 60)
                .text("25.6℃")
                .font(lv_customer_font_SourceHanSerifSC_Regular_15)
                .text_align(LV_TEXT_ALIGN_CENTER)
                .bg_color(COLOR_BG)
                .border_radius(8);

        // 湿度显示
        widgets::main::lbl_humi_value.init(data_panel)
                .pos_size(20, 100, 260, 60)
                .text("65% RH")
                .font(lv_customer_font_SourceHanSerifSC_Regular_15)
                .text_align(LV_TEXT_ALIGN_CENTER)
                .bg_color(COLOR_BG)
                .border_radius(8);

        // 控制面板
        auto ctrl_panel = lv_obj_create(scr);
        lv_obj_set_size(ctrl_panel, 150, 250);
        lv_obj_align(ctrl_panel, LV_ALIGN_TOP_RIGHT, -10, 40);
        lv_obj_set_style_pad_all(ctrl_panel, 10, 0);

        // 风扇控制按钮
        widgets::main::btn_fan.init()
                .pos_size(0, 0, 130, 50)
                .text("FAN: OFF")
                .bg_color(COLOR_SECONDARY)
                .OnClicked<toggle_fan>();

        // 灯光控制按钮
        widgets::main::btn_light.init()
                .pos_size(0, 60, 130, 50)
                .text("LIGHT")
                .bg_color(COLOR_PRIMARY)
                .OnClicked([](Event_t e) {

                });

        // 底部操作栏
        auto bottom_bar = lv_obj_create(scr);
        lv_obj_set_size(bottom_bar, 480, 40);
        lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_MID, 0, 0);

        widgets::main::btn_settings.init()
                .pos_size(400, 5, 70, 30)
                .text("SETTING")
                .bg_color(COLOR_PRIMARY);
    }
}

namespace gui::init{
    void events()
    {

    }
}

/// UI接口
namespace gui::interface
{

}




