module;
#include <lvgl.h>
export module gui:ui;
export import :render;

/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/

// ---------------- 资源声明 ----------------
extern "C" {
    // 字体声明
    LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_13)
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
    // 图片声明
    LV_IMG_DECLARE(_dianzisheji_RGB565A8_83x55)
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
    // 其他
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}

// ---------------- 组件定义 ----------------
export namespace gui::widgets::main {
    using namespace gui::compose;
    inline Component screen;
	inline Component screen_img_1;
	inline Component screen_chart_1;
	inline Component screen_btn_3;
	inline Label screen_btn_3_label;
	inline Label screen_label_h2_data;
	inline Label screen_label_h2;
	inline Label screen_label_h3;
	inline Label screen_label_h3_data;
	inline Label screen_label_h4;
	inline Label screen_label_h4_data;
	inline Label screen_label_thd;
	inline Label screen_label_thd_data;
	inline Label screen_label_h5;
	inline Label screen_label_h5_data;
	inline Label screen_label_freq;
	inline Label screen_label_freq_data;
	inline Label screen_label_h1;
	inline Label screen_label_h1_data;
	inline Label screen_label_x;
	inline Label screen_label_x_data;
	inline Label screen_label_y;
	inline Label screen_label_y_data;
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}

// ================== 用户空间 ==================
/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/

// ---------------- UI接口 ----------------
export namespace gui::ui {
    using namespace gui::widgets::main;
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}

// ---------------- 初始化逻辑 ----------------
export namespace gui {
    void Render::screenInit() {
        using namespace gui::widgets::main;
        
        screen.init(nullptr)
			.bg_opa(255);
		lv_obj_set_size(screen,480,320);
		lv_obj_set_scrollbar_mode(screen,LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_style_bg_color(screen,lv_color_hex(0xffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(screen,LV_GRAD_DIR_NONE,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_update_layout(screen);

		screen_img_1.init(screen)
			.pos(389,7);
		lv_obj_set_size(screen_img_1,83,55);
		lv_obj_add_flag(screen_img_1,LV_OBJ_FLAG_CLICKABLE);
		lv_image_set_src(screen_img_1,&_dianzisheji_RGB565A8_83x55);
		lv_image_set_pivot(screen_img_1,50,50);
		lv_image_set_rotation(screen_img_1,0);
		lv_obj_set_style_image_recolor_opa(screen_img_1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_image_opa(screen_img_1,255,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_chart_1.init(screen)
			.pos(2,79)
			.bg_opa(255);
		lv_obj_set_size(screen_chart_1,341,236);
		lv_obj_set_scrollbar_mode(screen_chart_1,LV_SCROLLBAR_MODE_OFF);
		lv_chart_set_type(screen_chart_1,LV_CHART_TYPE_LINE);
		lv_chart_set_div_line_count(screen_chart_1,11,15);
		lv_chart_set_point_count(screen_chart_1,5);
		lv_chart_set_range(screen_chart_1,LV_CHART_AXIS_PRIMARY_Y,0,100);
		lv_chart_set_range(screen_chart_1,LV_CHART_AXIS_SECONDARY_Y,0,100);
		lv_obj_set_style_bg_color(screen_chart_1,lv_color_hex(0xffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(screen_chart_1,LV_GRAD_DIR_NONE,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(screen_chart_1,1,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(screen_chart_1,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(screen_chart_1,lv_color_hex(0xFFFFFF),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_side(screen_chart_1,LV_BORDER_SIDE_FULL,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_chart_1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_chart_1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_line_width(screen_chart_1,2,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_line_color(screen_chart_1,lv_color_hex(0xe8e8e8),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_line_opa(screen_chart_1,255,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_btn_3.init(screen)
			.pos(29,29)
			.bg_opa(255);
		lv_obj_set_size(screen_btn_3,51,32);
		lv_obj_set_style_pad_all(screen_btn_3,0,LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(screen_btn_3,lv_color_hex(0x2195f6),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(screen_btn_3,LV_GRAD_DIR_NONE,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_width(screen_btn_3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_btn_3,5,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_btn_3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_btn_3,lv_color_hex(0xffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_btn_3,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_btn_3,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_btn_3,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_btn_3_label.init(screen_btn_3);
		lv_label_set_text(screen_btn_3_label,"测量");
		lv_label_set_long_mode(screen_btn_3_label,LV_LABEL_LONG_WRAP);
		lv_obj_align(screen_btn_3_label,LV_ALIGN_CENTER,0,0);
		lv_obj_set_width(screen_btn_3_label,LV_PCT(100));

		screen_label_h2_data.init(screen)
			.pos(418,193)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h2_data,49,19);
		lv_label_set_text(screen_label_h2_data,"0");
		lv_label_set_long_mode(screen_label_h2_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h2_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h2_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h2_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h2_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h2_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h2.init(screen)
			.pos(354,193)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h2,49,19);
		lv_label_set_text(screen_label_h2,"H2/H1");
		lv_label_set_long_mode(screen_label_h2,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h2,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h2,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h2,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h2,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h2,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h3.init(screen)
			.pos(354,223)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h3,49,19);
		lv_label_set_text(screen_label_h3,"H3/H1");
		lv_label_set_long_mode(screen_label_h3,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h3,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h3,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h3,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h3,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h3,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h3_data.init(screen)
			.pos(418,223)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h3_data,49,19);
		lv_label_set_text(screen_label_h3_data,"0");
		lv_label_set_long_mode(screen_label_h3_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h3_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h3_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h3_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h3_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h3_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h4.init(screen)
			.pos(354,253)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h4,49,19);
		lv_label_set_text(screen_label_h4,"H4/H1");
		lv_label_set_long_mode(screen_label_h4,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h4,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h4,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h4,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h4,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h4,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h4_data.init(screen)
			.pos(418,253)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h4_data,49,19);
		lv_label_set_text(screen_label_h4_data,"0");
		lv_label_set_long_mode(screen_label_h4_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h4_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h4_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h4_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h4_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h4_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_thd.init(screen)
			.pos(354,163)
			.bg_opa(0);
		lv_obj_set_size(screen_label_thd,49,19);
		lv_label_set_text(screen_label_thd,"THD");
		lv_label_set_long_mode(screen_label_thd,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_thd,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_thd,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_thd,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_thd,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_thd,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_thd_data.init(screen)
			.pos(418,163)
			.bg_opa(0);
		lv_obj_set_size(screen_label_thd_data,49,19);
		lv_label_set_text(screen_label_thd_data,"0");
		lv_label_set_long_mode(screen_label_thd_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_thd_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_thd_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_thd_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_thd_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_thd_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h5.init(screen)
			.pos(354,283)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h5,49,19);
		lv_label_set_text(screen_label_h5,"H5/H1");
		lv_label_set_long_mode(screen_label_h5,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h5,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h5,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h5,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h5,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h5,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h5_data.init(screen)
			.pos(418,283)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h5_data,49,19);
		lv_label_set_text(screen_label_h5_data,"0");
		lv_label_set_long_mode(screen_label_h5_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h5_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h5_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h5_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h5_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h5_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_freq.init(screen)
			.pos(354,102)
			.bg_opa(0);
		lv_obj_set_size(screen_label_freq,52,19);
		lv_label_set_text(screen_label_freq,"采样率");
		lv_label_set_long_mode(screen_label_freq,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_freq,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_freq,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_freq,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_freq,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_freq,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_freq_data.init(screen)
			.pos(408,102)
			.bg_opa(0);
		lv_obj_set_size(screen_label_freq_data,68,19);
		lv_label_set_text(screen_label_freq_data,"0");
		lv_label_set_long_mode(screen_label_freq_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_freq_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_freq_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_freq_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_freq_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_freq_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h1.init(screen)
			.pos(354,132)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h1,52,19);
		lv_label_set_text(screen_label_h1,"频率");
		lv_label_set_long_mode(screen_label_h1,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h1,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h1,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h1,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h1,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h1,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h1_data.init(screen)
			.pos(418,135)
			.bg_opa(0);
		lv_obj_set_size(screen_label_h1_data,49,19);
		lv_label_set_text(screen_label_h1_data,"0");
		lv_label_set_long_mode(screen_label_h1_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_h1_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_h1_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_h1_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h1_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_h1_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_x.init(screen)
			.pos(255,19)
			.bg_opa(0);
		lv_obj_set_size(screen_label_x,13,19);
		lv_label_set_text(screen_label_x,"X:");
		lv_label_set_long_mode(screen_label_x,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_x,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_x,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_x,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_x,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_x,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_x_data.init(screen)
			.pos(274,19)
			.bg_opa(0);
		lv_obj_set_size(screen_label_x_data,71,19);
		lv_label_set_text(screen_label_x_data,"0");
		lv_label_set_long_mode(screen_label_x_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_x_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_x_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_x_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_x_data,LV_TEXT_ALIGN_LEFT,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_x_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_y.init(screen)
			.pos(255,48)
			.bg_opa(0);
		lv_obj_set_size(screen_label_y,13,19);
		lv_label_set_text(screen_label_y,"Y:");
		lv_label_set_long_mode(screen_label_y,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_y,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_y,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_y,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_y,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_y,0,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_y_data.init(screen)
			.pos(274,48)
			.bg_opa(0);
		lv_obj_set_size(screen_label_y_data,77,19);
		lv_label_set_text(screen_label_y_data,"0");
		lv_label_set_long_mode(screen_label_y_data,LV_LABEL_LONG_WRAP);
		lv_obj_set_style_border_width(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_radius(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_color(screen_label_y_data,lv_color_hex(0x000000),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_label_y_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_opa(screen_label_y_data,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_letter_space(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_line_space(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_y_data,LV_TEXT_ALIGN_LEFT,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_top(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_right(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_left(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_shadow_width(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_pad_bottom(screen_label_y_data,0,LV_PART_MAIN|LV_STATE_DEFAULT);

        /*!USER_DECLARE_BEGIN!*/
        /*!USER_DECLARE_END!*/
    }
    
    void Render::eventInit() {
        using namespace gui::widgets::main;
        /*!USER_DECLARE_BEGIN!*/
        /*!USER_DECLARE_END!*/
    }
}

/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/
