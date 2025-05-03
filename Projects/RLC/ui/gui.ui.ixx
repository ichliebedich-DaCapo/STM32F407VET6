module;
#include <lvgl.h>
export module gui:ui;
export import :render;

/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_END!*/

// ---------------- 资源声明 ----------------
extern "C" {
    // 字体声明
    LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)
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
    inline Image screen_img_1;
	inline Chart screen_chart_1;
	inline Button screen_btn_3;
	inline Label screen_btn_3_label;
	inline Label screen_label_Vi_data;
	inline Label screen_label_Vi;
	inline Label screen_label_Vq;
	inline Label screen_label_Vq_data;
	inline Label screen_label_C;
	inline Label screen_label_C_data;
	inline Label screen_label_R;
	inline Label screen_label_R_data;
	inline Label screen_label_L;
	inline Label screen_label_L_data;
	inline Label screen_label_x;
	inline Label screen_label_x_data;
	inline Label screen_label_y;
	inline Label screen_label_y_data;
	inline Label screen_label_1;
	inline Label screen_label_2;
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
		

		screen_img_1.init(screen)
			.pos(389,7)
			.size(83,55);
		lv_obj_add_flag(screen_img_1,LV_OBJ_FLAG_CLICKABLE);
		lv_image_set_src(screen_img_1,&_dianzisheji_RGB565A8_83x55);
		lv_image_set_pivot(screen_img_1,50,50);

		screen_chart_1.init(screen)
			.pos(2,79)
			.size(341,236)
			.bg_opa(255)
			.border_width(1)
			.border_color(lv_color_hex(0xFFFFFF))
			.line_width(2)
			.line_color(lv_color_hex(0xe8e8e8));
		lv_chart_set_type(screen_chart_1,LV_CHART_TYPE_LINE);
		lv_chart_set_div_line_count(screen_chart_1,11,15);
		lv_chart_set_point_count(screen_chart_1,5);

		screen_btn_3.init(screen)
			.pos(361,85)
			.size(51,32)
			.bg_opa(255)
			.bg_color(lv_color_hex(0x2195f6))
			.radius(5)
			.text_color(lv_color_hex(0xffffff))
			.text_align(LV_TEXT_ALIGN_CENTER);
		lv_obj_set_style_pad_all(screen_btn_3,0,LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_btn_3,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_btn_3_label.init(screen_btn_3)
			.center(0,0);
		lv_label_set_text(screen_btn_3_label,"测量");
		lv_obj_set_width(screen_btn_3_label,LV_PCT(100));

		screen_label_Vi_data.init(screen)
			.pos(418,230)
			.size(49,19)
			.bg_opa(0)
			.text_align(LV_TEXT_ALIGN_CENTER);
		lv_label_set_text(screen_label_Vi_data,"0");
		lv_obj_set_style_text_font(screen_label_Vi_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vi.init(screen)
			.pos(354,230)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vi,"Vi:");
		lv_obj_set_style_text_font(screen_label_Vi,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vq.init(screen)
			.pos(354,260)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vq,"Vq:");
		lv_obj_set_style_text_font(screen_label_Vq,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vq_data.init(screen)
			.pos(418,260)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vq_data,"0");
		lv_obj_set_style_text_font(screen_label_Vq_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_C.init(screen)
			.pos(354,200)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_C,"C:");
		lv_obj_set_style_text_font(screen_label_C,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_C_data.init(screen)
			.pos(418,200)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_C_data,"0");
		lv_obj_set_style_text_font(screen_label_C_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_R.init(screen)
			.pos(354,140)
			.size(52,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_R,"R:");
		lv_obj_set_style_text_font(screen_label_R,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_R_data.init(screen)
			.pos(418,170)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_R_data,"0");
		lv_obj_set_style_text_font(screen_label_R_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_L.init(screen)
			.pos(354,170)
			.size(52,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_L,"L:");
		lv_obj_set_style_text_font(screen_label_L,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_L_data.init(screen)
			.pos(418,140)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_L_data,"0");
		lv_obj_set_style_text_font(screen_label_L_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_x.init(screen)
			.pos(16,13)
			.size(13,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_x,"X:");
		lv_obj_set_style_text_font(screen_label_x,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_x_data.init(screen)
			.pos(40,13)
			.size(32,19)
			.text_align(LV_TEXT_ALIGN_LEFT)
			.bg_opa(0);
		lv_label_set_text(screen_label_x_data,"0");
		lv_obj_set_style_text_font(screen_label_x_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_y.init(screen)
			.pos(13,40)
			.size(24,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_y,"Y1:");
		lv_obj_set_style_text_font(screen_label_y,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_y_data.init(screen)
			.pos(40,40)
			.size(38,18)
			.text_align(LV_TEXT_ALIGN_LEFT)
			.bg_opa(0);
		lv_label_set_text(screen_label_y_data,"0");
		lv_obj_set_style_text_font(screen_label_y_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_1.init(screen)
			.pos(78,40)
			.size(22,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_1,"Y2:");
		lv_obj_set_style_text_font(screen_label_1,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_2.init(screen)
			.pos(105,40)
			.size(77,19)
			.text_align(LV_TEXT_ALIGN_LEFT)
			.bg_opa(0);
		lv_label_set_text(screen_label_2,"0");
		lv_obj_set_style_text_font(screen_label_2,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

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
