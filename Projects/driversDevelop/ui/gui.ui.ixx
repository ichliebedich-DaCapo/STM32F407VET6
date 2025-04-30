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
			.size(480,320)
			.bg_opa(255);
		lv_obj_set_scrollbar_mode(screen,LV_SCROLLBAR_MODE_OFF);
		lv_obj_set_style_bg_color(screen,lv_color_hex(0xffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(screen,LV_GRAD_DIR_NONE,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_img_1.init(screen)
			.pos(389,7)
			.size(83,55);
		lv_obj_add_flag(screen_img_1,LV_OBJ_FLAG_CLICKABLE);
		lv_image_set_src(screen_img_1,&_dianzisheji_RGB565A8_83x55);
		lv_image_set_pivot(screen_img_1,50,50);
		lv_image_set_rotation(screen_img_1,0);
		lv_obj_set_style_image_recolor_opa(screen_img_1,0,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_image_opa(screen_img_1,255,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_chart_1.init(screen)
			.pos(2,79)
			.size(341,236)
			.bg_opa(255)
			.border_width(1);
		lv_obj_set_scrollbar_mode(screen_chart_1,LV_SCROLLBAR_MODE_OFF);
		lv_chart_set_type(screen_chart_1,LV_CHART_TYPE_LINE);
		lv_chart_set_div_line_count(screen_chart_1,11,15);
		lv_chart_set_point_count(screen_chart_1,5);
		lv_chart_set_range(screen_chart_1,LV_CHART_AXIS_PRIMARY_Y,0,100);
		lv_chart_set_range(screen_chart_1,LV_CHART_AXIS_SECONDARY_Y,0,100);
		lv_obj_set_style_bg_color(screen_chart_1,lv_color_hex(0xffffff),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(screen_chart_1,LV_GRAD_DIR_NONE,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_opa(screen_chart_1,255,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_color(screen_chart_1,lv_color_hex(0xFFFFFF),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_border_side(screen_chart_1,LV_BORDER_SIDE_FULL,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_line_width(screen_chart_1,2,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_line_color(screen_chart_1,lv_color_hex(0xe8e8e8),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_line_opa(screen_chart_1,255,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_btn_3.init(screen)
			.pos(29,29)
			.size(51,32)
			.bg_opa(255)
			.radius(5)
			.text_color(lv_color_hex(0xffffff));
		lv_obj_set_style_pad_all(screen_btn_3,0,LV_STATE_DEFAULT);
		lv_obj_set_style_bg_color(screen_btn_3,lv_color_hex(0x2195f6),LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_bg_grad_dir(screen_btn_3,LV_GRAD_DIR_NONE,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_btn_3,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_btn_3,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_btn_3_label.init(screen_btn_3)
			.center(0,0);
		lv_label_set_text(screen_btn_3_label,"测量");
		lv_obj_set_width(screen_btn_3_label,LV_PCT(100));

		screen_label_h2_data.init(screen)
			.pos(418,193)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h2_data,"0");
		lv_obj_set_style_text_font(screen_label_h2_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h2_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h2.init(screen)
			.pos(354,193)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h2,"H2/H1");
		lv_obj_set_style_text_font(screen_label_h2,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h2,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h3.init(screen)
			.pos(354,223)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h3,"H3/H1");
		lv_obj_set_style_text_font(screen_label_h3,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h3,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h3_data.init(screen)
			.pos(418,223)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h3_data,"0");
		lv_obj_set_style_text_font(screen_label_h3_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h3_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h4.init(screen)
			.pos(354,253)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h4,"H4/H1");
		lv_obj_set_style_text_font(screen_label_h4,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h4,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h4_data.init(screen)
			.pos(418,253)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h4_data,"0");
		lv_obj_set_style_text_font(screen_label_h4_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h4_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_thd.init(screen)
			.pos(354,163)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_thd,"THD");
		lv_obj_set_style_text_font(screen_label_thd,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_thd,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_thd_data.init(screen)
			.pos(418,163)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_thd_data,"0");
		lv_obj_set_style_text_font(screen_label_thd_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_thd_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h5.init(screen)
			.pos(354,283)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h5,"H5/H1");
		lv_obj_set_style_text_font(screen_label_h5,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h5,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h5_data.init(screen)
			.pos(418,283)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h5_data,"0");
		lv_obj_set_style_text_font(screen_label_h5_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h5_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_freq.init(screen)
			.pos(354,102)
			.size(52,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_freq,"采样率");
		lv_obj_set_style_text_font(screen_label_freq,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_freq,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_freq_data.init(screen)
			.pos(408,102)
			.size(68,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_freq_data,"0");
		lv_obj_set_style_text_font(screen_label_freq_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_freq_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h1.init(screen)
			.pos(354,132)
			.size(52,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h1,"频率");
		lv_obj_set_style_text_font(screen_label_h1,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h1,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_h1_data.init(screen)
			.pos(418,135)
			.size(49,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_h1_data,"0");
		lv_obj_set_style_text_font(screen_label_h1_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_h1_data,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_x.init(screen)
			.pos(255,19)
			.size(13,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_x,"X:");
		lv_obj_set_style_text_font(screen_label_x,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_x,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_x_data.init(screen)
			.pos(274,19)
			.size(71,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_x_data,"0");
		lv_obj_set_style_text_font(screen_label_x_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_x_data,LV_TEXT_ALIGN_LEFT,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_y.init(screen)
			.pos(255,48)
			.size(13,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_y,"Y:");
		lv_obj_set_style_text_font(screen_label_y,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_y,LV_TEXT_ALIGN_CENTER,LV_PART_MAIN|LV_STATE_DEFAULT);

		screen_label_y_data.init(screen)
			.pos(274,48)
			.size(77,19)
			.bg_opa(0);
		lv_label_set_text(screen_label_y_data,"0");
		lv_obj_set_style_text_font(screen_label_y_data,&lv_font_SourceHanSerifSC_Regular_13,LV_PART_MAIN|LV_STATE_DEFAULT);
		lv_obj_set_style_text_align(screen_label_y_data,LV_TEXT_ALIGN_LEFT,LV_PART_MAIN|LV_STATE_DEFAULT);

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
