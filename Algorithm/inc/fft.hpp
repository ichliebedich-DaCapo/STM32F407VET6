//
// Created by DV on 2024/10/14.
//
#include "common_config.h"
#include "arm_math.h"
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
    static void voltageAmplitude_Convert(float32_t outputArray[]);
    static void normalize_to_fundamental(float32_t outputArray[],const uint16_t *peaks, int num_peaks, float32_t *normalized);
    static void find_peaks(const float32_t *fftOutput, uint16_t fftLength, uint16_t *peaks, uint32_t NumPeaks, uint16_t windowSize);
    static inline void Amplitude_Convert(float32_t inputArray[], float32_t outputArray[]);
    static void ADCdataToSpectrum(const int16_t ADCdata[], float32_t fftArray[]);
    static void signalDistortionDegree(const float32_t *fft_Array, const uint16_t *peaks, uint16_t NumPeaks, float *thd);
    static auto FFT_Calculate(float *DATA, int num, int N)->float;

private:
    static constexpr const int NUM_SAMPLES=1024;  //采样点
    static constexpr const float AV=1;            //放大增益
    static constexpr const int NUM_PEAKS=9;       //取9-1个基波谐波成分
    static constexpr const int IFFTFLAG=0;        //正变换
    static constexpr const int BITREVERSE=1;      //逆序排列
    static int16_t ADC_Data[1024];                       //ADC采样数据
    static float32_t fft_inputBuff[NUM_SAMPLES * 2];     //存储复数的数组
    static float32_t fft_outputbuf[NUM_SAMPLES / 2];     //存储实数的数组，由于奈奎斯特的特性，需要除以2
    static uint16_t peaks[NUM_PEAKS];//幅值谱的极大值点
    static float power[NUM_PEAKS];   //功率谱
    static float totalPower;         //总功率单位为V^2
    static float thd;                //失真度
    volatile bool waitFPGAData_Flag = true;//用于检测是否需要等待

};

// 接口

/* 预编译命令 */
#endif
#endif //FURINA_DSP_HPP