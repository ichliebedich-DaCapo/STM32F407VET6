# Middleware层存在的目的是为了提供给Adapter层作适配

# 设置中间件子目录
set(DATA_DIR ${MIDDLEWARE_DIR}/Data)
set(ARM_MATH_DIR ${MIDDLEWARE_DIR}/ARM_MATH)
set(ST_DIR ${MIDDLEWARE_DIR}/ST)
set(THIRD_PARTY_DIR ${MIDDLEWARE_DIR}/Third_Party)
# 设置ARM_MATH子目录
set(DSP_DIR ${ARM_MATH_DIR}/DSP)
# 设置ST子目录
set(AI_DIR ${ST_DIR}/AI)
# 设置第三方库子目录
set(FREERTOS_DIR ${THIRD_PARTY_DIR}/FreeRTOS)
set(LVGL_DIR ${THIRD_PARTY_DIR}/LVGL)


#---------------------Data库-----------------------
file(GLOB_RECURSE DATA_SRCS "${DATA_DIR}/*.c")
set(DATA_INC_DIRS ${DATA_DIR})
add_library(libdata STATIC ${DATA_SRCS})
target_include_directories(libdata INTERFACE ${DATA_INC_DIRS})
# 设置静态库的输出目录
set_target_properties(libdata PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


#---------------------第三方库-----------------------
# -------lvgl库-------
file(GLOB_RECURSE LVGL_SRCS
        # LVGL库
        "${LVGL_DIR}/lvgl/src/*.c"
        "${LVGL_DIR}/lvgl/examples/porting/*.c"
)
# 定义lvgl库的头文件目录
set(LVGL_INC_DIRS
        ${LVGL_DIR}
        ${LVGL_DIR}/lvgl/src
        ${LVGL_DIR}/lvgl/examples/porting
)




# ------FreeRTOS库------
file(GLOB_RECURSE FREERTOS_SRCS
        "${FREERTOS_DIR}/Source/*.c"
        "${FREERTOS_DIR}/Source/CMSIS_RTOS_V2/cmsis_os2.c"
        "${FREERTOS_DIR}/Source/portable/GCC/ARM_CM4F/port.c"
)
# FreeRTOS依赖于CMSIS，而CMSIS部分依赖于HAL，所以引入BSP目录
set(FREERTOS_INC_DIRS
        ${BSP_INC_DIRS}
        ${FREERTOS_DIR}
        ${FREERTOS_DIR}/Source/include
        ${FREERTOS_DIR}/Source/CMSIS_RTOS_V2
        ${FREERTOS_DIR}/Source/portable/GCC/ARM_CM4F
)



# ------DSP库------
file(GLOB_RECURSE DSP_SRCS
        "${DSP_DIR}/Source/TransformFunctions/arm_cfft_f32.c"
        "${DSP_DIR}/Source/TransformFunctions/arm_cfft_init_f32.c"
        "${DSP_DIR}/Source/ComplexMathFunctions/arm_cmplx_mag_f32.c"
)
set(DSP_INC_DIRS
        ${CMSIS_DIR}/Include
        ${DSP_DIR}/Include
        ${DSP_DIR}/PrivateInclude
)
add_library(libdsp STATIC ${DSP_SRCS})
target_include_directories(libdsp PUBLIC ${DSP_INC_DIRS})
# 设置静态库的输出目录
set_target_properties(libdsp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# -------------------------------Middleware层------------------------------
set(MIDDLEWARE_INC_DIRS
        ${DATA_INC_DIRS}
        ${DSP_INC_DIRS}
)

set(MIDDLEWARE_SRCS
        ${DATA_SRCS}
        ${DSP_SRCS}
)

if (GUI_ENABLE)
    list(APPEND MIDDLEWARE_INC_DIRS ${LVGL_INC_DIRS})
    list(APPEND MIDDLEWARE_SRCS ${LVGL_SRCS})
    message(STATUS "[GUI]:ON")

    add_library(liblvgl STATIC ${LVGL_SRCS})
    target_include_directories(liblvgl PUBLIC ${LVGL_INC_DIRS})
    set_target_properties(liblvgl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
else ()
    message(STATUS "[GUI]:OFF")
endif ()


# 控制FreeRTOS的编译
if (FREERTOS_ENABLE)
    list(APPEND MIDDLEWARE_INC_DIRS ${FREERTOS_INC_DIRS})
    list(APPEND MIDDLEWARE_SRCS ${FREERTOS_SRCS})
    message(STATUS "[FreeRTOS]:ON")

    add_library(libfreertos STATIC ${FREERTOS_SRCS})
    target_include_directories(libfreertos PUBLIC ${FREERTOS_INC_DIRS})
    set_target_properties(libfreertos PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
else ()
    message(STATUS "[FreeRTOS]:OFF")
endif ()



