//
// Created by DV on 2024/10/14.
//
#include "common_config.h"
#include "arm_math.h"
#include "arm_const_structs.h"
#ifndef FURINA_DSP_HPP
#define FURINA_DSP_HPP



/* 预编译命令 */
#ifdef FFT_ENABLE
// 头文件

// 宏定义
/**
 * @brief FFT
 * @note 实现FFT计算
 *
 */
class FFT_Calculation
{
public:
    void voltageAmplitude_Convert() {
        //假设原始信号的峰值为A，那么FFT的结果的每个点（除了第一个点直流分量之外）的模值就是A的N/2倍。而第一个点就是直流分量，它的模值就是直流分量的N倍。
        for (uint16_t i = 1; i < NUM_SAMPLES / 2; ++i) {
            //理论上模值为峰峰值的N/2倍，实测中还应再除以0.75,后来发现就不需要了
            fft_outputbuf[i] *= 2.0f / NUM_SAMPLES; // 直接操作类内数组
        }
        fft_outputbuf[0] /= NUM_SAMPLES;
    }
    void normalize_to_fundamental(int num_peaks) { // 移除outputArray参数
        if (num_peaks == 0) return;// 避免除以零或空数组
        float32_t fundamental = fft_outputbuf[peaks[1]]; //假设基波是第一个峰值,需要验证一下 peaks[1]是不是基波
        for (int i = 1; i < num_peaks; i++) {
            fft_output_normalized[i] = fft_outputbuf[peaks[i]] / fundamental;// 计算归一化值
        }
    }
    void find_peaks(uint32_t NumPeaks, uint16_t windowSize)
    {
        {
            float32_t maxVal;
            uint16_t maxIdx;
            uint16_t halfWindowSize = windowSize / 2;//以一般理性而言，不会为0
            uint16_t numPeaks = 1;                   //从一次谐波开始

            //重置
            for (uint32_t i = 0; i <NumPeaks ; ++i)
            {
                peaks[i] = 0;
            }
            // 开始寻找交流成分
            for (uint16_t i = halfWindowSize; i < 512 - halfWindowSize; i++)
            {
                maxVal = fft_outputbuf[i];
                maxIdx = i;

                // 在滑动窗口内查找最大值
                for (uint16_t j = i - halfWindowSize; j <= i + halfWindowSize; j++)
                {
                    if (fft_outputbuf[j] > maxVal)
                    {
                        maxVal = fft_outputbuf[j];
                        maxIdx = j;
                    }
                }

                // 检查窗口中心点是否为窗口内的最大值
                if (maxIdx == i)
                {
                    // 检查是否与最近的极大值足够远
                    if (peaks[numPeaks] == 0 || (i - peaks[numPeaks - 1]) > windowSize)
                    {
                        peaks[numPeaks] = i;//peaks存的是索引
                        ++numPeaks;

                        //存满以后就退出
                        if (numPeaks == NumPeaks)
                            return;
                    }
                }
            }
        }
    }
    inline void Amplitude_Convert(){
        //计算信号的幅度谱  第一个参数指定了需要计算复数模的数组指针，第二个参数指定了计算结果存放的数组指针，第三个参数是需要计算复数模的数据个数。
        // 分辨率=fs（采样频率）/N（采样点数）  output输出数组，索引*分辨率=频率成分
        arm_cmplx_mag_f32(fft_inputBuff, fft_outputbuf, NUM_SAMPLES / 2);
    }
    void ADCdataToSpectrum(const int32_t ADCdata[]){
        /**将实数序列转为复数序列*/
        for (uint32_t i = 0; i < NUM_SAMPLES; ++i)
        {
            fft_inputBuff[i * 2] = (float) (ADCdata[i] * 3.3 / (255 * AV));//转为实际电压，单位为V
            fft_inputBuff[i * 2 + 1] = 0;                                   //虚部为零
        }
        arm_cfft_f32(&arm_cfft_sR_f32_len1024, fft_inputBuff, IFFTFLAG, BITREVERSE);
    }

    float signalDistortionDegree(uint16_t NumPeaks)
    {
        float sum = 0;
        //从二次谐波开始计算平方和
        for (uint16_t i = 2; i < NumPeaks; i++)
        {
            if (peaks[i] == 0) break;
            sum += fft_outputbuf[peaks[i]] * fft_outputbuf[peaks[i]]; // 直接使用类内数组
        }
        sum /= (fft_outputbuf[peaks[1]] * fft_outputbuf[peaks[1]]);//除以1次谐波的平方
        return thd = sqrtf(sum) * 100;//计算出失真度
    }

    float_t* get_fft_output_normalized() {
        return fft_output_normalized;  // 返回指针
    }
    float_t get_thd() {
        return thd;  // 返回指针
    }

private:
    static constexpr const int NUM_SAMPLES=1024;  //采样点
    static constexpr const float AV=1;            //放大增益
    static constexpr const int NUM_PEAKS=9;       //取9-1个基波谐波成分
    static constexpr const int IFFTFLAG=0;        //正变换
    static constexpr const int BITREVERSE=1;      //逆序排列
    float32_t fft_inputBuff[NUM_SAMPLES * 2]={};     //存储复数的数组
    float32_t fft_outputbuf[NUM_SAMPLES / 2]={};     //存储实数的数组，由于奈奎斯特的特性，需要除以2
    float32_t fft_output_normalized[NUM_PEAKS]={};
    uint16_t peaks[NUM_PEAKS]={};//幅值谱的极大值点
    float power[NUM_PEAKS]={};   //功率谱
    float totalPower=0;         //总功率单位为V^2
    float thd=0;                //失真度

};

// 接口

/* 预编译命令 */
#endif
#endif //FURINA_DSP_HPP