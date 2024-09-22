   This is a project based on stm32f407vet6. <br>


## 简介
   此为stm32f407vet6的一个C/C++混编工程，包含了一些实验项目，使用了FreeRTOS、LVGL
此工程分为Application、Module、BSP、Library、GUI、Drivers这几个部分

**Application**：即应用级，是与硬件无关的部分，只管应用的实现。包含各种实验项目，如语音存储与回放、双音频信号发生器等。
通过其Base下的JYZQ_Conf.h来控制使用哪个项目，主要由C++实现  <br><br>
**Module**：即模块级，主要用于编写各种算法，与驱动无关，主要由C++实现  <br><br>
**BSP**：板级软件包，用于编写驱动，同时提供对应接口供使用，主要由C++实现，个别使用C  <br><br>
**Library**：库，包含了第三方库如FreeRTOS、LVGL、FATS等，同时我把DATA目录也加了进去，DATA里面主要是存放一些数据，如正弦波等波形数据  <br><br>
**GUI**：设计用户界面的，使用的是LVGL，里面包含了界面设计和界面逻辑以及资源文件三部分  <br><br>
**Drivers**：里面存放的是ST官方提供的HAL，除了stm32fxx_hal.h里我添加了一个Error_Handler内联函数外，其余地方未修改。同时为了避免误操作，把整个文件夹设置成了只读状态  <br><br>

------
______

### 2024-9-22
  曾有一个完整的stm32f407工程，但是只有本地仓库，没有上传。结果在一次重装过程中手误，把电脑上所有数据都清除了
  现在只好重新来一遍了，现在手上还没有板子，先写一个大致的框架，之后再慢慢补充
