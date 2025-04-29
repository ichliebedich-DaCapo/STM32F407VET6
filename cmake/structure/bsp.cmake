# 设置Core子目录
set(DRIVERS_DIR Drivers)
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
set(BSP_DIR ${CMAKE_SOURCE_DIR}/BSP)
# 驱动注册表（名称:依赖项）
# 默认包含 gpio
# 驱动注册表（名称:依赖项）
# 说明：需要确保注册的名称与实际文件相同，包括大小写。一般建议使用小写加下划线来命名，并且不要包含hal_或者ll_
#      注册时，把驱动文件写在左边，依赖写在右边
set(BSP_DRIVERS
        "default:hal,hal_cortex,baseInit,gpio,ll_utils"
        # BSP驱动
        "adc:timer,hal_adc,hal_adc_ex"
        "baseInit:rcc,lcd,hal_tim,key_exit,ll_rcc"
        "cpu_runtime:"
        "crc:hal_crc"
        "dac:hal_dac,timer"
        "debug:"
        "delay:timer"
        "esp8266:"
        "esp_8266:"
        "fsmc:hal_sram,hal_dma,ll_dma"
        "gpio:hal_gpio,ll_gpio"
        "i2c:hal_i2c,ll_i2c"
        "key_exit:"
        "lcd:fsmc,spi,registers"
        "mqtt:"
        "pwr:hal_pwr"
        "rcc:hal_rcc,hal_flash_ex"
        "rng:hal_rng"
        "sdio:"
        "spi:hal_spi"
        "timer:hal_tim_ex"
        "touch:delay,i2c"
        "usart:hal_uart"
        "w25qxx:spi"
        # HAL库
        "hal_sram:ll_fsmc"
)


# -----------根据项目需求选择驱动----------
if (EXISTS "${PROJECT_SOURCE_DIR}/projects/${TARGET_PROJECT}/project_config.cmake")
    include(${PROJECT_SOURCE_DIR}/projects/${TARGET_PROJECT}/project_config.cmake)
else ()
    # 在Projects下对应的项目目录里添加project_config.cmake
    message(WARNING "Can not find project_config.cmake!!")
    set(BSP_COMPONENTS lcd adc) # 默认组件
endif ()


# 初始化已添加组件列表和源文件列表
set(ADDED_COMPONENTS "" CACHE INTERNAL "List of added components")
set(BSP_SRCS "" CACHE INTERNAL "List of BSP source files")
set(BSP_INC_DIRS "${DRIVERS_INC_DIRS}" CACHE INTERNAL "List of BSP header dirs")
set(bsp_modules "" CACHE INTERNAL "List of BSP module files")


# 递归解析依赖
function(add_driver_component comp)
    # 检查当前组件是否已经添加
    list(FIND ADDED_COMPONENTS ${comp} idx)
    if (idx EQUAL -1)
        # 添加当前组件到已添加列表中
        list(APPEND ADDED_COMPONENTS ${comp})
        set(ADDED_COMPONENTS "${ADDED_COMPONENTS}" CACHE INTERNAL "List of added components")

        # ============= 根据组件名称设置对应的变量（HAL或BSP）=============
        string(TOUPPER ${comp} COMP_UPPER)
        if (comp MATCHES "^hal_")
            # HAL组件：去掉前缀"hal_"，设置HAL_USE_<COMPONENT>
            string(REGEX REPLACE "^hal_" "" HAL_COMP ${comp})
            string(REGEX REPLACE "_ex$" "" HAL_COMP ${HAL_COMP})  # 去除可能的"_ex"后缀
            string(TOUPPER ${HAL_COMP} HAL_COMP_UPPER)
            set(HAL_${HAL_COMP_UPPER}_MODULE_ENABLED ON CACHE INTERNAL "Enable HAL component ${HAL_COMP}")
        elseif (comp MATCHES "^ll_")
            # LL组件（可选）：设置LL_USE_<COMPONENT>
            string(REGEX REPLACE "^ll_" "" LL_COMP ${comp})
            string(TOUPPER ${LL_COMP} LL_COMP_UPPER)
            set(LL_USE_${LL_COMP_UPPER} ON CACHE INTERNAL "Enable LL component ${LL_COMP}")
        else ()
        endif ()

        # ================= 查找依赖项 ==================
        foreach (driver ${BSP_DRIVERS})
            string(REGEX MATCH "^${comp}:" match ${driver})
            if (match)
                # 提取依赖项部分（去掉驱动名称和冒号）
                string(REPLACE "${comp}:" "" deps_str ${driver})
                # 将依赖项字符串按逗号分割成列表
                string(REPLACE "," ";" deps_list "${deps_str}")
                # 递归处理每个依赖项
                foreach (dep ${deps_list})
                    add_driver_component(${dep})
                endforeach ()
            endif ()
        endforeach ()

        # =============== 添加源文件 ================
        string(FIND "${comp}" "hal_" HAL_POSITION)
        string(FIND "${comp}" "ll_" LL_POSITION)
        if ( "${comp}" STREQUAL "default")
            # 什么都不做
        elseif( "${comp}" STREQUAL "hal" OR HAL_POSITION GREATER -1 OR LL_POSITION GREATER -1)
            # 自动根据名称添加HAL库或LL库
            list(APPEND BSP_SRCS ${HAL_SRC_DIR}/stm32f4xx_${comp}.c)
        else ()
            # 添加BSP库
            set(bsp_src "${BSP_DIR}/${comp}.ixx")
            if (EXISTS ${bsp_src})
                get_property(current_modules CACHE bsp_modules PROPERTY VALUE)
                # 检查是否已存在
                list(FIND current_modules ${bsp_src} idx)
                if (idx EQUAL -1)
                    list(APPEND current_modules ${bsp_src})
                    set(bsp_modules "${current_modules}" CACHE INTERNAL "List of BSP module files")
                endif()
            else ()
                message(WARNING "|ignore|:${comp}")
            endif ()
        endif ()
        set(BSP_SRCS "${BSP_SRCS}" CACHE INTERNAL "List of BSP source files")
    endif ()
endfunction()


# ------------------------解析BSP依赖-------------------------------
# 添加默认组件
list(APPEND BSP_COMPONENTS "default")
foreach (comp ${BSP_COMPONENTS})
    add_driver_component(${comp})
    message(STATUS "[added]:${comp}  ")
endforeach ()
# ------------------------BSP库-----------------------------
# BSP_SRCS有BSP下的CMakeLists传递进来
list(APPEND BSP_INC_DIRS "${BSP_DIR}")
add_library(libbsp STATIC ${BSP_SRCS})
target_include_directories(libbsp PUBLIC ${BSP_INC_DIRS})
# 设置静态库的输出目录
set_target_properties(libbsp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

# 生成hal配置头文件
configure_file(cmake/structure/stm32f4xx_hal_conf.h.in ${DRIVERS_DIR}/stm32f4xx_hal_conf.h)
