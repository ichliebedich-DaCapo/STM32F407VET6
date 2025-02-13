//
// Created by fairy on 2025/1/10 18:43.
//
#include <cstring>
#include "ui.hpp"
#include "GUI.hpp"

// 变量
namespace gui::widgets::main
{
    Label label_counter; // 计数器显示
    Label label_weekday; // 星期几显示
    Button btn_x1;     // 触摸点1 x坐标 按钮
    Button btn_y1;    // 触摸点1 y坐标 按钮
    Button btn_x2;    // 触摸点2 x坐标 按钮
    Button btn_y2;    // 触摸点2 y坐标 按钮
    Slider slider_1;   // 滑动条
    Roller roller_1;   //滚轮控件
    Dropdown dropdown_1; // 下拉框控件
    Chart chart_1;       // 图表控价
    Button btn_hidden; // 隐藏按键
    Button btn_Random_data; // 生成随机数据按键

    static constexpr size_t POOL_SIZE = 400;
    static inline size_t current_index = 0;     // 当前读取位置
    ChartSeries_t series; //数据 系列1
    uint8_t length = 200;
    uint8_t wave_start_index = 0;
    uint16_t array_length = 400;
}
/*******WaveDraw参数表**********/
using HighPrecisionDraw = WaveDraw<uint32_t>;
// 初始化参数
HighPrecisionDraw::DrawParams params{
        /* 使用uint32_t坐标系统 */
        .x = 10,
        .y = 10,
        .height = 250,
        .width = 400,
        .margin = 2,
        .length = 200,
        .start_index = 0,
        .array_length = 400
};
/********缓冲区 待优化*************/
uint8_t CH0_buff[200] = {0};//波形数据存储
uint8_t read_wave[400]={111,110,102,109,118,108,114,123,117,121,
                        118,117,109,116,115,117,115,110,112,116,
                        113,104,113,122,127,125,117,123,115,115,
                        124,129,129,125,115,107,113,115,117,124,
                        122,112,120,120,119,111,111,117,127,124,
                        117,127,130,126,132,135,145,150,153,147,
                        144,135,141,141,141,149,151,155,149,150,
                        149,157,157,147,151,154,163,170,165,171,
                        168,168,175,178,174,169,165,155,158,167,
                        174,172,178,170,174,168,163,162,157,148,
                        140,132,134,136,130,133,128,118,125,124,
                        131,140,141,134,124,130,127,125,127,119,
                        116,120,127,133,125,126,129,122,124,127,
                        127,130,135,138,136,142,148,150,143,136,
                        132,125,116,111,107,114,124,117,109,104,
                        107,99,96,101,111,113,118,120,121,122,
                        116,115,109,115,111,109,106,115,112,119,
                        116,106,98,98,102,96,104,111,112,114,
                        121,131,125,135,139,134,134,124,121,115,
                        106,116,107,109,118,128,137,127,134,126,
                        128,121,111,102,101,95,93,85,93,85,
                        79,83,92,82,79,86,85,88,89,80,
                        86,87,89,84,84,75,65,58,62,67,
                        72,78,72,71,71,73,79,81,81,77,
                        79,80,81,84,89,99,101,99,104,96,
                        99,99,93,100,99,102,112,112,107,98,
                        96,95,95,97,103,109,103,93,89,97,
                        95,88,97,87,80,78,79,82,85,82,
                        89,95,85,91,81,72,62,62,61,65,
                        69,61,59,52,43,33,36,28,31,21,
                        13,9,9,10,10,6,2,11,21,24,
                        22,13,22,28,24,31,33,42,34,42,
                        42,36,34,24,16,26,31,36,45,55,
                        61,54,47,50,50,48,45,42,50,46,
                        40,41,42,42,46,47,43,42,42,43,
                        34,32,30,30,28,24,24,28,23,19,
                        11,6,1,0,2,0,5,0,0,0,
                        7,2,5,4,7,1,0,9,10,19,
                        28,34,35,30,27,36,37,43,49,45,
                        48,38,30,32,29,25,20,25,27,24,

}; // 随机数池



static int counter_value = 0; // 计数器值存储

// 计数器逻辑
class CounterLogic
{
public:
    // 更新触摸点坐标
    static inline auto update_point() -> void;

public:
    // 获取触摸点坐标
    static inline  auto get_touch_point() -> void;

    // 增加计数器
    static inline auto increment() -> void;

    // 减少计数器
    static inline auto decrement() -> void;

    // 复位计数器
    static inline auto reset() -> void;

    // 获取计数器值
    static inline auto get_value() -> int;

    // 设置计数器值
    static inline auto set_value_label(int value) -> void;

    // 触摸点1 x坐标
    static inline auto set_value_x1(int value) -> void;

    // 触摸点1 y坐标
    static inline auto set_value_y1(int value) -> void;

    // 触摸点2 x坐标
    static inline auto set_value_x2(int value) -> void;

    // 触摸点2 y坐标
    static inline auto set_value_y2(int value) -> void;

    // 设置星期几显示
    static inline auto set_value_weekday_roller()->void;

    // 设置星期几显示
    static inline auto set_value_weekday_dropdown()->void;

    // 隐藏
    static inline auto hidden_all()->void;

    // 生成随机数据
    static inline auto generate_data()->void;

    static inline auto toggle_generation() -> void;

    // 新增定时器回调函数
    static void timer_cb(lv_timer_t* timer)
    {
        if(CounterLogic::is_generating) {
            CounterLogic::generate_data();
        }
    }


private:
    static inline int counter_value = 0; // 计数器值
    static inline lv_point_t point_1{};
    static inline lv_point_t point_2{};// 不存在第二个触摸点
    // 添加生成状态标志
    static inline bool is_generating = false;
    // 添加定时器指针
    static inline lv_timer_t* update_timer = nullptr;

    static constexpr size_t RAND_POOL_SIZE = 400; // 预生成池大小
    static inline uint8_t rand_pool[RAND_POOL_SIZE]={111,110,102,109,118,108,114,123,117,121,
                                                     118,117,109,116,115,117,115,110,112,116,
                                                     113,104,113,122,127,125,117,123,115,115,
                                                     124,129,129,125,115,107,113,115,117,124,
                                                     122,112,120,120,119,111,111,117,127,124,
                                                     117,127,130,126,132,135,145,150,153,147,
                                                     144,135,141,141,141,149,151,155,149,150,
                                                     149,157,157,147,151,154,163,170,165,171,
                                                     168,168,175,178,174,169,165,155,158,167,
                                                     174,172,178,170,174,168,163,162,157,148,
                                                     140,132,134,136,130,133,128,118,125,124,
                                                     131,140,141,134,124,130,127,125,127,119,
                                                     116,120,127,133,125,126,129,122,124,127,
                                                     127,130,135,138,136,142,148,150,143,136,
                                                     132,125,116,111,107,114,124,117,109,104,
                                                     107,99,96,101,111,113,118,120,121,122,
                                                     116,115,109,115,111,109,106,115,112,119,
                                                     116,106,98,98,102,96,104,111,112,114,
                                                     121,131,125,135,139,134,134,124,121,115,
                                                     106,116,107,109,118,128,137,127,134,126,
                                                     128,121,111,102,101,95,93,85,93,85,
                                                     79,83,92,82,79,86,85,88,89,80,
                                                     86,87,89,84,84,75,65,58,62,67,
                                                     72,78,72,71,71,73,79,81,81,77,
                                                     79,80,81,84,89,99,101,99,104,96,
                                                     99,99,93,100,99,102,112,112,107,98,
                                                     96,95,95,97,103,109,103,93,89,97,
                                                     95,88,97,87,80,78,79,82,85,82,
                                                     89,95,85,91,81,72,62,62,61,65,
                                                     69,61,59,52,43,33,36,28,31,21,
                                                     13,9,9,10,10,6,2,11,21,24,
                                                     22,13,22,28,24,31,33,42,34,42,
                                                     42,36,34,24,16,26,31,36,45,55,
                                                     61,54,47,50,50,48,45,42,50,46,
                                                     40,41,42,42,46,47,43,42,42,43,
                                                     34,32,30,30,28,24,24,28,23,19,
                                                     11,6,1,0,2,0,5,0,0,0,
                                                     7,2,5,4,7,1,0,9,10,19,
                                                     28,34,35,30,27,36,37,43,49,45,
                                                     48,38,30,32,29,25,20,25,27,24,


    }; // 随机数池;



};

// 默认使用main屏幕里的组件（暂时用不到其他界面）
using namespace gui::widgets::main;
namespace gui::init
{
    void screen()
    {
        // 设置全局字体格式
        Label::Font(lv_customer_font_SourceHanSerifSC_Regular_15);
        Button::Font(lv_customer_font_SourceHanSerifSC_Regular_15);

        // 计数器显示
        widgets::main::label_counter.init("0").pos_size(260, 150, 80, 40);

        // 触摸点1 x坐标
        btn_x1.init(50, 100, 80, 40, "0").bg_color(lv_color_hex(0x34e6ff));

        // 触摸点1 y坐标
        btn_y1.init(150, 100, 80, 40, "0").bg_color(lv_color_hex(0x34e6ff));

        // 触摸点2 x坐标 按钮
        btn_x2.init(50, 150, 80, 40, "+").bg_color(lv_color_hex(0x34e6ff));

        // 触摸点2 y坐标 按钮
        btn_y2.init(150, 150, 80, 40, "-").bg_color(lv_color_hex(0x34e6ff));

        // 测试 滑动条
        slider_1.init(50, 200, 200, 10).bg_color(lv_color_hex(0x34e6ff));

        // 星期几显示
        label_weekday.init(305, 50, 80, 40,"None").text_align(LV_TEXT_ALIGN_CENTER);

        // 测试 滚轮控件
        roller_1.init(270, 100, 50, 40,"Mon\nTue\nWed\nThu\nFri\nSat\nSun").bg_color(lv_color_hex(0x34e6ff));

        // 测试 下拉框控件
        dropdown_1.init(320, 100, 100, 40,"Mon\nTue\nWed\nThu\nFri\nSat\nSun").bg_color(lv_color_hex(0x34e6ff)).direction(LV_DIR_BOTTOM);

        // 切换界面按键
        btn_hidden.init(400,280,80,40,"chart");

//        // 测试 图表控件
        chart_1.init(50,  50, 300, 250,128)
                .set_update_mode(LV_CHART_UPDATE_MODE_SHIFT)  // 改为SHIFT模式
                .set_div_color(lv_color_hex(0x34e6ff))
                .hidden();

        // 测试 生成随机数据按键
        btn_Random_data.init(360,150,80,40,"start/end").hidden();

        // print
        FPS::init(&lv_customer_font_SourceHanSerifSC_Regular_13, 415, 0);
        FPS::set_right();


        // 创建系列1（
        series =chart_1.add_series(lv_color_hex(0x34e6ff),LV_CHART_AXIS_PRIMARY_Y);
//        // 创建脏标记系列（白色点）
//        series_dirty = chart_1.add_series(lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);
//
//        // 初始化缓存
//        memset(data_cache, 0, sizeof(data_cache));
//        memset(dirty_flags, 0, sizeof(dirty_flags));

//        for(int i = 0;i < 128;++i)
//        {
//            lv_chart_set_next_value(chart_1, series, lv_rand(0, 255));
//        }


    }


    void events()
    {
        // 绑定 "+" 按钮事件
        widgets::main::btn_x2.OnClicked<CounterLogic::increment>();

        // 绑定 "-" 按钮事件
        widgets::main::btn_y2.OnClicked<CounterLogic::decrement>();

        // 屏幕触摸事件
        widgets::main::scr.OnPressing<CounterLogic::update_point>();

        // 绑定 星期几 滚轮控件事件
        widgets::main::roller_1.OnValueChanged<CounterLogic::set_value_weekday_roller>();

        // 绑定 星期几 下拉框事件
        widgets::main::dropdown_1.OnValueChanged<CounterLogic::set_value_weekday_dropdown>();

        // 绑定 隐藏事件
        widgets::main::btn_hidden.OnClicked<CounterLogic::hidden_all>();

        // 绑定 随机生成数据事件
        widgets::main::btn_Random_data.OnPressing<CounterLogic::generate_data>();

        // 绑定 随机生成数据事件
        widgets::main::btn_Random_data.OnClicked<CounterLogic::toggle_generation>();

    }
}


/// UI接口
namespace gui::interface
{
    auto set_counter_value(int value) -> void
    {
        CounterLogic::set_value_label(value);
    }

    auto set_x1_value(int value) -> void
    {
        CounterLogic::set_value_x1(value);
    }

    auto set_y1_value(int value) -> void
    {
        CounterLogic::set_value_y1(value);
    }

    auto set_x2_value(int value) -> void
    {
        CounterLogic::set_value_x2(value);
    }

    auto set_y2_value(int value) -> void
    {
        CounterLogic::set_value_y2(value);
    }

    auto get_counter_value() -> int
    {
        return CounterLogic::get_value();
    }
}



auto CounterLogic::increment() -> void
{
    counter_value++;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::decrement() -> void
{
    counter_value--;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::reset() -> void
{
    counter_value = 0;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::get_value() -> int
{
    return counter_value;
}

auto CounterLogic::set_value_label(int value) -> void
{
    counter_value = value;
    char buf[12];
    sprintf(buf, "%d", counter_value);
    label_counter.text(buf);
}

auto CounterLogic::set_value_x1(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_x1.text(buf);
}

auto CounterLogic::set_value_y1(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_y1.text(buf);
}

auto CounterLogic::set_value_x2(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_x2.text(buf);
}

auto CounterLogic::set_value_y2(int value) -> void
{
    char buf[12];
    sprintf(buf, "%d", value);
    btn_y2.text(buf);
}

auto CounterLogic::get_touch_point() -> void
{
    lv_indev_get_point(lv_indev_get_act(), &point_1);
}

auto CounterLogic::update_point() -> void
{
    get_touch_point();
    set_value_x1(point_1.x);
    set_value_y1(point_1.y);
}
auto CounterLogic::set_value_weekday_roller()->void
{
    char buf[12];
    roller_1.getSelectedStr(buf,12);
    label_weekday.text(buf);
}

auto CounterLogic::set_value_weekday_dropdown()->void
{
    char buf[12];
    dropdown_1.getSelectedStr(buf,12);
    label_weekday.text(buf);

}

auto CounterLogic::hidden_all() -> void
{
    lv_obj_delete(label_counter);
    lv_obj_delete(label_weekday);
    lv_obj_delete(btn_x1);
    lv_obj_delete(btn_y1);
    lv_obj_delete(btn_x2);
    lv_obj_delete(btn_y2);
    lv_obj_delete(slider_1);
    lv_obj_delete(roller_1);
    lv_obj_delete(dropdown_1);
    lv_obj_delete(btn_hidden);


//    label_counter.hidden();
//    label_weekday.hidden();
//    btn_x1.hidden();
//    btn_y1.hidden();
//    btn_x2.hidden();
//    btn_y2.hidden();
//    slider_1.hidden();
//    roller_1.hidden();
//    dropdown_1.hidden();
//    btn_hidden.hidden();
//    chart_1.appear();
    btn_Random_data.appear();
}
auto CounterLogic::generate_data()->void
{
// 批量设置128个点
    for (int i = 0; i < 128; ++i) {
//         循环访问数组
        size_t idx = (current_index + i) % POOL_SIZE;
        lv_chart_set_next_value(chart_1, series, rand_pool[idx]);
    }
//     更新索引（每次前进1位置）
    current_index = (current_index + 20) % POOL_SIZE;
    if(wave_start_index+length>array_length) wave_start_index=0;

    FPS::print(false);


    draw_dashed_dividers(border_info::x, border_info::y,  border_info::height, border_info::width,
                         border_info::h_div, border_info::v_div, 0X8410,
                         border_info::margin, border_info::grid_dash_length, border_info::grid_gap_length);

    HighPrecisionDraw::draw_wave<WaveDrawType::Interpolated_Line, uint8_t, uint16_t>(
            params,
            read_wave,
            CH0_buff,
            0XFCC0,
            0xFFFF
    );
    params.start_index=(params.start_index +  20) % params.array_length;
    if(params.start_index+params.length>params.array_length) params.start_index=0;

    FPS::print(false);
}


// 新增切换生成状态的方法
auto CounterLogic::toggle_generation() -> void
{
    is_generating = !is_generating;

    // 更新按钮文本
    btn_Random_data.text(is_generating  ? "Stop" : "Start");

    // 管理定时器
    if(is_generating) {
        // 创建定时器（20ms间隔 ≈ 50fps）
        update_timer = lv_timer_create(timer_cb, 50, NULL);
    } else {
        if(update_timer) {
            lv_timer_del(update_timer);
            update_timer = nullptr;
        }
    }
}
