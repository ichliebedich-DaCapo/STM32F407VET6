module;
#include <lvgl.h>
export module gui_compose:scale;

import :widget;

export namespace gui::compose
{
    /*****注意事项******/

    /**
     * @brief 图表类
     * @note 在类中要加像这样的函数 auto Osc::initData() -> void
        series=chart_screen_chart_1.update_mode(LV_CHART_UPDATE_MODE_SHIFT)  // 改为SHIFT模式\n
                .line_color(lv_color_hex(0x34e6ff))\n
                .point_count(128)\n
                .remove_dot()\n
                .add_series(lv_color_hex(0x34e6ff));\n
        cursor=chart_screen_chart_1.add_cursor(lv_color_hex(0xfffb00),LV_DIR_ALL);\n
     *
     */
    /***** 使用模版封装 Scale 类 *****/

    /**
     * @brief 刻度尺类
     * @note 封装 LVGL 的 Scale 组件，支持线性/环形刻度尺、刻度分区、指针等特性\n
             GUI-GUIDER有bug,默认状态下，GUI-GUIDER图形界面无边框，但生成的代码模拟器运行会有边框，
     */
    class Scale : public Widget<Scale> {
    public:
        // 初始化刻度尺
        inline Scale &init(Obj_t parent = parent_) {
            create_obj(&lv_scale_class, parent);
            return *this;
        }

        /**
         * @brief 初始化并配置基础参数
         * @param mode 刻度模式 (LV_SCALE_MODE_HORIZONTAL_TOP/LEFT/RIGHT/BOTTOM 或 LV_SCALE_MODE_VERTICAL_LEFT/RIGHT 等)
         * @param total_tick 总刻度数 (含主次刻度)
         * @param major_tick_every 主刻度间隔
         * @param min 最小值
         * @param max 最大值
         */
        Scale &init(ScareMode mod, uint32_t total_tick, uint32_t major_tick_every, int32_t min, int32_t max) {
            init();
            mode(mod)
                    .total_tick_count(total_tick)
                    .major_tick_every(major_tick_every)
                    .range(min, max);
            return *this;
        }

        /****************** 基础配置方法 **********************/

        /** 设置刻度模式 */
        Scale &mode(ScareMode mode) {
            lv_scale_set_mode(obj_, mode);
            return *this;
        }

        /** 设置总刻度数 */
        Scale &total_tick_count(uint32_t count) {
            lv_scale_set_total_tick_count(obj_, count);
            return *this;
        }

        /** 设置主刻度间隔 */
        Scale &major_tick_every(uint32_t every) {
            lv_scale_set_major_tick_every(obj_, every);
            return *this;
        }

        /** 显示/隐藏标签 */
        Scale &label_show(bool show=true) {
            lv_scale_set_label_show(obj_, show);
            return *this;
        }

        /** 设置量程范围 */
        Scale &range(int32_t min, int32_t max) {
            lv_scale_set_range(obj_, min, max);
            return *this;
        }

        /** 设置环形刻度角度范围 (仅 LV_SCALE_MODE_ROUND_*) */
        Scale &angle_range(uint32_t angle) {
            lv_scale_set_angle_range(obj_, angle);
            return *this;
        }

        /** 设置环形刻度旋转偏移 (仅 LV_SCALE_MODE_ROUND_*) */
        Scale &rotation(int32_t rotation) {
            lv_scale_set_rotation(obj_, rotation);
            return *this;
        }

        /****************** 指针配置方法 **********************/

        /**
         * @brief 设置线型指针值
         * @param needle_line 线对象 (需提前创建)
         * @param length 指针长度 (>0绝对值，<0相对半径)
         * @param value 指向的数值
         */
        Scale &line_needle(lv_obj_t* needle_line, int32_t length, int32_t value) {
            lv_scale_set_line_needle_value(obj_, needle_line, length, value);
            return *this;
        }

        /**
         * @brief 设置图像指针值
         * @param needle_img 图像对象 (需指向右侧，如 -O------>)
         * @param value 指向的数值
         */
        Scale &image_needle(lv_obj_t* needle_img, int32_t value) {
            lv_scale_set_image_needle_value(obj_, needle_img, value);
            return *this;
        }

        /****************** 高级配置方法 **********************/

        /** 设置自定义标签文本源 */
        Scale &text_src(const char* txt_src[]) {
            lv_scale_set_text_src(obj_, txt_src);
            return *this;
        }

        /** 启用后绘制 (在所有子对象之后绘制刻度) */
        Scale &post_draw(bool en) {
            lv_scale_set_post_draw(obj_, en);
            return *this;
        }

        /** 在顶层绘制刻度 */
        Scale &draw_ticks_on_top(bool en) {
            lv_scale_set_draw_ticks_on_top(obj_, en);
            return *this;
        }

        /****************** 刻度分区操作 **********************/

        /** 添加刻度分区 */
        ScareSection_t add_section() {
            return lv_scale_add_section(obj_);
        }

        /** 设置分区范围 */
        static void section_set_range(ScareSection_t section, int32_t minor, int32_t major) {
            lv_scale_section_set_range(section, minor, major);
        }

        /** 设置分区样式 */
        static void section_set_style(ScareSection_t section, lv_part_t part, lv_style_t* style) {
            lv_scale_section_set_style(section, part, style);
        }

        /****************** 获取属性方法 **********************/

        /** 获取当前模式 */
        ScareMode get_mode() const {
            return lv_scale_get_mode(obj_);
        }

        /** 获取总刻度数 */
        int32_t get_total_tick() const {
            return lv_scale_get_total_tick_count(obj_);
        }

        /** 获取主刻度间隔 */
        int32_t get_major_tick_every() const {
            return lv_scale_get_major_tick_every(obj_);
        }

        /** 获取标签显示状态 */
        bool get_label_show() const {
            return lv_scale_get_label_show(obj_);
        }

        /** 获取角度范围 */
        uint32_t get_angle_range() const {
            return lv_scale_get_angle_range(obj_);
        }

        /** 获取最小值 */
        int32_t get_min() const {
            return lv_scale_get_range_min_value(obj_);
        }

        /** 获取最大值 */
        int32_t get_max() const {
            return lv_scale_get_range_max_value(obj_);
        }
    };
}