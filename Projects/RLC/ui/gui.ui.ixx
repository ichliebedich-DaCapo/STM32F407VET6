module;
#include <lvgl.h>
#include <cstring>
#include <cstdio>
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
	inline Label screen_label_Vi_50k_data;
	inline Label screen_label_Vi;
	inline Label screen_label_Vq;
	inline Label screen_label_Vq_50k_data;
	inline Label screen_label_C;
	inline Label screen_label_C_data;
	inline Label screen_label_R;
	inline Label screen_label_L_data;
	inline Label screen_label_L;
	inline Label screen_label_R_data;
	inline Label screen_label_x;
	inline Label screen_label_x_data;
	inline Label screen_label_y;
	inline Label screen_label_y_data;
	inline Label screen_label_1;
	inline Label screen_label_2;
	inline Label screen_label_3;
	inline Label screen_label_4;
	inline Label screen_label_Vi_1k_data;
	inline Label screen_label_Vq_1k_data;
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}

// ================== 用户空间 ==================
/*!USER_DECLARE_BEGIN!*/
// 类声明
using namespace gui::widgets::main;
class RLC
{
public:
    static auto initChartComponent() -> void;
    // 生成随机数据
    static inline auto generate_data(int32_t* pool)->void;
    static inline auto generate_text(float Vi_50k,float Vq_50k,float Vq_1k,float Vi_1k,float R,float L,float C)->void;
    static inline auto clear_text()->void;
    static inline auto toggle_generation() -> void;
    static inline auto get_generate_state() -> bool;
    static inline auto get_chart_type() -> int;
public:
    static inline auto set_cursor_on_press()->void
    {
        screen_chart_1.set_cursor_pos_on_pressed(cursor);
//        if(had_generated)
//        {
//            char buf[10];
//            lv_snprintf(buf, sizeof(buf), "%d %d",chart_screen_chart_1.get_pressed_point(), chart_screen_chart_1.get_cursor_point_y(series));//格式化点数值成字符串
//            label_screen_label_1.text(buf);
//        }
    }

    static inline bool had_generated = false;
private:
    // 添加生成状态标志
    static inline bool is_generating = false;
    static  inline  ChartSeries_t series{}; //数据 系列
    static  inline  ChartCursor_t cursor{}; //光标 系列1
    static  inline  lv_point_t cursor_point{}; //光标 系列1
    static  inline constexpr   uint32_t point_count=24;
    static  inline int chart_type=0;
};
// 类声明
/*!USER_DECLARE_END!*/

// ---------------- UI接口 ----------------
export namespace gui::ui {
    using namespace gui::widgets::main;
    /*!USER_DECLARE_BEGIN!*/
    // 生成图表数据外部接口
    void gui_generate_chart_data(int32_t* data_pool)
    {
        RLC::generate_data(data_pool);
    }
    // 生成谐波数据外部接口
    void gui_generate_text_data(float* text_data,float thd,float div,float fundamental_freq)
    {
//        RLC::generate_text(text_data,thd,div,fundamental_freq);
    }
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
			.pos(354,79)
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

		screen_label_Vi_50k_data.init(screen)
			.pos(418,212)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vi_50k_data,"0");
		lv_obj_set_style_text_font(screen_label_Vi_50k_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vi.init(screen)
			.pos(354,212)
			.size(57,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vi,"Vi_50k:");
		lv_obj_set_style_text_font(screen_label_Vi,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vq.init(screen)
			.pos(354,242)
			.size(57,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vq,"Vq_50k:");
		lv_obj_set_style_text_font(screen_label_Vq,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vq_50k_data.init(screen)
			.pos(418,242)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vq_50k_data,"0");
		lv_obj_set_style_text_font(screen_label_Vq_50k_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_C.init(screen)
			.pos(354,182)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_C,"C:");
		lv_obj_set_style_text_font(screen_label_C,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_C_data.init(screen)
			.pos(418,182)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_C_data,"0");
		lv_obj_set_style_text_font(screen_label_C_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_R.init(screen)
			.pos(354,122)
			.size(52,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_R,"R:");
		lv_obj_set_style_text_font(screen_label_R,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_L_data.init(screen)
			.pos(418,152)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_L_data,"0");
		lv_obj_set_style_text_font(screen_label_L_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_L.init(screen)
			.pos(354,152)
			.size(52,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_L,"L:");
		lv_obj_set_style_text_font(screen_label_L,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_R_data.init(screen)
			.pos(418,122)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_R_data,"0");
		lv_obj_set_style_text_font(screen_label_R_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

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

		screen_label_3.init(screen)
			.pos(354,272)
			.size(57,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_3,"Vi_1k:");
		lv_obj_set_style_text_font(screen_label_3,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_4.init(screen)
			.pos(354,300)
			.size(57,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_4,"Vq_1k:");
		lv_obj_set_style_text_font(screen_label_4,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vi_1k_data.init(screen)
			.pos(418,272)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vi_1k_data,"0");
		lv_obj_set_style_text_font(screen_label_Vi_1k_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_Vq_1k_data.init(screen)
			.pos(418,300)
			.size(49,19)
			.text_align(LV_TEXT_ALIGN_CENTER)
			.bg_opa(0);
		lv_label_set_text(screen_label_Vq_1k_data,"0");
		lv_obj_set_style_text_font(screen_label_Vq_1k_data,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

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
using namespace gui::widgets::main;
// 类定义
auto RLC::initChartComponent() -> void
{

    series=screen_chart_1.update_mode(LV_CHART_UPDATE_MODE_SHIFT)  // 改为SHIFT模式
            .line_color(lv_color_hex(0x000000))
            .point_count(point_count)
            .remove_dot()
            .add_series(lv_color_hex(0x34e6ff))
            ;

    screen_chart_1
            .line_color(lv_color_hex(0x808080))
            .bg_color(lv_color_hex(0x000000))
            .range(LV_CHART_AXIS_PRIMARY_Y, 0, 4095)
            ;

    cursor=screen_chart_1.add_cursor(lv_color_hex(0x0bce11),LV_DIR_ALL);
//    scale_screen_scale_1.border_opa(0);
}
auto RLC::generate_data(int32_t* pool)->void
{
    screen_chart_1.ext_y_array(series, pool);
}
auto RLC::generate_text(float Vi_50k,float Vq_50k,float Vq_1k,float Vi_1k,float R,float L,float C)->void
{
    char vi_50k[10];
    char vq_50k[10];
    char vi_1k[10];
    char vq_1k[10];
    char r[10];
    char l[10];
    char c[10];

//    lv_snprintf(bufthd, sizeof(bufthd), "%.4f",thd);//格式化点数值成字符串
//    lv_snprintf(buf1, sizeof(buf1), "",normal_pool[1]);//格式化点数值成字符串
//    lv_snprintf(buf2, sizeof(buf2), "%.4f",normal_pool[2]);//格式化点数值成字符串
//    lv_snprintf(buf3, sizeof(buf3), "%.4f",normal_pool[3]);//格式化点数值成字符串
//    lv_snprintf(buf4, sizeof(buf4), "%.4f",normal_pool[4]);//格式化点数值成字符串

    sprintf(vi_50k, "%.1f", Vi_50k);          // 格式化 thd，保留四位小数
    sprintf(vq_50k, "%.1f", Vq_50k);          // 格式化 div, 保留一位小数
    sprintf(vi_1k, "%.1f", Vq_1k);          // 格式化 thd，保留四位小数
    sprintf(vq_1k, "%.1f", Vi_1k);          // 格式化 div, 保留一位小数
    sprintf(r,  "%.1f", R);           // 格式化 normal_pool[1]，保留四位小数
    sprintf(l,  "%.1f", L);           // 格式化 normal_pool[1]，保留四位小数
    sprintf(c,  "%.1f", C);           // 格式化 normal_pool[2]，保留四位小数

    screen_label_Vi_50k_data.text(vi_50k);
    screen_label_Vq_50k_data.text(vq_50k);
    screen_label_Vi_1k_data.text( vi_1k);
    screen_label_Vq_1k_data.text( vq_1k);
    screen_label_R_data.text(r);
    screen_label_L_data.text(l);
    screen_label_C_data.text(c);
}
auto RLC::clear_text()->void
{
    screen_label_Vi_50k_data.text("-");
    screen_label_Vq_50k_data.text("-");
    screen_label_Vi_1k_data.text( "-");
    screen_label_Vq_1k_data.text( "-");
    screen_label_R_data.text("-");
    screen_label_L_data.text("-");
    screen_label_C_data.text("-");
}

// 新增切换生成状态的方法
auto RLC::toggle_generation() -> void
{
    is_generating = !is_generating;
    had_generated=true;

}

// 获取图表类型
auto RLC::get_chart_type() -> int
{
    return chart_type;
}
// 获取生成数据状态
auto RLC::get_generate_state() -> bool
{
    return is_generating ;
}
/*!USER_DECLARE_END!*/
