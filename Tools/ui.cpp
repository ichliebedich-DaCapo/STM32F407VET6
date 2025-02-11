#include "ui.hpp"

namespace gui::widgets::main 
{
    Component  obj_blueCounter_cont_1;
	ImageButton  imgbtn_blueCounter_plus;
	ImageButton  imgbtn_blueCounter_minus;
	Label  label_blueCounter_counter;
	Image  img_blueCounter_logo;
}

// 使用命名空间
using namespace gui::widgets::main;
namespace gui::init 
{
    void screen()
    {
        scr.bg_color(lv_color_hex(0x599cc7))
			.bg_grad_dir(LV_GRAD_DIR_VER)
			.bg_grad_color(lv_color_hex(0x8dcdeb))
			.bg_main_stop(0)
			.bg_grad_stop(255);

		obj_blueCounter_cont_1
			.pos_size(93, 60, 294, 200)
			.border_width(4)
			.border_opa(255)
			.border_color(lv_color_hex(0xacceea))
			.border_side(LV_BORDER_SIDE_FULL)
			.radius(14)
			.bg_color(lv_color_hex(0xffffff))
			.bg_grad_dir(LV_GRAD_DIR_VER)
			.bg_grad_color(lv_color_hex(0x4e95cb))
			.bg_main_stop(0)
			.bg_grad_stop(255);

		imgbtn_blueCounter_plus
			.pos_size(193, 15, 65, 65)
			.checkable()
			.src(LV_IMAGEBUTTON_STATE_RELEASED, &_btn_RGB565A8_65x65)
			.src(LV_IMAGEBUTTON_STATE_PRESSED, &_btn_RGB565A8_65x65)
			.src(LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, &_btn_RGB565A8_65x65)
			.src(LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, &_btn_RGB565A8_65x65)
			.center()
			.text_color(lv_color_hex(0x4d6e81))
			.text_font(&lv_font_arial_34)
			.image_recolor_opa(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_opa(129, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_color(lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED)
			.text_font(&lv_font_montserratMedium_34, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_opa(255, LV_PART_MAIN|LV_STATE_PRESSED)
			.shadow_width(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_recolor_opa(0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED)
			.image_opa(255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

		imgbtn_blueCounter_minus
			.pos_size(193, 100, 65, 65)
			.checkable()
			.src(LV_IMAGEBUTTON_STATE_RELEASED, &_btn_RGB565A8_65x65)
			.src(LV_IMAGEBUTTON_STATE_PRESSED, &_btn_RGB565A8_65x65)
			.src(LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, &_btn_RGB565A8_65x65)
			.src(LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, &_btn_RGB565A8_65x65)
			.center()
			.text_color(lv_color_hex(0x4d6e81))
			.text_font(&lv_font_arial_34)
			.image_recolor_opa(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_opa(129, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_color(lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED)
			.text_font(&lv_font_montserratMedium_34, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_opa(255, LV_PART_MAIN|LV_STATE_PRESSED)
			.shadow_width(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_recolor_opa(0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED)
			.image_opa(255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

		label_blueCounter_counter
			.pos_size(58, 74, 114, 50)
			.radius(8)
			.text_color(lv_color_hex(0x024f5a))
			.text_font(&lv_font_arial_40)
			.text_letter_space(2)
			.bg_opa(0);

		img_blueCounter_logo
			.pos_size(11, 10, 60, 34)
			.add_flag(LV_OBJ_FLAG_CLICKABLE)
			.image_recolor_opa(0)
			.image_opa(255);

    }
    
    void events()
    {
    
    }
}