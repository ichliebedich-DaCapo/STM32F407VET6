//
// Created by fairy on 2025/1/11 15:18.
//
#ifndef FURINA_CPU_HPP
#define FURINA_CPU_HPP

#include <cstdio>
#include "label.hpp"

/**
 * @brief CPU温度显示
 * @details 由于调用的是Label的API，父对象与前面一致，可以使用Component::set_parent来设置父对象
 */
class CPU
{
public:
    static auto init(Coord x = 3, Coord y = 2, Coord width = 90, Coord height = 20) -> void;

    // 显示温度
    template<float (*get_cpu_temperature)()>
    static auto print() -> void;

private:
    static inline Label label_cpu{};
};
// 显示温度
template<float (*get_cpu_temperature)()>
auto CPU::print() -> void
{
    char buf[13];
    sprintf(buf, "CPU: %2.2f°C", get_cpu_temperature());
    label_cpu.text(buf);
}

#endif //FURINA_CPU_HPP