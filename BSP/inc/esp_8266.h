#ifndef FURINA_ESP_8266_H
#define FURINA_ESP_8266_H
#include <bsp_config.h>
#ifdef __cplusplus
extern "C" {
#endif


#define ESP8266_Result_OK  1
#define ESP8266_Result_Err  2

/*
 * ESP8266函数
 */
uint16_t  ESP8266_Init(void);
uint16_t ESP8266_wifi_connect(void);
void ESP8266_mqtt_connect(void);
void ESP8266_reset();
void send_temp();
#ifdef __cplusplus
}
#endif
#endif //FURINA_ESP_8266_H
