export module flash_storage;


export
{
#include <cstdint>

    /**
     * @brief Flash存储器读写算法
     * @note
     *    1.该算法为单线程算法，即只能由一个线程调用该算法，否则会出错
     *    2，内置了两个缓冲区，一个作为后台缓冲区，一个作为前台缓冲区，前台缓冲区用于读取ADC的数据，后台缓冲区用于
     *    在主循环里向存储器里写入数据
     *    3，当前台缓冲区满时，会自动切换两个缓冲区，并重置索引
     *    4，之所以要读取一页，那是因为要做傅里叶变换
     *
     */
    template<
            void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
            void (*ReadFunc)(uint8_t *data, uint32_t address, uint32_t length) = nullptr,
            void (*EraseSectorFunc)(uint32_t sectorAddress) = nullptr
    >
    class FlashStorage
    {
    public:

        // 预写入，用于在开始写入之前进行的一次地址归整操作。
        static auto write_isr_pre(uint32_t address = 256) -> void;

        static auto write_isr(uint8_t byte) -> void;

        static auto background_processing() -> void;
        /*还需要提供擦除所有扇区的接口、提供Flash记录地址的接口、修改Flash记录地址的接口*/

    private:
        // 切换缓冲区并重置索引
        static auto switch_buffer() -> void
        {
            // 重置索引
            reset_index();
            uint8_t *temp = pBackBuffer;
            pBackBuffer = pForeBuffer;
            pForeBuffer = temp;
        }


        static auto set_write_full_sign() -> void { status |= static_cast<uint8_t>(Flags::WRITE_FULL_FLAG); }// 设置写满标志

        // 设置写残缺标志
        static auto
        set_write_incomplete_sign() -> void { status |= static_cast<uint8_t>(Flags::WRITE_INCOMPLETE_FLAG); }


        // 检测写满标志
        static auto check_write_full_sign() -> uint8_t
        {
            if (status & static_cast<uint8_t>(Flags::WRITE_FULL_FLAG))
            {
                clear_write_full_sign();
                return 1;
            } else
            {
                return 0;
            }
        }


        // 检测写残缺标志
        static auto check_write_incomplete_sign() -> uint8_t
        {
            if (status & static_cast<uint8_t>(Flags::WRITE_INCOMPLETE_FLAG))
            {
                clear_write_incomplete_sign();
                return 1;
            } else
            {
                return 0;
            }
        }


        static auto
        clear_write_full_sign() -> void { status &= ~static_cast<uint8_t>(Flags::WRITE_FULL_FLAG); }// 清除写满标志

        static auto
        clear_write_incomplete_sign() -> void { status &= ~static_cast<uint8_t>(Flags::WRITE_INCOMPLETE_FLAG); }// 清除写残缺标志

        static auto reset_index() -> void { index = 0; }// 重置缓冲区索引

        //低耦合高内聚
    private:
        static constexpr const uint16_t page_size = 256;// 页大小,不占内存
        static inline uint8_t buffer1[page_size] = {};
        static inline uint8_t buffer2[page_size] = {};
        static inline uint8_t *pBackBuffer = buffer1;// 后台缓冲区指针
        static inline uint8_t *pForeBuffer = buffer2;// 前台缓冲区指针
        // FLASH存储地址 鉴于加载缓冲区都依靠该地址，所以该地址表示的是“将要”加载到缓冲区对应的Flash的起始地址
        static inline uint32_t addr = 256;
        static inline uint32_t addr_remaining = 256;// 剩余地址，用于存储初始地址残缺的量
        /**
         * @details 0'b:读写标志，默认0为写
         * 1'b:读满标志，默认0为未满
         * 2'b:写满标志，默认0为未满
         * 3'b:表示读残缺，默认0为无残缺    所谓残缺就是指输入的地址不是Flash的页字节的整数倍
         * 4'b:表示写残缺，默认0为无残缺
         */
        static inline uint8_t status = 0; // 状态，用于在单线程下处理各种“标志”
        static inline uint16_t index = 0; // 记录缓冲数组的索引，可根据需要改变数据类型,现在留一定余量

        // 使用 enum class 定义常量,由于它是强类型，必须使用类型转换static_cast并不会占用内存，与constexpr相一样
        enum class Flags : uint8_t
        {
            WRITE_FULL_FLAG = 1U << 0,// 写满标志->从ADC读取到前台缓冲区读满
            WRITE_INCOMPLETE_FLAG = 1U << 1 // 写残缺标志
        };

        // 起始地址预留1页用于存储一些数据信息，比如上次写入地址等等
        enum class Addresses : uint32_t
        {
            FLASH_START_ADDR = 0x100, // 256B
            FLASH_END_ADDR = 0x200000 // 2MB
        };
    };
}

/**
 * @brief 中断预写入函数
 * @details 先判断此前是否处于写的状态，若不是则重置索引
 */
template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint32_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr_pre(uint32_t address) -> void
{
    addr = address;
    // 判断地址是否是页数的整数倍
    index = addr % page_size;
    addr_remaining = page_size - index;
    if (index != 0)
        set_write_incomplete_sign();
}


template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint32_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr(uint8_t byte) -> void
{
    if (index == page_size - 1)
    {
        pForeBuffer[page_size - 1] = byte;
        switch_buffer();// 切换缓冲区
        set_write_full_sign();// 标记为写满
    } else
    {
        pForeBuffer[index++] = byte;
    }
}


/**
 * @brief 后台处理函数，用于在主循环中调用
 * @note 检测对应标志就进行相应处理，不要做多余的操作
 */
template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint32_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::background_processing() -> void
{
    // 写满标志检测
    if (check_write_full_sign())
    {


        // 残缺写入检测
        if (check_write_incomplete_sign())
        {
            /* 进行残缺的写入页数据*/
            WriteFunc(pBackBuffer+page_size-addr_remaining, addr, addr_remaining);//写入完整一页
            addr += addr_remaining;
        } else
        {
            /* 进行完整页的写入 */
            WriteFunc(pForeBuffer, addr, page_size);//写入完整一页
            addr += page_size;
        }
    }
}



