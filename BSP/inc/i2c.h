//
// Created by DV on 2025/2/4.
//

#ifndef FURINA_I2C_H
#define FURINA_I2C_H
#include "App_Conf.h"



#ifdef __cplusplus
extern "C" {
#endif

void I2C1_Init(); // I2C1��ʼ��
void i2c_init(void);                // ��ʼ��I2CӲ����GPIO��ʱ�ӡ����ʵȣ�
void i2c_start(void);               // ������ʼ������START��
void i2c_stop(void);                // ����ֹͣ������STOP��

//����AIԭ��������uint_8
void i2c_send_byte(int data);   // ����һ���ֽ�����
int i2c_read_byte(void);        // ��ȡһ���ֽ�����

void i2c_wait_ack(void);            // �ȴ��ӻ�Ӧ��ACK��
void i2c_send_ack(void);            // ��������ACK
void i2c_send_nack(void);           // ��������NACK


#ifdef __cplusplus
}
#endif

#endif //FURINA_I2C_H