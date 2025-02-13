//
// Created by fairy on 2024/10/27 14:49.
//
#ifndef SIMULATOR_CHART_HPP
#define SIMULATOR_CHART_HPP

#include "widget.hpp"

/**
 * @brief 图表类
 */
class Chart : public Widget<Chart>
{
public:
    // 初始化图表
    inline Chart &init(Obj_t parent = parent_)
    {
        create_obj(&lv_chart_class,parent);
        return *this;
    }

    Chart &init(Coord x_offset, Coord y_offset, Coord w, Coord h, uint16_t point_cnt = 128,
                bool is_grad = true)
    {
        init();
        center(x_offset, y_offset);
        size(w, h);// 设置图表大小位置

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
        return *this;
    }

    // 设置图表类型
    Chart &set_type(ChartType type)
    {
        lv_chart_set_type(obj_, type);
        return *this;
    }


    // 设置图表范围，默认设置y轴的显示范围
    Chart &set_range(Coord min, Coord max, ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y)
    {
        lv_chart_set_range(obj_, axis, min, max);
        return *this;
    }

    // 设置数据点个数
    Chart &set_point_count(uint16_t point_cnt)
    {
        lv_chart_set_point_count(obj_, point_cnt);
        return *this;
    }


    // 设置更新模式
    Chart &set_update_mode(lv_chart_update_mode_t mode)
    {
        lv_chart_set_update_mode(obj_, mode);
        return *this;
    }

    // 设置下一个值
    Chart &set_next_value(ChartSeries series, Coord value)
    {
        lv_chart_set_next_value(obj_, series, value);
        return *this;
    }

    // 移除折线上点的样式
    Chart &remove_dot()
    {
        style_size(0, 0, LV_PART_INDICATOR);
        return *this;
    }

    // 设置图表的网格线数量
    Chart &set_div_count(uint8_t hor_div, uint8_t ver_div)
    {
        lv_chart_set_div_line_count(obj_, hor_div, ver_div);
        return *this;
    }

    // 添加系列
    Chart &add_series(ChartSeries series, Color color, ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y)
    {
        add_series(series, color, axis);
        return *this;
    }

};


#endif //SIMULATOR_CHART_HPP