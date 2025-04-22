set(CORE_DIR ${CMAKE_SOURCE_DIR}/Core)
# 设置Core子目录
set(DRIVERS_DIR ${CORE_DIR}/Drivers)
# 设置Drivers子目录
set(CMSIS_DIR ${DRIVERS_DIR}/CMSIS)
set(HAL_SRC_DIR ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Src)

# -----------------------CMSIS头目录-----------------------
set(CMSIS_INC_DIRS
        ${DRIVERS_DIR}/CMSIS/Include
        ${DRIVERS_DIR}/CMSIS/Device/ST/STM32F4xx/Include
)

# -----------------------Drivers库-----------------------
# 定义驱动库的头文件目录
set(DRIVERS_INC_DIRS
        ${CMSIS_INC_DIRS}
        ${DRIVERS_DIR}
        ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc
        ${DRIVERS_DIR}/STM32F4xx_HAL_Driver/Inc/Legacy
)




