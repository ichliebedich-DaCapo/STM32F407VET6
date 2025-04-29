//
// Created by DV on 2025/2/4.
//
module;
#include <concepts>
#include <project_config.h>
#include <span>

#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_i2c.h"
#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_gpio.h"
export module i2c;
import gpio;

export namespace bsp::i2c
{
    template<uint32_t I2C_Base>
    class I2C
    {
        static inline I2C_HandleTypeDef hi2c = {};
        static inline DMA_HandleTypeDef hdma_i2c_tx = {};
        static inline DMA_HandleTypeDef hdma_i2c_rx = {};

        // 传输完成回调签名
        using Callback = void(I2C::*)();
        static inline I2C *instance = nullptr;
        static inline Callback txCallback = nullptr;
        static inline Callback rxCallback = nullptr;

    public:
        I2C() { instance = this; }

        static void init()
        {
            GPIO_InitTypeDef config = {};
            config.Mode = GPIO_MODE_AF_OD;
            config.Pull = GPIO_NOPULL;
            config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

            if constexpr (I2C_Base == I2C1_BASE)
            {
                __HAL_RCC_I2C1_CLK_ENABLE();
                /**I2C1 GPIO Configuration
                PB8     ------> I2C1_SCL
                PB9     ------> I2C1_SDA
                */
                config.Alternate = GPIO_AF4_I2C1;
                gpio<GPIOB_BASE,GPIO_PIN_8 | GPIO_PIN_9>::init(config);

                // 绑定DMA通道
                hdma_i2c_tx.Instance = DMA1_Stream6;
                hdma_i2c_tx.Init.Channel = DMA_CHANNEL_1;
                hdma_i2c_rx.Instance = DMA1_Stream0;
                hdma_i2c_rx.Init.Channel = DMA_CHANNEL_1;
            }
            else
            {
                __HAL_RCC_I2C2_CLK_ENABLE();
                /**I2C2 GPIO Configuration
                PB10     ------> I2C2_SCL
                PB11     ------> I2C2_SDA
                */
                config.Alternate = GPIO_AF4_I2C2;
                gpio<GPIOB_BASE,GPIO_PIN_10 | GPIO_PIN_11>::init(config);

                // 绑定DMA通道
                hdma_i2c_tx.Instance = DMA1_Stream7;
                hdma_i2c_tx.Init.Channel = DMA_CHANNEL_7;
                hdma_i2c_rx.Instance = DMA1_Stream2;
                hdma_i2c_rx.Init.Channel = DMA_CHANNEL_7;
            }

            // /* I2C1 DMA 初始化 */
            // /* I2C1_TX 初始化 */
            // hdma_i2c_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
            // hdma_i2c_tx.Init.PeriphInc = DMA_PINC_DISABLE;
            // hdma_i2c_tx.Init.MemInc = DMA_MINC_ENABLE;
            // hdma_i2c_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            // hdma_i2c_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            // hdma_i2c_tx.Init.Mode = DMA_NORMAL;
            // hdma_i2c_tx.Init.Priority = DMA_PRIORITY_MEDIUM;
            // hdma_i2c_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
            // if (HAL_DMA_Init(&hdma_i2c_tx) != HAL_OK)
            // {
            //     Error_Handler();
            // }
            // __HAL_LINKDMA(&hi2c, hdmatx, hdma_i2c_tx);
            //
            // /* I2C1_RX 初始化 */
            // hdma_i2c_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
            // hdma_i2c_rx.Init.PeriphInc = DMA_PINC_DISABLE;
            // hdma_i2c_rx.Init.MemInc = DMA_MINC_ENABLE;
            // hdma_i2c_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
            // hdma_i2c_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
            // hdma_i2c_rx.Init.Mode = DMA_NORMAL;
            // hdma_i2c_rx.Init.Priority = DMA_PRIORITY_MEDIUM;
            // hdma_i2c_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
            // if (HAL_DMA_Init(&hdma_i2c_rx) != HAL_OK)
            // {
            //     Error_Handler();
            // }
            // __HAL_LINKDMA(&hi2c, hdmarx, hdma_i2c_rx);

            hi2c.Instance = reinterpret_cast<I2C_TypeDef *>(I2C_Base);
            hi2c.Init.ClockSpeed = 100000;
            hi2c.Init.DutyCycle = I2C_DUTYCYCLE_2;
            hi2c.Init.OwnAddress1 = 0;
            hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
            hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
            hi2c.Init.OwnAddress2 = 0;
            hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
            hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
            if (HAL_I2C_Init(&hi2c) != HAL_OK)
            {
                Error_Handler();
            }
        }


        // 阻塞式传输
        template<typename T>
            requires std::same_as<T, uint8_t>
        static HAL_StatusTypeDef write(const uint16_t devAddr, std::span<const T> data)
        {
            return HAL_I2C_Master_Transmit(&hi2c, devAddr,
                                           const_cast<uint8_t *>(data.data()),
                                           data.size(), HAL_MAX_DELAY);
        }

        // 编译期切片快捷方法（C++20）
        template<size_t Offset, size_t Count = std::dynamic_extent>
        static auto slice(auto &&container)
        {
            return std::span{container}.template subspan<Offset, Count>();
        }

        /**
         * @brief 从I2C设备读取数据（支持缓冲区切片操作）
         * @param devAddr 7位I2C设备地址（无需手动左移，函数内部处理地址转换）
         * @param buffer 数据接收缓冲区，支持任意连续容器及其子范围
         * @retval HAL状态值（HAL_OK表示成功）
         *
         * @note 此函数支持零拷贝切片操作，适用于以下场景：
         * @code
         * // 示例1：读取到std::array
         * std::array<uint8_t, 8> arrBuf;
         * i2c1::read(0x50, arrBuf); // 读取8字节
         *
         * // 示例2：读取到std::vector
         * std::vector<uint8_t> vecBuf(32); // 预分配32字节
         * i2c1::read(0x68, vecBuf);        // 读取全部32字节
         *
         * // 示例3：读取到C风格数组
         * uint8_t cBuf[64];
         * i2c1::read(0x48, {cBuf, sizeof(cBuf)}); // 显式构造span
         *
         * // 示例4：切片操作（前2字节）
         * std::array<uint8_t, 8> buf;
         * i2c1::read(0x50, buf.first(2)); // 仅读取前2字节
         *
         * // 示例5：切片操作（中间3字节）
         * i2c1::read(0x50, buf.subspan(2, 3)); // 读取索引2-4
         *
         * // 示例6：动态切片（运行时确定范围）
         * size_t offset = 3;
         * size_t count = 4;
         * i2c1::read(0x50, buf.subspan(offset, count));
         *
         * // 示例7：链式切片
         * i2c1::read(0x50, buf.last(5).first(2)); // 取最后5字节的前2字节
         * @endcode
         */
        static HAL_StatusTypeDef read(const uint16_t devAddr, std::span<uint8_t> buffer)
        {
            return HAL_I2C_Master_Receive(&hi2c, devAddr,
                                          buffer.data(), buffer.size(),
                                          HAL_MAX_DELAY);
        }

        // 带寄存器的存储器操作
        // enum class MemAddrSize { _8BIT = I2C_MEMADD_SIZE_8BIT, _16BIT = I2C_MEMADD_SIZE_16BIT };


        // 与read函数用法差不多
        static HAL_StatusTypeDef  readMemory(const uint16_t devAddr, const uint16_t memAddr, std::span<uint8_t> buffer)
        {
            return HAL_I2C_Mem_Read(&hi2c, devAddr, memAddr, I2C_MEMADD_SIZE_8BIT,
                                    buffer.data(), buffer.size(),HAL_MAX_DELAY);
        }

        // 零拷贝传输
        template<typename T>
            requires std::is_trivially_copyable_v<T>
        HAL_StatusTypeDef sendStruct(uint16_t devAddr, const T &data)
        {
            return write(devAddr, std::span{reinterpret_cast<const uint8_t *>(&data), sizeof(T)});
        }


        template<typename T>
            requires std::same_as<T, uint8_t>
        static HAL_StatusTypeDef writeMemory(const uint16_t devAddr7bit, const uint16_t memAddr, std::span<T> buffer)
        {
            return HAL_I2C_Mem_Write(&hi2c, devAddr7bit << 1, memAddr,I2C_MEMADD_SIZE_8BIT,
                                     buffer.data(), buffer.size(),HAL_MAX_DELAY);
        }


        // DMA异步操作
        template<typename T>
            requires std::same_as<T, uint8_t>
        static HAL_StatusTypeDef writeAsync(const uint16_t devAddr7bit, std::span<const T> data)
        {
            return HAL_I2C_Master_Transmit_DMA(&hi2c, devAddr7bit << 1,
                                               const_cast<uint8_t *>(data.data()),
                                               data.size());
        }

        // 状态检查
        static bool isReady()
        {
            return hi2c.State == HAL_I2C_STATE_READY;
        }

        // 回调处理器（需在HAL中断中路由）
        static void handleTxComplete()
        {
            if (instance && I2C<I2C_Base>::txCallback)
            {
                (instance->*I2C<I2C_Base>::txCallback)(); // 正确调用成员函数指针
            }
        }

        // 回调注册
        static void setTxCallback(Callback cb) { txCallback = cb; }

        static void setRxCallback(Callback cb) { rxCallback = cb; }
    };
}


export namespace bsp
{
    // 实例化I2C
    using i2c1 = i2c::I2C<I2C1_BASE>;
    using i2c2 = i2c::I2C<I2C2_BASE>;
}
