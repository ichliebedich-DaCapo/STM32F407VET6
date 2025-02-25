#include "fatfs.h"
#include "stm32f4xx_hal.h"
#include "sd_spi.h"


/* 为每个设备定义一个物理编号 */
#define ATA			           0     // SD卡
#define SPI_FLASH		       1     // 预留外部SPI Flash使用

//固定只支持blocksize大小为512的卡，兼容大于512的卡时，该卡容量会变小
#define SD_BLOCKSIZE     512//SDCardInfo.CardBlockSize

extern SD_CardInfo SDCardInfo;
/*-----------------------------------------------------------------------*/
/* 获取设备状态                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS fatfs_status (
        BYTE pdrv		/* 物理编号 */
)
{
    DSTATUS status = STA_NOINIT;

    switch (pdrv) {
        case ATA:	/* SD CARD */
            status &= ~STA_NOINIT;
            break;

        case SPI_FLASH:        /* SPI Flash */
            break;

        default:
            status = STA_NOINIT;
    }
    return status;
}

/*-----------------------------------------------------------------------*/
/* 设备初始化                                                            */
/*-----------------------------------------------------------------------*/
DSTATUS fatfs_init (
        BYTE pdrv				/* 物理编号 */
)
{
    DSTATUS status = STA_NOINIT;
    switch (pdrv) {
        case ATA:	         /* SD CARD */
            if(spi_sd_init()==SD_RESPONSE_NO_ERROR)
            {
                status &= ~STA_NOINIT;
            }
            else
            {
                status = STA_NOINIT;
            }

            break;

        case SPI_FLASH:    /* SPI Flash */
            break;

        default:
            status = STA_NOINIT;
    }
    return status;
}


/*-----------------------------------------------------------------------*/
/* 读扇区：读取扇区内容到指定存储区                                              */
/*-----------------------------------------------------------------------*/
DRESULT fatfs_read (
        BYTE pdrv,		/* 设备物理编号(0..) */
        BYTE *buff,		/* 数据缓存区 */
        DWORD sector,	/* 扇区首地址 */
        UINT count		/* 扇区个数(1..128) */
)
{
    DRESULT status = RES_PARERR;
    SD_Error SD_state = SD_RESPONSE_NO_ERROR;

    switch (pdrv) {
        case ATA:	/* SD CARD */

            SD_state=SD_ReadMultiBlocks(buff,(uint64_t)sector*SD_BLOCKSIZE,SD_BLOCKSIZE,count);

            if(SD_state!=SD_RESPONSE_NO_ERROR)
                status = RES_PARERR;
            else
                status = RES_OK;
            break;

        case SPI_FLASH:
            break;

        default:
            status = RES_PARERR;
    }
    return status;
}



/*-----------------------------------------------------------------------*/
/* 写扇区：见数据写入指定扇区空间上                                      */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT fatfs_write (
        BYTE pdrv,			  /* 设备物理编号(0..) */
        const BYTE *buff,	/* 欲写入数据的缓存区 */
        DWORD sector,		  /* 扇区首地址 */
        UINT count			  /* 扇区个数(1..128) */
)
{
    DRESULT status = RES_PARERR;
    SD_Error SD_state = SD_RESPONSE_NO_ERROR;

    if (!count) {
        return RES_PARERR;		/* Check parameter */
    }

    switch (pdrv) {
        case ATA:	/* SD CARD */

            SD_state=SD_WriteMultiBlocks((uint8_t *)buff,(uint64_t)sector*SD_BLOCKSIZE,SD_BLOCKSIZE,count);

            if(SD_state!=SD_RESPONSE_NO_ERROR)
                status = RES_PARERR;
            else
                status = RES_OK;
            break;

        case SPI_FLASH:
            break;

        default:
            status = RES_PARERR;
    }
    return status;
}
#endif


/*-----------------------------------------------------------------------*/
/* 其他控制                                                              */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT fatfs_ioctl (
        BYTE pdrv,		/* 物理编号 */
        BYTE cmd,		  /* 控制指令 */
        void *buff		/* 写入或者读取数据地址指针 */
)
{
    DRESULT status = RES_PARERR;
    switch (pdrv) {
        case ATA:	/* SD CARD */
            switch (cmd)
            {
                // Get R/W sector size (WORD)
                case GET_SECTOR_SIZE :
                    *(WORD * )buff = SD_BLOCKSIZE;
                    break;
                    // Get erase block size in unit of sector (DWORD)
                case GET_BLOCK_SIZE :
                    *(DWORD * )buff = 1;
                    break;

                case GET_SECTOR_COUNT:
                    *(DWORD * )buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
                    break;
                case CTRL_SYNC :
                    break;
            }
            status = RES_OK;
            break;

        case SPI_FLASH:
            break;

        default:
            status = RES_PARERR;
    }
    return status;
}
#endif


__weak DWORD get_fattime(void) {
    /* 返回当前时间戳 */
    return	  ((DWORD)(2015 - 1980) << 25)	/* Year 2015 */
                | ((DWORD)1 << 21)				/* Month 1 */
                | ((DWORD)1 << 16)				/* Mday 1 */
                | ((DWORD)0 << 11)				/* Hour 0 */
                | ((DWORD)0 << 5)				  /* Min 0 */
                | ((DWORD)0 >> 1);				/* Sec 0 */
}
