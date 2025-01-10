//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
// 头文件

// 变量
struct lv_ui_t lv_ui;
struct lv_ui_t *gui = &lv_ui;


lv_obj_t *panel;

static void panel_event_cb(lv_event_t *e)
{
    auto *content = (Obj_t) lv_event_get_user_data(e);
    if (lv_obj_has_flag(content, LV_OBJ_FLAG_HIDDEN))
    {
        lv_obj_clear_flag(content, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height(panel, 100); // 展开高度
    }
    else
    {
        lv_obj_add_flag(content, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_height(panel, 40); // 折叠高度
    }
}

// 函数
void GUI_Base::screen_init()
{
    gui->main.btn_test.init(100, 100, 60, 50, "123", &lv_font_montserrat_14);
    gui->main.btn_test.text_color(lv_color_black());

    Obj_t parent = gui->main.screen.get_obj();




}

void GUI_Base::events_init()
{

}


/****************************************UI接口*********************************/
void UI::pressA()
{


}

void UI::pressS()
{


}
