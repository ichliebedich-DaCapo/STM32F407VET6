&nbsp;&nbsp;This is a project based on stm32f407vet6. <br>
___
## 环境
`IDE`: CLion
<br>
`辅助工具`： GUI Guider STM32CubeMX
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
`分支说明`:main分支是已经测试通过，ZQ和DV分支是两条测试用的分支
<br>


<!-- 
- [x] xxx:显示任务完成图标,方括号里放空格就是未完成
-->
___
## 简介
> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
> 此为stm32f407vet6的一个C/C++混编工程用于练手，会不定期更新一些简单基础实验项目可供参考。
整个工程使用CMakeLists组织,可分为下面几个部分


- `Application`：即应用级，是与硬件无关的部分，只管应用的实现。包含各种实验项目，如语音存储与回放、双音频信号发生器等。
通过其Conf下的Module_Conf.h来控制使用哪个项目，由C++实现。同时其头文件不提供任何接口，仅有宏声明  <br><br>
- `Module`：即模块级，主要用于编写各种算法等，与驱动无关，主要由C++实现  <br><br>
- `BSP`：板级支持包，本想建立Drivers目录但与CubeMX生成的目录重名了。基于HAL编写的裸机驱动，同时提供对应C接口供使用。后文中所指的驱动级即此处 <br><br>
- `Library`：库，包含了第三方库如FreeRTOS、LVGL、FATS等，同时添加了DATA目录，用于存放一些数据，如正弦波波形数据等  <br><br>
- `GUI`：设计用户界面的，使用的是LVGL，包含了界面设计和界面逻辑以及资源文件三部分 。同时里面内嵌了一个LVGL模拟器，在lv-simulator目录。 <br><br>
- `Drivers`：里面存放的是ST官方提供的HAL，除了stm32fxx_hal.h里我添加了一个Error_Handler内联函数外，其余地方未修改。同时为了避免误操作，把整个文件夹设置成了只读状态  <br><br>

___

## 项目

### _说明_：
- 本工程由于融合了多个简单实验项目，为避免冲突，决定使用预编译来控制。相关控制宏放在了Conf目录下。<br><br>
其下的App_Conf.h里决定了哪个项目要编译，Module_Conf.h包含了项目头文件，自动控制模块驱动级文件的编译。<br><br>
- 为了在main.cpp里写下大量的预编译命令，使用了不少__weak修饰的函数，
开发每个“应用级”，需要自己实现key_handler()和app_init()函数。<br><br>
-  编写实现代码一般在 Application/App_xxx.c/h、Module/xxx.c/h、BSP/xxx.c/h里。理想情况下：
BSP提供接口供Module，Module提供接口供Application，不涉及驱动的初始化，最后再由Application统一调用并初始化给模块驱动<br><br>
- 补充了是否启用FreeRTOS的宏，如果不需要使用RTOS，可在应用级文件定义一个宏APP_NO_RTOS,同时在CMakeLists里把FreeRTOS资源文件注释掉
<br><br>

### _流程_：
- **开始**： 先从App_Conf.h开始看，内部有控制应用级文件编译的宏，Module_Conf.h里则有控制不同驱动模块的宏。<br><br>
- **架构**：整个工程如前面所说分成三个部分，编写代码的主要地方也在三级目录下的.c/h文件里，通过文件名可以识别出。
  先从顶层Application开始看，里面主要实现了两个函数app_init和key_handler，见名知义，同时main函数不再是我们
  编写代码的主场地。<br><br>
- **宏**：在Module_Conf.h文件中使用各种宏来控制各级文件的编译，但没有随应用自动决定哪些驱动、模块级文件要编译。
  主要是这样做了之后，预编译指令会到处飘，很乱。<br><br>
- **模块层的静态类**：模块层基本上都是用静态类来实现的，这是为了更好地封装同时不至于二进制文件膨胀。其中，
  变量和一些简单函数基本上都是私有成员，这么做主要是不让其他函数随意访问这里的变量，只能通过给定的接口实现（代码逻辑更清晰），
  不然与以前随处定义的全局变量一样容易产生混淆。<br><br>

___

## 规范：
&nbsp;&nbsp;&nbsp;&nbsp;为了让项目的代码规范一点，我们约定了一些规则。<br><br>
&nbsp;&nbsp;&nbsp;&nbsp;框架主体使用的是**FreeRTOS**，除了FreeRTOS本身自由、简单易上手的特点外，
另一点是CLion集成了FreeRTOS的图形化调试，非常直观。 同时，把整体开发流程主要分为了驱动、模块、应用三级，
同时这些驱动、模块、应用相关文件由一个xxx_Conf.h来控制，决定了哪些文件可以被编译。<br><br>


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
&nbsp;&nbsp;&nbsp;&nbsp;适合单片机开发的一些C++特性，在尽可能减少开销的情况下尽量提升开发效率。

<!--C++中适合单片机开发的新特性，C++11以后每三年一个版本，且按照Major-Minor-Minor进行迭代
C++20与C++23基本都是对标准库、泛型编程等进行了优化，很难用到单片机上。尤其是那该死的模块，听说MSVC支持的比较好，
cmake也还行，但是试来试去就是不行，淦。-->

```
 * // -------------------------------------C++98-------------------------------------
 *
 * 【命名空间】：命名空间是C++中唯一一种作用域机制，它允许将变量、函数、类等定义放在一个独立的命名空间中，避免命名冲突。
 *          在单片机开发中，我感觉它与静态类没有太大区别（不讨论封装、继承的情况下），但命名空间只是逻辑上的隔离，并不会带来格外的开销。
 *          话说，感觉有用但与静态类比起来有种鸡肋的感觉，真没办法，毕竟用不了泛型编程、继承、多态什么什么的。
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
<!-- 闹着玩的 -->
**人员**：WJY、WZX、XZQ +HNT &nbsp;&nbsp;&nbsp; 软：JY、ZQ &nbsp;&nbsp; 硬：ZX +NT <br>
<!-- 一个磕磕碰碰的团队 -->
<!-- ZQ开发&维护  JY开发  ZX硬件支持-->
