//
// Created by DV on 2024/10/14.
//
module;
#include "arm_math.h"
#include "arm_const_structs.h"
export module fft;


/**
 * @brief FFT
 * @note 实现FFT计算
 *
 */
export  namespace utils
{
    class FFT
    {
    public:
        static void voltageAmplitude_Convert() {
            //假设原始信号的峰值为A，那么FFT的结果的每个点（除了第一个点直流分量之外）的模值就是A的N/2倍。而第一个点就是直流分量，它的模值就是直流分量的N倍。
            for (int i = 1; i < NUM_SAMPLES / 2; ++i) {
                //理论上模值为峰峰值的N/2倍，实测中还应再除以0.75,后来发现就不需要了
                fft_outputbuf[i] *= 2.0f / NUM_SAMPLES; // 直接操作类内数组
            }
            fft_outputbuf[0] /= NUM_SAMPLES;
        }
        //归一化处理
        static void normalize_to_fundamental(int num_peaks) { // 移除outputArray参数
            if (num_peaks == 0) return;// 避免除以零或空数组
            float32_t fundamental = fft_outputbuf[peaks[1]]; //假设基波是第一个峰值,需要验证一下 peaks[1]是不是基波
            for (int i = 1; i < num_peaks; i++) {
                fft_output_normalized[i] = fft_outputbuf[peaks[i]] / fundamental;// 计算归一化值
            }
        }

        //滑动窗口找极值
        static void find_peaks_fundaments(uint32_t NumPeaks, uint16_t windowSize)
        {
            uint16_t maxIdx = 0;
            const uint16_t halfWindowSize = windowSize / 2;//以一般理性而言，不会为0
            //找到第一个峰值
            float32_t maxVal = 0;
            for (int i = 1; i <windowSize ; ++i)
            {

                if (fft_outputbuf[i] > maxVal)
                {
                    maxVal = fft_outputbuf[i];
                    maxIdx = i;
                }
            }
            peaks[1]=maxIdx;
            for (int i = 2; i < NumPeaks; ++i)
            {
                //估测谐波的索引
                peaks[i] = peaks[1] * i;
                //确定谐波的索引
                for (int j = peaks[i] - halfWindowSize; j < peaks[i] + halfWindowSize; ++j)
                {
                    if (fft_outputbuf[j] > fft_outputbuf[peaks[i]])
                    {
                        peaks[i] = j;
                    }
                }
            }


        }
        // 计算信号的幅度谱
        static inline void Amplitude_Convert(){
            //计算信号的幅度谱  第一个参数指定了需要计算复数模的数组指针，第二个参数指定了计算结果存放的数组指针，第三个参数是需要计算复数模的数据个数。
            // 分辨率=fs（采样频率）/N（采样点数）  output输出数组，索引*分辨率=频率成分
            arm_cmplx_mag_f32(fft_inputBuff, fft_outputbuf, NUM_SAMPLES / 2);
        }
        static void ADCdataToSpectrum(const int16_t ADCdata[]){
            /**将实数序列转为复数序列*/
            memset(fft_inputBuff, 0, sizeof(fft_inputBuff)); //虚部为零
            for (int i = 0; i < NUM_SAMPLES; ++i)
            {
                fft_inputBuff[i * 2] =  ADCdata[i];//填充实部
            }
            arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputBuff, IFFTFLAG, BITREVERSE);
        }

        // 计算信号的失真度
        static float signalDistortionDegree(uint16_t NumPeaks)
        {
            float sum = 0;
            //从二次谐波开始计算平方和
            for (int i = 2; i < NumPeaks; i++)
            {
                if (peaks[i] == 0) break;
                sum += fft_outputbuf[peaks[i]] * fft_outputbuf[peaks[i]]; // 直接使用类内数组
            }
            sum /= (fft_outputbuf[peaks[1]] * fft_outputbuf[peaks[1]]);//除以1次谐波的平方
            return thd = sqrtf(sum) * 100;//计算出失真度
        }
        /*
         * @brief 将FFT结果转换为lvgl的图表数据
         * @param input FFT结果数组
         * @param output lvgl图表数据数组
         * @param div 采样点数
         * @param offset 偏移量(去除偏置带来的直流分量)
         */
        static void convert_fft_to_lvgl(const float_t* input, int32_t* output,size_t div,uint16_t offset) {
            const size_t output_len = NUM_SAMPLES / div;

            for (size_t i = 0; i < output_len; ++i) {
                // 每div个点取最大值
                float_t max_val = input[i*div];
                for (size_t j = 1; j < div; ++j) {
                    if (input[i*div + j] > max_val) {
                        max_val = input[i*div + j];
                    }
                }
                // 直接转换为int32_t
                output[i] = static_cast<int32_t>(max_val);
            }
            output[0] -=offset;
        }

        static float_t* get_fft_output_normalized() {
            return fft_output_normalized;  // 返回指针
        }
        static float_t * get_fft_output_ptr() {
            return fft_outputbuf;  // 返回指针
        }
        static float_t get_thd() {
            return thd;  // 返回指针
        }
    private:
        static constexpr const int NUM_SAMPLES=1024;  //采样点
        static constexpr const int NUM_PEAKS=10;       //取10-1个基波谐波成分
        static constexpr const int IFFTFLAG=0;        //正变换
        static constexpr const int BITREVERSE=1;      //逆序排列
        static inline float32_t fft_inputBuff[NUM_SAMPLES * 2]={};     //存储复数的数组
        static inline float32_t fft_outputbuf[NUM_SAMPLES / 2]={};     //存储实数的数组，由于奈奎斯特的特性，需要除以2
        static inline float32_t fft_output_normalized[NUM_PEAKS]={};
        static inline uint16_t peaks[NUM_PEAKS]={};//幅值谱的极大值点
        static inline float power[NUM_PEAKS]={};   //功率谱
        static inline float totalPower=0;         //总功率单位为V^2
        static inline float thd=0;                //失真度

    };
}