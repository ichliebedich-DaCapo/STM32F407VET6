//
// Created by fairy on 2025/1/11 01:04.
//
#ifndef SIMULATOR_ANIM_HPP
#define SIMULATOR_ANIM_HPP

#include "component.hpp"

class Animator
{
public:
    // 初始化动画对象
    Animator()
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

    // 创建淡入淡出动画
    void createFadeAnimation(Obj_t target, int32_t startOpacity, int32_t endOpacity, uint16_t time,
                             AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                             AnimCompletedCallback completed_callback = nullptr);



    // 创建旋转动画
    void createRotationAnimation(Obj_t target, int32_t angle, uint16_t time, bool is_relative = false,
                                 AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                                 AnimCompletedCallback completed_callback = nullptr);


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
    void createScaleBounceAnimation(lv_obj_t *target, int32_t startScale, int32_t endScale, uint16_t time,
                                    AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                                    AnimCompletedCallback completed_callback = nullptr)
    {
        lv_anim_set_var(&anim, target);
        lv_anim_set_values(&anim, startScale, endScale);
        lv_anim_set_time(&anim, time);
        lv_anim_set_path_cb(&anim, lv_anim_path_bounce); // 使用弹跳路径

        if (exec_callback)
        {
            lv_anim_set_exec_cb(&anim, exec_callback);
            lv_anim_set_user_data(&anim, userData);
        }
        else
        {
            lv_anim_set_exec_cb(&anim, [](void *var, int32_t value)
            {
                auto *target = static_cast<lv_obj_t *>(var);
                lv_obj_set_style_transform_width(target, value, 0); // 更新宽度
                lv_obj_set_style_transform_height(target, value, 0); // 更新高度
            });
        }

        if (completed_callback)
        {
            lv_anim_set_completed_cb(&anim, completed_callback);
        }

        lv_anim_start(&anim);
    }

    /**暂时不能用，因为没有解决创建对象逐渐增多，这会导致卡死**/
    void createParticleEffect(lv_obj_t *parent, int32_t particleCount, uint16_t time,
                              AnimCompletedCallback completed_callback = nullptr);


    // 停止动画
    void stop()
    {
        lv_anim_del(&anim, nullptr);
    }

private:
    Anim_t anim{}; // LVGL动画对象
};

#endif //SIMULATOR_ANIM_HPP