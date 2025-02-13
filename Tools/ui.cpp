#include "ui.hpp"

namespace gui::widgets::main 
{
    
	Component obj_blueCounter_cont_1;
	ImageButton imgbtn_blueCounter_plus;
	Label label_blueCounter_plus_label;
	ImageButton imgbtn_blueCounter_minus;
	Label label_blueCounter_minus_label;
	Button btn_blueCounter_btn_1;
	Label label_blueCounter_btn_1_label;
	Image img_blueCounter_logo;
	Label label_blueCounter_counter;
	CheckBox chekcbox_blueCounter_cb_1;
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

	obj_blueCounter_cont_1.init()
		.pos(93, 60)
		.size(294, 200)
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

	imgbtn_blueCounter_plus.init(obj_blueCounter_cont_1)
		.pos(193, 15)
		.size(65, 65)
		.checkable()
		.released_src(&_btn_RGB565A8_65x65)
		.pressed_src(&_btn_RGB565A8_65x65)
		.checked_released_src(&_btn_RGB565A8_65x65)
		.checked_pressed_src(&_btn_RGB565A8_65x65)
		.pad_all(0, LV_STATE_DEFAULT)
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

	label_blueCounter_plus_label.init(imgbtn_blueCounter_plus)
		.text("+")
		.long_mode()
		.center(0, 0);

	imgbtn_blueCounter_minus.init(obj_blueCounter_cont_1)
		.pos(193, 100)
		.size(65, 65)
		.checkable()
		.released_src(&_btn_RGB565A8_65x65)
		.pressed_src(&_btn_RGB565A8_65x65)
		.checked_released_src(&_btn_RGB565A8_65x65)
		.checked_pressed_src(&_btn_RGB565A8_65x65)
		.pad_all(0, LV_STATE_DEFAULT)
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

	label_blueCounter_minus_label.init(imgbtn_blueCounter_minus)
		.text("-")
		.long_mode()
		.center(0, 0);

	btn_blueCounter_btn_1.init(obj_blueCounter_cont_1)
		.pos(260, -42)
		.size(100, 50)
		.pad_all(0, LV_STATE_DEFAULT)
		.bg_color(lv_color_hex(0x2195f6))
		.bg_grad_dir(LV_GRAD_DIR_NONE)
		.radius(5)
		.text_font(&lv_font_montserratMedium_16);

	label_blueCounter_btn_1_label.init(btn_blueCounter_btn_1)
		.text("Button")
		.long_mode()
		.center(0, 0)
		.width(LV_PCT(100));

	img_blueCounter_logo.init(obj_blueCounter_cont_1)
		.pos(11, 10)
		.size(60, 34)
		.add_flag(LV_OBJ_FLAG_CLICKABLE)
		.src(&_NXP_Logo_RGB565A8_60x34)
		.pivot(50, 50)
		.image_recolor_opa(0)
		.image_opa(255);

	label_blueCounter_counter.init(obj_blueCounter_cont_1)
		.pos(58, 74)
		.size(114, 50)
		.text("0")
		.long_mode()
		.radius(8)
		.text_color(lv_color_hex(0x024f5a))
		.text_font(&lv_font_arial_40)
		.text_letter_space(2)
		.bg_opa(0);

	chekcbox_blueCounter_cb_1.init()
		.pos(32, 260)
		.text("checkbox")
		.text_color(lv_color_hex(0x0D3055))
		.text_font(&lv_font_montserratMedium_16)
		.text_letter_space(2)
		.radius(6)
		.bg_color(lv_color_hex(0xffffff))
		.bg_grad_dir(LV_GRAD_DIR_NONE)
		.pad_all(3, LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.border_width(2, LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.border_opa(255, LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.border_color(lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.border_side(LV_BORDER_SIDE_FULL, LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.radius(6, LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.bg_opa(255, LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.bg_color(lv_color_hex(0xffffff), LV_PART_INDICATOR|LV_STATE_DEFAULT)
		.bg_grad_dir(LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    }
    
    void events()
    {
    
    }
}