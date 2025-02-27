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

    Chart &init(Coord x, Coord y, Coord w, Coord h, uint16_t point_cnt = 128,
                bool is_grad = false)
    {
        init();
        pos_size(x, y, w, h);
//        center(x_offset, y_ofx_offsetfset);
//        size(w, h);// 设置图表大小位置

        // 配置图表
        type(LV_CHART_TYPE_LINE);// 设置类型:折线 柱形
        point_count(point_cnt);// 设置数据点数量
        update_mode(LV_CHART_UPDATE_MODE_SHIFT);
        range(LV_CHART_AXIS_PRIMARY_Y,0, 255);// 设置Y轴范围
        remove_dot();// 取消折线点样式

        // 自定义刻度线和边框
        div_count(0, 0);// 设置刻度线
        border(lv_color_hex(0x3f3f3f), 6, 1);


        // 背景渐变
        if (is_grad)
            bg_grad(Color_Firefly_Green, Color_Firefly_Gold, 105, LV_GRAD_DIR_HOR);
        return *this;
    }
/******************基础图表配置方法**********************/
    // 设置图表类型
    Chart &type(ChartType type)
    {
        lv_chart_set_type(obj_, type);
        return *this;
    }


    // 设置图表范围，默认设置y轴的显示范围
    Chart &range(ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y,Coord min=0, Coord max=255)
    {
        lv_chart_set_range(obj_, axis, min, max);
        return *this;
    }

    // 设置数据点个数
    Chart &point_count(uint16_t point_cnt)
    {
        lv_chart_set_point_count(obj_, point_cnt);
        return *this;
    }

    // 设置显示刷新方式 SHIFT表示每个点都会更新到前一个点，最左侧的点消失；CIRCULAR是循环添加。
    Chart &update_mode(lv_chart_update_mode_t mode)
    {
        lv_chart_set_update_mode(obj_, mode);
        return *this;
    }

    // 设置图表的网格线数量
    Chart &div_count(uint8_t hor_div, uint8_t ver_div)
    {
        lv_chart_set_div_line_count(obj_, hor_div, ver_div);
        return *this;
    }
    // 设置图表网格颜色
    Chart &line_color(Color color)
    {
        lv_obj_set_style_line_color(obj_, color, LV_PART_MAIN);
        return *this;
    }
    // 移除折线上点的样式
    Chart &remove_dot()
    {
        style_size(0, 0, LV_PART_INDICATOR);
        return *this;
    }
    // 设置线宽
    Chart &line_width(uint16_t width)
    {
        lv_obj_set_style_line_width(obj_, width, LV_PART_MAIN);
        return *this;
    }
    // 设置线的透明度
    Chart &line_opa(uint8_t opa)
    {
        lv_obj_set_style_line_opa(obj_, opa, LV_PART_MAIN);
        return *this;
    }
    // 设置系列颜色
    Chart&series_color(ChartSeries_t series, lv_color_t color) {
        lv_chart_set_series_color(obj_, series, color);
        return *this;
    }
    // 设置点的大小 还没搞好
    Chart &dot_size(uint16_t size)
    {
        lv_obj_set_style_line_width(obj_, size, LV_PART_INDICATOR);
        lv_obj_set_style_height(obj_, size, LV_PART_INDICATOR);
        return *this;
    }
    /**************记号和标签相关操作********************/
    //设置记号和标签
//    Chart &set_series_point()
//    {
//        lv_chart_set_axis_tick( );
//        return *this;
//    }
    //设置记号和标签颜色
    //更改标签
    /**************数据系列（Series）相关操作************/
    // 设置下一个值
    Chart &next_value(ChartSeries_t series, Coord value)
    {
        lv_chart_set_next_value(obj_, series, value);
        return *this;
    }
    // 设置下一个数据点（X 和 Y 值）
    Chart &next_value_xy(ChartSeries_t series, Coord x, Coord y) {
        lv_chart_set_next_value2(obj_, series, x, y);
        return *this;
    }
    // 设置整个图的数据点 未测试
    Chart &ext_y_array(ChartSeries_t series, Coord array[])
    {
        lv_chart_set_ext_y_array(obj_,series,array);
        return *this;
    }
    // 添加系列
    /**
     * @brief 添加系列
     * @param series 系列
     * @param color 系列颜色
     * @param axis 轴
     * @return lv_chart_series_t*
     */
    ChartSeries_t add_series(Color color, ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y)
    {
        return  lv_chart_add_series(obj_,color, axis);
        //        lv_chart_add_series(series, color, axis);
    }


    /**************光标（Cursor）操作************/

};


#endif //SIMULATOR_CHART_HPP