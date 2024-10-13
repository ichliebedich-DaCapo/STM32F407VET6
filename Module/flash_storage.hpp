//
// Created by 34753 on 2024/10/11.
//

#ifndef FURINA_FLASH_STORAGE_HPP
#define FURINA_FLASH_STORAGE_HPP

#include "JYZQ_Conf.h"

#ifdef __cplusplus
extern "C" {
#endif
#if USE_FLASH_STORAGE

/**
 * @brief Flash存储类
 * @details
 */
class FlashStorage
{
public:

    static auto read_isr_ready() -> void;

    static auto write_isr_ready() -> void;

    static auto read_isr() -> uint8_t;

    static auto write_isr(uint8_t byte) -> void;

    static auto background_processing() -> void;

    /*还需要提供擦除所有扇区的接口、提供Flash记录地址的接口、修改Flash记录地址的接口*/


private:
    // 切换缓冲区并重置索引
    static auto switch_buffer() -> void
    {
        reset_index();
        uint8_t *temp = pBackBuffer;
        pBackBuffer = pForeBuffer;
        pForeBuffer = temp;
    }

    static auto set_read_sign() -> void { status |= static_cast<uint8_t>(Flags::READ_WRITE_FLAG); }// 设置读标志

    static auto set_write_sign() -> void { status &= ~static_cast<uint8_t>(Flags::READ_WRITE_FLAG); }// 设置写标志

    static auto set_read_full_sign() -> void { status |= static_cast<uint8_t>(Flags::READ_FULL_FLAG); }// 设置读满标志

    static auto set_write_full_sign() -> void { status |= static_cast<uint8_t>(Flags::WRITE_FULL_FLAG); }// 设置写满标志

    static auto set_read_incomplete_sign() -> void { status |= static_cast<uint8_t>(Flags::READ_INCOMPLETE_FLAG); }// 设置读残缺标志

    static auto set_write_incomplete_sign() -> void { status |= static_cast<uint8_t>(Flags::WRITE_INCOMPLETE_FLAG); }// 设置写残缺标志

    static auto check_read_sign() -> uint8_t { return status & static_cast<uint8_t>(Flags::READ_WRITE_FLAG); }// 检测读标志

    static auto check_write_sign() -> uint8_t
    {
        return !(status & static_cast<uint8_t>(Flags::READ_WRITE_FLAG));
    }// 检测写标志

    static auto check_read_full_sign() -> uint8_t
    {
        return status & static_cast<uint8_t>(Flags::READ_FULL_FLAG);
    }// 检测读满标志

    static auto check_write_full_sign() -> uint8_t
    {
        return status & static_cast<uint8_t>(Flags::WRITE_FULL_FLAG);
    }// 检测写满标志

    static auto check_read_incomplete_sign() -> uint8_t
    {
        return status & static_cast<uint8_t>(Flags::READ_INCOMPLETE_FLAG);
    }// 检测读残缺标志

    static auto check_write_incomplete_sign() -> uint8_t
    {
        return status & static_cast<uint8_t>(Flags::WRITE_INCOMPLETE_FLAG);
    }// 检测写残缺标志

    static auto clear_read_full_sign() -> void { status &= ~static_cast<uint8_t>(Flags::READ_FULL_FLAG); }// 清除读满标志

    static auto clear_write_full_sign() -> void { status &= ~static_cast<uint8_t>(Flags::WRITE_FULL_FLAG); }// 清除写满标志

    static auto clear_read_incomplete_sign() -> void { status &= ~static_cast<uint8_t>(Flags::READ_INCOMPLETE_FLAG); }// 清除读残缺标志

    static auto clear_write_incomplete_sign() -> void { status &= ~static_cast<uint8_t>(Flags::WRITE_INCOMPLETE_FLAG); }// 清除写残缺标志

    static auto reset_index() -> void { index = 0; }// 重置缓冲区索引

private:
    static constexpr const uint16_t page_size = 256;// 页大小,不占内存
    static inline uint8_t buffer1[page_size] = {};
    static inline uint8_t buffer2[page_size] = {};
    static inline uint8_t *pBackBuffer = buffer1;// 后台缓冲区指针
    static inline uint8_t *pForeBuffer = buffer2;// 前台缓冲区指针
    // FLASH存储地址 鉴于加载缓冲区都依靠该地址，所以该地址表示的是“将要”加载到缓冲区对应的Flash的起始地址
    static inline uint32_t addr = 0;
    static inline uint32_t addr_remaining = 0;// 剩余地址，用于存储初始地址残缺的量
    /**
     * @details 0'b:读写标志，默认0为写
     * 1'b:读满标志，默认0为未满
     * 2'b:写满标志，默认0为未满
     * 3'b:表示读残缺，默认0为无残缺    所谓残缺就是指输入的地址不是Flash的页字节的整数倍
     * 4'b:表示写残缺，默认0为无残缺
     */
    static inline uint8_t status = 0; // 状态，用于在单线程下处理各种“标志”
    static inline uint16_t index = 0; // 记录缓冲数组的索引，可根据需要改变数据类型,现在留一定余量

    // 使用 enum class 定义常量,由于它是强类型，必须使用类型转换static_cast并不会占用内存，与constexpr相一样
    enum class Flags : uint8_t
    {
        READ_WRITE_FLAG = 1U,// 读写标志，默认0为写标志,这样读取预加载时会少一个分支
        READ_FULL_FLAG = 1U << 1, // 读满标志
        WRITE_FULL_FLAG = 1U << 2,// 写满标志
        READ_INCOMPLETE_FLAG = 1U << 3,// 读残缺标志
        WRITE_INCOMPLETE_FLAG = 1U << 4
    };

    // 起始地址预留1页用于存储一些数据信息，比如上次写入地址等等
    enum class Addresses : uint32_t
    {
        FLASH_START_ADDR = 0x100, // 256B
        FLASH_END_ADDR = 0x200000 // 2MB
    };
};

#endif
#ifdef __cplusplus
}
#endif
#endif //FURINA_FLASH_STORAGE_HPP
