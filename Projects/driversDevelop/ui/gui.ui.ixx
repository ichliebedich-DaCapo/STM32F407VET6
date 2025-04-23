module;
#include <lvgl.h>
export module gui:ui;

export import widgets;

// ------加载资源------
//  字体资源
LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)
//  图片资源
LV_IMG_DECLARE(_dianzisheji_RGB565A8_61x42)

// ---------------- 定义组件 ----------------
export namespace gui::widgets::main
{
	using namespace gui::compose;

	inline 	Image img_screen_img_1;
	inline 	Chart chart_screen_chart_1;
	inline 	Button btn_screen_btn_1;
	inline 	Label label_screen_btn_1_label;
	inline 	Button btn_screen_btn_2;
	inline 	Label label_screen_btn_2_label;
	inline 	Button btn_screen_btn_3;
	inline 	Label label_screen_btn_3_label;
	inline 	Button btn_screen_btn_4;
	inline 	Label label_screen_btn_4_label;
	inline 	Button btn_screen_btn_5;
	inline 	Label label_screen_btn_5_label;
	inline 	Slider slider_screen_slider_1;
	inline 	Label label_screen_label_1;
	inline 	Label label_screen_label_2;
	inline 	Button btn_screen_btn_6;
	inline 	Label label_screen_btn_6_label;
	inline 	Button btn_screen_btn_7;
	inline 	Label label_screen_btn_7_label;
	inline 	Label label_screen_label_3;
	inline 	Scale scale_screen_scale_1;
/*!WIDGETS_DECLARE_BEGIN!*/

/*!WIDGETS_DECLARE_END!*/
}  

/*!USER_DECLARE_BEGIN!*/
/*!USER_DECLARE_BEGIN!*/




