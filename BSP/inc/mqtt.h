#ifndef __MQTT_H
#define __MQTT_H 			   



/*
*以下参数需要用户自行修改才能测试用过
*/


#define User_ESP8266_SSID     "miot_default"    //wifi名
#define User_ESP8266_PWD      "123456789x"      //wifi密码

#define User_ESP8266_client_id    "aithinker"   //MQTTclientID 用于标志client身份  最长256字节
#define User_ESP8266_username     "admin"						//用于登录 MQTT 服务器 的 username, 最长 64 字节	
#define User_ESP8266_password			"public"          //用于登录 MQTT 服务器 的 password, 最长 64 字节
#define User_ESP8266_MQTTServer_IP     "192.168.31.16"     //MQTT本地服务器IP
#define User_ESP8266_MQTTServer_PORT   1883     //服务器端口号
#define User_ESP8266_MQTTServer_Topic  "topic"    //订阅MQTT主题

void ESP8266_STA_MQTTClient_Test(void);

#endif
