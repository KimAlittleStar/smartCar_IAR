#ifndef _WIFIDRIVE_H_
#define _WIFIDRIVE_H_


#include "Global_Var.h"
/*------------------------宏定义  区----------------------*/

#define WIFI_Uart UART1


/*------------------------函数声明区----------------------*/

u8_Pro WIFI_Init(void);

void WIFI_SentArrayToComputer(u8_Pro* ArrayList,u16_Pro ListLength);

void WIFI_prinft(char* fmt,...);

u8_Pro atk_8266_send_cmd(u8_Pro *cmd,u8_Pro* ack,u16_Pro waittime );
u8_Pro atk_8266_send_data(u8_Pro *data,u8_Pro *ack,u16_Pro waittime);
u8_Pro* WIFi_Get_Rx_BUF(void);
void WIFI_PITIQR_Hander(void);
void WIFI_Rx_IRQHander(void);
void WIFi_Show_Recive_Stirng(void);
void WIFI_Creat_AP(const String_Pro SSID,const String_Pro password);
u8_Pro WIFI_STA_Join(const String_Pro SSID,const String_Pro pwd);


#define WIFI_CMD atk_8266_send_cmd
#define WIFI_DATAS atk_8266_send_data


#endif

