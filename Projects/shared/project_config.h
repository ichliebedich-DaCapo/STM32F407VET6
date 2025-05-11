#ifndef PROJECT_CONFIG_H
#define PROJECT_CONFIG_H
#include<common_config.h>
#include<stm32f4xx_hal.h>

// ---------配置项-----------
// 与启用中间库和一些工程的编译有关
/*-------------------------------- 系统功能配置 --------------------------------*/
/* #undef GUI_ENABLE */
/* #undef FREERTOS_ENABLE */
/* #undef FREERTOS_DEBUG */
/* #undef AI_ENABLE */

/*-------------------------------- 编译优化配置 --------------------------------*/
/* #undef STATIC_LIB_LD */
/* #undef LTO_ENABLE */

/*-------------------------------- 外设驱动配置 --------------------------------*/
/* #undef DMA_SPI_ENABLE */
/* #undef DMA_FSMC_ENABLE */
/* #undef SD_SPI_ENABLE */
/* #undef LCD_8080_PORT_ENABLE */
/* #undef LCD_SPI_PORT_ENABLE */
#define DMA_USART_ENABLE      /* USART DMA传输支持 */

/*-------------------------------- 算法库配置 --------------------------------*/
/* #undef FFT_ENABLE */



// -------------与gcc扩展有关--------------
#ifdef LTO_ENABLE
#define USED __attribute__((used))
#else
#define USED

#endif

#endif // PROJECT_CONFIG_H
