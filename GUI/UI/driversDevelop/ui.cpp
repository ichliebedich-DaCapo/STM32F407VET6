//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
// 头文件

// 变量
struct lv_ui_t lv_ui;
struct lv_ui_t *gui = &lv_ui;


#include "lvgl.h"

#include "lvgl.h"
#include <cstdint>

class LvglAnimator
{
public:
    // 初始化动画对象
    LvglAnimator()
    {
        lv_anim_init(&anim);
    }

    // 创建滚动动画（从指定起始位置到结束位置）
    void createScrollAnimation(lv_obj_t *target, int32_t endX, int32_t endY, uint16_t time,
                               AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                               AnimCompletedCallback completed_callback = nullptr)
    {
        lv_anim_set_var(&anim, target);
        lv_anim_set_values(&anim, 0, 100); // 占位值，实际值在回调中计算
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out); // 默认使用缓入缓出效果

        // 如果用户没有提供执行回调，使用默认的回调
        if (exec_callback)
        {
            // 使用用户提供的执行回调和 userData
            lv_anim_set_exec_cb(&anim, exec_callback);
            lv_anim_set_user_data(&anim, userData);
        }
        else
        {
            // 将起始和结束坐标通过 userData 传递
            auto *coords = static_cast<int32_t *>(lv_malloc(4 * sizeof(int32_t)));
            coords[0] = lv_obj_get_x(target);// startX
            coords[1] = lv_obj_get_y(target);// startY
            coords[2] = endX;
            coords[3] = endY;

            lv_anim_set_user_data(&anim, coords);

            lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
            {
                auto *target = static_cast<lv_obj_t *>(var);
                lv_anim_t *anim = lv_anim_get(var, nullptr); // 获取当前动画对象
                auto *coords = static_cast<int32_t *>(anim->user_data);
                int32_t currentX = coords[0] + (coords[2] - coords[0]) * value / 100;
                int32_t currentY = coords[1] + (coords[3] - coords[1]) * value / 100;
                lv_obj_set_pos(target, currentX, currentY);
            });

            // 设置完成回调以释放 userData
            lv_anim_set_completed_cb(&anim, [](lv_anim_t *anim)
            {
                lv_free(anim->user_data);
            });
        }

        // 设置用户提供的完成回调
        if (completed_callback)
        {
            lv_anim_set_completed_cb(&anim, completed_callback);
        }

        lv_anim_start(&anim);
    }

    // 创建滚动动画（从当前位置滑动到相对偏移量）
    void createScrollAnimationByOffset(lv_obj_t *target, int32_t offsetX, int32_t offsetY, uint16_t time,
                                       AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                                       AnimCompletedCallback completed_callback = nullptr)
    {
        int32_t startX = lv_obj_get_x(target); // 获取当前X坐标
        int32_t startY = lv_obj_get_y(target); // 获取当前Y坐标
        int32_t endX = startX + offsetX;       // 计算目标X坐标
        int32_t endY = startY + offsetY;       // 计算目标Y坐标
        createScrollAnimation(target, endX, endY, time, exec_callback, userData, completed_callback);
    }

    // 停止动画
    void stop()
    {
        lv_anim_del(&anim, nullptr);
    }

private:
    Anim_t anim{}; // LVGL动画对象
};


// 全局变量
LvglAnimator animator;

// 函数
void GUI_Base::screen_init()
{
    gui->main.btn_test.init(100, 100, 60, 50, "123", &lv_font_montserrat_14);
    gui->main.btn_test.text_color(lv_color_black());


}

void GUI_Base::events_init()
{

}


/****************************************UI接口*********************************/
void UI::pressA()
{
//    animator.createScrollAnimation(gui->main.btn_test.get_obj(), 200, 200, 1000);
    animator.createScrollAnimationByOffset(gui->main.btn_test.get_obj(), 100, 120, 1000);
}

void UI::pressS()
{
//    animator.createScrollAnimation(gui->main.btn_test.get_obj(), 200, 100, 1000);
    animator.createScrollAnimationByOffset(gui->main.btn_test.get_obj(), -90, -110, 1000);
}
