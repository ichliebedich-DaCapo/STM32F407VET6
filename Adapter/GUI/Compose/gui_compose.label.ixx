module;
#include <lvgl.h>
export module gui_compose:label;

import :widget;

export namespace gui::compose
{
    /**
     * @brief 文本框类
     * @details 如果文本不设置字体就传入字符串，那么会卡死
     */
    class Label : public Widget<Label>
    {
    public:

        // 初始化文本框，默认为黑色字体
        inline Label &init(Obj_t parent = parent_)
        {
            create_obj(&lv_label_class, parent);
            text_color(lv_color_black());// 设置文本颜色为黑色
            return *this;
        }

        // 只要设置文本，就默认为黑色字体
        inline Label &init(const Strings text, const Font font_,Obj parent = parent_)
        {
            init(parent);
            // 初始化文本内容
            lv_label_set_text(obj_, text);
            lv_obj_set_style_text_font(obj_, font_, selector_default);
            return *this;
        }


        // 文本颜色默认为黑,感觉这个最通用
        inline Label &init(Coord x, Coord y, Coord w, Coord h,Strings text="",Color text_color=lv_color_black())
        {
            init();
            pos_size(x, y, w, h);
            lv_label_set_text(obj_, text);
            Label::text_color(text_color);// 设置文本颜色为黑色
            return *this;
        }


        // 设置字间距
        inline Label &space(const Coord space, const Selector selector = selector_default)
        {
            lv_obj_set_style_text_letter_space(obj_, space, selector);
            return *this;
        }

        // 设置文本对齐方式
        inline Label &text_align(Align_text align, Selector selector = selector_default)
        {
            lv_obj_set_style_text_align(obj_, align, selector);
            return *this;
        }

        // 设置字体
        inline Label &font(const Font font)
        {
            lv_obj_set_style_text_font(obj_, font, selector_default);
            return *this;
        }

        inline Label &font(Font_t &font)
        {
            lv_obj_set_style_text_font(obj_, &font, selector_default);
            return *this;
        }


        /**
         * @brief 设置文本
         * @note 不使用auto来简化，因为会导致二进制膨胀
         * @return
         */
        Label &text(Strings text)
        {
            lv_label_set_text(obj_, text);
            return *this;
        }
    };
}

