//
// Created by DV on 2025/2/4.
//

#ifndef FURINA_I2C_H
#define FURINA_I2C_H
#include "App_Conf.h"



#ifdef __cplusplus
extern "C" {
#endif

void I2C1_Init(); // I2C1初始化
void i2c_init(void);                // 初始化I2C硬件（GPIO、时钟、速率等）
void i2c_start(void);               // 发送起始条件（START）
void i2c_stop(void);                // 发送停止条件（STOP）

//这里AI原来给的是uint_8
void i2c_send_byte(int data);   // 发送一个字节数据
int i2c_read_byte(void);        // 读取一个字节数据

void i2c_wait_ack(void);            // 等待从机应答（ACK）
void i2c_send_ack(void);            // 主机发送ACK
void i2c_send_nack(void);           // 主机发送NACK


#ifdef __cplusplus
}
#endif

#endif //FURINA_I2C_H