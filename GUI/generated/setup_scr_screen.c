/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 480, 320);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_11
    ui->screen_img_11 = lv_img_create(ui->screen);
    lv_obj_add_flag(ui->screen_img_11, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_11, &_icn_slider_alpha_15x15);
    lv_img_set_pivot(ui->screen_img_11, 0,0);
    lv_img_set_angle(ui->screen_img_11, 0);
    lv_obj_set_pos(ui->screen_img_11, 235, 152);
    lv_obj_set_size(ui->screen_img_11, 15, 15);
    lv_obj_add_flag(ui->screen_img_11, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_img_11, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_11, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_11, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_11, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_speed
    ui->screen_label_speed = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_speed, "快进×1.75");
    lv_label_set_long_mode(ui->screen_label_speed, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_speed, 392, 26);
    lv_obj_set_size(ui->screen_label_speed, 74, 13);
    lv_obj_add_flag(ui->screen_label_speed, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_label_speed, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_speed, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_speed, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_speed, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_speed, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_speed, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_speed, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_imgbtn_slow
    ui->screen_imgbtn_slow = lv_imgbtn_create(ui->screen);
    lv_obj_add_flag(ui->screen_imgbtn_slow, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->screen_imgbtn_slow, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_prev_alpha_37x37, NULL);
    ui->screen_imgbtn_slow_label = lv_label_create(ui->screen_imgbtn_slow);
    lv_label_set_text(ui->screen_imgbtn_slow_label, "");
    lv_label_set_long_mode(ui->screen_imgbtn_slow_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_imgbtn_slow_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_imgbtn_slow, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_imgbtn_slow, 122, 240);
    lv_obj_set_size(ui->screen_imgbtn_slow, 37, 37);

    //Write style for screen_imgbtn_slow, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_imgbtn_slow, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_imgbtn_slow, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_slow, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_imgbtn_slow, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_imgbtn_slow, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_imgbtn_slow, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_slow, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_imgbtn_slow, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_imgbtn_slow, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_slow, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_imgbtn_slow, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_slow, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_imgbtn_slow, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_imgbtn_slow, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_slow, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_imgbtn_slow, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_slow, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_slow, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_imgbtn_acc
    ui->screen_imgbtn_acc = lv_imgbtn_create(ui->screen);
    lv_obj_add_flag(ui->screen_imgbtn_acc, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->screen_imgbtn_acc, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_next_alpha_37x37, NULL);
    lv_imgbtn_set_src(ui->screen_imgbtn_acc, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_icn_slider_alpha_37x37, NULL);
    ui->screen_imgbtn_acc_label = lv_label_create(ui->screen_imgbtn_acc);
    lv_label_set_text(ui->screen_imgbtn_acc_label, "");
    lv_label_set_long_mode(ui->screen_imgbtn_acc_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_imgbtn_acc_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_imgbtn_acc, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_imgbtn_acc, 318, 240);
    lv_obj_set_size(ui->screen_imgbtn_acc, 37, 37);

    //Write style for screen_imgbtn_acc, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_imgbtn_acc, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_imgbtn_acc, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_acc, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_imgbtn_acc, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_imgbtn_acc, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_imgbtn_acc, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_acc, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_imgbtn_acc, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_imgbtn_acc, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_acc, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_imgbtn_acc, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_acc, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_imgbtn_acc, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_imgbtn_acc, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_acc, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_imgbtn_acc, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_acc, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_acc, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //Write codes screen_label_title_music
    ui->screen_label_title_music = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_title_music, "Waiting for true love");
    lv_label_set_long_mode(ui->screen_label_title_music, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_title_music, 150, 5);
    lv_obj_set_size(ui->screen_label_title_music, 180, 21);

    //Write style for screen_label_title_music, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_title_music, lv_color_hex(0x504d6d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_title_music, &lv_font_arial_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_title_music, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_title_music, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_title_music, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_slider_time
    ui->screen_label_slider_time = lv_label_create(ui->screen);
    lv_label_set_text(ui->screen_label_slider_time, "0:00");
    lv_label_set_long_mode(ui->screen_label_slider_time, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_label_slider_time, 420, 288);
    lv_obj_set_size(ui->screen_label_slider_time, 36, 12);

    //Write style for screen_label_slider_time, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_slider_time, lv_color_hex(0x8a86b8), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_slider_time, &lv_font_arial_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_slider_time, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_slider_time, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_slider_time, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_slider_1
    ui->screen_slider_1 = lv_slider_create(ui->screen);
    lv_slider_set_range(ui->screen_slider_1, 0, 100);
    lv_slider_set_mode(ui->screen_slider_1, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_slider_1, 0, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_slider_1, 42, 294);
    lv_obj_set_size(ui->screen_slider_1, 342, 1);

    //Write style for screen_slider_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_VER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_slider_1, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_slider_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 58, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_slider_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_slider_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_1, lv_color_hex(0x5691f8), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_1, LV_GRAD_DIR_HOR, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui->screen_slider_1, lv_color_hex(0xa666f1), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui->screen_slider_1, 0, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui->screen_slider_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 58, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_slider_1, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_1, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_1, 58, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_img_wave_top
    ui->screen_img_wave_top = lv_img_create(ui->screen);
    lv_obj_add_flag(ui->screen_img_wave_top, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_wave_top, &_wave_top_alpha_480x34);
    lv_img_set_pivot(ui->screen_img_wave_top, 0,0);
    lv_img_set_angle(ui->screen_img_wave_top, 0);
    lv_obj_set_pos(ui->screen_img_wave_top, 0, 0);
    lv_obj_set_size(ui->screen_img_wave_top, 480, 34);

    //Write style for screen_img_wave_top, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_wave_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_wave_top, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_wave_top, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_wave_top, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_img_wave_bottom
    ui->screen_img_wave_bottom = lv_img_create(ui->screen);
    lv_obj_add_flag(ui->screen_img_wave_bottom, LV_OBJ_FLAG_CLICKABLE);
    lv_img_set_src(ui->screen_img_wave_bottom, &_wave_bottom_alpha_480x34);
    lv_img_set_pivot(ui->screen_img_wave_bottom, 0,0);
    lv_img_set_angle(ui->screen_img_wave_bottom, 0);
    lv_obj_set_pos(ui->screen_img_wave_bottom, 0, 285);
    lv_obj_set_size(ui->screen_img_wave_bottom, 480, 34);

    //Write style for screen_img_wave_bottom, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_img_recolor_opa(ui->screen_img_wave_bottom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_img_opa(ui->screen_img_wave_bottom, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_img_wave_bottom, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_img_wave_bottom, true, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_imgbtn_play
    ui->screen_imgbtn_play = lv_imgbtn_create(ui->screen);
    lv_obj_add_flag(ui->screen_imgbtn_play, LV_OBJ_FLAG_CHECKABLE);
    lv_imgbtn_set_src(ui->screen_imgbtn_play, LV_IMGBTN_STATE_RELEASED, NULL, &_btn_play_alpha_48x48, NULL);
    lv_imgbtn_set_src(ui->screen_imgbtn_play, LV_IMGBTN_STATE_PRESSED, NULL, &_btn_play_alpha_48x48, NULL);
    lv_imgbtn_set_src(ui->screen_imgbtn_play, LV_IMGBTN_STATE_CHECKED_RELEASED, NULL, &_btn_pause_alpha_48x48, NULL);
    lv_imgbtn_set_src(ui->screen_imgbtn_play, LV_IMGBTN_STATE_CHECKED_PRESSED, NULL, &_btn_pause_alpha_48x48, NULL);
    ui->screen_imgbtn_play_label = lv_label_create(ui->screen_imgbtn_play);
    lv_label_set_text(ui->screen_imgbtn_play_label, "");
    lv_label_set_long_mode(ui->screen_imgbtn_play_label, LV_LABEL_LONG_WRAP);
    lv_obj_align(ui->screen_imgbtn_play_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_pad_all(ui->screen_imgbtn_play, 0, LV_STATE_DEFAULT);
    lv_obj_set_pos(ui->screen_imgbtn_play, 216, 234);
    lv_obj_set_size(ui->screen_imgbtn_play, 48, 48);

    //Write style for screen_imgbtn_play, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_imgbtn_play, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_imgbtn_play, &lv_font_arial_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_play, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_imgbtn_play, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_clip_corner(ui->screen_imgbtn_play, true, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_imgbtn_play, Part: LV_PART_MAIN, State: LV_STATE_PRESSED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_play, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_color(ui->screen_imgbtn_play, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_font(ui->screen_imgbtn_play, &lv_font_arial_16, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_play, 255, LV_PART_MAIN|LV_STATE_PRESSED);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_STATE_PRESSED);

    //Write style for screen_imgbtn_play, Part: LV_PART_MAIN, State: LV_STATE_CHECKED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_play, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_color(ui->screen_imgbtn_play, lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_font(ui->screen_imgbtn_play, &lv_font_arial_16, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_text_opa(ui->screen_imgbtn_play, 255, LV_PART_MAIN|LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_STATE_CHECKED);

    //Write style for screen_imgbtn_play, Part: LV_PART_MAIN, State: LV_IMGBTN_STATE_RELEASED.
    lv_obj_set_style_img_recolor_opa(ui->screen_imgbtn_play, 0, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);
    lv_obj_set_style_img_opa(ui->screen_imgbtn_play, 255, LV_PART_MAIN|LV_IMGBTN_STATE_RELEASED);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}
