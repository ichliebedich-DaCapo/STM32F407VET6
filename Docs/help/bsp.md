# BSP帮助文档

## 简介
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
BSP是为了适配开发板上的驱动，相同SoC（比如stm32f407），其内的外设和寄存器都是相同的，
由HAL统一提供各种接口。但不同开发板上焊接的是不同外设，，比如A0这个引脚有的开发板接LED有的接Beep，
那么就需要进行适配，即开发BSP（板级支持包）。<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
本工程中，BSP的开发集中在BSP目录下。BSP目录下有inc和src目录，分别存放bsp驱动的头文件和源文件。
此外，还有CMakeLists、cmake和.in配置文件。为了不同项目的编译，引入了注册依赖机制。

## 注册依赖机制
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 正常情况下，BSP/src目录下有多少源文件，编译的时候就会编译多少。虽不会让编译的二进制文件膨胀，
> 但编译的时间会变长，编译的输出信息也会由于包含各种无关文件而变长，且出现编译文件相关错误时不容易定位。<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 为解决这个问题，在BSP目录下创建了一个CMakeLists文件和cmake配置文件，用于单独处理BSP目录下的源文件包含。

### `bsp_drivers.cmake`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
定义了注册表的格式，如下所示，每一行字符串都是表示一个驱动，格式为：`<BSP驱动名>:<依赖的驱动>`。
驱动与依赖使用冒号隔开，依赖的驱动用逗号隔开。依赖可以时HAL或者BSP驱动。
这些名称，比如`hal_adc`、`hal_adc_ex`、`hal_dac`等，并非时凭空定义的，它们需要满足一些规范。
需注意，每个名称都指代了一个驱动文件。
```cmake
set(BSP_DRIVERS
        "default:hal,hal_cortex,hal_gpio,baseinit"
        # BSP驱动
        "adc:timer,hal_adc,hal_adc_ex"
        "baseinit:rcc,lcd,hal_tim,key_exit"
        "cpu_runtime:"
        "dac:hal_dac"
)
```


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
每当引入了一个文件，就可以注册一个驱动名称，比如hal_adc代表的其实是stm32f4xx_hal_adc.c文件，
它有下面的变量定义hal_adc_SRCS决定,该变量指明了hal_adc所代表的源文件路径。同时为了便于管理，
BSP文件和HAL的文件映射分别集中定义，且HAL的驱动均有前缀hal_以示区分。


```cmake
# 依赖项文件映射
include(core.cmake)
set(hal_SRC ${HAL_SRC_DIR}/stm32f4xx_hal.c)
# ADC
set(hal_adc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_adc.c)
```

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
也就是说，如果你开发一个bsp文件，名为`beep.c`，那么应使用`beep`来表示beep驱动，同时在指定位置，
按照`<BSP驱动名_SRC>`的格式指定文件路径。那么定义之后就可以在驱动注册表里使用`beep`
```cmake
set(BSP_DRIVERS
        "default:hal,hal_cortex,hal_gpio,baseinit"
        # BSP驱动
        "adc:timer,hal_adc,hal_adc_ex"
        "baseinit:rcc,lcd,hal_tim,key_exit"
        "cpu_runtime:"
        "dac:hal_dac"
        # 新定义的beep驱动，没有任何依赖
        "beep:"
)

# 在指定位置处指定beep驱动的文件路径
set(beep_SRC ${BSP_SRC_DIR}/beep.c)
```



&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
接下来根据beep驱动的开发来指定其依赖项，以beep为例，一般会使用到HAL库中的gpio驱动，
那么其依赖应为hal_gpio，那么应写成`"beep:hal_gpio"`。不过由于hal_gpio经常使用，
这里为了简化书写，定义了一个`default`，它本身不指定任何源文件（没有相应的`default_SRC`变量），
它只是包含了一些常用的驱动，比如`hal_gpio`、`hal_tim`、`hal_cortex`等。


### `core.cmake`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
定义了Core目录下的相关子目录及文件，包含了HAL、CMSIS头文件目录、系统调用源文件和启动文件，
仅仅是为了给bsp_drivers.cmake的变量命名提供方便。



### `bsp_config.h.in`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
bsp配置的模板文件，用于生成`bsp_config.h`，生成的目录在cmake构建目录下的build/inc。
由于每次开发BSP驱动，都需要包含`stm32f4xx_hal.h`，为了方便，配置文件里引入了该头文件，
以便开发bsp文件时只需要引入`bsp_config.h`即可。
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
同时出于选择性编译的扩展性，里面引入了一堆BSP宏，它们均由cmake自动生成，只要使用到了哪个BSP，
对应的宏就会在cmake构建的时候自动定义生成。借由这些宏，可以选择性完成特定的功能，比如在ISR文件里，
定义了许多中断，但有时候一些中断会冲突，那么就可以选择性地关闭。
```c
#include<common_config.h>
#include<stm32f4xx_hal.h>

// 自动生成的驱动开关
#cmakedefine BSP_USE_ADC
#cmakedefine BSP_USE_BASEINIT
#cmakedefine BSP_USE_CPU_RUNTIME
```


### `stm32f4xx_hal_conf.h.in`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
HAL库的配置文件模板，用于生成`stm32f4xx_hal_conf.h`，生成的目录在cmake构建目录下的build/inc。
由原`stm32f4xx_hal_conf.h`改造而成，内部只是把宏定义替换为了cmake的`cmakedefine`，
从而实现了自动定义宏，避免了手动修改宏定义，既麻烦又容易忘。


### `CMakeLists.txt`
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
用于解析驱动依赖，定义了相关函数。可以根据项目目录下的`bsp_component.cmake`中包含的bsp驱动，
以及BSP目录下的驱动依赖注册表`bsp_drivers.cmake`来解析该项目需要哪些HAL文件和BSP文件，
进而实现精准控制编译过程。


## 开发流程
> ### 创建驱动文件
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 先根据需要使用的驱动来决定驱动文件如何命名，比如开发`lcd`驱动。
> 然后在工程根目录下的CMakeLists中的寻一空白处（比如项目选择的下方）
> 调用函数`create_bsp()`(输入前几个字母即可看到代码补全提示)，
> 再向里面输入参数`lcd`，即`create_bsp(lcd)`。接着CMakeLists界面右上方会出现刷新的提示，
> 重新cmake一下，BSP的对应目录下即可生成lcd.h/c文件。<br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 不过由于CLion的项目目录有延迟，可以右键项目目录，选择`从磁盘上重新加载`，即可看到创建的驱动文件
> ### 开发驱动
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 开发驱动需要遵循一定的规范，比如接口的命名**尽量**使用同一种命名方法命名。
> 驱动的头文件尽量不要放各种宏，这会造成严重的污染，很不好。比如某个文件引用了这个头文件，
> 那么该头文件的所有宏都会暴露到该文件中，
> 有些不规范的宏名称可能让被引用文件里的一些数据、变量或函数等被隐式更改。
> 此外,头文件充斥大量的宏，只会让CLion的代码补全提示看起来愈发困难。
> <br>
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 实际开发过程中，宏完全没必要大量放在头文件中，因为许多宏其实只再该驱动的源文件中引用。因此，
> 头文件中只需要留下清晰明了的接口声明和必要的宏即可。
> 比如下面的lcd.h/c文件，lcd.h里只有若干接口声明，而lcd.c里则包含了很多宏
> ```c
> // lcd.h
> #ifndef FURINA_LCD_H
> #define FURINA_LCD_H
> #include <bsp_config.h>
>
> #ifdef __cplusplus
> extern "C" {
> #endif
> 
> void lcd_init(void);// lcd初始化
> void lcd_clear(uint16_t color);// 清屏
> void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);// 画点
> 
> #ifdef __cplusplus
> }
> #endif
>
> #endif //FURINA_LCD_H
> ```
> ```c
> // lcd.c
> #include <string.h>
> #include "lcd.h"
> #include "spi.h"
> #include "fsmc.h"
> #include "stm32f4xx_hal.h"
>
> extern DMA_HandleTypeDef hdma_memtomem_dma2_stream6;
>
>
> /*LCD地址*/
> #define TFT_CMD (*((volatile unsigned short *) 0x60060000)) // TFT命令寄存器片选地址
> #define TFT_DATA (*((volatile unsigned short *) 0x60060002))// TFT数据寄存器片选地址
> #define TFT_RST (*((volatile unsigned short *) 0x60060004)) // TFT复位寄存器地址
> #define TFTLED (*((volatile unsigned short *) 0x60060008))  // TFT背光寄存器地址
> #define TFT_DATA_ADDR 0x60060002U
> #define LCD_WRITE_CMD(Command) (TFT_CMD = Command)
> #define LCD_WRITE_DATA(Data) (TFT_DATA = Data)
> // …………
> 
> /********************************************************************
> * 名称 : LCD_Init9481
> * 功能 : 液晶初始化
> * 输入 : 无
> * 输出 : 无
>   ***********************************************************************/
>   void lcd_init(void)
>   {
>     // …………
>    }
> 
>   // …………
> }
> ```
> ### 单元测试
> `前面的区域，以后再来吧`