#include "ui.hpp"

namespace gui::widgets::main 
{
    Component  obj_blueCounter_cont_1;
	Component  imgbtn_blueCounter_plus;
	Component  imgbtn_blueCounter_minus;
	Component  label_blueCounter_counter;
	Component  img_blueCounter_logo;
}

// 使用命名空间
using namespace gui::widgets::main;
namespace gui::init 
{
    void screen()
    {
        scr.bg_opa(255)
			.bg_color(lv_color_hex(0x599cc7))
			.bg_grad_dir(LV_GRAD_DIR_VER)
			.bg_grad_color(lv_color_hex(0x8dcdeb))
			.bg_main_stop(0)
			.bg_grad_stop(255);

		obj_blueCounter_cont_1.init( 93,   60,   294,   200)
			.border_width(4)
			.border_opa(255)
			.border_color(lv_color_hex(0xacceea))
			.border_side(LV_BORDER_SIDE_FULL)
			.radius(14)
			.bg_opa(255)
			.bg_color(lv_color_hex(0xffffff))
			.bg_grad_dir(LV_GRAD_DIR_VER)
			.bg_grad_color(lv_color_hex(0x4e95cb))
			.bg_main_stop(0)
			.bg_grad_stop(255)
			.pad_top(0)
			.pad_bottom(0)
			.pad_left(0)
			.pad_right(0)
			.shadow_width(0);

		imgbtn_blueCounter_plus.init( 193,   15,   65,   65)
			.add_flag(LV_OBJ_FLAG_CHECKABLE)
			.text_color(lv_color_hex(0x4d6e81))
			.text_font(&lv_font_arial_34)
			.text_opa(255)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.shadow_width(0)
			.image_recolor_opa(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_opa(129, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_color(lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED)
			.text_font(&lv_font_montserratMedium_34, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_opa(255, LV_PART_MAIN|LV_STATE_PRESSED)
			.shadow_width(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_recolor_opa(0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED)
			.image_opa(255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

		imgbtn_blueCounter_minus.init( 193,   100,   65,   65)
			.add_flag(LV_OBJ_FLAG_CHECKABLE)
			.text_color(lv_color_hex(0x4d6e81))
			.text_font(&lv_font_arial_34)
			.text_opa(255)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.shadow_width(0)
			.image_recolor_opa(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_opa(129, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_color(lv_color_hex(0xFF33FF), LV_PART_MAIN|LV_STATE_PRESSED)
			.text_font(&lv_font_montserratMedium_34, LV_PART_MAIN|LV_STATE_PRESSED)
			.text_opa(255, LV_PART_MAIN|LV_STATE_PRESSED)
			.shadow_width(0, LV_PART_MAIN|LV_STATE_PRESSED)
			.image_recolor_opa(0, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED)
			.image_opa(255, LV_PART_MAIN|LV_IMAGEBUTTON_STATE_RELEASED);

		label_blueCounter_counter.init( 58,   74,   114,   50)
			.border_width(0)
			.radius(8)
			.text_color(lv_color_hex(0x024f5a))
			.text_font(&lv_font_arial_40)
			.text_opa(255)
			.text_letter_space(2)
			.text_line_space(0)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0)
			.pad_top(0)
			.pad_right(0)
			.pad_bottom(0)
			.pad_left(0)
			.shadow_width(0);

		img_blueCounter_logo.init( 11,   10,   60,   34)
			.add_flag(LV_OBJ_FLAG_CLICKABLE)
			.image_recolor_opa(0)
			.image_opa(255);

    }
    
    void events()
    {
    
    }
}