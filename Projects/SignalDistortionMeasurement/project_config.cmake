set_bsp_components(
        lcd
        touch
        delay
        rng
        spi
        adc
        usart
        tcp
        blue_tooth
#        debug
)

#options_disable_GUI()
#options_enable_FreeRTOS()
#options_enable_LTO()
#options_enable_static_lib_ld()
options_enable_DMA_SPI()
#options_enable_SD_SPI()
#options_enable_LCD_8080_PORT()
options_enable_LCD_SPI_PORT()
#options_enable_DMA_FSMC()
#options_enable_DMA_USART()
options_enable_FFT()