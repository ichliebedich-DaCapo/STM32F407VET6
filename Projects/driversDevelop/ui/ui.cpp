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

    static constexpr size_t DATA_POINTS = 128;
    ChartSeries_t series; //数据 系列1
    ChartSeries_t series_dirty; //数据 系列2 脏数据
    int16_t data_cache[DATA_POINTS];  // 数据缓存
    bool dirty_flags[DATA_POINTS];    // 脏标记数组
}
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
        btn_Random_data.init(360,150,80,40,"start/end").hidden();

        series =chart_1.add_series(lv_color_hex(0x34e6ff),LV_CHART_AXIS_PRIMARY_Y);
        // 创建脏标记系列（白色点）
        series_dirty = chart_1.add_series(lv_color_white(), LV_CHART_AXIS_PRIMARY_Y);

        // 初始化缓存
        memset(data_cache, 0, sizeof(data_cache));
        memset(dirty_flags, 0, sizeof(dirty_flags));

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
    chart_1.appear();
    btn_Random_data.appear();
}
auto CounterLogic::generate_data()->void
{
    for (int i = 0; i < 128; ++i)
    {
        lv_chart_set_next_value(chart_1, series, lv_rand(0, 255));
    }
}
// 生成随机数据
//auto CounterLogic::generate_data()->void
//{
//    for(int i = 0;i < 128;++i)
//    {
//        lv_chart_set_next_value(chart_1, series, lv_rand(0, 255));
//    }
//
//    for(int i = 0; i < DATA_POINTS; ++i) {
//        int16_t new_val = lv_rand(0, 255);
//
//        // 仅当数值变化时标记脏点
//        if(data_cache[i] != new_val) {
//            data_cache[i] = new_val;
//            dirty_flags[i] = true;
//
//            // 设置白色点实现擦除效果
//            lv_chart_set_value_by_id(chart_1, series_dirty, i, 255);
//        }
//    }
//    for(int i = 0; i < DATA_POINTS; ++i) {
//        if(dirty_flags[i]) {
//            // 更新实际数据点
//            lv_chart_set_value_by_id(chart_1, series, i, data_cache[i]);
//
//            // 重置脏标记
//            dirty_flags[i] = false;
//            lv_chart_set_value_by_id(chart_1, series_dirty, i, LV_CHART_POINT_NONE);
//        }
//    }
//
//    // 控制刷新频率
//    static uint32_t last_refresh = 0;
//    if(lv_tick_elaps(last_refresh) > 30) { // ~30fps
//        lv_chart_refresh(chart_1);
//        last_refresh = lv_tick_get();
//    }
//}


// 修改后的generate_data函数
//auto CounterLogic::generate_data()->void
//{
//    static uint32_t last_refresh = 0;
//    bool need_partial_refresh = false;
//
//    // 第一阶段：数据变化检测
//    for(int i = 0; i < DATA_POINTS; ++i) {
//        int16_t new_val = lv_rand(0, 255);
//        if(data_cache[i] != new_val) {
//            data_cache[i] = new_val;
//            dirty_flags[i] = true;
//            need_partial_refresh = true;
//        }
//    }
//
//    // 第二阶段：智能区域更新
//    if(need_partial_refresh) {
//        for(int i = 0; i < DATA_POINTS; ++i) {
//            if(dirty_flags[i]) {
//                // 获取旧点位置
//                lv_point_t old_pos;
//                lv_chart_get_point_pos_by_id(chart_1, series, i, &old_pos);
//
//                // 更新数据点
//                lv_chart_set_value_by_id(chart_1, series, i, data_cache[i]);
//
//                // 计算刷新区域（扩展2像素）
//                lv_area_t area = {
//                        .x1 = (lv_coord_t)(old_pos.x - 2),
//                        .y1 = (lv_coord_t)(old_pos.y - 2),
//                        .x2 = (lv_coord_t)(old_pos.x + 2),
//                        .y2 = (lv_coord_t)(old_pos.y + 2)
//                };
//
//                // 标记需要刷新的区域
//                lv_obj_invalidate_area(chart_1, &area);
//
//                // 获取新点位置并标记
//                lv_chart_get_point_pos_by_id(chart_1, series, i, &old_pos);
//                area.x1 = old_pos.x - 2;
//                area.y1 = old_pos.y - 2;
//                area.x2 = old_pos.x + 2;
//                area.y2 = old_pos.y + 2;
//                lv_obj_invalidate_area(chart_1, &area);
//
//                dirty_flags[i] = false;
//            }
//        }
//    }
//
//    // 第三阶段：受控刷新
//    if(lv_tick_elaps(last_refresh) > 30) { // 33ms ≈ 30fps
//        // 仅刷新标记区域
//        lv_refr_now(NULL);
//        last_refresh = lv_tick_get();
//    }
//}
// 新增切换生成状态的方法
auto CounterLogic::toggle_generation() -> void
{
    is_generating = !is_generating;

    // 更新按钮文本
    btn_Random_data.text(is_generating  ? "Stop" : "Start");

    // 管理定时器
    if(is_generating) {
        // 创建定时器（20ms间隔 ≈ 50fps）
        update_timer = lv_timer_create(timer_cb, 20, NULL);
    } else {
        if(update_timer) {
            lv_timer_del(update_timer);
            update_timer = nullptr;
        }
    }
}