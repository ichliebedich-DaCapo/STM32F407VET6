/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <cstdio>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"
#include "scr_screen.hpp"
#include <iostream>

auto Screen::init(lv_ui *ui) -> void
{
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 480, 320);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);



    //Write codes screen_label_speed
    ui->screen_label_speed = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_speed, "快进×1.75");
    lv_label_set_long_mode(ui->screen_label_speed, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_speed, 392, 26);
    lv_obj_set_size(ui->screen_label_speed, 74, 13);
    lv_obj_add_flag(ui->screen_label_speed, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_label_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_label_speed, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_speed, &lv_font_SourceHanSerifSC_Regular_12,
                               LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_speed, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_speed, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_speed, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);



    //Write codes screen_label_title_music
    ui->screen_label_title_music = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_title_music, "Waiting for true love");
    lv_label_set_long_mode(ui->screen_label_title_music, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_title_music, 150, 5);
    lv_obj_set_size(ui->screen_label_title_music, 180, 21);

    //Write style for screen_label_title_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_label_title_music, lv_color_hex(0x504d6d), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_title_music, &lv_font_arial_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_title_music, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_title_music, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);


    //Write codes screen_label_slider_time
    ui->screen_label_slider_time = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_slider_time, "0:00");
    lv_label_set_long_mode(ui->screen_label_slider_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_slider_time, 420, 288);
    lv_obj_set_size(ui->screen_label_slider_time, 36, 12);

    //Write style for screen_label_slider_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_label_slider_time, lv_color_hex(0x8a86b8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_slider_time, &lv_font_arial_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_slider_time, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_slider_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);


    //Write codes screen_slider_1
    ui->screen_slider_1 = lv_slider_create(ui->screen);
    lv_slider_set_range(ui->screen_slider_1, 0, 100);
    lv_slider_set_mode(ui->screen_slider_1, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_slider_1, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_slider_1, 42, 294);
    lv_obj_set_size(ui->screen_slider_1, 342, 1);

    //Write style for screen_slider_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 100, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_slider_1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 58, LV_PART_MAIN | LV_STATE_DEFAULT);

    //Write style for screen_slider_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x5691f8), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_HOR, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_slider_1, lv_color_hex(0xa666f1), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_slider_1, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 58, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    //Write style for screen_slider_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_radius(ui->screen_slider_1, 58, LV_PART_KNOB | LV_STATE_DEFAULT);

    /*************************************图片*************************************/
    ImageInitializer img{};

    //Write codes screen_img_11
    img.init(ui->screen, ui->screen_img_11);
    img.set_Src(&_icn_slider_alpha_15x15);
    img.set_pos_size(235, 152, 15, 15);
    img.add_flag(LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_HIDDEN);
    img.set_style_clip_corner(LV_PART_MAIN | LV_STATE_DEFAULT);// 设置为圆角


    /************************************图片按钮************************************/
    ImageButtonInitializer imgbtn{};

    //Write codes screen_imgbtn_play
    imgbtn.init(ui->screen, ui->screen_imgbtn_play);
    imgbtn.set_pos_size(216, 234, 48, 48);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
//    imgbtn.set_Src(LV_IMGBTN_STATE_RELEASED, &_btn_play_alpha_48x48);// 正常状态->播放
//    imgbtn.set_Src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_btn_pause_alpha_48x48);// 按下按钮->暂停


    //Write codes screen_imgbtn_acc
    imgbtn.init(ui->screen, ui->screen_imgbtn_acc);
    imgbtn.set_pos_size(318, 240, 37, 37);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
//    imgbtn.set_Src(LV_IMGBTN_STATE_RELEASED, &_btn_next_alpha_37x37);
//    imgbtn.set_Src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_icn_slider_alpha_37x37);

    //Write codes screen_imgbtn_slow
    imgbtn.init(ui->screen, ui->screen_imgbtn_slow);
    imgbtn.set_pos_size(122, 240, 37, 37);
    imgbtn.set_style_clip_corner((LV_PART_MAIN | LV_STATE_DEFAULT));
    imgbtn.add_flag(LV_OBJ_FLAG_CHECKABLE);
//    imgbtn.set_Src(LV_IMGBTN_STATE_RELEASED, &_btn_prev_alpha_37x37);
//    imgbtn.set_Src(LV_IMGBTN_STATE_CHECKED_RELEASED, &_icn_slider_alpha_37x37);

    //Init events for screen.
    lv_obj_update_layout(guider_ui.screen);
    events_init_screen(&guider_ui);

}
