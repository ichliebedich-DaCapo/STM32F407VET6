set(BSP_DIR ${CMAKE_SOURCE_DIR}/BSP)
# 驱动注册表（名称:依赖项）
# 默认包含 gpio
# 驱动注册表（名称:依赖项）
# 说明：需要确保注册的名称与实际文件相同，包括大小写。一般建议使用小写加下划线来命名，并且不要包含hal_或者ll_
#      注册时，把驱动文件写在左边，依赖写在右边
set(BSP_DRIVERS
        "default:hal,hal_cortex,hal_gpio,baseInit"
        # BSP驱动
        "adc:timer,hal_adc,hal_adc_ex"
        "baseInit:rcc,lcd,hal_tim,key_exit"
        "cpu_runtime:"
        "crc:hal_crc"
        "dac:hal_dac,timer"
        "debug:"
        "delay:timer"
        "esp8266:"
        "esp_8266:"
        "fsmc:hal_sram,hal_dma"
        "i2c:hal_i2c"
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


# 依赖项文件映射
include(cmake/structure/cmsis_hal.cmake)
set(hal_SRC ${HAL_SRC_DIR}/stm32f4xx_hal.c)
# ADC
set(hal_adc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_adc.c)
set(hal_adc_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_adc_ex.c)
# CAN
set(hal_can_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_can.c)
# CEC
set(hal_cec_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_cec.c)
# CORTEX
set(hal_cortex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_cortex.c)
# CRC
set(hal_crc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_crc.c)
# CRYP
set(hal_cryp_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_cryp.c)
set(hal_cryp_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_cryp_ex.c)
# DAC
set(hal_dac_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dac.c)
set(hal_dac_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dac_ex.c)
# DCMI
set(hal_dcmi_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dcmi.c)
set(hal_dcmi_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dcmi_ex.c)
# DFSDM
set(hal_dfsdm_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dfsdm.c)
# DMA
set(hal_dma_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dma.c)
set(hal_dma2d_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dma2d.c)
set(hal_dma_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dma_ex.c)
# DSI
set(hal_dsi_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_dsi.c)
# ETH
set(hal_eth_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_eth.c)
# FLASH
set(hal_flash_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_flash.c)
set(hal_flash_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_flash_ex.c)
set(hal_flash_ramfunc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_flash_ramfunc.c)
# FMPI2C
set(hal_fmpi2c_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_fmpi2c.c)
set(hal_fmpi2c_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_fmpi2c_ex.c)
# FMPSMBUS
set(hal_fmpsmbus_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_fmpsmbus.c)
set(hal_fmpsmbus_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_fmpsmbus_ex.c)
# GPIO
set(hal_gpio_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_gpio.c)
# HASH
set(hal_hash_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_hash.c)
set(hal_hash_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_hash_ex.c)
# HCD
set(hal_hcd_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_hcd.c)
# I2C
set(hal_i2c_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_i2c.c)
set(hal_i2c_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_i2c_ex.c)
# I2S
set(hal_i2s_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_i2s.c)
set(hal_i2s_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_i2s_ex.c)
# IRDA
set(hal_irda_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_irda.c)
# IWDG
set(hal_iwdg_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_iwdg.c)
# LPTIM
set(hal_lptim_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_lptim.c)
# LTDC
set(hal_ltdc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_ltdc.c)
set(hal_ltdc_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_ltdc_ex.c)
# MMC
set(hal_mmc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_mmc.c)
# NAND
set(hal_nand_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_nand.c)
# NOR
set(hal_nor_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_nor.c)
# PCCARD
set(hal_pccard_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_pccard.c)
# PCD
set(hal_pcd_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_pcd.c)
set(hal_pcd_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_pcd_ex.c)
# PWR
set(hal_pwr_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_pwr.c)
set(hal_pwr_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_pwr_ex.c)
# QSPI
set(hal_qspi_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_qspi.c)
# RCC
set(hal_rcc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_rcc.c)
set(hal_rcc_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_rcc_ex.c)
# RNG
set(hal_rng_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_rng.c)
# RTC
set(hal_rtc_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_rtc.c)
set(hal_rtc_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_rtc_ex.c)
# SAI
set(hal_sai_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_sai.c)
set(hal_sai_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_sai_ex.c)
# SD
set(hal_sd_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_sd.c)
# SDRAM
set(hal_sdram_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_sdram.c)
# SMARTCARD
set(hal_smartcard_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_smartcard.c)
# SMBUS
set(hal_smbus_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_smbus.c)
# SPDIFRX
set(hal_spdifrx_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_spdifrx.c)
# SPI
set(hal_spi_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_spi.c)
# SRAM
set(hal_sram_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_sram.c)
# TIM
set(hal_tim_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_tim.c)
set(hal_tim_ex_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_tim_ex.c)
# UART
set(hal_uart_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_uart.c)
# USART
set(hal_usart_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_usart.c)
# WWDG
set(hal_wwdg_SRC ${HAL_SRC_DIR}/stm32f4xx_hal_wwdg.c)
# LL (Low Layer) Drivers
set(ll_adc_SRC ${HAL_SRC_DIR}/stm32f4xx_ll_adc.c)
set(ll_fmc_SRC ${HAL_SRC_DIR}/stm32f4xx_ll_fmc.c)
set(ll_fsmc_SRC ${HAL_SRC_DIR}/stm32f4xx_ll_fsmc.c)
set(ll_sdmmc_SRC ${HAL_SRC_DIR}/stm32f4xx_ll_sdmmc.c)

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

        # message(STATUS "ADDED_COMPONENTS:${ADDED_COMPONENTS}")

        # 根据组件名称设置对应的变量（HAL或BSP）
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

        # 查找依赖项
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

        # 添加源文件
        string(FIND "${comp}" "hal_" HAL_POSITION)
        string(FIND "${comp}" "ll_" LL_POSITION)
        if (HAL_POSITION GREATER -1 OR LL_POSITION GREATER -1 OR "${comp}" STREQUAL "hal" OR "${comp}" STREQUAL "default")
            # 包含hal
            if (${comp}_SRC)
                list(APPEND BSP_SRCS ${${comp}_SRC})
            endif ()
            # 修改后的代码段：
        else ()
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
configure_file(
        cmake/structure/stm32f4xx_hal_conf.h.in
        ${DRIVERS_DIR}/stm32f4xx_hal_conf.h
)
