//
// Created by DV on 2025/2/10.
//

#ifndef SIMULATOR_Roller_HPP
#define SIMULATOR_Roller_HPP
#include "widget.hpp"

class Roller : public Widget<Roller>
{
public:

    Roller& init(Obj_t parent=parent_)
    {
        create_obj(&lv_roller_class, parent);
        return *this;
    }
    // 使用前必须设置父对象
    Roller& init(Coord x, Coord y, Coord w, Coord h,
                 const char* options="",uint32_t visible_rows = 3,
                 Color bg_color = lv_color_hex(0x2195f6), uint8_t opa = 125,
                 lv_roller_mode_t mode =  LV_ROLLER_MODE_INFINITE,
                 uint32_t default_selection = 0)
    {
        init();
        pos_size(x, y, w, h);
        Roller::bg_color(bg_color, opa);
        Roller::VisibleRowCount(visible_rows);
        Roller::options(options, mode);
        Roller::selected(default_selection);
        return *this;
    }
    // 设置选项列表 LV_ROLLER_MODE_NORMAL
    Roller& options(const char* options, lv_roller_mode_t mode =  LV_ROLLER_MODE_INFINITE) {
        lv_roller_set_options(obj_, options, mode);
        return *this;
    }

    // 设置选中项
    Roller& selected(uint32_t sel_opt, lv_anim_enable_t anim = LV_ANIM_OFF) {
        lv_roller_set_selected(obj_, sel_opt, anim);
        return *this;
    }

    // 设置可见行数
    Roller& VisibleRowCount(uint32_t row_cnt) {
        lv_roller_set_visible_row_count(obj_, row_cnt);
        return *this;
    }

    // 获取当前选中索引
    [[nodiscard]] uint32_t getSelected() const {
        return lv_roller_get_selected(obj_);
    }

    // 获取选中项文本（C风格缓冲区）
    void getSelectedStr(char* buf, uint32_t buf_size) const {
        lv_roller_get_selected_str(obj_, buf, buf_size);
    }

    // 获取原始选项数据
    [[nodiscard]] const char* getOptions() const {
        return lv_roller_get_options(obj_);
    }

    // 获取选项总数
    [[nodiscard]] uint32_t getOptionCount() const {
        return lv_roller_get_option_count(obj_);
    }
};
#endif //SIMULATOR_Roller_HPP