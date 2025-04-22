module;
#include <project_config.h>

export module blue_tooth;

import usart;

export namespace bsp::bluetooth {
    // 类型定义 ----------------------------------------------------
    enum class Error : uint8_t {
        OK,
        BUSY,
        CHECKSUM_ERROR,
        PACKET_SIZE_ERROR,
        DMA_ERROR
    };

    // 协议配置常量
    constexpr uint8_t PACKET_HEADER = 0xA5;   // 包头标识
    constexpr uint8_t PROTOCOL_VER = 0x01;    // 协议版本
    constexpr uint8_t WAVE_SAMPLES = 24;      // 波形点数
    constexpr uint8_t HARMONICS_NUM = 10;     // 谐波数量
    constexpr uint16_t PACKET_SIZE = 155;     // 总包大小

    // 传感器数据结构（严格内存对齐）
    #pragma pack(push, 1)
    typedef struct {
        uint8_t header;                // 包头标识
        uint8_t version;               // 协议版本
        float thd;                     // 总谐波失真
        float freq;                    // 基波频率
        float rate;                    // 采样率
        float gain;                    // 增益系数
        float wave[WAVE_SAMPLES];      // 波形数据
        float harmonics[HARMONICS_NUM];// 谐波分量
        uint8_t checksum;              // 异或校验和
    } SensorData;
    #pragma pack(pop)

    // 静态断言确保内存布局
    static_assert(sizeof(SensorData) == PACKET_SIZE,
        "SensorData结构体大小必须严格为155字节");

    // 接口函数 --------------------------------------------------
    Error initialize(uint32_t baud = 9600) noexcept;
    Error send_data(float thd, float freq, float rate, float gain,
                   const int32_t* raw_wave, const float* harmonics) noexcept;
    bool is_busy() noexcept;
}

// ================== 模块实现 ==================
namespace {
    // 硬件控制层
    typedef struct {
        UART_HandleTypeDef* uart;
        DMA_HandleTypeDef* dma_tx;
        volatile uint8_t tx_status;  // 传输状态标志
    } UART_Controller;

    UART_Controller bt_controller = {
        .uart = &bsp::usart::huart1,
        .dma_tx = &bsp::usart::hdma_usart1_tx,
        .tx_status = 0
    };

    // 校验和计算
    uint8_t calculate_checksum(const bsp::bluetooth::SensorData* data) {
        uint8_t sum = 0;
        const auto* p = reinterpret_cast<const uint8_t *>(data);
        for(uint16_t i=0; i<sizeof(bsp::bluetooth::SensorData)-1; i++) {
            sum ^= p[i];
        }
        return sum;
    }
}

// 初始化蓝牙模块
bsp::bluetooth::Error bsp::bluetooth::initialize(uint32_t baud) noexcept {
    // 配置UART参数
    bt_controller.uart->Init.BaudRate = baud;
    if(HAL_UART_Init(bt_controller.uart) != HAL_OK) {
        return Error::BUSY;
    }

    // 绑定DMA通道
    __HAL_LINKDMA(bt_controller.uart, hdmatx, *bt_controller.dma_tx);
    return Error::OK;
}

// 发送传感器数据
bsp::bluetooth::Error bsp::bluetooth::send_data(
    float thd, float freq, float rate, float gain,
    const int32_t* raw_wave, const float* harmonics) noexcept
{
    static SensorData packet;  // 静态存储避免堆栈过大

    // 检查DMA状态
    if(bt_controller.tx_status) {
        return Error::BUSY;
    }

    // 填充包头
    packet.header = PACKET_HEADER;
    packet.version = PROTOCOL_VER;

    // 填充测量参数
    packet.thd = thd;
    packet.freq = freq;
    packet.rate = rate;
    packet.gain = gain;

    // 转换波形数据
    for(uint8_t i=0; i<WAVE_SAMPLES; i++) {
        packet.wave[i] = static_cast<float>(raw_wave[i]);
    }

    // 填充谐波数据（跳过直流分量）
    for(uint8_t i=0; i<HARMONICS_NUM; i++) {
        packet.harmonics[i] = harmonics[i+1];
    }

    // 计算校验和
    packet.checksum = calculate_checksum(&packet);

    // 启动DMA传输
    const HAL_StatusTypeDef status = HAL_UART_Transmit_DMA(
        bt_controller.uart,
        reinterpret_cast<uint8_t *>(&packet),
        PACKET_SIZE
    );

    if(status != HAL_OK) {
        return Error::DMA_ERROR;
    }

    bt_controller.tx_status = 1;
    return Error::OK;
}

// 查询传输状态
bool bsp::bluetooth::is_busy() noexcept {
    return bt_controller.tx_status;
}

// ================== 中断处理 ==================
extern "C" void HAL_UART_TxCpltCallback(const UART_HandleTypeDef *huart) {
    if(huart == bt_controller.uart) {
        bt_controller.tx_status = 0;
    }
}

