//
// Created by fairy on 2025/1/11 01:04.
//
#include "anim.hpp"
// 头文件
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
void Animator::createFadeAnimation(Obj_t target, int32_t startOpacity, int32_t endOpacity, uint16_t time,
                         AnimExecCallback exec_callback, void *userData,
                         AnimCompletedCallback completed_callback)
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
void Animator::createRotationAnimation(Obj_t target, int32_t angle, uint16_t time, bool is_relative,
                             AnimExecCallback exec_callback, void *userData,
                             AnimCompletedCallback completed_callback)
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



/**暂时不能用，因为没有解决创建对象逐渐增多，这会导致卡死**/
void Animator::createParticleEffect(lv_obj_t *parent, int32_t particleCount, uint16_t time,
                          AnimCompletedCallback completed_callback)
{
    for (int i = 0; i < particleCount; i++)
    {
        lv_obj_t *particle = lv_obj_create(parent);
        lv_obj_set_size(particle, 5, 5);
        lv_obj_set_style_bg_color(particle, lv_color_hex(0xFF0000), 0);

        lv_anim_init(&anim);
        lv_anim_set_var(&anim, particle);
        lv_anim_set_values(&anim, 0, 100);
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

        lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
        {
            auto *particle = static_cast<lv_obj_t *>(var);
            int32_t x = value * lv_rand(1, 10);
            int32_t y = value * lv_rand(1, 10);
            lv_obj_set_pos(particle, x, y);
            lv_obj_set_style_opa(particle, 100 - value, 0); // 逐渐消失
        });

        lv_anim_start(&anim);
    }

    if (completed_callback)
    {
        lv_anim_set_completed_cb(&anim, completed_callback);
    }
}