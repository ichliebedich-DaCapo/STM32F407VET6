//
// Created by fairy on 2024/10/18 18:34.
//
#include "JYZQ_Conf.h"
#include "component.hpp"
#include "events.hpp"
#include "custom.hpp"
#include "ui.hpp"

#if 1
// 头文件
lv_ui_t gui;


// 函数
void ui_init()
{
    guider_ui.screen = lv_obj_create(nullptr);
    lv_obj_set_size(guider_ui.screen, 480, 320);

    /******************************************文本框***************************************/

    TextInitializer text{};

    //Write codes screen_label_speed
    text.init(guider_ui.screen, guider_ui.screen_label_speed, "快进×1.75");
    text.set_pos_size(392, 26, 74, 13);
    text.add_flag(LV_OBJ_FLAG_HIDDEN);
    text.set_text_font_space(&lv_font_SourceHanSerifSC_Regular_12, 2);

    //Write codes screen_label_title_music
    text.init(guider_ui.screen, guider_ui.screen_label_title_music, "Waiting for true love");
    text.set_pos_size(150, 5, 180, 21);
    text.set_text_font_space(&lv_font_arial_14);
    text.set_text_color(lv_color_hex(0x504d6d));
    text.set_text_align(LV_TEXT_ALIGN_CENTER);// 设置文本居中

    //Write codes screen_label_slider_time
    text.init(guider_ui.screen, guider_ui.screen_label_slider_time, "0:00");
    text.set_pos_size(420, 288, 36, 12);
    text.set_text_font_space(&lv_font_arial_12);
    text.set_text_color(lv_color_hex(0x8a86b8));
    text.set_text_align(LV_TEXT_ALIGN_CENTER);

    /******************************************滑条***************************************/
    //Write codes screen_slider_1
    SliderInitializer slider{};
    slider.init(guider_ui.screen, guider_ui.screen_slider_1);
    slider.set_range(0, 100);
    slider.set_pos_size(42, 294, 342, 1);
    slider.set_bg_color(lv_color_hex(0x2195f6), 100);


    /*************************************图片*************************************/
    ImageInitializer img{};

    //Write codes screen_img_11
    img.init(guider_ui.screen, guider_ui.screen_img_11);
    img.set_src(&_icn_slider_alpha_15x15);
    img.set_pos_size(235, 152, 15, 15);
    img.add_flag(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_HIDDEN);
    img.set_style_clip_corner(LV_PART_MAIN | LV_STATE_DEFAULT);// 设置为圆角


    /************************************图片按钮************************************/
    ImageButtonInitializer imgbtn{};

    //Write codes screen_imgbtn_play
    imgbtn.init(guider_ui.screen, guider_ui.screen_imgbtn_play);
    imgbtn.set_pos_size(216, 234, 48, 48);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
    imgbtn.set_src(LV_IMGBTN_STATE_RELEASED, &_btn_play_alpha_48x48);// 正常状态.播放
    imgbtn.set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_btn_pause_alpha_48x48);// 按下按钮.暂停

    //Write codes screen_imgbtn_acc
    imgbtn.init(guider_ui.screen, guider_ui.screen_imgbtn_acc);
    imgbtn.set_pos_size(318, 240, 37, 37);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
    imgbtn.set_src(LV_IMGBTN_STATE_RELEASED, &_btn_next_alpha_37x37);
    imgbtn.set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_icn_slider_alpha_37x37);

    //Write codes screen_imgbtn_slow
    imgbtn.init(guider_ui.screen, guider_ui.screen_imgbtn_slow);
    imgbtn.set_pos_size(122, 240, 37, 37);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
    imgbtn.set_src(LV_IMGBTN_STATE_RELEASED, &_btn_prev_alpha_37x37);
    imgbtn.set_src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_icn_slider_alpha_37x37);

}

/******************************************事件实现*************************************************/



// 快进处理
static void acc_event(bool is_checked)
{
    lv_playspeed_print(is_checked ? LV_PLAYSPEED_ACC : LV_PLAYSPEED_NORMAL);// 显示播放速度信息
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


void events_init()
{
    Events::bond(guider_ui.screen_imgbtn_acc, [](event e) { Events::handler(e, acc_event); });// 快进按钮
    Events::bond(guider_ui.screen_imgbtn_play, [](event e) { Events::handler(e, play_event); });// 播放暂停按钮
    custom_init(&guider_ui);
}

#endif