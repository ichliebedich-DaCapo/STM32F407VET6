&nbsp;&nbsp;This is a project based on stm32f407vet6. <br>
___
## 环境
`IDE`: CLion
<br>
`辅助工具`： GUI Guider &nbsp;&nbsp; STM32CubeMX
<br>
`开发平台`： Windows 11 
<br>
`工具集/链`：
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[MinGW](https://github.com/niXman/mingw-builds-binaries/releases)
&nbsp;x86_64-14.2.0-release-posix-seh-ucrt-rt_v12-rev0
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
[arm-toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
&nbsp;arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi
<br>
`分支说明`:main分支是已经测试稳定通过的，ZQ和DV分支是两条测试用的分支  &nbsp;&nbsp;&nbsp;&nbsp; JY快更新你的DV
<br>
`其他`：工程里嵌套了一个来自GUI Guider的lvgl模拟器，目录为GUI/lvgl_simulator，使用的是GUI Guider的工具链，编译出的是32位程序。
<!-- 鉴于原工具链有些慢，后面有空换一下工具链。-->


<!-- 
- [x] xxx:显示任务完成图标,方括号里放空格就是未完成
-->
___
## 简介
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 此为stm32f407vet6的一个C/C++混编工程用于练手，会不定期更新一些简单基础实验项目可供参考。
整个工程使用CMakeLists组织,可分为下面几个部分


- `Application`：即应用级，是与硬件无关的部分，只管应用的实现。包含各种实验项目，如语音存储与回放、双音频信号发生器等。
通过CMakelists顶部的变量来控制使用哪个项目。主体由C++实现同时其头文件不提供任何接口，仅有宏声明  <br><br>
- `BSP`：即板级支持包(本想命名为Drivers但重名了),基于HAL编写的裸机驱动，同时提供对应C接口供使用。后文中所指的驱动级即此处 <br><br>
- `Core`：即SoC内核文件，存放的是系统调用、启动文件等与SoC相关的文件，不用做任何改变。<br><br>
- `Drivers`：由CubeMX生成的目录，存放的是ST官方HAL，仅在stm32fxx_hal.h里添加了一个Error_Handler内联函数。为避免误操作，整个文件夹设置成了只读状态  <br><br>
- `GUI`：即用户交互的，使用的是LVGL，包含了界面设计和界面逻辑以及资源文件三部分 。同时里面内嵌了一个LVGL模拟器，在lv-simulator目录。 <br><br>
- `Library`：即库，包含了第三方库如FreeRTOS、LVGL、FATS等，同时添加了DATA目录，用于存放一些数据，如正弦波波形数据等  <br><br>
- `Module`：即模块级，主要用于编写各种算法等，尽量与驱动无关，主要由C++实现  <br><br>


<br>

####  如何使用CMakelists控制不同实验项目的编译
<p style="text-indent: 2em;">
在CMakelists的头部有如下变量定义，set是CMakelists的“函数”，用于设置变量。用法为set(变量名 值)，变量名可以是自定义的，
也可以是CMakeLists里定义好的。这里可以看到，设置了一个变量“APP_DIR”，其值为目录“Application/signalGenerator”，
这就表明要使用“signalGenerator”这个实验项目，即信号发生器（双音频信号发生器）。<br>
</p>

<p style="text-indent: 2em;">
如果你要使用其他项目，比如语音存储与回放，那么只需要将set(APP_DIR Application/voiceStorage)即可。里面嵌入的lvgl模拟器，也是如此，
在CMakelists里通过相同的方式来设置。
</p>

```cmake
# --------------------------------------选择你的项目--------------------------------
# 如果你是在CLion里，那么可以直接在斜杆后面输入首字母，会自动弹出相关项目选项
set(APP_DIR Application/signalGenerator)
# --------------------------------------选择你的项目------------------------------
```

<p style="text-indent: 2em;">
那么它是如何实现的呢？在CMakelists往下翻，可以看到这一句，通过前面设置的变量${APP_DIR}来选择性编译其目录下的
app.cpp文件。你没有看错，为了方便管理，不同实验项目都是由app.hpp/cpp组成，只有目录名称不同，在Application目录里
你可以清晰地看到这一点。
</p>

```cmake
# ------Application库------
file(GLOB_RECURSE APPLICATION_SRC "${APP_DIR}/app.cpp" "Application/Base/*.cpp")
add_library(libapp STATIC ${APPLICATION_SRC})
target_include_directories(libapp PUBLIC Application/Base)
# 需要链接 libdsp libdata libgui libbsp libmodule等，根据需要选
target_link_libraries(libapp PUBLIC libdsp libdata libgui libbsp libmodule)
```


___

## 项目
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 本工程由于融合了多个简单实验项目，采用选择性编译的方法来避免冲突，原理在上文中提到。
在Application目录下的Conf目录里，有一个名为App_Conf.h，该文件的主要作用是传递应用级头文件的宏，
以此来控制不同模块的编译。

### _说明_：
- 应用级文件一般由app.hpp/cpp组成，每个实验项目都在Application有一个独立的目录，比如signalGenerator、voiceStorage等。
其中头文件app.hpp里没有任何接口变量什么的，只有一堆宏声明，用于控制不同模块的编译。
<br><br>
-  正如前文所述，app.hpp里有一堆宏来控制不同模块的编译，在不同模块里会通过引用App_Conf.h间接引用app.hpp，进而
通过预编译指令 **#ifdef** 来控制改编译单元是否编译。
<br><br>
- 补充了是否启用FreeRTOS的宏，如果不需要使用RTOS，可在应用级文件定义一个宏APP_NO_RTOS,同时在CMakeLists里把FreeRTOS资源文件注释掉
<br><br>

### _流程_：
- **开始**： 可以先从Application/Base里的main.cpp开始看，这里对初始化函数和处理函数都进行了封装。
<br><br>
- **架构**：整个工程如前面所说分成三个部分，编写代码的主要地方也在三级目录下的.c/h文件里，通过文件名可以识别出。
先从顶层Application开始看，里面主要实现了两个函数app_init和key_handler，见名知义，
同时main函数不再是我们编写代码的主场地。
<br><br>
- **宏**：在App_Conf.h里可以看到一些奇怪的宏，这里被注释的宏只是为了记忆，方便编写app.hpp/cpp时知道定义哪个宏
<br><br>
- **模块层的静态类**：模块层基本上都是用静态类来实现的，这是为了更好地封装同时不至于二进制文件膨胀。其中，
  变量和一些简单函数基本上都是私有成员，这么做主要是不让其他函数随意访问这里的变量，只能通过给定的接口实现（代码逻辑更清晰），
  不然与以前随处定义的全局变量一样容易产生混淆。
<br><br>

___

## 规范：
>为了让项目的代码规范一点，我们约定了一些规则。


<p style="text-indent: 2em;">
框架主体使用的是FreeRTOS，除了FreeRTOS本身自由、简单易上手的特点外，
另一点是CLion集成了FreeRTOS的图形化调试，非常直观。对于一些简单实验项目，采用的是裸机。同时，把整体开发流程主要分为了驱动、模块、应用三级，
同时这些驱动、模块、应用相关文件由一个xxx_Conf.h来控制，决定了哪些文件可以被编译。<br><br>
</p>

### _注意事项_：<br><br>
- `接口兼容性`：BSP统一使用C接口。而模块级与应用级则使用C++接口，方便开发，比如可以使用重载、引用、自动类型推导、lambda表达式、返回类型后置、结构化绑定等.
             只不过看着驱动开发里的C代码，总想用C++提高代码复用，唉兼容性…… <br><br>
- `文件命名规范`：BSP和模块级尽量小写为主，不同属性尽量用下划线隔开，方便调用接口；
应用级尽量采用驼峰命名法。之所以是尽量，是因为C/C++的命名本来就已经很混乱了哈哈哈哈哈<br><br>
- `函数`：为了提高驱动性能，实现简单功能的函数尽量内敛，其内局部变量尽量使用字长单位<br><br>
- `变量`：尽量不使用全局变量,尽量使用函数式编程，减少副作用<br><br>
- `调试`：统一开-O2级别优化，遇到问题再开-Og。在CLion里面体现为使用Release选项。同时要注意volatile变量的使用<br><br>
- `语法使用规范`：由于单片机资源有限，不能使用泛型编程如模版。类可以使用，尽量使用静态类以实现零成本抽象，
避免实例化对象，尤其是new的使用。<br><br>


### _C++特性_:<br><br>
>适合单片机开发的一些C++特性，在尽可能减少开销的情况下尽量提升开发效率。

<!--C++中适合单片机开发的新特性，C++11以后每三年一个版本，且按照Major-Minor-Minor进行迭代
C++20与C++23基本都是对标准库、泛型编程等进行了优化，很难用到单片机上。尤其是那该死的模块，听说MSVC支持的比较好，
cmake也还行，但是试来试去就是不行，淦。-->

```c++
 * // -------------------------------------C++98-------------------------------------
 *
 * 【命名空间】：命名空间是C++中唯一一种作用域机制，它允许将变量、函数、类等定义放在一个独立的命名空间中，避免命名冲突。
 *          在单片机开发中，我感觉它与静态类没有太大区别（不讨论封装、继承的情况下），但命名空间只是逻辑上的隔离，并不会带来格外的开销。
 *          话说，感觉有用但与静态类比起来有种鸡肋的感觉，真没办法，毕竟用不了泛型编程、继承、多态什么什么的。
 *             11-8 现在我觉得还是有用的，比如匿名函数空间，专治各种函数变量暴露狂，简直就是闭包
 *
 *
 * // -------------------------------------C++11-------------------------------------
 *
 * 【constexpr】：允许在编译时计算常量表达式，减少了运行时的开销。C++20中进一步提升了性能
 *      constexpr int square(int x) {return x * x;}
 *      constexpr int result = square(5);  // result 在编译时计算
 *
 * 【范围-based for 循环】：允许在循环中迭代容器中的元素，简化代码。即简化了索引操作，避免潜在管理错误
 *      for (auto& elem : array) {elem *= 2;}
 *
 * 【自动类型推导】：允许在函数参数中省略类型声明，让编译器自动推导类型。这个没什么好说的。但是要注意使用场合，像【范围-based for 循环】、【lambda表达式】
 *                等一些类型复杂或者显而易见的场景，这样做可以让代码简洁。对于类型不明显或容易混淆的场景，比如基本类型变量、函数参数、等，还是建议显式声明类型，避免使用泛滥
 *      auto add(int a, int b) {return a + b; }// 可以推导出返回值类型为 int，与返回类型后置差不多
 *
 *
 *  // -------------------------------------C++14-------------------------------------
 *
 *  【泛型 lambda 表达式】：允许在函数内部定义lambda表达式，使得代码更加简洁，不再又是声明又是定义的。
 *          auto print = [](auto x) {std::cout << x << std::endl;};
 *
 *  【返回类型后置】：这他宝贝的才是我喜欢的函数定义语法，跟Koltin里一样，可以使复杂的函数签名更易读。只不过前面还是要加上auto真是麻烦
 *          auto square(int x) -> int { return x * x;}
 *
 *
 * // -------------------------------------C++17-------------------------------------
 *
 * 【if 和 switch 初始化器 】：允许在if语句和switch语句中初始化变量。避免局部变量到处都是，全他宝贝的污染
 *      比如：if (auto value = read_sensor(); value > threshold){}
 *
 * 【[[nodiscard]]】：用来标记函数返回值不应该被忽略，防止潜在错误
 *      比如：[[nodiscard]] bool check_error() {}
 *
 * 【结构化绑定】：跟Koltin里的解构差不多，就是当函数返回值类型为结构体之类的，可以让函数返回多个值
 *      std::pair<int, int> coords = get_coordinates();
 *      auto [x, y] = get_coordinates();
 *
 * 【内联变量】：可以在头文件中直接定义静态成员变量，而不会导致重复定义的问题。这个嘛不一定用得上
  ```
___

## CLion开发体验提升
- **文件模板**：这个模板使用的是Apache Velocity,可以自行上网或者跟AI交流，让它给你生成符合自己需求的模板
<br><br>
C文件模板
```
#parse("C File Header.h")
#if (${HEADER_FILENAME})
#[[#include]]# "${HEADER_FILENAME}"
#end
\#if 1
// 头文件

// 函数


#endif
```

C头文件模板<br>
&nbsp;&nbsp;&nbsp;&nbsp;根据个人去配置，这里面可以检测是否是hpp文件
```
#parse("C File Header.h")
#[[#ifndef]]# ${INCLUDE_GUARD}
#[[#define]]# ${INCLUDE_GUARD}
## 设置一个变量名用来调用endsWith方法
#set($filename = ${FILE_NAME})
\#include "Module_Conf.h"
/* 预编译命令 */
\#if 1
// 头文件

#if ($filename.endsWith(".h"))
#ifdef __cplusplus
extern "C" {
#endif
#end
// 宏定义


// 接口

#if ($filename.endsWith(".h"))
#ifdef __cplusplus
}
#endif
#end
/* 预编译命令 */
#endif
#[[#endif]]# //${INCLUDE_GUARD}
```


___
<!-- 相戏耳 -->
**人员**：WJY、WZX、XZQ +HNT &nbsp;&nbsp;&nbsp; 软：JY、ZQ &nbsp;&nbsp; 硬：ZX <br>
<!-- ZQ发起&开发&维护  JY开发  ZX硬件支持-->
