export module flash_storage;


export
{
#include <cstdint>

    struct FlashInfo
    {
        uint32_t last_write_addr;// 最后一次写入地址

    };



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
            void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length) = nullptr,
            void (*EraseSectorFunc)(uint32_t sectorAddress) = nullptr
    >
    class FlashStorage
    {
    public:

        // 预写入，用于在开始写入之前进行的一次地址归整操作。
        static auto write_isr_pre(uint32_t address = 256) -> void;

        static auto write_isr(uint8_t byte) -> void;

        static auto background_processing() -> void;

        // 鉴于M25P16擦除的最小单位是块，而一块有64KB大小，所以就无法轻易在写入头信息了。只能在整块都擦除的情况下，才能进行头信息写入。
        static auto readInfo() -> FlashInfo;

        // 保存信息
        static auto saveInfo() -> bool;

////         擦除所有内容
//      static auto erase() -> void;


    private:
        // 使用 enum class 定义常量,由于它是强类型，必须使用类型转换static_cast并不会占用内存，与constexpr相一样
        enum class Flags : uint8_t
        {
            BUFFER_WRITE_FULL_FLAG = 1U << 0,// 缓冲区写满标志->从ADC读取到前台缓冲区读满
            BUFFER_WRITE_INCOMPLETE_FLAG = 1U << 1, // 缓冲区写残缺标志
            WRITE_FULL_FLAG = 1U << 2,// 写满标志
        };
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


        // 辅助函数：设置指定标志
        static void set_flag(Flags flag)
        {
            status |= static_cast<uint8_t>(flag);
        }

        // 辅助函数：清除指定标志
        static void clear_flag(Flags flag)
        {
            status &= ~static_cast<uint8_t>(flag);
        }

        // 辅助函数：检查并清除指定标志，返回是否设置了该标志
        static uint8_t check_and_clear_flag(Flags flag)
        {
            if (status & static_cast<uint8_t>(flag))
            {
                clear_flag(flag); // 清除标志
                return 1;
            }
            return 0;
        }

        // 设置缓冲区写满标志
        static void set_buffer_write_full_sign() { set_flag(Flags::BUFFER_WRITE_FULL_FLAG); }

        // 设置缓冲区写残缺标志
        static void set_buffer_write_incomplete_sign() { set_flag(Flags::BUFFER_WRITE_INCOMPLETE_FLAG); }

        // 设置写满标志
        static void set_write_full_sign() { set_flag(Flags::WRITE_FULL_FLAG); }

        // 检测缓冲区写满标志
        static uint8_t check_buffer_write_full_sign()
        {
            return check_and_clear_flag(Flags::BUFFER_WRITE_FULL_FLAG);
        }

        // 检测写残缺标志
        static uint8_t check_buffer_write_incomplete_sign()
        {
            return check_and_clear_flag(Flags::BUFFER_WRITE_INCOMPLETE_FLAG);
        }

        static auto reset_index() -> void { index = 0; }// 重置缓冲区索引

    public:
        // 检测写满标志
        static uint8_t check_write_full_sign()
        {
            return check_and_clear_flag(Flags::WRITE_FULL_FLAG);
        }


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



        // 起始地址预留1页用于存储一些数据信息，比如上次写入地址等等
        enum class Addresses : uint32_t
        {
            FLASH_START_ADDR = 0x100, // 256B
            FLASH_END_ADDR = 0x200000, // 2MB
            FLASH_DUMMY_ADDR = 0xFFFFFFFF,// 无效地址
        };


    };
}

/**
 * @brief 中断预写入函数
 * @details 先判断此前是否处于写的状态，若不是则重置索引
 */
template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr_pre(uint32_t address) -> void
{
    addr = address;
    // 判断地址是否是页数的整数倍
    index = addr % page_size;
    addr_remaining = page_size - index;
    if (index != 0)
        set_buffer_write_incomplete_sign();
}


template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::write_isr(uint8_t byte) -> void
{
    if (index >= page_size - 1)
    {
        pForeBuffer[page_size - 1] = byte;
        switch_buffer();// 切换缓冲区
        set_buffer_write_full_sign();// 标记为写满
    } else if (addr >= static_cast<uint32_t>(Addresses::FLASH_END_ADDR))
    {
        // 设置写满标志
        set_write_full_sign();
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
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::background_processing() -> void
{
    // 写满标志检测
    if (check_buffer_write_full_sign())
    {
        // 残缺写入检测
        if (check_buffer_write_incomplete_sign())
        {
            /* 进行残缺的写入页数据*/
            WriteFunc(pBackBuffer + page_size - addr_remaining, addr, addr_remaining);//写入完整一页
            addr += addr_remaining;
        } else
        {
            /* 进行完整页的写入 */
            WriteFunc(pForeBuffer, addr, page_size);//写入完整一页
            addr += page_size;
        }

        // 写满标志检测
        if (addr >= static_cast<uint32_t>(Addresses::FLASH_END_ADDR))
        {
            set_write_full_sign();
        }
    }
}

/**
 * @brief 读取信息
 * @tparam WriteFunc
 * @tparam ReadFunc
 * @tparam EraseSectorFunc
 * @note 信息是指用户存入的FlashInfo结构体，目前只包含了最后写入地址
 * @return
 */
template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::readInfo() -> FlashInfo
{
    FlashInfo flashInfo{};
    ReadFunc(reinterpret_cast<uint8_t *>(&flashInfo), static_cast<uint32_t>(Addresses::FLASH_START_ADDR),
             sizeof(FlashInfo));
    return flashInfo;
}


/**
 * @brief 保存信息
 * @tparam WriteFunc
 * @tparam ReadFunc
 * @tparam EraseSectorFunc
 * @note 信息是指用户存入的FlashInfo结构体，目前只包含了最后写入地址
 * @return flash表示写入信息失败，可能是已经写入过信息了。true表示写入信息成功
 */
template<
        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length),
        void (*EraseSectorFunc)(uint32_t sectorAddress)
>
auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::saveInfo() -> bool
{
    FlashInfo flashInfo{};
    // 检查

    if (readInfo().last_write_addr != static_cast<uint32_t>(Addresses::FLASH_DUMMY_ADDR))
    {
        // 已经保存过信息，不需要再次保存，除非重新擦除Flash
        return false;
    }

    // 保存信息
    flashInfo.last_write_addr = addr;

    // 写入信息
    WriteFunc(reinterpret_cast<uint8_t *>(&flashInfo), static_cast<uint32_t>(Addresses::FLASH_START_ADDR),
              sizeof(flashInfo));
    return true;
}


///**
// * @brief 擦除Flash
// * @tparam WriteFunc
// * @tparam ReadFunc
// * @tparam EraseSectorFunc
// * @note 根据保存的信息来决定擦除多少扇区
// * @return
// */
//template<
//        void (*WriteFunc)(uint8_t *data, uint32_t address, uint16_t length),
//        void (*ReadFunc)(uint8_t *data, uint32_t address, uint16_t length),
//        void (*EraseSectorFunc)(uint32_t sectorAddress)
//>
//auto FlashStorage<WriteFunc, ReadFunc, EraseSectorFunc>::erase() -> void
//{
//
//}


