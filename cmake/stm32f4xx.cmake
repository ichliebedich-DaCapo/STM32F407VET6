# 使用Debug，使用HAL库,指定目标芯片的型号。    -DUSE_FULL_LL_DRIVER 使用LL库
add_compile_definitions(ARM_MATH_CM4
        ARM_MATH_MATRIX_CHECK
        ARM_MATH_ROUNDING
        DEBUG
        USE_HAL_DRIVER
        STM32F407xx
)

# 设置链接脚本（用于指定内存布局）
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETX_FLASH.ld)
add_link_options(-T ${LINKER_SCRIPT})



