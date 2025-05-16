//
// Created by fairy on 2025/5/5 13:57.
//
module;
#include <array>
#include <span>
#include <project_config.h>
export module ads1115;
export import i2c;

export namespace bsp::adc::ads1115
{
    // 输入多路配置 bits[12~14] 输入多路复用器配置 前四个为差分输入，后四个为单端输入
    enum class MuxConfig : uint8_t
    {
        Diff_0_1 = 0, // 差分 0-1
        Diff_0_3 = 1, // 差分 0-3
        Diff_1_3 = 2, // 差分 1-3
        Diff_2_3 = 3, // 差分 2-3
        Single_0 = 4, // 单端 0
        Single_1 = 5, // 单端 1
        Single_2 = 6, // 单端 2
        Single_3 = 7 // 单端 3
    };
}

namespace bsp::adc::ads1115
{
    // 设备地址枚举
    enum class Address : uint8_t
    {
        // 设备地址
        Write = 0x90,
        Read = 0x91,
        // 内部寄存器地址
        Conversion = 0x00,
        Config = 0x01,
        Low_thresh = 0x02,
        High_thresh = 0x03
    };





    // 可编程增益放大器配置 bits[9~11]
    enum class PGAConfig : uint8_t
    {
        FS_6_144V = 0, // ±6.144V
        FS_4_096V = 1, // ±4.096V
        FS_2_048V = 2, // ±2.048V
        FS_1_024V = 3, // ±1.024V
        FS_0_512V = 4, // ±0.512V
        FS_0_256V = 5 // ±0.256V
    };

    // 数据速率配置 bits[5~7]
    enum class DataRate : uint8_t
    {
        SPS_8 = 0,
        SPS_16 = 1,
        SPS_32 = 2,
        SPS_64 = 3,
        SPS_128 = 4,
        SPS_250 = 5,
        SPS_475 = 6,
        SPS_860 = 7
    };

    // 比较器配置 bits[4]
    enum class ComparatorMode : uint8_t
    {
        Traditional = 0,
        Window = 1
    };

    // 比较器极性配置 bits[3]
    enum class ComparatorPolarity : uint8_t
    {
        ActiveLow = 0,
        ActiveHigh = 1
    };

    // 队列配置 bits[2]
    enum class ComparatorLatching : uint8_t
    {
        NonLatching = 0,
        Latching = 1
    };

    // 队列配置 bits[0~1]
    enum class ComparatorQueue : uint8_t
    {
        AssertAfter1 = 0,
        AssertAfter2 = 1,
        AssertAfter4 = 2,
        Disable = 3
    };
}


export namespace bsp::adc
{
    class ADS1115
    {
    public:
        // 已经使用I2C2
        static void init()
        {
            i2c2::init();

            // 初始化配置
            cmd[0] = static_cast<uint8_t>(ads1115::Address::Config);
            // [11:9]:010 	调节PGA增益  FSR = ±2.048 V (default)
            cmd[1]=0x85;
            // [11:9]:011   调节PGA增益  FSR = ±1.024 V
            //cmd[1]=0x87;
            cmd[2]=0x83;
            i2c2::write(static_cast<uint16_t>(ads1115::Address::Write), cmd);
        }

        //
        /**
         * 读取某个引脚ADC的值
         * @param channel 读取的某个引脚
         * @details VQ是AIN1,VI是AIN2
         * @return
         */
        static int16_t read(ads1115::MuxConfig channel)
        {
//            uint8_t cmd[3]={static_cast<uint8_t>(ads1115::Address::Config)};
//            // 读取原配置
//            i2c2::write(static_cast<uint16_t>(ads1115::Address::Write), {&cmd[0],1});
//            i2c2::read(static_cast<uint16_t>(ads1115::Address::Read), {&cmd[1],&cmd[2]});

            // ------ 修改配置 切换输入通道 ------
            cmd[0] = static_cast<uint8_t>(ads1115::Address::Config);
            // 高8位字节和低八位字节顺序颠倒 bits[12:14] → 高位字节bits[4:6];
            cmd[1] |= (1<<7);// OS置1，开始转换
            cmd[1] &=~(0x07<<4);// 清零
            cmd[1] |= ((static_cast<uint8_t>(channel)&0x07) << 4);
            i2c2::write(static_cast<uint16_t>(ads1115::Address::Write), cmd);

            // ------ 等待转换完成 ------
            std::array<uint8_t, 2> data{};
            cmd[0] = static_cast<uint8_t>(ads1115::Address::Config);
            do
            {
                i2c2::write(static_cast<uint16_t>(ads1115::Address::Write), {&cmd[0], 1});
                i2c2::read(static_cast<uint16_t>(ads1115::Address::Read), data);
            } while (!(data[0]>>7));

            // ------ 读取数据 ------
            cmd[0] = static_cast<uint8_t>(ads1115::Address::Conversion);
            i2c2::write(static_cast<uint16_t>(ads1115::Address::Write), {&cmd[0],1});
            i2c2::read(static_cast<uint16_t>(ads1115::Address::Read), data);

            // 解析数据
            const auto value =(int16_t)( (data[0] << 8) | data[1]);
            return value;
        }

    private:
        static inline uint8_t cmd[3];
    };
}
