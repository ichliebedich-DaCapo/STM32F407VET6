//
// Created by DV on 2025/2/5.
//
module;
#include <array>
#include <span>
#include <project_config.h>

export module touch;
import i2c;
import delay;
import gpio;



export namespace bsp::touch
{
    uint8_t init();

    int32_t read_single_point(int32_t *last_x, int32_t *last_y);
}


namespace
{
    // ========================== 设备状态码 ==========================
    constexpr int FT6336_OK = 1; ///< 操作成功标志
    constexpr int FT6336_ERROR = 0; ///< 操作失败标志

    // ======================== 设备逻辑状态定义 ========================
    constexpr int FT_FALSE = 0; ///< 逻辑假值（基础状态）
    constexpr int FT_TRUE = 1; ///< 逻辑真值（基础状态）
    constexpr int FT_FALSE1 = 2; ///< 扩展假值1（特殊错误状态）
    constexpr int FT_FALSE2 = 3; ///< 扩展假值2（特殊错误状态）

    // ======================== 设备物理层参数 =========================
    constexpr uint8_t FT6336_ADDR = 0x71; ///< I2C设备地址（7位地址格式）

    // ======================== 寄存器地址定义 =========================
    // --------------------- 基础功能寄存器 --------------------------
    constexpr uint8_t FT_DEVIDE_MODE = 0x00; ///< 模式控制寄存器（默认值：0x00）
    constexpr uint8_t FT_REG_NUM_FINGER = 0x02; ///< 触摸点数量寄存器（0-2个触点）

    // --------------------- 触摸数据寄存器 ----------------------------
    constexpr uint8_t FT_TP1_REG = 0x03; ///< 第1触摸点数据首地址（包含X/Y坐标等数据）
    constexpr uint8_t FT_TP2_REG = 0x09; ///< 第2触摸点数据首地址（包含X/Y坐标等数据）

    // ===================== 芯片识别与控制寄存器 ======================
    constexpr uint8_t FT_ID_G_CIPHER_MID = 0x9F; ///< 芯片代号中字节（默认值：0x26）
    constexpr uint8_t FT_ID_G_CIPHER_LOW = 0xA0; ///< 芯片代号低字节（0x01:FT6336G，0x02:FT6336U）
    constexpr uint8_t FT_ID_G_LIB_VERSION = 0xA1; ///< 固件版本寄存器（只读）
    constexpr uint8_t FT_ID_G_CIPHER_HIGH = 0xA3; ///< 芯片代号高字节（默认值：0x64）
    constexpr uint8_t FT_ID_G_MODE = 0xA4; ///< 中断模式控制寄存器（配置中断触发方式）

    // ====================== 设备配置寄存器 ==========================
    constexpr uint8_t FT_ID_G_FOCALTECH_ID = 0xA8; ///< 厂商ID寄存器（默认值：0x11，只读）
    constexpr uint8_t FT_ID_G_THGROUP = 0x80; ///< 触摸灵敏度寄存器（值越大越灵敏，范围0-80）
    constexpr uint8_t FT_ID_G_PERIODACTIVE = 0x88; ///< 工作周期寄存器（配置扫描频率，单位10ms）

    // ========================= 面板参数 ============================
    constexpr uint8_t PANNEL_ID = 0x11; ///< 面板标识码（用于多面板系统识别，默认0x11）

    /**GPIO引脚定义
    PB5     ------> CTP_RST
    PB6     ------> CTP_INT
    */
    using TOUCH_RST = bsp::gpio<GPIOB_BASE,GPIO_PIN_5>;// 触摸屏复位引脚

}


//触摸初始化函数（驱动芯片FT6336U）（返回1成功返回0失败）
uint8_t bsp::touch::init()
{
    // 初始化i2c1
    i2c1::init();

    //配置GPIO
    GPIO_InitTypeDef config = {};
    config.Mode = GPIO_MODE_OUTPUT_PP;
    config.Pull = GPIO_PULLUP;
    config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
   TOUCH_RST::init(config);

    // 重置
    TOUCH_RST::low();
    delay::us(10);
    TOUCH_RST::high();
    delay::ms(50);

    std::array<uint8_t,1> id_reg{100};

    if (const HAL_StatusTypeDef status =i2c1::readMemory(FT6336_ADDR,FT_ID_G_FOCALTECH_ID,id_reg); !status)
    {
        return FT_FALSE1; //I2C通信故障
    }
    if (id_reg[0] != PANNEL_ID)
    {
        return FT_FALSE2; //寄存器值不匹配
    }
    return id_reg[0];
}


int32_t bsp::touch::read_single_point(int32_t *last_x, int32_t *last_y)
{
    std::array<uint8_t,1> point_number{};
    std::array<uint8_t,4> touch_pos{};

    // 读取触摸点的数量
    i2c1::readMemory(FT6336_ADDR,FT_REG_NUM_FINGER,point_number);//读点数

    // 如果没有触摸点，返回0
    if (point_number[0]==0)
    {
        return 0;
    }

    // 读取第一个触摸点的坐标
    i2c1::readMemory(FT6336_ADDR,FT_TP1_REG,touch_pos);//读坐标

    // 解析触摸点的坐标 横屏
    *last_x = 480 - ((static_cast<uint16_t>(touch_pos[2] & 0x0F) << 8) + touch_pos[3]);
    *last_y = (static_cast<uint16_t>(touch_pos[0] & 0x0F) << 8) + touch_pos[1];

    // 返回1表示有触摸点
    return 1;
}
