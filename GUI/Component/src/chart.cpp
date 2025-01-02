//
// Created by fairy on 2025/1/2 16:22.
//

#include "chart.hpp"


// 初始化图表
auto Chart::init() -> void
{
    _obj = lv_chart_create(_parent);
}
auto Chart::init(Coord x_offset, Coord y_offset, Coord w, Coord h, uint16_t point_cnt, bool is_grad) -> void
{
    // 创建图表对象
    init();
    set_align(LV_ALIGN_CENTER, x_offset, y_offset);
    set_size(w, h);// 设置图表大小位置

    // 配置图表
    set_type(LV_CHART_TYPE_LINE);// 设置类型:折线 柱形
    set_point_count(point_cnt);// 设置数据点数量
    set_update_mode(LV_CHART_UPDATE_MODE_SHIFT);// 滚动模式
    set_range(0, 255, LV_CHART_AXIS_PRIMARY_Y);// 设置Y轴范围
    remove_dot();// 取消折线点样式

    // 自定义刻度线和边框
    set_div_count(0, 0);// 设置刻度线
    border(lv_color_hex(0x3f3f3f), 6, 1);

    // 背景渐变
    if (is_grad)
        bg_grad(Color_Firefly_Green, Color_Firefly_Gold, 105, LV_GRAD_DIR_HOR);
}

// 设置图表类型
auto Chart::set_type(ChartType type) -> void
{
    lv_chart_set_type(_obj, type);
}

// 设置图表范围，默认设置y轴的显示范围
auto Chart::set_range(Coord min, Coord max, ChartAxis axis) -> void
{
    lv_chart_set_range(_obj, axis, min, max);
}

// 设置数据点个数
auto Chart::set_point_count(uint16_t point_cnt) -> void
{
    lv_chart_set_point_count(_obj, point_cnt);
}

// 设置更新模式
auto Chart::set_update_mode(lv_chart_update_mode_t mode) -> void
{
    lv_chart_set_update_mode(_obj, mode);
}

// 设置下一个值
auto Chart::set_next_value(ChartSeries series, Coord value) -> void
{
    lv_chart_set_next_value(_obj, series, value);
}

// 移除折线上点的样式
auto Chart::remove_dot() -> void
{
    set_style_size(0, 0, LV_PART_INDICATOR);
}


/**
 * @brief 设置图表的网格线数量
 * @param hor_div 横向网格线数量
 * @param ver_div  纵向网格线数量
 */
auto Chart::set_div_count(uint8_t hor_div, uint8_t ver_div) -> void
{
    lv_chart_set_div_line_count(_obj, hor_div, ver_div);
}

/**
 * @brief 添加系列
 * @details 自动向初始化的组件添加系列，如果不是在初始化过程中需要在参数末尾传入组件参数
 * @param series
 * @param color
 * @param axis
 */
auto Chart::add_series(ChartSeries series, Color color, ChartAxis axis) -> void
{
    series = lv_chart_add_series(_obj, color, axis);
}

/**
 * @brief 初始化图表
 * @details 默认折线无点图居中,滚动模式，无刻度线.默认背景为渐变
 * @param chart
 * @param x_offset
 * @param y_offset
 * @param w
 * @param h
 * @param is_grad
 */

