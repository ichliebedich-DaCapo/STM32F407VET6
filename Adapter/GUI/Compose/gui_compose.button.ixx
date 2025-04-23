module;
#include <lvgl.h>
export module gui_compose:button;
import :widget;

export namespace gui::compose
{
    class Button : public Widget<Button>
    {
    public:
        /**
         * @brief 设置全局字体，但不会改变组件的font
         * @param parent
         */
        inline Button& init(Obj_t parent=parent_)
        {
            create_obj(&lv_button_class, parent);
            return *this;
        }

        // 点击事件
        inline Button &click()
        {
            send_event<LV_EVENT_CLICKED>();
            return *this;
        }

        // 设置字体类型
        inline Button &font(const Font font)
        {
            lv_obj_set_style_text_font(obj_, font, selector_default);
            return *this;
        }
        inline Button &font(Font_t& font)
        {
            lv_obj_set_style_text_font(obj_, &font, selector_default);
            return *this;
        }
    };
}

