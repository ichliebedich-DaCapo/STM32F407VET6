//
// Created by fairy on 2024/10/12.
//
/**
 * @brief 双音频信号发生器
 * @note
 *  第一轮测试：
 *      在128*16K的定时器周期下，向DAC发送数据（定时器中断有切换频率操作），最大只能达到128*13K，即使是直接通过DAC寄存器操作
 *      把初始时钟设为256*16K，并且屏蔽切换频率操作，最大能达到128*26K，即256*13K。补上切换频率操作，最大能达到128*13K，即256*6.5K
 *      与操作手册上的256K相比，要快一些
 *
 *      后来看到要求才发现是256*800与256*1K，无意中测了一下DAC最快转换速率
 */
#include "app.hpp"

#include "timer.h"
#include "dac.h"
#include "wave_signal.hpp"
#include "key.hpp"
#include "GUI.hpp"
#include "WaveCurve.hpp"


static inline auto set_freq(bool freq) -> void
{
    if (freq)
        timer6_set_freq(FREQ_84M_to_256x800);// 256*800
    else
        timer6_set_freq(FREQ_84M_to_256x1k);// 256*1k
}

/**************************************************/
void app_init()
{
    timer6_init(FREQ_84M_to_256x800);// 预期1KHz
    dac_init();
}

void key_handler()
{

    switch (Key::getCode())
    {
        case keyk0:// 播放
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                ImageButton::press(GUI_Base::get_ui()->main.imgbtn_play);

            } else
            {
                ImageButton::release(GUI_Base::get_ui()->main.imgbtn_play);
            }
            break;

        case keyk1:// 播放模式
            if (!Key::stateHandler(KEY_STATE_TWO))
            {// 单频模式
                set_freq(uiInterface::get_freq());
            }
            Button::click(GUI_Base::get_ui()->main.btn_mode);
            break;

        case keyk2:// 偏置-
            Button::click(GUI_Base::get_ui()->main.btn_bias_sub);
            break;

        case keyk3:// 偏置+
            Button::click(GUI_Base::get_ui()->main.btn_bias_add);
            break;

        case keyk4:// 比例-
            Button::click(GUI_Base::get_ui()->main.btn_ratio_sub);
            break;

        case keyk5:// 比例+
            Button::click(GUI_Base::get_ui()->main.btn_ratio_add);
            break;

        case keyk6:// 频率
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                timer6_set_freq(FREQ_84M_to_256x1k);// 256*1k
            } else
            {
                timer6_set_freq(FREQ_84M_to_256x800);// 256*800
            }
            if (uiInterface::get_mode())
            {
                set_freq(uiInterface::get_freq());
            }
            Button::click(GUI_Base::get_ui()->main.btn_freq);
            break;


        case keyk7:// 波形是否产生
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                uiInterface::wave_is_generate();
                timer6_start_it();
                dac_start();
            } else
            {
                uiInterface::wave_is_not_generate();
                timer6_stop_it();
                dac_stop();
            }
            break;

        case keyk8:// 减少波形点数
            uiInterface::sub_wave_cnt();
            break;
        case keyk9:// 增加波形点数
            uiInterface::add_wave_cnt();
            break;
        case keykA:// 减少周期
            uiInterface::sub_period();
            break;
        case keykB:// 增加周期
            uiInterface::add_period();
            break;
        case keykC:// 切换波形类型
            uiInterface::switch_wave_type();

            break;
        case keykD:// 清屏
            uiInterface::clear_screen();
            break;
        case keykE:// 是否显示FPS一帧时间
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                uiInterface::set_fps_mode(true);
            } else
            {
                uiInterface::set_fps_mode(false);
            }
            break;
        case keykF:// 是否显示FPS
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                uiInterface::show_fps(true);
            } else
            {
                uiInterface::show_fps(false);
            }
            break;

        default:
            break;
    }
}

// 切换频率
static inline auto switch_freq(bool &freq) -> void
{
    freq = !freq;
    if (freq)
        timer6_set_freq(FREQ_84M_to_256x800);// 256*800
    else
        timer6_set_freq(FREQ_84M_to_256x1k);// 256*1k
}
// DAC1 地址
#define DAC1_DR *((volatile uint32_t *)(0x40007408))
extern "C"
{
void timer6_isr()
{
    static bool flag = true;
    static bool freq = false;
    static uint8_t index = 0;// 索引
    static uint32_t count = HAL_GetTick();// 记录时间起点
    //    HAL_DAC_SetValue(&hdac, DAC1_CHANNEL_1, DAC_ALIGN_12B_R,sine_wave[index]<<4);

    // 防止数据溢出
    int wave_date = sineWave[index++] + uiInterface::get_bias();
    wave_date = (wave_date > 255) ? 255 : (wave_date < 0) ? 0 : wave_date;
    DAC1_DR = wave_date << 4;

    // 检测模式
    if (uiInterface::get_mode())
    {
        // 获取计差数值
        volatile uint32_t temp_count = HAL_GetTick();
        if (temp_count >= count)
        {
            temp_count -= count;
        } else
        {
            temp_count += (0xFFFFFFFF - count);// 溢出处理
        }

        /*切频处理**/

        // 1K -> 800
        if (freq)
        {
            if (uiInterface::get_ratio() != 0)
            {
                // 使用大于号是为了防止频率过低导致无法时间会偏离
                if (temp_count >= uiInterface::get_period() << 1)
                {
                    flag = true;
                    set_freq(false);
                    freq= false;
                    count = HAL_GetTick();// 重置计数值
                }
            }
        } else// 800 -> 1K
        {
            // 极端情况：
            // ① _ratio为100，那么会直接跳过
            // ② _ratio为0，那么会一直执行这个
            if (uiInterface::get_ratio() != 100)
            {
                // 切频处理
                if (flag)// 判断是否是第一次
                {
                    // 使用大于号是为了防止频率过低导致无法时间会偏离
                    if (temp_count >= uiInterface::get_period() * uiInterface::get_ratio() / 50)
                    {
                        flag = false;// 停止切换
                        freq = true;
                        set_freq(true);
                    }
                    /* 这个时候可以做一些别的处理，比如切换波形，但是这个机制还没有完善（低耦合）*/
                }
            }
        }
    }
}
}
