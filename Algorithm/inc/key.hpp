//
// Created by fairy on 2024/9/22.
//
#pragma once
#ifndef FURINA_KEY_HPP
#define FURINA_KEY_HPP
#include <cstdint>
#include <project_config.h>

constexpr uint8_t KEY_STATE_TWO =0x2;
constexpr uint8_t KEY_STATE_THREE =0x3;
constexpr uint8_t KEY_STATE_FOUR =0x4;

constexpr uint8_t keyK0 = 0x0;
constexpr uint8_t keyK1 = 0x1;
constexpr uint8_t keyK2 = 0x2;
constexpr uint8_t keyK3 = 0x3;
constexpr uint8_t keyK4 = 0x4;
constexpr uint8_t keyK5 = 0x5;
constexpr uint8_t keyK6 = 0x6;
constexpr uint8_t keyK7 = 0x7;
constexpr uint8_t keyK8 = 0x8;
constexpr uint8_t keyK9 = 0x9;
constexpr uint8_t keyKA = 0xA;
constexpr uint8_t keyKB = 0xB;
constexpr uint8_t keyKC = 0xC;
constexpr uint8_t keyKD = 0xD;
constexpr uint8_t keyKE = 0xE;
constexpr uint8_t keyKF = 0xF;

extern void key_handler();// 按键处理函数


class Key {
public:

    // 平台适配接口（通过模板参数注入）
    template<typename Adapter>
    class Core {
    public:
        // 状态处理（保持原有位操作逻辑）
        static uint8_t handle_state(uint8_t max_states) {
            const uint8_t shift = current_code_ <<1;
            uint8_t state = (states_ >> shift) & 0x03;
            state = (state + 1) % max_states;
            states_ = (states_ & ~(0x03 << shift)) | (state << shift);
            return state;
        }

        // 中断服务例程入口,传入按键编码
        static inline void isr_entry(uint8_t code) {
            current_code_ = code;
            Adapter::notify_from_isr();
        }

        // 主循环处理
        static inline void poll() {
            if (Adapter::acquire()) {
               key_handler();
            }
        }

        static inline uint8_t& getCode(){return current_code_;}


        template<void (*key_init)(void)>
        static inline void init()
        {
            key_init();
            Adapter::init();
        }

    private:
        static inline uint32_t states_ = 0;
        static inline uint8_t current_code_ = 0;
    };
};


// 平台适配层空实现（供用户特化）
template<typename T>
struct KeyAdapter {
    // 通知信号，裸机下设置标志，RTOS下释放信号量
    static void notify_from_isr() {}

    // 获取信号，裸机下等待标志，RTOS下等待信号量
    static bool acquire() { return false; }

    // 初始化
    static void init() {}
};


#endif //FURINA_KEY_HPP
