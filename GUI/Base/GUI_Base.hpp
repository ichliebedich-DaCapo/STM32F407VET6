//
// Created by fairy on 2024/10/19 10:43.
//
#ifndef SIMULATOR_GUI_BASE_HPP
#define SIMULATOR_GUI_BASE_HPP


/* 预编译命令 */

// 头文件
#include "ui.hpp"


/**********预置**********/

// 定义别名
using Obj = _lv_obj_t *&;// 组件对象(用于给函数传参）
using Obj_t = _lv_obj_t *;// 组件对象（用于定义组件）
using Coord = lv_coord_t;// 坐标
using Color = lv_color_t;// 颜色
using Selector = lv_style_selector_t;
using Align = lv_text_align_t;// 对齐类型
using Font = const lv_font_t *;// 字体类型
using Strings = const char *;// 字符串
using ImageSrc=const lv_img_dsc_t *;// 图片资源

// 色彩
#define Color_Firefly_Green  lv_color_make(133, 238, 223)// 流萤绿
#define Color_Firefly_Gold   lv_color_make(239, 221, 121)// 流萤金

// 屏幕尺寸
constexpr Coord Screen_Width = 480;
constexpr Coord Screen_Height = 320;

/**********类**********/

// 宏定义
class GUI_Base
{
public:
    // 谁都有逼不得已的时候,这个丑陋的用法
    static inline auto get_ui()
    {
        return gui;
    }

protected:
    static inline lv_ui_t ui{};// 内联变量，C++17特性
    static inline lv_ui_t *gui = &ui;// 可加可不加
};


// 暂时废弃，不同屏幕尺寸下，把各组件的的坐标和大小都对应乘以一个比例系数：【你的宽或高】/【本屏幕的宽或高】
// 这个系数放在组件类里设置尺寸和位置的函数里，这样不用每次都去改，比较方便。不过本工程还未这样做
// 对称：假设一个组件宽为w，起始x轴坐标为x，那么与对称的相同组件的起始坐标应为【屏幕宽】-w-x

/**
 * @brief 工具类
 * @details 提供接口用于计算各种坐标等
 *//*
class Tools
{
public:
    // 根据中心坐标，获取组件的起始坐标
    [[maybe_unused]] static inline constexpr auto get_mid_start(Coord axis_middle, Coord value) -> Coord;

    // 根据组件的宽或高，获取组件在屏幕中部时起始坐标
    [[maybe_unused]] static inline constexpr auto get_x_mid_start(Coord value) -> Coord;
    [[maybe_unused]] static inline constexpr auto get_y_mid_start(Coord value) -> Coord;


};

*//**
 * @brief 获取组件的起始y坐标
 * @details 编译期间确定，用于计算组件的起始y坐标
 * @param axis_middle 组件某个坐标轴的中部坐标，由用户自己定义
 * @param value 组件宽或高
 * @return
 *//*
[[maybe_unused]] constexpr auto Tools::get_mid_start(Coord axis_middle, Coord value) -> Coord
{
    return static_cast<Coord>(axis_middle - value / 2);
}

constexpr auto Tools::get_x_mid_start(Coord value) -> Coord
{
    return static_cast<Coord>((Screen_Width - value) / 2);
}

constexpr auto Tools::get_y_mid_start(Coord value) -> Coord
{
    return static_cast<Coord>((Screen_Height - value) / 2);
}*/



/* 预编译命令 */
#endif //SIMULATOR_GUI_BASE_HPP