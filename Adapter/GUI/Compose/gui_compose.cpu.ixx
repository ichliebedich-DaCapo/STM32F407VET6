module;
#include <cstdio>
#include <lvgl.h>
export module gui_compose:cpu;

import :widget;
import :label;


export namespace gui::compose
{
    /**
     * @brief CPU温度显示
     * @details 由于调用的是Label的API，父对象与前面一致，可以使用Component::set_parent来设置父对象
     */
    class cpu
    {
    public:
        static auto init(Coord x = 3, Coord y = 2, Coord width = 90, Coord height = 20) -> void
        {
            label_cpu.init(x, y, width, height, "CPU: -");
        }

        // 显示温度
        template<float (*get_cpu_temperature)()>
        static auto print() -> void;

    private:
        static inline Label label_cpu{};
    };
    // 显示温度
    template<float (*get_cpu_temperature)()>
    auto cpu::print() -> void
    {
        char buf[13];
        sprintf(buf, "CPU: %2.2f°C", get_cpu_temperature());
        label_cpu.text(buf);
    }
}