//
// Created by DV on 2025/2/10.
//

#ifndef SIMULATOR_SLIDER_HPP
#define SIMULATOR_SLIDER_HPP
#include "widget.hpp"

class Roller : public Widget<Roller>
{
public:
    // ʹ��ǰ�������ø�����
    Roller& init()
    {
        create_obj(&lv_roller_class);
        return *this;
    }

    Roller& init(Coord x, Coord y, Coord w, Coord h,
                 const char* options,Color bg_color = lv_color_hex(0x2195f6), uint8_t opa = 125,
                 uint32_t visible_rows = 3,
                 lv_roller_mode_t mode = LV_ROLLER_MODE_NORMAL,
                 uint32_t default_selection = 0)
    {
        init();
        pos_size(x, y, w, h);
        Roller::bg_color(bg_color, opa);
        Roller::setVisibleRowCount(visible_rows);
        Roller::setOptions(options, mode);
        Roller::setSelected(default_selection);
        return *this;
    }
    // ����ѡ���б�
    Roller& setOptions(const char* options, lv_roller_mode_t mode = LV_ROLLER_MODE_NORMAL) {
        lv_roller_set_options(obj_, options, mode);
        return *this;
    }

    // ����ѡ����
    Roller& setSelected(uint32_t sel_opt, lv_anim_enable_t anim = LV_ANIM_OFF) {
        lv_roller_set_selected(obj_, sel_opt, anim);
        return *this;
    }

    // ���ÿɼ�����
    Roller& setVisibleRowCount(uint32_t row_cnt) {
        lv_roller_set_visible_row_count(obj_, row_cnt);
        return *this;
    }

//    // ��ȡ��ǰѡ������
//    uint32_t getSelected() const {
//        return lv_roller_get_selected(obj_);
//    }

    // ��ȡѡ�����ı���C��񻺳�����
    void getSelectedStr(char* buf, uint32_t buf_size) const {
        lv_roller_get_selected_str(obj_, buf, buf_size);
    }

//    // ��ȡѡ�����ı���C++�ַ�����
//    std::string getSelectedString() const {
//        char buf[256];
//        lv_roller_get_selected_str(obj_, buf, sizeof(buf));
//        return std::string(buf);
//    }

//    // ��ȡԭʼѡ������
//    const char* getOptions() const {
//        return lv_roller_get_options(obj_);
//    }

//    // ��ȡѡ������
//    uint32_t getOptionCount() const {
//        return lv_roller_get_option_count(obj_);
//    }
};
#endif //SIMULATOR_SLIDER_HPP