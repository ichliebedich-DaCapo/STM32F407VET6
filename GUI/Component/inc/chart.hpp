//
// Created by fairy on 2024/10/27 14:49.
//
#ifndef SIMULATOR_CHART_HPP
#define SIMULATOR_CHART_HPP

#include "component.hpp"

/**
 * @brief 图表类
 */
class Chart : public Component
{
public:
    // 初始化图表
    auto init() -> void;

    auto init(Coord x_offset, Coord y_offset, Coord w, Coord h, uint16_t point_cnt = 128,
              bool is_grad = true) -> void;

    // 设置图表类型
    auto set_type(ChartType type) -> void;

    // 设置图表范围，默认设置y轴的显示范围
    auto
    set_range(Coord min, Coord max, ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y) -> void;

    // 设置数据点个数
    auto set_point_count(uint16_t point_cnt) -> void;

    // 设置更新模式
    auto set_update_mode(lv_chart_update_mode_t mode) -> void;

    // 设置下一个值
    auto set_next_value(ChartSeries series, Coord value) -> void;

    // 移除折线上点的样式
    auto remove_dot() -> void;

    // 设置图表的网格线数量
    auto set_div_count(uint8_t hor_div, uint8_t ver_div) -> void;

    // 添加系列
    auto
    add_series(ChartSeries series, Color color, ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y) -> void;

};


#endif //SIMULATOR_CHART_HPP