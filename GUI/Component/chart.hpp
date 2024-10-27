//
// Created by fairy on 2024/10/27 14:49.
//
#ifndef SIMULATOR_CHART_HPP
#define SIMULATOR_CHART_HPP

#include "component.hpp"
/* 预编译命令 */
#if 1
// 头文件

// 宏定义

/**
 * @brief 图表类
 */
class Chart : public Component
{
public:
    static inline auto init(_lv_obj_t *&component) -> void
    {
        component = lv_chart_create(_parent);
        _obj = component;
    }

    static inline auto set_type(lv_chart_type_t type) -> void
    {
        lv_chart_set_type(_obj, type);
    }

    // 设置图表范围，默认设置y轴的显示范围
    static inline auto set_range(lv_coord_t min, lv_coord_t max, lv_chart_axis_t axis = LV_CHART_AXIS_PRIMARY_Y) -> void
    {
        lv_chart_set_range(_obj, axis, min, max);
    }

    // 设置数据点个数
    static inline auto set_point_count(uint16_t point_cnt) -> void
    {
        lv_chart_set_point_count(_obj, point_cnt);
    }

    // 设置更新模式
    static inline auto set_update_mode(lv_chart_update_mode_t mode) -> void
    {
        lv_chart_set_update_mode(_obj, mode);
    }

    // 移除折线上点的样式
    static inline auto remove_style_dot() -> void
    {
        set_style_size(0, LV_PART_INDICATOR);
    }

   /**
    * @brief 设置图表的网格线数量
    * @param hor_div 横向网格线数量
    * @param ver_div  纵向网格线数量
    */

    static inline auto set_div_count(uint8_t hor_div, uint8_t ver_div) -> void
    {
        lv_chart_set_div_line_count(_obj, hor_div, ver_div);
    }

    static inline auto add_series(lv_chart_series_t *&series, lv_color_t color, lv_chart_axis_t axis=LV_CHART_AXIS_PRIMARY_Y) -> void
    {
        series=lv_chart_add_series(_obj, color,axis);
    }
};


#endif
#endif //SIMULATOR_CHART_HPP