set(CORE_DIR ${CMAKE_SOURCE_DIR}/Core)
# 设置Core子目录
set(DRIVERS_DIR ${CORE_DIR}/Drivers)
set(SYSCALL_DIR ${CORE_DIR}/syscall)
set(SYSTEM_DIR ${CORE_DIR}/system)
# 设置system子目录
set(STARTUP_DIR ${SYSTEM_DIR}/startup)

# -----------------------系统调用和启动文件-----------------------
file(GLOB_RECURSE SYSCALL_SRCS "${SYSCALL_DIR}/*.c")
file(GLOB_RECURSE STARTUP_SRC "${STARTUP_DIR}/startup_stm32f407vetx.s")


# --------------------------Core层--------------------------
set(CORE_SRCS ${SYSCALL_SRCS} ${STARTUP_SRC})

