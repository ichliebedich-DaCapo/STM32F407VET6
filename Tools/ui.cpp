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
        我要把lvgl代码转为特定的表达式，现在我准备定义一个函数，传入函数名和函数形参表两个参数，函数名是字符串类型，形参表是列表类型，
        已经做过处理，通过逗号分割并且去除空白符。比如“lv_obj_set_pos(ui->blueCounter_plus, 193, 15);”传入的参数是'lv_obj_set_pos'和['193','15']，前面一个参数会被丢弃不用考虑
        函数要做的事情是把函数名和形参表解析为'【特定函数名】(【形参(可选)】)'，具体的规则是：
        1，我把要处理的函数分成两部分，一部分是独立函数，也就是处理一次即可返回字符串；另一部分是组合函数，只要把参数信息存储到一个全局列表里，最后再定义一个函数来专门处理组合函数，返回的是None
        2，现在先讨论独立函数，需要根据函数名和参数表来生成特定的格式，这一部分我还没有想好，关系有些乱。总之，有的函数，形如lv_obj_set_style_pad_all，
        需要转换的特定函数名是'style_pad_all'，而【形参】则需要根据具体情况来决定是否省略，比如这个对于这个函数"lv_obj_set_style_pad_all(ui->blueCounter_plus, 0, LV_STATE_DEFAULT);"
        如果最后一个形参'LV_STATE_DEFAULT'，那么转换的形参表就可以把最后一个形参省略。如果倒数第二个形参为'0'并且最后一个形参是'LV_STATE_DEFAULT'，那么这两个形参都可以省略。其他情况都不能省略。
        有的函数比如'lv_obj_add_flag(ui->blueCounter_plus, LV_OBJ_FLAG_CHECKABLE);',如果最后一个形参是'LV_OBJ_FLAG_HIDDEN'等特定标志，那么转换的函数名就是hidden()，不含参；
        如果最后一个参数不是特定标志表里的，那么转换的函数名就是add_flag()，括号里还需要传入参数。
        3，从前面的表述你应该能看出来，不同函数名的转换规则不同，即使是同一个函数，其转换规则也会因为形参的不同而改变，而且还需要考虑到省略形参的情况。这些规则我不知道如何定义，方便后续扩展
        lv_obj_set_pos(ui->blueCounter_plus, 193, 15);
        lv_obj_set_size(ui->blueCounter_plus, 65, 65);
        lv_obj_add_flag(ui->blueCounter_plus, LV_OBJ_FLAG_CHECKABLE);
        lv_imagebutton_set_src(ui->blueCounter_plus, LV_IMAGEBUTTON_STATE_RELEASED, &_btn_RGB565A8_65x65, NULL, NULL);
        lv_imagebutton_set_src(ui->blueCounter_plus, LV_IMAGEBUTTON_STATE_PRESSED, &_btn_RGB565A8_65x65, NULL, NULL);
        lv_imagebutton_set_src(ui->blueCounter_plus, LV_IMAGEBUTTON_STATE_CHECKED_RELEASED, &_btn_RGB565A8_65x65, NULL, NULL);
        lv_imagebutton_set_src(ui->blueCounter_plus, LV_IMAGEBUTTON_STATE_CHECKED_PRESSED, &_btn_RGB565A8_65x65, NULL, NULL);
        lv_label_set_text(ui->blueCounter_plus_label, "+");
        lv_label_set_long_mode(ui->blueCounter_plus_label, LV_LABEL_LONG_WRAP);
        lv_obj_align(ui->blueCounter_plus_label, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_pad_all(ui->blueCounter_plus, 0, LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(ui->blueCounter_plus, &lv_font_arial_34, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(ui->blueCounter_plus, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_text_align(ui->blueCounter_plus, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(ui->blueCounter_plus, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    }
}