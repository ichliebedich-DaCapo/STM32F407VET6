//
// Created by fairy on 2024/10/27 14:49.
//
#ifndef SIMULATOR_CHART_HPP
#define SIMULATOR_CHART_HPP

#include "widget.hpp"

/*****使用模版******/

/**
 * @brief 图表类
 * @note 在类中要加像这样的函数 auto Osc::initData() -> void
    series=chart_screen_chart_1.update_mode(LV_CHART_UPDATE_MODE_SHIFT)  // 改为SHIFT模式\n
            .line_color(lv_color_hex(0x34e6ff))\n
            .point_count(128)\n
            .remove_dot()\n
            .add_series(lv_color_hex(0x34e6ff));\n
    cursor=chart_screen_chart_1.add_cursor(lv_color_hex(0xfffb00),LV_DIR_ALL);\n
 *
 */
class Chart : public Widget<Chart>
{
public:
    // 初始化图表
    inline Chart &init(Obj_t parent = parent_)
    {
        create_obj(&lv_chart_class, parent);
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
        range(LV_CHART_AXIS_PRIMARY_Y, 0, 255);// 设置Y轴范围
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
    Chart &range(ChartAxis axis = LV_CHART_AXIS_PRIMARY_Y, Coord min = 0, Coord max = 255)
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
    Chart &series_color(ChartSeries_t series, lv_color_t color)
    {
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

    // 获取触摸点id
    [[nodiscard]] uint32_t get_pressed_point() const
    {
        return lv_chart_get_pressed_point(obj_);
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

    /**
     *
     * @param color
     * @param dir
     * @return
     * @note   LV_DIR_HOR:水平光标(显示横线);LV_DIR_VER：垂直光标(显示竖线);LV_DIR_ALL：十字光标
     */

    ChartCursor_t add_cursor(Color color, lv_dir_t dir=LV_DIR_HOR)
    {
        return lv_chart_add_cursor(obj_, color, dir);
    }

    // 设置光标位置（通过lv_point_t结构体）
    Chart& set_cursor_pos(ChartCursor_t cursor, const lv_point_t& pos)
    {
        lv_chart_set_cursor_pos(obj_, cursor, const_cast<lv_point_t*>(&pos));
        return *this;
    }

    // 设置光标位置（通过x,y坐标）
    Chart& set_cursor_pos(ChartCursor_t cursor, Coord x, Coord y)
    {
        lv_point_t pos = {x, y};
        return set_cursor_pos(cursor, pos);
    }

    // 绑定光标到指定数据系列的点
    Chart& set_cursor_point(ChartCursor_t cursor, ChartSeries_t series, uint32_t point_id)
    {
        lv_chart_set_cursor_point(obj_, cursor, series, point_id);
        return *this;
    }

    // 获取光标的当前位置（返回结构体副本）
    lv_point_t get_cursor_point(ChartCursor_t cursor) const
    {
        return lv_chart_get_cursor_point(obj_, cursor);
    }

    // 触摸时改变光标位置（自己写的函数）
    Chart& set_cursor_pos_on_pressed(ChartCursor_t& cursor)
    {
        uint32_t last_id;
        last_id = get_pressed_point();  //获取点击对应的线点id
        if (last_id != LV_CHART_POINT_NONE)
        {  // 不是CHART点
            lv_chart_set_cursor_point(obj_, cursor, nullptr, last_id); //设置id对应的CHART上的点的光标线显示
        }
        return *this;
    }
    // 获取光标对应的点值
    Coord& get_cursor_point_y(ChartSeries_t& series)
    {

        uint32_t last_id;
        last_id = get_pressed_point();  //获取点击对应的线点id
        lv_coord_t* data_array = lv_chart_get_y_array(obj_, series); // 获取ser点对应的y轴值集合
        return  data_array[last_id];   //获取id对应的点值
    }
};



#endif //SIMULATOR_CHART_HPP