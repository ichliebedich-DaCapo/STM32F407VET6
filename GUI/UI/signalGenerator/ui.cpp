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
 *          不注释(64个点)，实际一帧为：181.29ms(缓慢增长中)     不注释(32个点)，画质很差且卡顿，且实际一帧为：177.45ms
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
 *        第三轮测试（双绘制）：
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
 *          第四轮测试（双绘制）：
 *              优化计算坐标过程中的重复计算：
 *                  平滑度：0.1
 *                  贝塞尔曲线（二次)：130.04ms -> 130.37ms   没有任何提升只能说编译器更懂代码(-Ofast)
 *
 *              选择性优化步长计算：
 *                  平滑度：0.1
 *                  贝塞尔曲线（二次)：130.37ms -> 129ms   优化很有限，并且还限制了宽度和采样点数，把代码变长了一些，我还是改回去吧
 *
 *              优化缓存命中没有任何提升
 *
 *              综上，除非是从根本上改变算法，否则在开了(-Ofast)的情况下，优化极为有限。
 *
 *          第五轮测试：
 *              Mud（murder），准备排除图表影响时，发现不小心在另一个定时器里也加了一个绘制函数，也就是说在绘制两遍的情况下还能达到130ms。把这个多余的函数注释掉之后
 *              耗时直接降到75.24ms.使用线性插值，耗时直降45.84ms，而且更加简约.样条算法是77.01ms，还是有些毛刺
 *
 *              排除图表影响后（其中毙掉了图表的计时器），75.24ms -> 31.72ms，只能说刁爆了，与定时器的24ms十分接近。出于好奇，
 *              注释掉绘制函数，测了一下定时器，发现是完全等于24.00ms，且测量多次结果相同，看来硬件定时器提供的lv_tick在lv_timer_handler()的英明指导下几乎没有损失
 *
 *          总结：使用自定义的绘制算法，可以将速度从181ms提升到75ms（贝塞尔二次曲线），甚至是45ms（线性插值），虽然达到不到定时器的24ms，
 *          但已经算是流畅了，我已经是处心积虑、手段用尽了哈
 *
 *          第六轮测试：
 *              线性插值：
 *                  发现绘制曲线时会有漏墨发生，也就是对于同一段曲线的绘制，你先绘制再清除，得到的结果竟然不同，会有残余的像素在里面。后来不知道修改了什么(x_x)
 *              原本发生在全屏的漏墨现象，结果只会出现在右半部分，而且非常严重。
 *              经过观察，发现暂停再重开会有一瞬间出现正确的曲线,这说明可能是绘制初始段时，过快导致没有绘制完全.于是我把定时器周期调成与实际刷新周期差不多，
 *              结果真的消失了，小概率会出现漏墨，但基本不会。只不过这种情况会很大程度上影响刷新，原本定时器周期为24ms实际为42ms，只能被迫改为定时器周期为60ms，
 *              如果改成50ms仍会漏墨。最好改为70ms
 *
 *              贝塞尔曲线（二次）：
 *                  现在发现前面为什么得到的漏墨现象为什么不同了，因为当初看到的是这个的漏墨。
 *                  实际定刷新周期为72ms，于是把定时器周期改为80ms，实际81ms但仍会漏墨。改为90ms还会漏墨。一口气改为160ms，还是会漏墨，没救了
 *
 *              尝试解决：
 *                  我首先想到的是更改清除旧线的函数，原先清除的方式时重新计算一遍曲线，然后上背景颜色，但这样显然太慢了。于是我把清除旧线函数改为清除矩形块
 *                  为了不影响之前的绘制，我选定x的最值作为矩形边界。好消息是绘制速度又提升了，坏消息是前段仍会漏墨，我得查找更底层的原因了。
 *                      线性插值：36.21ms 这个下降了，可能是因为计算上耗时远小于刷屏
 *                      贝塞尔曲线（二次）： 50.58ms
 *                      贝塞尔曲线（三次）：50.31ms
 *                      样条曲线： 测试样条函数时，结果程序卡死，换模拟器是正常的
 *                  提升效果之明显，以至于绘线的同时可以更新FPS，原本显示FPS时需要暂停，现在直接实时刷新。
 *
 *                      我改了一下贝塞尔曲线（二次）绘制函数，现在的思路是用两个数组分别轮流存计算值，在一个for循环里先进行一次计算，然后清除旧的一段曲线，
 *                  再绘制新的一段曲线，最后再交换指针。按理来说，这种方法应该最优，无论是计算开始for循环展开开销都是最小，可实测却是76.57ms(定时器开的是24ms)。
 *                  同时绘制区域右边部分会漏墨，这是我无法理解的，与模拟器上的结果相差较大。
 *                      不过好在漏墨不那么严重了，开到80ms基本不漏墨.试了一下线性插值，发现漏墨更严重了，并且速率只有45ms，我记得我没有改什么啊，真是奇怪。
 *
 *         第7轮尝试：
 *              我好像找到原因了，在模拟器上正常，在真机上不正常，把定时器周期调大之后就正常了。表层现象是右边开头的几个段没有绘制完全，真不知道是什么原因导致的。
 *              等会我试试硬件定时器，看看有没有问题。此时一帧为79ms左右，不知道之前是怎么做到50ms的
 *
 *
 *
 *
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
constexpr uint16_t start_x = 80;
constexpr uint16_t start_y = 45;
constexpr uint16_t sine_count = 128;// 采样点数量
constexpr uint16_t max_value = 255;

// 变量
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
                if (_ratio != 0)
                {
                    if (get_tick() >= period * 2)
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
                if (_ratio != 100)
                {
                    if (get_tick() >= period * _ratio / 50)
                    {
                        set_freq(true);
                    }
                }
            }
        }
        // 防止数据溢出
        int wave_date = sine_wave[wave_index] + bias;
        wave_date = (wave_date > max_value) ? max_value : (wave_date < 0) ? 0 : wave_date;

        WaveCurve::draw_curve<WaveCurve::Type::BezierCurve3, uint8_t>(Buf,point_cnt, static_cast<uint8_t>(wave_date),
                                                                                start_x,
                                                                                start_y, chart_width, chart_height, 255,
                                                                                0xFFFF, 0);

        if (wave_index >= sine_count - 1)[[unlikely]]
        {
            wave_index -= sine_count - 1;// 尽量让转折处柔和一些
        } else
        {
            wave_index += index_offset;
        }
    }

    static inline auto start() -> void
    {
        tick_timer.resume();
#warning "测试"
        Tools::restart_fps();
    }

    static inline auto stop() -> void
    {
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
    label.init(gui->main.label_tick, 20, 100, 50, 80, "tick：\n0");
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
    tick_timer.create([](lv_timer_t *)
                      {
                          SignalGenerator::handler();
                          Tools::fps();
                          SignalGenerator::print_tick();
                      }, Freq_8K);
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