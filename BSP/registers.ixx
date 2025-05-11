//
// Created by fairy on 2025/4/22 22:20.
//
module;
export module hw_registers;

#include <type_traits>

export namespace bsp::hw
{
    /**
     * @brief 硬件寄存器访问模板类
     * @tparam RegisterAddress 寄存器物理地址
     * @tparam RegisterWidth 寄存器位宽（默认uint32_t）
     *
     * @details 提供类型安全的寄存器访问接口，支持原子操作和位操作
     */
    template<unsigned int RegisterAddress, typename RegisterWidth = unsigned int>
    class Register
    {
        static_assert(std::is_unsigned_v<RegisterWidth>,
                      "Register type must be unsigned integral");

        using register_type = volatile RegisterWidth;

    public:
        // 基础访问方法
        static void write(RegisterWidth value) noexcept
        {
            *reinterpret_cast<register_type *>(RegisterAddress) = value;
        }

        static RegisterWidth read() noexcept
        {
            return *reinterpret_cast<register_type *>(RegisterAddress);
        }

        // 位操作增强方法
        static void set_bits(RegisterWidth mask) noexcept
        {
            write(read() | mask);
        }

        static void clear_bits(RegisterWidth mask) noexcept
        {
            write(read() & ~mask);
        }

        static void modify(RegisterWidth clear_mask, RegisterWidth set_mask) noexcept
        {
            const auto val = (read() & ~clear_mask) | set_mask;
            write(val);
        }
    };
}

export template<unsigned int Address,typename RegisterWidth = unsigned int>
using Register = bsp::hw::Register<Address,RegisterWidth>;
