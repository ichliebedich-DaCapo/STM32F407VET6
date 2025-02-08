# 设置编译工具集
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(CMAKE_SIZE arm-none-eabi-size)

# 设置编译标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# 添加编译器选项，使得支持Module特性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")

#--------------------------编译选项----------------------------
# -m 选项通常用于控制目标架构、指令集和硬件特性。
# 指定目标处理器为Cortex-M4  生成Thumb指令集的代码  允许Thumb和ARM指令集之间的互操作
add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
# 使用硬件浮点单元（FPU）进行浮点运算  指定浮点单元的类型为 FPv4-SP（单精度浮点单元），并限制寄存器数量为 16 个
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

# 将每个函数放在单独的段中，便于链接器优化   将每个全局变量放在单独的段中，便于链接器优化
# 将每个函数放在独立的节区中:允许链接器删除未使用的函数，减少代码大小;将每个全局变量放在独立的节区中:允许链接器删除未使用的变量，减少数据大小。
# 禁止将未初始化的全局变量放在COMMON节区中:确保未初始化的全局变量被正确分配到.bss 节区;设置编译器错误消息的最大长度为无限制:确保编译器错误消息完整显示
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
# 允许预处理器处理汇编文件
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)


#--------------------------编译优化----------------------------
# 如果不开优化，使用LVGL时有可能因为栈爆而导致数组越界，进而进入硬件错误中断。至少需要开-Og
# 选择优化级别（真是见鬼！在CMake配置文件改了构建选项，结果没反应，干脆手动操作）
#-O0：无优化，适合调试。
#-O1：基本优化，适合快速迭代开发。
#-O2：中级优化，适合大多数生产环境。
#-O3：高级优化，适合对性能要求较高的应用。
#-Ofast：极端优化，适合对性能要求极高且对标准合规性要求不高的应用。
#-Og：调试优化，适合开发和调试阶段。
#-Os:优化生成的代码大小，而不是性能。
#-Oz:极度优化大小,进一步优化生成的代码大小，比 -Os更激进,可能会牺牲一些性能。
#-flto:在链接阶段进行优化,允许进行跨文件的优化，进一步提高性能，但耗时间（项目本来就小我不在乎），并且这个优化容易出问题（这个我很在乎）
#-fprofile-generate（生成配置文件） 和 -fprofile-use（基于配置文件的优化）
add_compile_options(-Ofast -g)
# 必须关闭静态库独立模块编译，才能够使用链接优化
if (LTO_ENABLE)
    #仅使用 add_compile_options(-flto)：
    #   结果：编译器生成的中间表示（IR）包含优化所需的信息，但链接器不会进行优化。
    #   问题：LTO 的效果不完整，性能提升有限。
    #仅使用 add_link_options(-flto)：
    #   结果：链接器尝试进行优化，但编译器生成的对象文件可能不包含足够的信息。
    #   问题：链接器无法进行有效的优化，可能导致编译错误或优化失败。
    # 全局启用LTO并行优化
    if (STATIC_LIB_LD)
        # 处于静态库编译模式下，不支持LTO，因为各种依赖会混在一起，很麻烦（我实在不想搞了）
        message(WARNING "Can not use LTO!")
    else ()
        add_compile_options(-flto=auto -fuse-linker-plugin)
        add_link_options(-flto=auto -fuse-linker-plugin)
    endif ()
endif ()

#-finline-functions：自动内联合适的函数。
#-finline-limit=n：设置内联函数的最大复杂度，默认值为225。你可以根据需要调整这个值。
#-Winline-function-return-type：对返回类型不一致的内联函数发出警告。
add_compile_options(-finline-functions)

# ----------------------调试选项--------------------------
# 开启内联警告，当函数内联失败时，编译器会发出警告。
add_compile_options(-Winline)

#---------------------------链接选项--------------------------
# 链接标志：指在编译和链接过程中，传递给链接器（Linker）的选项或参数，和链接选项是一个东西
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)

# 设置链接选项
#-Wl,--gc-sections：删除未使用的节区。链接器会删除未使用的代码和数据节区，减小二进制文件的大小。
#特别适用于嵌入式系统，可以显著减少代码大小。
#-Wl,--as-needed：按需链接库。链接器只会链接实际使用的库文件，避免不必要的依赖。
#可以减少二进制文件的大小和加载时间。
#-Wl,--relax：优化链接过程。链接器会尝试优化链接过程，减少重定位条目，提高加载性能。
#特别适用于某些架构，如 ARM。
#-Wl,--strip-all：删除所有符号表和调试信息。可以显著减小二进制文件的大小，但会影响调试体验。
#-Wl,--strip-debug：仅删除调试信息。
add_link_options(-Wl,-gc-sections,--relax,--as-needed,--print-memory-usage,-Map=${BIN_DIR}/${PROJECT_NAME}.map)
