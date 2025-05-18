module;
#include <lvgl.h>
#include <array>
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
    inline Image screen_img_icon;
	inline Chart screen_chart;
	inline Button screen_btn_reset;
	inline Label screen_btn_reset_label;
	inline Label screen_label_ZI;
	inline Label screen_label_ZQ;
	inline Label screen_label_C;
	inline Label screen_label_R;
	inline Label screen_label_L;
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
// 类声明
using namespace gui::widgets::main;


// 类声明
/*!USER_DECLARE_END!*/

// ---------------- UI接口 ----------------
export namespace gui
{
    using namespace gui::widgets::main;
    /*!USER_DECLARE_BEGIN!*/
    class RLC
    {
    public:
        static auto initChartComponent(){
            screen_chart.update_mode(LV_CHART_UPDATE_MODE_SHIFT)  // 改为SHIFT模式
                    .line_color(lv_color_hex(0x000000))
                    .point_count(point_count)
                    .remove_dot();

            series_1 =screen_chart.add_series(lv_color_hex(0x34e6ff));
            series_2 =screen_chart.add_series(lv_color_hex(0xFFD700));

            screen_chart.line_color(lv_color_hex(0x808080))
                    .bg_color(lv_color_hex(0x000000))
                    .range(LV_CHART_AXIS_PRIMARY_Y, -32768, 32768);

            cursor = screen_chart.add_cursor(lv_color_hex(0x0bce11), LV_DIR_ALL);
        //    scale_screen_scale_1.border_opa(0);
        }
        // 生成随机数据
        static inline auto generate_data(int32_t* pool1,  int32_t* pool2,uint16_t size)
        {
            if(size != point_count)
            {
                screen_chart.point_count(size);
            }
            screen_chart.ext_y_array(series_1, pool1);
            screen_chart.ext_y_array(series_2, pool2);
        }


        static  auto generate_text(float VI_VZ,float VQ_VZ,float R, float L, float C)
        {
            char buffer[10]; // 单个缓冲区复用

            // 辅助lambda，捕获buffer，自动推导标签类型
            auto set_label = [&buffer](auto& label, const char* fmt, float value) {
                sprintf(buffer, fmt, value);
                label.text(buffer); // 假设text内部复制字符串而非保存指针
            };

            set_label(screen_label_ZI, "VIz: %.3fV", VI_VZ*1.024/32768);
            set_label(screen_label_ZQ, "VQz: %.3fV", VQ_VZ*1.024/32768);
            set_label(screen_label_R, "R: %.1fΩ", R);

            //检测电感是什么单位
            if(L>1e-3&&L<1e-1)
            {
                set_label(screen_label_L, "L: %.3fmH", L * 1e3);
            }
            else if(L>1e-6&&L<1e-3)
            {
                set_label(screen_label_L, "L: %.3fμH", L * 1e6);
            }
            else if(L>1e-9&&L<1e-6)
            {
                set_label(screen_label_L, "L: %.3fnH", L * 1e9);
            }
            else if(L>1e-12&&L<1e-9)
            {
                set_label(screen_label_L, "L: %.3fpH", L * 1e12);
            }
            else
            {
                set_label(screen_label_L, "L: %.3fH", L);
            }

            //检测电容是什么单位
            if(C>1e-3&&C<1e-1)
            {
                set_label(screen_label_C, "C: %.3fmF", C * 1e3);
            }
            else if(C>1e-6&&C<1e-3)
            {
                set_label(screen_label_C, "C: %.3fμF", C * 1e6);
            }
            else if(C>1e-9&&C<1e-6)
            {
                set_label(screen_label_C, "C: %.3fnF", C * 1e9);
            }
            else if(C>1e-12&&C<1e-9)
            {
                set_label(screen_label_C, "C: %.3fpF", C * 1e12);
            }
            else
            {
                set_label(screen_label_C, "C: %.3fF", C);
            }
        }

        static inline auto clear_text(){
            screen_label_ZI.text("-");
            screen_label_ZQ.text("-");
            screen_label_R.text("-");
            screen_label_L.text("-");
            screen_label_C.text("-");
        }
        static inline auto toggle_generation(){
            is_generating = !is_generating;
            had_generated = true;
        }
        static inline auto get_generate_state() {return is_generating;}
        static inline auto get_chart_type() {return chart_type;}

        static inline auto set_reset_callback(void (*reset_ptr)())->void
        {
            reset = reset_ptr;
        }
        static inline auto reset_call()
        {
            reset();
        }
    public:
        static inline auto set_cursor_on_press()->void
        {
            screen_chart.set_cursor_pos_on_pressed(cursor);
//        if(had_generated)
//        {
//            char buf[10];
//            lv_snprintf(buf, sizeof(buf), "%d %d",chart_screen_chart.get_pressed_point(), chart_screen_chart.get_cursor_point_y(series));//格式化点数值成字符串
//            label_screen_label_1.text(buf);
//        }
        }

        static inline bool had_generated = false;
    private:
        // 添加生成状态标志
        static inline bool is_generating = false;
        static  inline  ChartSeries_t series_1{}; //数据 系列
        static  inline  ChartSeries_t series_2{};
        static  inline  ChartCursor_t cursor{}; //光标 系列1
        static  inline  lv_point_t cursor_point{}; //光标 系列1
        static  inline constexpr   uint32_t point_count=4;
        static  inline int chart_type=0;
        static inline void (* reset)();
    };
    /*!USER_DECLARE_END!*/
}

// ---------------- 初始化逻辑 ----------------
export namespace gui {
    void Render::screenInit() {
        using namespace gui::widgets::main;
        
        screen.init(nullptr)
			.size(480,320)
			.bg_opa(255);
		

		screen_img_icon.init(screen)
			.pos(389,7)
			.size(83,55);
		lv_obj_add_flag(screen_img_icon,LV_OBJ_FLAG_CLICKABLE);
		lv_image_set_src(screen_img_icon,&_dianzisheji_RGB565A8_83x55);
		lv_image_set_pivot(screen_img_icon,50,50);

		screen_chart.init(screen)
			.pos(2,79)
			.size(341,236)
			.bg_opa(255)
			.border_width(1)
			.border_color(lv_color_hex(0xFFFFFF))
			.line_width(2)
			.line_color(lv_color_hex(0xe8e8e8));
		lv_chart_set_type(screen_chart,LV_CHART_TYPE_LINE);
		lv_chart_set_div_line_count(screen_chart,11,15);
		lv_chart_set_point_count(screen_chart,5);

		screen_btn_reset.init(screen)
			.pos(354,79)
			.size(51,32)
			.bg_opa(255)
			.bg_color(lv_color_hex(0x2195f6))
			.radius(5)
			.text_color(lv_color_hex(0xffffff))
			.text_align(LV_TEXT_ALIGN_CENTER);
		lv_obj_set_style_pad_all(screen_btn_reset,0,LV_STATE_DEFAULT);
		lv_obj_set_style_text_font(screen_btn_reset,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_btn_reset_label.init(screen_btn_reset)
			.center(0,0);
		lv_label_set_text(screen_btn_reset_label,"校准");
		lv_obj_set_width(screen_btn_reset_label,LV_PCT(100));

		screen_label_C.init(screen)
			.pos(350,182)
			.size(120,19)
			.text_align(LV_TEXT_ALIGN_LEFT)
			.bg_opa(0);
		lv_label_set_text(screen_label_C,"C:");
		lv_obj_set_style_text_font(screen_label_C,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_R.init(screen)
			.pos(350,122)
			.size(120,19)
			.text_align(LV_TEXT_ALIGN_LEFT)
			.bg_opa(0);
		lv_label_set_text(screen_label_R,"R:");
		lv_obj_set_style_text_font(screen_label_R,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

		screen_label_L.init(screen)
			.pos(350,152)
			.size(120,19)
			.text_align(LV_TEXT_ALIGN_LEFT)
			.bg_opa(0);
		lv_label_set_text(screen_label_L,"L:");
		lv_obj_set_style_text_font(screen_label_L,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);

        screen_label_ZI.init(screen)
                .pos(350,212)
                .size(120,19)
                .text_align(LV_TEXT_ALIGN_LEFT)
                .bg_opa(0);
        lv_label_set_text(screen_label_ZI,"ZI:");
        lv_obj_set_style_text_font(screen_label_ZI,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);


        screen_label_ZQ.init(screen)
                .pos(350,242)
                .size(120,19)
                .text_align(LV_TEXT_ALIGN_LEFT)
                .bg_opa(0);
        lv_label_set_text(screen_label_ZQ,"ZQ:");
        lv_obj_set_style_text_font(screen_label_ZQ,&lv_customer_font_SourceHanSerifSC_Regular_13,selector_default);


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
        RLC::initChartComponent();
        /*!USER_DECLARE_END!*/
    }
    
    void Render::eventInit() {
        using namespace gui::widgets::main;
        /*!USER_DECLARE_BEGIN!*/
        screen_btn_reset.OnClicked([](lv_event_t * e){
            RLC::reset_call();
        });
        screen_chart.OnPressed<RLC::set_cursor_on_press>();
        /*!USER_DECLARE_END!*/
    }
}

/*!USER_DECLARE_BEGIN!*/
using namespace gui::widgets::main;
namespace gui
{
// 类定义

}
/*!USER_DECLARE_END!*/
