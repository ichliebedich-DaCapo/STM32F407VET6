//
// Created by fairy on 2024/12/30 23:43.
//
export module Flags;

export
{
#include <cstdint>

    /**
     * @brief 标志类
     * @tparam enum_type 枚举类型
     * @tparam variable_type 存储标志位的变量类型
     * @note 该模板类由于使用的是静态变量来存储标志，所以目前对于同一个标志类型只能实例化一次
     */
    template <typename enum_type,typename variable_type=uint8_t>
    class Flags
    {
    public:
        // 辅助函数：设置指定标志
        static void set_flag(enum_type flag)
        {
            flags |= static_cast<variable_type>(flag);
        }

        // 辅助函数：清除指定标志
        static void clear_flag(enum_type flag)
        {
            flags &= ~static_cast<variable_type>(flag);
        }

        // 辅助函数：获取指定标志位
        static bool get_flag(enum_type flag)
        {
            return flags & static_cast<variable_type>(flag);
        }

        // 辅助函数：翻转指定标志
        static void toggle_flag(enum_type flag)
        {
            flags ^= static_cast<variable_type>(flag);
        }

        // 辅助函数：检查指定标志，返回为true的同时会清除标志位
        static bool check_flag(enum_type flag)
        {
            bool flag_result = get_flag(flag);
            if(flag_result)
            {
                clear_flag(flag);
                return true;
            }
            return false;
        }

    private:
        static inline variable_type flags = 0;
    };
}
