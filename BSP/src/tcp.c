#include "tcp.h"
#include "esp8266.h"
#include "stdio.h"
#include "string.h"

volatile unsigned char TcpClosedFlag = 0;
extern uint8_t Uart_RecvFlag(void);

void ESP8266_STA_TCPClient_Test(void)
{
    unsigned char res;

    char str[100]={0};
    ESP8266_AT_Test();
//    ESP8266_Send_AT_Cmd("AT+RESTORE","OK",NULL,500);
    HAL_Delay(1000);
    ESP8266_Net_Mode_Choose(STA);
    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));//运行到这就连上热点了

//    ESP8266_Enable_MultipleId ( DISABLE );
//    while(!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0));
//    while(!ESP8266_UnvarnishSend());
//    while ( 1 )
//    {
//        sprintf (str,"杭州光子物联科技有限公司\r\n" );//格式化发送字符串到TCP服务器
//        ESP8266_SendString ( ENABLE, str, 0, Single_ID_0 );
//        HAL_Delay(200);
//        Uart_RecvFlag();//接收数据
//        if(TcpClosedFlag) //判断是否失去连接
//        {
//            ESP8266_ExitUnvarnishSend(); //退出透传模式
//            do
//            {
//                res = ESP8266_Get_LinkStatus();     //获取连接状态
//            }
//            while(!res);
//
//            if(res == 4)                     //确认失去连接，重连
//            {
//                while (!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD ) );
//                while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TCPServer_IP, User_ESP8266_TCPServer_PORT, Single_ID_0 ) );
//            }
//            while(!ESP8266_UnvarnishSend());
//        }
//    }
}
