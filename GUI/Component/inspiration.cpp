//
// Created by fairy on 2025/1/2 16:21.
//


// 只是占位用的，不然Component就会被移走

//    导航菜单，一一些内置的按钮
/*lv_obj_t *list = lv_list_create(gui->main.screen.get_obj());
lv_obj_set_size(list, 160, 200);
lv_obj_center(list);

lv_obj_t *btn1 = lv_list_add_btn(list, LV_SYMBOL_HOME, "Home");
lv_obj_t *btn2 = lv_list_add_btn(list, LV_SYMBOL_SETTINGS, "Settings");
lv_obj_t *btn3 = lv_list_add_btn(list, LV_SYMBOL_POWER, "Power");

lv_group_t *g = lv_group_create();
lv_group_add_obj(g, btn1);
lv_group_add_obj(g, btn2);
lv_group_add_obj(g, btn3);*/

// 进度条
/*lv_obj_t *bar = lv_bar_create(parent);
lv_obj_set_size(bar, 200, 20);
lv_obj_center(bar);
lv_bar_set_value(bar, 50, LV_ANIM_ON);*/

// 加载条
/*lv_obj_t *spinner = lv_spinner_create(parent);
lv_obj_set_size(spinner, 60, 60);
lv_obj_center(spinner);*/


// 滑动开关
/*lv_obj_t *sw = lv_switch_create(parent);
lv_obj_center(sw);
lv_obj_add_event_cb(sw, switch_event_cb, LV_EVENT_VALUE_CHANGED, nullptr);

static void switch_event_cb(lv_event_t *e)
{
    auto *sw = (Obj_t)lv_event_get_target(e);
    if (lv_obj_has_state(sw, LV_STATE_CHECKED))
    {
        printf("Switch ON\n");
    }
    else
    {
        printf("Switch OFF\n");
    }
}*/

// 文本的滚动模式
/*
lv_obj_t *label = lv_label_create(parent);
lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
lv_obj_set_width(label, 150);
lv_label_set_text(label, "This is a scrolling text example.");
lv_obj_center(label);*/


// 滑动列表
/*lv_obj_t *list = lv_list_create(parent);
lv_obj_set_size(list, 160, 200);
lv_obj_center(list);

for(int i = 0; i < 10; i++) {
char buf[32];
snprintf(buf, sizeof(buf), "Item %d", i);
lv_list_add_btn(list, nullptr, buf);
}*/


// 切换页面
/*
lv_obj_t *tabview = lv_tabview_create(parent);
lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");
lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");

lv_obj_t *label1 = lv_label_create(tab1);
lv_label_set_text(label1, "Content of Tab 1");

lv_obj_t *label2 = lv_label_create(tab2);
lv_label_set_text(label2, "Content of Tab 2");*/


// 可折叠面板
/*
panel = lv_obj_create(parent);
lv_obj_set_size(panel, 200, 40); // 初始高度
lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_COLUMN);

lv_obj_t *header = lv_btn_create(panel);
lv_obj_set_size(header, 200, 40);
lv_obj_t *header_label = lv_label_create(header);
lv_label_set_text(header_label, "Click to Expand");
lv_obj_center(header_label);

lv_obj_t *content = lv_label_create(panel);
lv_label_set_text(content, "This is the hidden content.");
lv_obj_add_flag(content, LV_OBJ_FLAG_HIDDEN);

lv_obj_add_event_cb(header, panel_event_cb, LV_EVENT_CLICKED, content);*/


// 滑动选择器，非常适合多个选项，比如说10个数字
/*
lv_obj_t *roller = lv_roller_create(parent);
lv_roller_set_options(roller, "Option 1\nOption 2\nOption 3\nOption 4", LV_ROLLER_MODE_NORMAL);
lv_obj_set_size(roller, 100, 150);
lv_obj_center(roller);

lv_obj_add_event_cb(roller, [](lv_event_t *e)
{
auto *roller = (Obj_t)lv_event_get_target(e);
uint16_t sel = lv_roller_get_selected(roller);
printf("Selected option: %d\n", sel);
}, LV_EVENT_VALUE_CHANGED, nullptr);*/

// 指示器，相当于一个红绿灯那样的
/*lv_obj_t *indicator = lv_obj_create(parent);
lv_obj_set_size(indicator, 100, 20);
lv_obj_set_flex_flow(indicator, LV_FLEX_FLOW_ROW);
lv_obj_set_flex_align(indicator, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

for(int i = 0; i < 3; i++) {
lv_obj_t *dot = lv_obj_create(indicator);
lv_obj_set_size(dot, 10, 10);
lv_obj_set_style_radius(dot, LV_RADIUS_CIRCLE, 0);
lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_GREY), 0);
if(i == 0) {
lv_obj_set_style_bg_color(dot, lv_palette_main(LV_PALETTE_BLUE), 0);
}
}*/
