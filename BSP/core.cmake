set(CORE_DIR ${CMAKE_SOURCE_DIR}/Core)
# 设置Core子目录
set(DRIVERS_DIR ${CORE_DIR}/Drivers)
set(SYSCALL_DIR ${CORE_DIR}/syscall)
set(SYSTEM_DIR ${CORE_DIR}/system)
# 设置system子目录
set(LINKER_DIR ${SYSTEM_DIR}/linker)
set(STARTUP_DIR ${SYSTEM_DIR}/startup)
# 设置Drivers子目录
set(CMSIS_DIR ${DRIVERS_DIR}/CMSIS)
set(HAL_SRC_DIR ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src)

# -----------------------CMSIS头目录-----------------------
set(CMSIS_INC_DIRS
        ${DRIVERS_DIR}/CMSIS/Include
        ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include
)


# -----------------------Drivers库-----------------------
file(GLOB_RECURSE DRIVERS_SRCS "${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src/*.c")
# 定义驱动库的头文件目录
set(DRIVERS_INC_DIRS
        ${CMSIS_INC_DIRS}
        ${DRIVERS_DIR}
        ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc
        ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
)
add_library(libdrivers STATIC ${DRIVERS_SRCS})
target_include_directories(libdrivers PUBLIC ${DRIVERS_INC_DIRS})
# 设置官方驱动库的输出目录
set_target_properties(libdrivers PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# -----------------------系统调用和启动文件-----------------------
file(GLOB_RECURSE SYSCALL_SRCS "${SYSCALL_DIR}/*.c")
file(GLOB_RECURSE STARTUP_SRC "${STARTUP_DIR}/startup_stm32f407vetx.s")


# --------------------------Core层--------------------------
set(CORE_INC_DIRS ${DRIVERS_INC_DIRS})
set(CORE_SRCS ${DRIVERS_SRCS} ${SYSCALL_SRCS} ${STARTUP_SRC})

