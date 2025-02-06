# Projects帮助文档

## 简介
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 该层包含了共享目录和各个项目目录，共享目录有些公共的组件，如：
> `main.cpp`、`ISR.cpp`等
> 。每个项目目录有着相同的结构，目前根据项目的性质
> “只有ui和逻辑不同”，其下有app和ui两个目录，后续也方便扩展，比如添加一个ai目录
> <br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 每个项目目录除了上述子目录外，还有一个project_config.cmake配置文件，
> 用于配置该项目。比如该项目包含了哪些BSP驱动，是否启用GUI、FreeRTOS等。
 

## 项目目录
### `app目录`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
app目录下是存放应用逻辑的，一般只有一个文件`app.cpp`。当然，可以添加更多文件，
只要实现`app_init()`、`key_handler()`、`background_handler()`
等函数即可，这些函数采用虚定义，不实现也不会报错。
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`app_init()`是用来初始化驱动的，所谓实现其实是调用需要用到的bsp驱动初始化代码，比如：
```c++
void app_init()
{
    adc1_temperature_sensor_init();
    RNG_Init();
    ITM_Init();
    timer6_init(FREQ_84M_to_256x800);// 预期800Hz
    dac_init();
}
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`key_handler()`是按键处理函数，**基于4*4矩阵按键键盘**，封装了相关逻辑。当按键按下时，
会调用一次key_handler()函数，同时可以使用由Key类封装了各种操作按键的接口。如下，
`Key::stateHandler`是一个状态函数，可通过传参KEY_STATE_ONE等，来决定按同一个按键有几种状态。
如果传参为KEY_STATE_THREE，那么表达这个按键最多有3种状态，每按下一次，`Key::stateHandler`就会
返回0,1,2三个值，可用if或者switch语句来处理相应状态。
```c++
void key_handler()
{

    switch (Key::getCode())
    {
        case keyk0:// 播放
            if (Key::stateHandler(KEY_STATE_TWO))
            {
                gui->main.imgbtn_play.press();

            }
            else
            {
                gui->main.imgbtn_play.release();
            }
            break;
            
        default:
        break;
        
    }
}
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
`background_handler()`即后台处理函数，在裸机状态下（没有上RTOS），其实该函数是放在while死循环里的。
在RTOS下，该函数会放在一个单独的线程里。使用时完全可以把它当做while死循环，没有本质区别，除了函数入栈的开销外。
由于是跨文件，编译器并不会对齐进行内联操作，可以开启LTO，进行跨文件优化。不过该操作会让编译变慢许多，
所以平时不建议用，只有当开发好了之后再尝试进行LTO优化。


### `ui目录`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
ui目录下是存放ui界面的，使用的图形库是LVGL，但为了方便开发，对其进行了一些封装。
现在开发是通过定义对象、调用各种对象的函数来实现对应功能。


## project_config.cmake
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 即项目的配置文件，在里面可以配置该项目包含了哪些BSP驱动、启用了哪些功能。

### `BSP包含`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
通过函数`set_bsp_components`（必选）来表示项目包含了哪些BSP驱动，从而选择性编译。
```cmake
set_bsp_components(
        adc
        rng
        key
        debug
)
```

### `配置项`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
在cmake目录下的`common_functions.cmake`中定义了各种函数，可以针对性地选择开启哪些功能。
比如下面函数是禁用GUI和开启FreeRTOS，启用下面功能，会停止包含GUI相关资源文件并且自动注释GUI_ENABLE这个宏；
会自动包含FreeRTOS相关资源文件，并自动定义FreeRTOS宏。
```cmake
options_disable_GUI()
options_enable_FreeRTOS()
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
这些宏在`common_functions.cmake`中可以看到，如下所示。如果要新建配置项，
需要在前面定义新的缓存变量，然后赋予其默认值（ON/OFF），并附上描述。
接着在下面定义相关宏，宏的名称即是它的功能。
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
通过刚才定义的变量，可以在其他cmake或主CMakeLists中设置对应的判断条件，比如选择性包含资源文件。
```cmake
# -----------------------------项目宏----------------------------------
# 在项目下的bsp_components.cmake中使用
# 不使用option，是因为这些变量不需要缓存
# 定义的这些变量最后会在在配置头文件里定义相关宏
set(GUI_ENABLE ON CACHE INTERNAL "GUI option")
set(FREERTOS_ENABLE OFF CACHE INTERNAL "FreeRTOS option")
set(FREERTOS_DEBUG OFF CACHE INTERNAL "FreeRTOS debug option")
set(AI_ENABLE OFF CACHE INTERNAL "AI option")
set(STATIC_LIB_LD OFF CACHE INTERNAL "Independent static library link option")


# 关闭GUI
macro(options_disable_GUI)
    set(GUI_ENABLE OFF CACHE INTERNAL "GUI option")
endmacro()

# 开启FreeRTOS
macro(options_enable_FreeRTOS)
    set(FREERTOS_ENABLE ON CACHE INTERNAL "FreeRTOS option")
endmacro()

# 开启FreeRTOS调试
macro(options_enable_FreeRTOS_debug)
    set(FREERTOS_DEBUG ON CACHE INTERNAL "FreeRTOS debug option")
endmacro()

# ------------不在project_config.h.in中---------------
# 开启AI
macro(options_enable_AI)
    set(AI_ENABLE ON)
endmacro()

# 开启静态库链接
macro(options_enable_static_lib_ld)
    set(STATIC_LIB_LD ON CACHE INTERNAL "Independent static library link option")
endmacro()
```
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
接下来是对应的宏，C语言中的宏。在公共目录shared中的`project_config.h.in`文件，内容如下。
可以看到，`#cmakedefine`旁的宏其实就是前面命名的变量，如果变量为ON，那么这个宏在cmake过程中就会自动被定义。
其后的头文件包含，其实就是通过宏来实现各种判断逻辑。
```c
#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H
#include<common_config.h>


// ---------配置项-----------
// 与启用中间库和一些工程的编译有关
#cmakedefine GUI_ENABLE
#cmakedefine FREERTOS_ENABLE
#cmakedefine FREERTOS_DEBUG


// ----------与头文件包含有关----------
#ifdef __cplusplus
// C++头文件
#ifdef GUI_ENABLE
#include "GUI.hpp"
#endif

#else
// C头文件
#ifdef FreeRTOS_ENABLE
#include "cmsis_os2.h"
#endif

#endif // __cplusplus

#endif // PROJECT_CONFIG_H

```


## shared目录
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 公共目录下是各个项目目录都有的文件，比如main.cpp、ISR.cpp以及一些in文件（用于生成头文件）。


### `main.cpp`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
main.cpp的逻辑很简单，通过外部声明几个函数，让项目目录里的app.cpp来实现相关定义。
在main函数中，前面是各种初始化（各种封装），后面是while死循环。
```c++
#include <project_config.h>
#include "baseInit.h"
#include "key.hpp"

extern void app_init();// 应用程序初始化函数,强制定义
extern void background_handler();// 后台处理函数
__attribute__((weak)) void background_handler() {}


#ifdef FreeRTOS_ENABLE
void backgroundTask(void *argument)// 后台线程
{
    for (;;)
    {
        background_handler();
    }
}
#endif


int main()
{
    /*基础初始化*/
    BaseInit(); // 基础驱动初始化

#ifdef FreeRTOS_ENABLE
    osKernelInitialize();// FreeRTOS内核初始化
#endif// FreeRTOS_ENABLE

    Key::init();

#ifdef GUI_ENABLE
    GUI::init<lcd_flush>();
#endif

    app_init();

    /*主事件循环或调度器*/
#ifdef FreeRTOS_ENABLE
    // 创建后台线程
    const osThreadAttr_t backgroundTask_attributes = {
            .name = "backgroundTask",
            .stack_size = 256 * 4,
            .priority = (osPriority_t) osPriorityLow,
    };
    osThreadNew(backgroundTask, nullptr, &backgroundTask_attributes);

    // 创建GUI线程
#ifndef GUI_DISABLE
    const osThreadAttr_t GUITask_attributes = {
            .name = "GUITask",
            .stack_size = 512 * 4,
            .priority = (osPriority_t) osPriorityNormal,
    };
    osThreadNew([](void *)
                {
                    for (;;)
                    {
                        osDelay(5);
                        GUI::handler();
                    }
                }, nullptr, &GUITask_attributes);
#endif
    // 启动调度器
    osKernelStart();
#else
    for (;;)
    {
#ifdef GUI_ENABLE
        GUI::handler();
#endif
        Key::handler();
        background_handler();
    }
#endif
}
```