//#include "mqtt.h"
//#include "esp8266.h"
//
//
//void ESP8266_STA_MQTTClient_Test(void)
//{
//	  char str[1024];
//		printf("正在配置ESP8266参数\r\n");
//    ESP8266_AT_Test();//恢复出厂默认模式
//    ESP8266_Net_Mode_Choose(STA);
//    while(!ESP8266_JoinAP(User_ESP8266_SSID, User_ESP8266_PWD));
//	  ESP8266_MQTTUSERCFG(User_ESP8266_client_id,User_ESP8266_username,User_ESP8266_password);
//	  ESP8266_MQTTCONN( User_ESP8266_MQTTServer_IP, User_ESP8266_MQTTServer_PORT);
//	  ESP8266_MQTTSUB( User_ESP8266_MQTTServer_Topic);
//		printf("\r\nMQTT配置完成");
//		while(1)
//		{
//			  sprintf(str,"aithinker");//格式化发送字符串到MQTT服务器
//			  MQTT_SendString (User_ESP8266_MQTTServer_Topic,str);//发送数据到MQTT服务器
//		}
//}
