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
    Animator(){lv_anim_init(&anim);}

    void
    createAnimation(Obj target, int32_t start, int32_t end, uint16_t time, AnimPath path = lv_anim_path_ease_in_out,
                    AnimExecCallback exec_callback = nullptr, void *userData = nullptr,
                    AnimCompletedCallback completed_callback = nullptr);


    // 创建滚动动画（从当前位置滑动到目标位置）
    void createScrollAnimation(Obj target, int32_t endX, int32_t endY, uint16_t time, bool is_relative = false);

    // 创建淡入淡出动画
    void createFadeAnimation(Obj target, int32_t startOpacity, int32_t endOpacity, uint16_t time);


    // 创建旋转动画
    void createRotationAnimation(Obj target, int32_t angle, uint16_t time, bool is_relative = false);


    // 创建颜色动画
    void createColorAnimation(Obj_t target, Color startColor, Color endColor, uint16_t time);


    // 创建缩放弹跳动画
    void createScaleBounceAnimation(Obj target, int32_t startScale, int32_t endScale, uint16_t time);

    /**暂时不能用，因为没有解决创建对象逐渐增多，这会导致卡死**/
    void createParticleEffect(lv_obj_t *parent, int32_t particleCount, uint16_t time,
                              AnimCompletedCallback completed_callback = nullptr);


    // 停止动画
    void stop() { lv_anim_del(&anim, nullptr); }

private:
    Anim_t anim{}; // LVGL动画对象
};

#endif //SIMULATOR_ANIM_HPP