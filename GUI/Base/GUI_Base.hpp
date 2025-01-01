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
using ImageSrc = const lv_img_dsc_t *;// 图片资源

// 色彩
#define Color_Firefly_Green  lv_color_make(133, 238, 223)// 流萤绿
#define Color_Firefly_Gold   lv_color_make(239, 221, 121)// 流萤金


/**********类**********/

// 宏定义
class GUI_Base
{
public:
    static inline auto get_ui() { return gui; }
private:
    static inline lv_ui_t ui{};// 内联变量，C++17特性
protected:
    static inline lv_ui_t *gui = &ui;// 可加可不加
};




/* 预编译命令 */
#endif //SIMULATOR_GUI_BASE_HPP