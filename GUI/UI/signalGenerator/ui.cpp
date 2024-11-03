//
// Created by fairy on 2024/10/22 16:34.
//
#include "ui.hpp"
#include "GUI.hpp"
/**
 * @简介：双音频信号发生器
 * @note
 *      第一轮测试：
 *          绘制速度极慢，预期6ms(10ms)一帧。实际一帧为：
 *          开Ofast -> 40000/230=174ms    开Og -> 400000/216=185ms    开Oz -> 400000/213=188ms
 *
 *      第二轮测试（开Ofast）：
 *          动手做了一个简单的测试辅助工具，测试结果如下：
 *          注释掉回调函数的实现，实际一帧为22.51ms    仅注释掉回调函数中向图表中添加数据的部分，实际一帧为：21.23ms
 *          不注释(64个点)，实际一帧为：181.29ms(缓慢增长中)     不注释(32个点)，画质很差，且实际一帧为：177.45ms
 *          说明耗时在图表添加数据中,刷屏和图表更新数据机制，使得降幅可达159.43ms，即降低了88%的性能。
 *
 *          【测试刷屏的影响，把回调函数实现部分替换为刷屏函数
 *                  使用DMA ，整个图表区域刷屏(320*200)->140.39ms   1/2屏->81ms     1/4个屏->52ms
 *                  不使用DMA，整个图表区域刷屏->140ms↓    1/2屏->82ms↓     1/4屏->52ms↓
 *           即便DMA开到最大，但并没有提高多少，原因应该在于HAL繁冗的中断调用
 *
 *          使用寄存器写法优化了一下DMA中断回调，整个图表区域->141ms↓    1/2屏->81ms↓     1/4屏->52ms↓
 *          没有任何提升，说明瓶颈并不在中断回调。期间也把DMA的各种配置拉满了（再往上就白屏了），说明瓶颈也不在DMA
 *
 *          把缓冲区从480*10扩展为480*30，图表添加数据（64个点）->165.94ms     把缓冲区拉到最大480*109（内存占用达到99.76%）->161.47ms↑
 *          480*60->161ms↑  480*20->162.74ms    480*15->174.16ms
 *          后续测了几个数值，缓冲区拉大之后对图表添加数据没有明显帮助，刷屏确比平时快一点。
 *
 *          使用双缓冲(480*20*2)后，图表添加数据（64个点）->142.82ms 内存没有增加太多的情况下，有了些许提升
 *
 *          总结： 原版:181.29ms -----增加缓冲区----> 165.94ms  -----使用双缓冲---->  142.82ms
 *                那么此时刷屏限制在于图表更新数据的机制
 *
 *        第三轮测试：
 *          由于没怎么搞过这么底层的绘制操作，不熟练，搞了一夜总算搞个能用的示波器显示算法了，原理很简单：清除旧点->更新数据->绘制新点。
 *          当然顺序可以改变，因为是滚动更新且一次只更改一个点,这里我采用的是从最后的几个点数据开始，计算出点的坐标，清除旧点，绘制新点，直至遍历
 *          所有点数据之后，再使用memmove更新点数据数组
 *          这轮测试有三个绘制算法：线性插值、贝塞尔曲线（二次、三次）和样条曲线
 *
 *          显示128点，采样数据点为128：
 *              线性插值：71.28ms 流畅,简约
 *              平滑度：0.01
 *              贝塞尔曲线（二次)：1134.32ms 虽然很还原，但太慢了，可能是里面的浮点运算（已经开了硬件浮点的情况下）太多了,需要改一下平滑度
 *              贝塞尔曲线（三次)：1127.41ms 这是什么情况？！三次比二次还快，重新又测了一遍二次和三次，结果还是三次快。
 *                              看了一下算法，三次我只是把for循环改成了while循环，我改一下再测
 *                              都改为while循环，重测了一下，二次1136ms，三次1130ms 这是什么情况？！可能是三次贝塞尔曲线一次会计算4个点，
 *                              从而减少绘制时间，这可能是个优化思路，但并不好更改
 *              样条曲线：1179.65ms，从绘制正弦波的角度来说，不如贝塞尔曲线更平滑，有种草图毛茸茸的感觉，但更均匀
 *              平滑度：0.1
 *              贝塞尔曲线（二次)：130.04ms  确实是十倍提升，并且视觉上的平滑效果并没有减弱
 *              贝塞尔曲线（三次)：128.96ms  如预期
 *              样条曲线：133.52ms 毛刺多了一点点，但整体还是一样均匀
 *              平滑度：0.3
 *              贝塞尔曲线（二次)：64.31ms  视觉上的平滑效果降了一点点，但还是可以的
 *              贝塞尔曲线（三次)：64.15ms  视觉上的平滑效果弱了很多，仿佛散点图
 *              样条曲线：65.42ms 提升没有三倍那么大了，说明浮点运算的开销占比下降了。视觉上毛刺多了一点点，但整体均匀
 *              平滑度：0.15
 *              贝塞尔曲线（二次)：97.11ms  视觉上的平滑效果降了一点点，但还是可以的，比0.3要强一些
 *              贝塞尔曲线（三次)：96.75ms  视觉上虽平滑，但会抽动（更新速率有些慢）
 *              样条曲线：99.66ms 一如既往毛刺多，但整体均匀
 *
 *          显示64点，采样数据点为128：
 *              平滑度：0.1
 *              贝塞尔曲线（二次)：55.82ms，速度又快，平滑度还行
 *
 *          综上，论效果和速率，最好的是贝塞尔曲线（二次），比线性插值还快还平滑，这一点我是真没想到
 *          其实效率上还能继续优化，比如把步进从浮点型改为整型，不过这样做了之后，显示点数必须是宽度的整数倍，不然会不符合实际宽度。
 *          这一点我选择用constexpr选择性编译，这样可以有个自主选择的机会
 *
 *
 *          第四轮测试：
 *              优化计算坐标过程中的重复计算：
 *                  平滑度：0.1
 *                  贝塞尔曲线（二次)：130.04ms -> 130.37ms   没有任何提升只能说编译器更懂代码(-Ofast)
 *
 *              选择性优化步长计算：
 *                  平滑度：0.1
 *                  贝塞尔曲线（二次)：130.37ms -> 129ms   优化很有限，并且还限制了宽度和采样点数，把代码变长了一些，我还是改回去吧
 *
 *
 *
 *
 */
#if 1
// 头文件
#include "wave.h"
#include <cstdio>
// 宏定义
#define Strong_Print 0 // 使用更强烈的显示（时间和索引都会加倍）


// 常量
//constexpr uint32_t Freq_8K = 30;
//constexpr uint32_t Freq_16K = 15;
constexpr uint32_t Freq_8K = 24;
constexpr uint32_t Freq_16K = 12;
constexpr uint8_t index_offset_8K = 1;// 8K下索引递增值
constexpr uint8_t index_offset_16K = 2;// 16K下索引递增值
constexpr uint16_t point_cnt = 128;// 点的数量
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;

// 变量
lv_chart_series_t *series;
LV_Timer timer;
LV_Timer tick_timer;

#include "WaveCurve.hpp"

uint8_t Buf[point_cnt];

class SignalGenerator : public GUI_Base
{
public:
    // 播放
    static inline auto handler() -> void
    {
        /**可以加个值判断*/
        if (_mode)
        {
            // 16K -> 8K
            if (_freq)
            {
                if (get_tick() >= period * 2)
                {
                    if (_ratio != 0)
                    {
                        set_freq(false);// 切换为8K
                        update_count();

                    }
                }
            } else// 8K -> 16K
            {
                // 极端情况：
                // ① _ratio为100，那么会直接跳过
                // ② _ratio为0，那么会一直执行这个
                if (get_tick() >= period * _ratio / 50)
                {
                    if (_ratio != 100)
                    {
                        set_freq(true);
                    }
                }
            }
        }
//        Chart::set_next_value(series, static_cast<Coord>(sine_wave[wave_index] + bias), gui->main.chart);
        WaveCurve::draw_curve<uint8_t, uint16_t, Coord,3>(WaveCurve::draw_BezierCurve2, Buf,
                                                        static_cast<Coord>(sine_wave[wave_index] + bias), point_cnt, 80,
                                                        60, chart_width, chart_height, 255,
                                                        0xFFFF, 0);
        wave_index += index_offset;
        if (wave_index >= 128)
            wave_index = 0;
    }

    static inline auto start() -> void
    {
        timer.resume();
        tick_timer.resume();
#warning "测试"
        Tools::restart_fps();
    }

    static inline auto stop() -> void
    {
        timer.pause();
        tick_timer.pause();
    }

    // 切换频率
    static inline auto switch_freq() -> void
    {
        _freq = !_freq;
        set_freq(_freq);
        update_count();
    }

    // 切换模式
    static inline auto switch_mode() -> void
    {
        _mode = !_mode;
        set_mode(_mode);
        update_count();
    }

    // 加偏置
    static inline auto add_bias() -> void
    {
        if (bias < 120)
            bias += 10;
        char buf[12];
        sprintf(buf, "偏置：%d", bias);
        Text::set_text(buf, gui->main.label_bias);
    }

    // 减偏置
    static inline auto sub_bias() -> void
    {
        if (bias > -120)
            bias -= 10;
        char buf[12];
        sprintf(buf, "偏置：%d", bias);
        Text::set_text(buf, gui->main.label_bias);
    }

    static inline auto add_ratio() -> void
    {
        if (_ratio < 100)
            _ratio += 10;
        set_ratio(_ratio);
    }

    static inline auto sub_ratio() -> void
    {
        if (_ratio > 0)
            _ratio -= 10;
        set_ratio(_ratio);
    }

    static inline auto print_tick() -> void
    {
        char buf[12];
        sprintf(buf, "tick：\n%lu", get_tick());
        Text::set_text(buf, gui->main.label_tick);
    }

private:
    // 设置频率,true表示16K，false表示8K
    static inline auto set_freq(bool freq) -> void
    {
        _freq = freq;
        index_offset = _freq ? index_offset_16K : index_offset_8K;
        Text::set_text(_freq ? "16K" : "8K", gui->main.btn_freq_label);

#if Strong_Print
        timer.set_period(_freq ? Freq_16K : Freq_8K);
#endif
    }

    static inline auto set_mode(bool &mode) -> void
    {
        _mode = mode;
        Text::set_text(_mode ? "双频" : "单频", gui->main.btn_mode_label);
    }

    static inline auto set_ratio(uint8_t ratio) -> void
    {
        _ratio = ratio;
        char buf[12];
        sprintf(buf, "占比：%d%%", _ratio);
        Text::set_text(buf, gui->main.label_ratio);
    }

    // 更新时间
    static inline auto update_count() -> void
    {
        count = lv_tick_get();
    }

    // 获取时间
    static inline auto get_tick() -> uint32_t
    {
        uint32_t temp_count = lv_tick_get();
        // 防止溢出
        if (temp_count < count)
            temp_count += (0xFFFF'FFFF - count);
        else
            temp_count -= count;

        return temp_count;
    }

private:
    static inline uint32_t count = 0;// 用于计时
    static inline int8_t bias = 0;
    static inline uint8_t _ratio = 50;// 8K的占比
    static inline uint8_t wave_index = 0;
    static inline uint8_t index_offset = 1;
    static inline bool _mode = false;// 默认模式为单频
    static inline bool _freq = false;// 默认频率为8K
    static constexpr uint32_t period = 1500;// 周期为1500ms
};


/***********************函数实现***********************/
auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);

    // 创建图表对象
    Chart::init(gui->main.chart, 0, -15, 320, 200, point_cnt);
    Chart::add_series(series, lv_color_hex(0x00ff00)); // 添加数据序列

    // 初始化器
    Button customBtn;
    customBtn.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    Text label;
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_13);
    Component::set_parent(gui->main.screen);// 设置父对象

//     按钮_模式：单频、混频
    customBtn.init(gui->main.btn_mode, gui->main.btn_mode_label, 40, 265, 40, 30, "单频");
    label.init(gui->main.label_mode, 45, 300, 40, 20, "模式");

//     按钮_偏置：+、-
    customBtn.init(gui->main.btn_bias_sub, gui->main.btn_bias_sub_label, 115, 265, 30, 30, "-");
    customBtn.init(gui->main.btn_bias_add, gui->main.btn_bias_add_label, 155, 265, 30, 30, "+");
    label.init(gui->main.label_bias, 118, 300, 80, 20, "偏置：0");

//     按钮_频率：8K、16K
    customBtn.init(gui->main.btn_freq, gui->main.btn_freq_label, 400, 265, 40, 30, "8K");// 根据对称性，x：480-40-40
    label.init(gui->main.label_freq, 405, 300, 40, 20, "频率");

//     按钮_占比：+、-
    customBtn.init(gui->main.btn_ratio_sub, gui->main.btn_ratio_sub_label, 295, 265, 30, 30, "-");
    customBtn.init(gui->main.btn_ratio_add, gui->main.btn_ratio_add_label, 335, 265, 30, 30, "+");
    label.init(gui->main.label_ratio, 298, 300, 80, 20, "占比：50%");

//     图片按钮：停止、播放
    ImageButton::init(gui->main.imgbtn_play, 216, 256, 48, 48, &_btn_list_play_alpha_48x48,
                      &_btn_list_pause_alpha_48x48);

    // 标签：信息
    label.init(gui->main.label_info, 420, 60, 60, 80, "  队伍：\n\n王正翔\n吴俊颐\n谢智晴\n何乃滔");

    // 标签：时刻
    label.init(gui->main.label_tick, 20, 100, 60, 80, "tick：\n0");
    Text::set_text_color(Color_Firefly_Green);

    // 标签：CPU温度
    label.init(gui->main.label_cpu, 0, 0, 80, 30, "CPU:35℃\n");

    // 标签：标题
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    label.init(gui->main.label_title, 190, 13, 140, 30, "双音频信号发生器");


    Tools::fps_init(&lv_customer_font_SourceHanSerifSC_Regular_13, 10, 40);
}


/***********************函数实现***********************/

//uint16_t color[320*200];
auto Events::init() -> void
{
    /*  创建定时器*/
    timer.create([](lv_timer_t *)
                 {
                     SignalGenerator::handler();
//                     static uint16_t cnt = 0;
//                     constexpr uint16_t N = 256;
//                     WaveCurve::draw_curve<uint8_t, uint16_t, Coord>(WaveCurve::draw_interpolated_line, Buf,
//                                                                     sine_wave[(cnt++) % 128], N, 80, 60, 320, 200, 255,
//                                                                     0xFFFF, 0);
                     Tools::fps();
                 }, Freq_8K);
    tick_timer.create([](lv_timer_t *)
                      {
                          SignalGenerator::handler();
                          SignalGenerator::print_tick();
                      }, 10);
#warning "测试中"
    // 绑定播放事件
    bond(gui->main.imgbtn_play, imgbtn_fun2(
            fun(SignalGenerator::start();),
            fun(SignalGenerator::stop();)));

    // 绑定频率事件
    bond(gui->main.btn_freq, btn_fun(
            fun(SignalGenerator::switch_freq();)));

    // 绑定偏置事件
    bond(gui->main.btn_bias_add, btn_fun(
            fun(SignalGenerator::add_bias();)));
    bond(gui->main.btn_bias_sub, btn_fun(
            fun(SignalGenerator::sub_bias();)));

    // 绑定模式事件
    bond(gui->main.btn_mode, btn_fun(
            fun(SignalGenerator::switch_mode();)));

    // 绑定占比事件
    bond(gui->main.btn_ratio_add, btn_fun(
            fun(SignalGenerator::add_ratio();)));
    bond(gui->main.btn_ratio_sub, btn_fun(
            fun(SignalGenerator::sub_ratio();)));
}


#endif