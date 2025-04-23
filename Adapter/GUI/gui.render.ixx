//
// Created by fairy on 2025/4/23 13:16.
//
module;
#include "lvgl.h"
export module gui:render;
export import gui_compose;// 包含所有组件模块并导出


export namespace gui::widgets::main
{
    using namespace gui::compose;// 使用命名空间

    inline Component scr;// 主屏幕，由于会与init里的screen函数重名，所以就简化了名称
}




export namespace gui
{
    constexpr uint16_t DISP_HOR_RES = 480;
    constexpr uint16_t DISP_VER_RES = 320;
    constexpr uint16_t DISP_BUF_SIZE = 20; //20
    constexpr uint8_t BYTE_PER_PIXEL = (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565));

    /**
     * @brief Render类
     * @note 渲染类，用于初始化GUI和运行GUI
     */
    class Render
    {
    public:
        template<void(*lcd_init)(), void (*disp_flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                                       const uint16_t *color_p),
            int32_t (*touchpad_read_xy)(int32_t *last_x, int32_t *last_y) = nullptr>
        static auto init() -> void
        {
            /********初始化LCD*******/
            if constexpr (lcd_init != nullptr)
            {
                lcd_init();
            }

            /********初始化LVGL*******/
            lv_init();

            /******初始化显示设备******/
            if constexpr (disp_flush != nullptr) { disp_drv_init<disp_flush>(); }

            /*****初始化触摸屏******/
            if constexpr (touchpad_read_xy != nullptr) { touchpad_init<touchpad_read_xy>(); }


            /*****初始化GUI组件*****/
            resource_init();
        }

        // GUI处理函数
        static inline auto handler() -> void { lv_task_handler(); }

        // 刷新回调,可供DMA回调函数使用
        static inline auto display_flush_ready() -> void { lv_display_flush_ready(disp); }

        // 获取设备
        static inline auto get_display() -> lv_display_t * { return disp; }
        static inline auto get_indev() -> lv_indev_t * { return indev_touchpad; }


    private:

        static void screenInit();// 由用户实现
        static void eventInit();// 由用户实现

        // 初始化界面
        static auto resource_init() -> void
        {
            widgets::main::scr.init().size(DISP_HOR_RES, DISP_VER_RES);// 默认父对象为nullpter
            compose::Component::parent(widgets::main::scr);// 默认主屏幕为父对象
            screenInit();// 初始化屏幕
            lv_obj_update_layout(widgets::main::scr);// 更新布局
            eventInit();// 初始化事件
            lv_scr_load(widgets::main::scr);// 加载主屏幕
        }


        /**
         * @brief 初始化显示驱动
         * @tparam flush 涂色函数，有LCD驱动提供
         * @note 为了让lambda表达式可以不用捕获外部函数，只能使用函数模板。如果使用函数指针来传递就必须要显示捕获
         */
        template<void (*flush)(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *color_p)>
        static inline auto disp_drv_init() -> void
        {
            disp = lv_display_create(DISP_HOR_RES, DISP_VER_RES);

            // 刷新回调
            lv_display_set_flush_cb(disp, [](lv_display_t *disp_drv, const lv_area_t *area, uint8_t *px_map)
            {
                flush(area->x1, area->y1, area->x2, area->y2, reinterpret_cast<const uint16_t *>(px_map));

                // 只有定义了DMA中断回调才不需要这个函数，那么这就要求DMA中断启用时需要设置相关宏定义
#if !defined(ARM_MATH_CM4) || !defined(DMA_SPI_ENABLE)
                display_flush_ready();
#endif
            });


            // 缓冲区  双缓冲明显优于单缓冲
            LV_ATTRIBUTE_MEM_ALIGN
            static uint8_t buf_2_1[DISP_HOR_RES * DISP_BUF_SIZE * BYTE_PER_PIXEL];
            LV_ATTRIBUTE_MEM_ALIGN
            static uint8_t buf_2_2[DISP_HOR_RES * DISP_BUF_SIZE * BYTE_PER_PIXEL];
            lv_display_set_buffers(disp, buf_2_1, buf_2_2, sizeof(buf_2_1), LV_DISPLAY_RENDER_MODE_PARTIAL);
        }

        /**
         * @brief 初始化触摸屏驱动
         * @tparam touchpad_read_xy
         */
        template<int32_t (*touchpad_read_xy)(int32_t *last_x, int32_t *last_y)>
        static inline auto touchpad_init() -> void
        {
            indev_touchpad = lv_indev_create();
            lv_indev_set_type(indev_touchpad, LV_INDEV_TYPE_POINTER);
            lv_indev_set_read_cb(indev_touchpad, [](lv_indev_t *indev_drv, lv_indev_data_t *data)
            {
                if (touchpad_read_xy(&(data->point.x), &(data->point.y)))
                {
                    data->state = LV_INDEV_STATE_PRESSED;
                }
                else
                {
                    data->state = LV_INDEV_STATE_RELEASED;
                }
            });
        }

        // 显示设备
        static inline lv_display_t *disp;
        static inline lv_indev_t *indev_touchpad;
    };
}
