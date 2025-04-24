module;

#include <array>
#include <project_config.h>

export module lcd;

import spi;
import fsmc;
import hw_registers;// 硬件抽象寄存器
import gpio;


// ==================== 属性 ====================
// 寄存器
using TFT_CMD = Register<0x60060000>; // TFT命令寄存器片选地址
using TFT_DATA = Register<0x60060002>; // TFT数据寄存器片选地址
using TFT_RST = Register<0x60060004>; // TFT复位寄存器地址
using TFT_LED = Register<0x60060008>; // TFT背光寄存器地址
constexpr uint32_t TFT_DATA_ADDR = 0x60060002U;
// 引脚
using LCD_RST = bsp::gpio<GPIOB_BASE,GPIO_PIN_15>; // 复位引脚
using LCD_RS = bsp::gpio<GPIOB_BASE,GPIO_PIN_13>; // 命令数据引脚 高电平为数据


// =========== 配置信息 ============
export namespace bsp::lcd
{
    // 接口类型
    enum class InterfaceType
    {
        Parallel8080, // 8080并口
        SPI, // SPI接口
    };

    // 设备类型
    enum class DeviceType
    {
        ILI9481, // 实验平台上的那块，只能8080并口
        ILI9488, // 实验平台上的那块，只能8080并口
        ST7796, //  自己买的，外接的触摸屏，只能SPI
    };

    // DMA传输配置
    enum class DMAConfigType
    {
        disable, // 不使用DMA传输
        enable, // 使用DMA传输
    };

    enum class Rotation { Deg0, Deg90, Deg180, Deg270 };
}

namespace bsp::lcd::detail
{
    // =========== 初始化命令结构 ===========
    struct InitCommand
    {
        uint8_t cmd; // 命令
        std::array<uint8_t, 16> data; // 参数
        uint8_t data_size; // 实际参数数量
        uint16_t delay_ms = 0; // 延时

        // 支持编译期构造的 constexpr 构造函数
        constexpr InitCommand(
            const uint8_t cmd,
            const std::initializer_list<uint8_t> args,
            const uint16_t delay = 0
        ) : cmd(cmd), data{}, data_size(args.size()), delay_ms(delay)
        {
            std::copy(args.begin(), args.end(), data.begin());
        }
    };

    // =========== 接口策略模板 ===========
    template<InterfaceType T>
    struct InterfacePolicy;

    // SPI传输数据接口
    template<>
    struct InterfacePolicy<InterfaceType::SPI>
    {
        static inline void write_cmd(const uint8_t cmd)
        {
            spi::cs_low();
            LCD_RS::low(); // 低电平发送命令
            spi::send_byte(cmd);
            spi::cs_high();
        }

        static inline void write_data(const uint8_t data)
        {
            spi::cs_low();
            LCD_RS::high(); // 高电平发送数据
            spi::send_byte(data);
            spi::cs_high();
        }

        template<const auto& Cmds, size_t... I> // 关键修改：Cmds作为第一个模板参数
        static inline void send_sequence_impl(std::index_sequence<I...>)
        {
            (([&]{
                constexpr auto& cmd = Cmds[I]; // 编译期访问
                write_cmd(cmd.cmd);
                for (auto d : cmd.data) spi::send_byte(d);
                if constexpr (cmd.delay_ms != 0) HAL_Delay(cmd.delay_ms);
            }()), ...);
        }

        template<const auto& Cmds> // 只需要传递数组引用
        static inline void send_sequence()
        {
            send_sequence_impl<Cmds>(std::make_index_sequence<Cmds.size()>{});
        }
    };

    // 8080并口传输数据接口
    template<>
    struct InterfacePolicy<InterfaceType::Parallel8080>
    {
        static inline void write_cmd(const uint8_t cmd) { TFT_CMD::write(cmd); }

        static inline void write_data(const uint16_t data) { TFT_DATA::write(data); }

        template<size_t N>
        static void send_sequence(const std::array<InitCommand, N> &cmds)
        {
            for (const auto &cmd: cmds)
            {
                write_cmd(cmd.cmd);
                // 只发送有效数据（根据 data_size）
                for (size_t i = 0; i < cmd.data_size; ++i)
                {
                    TFT_DATA::write(cmd.data[i]);
                }
                if constexpr (cmd.delay_ms) HAL_Delay(cmd.delay_ms);
            }
        }
    };

    // =========== LCD初始化特化 ===========
        template<DeviceType T>
    struct InitSequence;

    template<>
struct InitSequence<DeviceType::ILI9488>
    {
        static inline constexpr auto value = std::array{
            // ILI9488初始化命令与参数
            InitCommand{0x11, {}, 5}, // 退出睡眠模式
            InitCommand{0xD0, {0x07, 0x47, 0x19}}, // 电源控制命令
            InitCommand{0xD1, {0x00, 0x36, 0x1F}}, // 电源控制命令
            InitCommand{0xD2, {0x01, 0x11}}, // 电源控制命令
            InitCommand{0xE4, {0xA0}}, // 驱动模式设置
            InitCommand{0xF3, {0x00, 0x2A}}, // 帧速率控制
            InitCommand{0xC0, {0x10, 0x3B, 0x00, 0x02, 0x11}}, // MV偏压控制
            InitCommand{0xC5, {0x03}}, // VCOM控制
            InitCommand{0xC8, {0x00, 0x35, 0x23, 0x07, 0x00, 0x04, 0x45, 0x53, 0x77, 0x70, 0x00, 0x04}}, // Gamma设置
            InitCommand{0x36, {0xE9}}, // 显示行列设置
            InitCommand{0x3A, {0x55}}, // RGB信号格式设置
            InitCommand{0x20, {}, 0}, // RAM写入控制
            InitCommand{0x2A, {0x00, 0x00, 0x01, 0xDF}}, // 水平地址设置
            InitCommand{0x2B, {0x00, 0x00, 0x01, 0x3F}, 5}, // 垂直地址设置
            InitCommand{0x29, {}}, // 唤醒命令
            InitCommand{0x2C, {}, 5} // 写入RAM命令
        };
    };

    template<>
struct InitSequence<DeviceType::ILI9481>
    {
        static inline constexpr auto value = std::array{
            // ILI9481初始化命令与参数
            InitCommand{0x11, {}, 5}, // 退出睡眠模式
            InitCommand{0xD0, {0x07, 0x41, 0x18}}, // 电源控制1 (注意参数与ILI9488不同)
            InitCommand{0xD1, {0x00, 0x0A, 0x10}}, // 电源控制2 (0x000a转换为0x0A)
            InitCommand{0xD2, {0x01, 0x11}}, // 电源控制3
            InitCommand{0xC0, {0x10, 0x3B, 0x00, 0x02, 0x11}}, // 面板驱动控制
            InitCommand{0xC1, {0x10, 0x13, 0x88}}, // 时序控制1
            InitCommand{0xC5, {0x02}}, // VCOM控制 (电压与ILI9488不同)
            InitCommand{0xC8, {0x00, 0x37, 0x25, 0x06, 0x04, 0x1E, 0x26, 0x42, 0x77, 0x44, 0x0F, 0x12}}, // Gamma设置
            InitCommand{0xF3, {0x40, 0x0A}}, // 帧速率控制 (参数格式不同)
            InitCommand{0xF6, {0x80}}, // 接口控制
            InitCommand{0xF7, {0x80}}, // 面板控制
            InitCommand{0x36, {0x2F}}, // 屏幕旋转180度 (0x2F方向参数)
            InitCommand{0x3A, {0x55}}, // RGB565格式
            InitCommand{0x20, {}}, // 显存写入模式
            InitCommand{0x2A, {0x00, 0x00, 0x01, 0x3F}}, // 水平地址设置 (X方向分辨率)
            InitCommand{0x2B, {0x00, 0x00, 0x01, 0xDF}}, // 垂直地址设置 (Y方向分辨率)
            InitCommand{0xC1, {0x00, 0x10, 0x22}, 5}, // 时序控制2 + 5ms延迟
            InitCommand{0x29, {}, 5}, // 唤醒命令 + 5ms延迟
            InitCommand{0x2C, {}, 20} // 显存写入命令
        };
    };

    template<>
struct InitSequence<DeviceType::ST7796>
    {
        static inline constexpr auto value = std::array{
            // ST7796初始化命令与参数
            InitCommand{0x11, {}, 120}, // 退出睡眠模式
            InitCommand{0x36, {0x48}}, // 屏幕方向设置（1<<6|1<<3）
            InitCommand{0x3A, {0x55}}, // RGB565接口格式
            InitCommand{0xF0, {0xC3}}, // 命令集控制1
            InitCommand{0xF0, {0x96}}, // 命令集控制2
            InitCommand{0xB4, {0x01}}, // 显示反转控制
            InitCommand{0xB7, {0xC6}}, // 门驱动控制
            InitCommand{0xC0, {0x80, 0x45}}, // 电源控制1（AVDD/VCL升压电压）
            InitCommand{0xC1, {0x13}}, // 电源控制2（VGH/VGL电压）
            InitCommand{0xC2, {0xA7}}, // 电源控制3（VCOM偏移）
            InitCommand{0xC5, {0x0A}}, // VCOM控制（0x0A电压值）
            InitCommand{0xE8, {0x40, 0x8A, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33}}, // 面板配置
            InitCommand{0xE0, {0xD0, 0x08, 0x0F, 0x06, 0x06, 0x33, 0x30, 0x33, 0x47, 0x17, 0x13, 0x13, 0x2B, 0x31}},
            // 正伽马校正
            InitCommand{0xE1, {0xD0, 0x0A, 0x11, 0x0B, 0x09, 0x07, 0x2F, 0x33, 0x47, 0x38, 0x15, 0x16, 0x2C, 0x32}},
            // 负伽马校正
            InitCommand{0xF0, {0x3C}}, // 命令集控制3
            InitCommand{0xF0, {0x69}, 120}, // 命令集控制4 + 120ms延迟
            InitCommand{0x21, {}}, // 反色关闭
            InitCommand{0x29, {}}, // 显示开启
            InitCommand{0x36, {0xE8}, 20} // 最终显示方向（封装set_direction(0)）
        };
    };


    // =========== 方向控制模板 ===========
    template<DeviceType T>
    struct RotationControl;

    template<>
    struct RotationControl<DeviceType::ILI9488>
    {
        static void set(Rotation rot)
        {
            constexpr uint8_t params[] = {0xE9, 0x29, 0x69, 0xA9};
            InterfacePolicy<InterfaceType::Parallel8080>::write_cmd(0x36);
            InterfacePolicy<InterfaceType::Parallel8080>::write_data(params[static_cast<int>(rot)]);
        }
    };

    template<>
    struct RotationControl<DeviceType::ST7796>
    {
        static void set(Rotation rot)
        {
            constexpr uint8_t params[] = {0x48, 0x88, 0x28, 0xE8};
            InterfacePolicy<InterfaceType::SPI>::write_cmd(0x36);
            InterfacePolicy<InterfaceType::SPI>::write_data(params[static_cast<int>(rot)] | 0x08);
        }
    };
}


// ==================== 导出接口 ====================
export namespace bsp::lcd
{
    void init(); // 初始化
    void flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *color_p); // 涂块


    template<void(*delay_ms)(uint32_t), DeviceType Type, InterfaceType Interface = InterfaceType::Parallel8080,
        DMAConfigType DMAConfig = DMAConfigType::disable>
    class LCDController
    {
        using Policy = detail::InterfacePolicy<Interface>;

    public:
        static void init()
        {
            // 硬件复位
            if constexpr (Interface == InterfaceType::Parallel8080)
            {
                // 复位TFT显示屏
                TFT_LED::write(0); // 关闭背光LED
                TFT_RST::write(1); // 将TFT复位引脚设为高电平
                delay_ms(5); // 等待100毫秒
                TFT_RST::write(0); // 将TFT复位引脚设为低电平
                delay_ms(5); // 等待100毫秒
                TFT_RST::write(1); // 将TFT复位引脚设为高电平，完成复位操作
                delay_ms(5); // 等待100毫秒
            }
            else if constexpr (Interface == InterfaceType::SPI)
            {
                GPIO_InitTypeDef config = {};
                config.Mode = GPIO_MODE_OUTPUT_PP;
                config.Pull = GPIO_NOPULL;
                config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
                LCD_RST::init(config);
                LCD_RS::init(config);

                //LCD 复位
                LCD_RST::high();
                delay_ms(30);
                LCD_RST::low();
                delay_ms(100);
                LCD_RST::high();
                delay_ms(50);
            }

            // 获取初始化序列并发送
            // 修改调用方式为模板参数传递
            Policy::template send_sequence<detail::InitSequence<Type>::value>();


            if constexpr (Interface == InterfaceType::Parallel8080)
            {
                TFT_LED::write(1); // 开启背光LCD
            }

            // // 设置默认方向
            // set_rotation(Rotation::Deg0);
            // delay_ms(20);
        }

        static void set_rotation(Rotation rot)
        {
            detail::RotationControl<Type>::set(rot);
        }

        static void flush(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2, uint8_t *colors)
        {
            if constexpr (Interface == InterfaceType::SPI && DMAConfig == DMAConfigType::enable)
            {
                // 使用DMA的情况
                while (HAL_DMA_GetState(&spi::hdma_spi2_tx) != HAL_DMA_STATE_READY) {}
            }

            set_window(x1, y1, x2, y2);
            Policy::write_cmd(0x2C); // 开始传输数据到LCD
            const uint32_t pixel_count = (x2 - x1 + 1) * (y2 - y1 + 1); // 计算像素数量

            if constexpr (Interface == InterfaceType::Parallel8080)
            {
                // ====================== 8080并口接口 ======================
                if constexpr (DMAConfig == DMAConfigType::disable)
                {
                    // 不使用DMA
                    for (uint32_t i = 0; i < pixel_count; ++i)
                    {
                        TFT_DATA::write(colors[i]);
                    }
                }
                else
                {
                    // 使用DMA传输数据到LCD
                    // HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream6, (uint32_t) color_p, TFT_DATA_ADDR,
                    //      ((x2 + 1) - x1) * ((y2 + 1) - y1));
                }
            }
            else if constexpr (Interface == InterfaceType::SPI)
            {
                // ====================== SPI接口 ======================


                LCD_RS::high();
                spi::cs_low();

                if constexpr (DMAConfig == DMAConfigType::disable)
                {
                    for (uint32_t i = 0; i < pixel_count; ++i)
                    {
                        spi::send_byte(colors[i] >> 8); // 发送高字节
                        spi::send_byte(colors[i] & 0xFF); // 发送低字节
                    }
                    spi::cs_high(); // 禁用LCD片选
                }
                else
                {
                    // ====== 使用DMA =====

                    // // 交换颜色数据高低字节
                    // auto *p = reinterpret_cast<uint32_t *>(colors);
                    // uint32_t pairs = pixel_count >> 1;
                    // while (pairs--)
                    // {
                    //     *p = __REV16(*p); // 同时处理两个16位元素
                    //     p++;
                    // }
                    // // 处理剩余单个元素（如有）
                    // if (pixel_count & 1)
                    // {
                    //     auto *last = reinterpret_cast<uint16_t *>(p);
                    //     *last = __REV16(*last);
                    // }
                    auto *p = reinterpret_cast<uint16_t *>(colors);
                    for (uint32_t i = 0; i < pixel_count; ++i)
                    {
                        p[i] = __REV16(p[i]);
                    }

                    HAL_DMA_Start_IT(&spi::hdma_spi2_tx,
                                     reinterpret_cast<uint32_t>(colors), // 直接使用uint16_t*地址
                                     reinterpret_cast<uint32_t>(&spi::hspi2.Instance->DR), // SPI数据寄存器地址
                                     pixel_count << 1);

                    __HAL_DMA_ENABLE_IT(&bsp::spi::hdma_spi2_rx, DMA_IT_TC); // SPI2向DMA发出请求
                }
            }
        }

    private:
        static void set_window(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2)
        {
            if constexpr (Interface == InterfaceType::Parallel8080)
            {
                TFT_CMD::write(0x2A); // 设置列地址范围
                TFT_DATA::write(x1 >> 8);
                TFT_DATA::write(x1 & 0xFF);
                TFT_DATA::write(x2 >> 8);
                TFT_DATA::write(x2 & 0xFF);

                TFT_CMD::write(0x2B); // 设置行地址范围
                TFT_DATA::write(y1 >> 8);
                TFT_DATA::write(y1 & 0xFF);
                TFT_DATA::write(y2 >> 8);
                TFT_DATA::write(y2 & 0xFF);
            }
            else if constexpr (Interface == InterfaceType::SPI)
            {
                Policy::write_cmd(0x2A);
                Policy::write_data(x1 >> 8);
                Policy::write_data(x1 & 0xFF);
                Policy::write_data(x2 >> 8);
                Policy::write_data(x2 & 0xFF);

                Policy::write_cmd(0x2B);
                Policy::write_data(y1 >> 8);
                Policy::write_data(y1 & 0xFF);
                Policy::write_data(y2 >> 8);
                Policy::write_data(y2 & 0xFF);
            }
        }
    };
}


/*预编译*/
#define LCD_SORTS 9488
#define LCD_INTERFACE_TYPE 1 // 0:8080接口 1:SPI接口 GUI.c中的同时修改
#define delay_ms(ms)   HAL_Delay(ms)


// ==================== 函数声明 ====================
namespace bsp::lcd
{
    void set_direction(uint8_t direction);

    void LCD_Clear(uint16_t color);


    // ==================== 内联函数 ====================

#ifdef LCD_SPI_PORT_ENABLE

    void wr_reg(uint8_t data)
    {
        spi::cs_low();
        LCD_RS::low(); // 低电平发送命令
        spi::send_byte(data);
        spi::cs_high();
    }

    void wr_data(uint8_t data)
    {
        spi::cs_low();
        LCD_RS::high(); // 高电平发送数据
        spi::send_byte(data);
        spi::cs_high();
    }
#endif
}


/********************************************************************
 * 名称 : LCD_Init9481
 * 功能 : 液晶初始化
 * 输入 : 无
 * 输出 : 无
 ***********************************************************************/
void bsp::lcd::init()
{
#ifdef LCD_8080_PORT_ENABLE

#if LCD_SORTS == 9481
    TFT_RST = 0;
    HAL_Delay(5);
    TFT_RST = 1;

    TFT_CMD::write(0x0011);
    HAL_Delay(5);
    TFT_CMD::write(0x00D0);
    TFT_DATA::write(0x0007);
    TFT_DATA::write(0x0041);
    TFT_DATA::write(0x0018);

    TFT_CMD::write(0x00D1);

    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x000a);
    TFT_DATA::write(0x0010);

    TFT_CMD::write(0x00D2);
    TFT_DATA::write(0x0001);
    TFT_DATA::write(0x0011);

    TFT_CMD::write(0x00C0);
    TFT_DATA::write(0x0010);
    TFT_DATA::write(0x003B);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0002);
    TFT_DATA::write(0x0011);

    TFT_CMD::write(0x00C1);
    TFT_DATA::write(0x0010);
    TFT_DATA::write(0x0013);
    TFT_DATA::write(0x0088);

    TFT_CMD::write(0x00C5);
    TFT_DATA::write(0x0002);

    TFT_CMD::write(0x00C8);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0037);
    TFT_DATA::write(0x0025);
    TFT_DATA::write(0x0006);
    TFT_DATA::write(0x0004);
    TFT_DATA::write(0x001e);
    TFT_DATA::write(0x0026);
    TFT_DATA::write(0x0042);
    TFT_DATA::write(0x0077);
    TFT_DATA::write(0x0044);
    TFT_DATA::write(0x000f);
    TFT_DATA::write(0x0012);

    TFT_CMD::write(0x00F3);
    TFT_DATA::write(0x0040);
    TFT_DATA::write(0x000A);

    TFT_CMD::write(0x00F6);
    TFT_DATA::write(0x0080);

    TFT_CMD::write(0x00F7);
    TFT_DATA::write(0x0080);

    TFT_CMD::write(0x0036); //显示行列设置
    TFT_DATA::write(0x002f);//屏幕需要旋转180度 A9是倒过来的

    TFT_CMD::write(0x003a);
    TFT_DATA::write(0x0055);

    TFT_CMD::write(0x0020);
    //	 TFT_CMD::write(0x0021);

    TFT_CMD::write(0x002A);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0001);
    TFT_DATA::write(0x003F);

    TFT_CMD::write(0x002B);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0001);
    TFT_DATA::write(0x00df);

    TFT_CMD::write(0x00C1);
    TFT_DATA::write(0x0000);
    TFT_DATA::write(0x0010);
    TFT_DATA::write(0x0022);

    HAL_Delay(5);
    TFT_CMD::write(0x0029);
    HAL_Delay(5);
    TFT_CMD::write(0x002C);
    TFTLED = 0x01;
    HAL_Delay(20);
//    LCD_Clear(BLACK);
#endif

#if LCD_SORTS == 9488
    // 复位TFT显示屏
    TFT_RST = 1;  // 将TFT复位引脚设为高电平
    HAL_Delay(5);// 等待100毫秒
    TFT_RST = 0;  // 将TFT复位引脚设为低电平
    HAL_Delay(5);// 等待100毫秒
    TFT_RST = 1;  // 将TFT复位引脚设为高电平，完成复位操作
    HAL_Delay(5);// 等待100毫秒

    // 向LCD发送命令退出睡眠模式
    TFT_CMD::write(0x11);// 退出睡眠模式
    HAL_Delay(5);      // 等待100毫秒，确保命令执行完毕

    // 设置LCD电源控制
    TFT_CMD::write(0xd0); // 电源控制命令
    TFT_DATA::write(0x07);// 设置参数
    TFT_DATA::write(0x47);// 设置参数
    TFT_DATA::write(0x19);// 设置参数

    TFT_CMD::write(0xd1); // 电源控制命令
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x36);// 设置参数
    TFT_DATA::write(0x1f);// 设置参数

    TFT_CMD::write(0xd2); // 电源控制命令
    TFT_DATA::write(0x01);// 设置参数
    TFT_DATA::write(0x11);// 设置参数

    // 其他LCD驱动相关设置
    TFT_CMD::write(0xE4); // 驱动模式设置
    TFT_DATA::write(0xa0);// 设置参数

    TFT_CMD::write(0xf3); // 帧速率控制
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x2a);// 设置参数

    TFT_CMD::write(0xc0); // MV偏压控制
    TFT_DATA::write(0x10);// 设置参数
    TFT_DATA::write(0x3b);// 设置参数
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x02);// 设置参数
    TFT_DATA::write(0x11);// 设置参数

    TFT_CMD::write(0xc5); // VCOM控制
    TFT_DATA::write(0x03);// 设置参数

    TFT_CMD::write(0xc8); // Gamma设置
    TFT_DATA::write(0x00);// 设置Gamma参数
    TFT_DATA::write(0x35);// 设置Gamma参数
    TFT_DATA::write(0x23);// 设置Gamma参数
    TFT_DATA::write(0x07);// 设置Gamma参数
    TFT_DATA::write(0x00);// 设置Gamma参数
    TFT_DATA::write(0x04);// 设置Gamma参数
    TFT_DATA::write(0x45);// 设置Gamma参数
    TFT_DATA::write(0x53);// 设置Gamma参数
    TFT_DATA::write(0x77);// 设置Gamma参数
    TFT_DATA::write(0x70);// 设置Gamma参数
    TFT_DATA::write(0x00);// 设置Gamma参数
    TFT_DATA::write(0x04);// 设置Gamma参数

    /**************************设置LCD显示区域****************************/
    TFT_CMD::write(0x0036); // 显示行列设置
    TFT_DATA::write(0x00E9);// 设置参数1110 1001
    /*前两位为调整方向*/
    //默认方向为A9，左下角与右上角 10
    // 29为右下角与左上角  00
    //E9为左上角，右下角  11
    // 69   01

    TFT_CMD::write(0x003a); // RGB信号格式设置
    TFT_DATA::write(0x0055);// 设置参数

    TFT_CMD::write(0x0020);// RAM写入控制

    TFT_CMD::write(0x2a); // 水平地址设置
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x01);// 设置参数
    TFT_DATA::write(0xDF);// 设置参数

    TFT_CMD::write(0x2b); // 垂直地址设置
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x00);// 设置参数
    TFT_DATA::write(0x01);// 设置参数
    TFT_DATA::write(0x3F);// 设置参数

    HAL_Delay(5);      // 等待100毫秒
    TFT_CMD::write(0x29);// 唤醒命令
    TFT_CMD::write(0x2c);// 写入RAM命令
    HAL_Delay(5);      // 等待100毫秒
    TFTLED = 0x01;      // 背光寄存器初始化

    /*我觉得没必要清屏函数*/
    HAL_Delay(20);

//     LCD_Clear(0xFFFF);  // 清除屏幕，设置为白色
#endif

#elifdef LCD_SPI_PORT_ENABLE

    GPIO_InitTypeDef config = {};
    config.Mode = GPIO_MODE_OUTPUT_PP;
    config.Pull = GPIO_NOPULL;
    config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    LCD_RST::init(config);

    LCD_RST::high();

    LCD_RS::init(config);

    //LCD 复位
    delay_ms(30);
    LCD_RST::low();
    delay_ms(100);
    LCD_RST::high();
    delay_ms(50);

    //*************3.5 ST7796S IPS初始化**********//
    wr_reg(0x11);

    delay_ms(120); //Delay 120ms

    wr_reg(0x36); // Memory Data Access Control MY,MX~~
    wr_data(0x48); // (1<<6|1<<3)

    wr_reg(0x3A);
    wr_data(0x55); //wr_data(0x66);

    wr_reg(0xF0); // Command Set Control
    wr_data(0xC3);

    wr_reg(0xF0);
    wr_data(0x96);

    wr_reg(0xB4);
    wr_data(0x01);

    wr_reg(0xB7);
    wr_data(0xC6);

    //wr_reg(0xB9);
    //wr_data(0x02);
    //wr_data(0xE0);

    wr_reg(0xC0);
    wr_data(0x80);
    wr_data(0x45);

    wr_reg(0xC1);
    wr_data(0x13); //18  //00

    wr_reg(0xC2);
    wr_data(0xA7);

    wr_reg(0xC5);
    wr_data(0x0A);

    wr_reg(0xE8);
    wr_data(0x40);
    wr_data(0x8A);
    wr_data(0x00);
    wr_data(0x00);
    wr_data(0x29);
    wr_data(0x19);
    wr_data(0xA5);
    wr_data(0x33);

    wr_reg(0xE0);
    wr_data(0xD0);
    wr_data(0x08);
    wr_data(0x0F);
    wr_data(0x06);
    wr_data(0x06);
    wr_data(0x33);
    wr_data(0x30);
    wr_data(0x33);
    wr_data(0x47);
    wr_data(0x17);
    wr_data(0x13);
    wr_data(0x13);
    wr_data(0x2B);
    wr_data(0x31);

    wr_reg(0xE1);
    wr_data(0xD0);
    wr_data(0x0A);
    wr_data(0x11);
    wr_data(0x0B);
    wr_data(0x09);
    wr_data(0x07);
    wr_data(0x2F);
    wr_data(0x33);
    wr_data(0x47);
    wr_data(0x38);
    wr_data(0x15);
    wr_data(0x16);
    wr_data(0x2C);
    wr_data(0x32);

    wr_reg(0xF0);
    wr_data(0x3C);

    wr_reg(0xF0);
    wr_data(0x69);

    delay_ms(120);
    wr_reg(0x21);
    wr_reg(0x29);
    set_direction(0); //下两句被封装为此句，默认显示方向为横屏 切记：切换屏幕方向，LCD_Clear函数内要交互x,y
    //    wr_reg(0x36);
    //    wr_data(0x60);

#ifdef DMA_SPI_ENABLE

#endif

#else
#endif
}


namespace bsp::lcd
{
    /**********************************绘制接口*********************************************/
    /**
     * 设置LCD显示窗口
     * 该函数用于配置LCD的显示区域，通过指定窗口的起始和结束坐标
     *
     * @param sx 窗口起始点的X坐标
     * @param sy 窗口起始点的Y坐标
     * @param ex 窗口结束点的X坐标
     * @param ey 窗口结束点的Y坐标
     */
    void set_window(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey)
    {
#ifdef LCD_8080_PORT_ENABLE
    // 设置列地址范围
    TFT_CMD::write(0x002A);
    TFT_DATA::write(sx >> 8);
    TFT_DATA::write(0x00FF & sx);
    TFT_DATA::write((ex) >> 8);
    TFT_DATA::write(0x00FF & (ex));

    // 设置行地址范围
    TFT_CMD::write(0x002B);
    TFT_DATA::write(sy >> 8);
    TFT_DATA::write(0x00FF & sy);
    TFT_DATA::write(ey >> 8);
    TFT_DATA::write(0x00FF & ey);

    // 开始传输数据到LCD
    TFT_CMD::write(0x002C);
#elifdef LCD_SPI_PORT_ENABLE
        wr_reg(0x2A);
        wr_data(sx >> 8);
        wr_data(0x00FF & sx);
        wr_data(ex >> 8);
        wr_data(0x00FF & ex);

        wr_reg(0x2B);
        wr_data(sy >> 8);
        wr_data(0x00FF & sy);
        wr_data(ey >> 8);
        wr_data(0x00FF & ey);

        wr_reg(0x2C); //开始写入GRAM
#endif
    }

    void flush(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t *color_p)
    {
#ifdef LCD_8080_PORT_ENABLE
#ifdef DMA_FSMC_ENABLE

    set_window(x1, y1, x2, y2);//设置LCD屏幕的扫描区域
    HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream6, (uint32_t) color_p, TFT_DATA_ADDR,
                     ((x2 + 1) - x1) * ((y2 + 1) - y1));
#else
    LCD_Color_Fill(area->x1, area->y1, area->x2, area->y2, (const uint16_t *)color_p);
#endif
#elifdef LCD_SPI_PORT_ENABLE

#ifdef DMA_SPI_ENABLE
        while (HAL_DMA_GetState(&spi::hdma_spi2_tx) != HAL_DMA_STATE_READY) {}

        set_window(x1, y1, x2, y2);
        uint32_t pixel_count = (x2 - x1 + 1) * (y2 - y1 + 1);

        spi::cs_low();
        LCD_RS::high();

        // =====交换颜色数据高低字节=====
        auto *p = reinterpret_cast<uint32_t *>(color_p);
        uint32_t pairs = pixel_count >> 1;
        while (pairs--)
        {
            *p = __REV16(*p); // 同时处理两个16位元素
            p++;
        }
        // 处理剩余单个元素（如有）
        if (pixel_count & 1)
        {
            auto *last = reinterpret_cast<uint16_t *>(p);
            *last = __REV16(*last);
        }

        HAL_DMA_Start_IT(&spi::hdma_spi2_tx,
                         reinterpret_cast<uint32_t>(color_p), // 直接使用uint16_t*地址
                         reinterpret_cast<uint32_t>(&spi::hspi2.Instance->DR), // SPI数据寄存器地址
                         pixel_count << 1
        );
        __HAL_DMA_ENABLE_IT(&bsp::spi::hdma_spi2_rx, DMA_IT_TC);
#else
        set_window(x1, y1, x2, y2); // 设置LCD屏幕的扫描区域
    CS_LOW(); // 使能LCD片选
    LCD_RS::high(); // 设置为数据模式

    uint32_t pixel_count = (x2 - x1 + 1) * (y2 - y1 + 1); // 计算像素数量
    for (uint32_t i = 0; i < pixel_count; ++i)
    {
        bsp::spi::send_byte(color_p[i] >> 8); // 发送高字节
        bsp::spi::send_byte(color_p[i] & 0xFF); // 发送低字节
    }

    CS_HIGH(); // 禁用LCD片选

#endif
#endif
    }


    /**
     * 设置LCD显示方向
     * 该函数用于设置LCD的显示方向，通过指定显示方向参数来确定LCD的显示方式
     * @param direction 显示方向参数，取值范围为0~3，分别表示0度、90度、180度、270度
     */
    void set_direction(const uint8_t direction)
    {
#ifdef LCD_SPI_PORT_ENABLE
        // 初始为竖屏
        // 定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转
        wr_reg(0x36);
        //注意：原始方向为竖屏
        //    位	名称	描述
        //    7	    MY	垂直方向镜像（1：镜像，0：正常）
        //    6	    MX	水平方向镜像（1：镜像，0：正常）
        //    5	    MV	行列交换（1：交换，0：不交换）
        //    4   	ML	垂直刷新顺序（1：反向，0：正常）
        //    3 	BGR	RGB/BGR 顺序（1：BGR，0：RGB）
        //    2 	MH	水平刷新顺序（1：反向，0：正常）
        //    1 	-	保留位
        //    0 	-	保留位
        switch (direction)
        {
            case 0:
                wr_data((1 << 5) | (1 << 6) | (1 << 7) | (1 << 3)); // 横屏1; 0度
                break;
            case 1:
                wr_data((1 << 6) | (1 << 3)); // 竖屏1; 90度
                break;
            case 2:
                wr_data((1 << 5) | (1 << 3)); // 横屏2; 180度
                break;
            case 3:
                wr_data((1 << 7) | (1 << 3)); // 竖屏2; 270度
                break;
            default:
                break;
        }
#endif
    }

    //切记：切换屏幕方向，LCD_Clear函数内要交互x,y
    void LCD_Clear(uint16_t color)
    {
#ifdef LCD_8080_PORT_ENABLE
    set_window(0, 0, 479, 319);
    for (uint32_t i = 0; i < 0x25800; ++i)
    {
        TFT_DATA::write(color);
    }
#elifdef LCD_SPI_PORT_ENABLE
        // 默认是用阻塞式刷屏，不使用DMA刷屏
        set_window(0, 0, 479, 319); //横屏
        //    set_window(0, 0, 319, 479);//竖屏
        spi::cs_low();
        LCD_RS::high();
        for (uint32_t i = 0; i < 480 * 320; ++i)
        {
            spi::send_byte(color >> 8);
            spi::send_byte(color & 0xFF);
        }
        spi::cs_high();
#endif
    }


    /// 设置一个像素
    void set_pixel(uint16_t x, uint16_t y, uint16_t color)
    {
#ifdef LCD_8080_PORT_ENABLE
    set_window(x, y, x, y);
    write_data(color);
#elifdef LCD_SPI_PORT_ENABLE
        set_window(x, y, x, y);
        spi::cs_low();
        LCD_RS::high();
        spi::send_byte(color >> 8);
        spi::send_byte(color & 0xFF);
        spi::cs_high();
#endif
    }
}
