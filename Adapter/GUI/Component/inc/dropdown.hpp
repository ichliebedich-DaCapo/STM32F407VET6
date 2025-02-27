//
// Created by DV on 2025/2/10.
//

#ifndef SIMULATOR_DROPDOWN_HPP
#define SIMULATOR_DROPDOWN_HPP

#include "widget.hpp"

/**
 * @class Dropdown
 * @brief 封装LVGL下拉列表操作的组件类
 * @tparam Dropdown CRTP模板参数，实现静态多态
 * @inherits Widget<Dropdown> 继承自基础组件类
 *
 * 提供完整的下拉列表创建、配置和状态获取功能，支持链式调用。
 * 注意：所有方法必须在LVGL主线程上下文调用。
 */
class Dropdown : public Widget<Dropdown>
{
public:
    /**
     * @brief 基础初始化方法
     * @return Dropdown& 返回当前对象引用，支持链式调用
     *
     * 创建基础下拉列表对象，需后续配置位置和样式。
     * 示例：
     * @code
     * Dropdown().init().pos(100,50).options("A\\nB");
     * @endcode
     */
    Dropdown& init()
    {
        create_obj(&lv_dropdown_class);
        return *this;
    }

    /**
     * @brief 带参数的完整初始化
     * @param x X坐标
     * @param y Y坐标
     * @param w 宽度
     * @param h 高度
     * @param bg_color 背景颜色，默认#2195f6
     * @param opa 透明度(0-255)，默认125
     * @return Dropdown& 当前对象引用
     *
     * 创建并立即配置基本样式参数的下拉列表。
     * 典型用法：
     * @code
     * Dropdown().init(0,0,200,80);
     * @endcode
     */
    Dropdown& init(Coord x, Coord y, Coord w, Coord h,const char* options="",
                 Color bg_color = lv_color_hex(0x2195f6), uint8_t opa = 125)
    {
        init();
        pos_size(x, y, w, h);
        Dropdown::bg_color(bg_color, opa);
        Dropdown::options(options);
        return *this;
    }

    /**
     * @brief 设置下拉按钮显示文本
     * @param txt 要显示的文本（传NULL则显示选中项）
     * @return Dropdown& 当前对象引用
     *
     * 注意：文本指针会被LVGL保存，需确保生命周期。
     * 示例：
     * @code
     * dd.text("Choose:");  // 固定显示"Choose:"
     * @endcode
     */
    Dropdown& text(const char* txt) {
        lv_dropdown_set_text(obj_, txt);
        return *this;
    }

    /**
     * @brief 设置动态选项列表
     * @param options 以'\n'分隔的选项字符串
     * @return Dropdown& 当前对象引用
     *
     * 选项会被复制到内部缓冲区，可安全释放原始字符串。
     * 示例：
     * @code
     * options("Apple\\nBanana\\nOrange");
     * @endcode
     */
    Dropdown& options(const char* options) {
        lv_dropdown_set_options(obj_, options);
        return *this;
    }

    /**
     * @brief 设置静态选项列表
     * @param options 以'\n'分隔的静态字符串
     * @return Dropdown& 当前对象引用
     *
     * 直接引用外部字符串，需确保其生命周期长于下拉列表。
     * 典型用于常量字符串：
     * @code
     * setStaticOptions("OK\\nCancel\\nExit");
     * @endcode
     */
    Dropdown& setStaticOptions(const char* options) {
        lv_dropdown_set_options_static(obj_, options);
        return *this;
    }

    /**
     * @brief 添加单个选项
     * @param option 要添加的选项字符串（不得含'\n'）
     * @param pos 插入位置（默认最后）
     * @return Dropdown& 当前对象引用
     *
     * 仅适用于动态选项列表，静态列表调用此方法无效。
     * 示例：
     * @code
     * addOption("New Item", 0); // 插入到首位
     * @endcode
     */
    Dropdown& addOption(const char* option, uint32_t pos = LV_DROPDOWN_POS_LAST) {
        lv_dropdown_add_option(obj_, option, pos);
        return *this;
    }

    /**
     * @brief 清空所有选项
     * @return Dropdown& 当前对象引用
     *
     * 对静态/动态选项列表均有效，清空后需重新设置选项。
     */
    Dropdown& clearOptions() {
        lv_dropdown_clear_options(obj_);
        return *this;
    }

    /**
     * @brief 设置选中项索引
     * @param index 选项索引（0-based）
     * @return Dropdown& 当前对象引用
     *
     * 若索引超出范围，实际选中项不会改变。
     * 示例：
     * @code
     * selected(2); // 选择第三项
     * @endcode
     */
    Dropdown& selected(uint32_t index) {
        lv_dropdown_set_selected(obj_, index);
        return *this;
    }

    /**
     * @brief 设置下拉方向
     * @param dir 方向枚举值（LV_DIR_LEFT/RIGHT/TOP/BOTTOM）
     * @return Dropdown& 当前对象引用
     *
     * 默认方向为LV_DIR_BOTTOM（向下展开）
     */
    Dropdown& direction(lv_dir_t dir) {
        lv_dropdown_set_dir(obj_, dir);
        return *this;
    }

    /**
     * @brief 设置下拉按钮符号
     * @param sym 符号类型，可以是：
     *            - LV_SYMBOL_* 系统符号
     *            - 图像指针（需确保生命周期）
     *            - 文本符号（如"▼"）
     * @return Dropdown& 当前对象引用
     *
     * 传NULL可移除符号显示。
     */
    Dropdown& symbol(const void* sym) {
        lv_dropdown_set_symbol(obj_, sym);
        return *this;
    }

    /**
     * @brief 设置选中项高亮
     * @param enable 是否启用高亮（默认true）
     * @return Dropdown& 当前对象引用
     */
    Dropdown& highlight(bool enable = true) {
        lv_dropdown_set_selected_highlight(obj_, enable);
        return *this;
    }

    //------------------- Getter Methods -------------------//

    /**
     * @brief 获取按钮当前显示文本
     * @return const char* 文本内容指针（可能为NULL）
     */
    [[nodiscard]] const char* getText() const {
        return lv_dropdown_get_text(obj_);
    }

    /**
     * @brief 获取完整选项列表
     * @return const char* 以'\n'分隔的选项字符串
     */
    [[nodiscard]] const char* getOptions() const {
        return lv_dropdown_get_options(obj_);
    }

    /**
     * @brief 获取当前选中项索引
     * @return uint32_t 选中项索引（0-based）
     */
    [[nodiscard]] uint32_t getSelected() const {
        return lv_dropdown_get_selected(obj_);
    }

    /**
     * @brief 获取选项总数
     * @return uint32_t 总选项数
     */
    [[nodiscard]] uint32_t optionCount() const {
        return lv_dropdown_get_option_count(obj_);
    }

    /**
     * @brief 获取选中项字符串（C风格接口）
     * @param buf 输出缓冲区
     * @param size 缓冲区大小
     *
     * 建议先通过optionCount()确认最大所需缓冲区大小。
     */
    void getSelectedStr(char* buf, uint32_t size) const {
        lv_dropdown_get_selected_str(obj_, buf, size);
    }

    /**
     * @brief 查找选项索引
     * @param option 要查找的选项字符串
     * @return int32_t 选项索引（未找到返回-1）
     */
    [[nodiscard]] int32_t findOption(const char* option) const {
        return lv_dropdown_get_option_index(obj_, option);
    }

    /**
     * @brief 获取当前设置的符号
     * @return const void* 符号指针（可能为NULL）
     */
    [[nodiscard]] const void* getSymbol() const {
        return lv_dropdown_get_symbol(obj_);
    }

    /**
     * @brief 检查是否启用高亮
     * @return bool 高亮状态
     */
    [[nodiscard]] bool isHighlighted() const {
        return lv_dropdown_get_selected_highlight(obj_);
    }

    /**
     * @brief 获取当前下拉方向
     * @return lv_dir_t 方向枚举值
     */
    [[nodiscard]] lv_dir_t getDirection() const {
        return lv_dropdown_get_dir(obj_);
    }
};


#endif //SIMULATOR_DROPDOWN_HPP