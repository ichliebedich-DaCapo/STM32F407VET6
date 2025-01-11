//
// Created by fairy on 2025/1/11 01:04.
//
#include "anim.hpp"
// 头文件

void
Animator::createAnimation(Obj target, int32_t start, int32_t end, uint16_t time, AnimPath path,
                          AnimExecCallback exec_callback, void *userData,
                          AnimCompletedCallback completed_callback)
{
    lv_anim_set_var(&anim, target);
    lv_anim_set_values(&anim, start, end); // 占位值，实际值在回调中计算
    lv_anim_set_time(&anim, time);
    lv_anim_set_path_cb(&anim, path); // 默认使用缓入缓出效果
    if (exec_callback)
    {
        lv_anim_set_exec_cb(&anim, exec_callback);
        if (userData)
            lv_anim_set_user_data(&anim, userData);
    }
    if (completed_callback)
    {
        lv_anim_set_completed_cb(&anim, completed_callback);
    }
    lv_anim_start(&anim);
}


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

void Animator::createScrollAnimation(Obj target, int32_t endX, int32_t endY, uint16_t time, bool is_relative)
{
    // 将起始和结束坐标通过 userData 传递
    auto *coords = static_cast<int32_t *>(lv_malloc(4 * sizeof(int32_t)));
    if (coords)
    {
        coords[0] = lv_obj_get_x(target); // startX
        coords[1] = lv_obj_get_y(target); // startY
        coords[2] = is_relative ? coords[0] + endX : endX;                 // endX
        coords[3] = is_relative ? coords[1] + endY : endY;                 // endY

        // 默认使用缓入缓出效果
        createAnimation(target, 0, 100, time, lv_anim_path_ease_in_out,
                        [](void *var, int32_t value)
                        {
                            auto *target = static_cast<Obj_t>(var);
                            lv_anim_t *anim = lv_anim_get(var, nullptr); // 获取当前动画对象
                            auto *coords = static_cast<int32_t *>(anim->user_data);
                            int32_t currentX = coords[0] + (coords[2] - coords[0]) * value / 100;
                            int32_t currentY = coords[1] + (coords[3] - coords[1]) * value / 100;
                            lv_obj_set_pos(target, currentX, currentY);
                        }, coords, [](Anim_t *anim) { lv_free(anim->user_data); });

    }
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
void Animator::createFadeAnimation(Obj target, int32_t startOpacity, int32_t endOpacity, uint16_t time)
{
    createAnimation(target, startOpacity, endOpacity, time, lv_anim_path_ease_in_out,
                    [](void *var, int32_t value)
                    {
                        lv_obj_set_style_opa(static_cast<Obj_t>(var), value, 0); // 更新对象的透明度
                    });

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
void Animator::createRotationAnimation(Obj target, int32_t angle, uint16_t time, bool is_relative)
{
    int32_t startAngle = lv_obj_get_style_transform_angle(target, 0);
    lv_anim_set_path_cb(&anim, lv_anim_path_ease_in_out); // 默认使用缓入缓出效果
    createAnimation(target, startAngle, is_relative ? (startAngle + angle) : angle, time, lv_anim_path_ease_in_out,
                    [](void *var, int32_t value)
                    {
                        // 更新旋转角度
                        lv_obj_set_style_transform_angle(static_cast<Obj_t>(var), value, 0);
                    });
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
void Animator::createColorAnimation(Obj_t target, Color startColor, Color endColor, uint16_t time)
{

    auto *colors = static_cast<Color *>(lv_malloc(2 * sizeof(Color)));
    colors[0] = startColor;
    colors[1] = endColor;

    createAnimation(target, 0, 100, time, lv_anim_path_linear, [](void *var, int32_t value)
    {
        lv_anim_t *anim = lv_anim_get(var, nullptr); // 获取当前动画对象
        auto *colors = static_cast<Color *>(anim->user_data);
        // 计算当前颜色
        Color currentColor = lv_color_make(
                colors[0].red + (colors[1].red - colors[0].red) * value / 100,
                colors[0].green + (colors[1].green - colors[0].green) * value / 100,
                colors[0].blue + (colors[1].blue - colors[0].blue) * value / 100
        );
        lv_obj_set_style_bg_color(static_cast<Obj_t>(var), currentColor, 0); // 更新背景颜色
    }, colors, [](Anim_t *anim) { lv_free(anim->user_data); });


}

/**
 * @brief 创建缩放弹跳动画
 * @param target
 * @param startScale
 * @param endScale
 * @param time
 * @param exec_callback
 * @param userData
 * @param completed_callback
 */
void Animator::createScaleBounceAnimation(Obj target, int32_t startScale, int32_t endScale, uint16_t time)
{

    createAnimation(target, startScale, endScale, time, lv_anim_path_bounce,
                    [](void *var, int32_t value)
    {
        auto *target = static_cast<Obj_t>(var);
        lv_obj_set_style_transform_width(target, value, 0); // 更新宽度
        lv_obj_set_style_transform_height(target, value, 0); // 更新高度
    });
}

/**暂时不能用，因为没有解决创建对象逐渐增多，这会导致卡死**/
void Animator::createParticleEffect(Obj_t parent, int32_t particleCount, uint16_t time,
                                    AnimCompletedCallback completed_callback)
{
    for (int i = 0; i < particleCount; i++)
    {
        Obj_t particle = lv_obj_create(parent);
        lv_obj_set_size(particle, 5, 5);
        lv_obj_set_style_bg_color(particle, lv_color_hex(0xFF0000), 0);

        lv_anim_init(&anim);
        lv_anim_set_var(&anim, particle);
        lv_anim_set_values(&anim, 0, 100);
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_ease_out);

        lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
        {
            auto *particle = static_cast<Obj_t>(var);
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