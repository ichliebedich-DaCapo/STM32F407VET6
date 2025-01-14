#ifndef __ESP8266_H
#define __ESP8266_H 			   


#include <string.h>
#include <stdbool.h>
#include "stdint.h"

#if defined ( __CC_ARM)
#pragma anon_unions
#endif

//ESP8266模式选择
typedef enum
{
    STA,
    AP,
    STA_AP  
}ENUM_Net_ModeTypeDef;

//网络传输层协议，枚举类型
typedef enum{
     enumTCP,
     enumUDP,
} ENUM_NetPro_TypeDef;
//连接号，指定为该连接号可以防止其他计算机访问同一端口而发生错误
typedef enum{
    Multiple_ID_0 = 0,
    Multiple_ID_1 = 1,
    Multiple_ID_2 = 2,
    Multiple_ID_3 = 3,
    Multiple_ID_4 = 4,
    Single_ID_0 = 5,
} ENUM_ID_NO_TypeDef;





//初始化和TCP功能函数
void ESP8266_Init(uint32_t bound);
void ESP8266_AT_Test(void);
bool ESP8266_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,uint32_t time);
void ESP8266_Rst(void);
bool ESP8266_Net_Mode_Choose(ENUM_Net_ModeTypeDef enumMode);
bool ESP8266_JoinAP( char * pSSID, char * pPassWord );
bool ESP8266_Enable_MultipleId ( FunctionalState enumEnUnvarnishTx );
bool ESP8266_Link_Server(ENUM_NetPro_TypeDef enumE, char * ip, char * ComNum, ENUM_ID_NO_TypeDef id);
bool ESP8266_SendString(FunctionalState enumEnUnvarnishTx, char * pStr, uint32_t ulStrLength, ENUM_ID_NO_TypeDef ucId );
bool ESP8266_UnvarnishSend ( void );
void ESP8266_ExitUnvarnishSend ( void );
uint8_t ESP8266_Get_LinkStatus ( void );
void USART_printf( USART_TypeDef * USARTx, char * Data, ... );

//MQTT功能函数
bool ESP8266_MQTTUSERCFG( char * pClient_Id, char * pUserName,char * PassWord);
bool ESP8266_MQTTCONN( char * Ip, int Num);
bool ESP8266_MQTTSUB(char * Topic);
bool ESP8266_MQTTPUB( char * Topic,char *temp);
bool ESP8266_MQTTCLEAN(void);
bool MQTT_SendString(char * pTopic,char *temp2);

#endif
