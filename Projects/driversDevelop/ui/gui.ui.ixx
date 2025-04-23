module;
#include <lvgl.h>
export module gui:ui;
export import :render;
// 导入其他资源
/*!USER_DECLARE_BEGIN!*/
import ui_data;
/*!USER_DECLARE_END!*/

// ---------------- 导出并加载资源 ----------------
extern "C"
{
    //  字体资源
    LV_FONT_DECLARE(lv_customer_font_SourceHanSerifSC_Regular_13)
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
    //  图片资源
    LV_IMG_DECLARE(_dianzisheji_RGB565A8_61x42)
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
    // 其他
    /*!USER_DECLARE_BEGIN!*/
    /*!USER_DECLARE_END!*/
}


// ---------------- 导出并定义组件 ----------------
export namespace gui::widgets::main
{
    using namespace gui::compose;

    inline Image img_screen_img_1;
    inline Chart chart_screen_chart_1;
    inline Button btn_screen_btn_1;
    inline Label label_screen_btn_1_label;
    inline Button btn_screen_btn_2;
    inline Label label_screen_btn_2_label;
    inline Button btn_screen_btn_3;
    inline Label label_screen_btn_3_label;
    inline Button btn_screen_btn_4;
    inline Label label_screen_btn_4_label;
    inline Button btn_screen_btn_5;
    inline Label label_screen_btn_5_label;
    inline Slider slider_screen_slider_1;
    inline Label label_screen_label_1;
    inline Label label_screen_label_2;
    inline Button btn_screen_btn_6;
    inline Label label_screen_btn_6_label;
    inline Button btn_screen_btn_7;
    inline Label label_screen_btn_7_label;
    inline Label label_screen_label_3;
    inline Scale scale_screen_scale_1;
    /*!USER_DECLARE_BEGIN!*/
    Timer updata_timer;
    /*!USER_DECLARE_END!*/
}

// ======================= 用户匿名空间 =======================
/*!USER_DECLARE_BEGIN!*/
// 全局变量定义
inline uint8_t length = 200;
inline uint8_t wave_start_index = 0;
inline uint16_t array_length = 400;
inline size_t current_index = 0; // 当前读取位置
inline uint8_t count = 0;


/*!USER_DECLARE_END!*/
// ======================= 用户匿名空间 =======================


// ---------------- 导出用户接口 ----------------
export namespace gui::ui
{
    using namespace gui::widgets::main; // 使用组件命名空间

    /*!USER_DECLARE_BEGIN!*/
    // 类声明
    class Osc
    {
    public:
        static auto initChartComponent() -> void;

        // 生成随机数据
        static inline auto generate_data() -> void;

        static inline auto toggle_generation() -> void;

        // 新增定时器回调函数
        static void timer_cb(lv_timer_t *timer)
        {
            if (is_generating)
            {
                generate_data();
            }
        }

    public:
        static inline auto set_cursor_on_press() -> void
        {
            chart_screen_chart_1.set_cursor_pos_on_pressed(cursor);
            if (had_generated)
            {
                char buf[10];
                lv_snprintf(buf, sizeof(buf), "%d %d", chart_screen_chart_1.get_pressed_point(),
                            chart_screen_chart_1.get_cursor_point_y(series)); //格式化点数值成字符串
                label_screen_label_1.text(buf);
            }
        }

        static inline bool had_generated = false;

    private:
        // 添加生成状态标志
        static inline bool is_generating = false;
        static inline ChartSeries_t series{}; //数据 系列1
        static inline ChartCursor_t cursor{}; //光标 系列1
        static inline lv_point_t cursor_point{}; //光标 系列1
    };

    /*!USER_DECLARE_END!*/
}


// ---------------- 初始化UI和事件 ----------------
export namespace gui
{
    void Render::screenInit()
    {
        using namespace gui::widgets::main; // 使用组件命名空间

        scr.bg_color(lv_color_hex(0xffffff))
                .bg_grad_dir(LV_GRAD_DIR_NONE);

        img_screen_img_1.init()
                .pos(405, 5)
                .size(61, 42)
                .add_flag(LV_OBJ_FLAG_CLICKABLE)
                .src(&_dianzisheji_RGB565A8_61x42)
                .pivot(50, 50)
                .image_recolor_opa(0);

        chart_screen_chart_1.init()
                .pos(18, 8)
                .size(375, 227)
                .scrollbar_mode(LV_SCROLLBAR_MODE_OFF)
                .div_count(11, 15)
                .point_count(5)
                .range(LV_CHART_AXIS_PRIMARY_Y)
                .range(LV_CHART_AXIS_SECONDARY_Y)
                .bg_color(lv_color_hex(0xffffff))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .border_width(1)
                .border_opa(255)
                .border_color(lv_color_hex(0xe8e8e8))
                .border_side(LV_BORDER_SIDE_FULL)
                .radius(0)
                .line_width(2)
                .line_color(lv_color_hex(0xe8e8e8));

        btn_screen_btn_1.init()
                .pos(409, 59)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_btn_1_label.init(btn_screen_btn_1)
                .text("BB")
                .center()
                .width(LV_PCT(100));

        btn_screen_btn_2.init()
                .pos(409, 129)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_btn_2_label.init(btn_screen_btn_2)
                .text("Button")
                .center()
                .width(LV_PCT(100));

        btn_screen_btn_3.init()
                .pos(18, 265)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_btn_3_label.init(btn_screen_btn_3)
                .text("Button")
                .center()
                .width(LV_PCT(100));

        btn_screen_btn_4.init()
                .pos(115, 265)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_btn_4_label.init(btn_screen_btn_4)
                .text("Button")
                .center()
                .width(LV_PCT(100));

        btn_screen_btn_5.init()
                .pos(215, 265)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_btn_5_label.init(btn_screen_btn_5)
                .text("Button")
                .center()
                .width(LV_PCT(100));

        slider_screen_slider_1.init()
                .pos(18, 246)
                .size(370, 5)
                .range(0, 100)
                .value(50)
                .bg_opa(60)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(50)
                .outline_width(0)
                .bg_opa(255, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .bg_color(lv_color_hex(0x2195f6),
                          static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .bg_grad_dir(LV_GRAD_DIR_NONE, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .radius(50, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .bg_opa(255, static_cast<int>(LV_PART_KNOB) | static_cast<int>(LV_STATE_DEFAULT))
                .bg_color(lv_color_hex(0x2195f6), static_cast<int>(LV_PART_KNOB) | static_cast<int>(LV_STATE_DEFAULT))
                .bg_grad_dir(LV_GRAD_DIR_NONE, static_cast<int>(LV_PART_KNOB) | static_cast<int>(LV_STATE_DEFAULT))
                .radius(50, static_cast<int>(LV_PART_KNOB) | static_cast<int>(LV_STATE_DEFAULT));

        label_screen_label_1.init()
                .pos(409, 97)
                .size(49, 19)
                .text("Label")
                .bg_opa(0)
                .radius(0)
                .text_color(lv_color_hex(0x000000))
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_label_2.init()
                .pos(409, 174)
                .size(52, 25)
                .text("Label")
                .radius(0)
                .text_color(lv_color_hex(0x000000))
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER)
                .bg_opa(0);

        btn_screen_btn_6.init()
                .pos(316, 265)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE);

        label_screen_btn_6_label.init(btn_screen_btn_6)
                .text("Button")
                .center()
                .width(LV_PCT(100));

        btn_screen_btn_7.init()
                .pos(409, 205)
                .size(54, 28)
                .pad_all(0, LV_STATE_DEFAULT)
                .bg_color(lv_color_hex(0x2195f6))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .radius(5)
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        label_screen_btn_7_label.init(btn_screen_btn_7)
                .text("Button")
                .center()
                .width(LV_PCT(100));

        label_screen_label_3.init()
                .pos(409, 246)
                .size(52, 25)
                .text("Label")
                .bg_opa(0)
                .radius(0)
                .text_color(lv_color_hex(0x000000))
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .text_align(LV_TEXT_ALIGN_CENTER);

        scale_screen_scale_1.init()
                .pos(0, 8)
                .size(20, 227)
                .mode(LV_SCALE_MODE_VERTICAL_RIGHT)
                .total_tick_count(25)
                .major_tick_every(8)
                .range(0, 255)
                .radius(0)
                .text_color(lv_color_hex(0x0D3055))
                .font(&lv_customer_font_SourceHanSerifSC_Regular_13)
                .bg_color(lv_color_hex(0xffffff))
                .bg_grad_dir(LV_GRAD_DIR_NONE)
                .line_width(2)
                .line_color(lv_color_hex(0x757575))
                .length(5, static_cast<int>(LV_PART_ITEMS) | static_cast<int>(LV_STATE_DEFAULT))
                .line_width(2, static_cast<int>(LV_PART_ITEMS) | static_cast<int>(LV_STATE_DEFAULT))
                .line_color(lv_color_hex(0x757575),
                            static_cast<int>(LV_PART_ITEMS) | static_cast<int>(LV_STATE_DEFAULT))
                .line_opa(255, static_cast<int>(LV_PART_ITEMS) | static_cast<int>(LV_STATE_DEFAULT))
                .line_rounded(true, static_cast<int>(LV_PART_ITEMS) | static_cast<int>(LV_STATE_DEFAULT))
                .length(10, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .line_width(2, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .line_color(lv_color_hex(0x757575),
                            static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .line_opa(255, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT))
                .line_rounded(true, static_cast<int>(LV_PART_INDICATOR) | static_cast<int>(LV_STATE_DEFAULT));
        /*!USER_DECLARE_BEGIN!*/
        ui::Osc::initChartComponent();
        /*!USER_DECLARE_END!*/
    }

    void Render::eventInit()
    {
        using namespace gui::widgets::main; // 使用组件命名空间
        /*!USER_DECLARE_BEGIN!*/
        // 绑定 随机生成数据事件
        btn_screen_btn_1.OnClicked<ui::Osc::toggle_generation>();
        updata_timer.create(ui::Osc::timer_cb, 20);
        chart_screen_chart_1.OnPressed<ui::Osc::set_cursor_on_press>();
        /*!USER_DECLARE_END!*/
    }
}


// ---------------- 模块内部实现 ----------------
namespace gui::ui
{
    // 新增切换生成状态的方法
    auto Osc::toggle_generation() -> void
    {
        is_generating = !is_generating;
        had_generated = true;

        // 更新按钮文本
        label_screen_btn_1_label.text(is_generating ? "BB" : "LL");
        if (is_generating)
        {
            updata_timer.resume();
        }
        else
        {
            updata_timer.pause();
        }
    }

    auto Osc::generate_data() -> void
    {
        // 批量设置128个点
        for (int i = 0; i < 128; ++i)
        {
            //         循环访问数组
            size_t idx = (current_index + i) % RAND_POOL_SIZE;
            chart_screen_chart_1.next_value(series, rand_pool[idx]);
        }
        //     更新索引（每次前进1位置）
        current_index = (current_index + 20) % RAND_POOL_SIZE;
        if (wave_start_index + length > array_length) wave_start_index = 0;

        //    if (count == 0)
        chart_screen_chart_1.ext_y_array(series, rand_pool);
        //    if (count == 1) chart_screen_chart_1.ext_y_array(series, rand_pool1);
        //    if (count == 2) chart_screen_chart_1.ext_y_array(series, rand_pool2);
        //    count++;
        //    if(count==3) count=0;
    }

    auto Osc::initChartComponent() -> void
    {
        series = chart_screen_chart_1.update_mode(LV_CHART_UPDATE_MODE_SHIFT) // 改为SHIFT模式
                .line_color(lv_color_hex(0x34e6ff))
                .point_count(128)
                .remove_dot()
                .add_series(lv_color_hex(0x34e6ff));
        cursor = chart_screen_chart_1.add_cursor(lv_color_hex(0xfffb00), LV_DIR_ALL);
        scale_screen_scale_1.border_opa(0);
    }
}
