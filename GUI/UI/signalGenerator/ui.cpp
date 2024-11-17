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
 *              试了一下不是硬件定时器的原因，线性插值函数测量结果如下：
 *                  100ms执行，结果实际为75ms？？
 *                  50ms执行，实际为23ms ？？？
 *                  30ms执行，实际为7ms，漏墨严重
 *                  60帧，即16.7ms 停不下来
 *                  3ms执行，还是停不下来
 *                  2ms执行，根本停不下来 50Hz，有点漏墨
 *
 *               也就是说硬件定时器显然更稳一点，但还是会漏墨，又试了一下把绘制函数直接放在主循环里，还是会漏墨。这说明漏墨无关软件，是硬件层面了。
 *               这也能解释为什么模拟器没有问题，但真机会漏墨了。只不过这个显示帧率为毛会这么快，fps显示我是根据lv_tick_get计算的，这个时钟获取
 *               是非常准的，因为我把lv_tick_inc丢到了与hal_tick_inc同位置的系统定时器中断里，真是诡异
 *
 *               接下来准备最终的测试，根据效果决定使用哪种算法和条件：
 *               点数：128 正弦波形数据：128（除了样条曲线，其余均-Ofast优化）
 *                  线性插值：
 *                      40ms -> 45-46ms: 漏墨严重
 *                      50ms -> 50~53ms： 基本正常，如果波形变化剧烈则会严重漏墨
 *                      60ms -> 60-63ms： 很稳，只要按按键时不要过于剧烈迅速就不会漏墨
 *                  !!  70ms -> 70.0-70.1ms： 帧率很稳，但还是不要剧烈按按键
 *                      80ms -> 80ms-80.08ms： 非常稳，剧烈按按键的情况下，小概率会漏墨
 *
 *                  贝塞尔曲线（二次）：
 *                      70ms -> 77-79ms:   漏墨严重
 *                      80ms -> 80-81ms:    轻微漏墨，绘制曲线按按键时，文本框更新会有轻微失常
 *                  !!  90ms -> 90-90.3ms:  非常稳定，基本不会漏墨，但不够连续，有点像散点图
 *
 *                  贝塞尔曲线（三次）：
 *                      70ms -> 77-79ms: 右侧中度漏墨
 *                      80ms -> 80-82ms: 轻微漏墨，绘制曲线按按键时，文本框更新会有轻微失常
 *                  !!  90ms -> 从93稳定到90.2ms： 非常稳定，强烈按按键时，小概率漏墨，比二次曲线更连续一些，有点毛刺
 *
 *                  Catmull-Rom样条曲线：
 *                       直接卡死： 这个我也无法理解，自从不知道什么时候起，优化过后在模拟器里一切正常，但真机会直接卡死。在数组前加上volatile，
 *                       栈大小增到1000B，但还是会卡死。然后把优化等级开为-Og就正常了，开-O2以上级别都会卡死，难评
 *                  !!   90ms -> 90-91ms 非常稳定，强烈按按键时，小概率会漏墨，像就像马赛克一样的曲线，不过非常均匀，如果粗一些，那就是最优雅的曲线
 *                       80ms -> 80-81ms: 按按键会右侧中度漏墨
 *
 *               点数：100 正弦波形数据：64（除了样条曲线，其余均-Ofast优化）
 *                  线性插值：
 *                      40ms -> 40-42ms: 右侧中度漏墨
 *                      50ms -> 50-51ms: 按按键时，轻微漏墨,曲线看着很流畅
 *                      60ms -> 60-60.2ms: 稳定一批，基本不漏墨，但16K波形显示会严重漏墨
 *                      70ms -> 72-74ms: 直接测量16K波形显示，所以帧率会下降，且右侧严重漏墨，尤其是按按键
 *                  !!  80ms -> 80.1-80.4ms：16K波形依旧稳定，有些卡顿的感觉，看来前面没有测16K就有些不妥了，后面全测16K情况
 *
 *                  贝塞尔曲线（二次）：
 *                      70ms -> 70-70.2ms: 很稳定，只有不剧烈按按键就行，总算反超线性插值了
 *                  !!  80ms -> 80.1-80.2ms: 非常之稳定，但有些散点。之所以稳定，原因很简单，平缓度设置的为0.1，即两个控制点固定描11个点。
 *
 *                  贝塞尔曲线（三次）：
 *                      70ms -> 70.06-70.1ms: 很稳定，只有不剧烈按按键就行，第帧率情况下只能说散点图看着更流畅，不像某某线性插值
 *                      80ms： 这还用测吗，肯定和二次差不多，这种情况下它俩基本没什么差别，只不过三次毛刺多一点
 *
 *                  Catmull-Rom样条曲线：
 *                  !!    80ms -> 80ms-80.2ms: 非常稳定，强烈按按键时，小概率会漏墨
 *                      60ms -> 65-68ms: 右侧中度漏墨，曲线还行，就像一条毛线
 *
 *             总结：
 *                  点数多（>100），那么就选用线性插值，比其他两种方法要流畅且稳定
 *                  点数少（<100），那么就选用贝塞尔曲线或者Catmull-Rom样条曲线，低帧率下散点图会比线性插值更流畅。同时Catmull-Rom样条曲线非常
 *                  优雅均匀。
 *                  线性插值不适合变化剧烈的波形数据，否则容易漏墨，而其他两种比较适合这种情况
 *
 *
 *

 */
// 头文件
#include "wave.h"
#include <cstdio>
#include "WaveCurve.hpp"

// 宏定义
#define Strong_Print 0 // 使用更强烈的显示（时间和索引都会加倍）

// 常量
//constexpr uint32_t Freq_8K = 30;
//constexpr uint32_t Freq_16K = 15;
constexpr uint32_t Freq_8K = 75;
constexpr uint32_t Freq_16K = 12;
constexpr uint8_t index_offset_8K = 2;// 8K下索引递增值
constexpr uint8_t index_offset_16K = 4;// 16K下索引递增值
constexpr uint16_t point_cnt = 256;// 点的数量
constexpr uint16_t chart_width = 320;
constexpr uint16_t chart_height = 200;
constexpr uint16_t start_x = 80;
constexpr uint16_t start_y = 40;
constexpr uint16_t sine_count = 128;// 采样点数量
constexpr uint16_t max_value = 255;
constexpr uint16_t bg_color = 0xFFFF;
constexpr uint16_t line_color = 0x0000;

// 变量
LV_Timer tick_timer;
uint8_t Buf[point_cnt];


// 播放
class SignalGenerator
{
public:
    // 播放
    static inline auto handler() -> void;

    // 开始
    static inline auto start() -> void;

    // 停止
    static inline auto stop() -> void;

    // 切换频率
    static inline auto switch_freq() -> void;

    // 切换模式
    static inline auto switch_mode() -> void;

    // 加偏置
    static inline auto add_bias() -> void;

    // 减偏置
    static inline auto sub_bias() -> void;

    // 获取偏置
    static inline auto get_bias() -> int8_t;

    // 加比例
    static inline auto add_ratio() -> void;

    // 减比例
    static inline auto sub_ratio() -> void;

    // 获取比例
    static inline auto get_ratio() -> uint8_t;

    // 打印时间
    static inline auto print_tick() -> void;

    // 是否显示FPS
    static inline auto show_fps(bool is_show) -> void;

    // 是否显示FPS一帧时间
    static inline auto set_fps_mode(bool fps_mode) -> void;

    // 增加波形点数
    static inline auto add_wave_cnt() -> void;

    // 减少波形点数
    static inline auto sub_wave_cnt() -> void;

    // 切换波形类型
    static inline auto switch_wave_type() -> void;

    // 波形已产生
    static inline auto wave_is_generate() -> void;

    // 波形没有产生
    static inline auto wave_is_not_generate() -> void;

    // 增加时钟周期
    static inline auto add_period() -> void;

    // 减少时钟周期
    static inline auto sub_period() -> void;

    // 获取时钟周期
    static inline auto get_period() -> uint32_t;

    // 获取波形模式
    static inline auto get_mode() -> bool ;

    // 获取频率
    static inline auto get_freq() -> bool;

private:
    // 设置频率,true表示16K，false表示8K
    static inline auto set_freq(bool freq) -> void;

    static inline auto set_mode(bool &mode) -> void;

    static inline auto set_ratio(uint8_t ratio) -> void;

    // 更新时间
    static inline auto update_count() -> void;

    // 获取时间
    static inline auto get_tick() -> uint32_t;

private:
    static inline uint32_t count = 0;// 用于计时
    static inline uint32_t period = 1500;// 周期为1500ms
    static inline int8_t bias = 0;
    static inline uint8_t _ratio = 50;// 8K的占比
    static inline uint8_t wave_index = 0;
    static inline uint8_t index_offset = 1;
    static inline bool _mode = false;// 默认模式为单频
    static inline bool _freq = false;// 默认频率为8K

    static inline uint8_t wave_cnt = 100;//显示点数 -> keyk8、keyk9
    static inline uint8_t wave_type = 1;// 波形类型 keyk10 过一段时间会消失
    static inline bool is_fps = false;// 是否开启FPS -> keyk15
    static inline bool is_fps_time = false;// 是否开启FPS时间 -> keyk14
};


/***********************函数实现***********************/
auto Screen::init() -> void
{
    Component::set_parent(gui->main.screen);

    // 设置边框
    Component::init(gui->main.rect);
    Component::set_pos_size(78, 36, 324, 208);
    Component::border_radius(5);

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
    customBtn.init(gui->main.btn_freq, gui->main.btn_freq_label, 400, 265, 40, 30, "800");// 根据对称性，x：480-40-40
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

    // 标签：CPU温度
    label.init(gui->main.label_cpu, 0, 0, 80, 30, "CPU:35℃\n");

    // 标签：时刻
    label.init(gui->main.label_tick, 5, 50, 50, 80, "tick：\n0");

    // 标签：显示点数
    label.init(gui->main.label_wave_cnt, 5, 110, 60, 80, "点数：128");

    // 标签：波形周期
    label.init(gui->main.label_wave_period, 5, 160, 70, 80, "周期：1.5s");

    // 标签：算法
    label.init(gui->main.label_wave_type, 5, 210, 60, 80, "线性插值");

    // 标签：波形生成
    label.init(gui->main.label_wave_generate, 430, 200, 50, 80, "波形已产生");
    Text::hidden(gui->main.label_wave_generate);

    // 标签：标题
    label.init_font(&lv_customer_font_SourceHanSerifSC_Regular_15);
    label.init(gui->main.label_title, 190, 13, 140, 30, "双音频信号发生器");

    // print
    FPS::init(&lv_customer_font_SourceHanSerifSC_Regular_13, 415, 0);
    FPS::set_right();
}


/***********************函数实现***********************/

//uint16_t color[320*200];
auto Events::init() -> void
{
    /*  创建定时器*/
    tick_timer.create([](lv_timer_t *)
                      {
                          SignalGenerator::handler();
                          SignalGenerator::print_tick();
                      }, Freq_8K);
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


auto SignalGenerator::handler() -> void
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

    // 选择算法绘制波形
    switch (wave_type)
    {
        case 1:
            WaveCurve<>::draw_curve<WaveCurveType::Interpolated_Line, uint8_t>(Buf, wave_cnt, wave_date, start_x,
                                                                               start_y,
                                                                               chart_width, chart_height, 255,
                                                                               0xFFFF, 0);
            break;
        case 2:
            WaveCurve<>::draw_curve<WaveCurveType::BezierCurve2, uint8_t>(Buf, wave_cnt, wave_date, start_x,
                                                                          start_y,
                                                                          chart_width, chart_height, 255, 0xFFFF,
                                                                          0);
            break;
        case 3:
            WaveCurve<>::draw_curve<WaveCurveType::BezierCurve3, uint8_t>(Buf, wave_cnt, wave_date, start_x,
                                                                          start_y,
                                                                          chart_width, chart_height, 255, 0xFFFF,
                                                                          0);
            break;
        case 4:
            WaveCurve<>::draw_curve<WaveCurveType::CatmullRomSp_line, uint8_t>(Buf, wave_cnt, wave_date, start_x,
                                                                               start_y,
                                                                               chart_width, chart_height, 255,
                                                                               0xFFFF, 0);
            break;

        default:
            break;

    }


    wave_index += index_offset;
    if (wave_index >= sine_count)[[unlikely]]
    {
        wave_index -= sine_count;// 尽量让转折处柔和一些
    }

    // 显示FPS
    if (is_fps)
        FPS::print(is_fps_time);
}


auto SignalGenerator::start() -> void
{
    tick_timer.resume();
    // 重置FPS刷新计数
    if (is_fps)
        FPS::restart();
}

auto SignalGenerator::stop() -> void
{
    tick_timer.pause();
}

// 切换频率
auto SignalGenerator::switch_freq() -> void
{
    _freq = !_freq;
    set_freq(_freq);
    update_count();
}

// 切换模式
auto SignalGenerator::switch_mode() -> void
{
    _mode = !_mode;
    set_mode(_mode);
    update_count();
}

// 加偏置
auto SignalGenerator::add_bias() -> void
{
    if (bias < 120)
        bias += 10;
    char buf[12];
    sprintf(buf, "偏置：%d", bias);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_bias);
}

// 减偏置
auto SignalGenerator::sub_bias() -> void
{
    if (bias > -120)
        bias -= 10;
    char buf[12];
    sprintf(buf, "偏置：%d", bias);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_bias);
}

auto SignalGenerator::add_ratio() -> void
{
    if (_ratio < 100)
        _ratio += 10;
    set_ratio(_ratio);
}

auto SignalGenerator::sub_ratio() -> void
{
    if (_ratio > 0)
        _ratio -= 10;
    set_ratio(_ratio);
}

auto SignalGenerator::print_tick() -> void
{
    Text::set_text(GUI_Base::get_ui()->main.label_tick, "tick：\n%lu", get_tick());
}


// 设置频率,true表示16K，false表示8K
// 由于800与1K没什么区分度，这里实际使用了8K与16K的关系，即两倍
auto SignalGenerator::set_freq(bool freq) -> void
{
    _freq = freq;
    index_offset = _freq ? index_offset_16K : index_offset_8K;
    Text::set_text(_freq ? "1K" : "800", GUI_Base::get_ui()->main.btn_freq_label);

#if Strong_Print
    timer.set_period(_freq ? Freq_16K : Freq_8K);
#endif
}

auto SignalGenerator::set_mode(bool &mode) -> void
{
    _mode = mode;
    Text::set_text(_mode ? "双频" : "单频", GUI_Base::get_ui()->main.btn_mode_label);
}

auto SignalGenerator::set_ratio(uint8_t ratio) -> void
{
    _ratio = ratio;
    char buf[12];
    sprintf(buf, "占比：%d%%", _ratio);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_ratio);
}

// 更新时间
auto SignalGenerator::update_count() -> void
{
    count = lv_tick_get();
}

// 获取时间
auto SignalGenerator::get_tick() -> uint32_t
{
    uint32_t temp_count = lv_tick_get();
    // 防止溢出
    if (temp_count < count)
        temp_count += (0xFFFF'FFFF - count);
    else
        temp_count -= count;

    return temp_count;
}

auto SignalGenerator::show_fps(bool is_show) -> void
{
    is_fps = is_show;
    if (is_show)
        FPS::restart();
}

/**
 * 设置FPS显示模式
 * @param fps_mode ture表示显示一帧消耗的时间，false表示显示帧数
 */
auto SignalGenerator::set_fps_mode(bool fps_mode) -> void
{
    is_fps_time = fps_mode;
}

auto SignalGenerator::add_wave_cnt() -> void
{
    char buf[15];
    if (wave_cnt < 180)
    {
        if (wave_cnt == 128)
            wave_cnt += 2;
        else
            wave_cnt += 10;
    }

    LCD_Color_Clean(start_x, start_y, start_x + chart_width, start_y + chart_height, bg_color);// 清除显示
    sprintf(buf, "点数：%3d", wave_cnt);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_wave_cnt);
}

auto SignalGenerator::sub_wave_cnt() -> void
{
    char buf[15];
    if (wave_cnt > 20)
    {
        if (wave_cnt == 128)
            wave_cnt -= 8;
        else
            wave_cnt -= 10;
    }

    LCD_Color_Clean(start_x, start_y, start_x + chart_width, start_y + chart_height, bg_color);// 清除显示
    sprintf(buf, "点数：%3d", wave_cnt);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_wave_cnt);
}

auto SignalGenerator::switch_wave_type() -> void
{
    char buf[32];
    ++wave_type;
    if (wave_type > 4)
        wave_type = 1;

    switch (wave_type)
    {
        case 1:
            sprintf(buf, "线性插值");
            break;

        case 2:
            sprintf(buf, "二次贝塞尔曲线");
            break;

        case 3:
            sprintf(buf, "三次贝塞尔曲线");
            break;

        case 4:
            sprintf(buf, "样条曲线");
            break;

        default:
            sprintf(buf, "无效曲线");
            break;
    }
//    LCD_Color_Clean(80, 40, 400, 240, 0xFFFF);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_wave_type);
}


auto SignalGenerator::wave_is_generate() -> void
{
    Text::appear(GUI_Base::get_ui()->main.label_wave_generate);
}

auto SignalGenerator::wave_is_not_generate() -> void
{
    Text::hidden(GUI_Base::get_ui()->main.label_wave_generate);
}

auto SignalGenerator::add_period() -> void
{
    char buf[14];
    if (period < 9000)
        period += 100;
    sprintf(buf, "周期：%lu.%lus", period/1000,period%1000/100);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_wave_period);
}

auto SignalGenerator::sub_period() -> void
{
    char buf[14];
    if (period > 100)
        period -= 100;
    sprintf(buf, "周期：%lu.%lus", period/1000,period%1000/100);
    Text::set_text(buf, GUI_Base::get_ui()->main.label_wave_period);
}

auto SignalGenerator::get_period() -> uint32_t
{
    return period;
}


auto SignalGenerator::get_ratio() -> uint8_t
{
    return _ratio;
}

auto SignalGenerator::get_bias() -> int8_t
{
    return bias;
}

auto SignalGenerator::get_mode() -> bool
{
    return _mode;
}

auto SignalGenerator::get_freq() -> bool
{
    return _freq;
}


/*****************************对外接口**************************/
auto uiInterface::show_fps(bool is_show) -> void
{
    SignalGenerator::show_fps(is_show);
}

/**
 * 设置FPS显示模式
 * @param fps_mode ture表示显示一帧消耗的时间，false表示显示帧数
 */
auto uiInterface::set_fps_mode(bool fps_mode) -> void
{
    SignalGenerator::set_fps_mode(fps_mode);
}

auto uiInterface::add_wave_cnt() -> void
{
    SignalGenerator::add_wave_cnt();
}

auto uiInterface::sub_wave_cnt() -> void
{
    SignalGenerator::sub_wave_cnt();
}

auto uiInterface::switch_wave_type() -> void
{
    SignalGenerator::switch_wave_type();
}


auto uiInterface::wave_is_generate() -> void
{
    SignalGenerator::wave_is_generate();
}

auto uiInterface::wave_is_not_generate() -> void
{
    SignalGenerator::wave_is_not_generate();
}

auto uiInterface::clear_screen() -> void
{
    LCD_Color_Clean(80, 40, 400, 240, 0xFFFF);
}

auto uiInterface::add_period() -> void
{
    SignalGenerator::add_period();
}

auto uiInterface::sub_period() -> void
{
    SignalGenerator::sub_period();
}

auto uiInterface::get_period() -> uint32_t
{
    return SignalGenerator::get_period();
}

auto uiInterface::get_ratio() -> uint8_t
{
    return SignalGenerator::get_ratio();
}

auto uiInterface::get_bias() -> int8_t
{
    return SignalGenerator::get_bias();
}

/**
 * @brief 获取模式
 * @return ture为混频，false为单频
 */
auto uiInterface::get_mode() -> bool
{
    return SignalGenerator::get_mode();
}

/**
 * @brief 获取频率
 * @return false为800，true为1K
 */
auto uiInterface::get_freq() -> bool
{
    return SignalGenerator::get_freq();
}

