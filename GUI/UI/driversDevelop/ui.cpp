//
// Created by fairy on 2025/1/10 18:43.
//
#include "ui.hpp"
// 头文件

// 变量
struct lv_ui_t lv_ui;
struct lv_ui_t *gui = &lv_ui;

Obj_t obj;

#include "lvgl.h"

class LvglAnimator
{
public:
    // 初始化动画对象
    LvglAnimator()
    {
        lv_anim_init(&anim);
    }

    // 创建滚动动画（从当前位置滑动到目标位置）
    /**
     * @brief 创建滚动动画
     * @param target
     * @param endX
     * @param endY
     * @param time
     * @param is_relative false 表示绝对位置，true表示相对位置
     * @param exec_callback
     * @param userData
     * @tparam completed_callback   为了防止忘记释放申请的内存（不忘记也设置不了），这里使用模板函数
     */
    template<AnimCompletedCallback completed_callback = nullptr>
    void createScrollAnimation(Obj_t target, int32_t endX, int32_t endY, uint16_t time, bool is_relative = false,
                               AnimExecCallback exec_callback = nullptr, void *userData = nullptr)
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
            if (coords)
            {
                coords[0] = lv_obj_get_x(target); // startX
                coords[1] = lv_obj_get_y(target); // startY
                coords[2] = is_relative ? coords[0] + endX : endX;                 // endX
                coords[3] = is_relative ? coords[1] + endY : endY;                 // endY

                lv_anim_set_user_data(&anim, coords);

                lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
                {
                    auto *target = static_cast<Obj_t >(var);
                    lv_anim_t *anim = lv_anim_get(var, nullptr); // 获取当前动画对象
                    auto *coords = static_cast<int32_t *>(anim->user_data);
                    int32_t currentX = coords[0] + (coords[2] - coords[0]) * value / 100;
                    int32_t currentY = coords[1] + (coords[3] - coords[1]) * value / 100;
                    lv_obj_set_pos(target, currentX, currentY);
                });

                // 设置完成回调以释放 userData
                lv_anim_set_completed_cb(&anim, [](Anim_t *anim)
                {
                    lv_free(anim->user_data);

                });
            }
        }

        // 设置用户提供的完成回调
        // 设置完成回调以释放 userData
        lv_anim_set_completed_cb(&anim, [](Anim_t *anim)
        {
            lv_free(anim->user_data);
            if constexpr (completed_callback != nullptr)
                completed_callback(anim);
        });

        lv_anim_start(&anim);
    }

/**
 * @brief 创建淡入淡出动画
 * @param target
 * @param startOpacity
 * @param endOpacity
 * @param time
 * @param exec_callback
 * @param userData
 * @param completed_callback
 * @note 示例：
 *        animator.createFadeAnimation(gui->main.btn_test.get_obj(), LV_OPA_COVER, LV_OPA_TRANSP, 500);// 从完全不透明到完全透明
 */
    void createFadeAnimation(Obj_t target, int32_t startOpacity, int32_t endOpacity, uint16_t time,
                             AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                             AnimCompletedCallback completed_callback = nullptr)
    {
        lv_anim_set_var(&anim, target);
        lv_anim_set_values(&anim, startOpacity, endOpacity);
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out); // 默认使用缓入缓出效果

        // 如果用户没有提供执行回调，使用默认的回调
        if (exec_callback)
        {
            lv_anim_set_exec_cb(&anim, exec_callback);
            lv_anim_set_user_data(&anim, userData);
        }
        else
        {
            lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
            {
                lv_obj_set_style_opa(static_cast<Obj_t >(var), value, 0); // 更新对象的透明度
            });
        }

        // 设置用户提供的完成回调
        if (completed_callback)
        {
            lv_anim_set_completed_cb(&anim, completed_callback);
        }

        lv_anim_start(&anim);
    }


// 创建旋转动画
/**
 * @brief 创建旋转动画
 * @param target
 * @param angle 角度，其值表示0.1度。比如3600表示360度
 * @param time
 * @param is_relative 是否为相对角度
 * @param exec_callback
 * @param userData
 * @param completed_callback
 * @note 从当前角度到目标角度。需要注意的是，lvgl对角度并没有限制，可以超过360度或者小于0度。
 */
    void createRotationAnimation(Obj_t target, int32_t angle, uint16_t time, bool is_relative = false,
                                 AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                                 AnimCompletedCallback completed_callback = nullptr)
    {
        lv_anim_set_var(&anim, target);
        int32_t startAngle = lv_obj_get_style_transform_angle(target, 0);
        lv_anim_set_values(&anim, startAngle, is_relative ? (startAngle + angle) : angle);
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out); // 默认使用缓入缓出效果

        // 如果用户没有提供执行回调，使用默认的回调
        if (exec_callback)
        {
            lv_anim_set_exec_cb(&anim, exec_callback);
            lv_anim_set_user_data(&anim, userData);
        }
        else
        {
            lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
            {
                lv_obj_set_style_transform_angle(static_cast<Obj_t >(var), value, 0); // 更新旋转角度
            });
        }

        // 设置用户提供的完成回调
        if (completed_callback)
        {
            lv_anim_set_completed_cb(&anim, completed_callback);
        }

        lv_anim_start(&anim);
    }


    /**
     * @brief 创建颜色渐变动画
     * @tparam completed_callback 为了防止忘记释放堆，所以使用模板
     * @param target
     * @param startColor
     * @param endColor
     * @param time
     * @param exec_callback
     * @param userData
     */
    template<AnimCompletedCallback completed_callback = nullptr>
    void createColorAnimation(Obj_t target, Color startColor, Color endColor, uint16_t time,
                              AnimExecCallback exec_callback = nullptr, void *userData = nullptr
    )
    {
        lv_anim_set_var(&anim, target);
        lv_anim_set_values(&anim, 0, 100); // 占位值，实际值在回调中计算
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_linear); // 默认使用线性，这样效果会好很多

        // 如果用户没有提供执行回调，使用默认的回调
        if (exec_callback)
        {
            lv_anim_set_exec_cb(&anim, exec_callback);
            lv_anim_set_user_data(&anim, userData);
        }
        else
        {
            auto *colors = static_cast<Color *>(lv_malloc(2 * sizeof(Color)));
            colors[0] = startColor;
            colors[1] = endColor;
            lv_anim_set_user_data(&anim, colors);
            lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
            {
                lv_anim_t *anim = lv_anim_get(var, nullptr); // 获取当前动画对象
                auto *colors = static_cast<Color *>(anim->user_data);
                // 计算当前颜色
                lv_color_t currentColor = lv_color_make(
                        colors[0].red + (colors[1].red - colors[0].red) * value / 100,
                        colors[0].green + (colors[1].green - colors[0].green) * value / 100,
                        colors[0].blue + (colors[1].blue - colors[0].blue) * value / 100
                );
                lv_obj_set_style_bg_color(static_cast<Obj_t>(var), currentColor, 0); // 更新背景颜色
            });
        }

        // 设置用户提供的完成回调
        lv_anim_set_completed_cb(&anim, [](lv_anim_t *a)
        {
            lv_free(a->user_data);
            if constexpr (completed_callback != nullptr)
            {
                completed_callback(a);
            }
        });


        lv_anim_start(&anim);
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

    obj = gui->main.btn_test.get_obj();
}

void GUI_Base::events_init()
{

}


/****************************************UI接口*********************************/
void UI::pressA()
{

//    animator.createRotationAnimation(obj, 3600, 1000, true);
//    animator.createColorAnimation(obj, lv_color_hex(0xFF0000), lv_color_hex(0x0000FF), 1000);
//animator.createFadeAnimation(obj, LV_OPA_COVER, LV_OPA_TRANSP,1000);
}

void UI::pressS()
{
//    animator.createColorAnimation(obj, lv_color_hex(0x0000FF), lv_color_hex(0xFF0000), 1000);
//animator.createFadeAnimation(obj, LV_OPA_TRANSP, LV_OPA_COVER,1000);

}
