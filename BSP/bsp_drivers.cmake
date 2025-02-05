# 驱动注册表（名称:依赖项）
# 默认包含 gpio
# 驱动注册表（名称:依赖项）
set(BSP_DRIVERS
        "default:hal,hal_cortex,hal_gpio,baseinit"
        # BSP驱动
        "adc:timer,hal_adc,hal_adc_ex"
        "baseinit:rcc,lcd,hal_tim,key_exit"
        "cpu_runtime:"
        "dac:hal_dac"
        "debug:"
        "esp8266:"
        "fsmc:hal_sram,hal_dma"
        "key_exit:"
        "lcd:fsmc,spi"
        "mqtt:"
        "pwr:"
        "rcc:hal_rcc,hal_flash_ex"
        "rng:hal_rng"
        "sdio:"
        "spi:hal_spi"
        "tcp:"
        "timer:"
        "usart:"
        "w25qxx:"
        # HAL库
        "hal_sram:ll_fsmc"
)

# 驱动源文件映射
set(adc_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/adc.c)
set(baseinit_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/baseInit.c)
set(cpu_runtime_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/CPU_RunTime.c)
set(dac_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dac.c)
set(debug_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/debug.c)
set(esp8266_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/esp8266.c)
set(fsmc_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/fsmc.c)
set(key_exit_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/key_exit.c)
set(lcd_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/lcd.c)
set(mqtt_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/mqtt.c)
set(pwr_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/pwr.c)
set(rcc_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/RCC.c)
set(rng_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/RNG.c)
set(sdio_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/sdio.c)
set(spi_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/spi.c)
set(tcp_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/tcp.c)
set(timer_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/timer.c)
set(usart_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/usart.c)
set(w25qxx_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/w25qxx.c)


# 依赖项文件映射
include(core.cmake)
set(hal_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal.c)
# ADC
set(hal_adc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_adc.c)
set(hal_adc_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_adc_ex.c)
# CAN
set(hal_can_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_can.c)
# CEC
set(hal_cec_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_cec.c)
# CORTEX
set(hal_cortex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_cortex.c)
# CRC
set(hal_crc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_crc.c)
# CRYP
set(hal_cryp_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_cryp.c)
set(hal_cryp_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_cryp_ex.c)
# DAC
set(hal_dac_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dac.c)
set(hal_dac_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dac_ex.c)
# DCMI
set(hal_dcmi_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dcmi.c)
set(hal_dcmi_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dcmi_ex.c)
# DFSDM
set(hal_dfsdm_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dfsdm.c)
# DMA
set(hal_dma_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dma.c)
set(hal_dma2d_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dma2d.c)
set(hal_dma_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dma_ex.c)
# DSI
set(hal_dsi_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_dsi.c)
# ETH
set(hal_eth_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_eth.c)
# FLASH
set(hal_flash_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_flash.c)
set(hal_flash_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_flash_ex.c)
set(hal_flash_ramfunc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_flash_ramfunc.c)
# FMPI2C
set(hal_fmpi2c_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_fmpi2c.c)
set(hal_fmpi2c_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_fmpi2c_ex.c)
# FMPSMBUS
set(hal_fmpsmbus_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_fmpsmbus.c)
set(hal_fmpsmbus_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_fmpsmbus_ex.c)
# GPIO
set(hal_gpio_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_gpio.c)
# HASH
set(hal_hash_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_hash.c)
set(hal_hash_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_hash_ex.c)
# HCD
set(hal_hcd_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_hcd.c)
# I2C
set(hal_i2c_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_i2c.c)
set(hal_i2c_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_i2c_ex.c)
# I2S
set(hal_i2s_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_i2s.c)
set(hal_i2s_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_i2s_ex.c)
# IRDA
set(hal_irda_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_irda.c)
# IWDG
set(hal_iwdg_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_iwdg.c)
# LPTIM
set(hal_lptim_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_lptim.c)
# LTDC
set(hal_ltdc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_ltdc.c)
set(hal_ltdc_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_ltdc_ex.c)
# MMC
set(hal_mmc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_mmc.c)
# NAND
set(hal_nand_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_nand.c)
# NOR
set(hal_nor_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_nor.c)
# PCCARD
set(hal_pccard_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_pccard.c)
# PCD
set(hal_pcd_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_pcd.c)
set(hal_pcd_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_pcd_ex.c)
# PWR
set(hal_pwr_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_pwr.c)
set(hal_pwr_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_pwr_ex.c)
# QSPI
set(hal_qspi_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_qspi.c)
# RCC
set(hal_rcc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_rcc.c)
set(hal_rcc_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_rcc_ex.c)
# RNG
set(hal_rng_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_rng.c)
# RTC
set(hal_rtc_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_rtc.c)
set(hal_rtc_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_rtc_ex.c)
# SAI
set(hal_sai_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_sai.c)
set(hal_sai_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_sai_ex.c)
# SD
set(hal_sd_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_sd.c)
# SDRAM
set(hal_sdram_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_sdram.c)
# SMARTCARD
set(hal_smartcard_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_smartcard.c)
# SMBUS
set(hal_smbus_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_smbus.c)
# SPDIFRX
set(hal_spdifrx_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_spdifrx.c)
# SPI
set(hal_spi_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_spi.c)
# SRAM
set(hal_sram_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_sram.c)
# TIM
set(hal_tim_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_tim.c)
set(hal_tim_ex_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_tim_ex.c)
# UART
set(hal_uart_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_uart.c)
# USART
set(hal_usart_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_usart.c)
# WWDG
set(hal_wwdg_SRCS ${HAL_SRC_DIR}/stm32f4xx_hal_wwdg.c)
# LL (Low Layer) Drivers
set(ll_adc_SRCS ${HAL_SRC_DIR}/stm32f4xx_ll_adc.c)
set(ll_fmc_SRCS ${HAL_SRC_DIR}/stm32f4xx_ll_fmc.c)
set(ll_fsmc_SRCS ${HAL_SRC_DIR}/stm32f4xx_ll_fsmc.c)
set(ll_sdmmc_SRCS ${HAL_SRC_DIR}/stm32f4xx_ll_sdmmc.c)