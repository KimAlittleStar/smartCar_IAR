/****************************************************************************
* 文件名称 : wifi 模块控制 模块
* 文件功能 : 相关wifi 函数将在这里被实现
* 注意事项 :
*****************************************************************************/
#include "WIFIDrive.h"
#include"OledDrive.h"
#include"KeyScan.h"
u8_Pro WIFI_Tx_BUF[4800] = {0};
u8_Pro mesage[30];
u8_Pro WIFI_Rx_BUF[Rx_BUF_MAX_SIZE] = {0};
u8_Pro WIFI_RX_STA = 0;		/*<! 要求 MAX SIZE 小于等于 63 */


/****************************************************************************
* 函数名称: WIFI 模块初始化 
* 函数功能 : WIFI 模块初始化程序将在这里被执行
* 输入 :void(传参暂时不明，但函数功能不变)
* 输出 :初始化状态 ：成功 0，失败 1
* 作者 :
*****************************************************************************/
u8_Pro WIFI_Init(void)
{
// #if WIFI_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	u8_Pro i = 2;
	uart_init(WIFI_Uart,256000);							/*!< 初始化 WiFi 发送串口 */
	uart_rx_irq_en(WIFI_Uart);								/*!< 使能 串口接收中断 */
	pit_init_ms(PIT3,10);									/*!< 初始化 定时器 10ms 强制标志一次 接收完成 */
	pit_close(PIT3);										/*!< 关闭定时器 */
	set_vector_handler(PIT3_VECTORn,WIFI_PITIQR_Hander);	/*!< 重定向定 中断服务函数 */
	set_vector_handler(UART1_RX_TX_VECTORn,WIFI_Rx_IRQHander);
	enable_irq(UART3_RX_TX_IRQn);
	enable_irq(PIT3_IRQn);
	while(atk_8266_send_cmd("AT","OK",20))					/*!< 测试 wifi 响应函数 */
	{
		WIFI_prinft("+");
		DELAY_MS(30);
		WIFI_prinft("+");
		DELAY_MS(30);
		WIFI_prinft("+");
		DELAY_MS(500);
		atk_8266_send_cmd("AT+CIPMODE=0","OK",20);
		printf("defeat\r\n");
		if(Key_Of_Cancel(KEY_MODE_Once) == 1)
		{
			
			return 0;
		}
	}
	
//	if(WIFI_STA_Join("H3","123454321") == 0)
//	{
//		OLED_Show_String(0,0,"H3:192.168.43.124:8086",charSize12);
//		OLED_Refrech_Gram();
//		return 0;
//	}else
//	{
		WIFI_Creat_AP("ATK_ESP","123456789");
		OLED_Show_String(0,0,"SAP:192.168.4.1:8086",charSize12);
		OLED_Refrech_Gram();

//	}

// 	while(	WIFI_CMD("AT+CWMODE=2","OK",200))							/*!< 设置 wifi 模块为 AP 即 创建热点模式 */
// 	{
// 		if(WIFI_CMD("AT+CWMODE?","MODE=2",20))
// 		  break;
// 	}
// //	WIFI_CMD("AT+CWLAP=\"H\"","OK",2000);
// //	WIFI_CMD("AT+CWLAP=4","OK",2000);
// 	while(WIFI_CMD("AT+RST","OK",20)) printf("RST_Defeat");									/*!< 重启wifi							 */
// 	// WIFI_CMD("ATE0","OK",20);								/*!< 关闭 回显 ，关闭 					 */
// 	while(WIFI_CMD("AT+CWSAP=\"ATK_ESP\",\"123456789\",1,4","OK",500))
// 		DELAY_MS(3000);	/*!< 设置 wifi 热点 密码 通道  加密方式	 */
	WIFI_CMD("AT+CIFSR","OK",800);							
	WIFI_CMD("AT+CIPMUX=1","OK",20);							/*!< 开启多连接						   */
	DELAY_MS(500);
	WIFI_CMD("AT+CIPSERVER=1,8086","OK",20);					/*!< 设置 为 TCP 服务器模式 对应的PC Android 端设为 TCP 客户端模式						*/
//	WIFI_CMD("AT+CIPSTART=\"UDP\",\"192.168.23.1\",8086,0","OK",50);
	DELAY_MS(500);
	WIFI_CMD("ATE0","OK",20);
	while(Key_Of_Confirm(KEY_MODE_Once) == 0)
	{
		DELAY_MS(50);
		if(strstr((const char*)WIFi_Get_Rx_BUF(),"Link"))
		break;
	}
//	}
	return 0;													/*!< 获取 服务器的  IP mac 地址			*/
}


/****************************************************************************
* 函数名称: 将数组中的数据发送到 某个电脑上
* 函数功能 : 将数组中的数据发送到电脑上
* 输入 :数组名  数组长度
* 输出 :void
* 作者 :
*****************************************************************************/
void WIFI_SentArrayToComputer(u8_Pro* ArrayList,u16_Pro ListLength)
{
	u8_Pro temp[20];
// #if WIFI_FUNCTION == 1
// 	#warning "函数未重写"		/*!< 重写后请删除此段 */
// #endif
	sprintf((char*)temp,"AT+CIPSEND=0,%d",ListLength);
	WIFI_CMD(temp,"OK",20);
        WIFI_prinft("%s\r\n",ArrayList);

}

/****************************************************************************
* 函数名称: WIFI 串口打印函数
* 函数功能 : 使用 printf 格式 发送数据
* 输入 :与 Printf 相同格式
* 输出 :void
* 作者 :kim_alittle_star
*****************************************************************************/
void WIFI_prinft(char* fmt,...)
{
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)WIFI_Tx_BUF,fmt,ap);
	va_end(ap);
	uart_putstr(WIFI_Uart,(const u8_Pro*)WIFI_Tx_BUF);
}

/****************************************************************************
* 函数名称: WiFi 模块发送 wifi  指令 函数
* 函数功能 :设置 指令 CMD 
* 输入 :u8_Pro * cmd， Ack 期望回答 ，waittime 等待时间
* 输出 :成功 返回 0 ，失败 返回 1
* 作者 :Kim_alitle_star
*****************************************************************************/
u8_Pro atk_8266_send_cmd(u8_Pro *cmd,u8_Pro* ack,u16_Pro waittime )
{
	u8_Pro res=0; 
	u8_Pro * msg;
	WIFI_prinft("%s\r\n",cmd);
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			DELAY_MS(10);
			msg = WIFi_Get_Rx_BUF();		
			if(msg != NULL)
			{
				printf((const char*)msg);
				if(strstr((const char*)msg,(const char*)ack))
				break;
			}
		}
		
		if(waittime==0)res=1; 
	}
	return res;
}

/****************************************************************************
* 函数名称: WiFI发动 数据 
* 函数功能 :
* 输入 :要发送的数据 数组
* 输出 :0 即 成功， 非零 不成功
* 作者 :Kim_alitle_star
*****************************************************************************/
u8_Pro atk_8266_send_data(u8_Pro *data,u8_Pro *ack,u16_Pro waittime)
{
	u8_Pro res=0;
	u8_Pro* msg; 
	WIFI_prinft("%s",data);	//发送命令
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			DELAY_MS(10);
			msg = WIFi_Get_Rx_BUF();
			if(msg != NULL)//接收到期待的应答结果
			{
				if(strstr((const char*)msg,(const char*)ack))
				break;//得到有效数据 				
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

/****************************************************************************
* 函数名称: WIFI_Rx_IRQHander
* 函数功能 :实现 WIFi 串口的 接收中断
* 输入 : void
* 输出 : void 
* 作者 :Kim_alitle_star
*****************************************************************************/
void WIFI_Rx_IRQHander(void)
{
	u8_Pro res;
//	res =   UART_D_REG(UARTN[WIFI_Uart]);
    if(UART_S1_REG(UARTN[WIFI_Uart]) & UART_S1_RDRF_MASK)   //接收数据寄存器满
    {
        //用户需要处理接收数据
		res =   UART_D_REG(UARTN[WIFI_Uart]);
		if((WIFI_RX_STA & 0x80) != 1)		/*<! 上一次接收的数据 有没有被取走？ */
		{
			if((WIFI_RX_STA & 0x3f) < Rx_BUF_MAX_SIZE)   /*<! 有无 溢出 */
			{
				pit_time_start(PIT3);
				WIFI_Rx_BUF[(WIFI_RX_STA&0x3f)] = res;
				WIFI_RX_STA++;
			}else
			{
				WIFI_RX_STA |= 0x80;
			}
		}
    }
 //               printf("%d\n",res);
}

/****************************************************************************
* 函数名称: WIFI_PITIQR_Hander
* 函数功能 :Wifi 专属定时器 中断处理函数
* 输入 : void
* 输出 : void 
* 作者 :Kim_alitle_star
*****************************************************************************/
void WIFI_PITIQR_Hander(void)
{
	PIT_Flag_Clear(PIT3);
	pit_close(PIT3);
        printf("PIt\n");
	WIFI_RX_STA |= 0x80;
}

/****************************************************************************
* 函数名称: WIFi_Get_Rx_BUF
* 函数功能 :返回 接收 WIFI 收到的字符串的数组
* 输入 : void
* 输出 : u8_Pro* 
* 作者 :Kim_alitle_star
*****************************************************************************/
u8_Pro* WIFi_Get_Rx_BUF(void)
{
	if(WIFI_RX_STA)
	{
		WIFI_Rx_BUF[(WIFI_RX_STA & 0x3f)+1] = '\0';
		WIFI_RX_STA = 0;
		return  WIFI_Rx_BUF;
	}else
	{
		return NULL;
	}
}
/****************************************************************************
* 函数名称: WIFi_Show_Recive_Stirng
* 函数功能 :返回 接收 WIFI 收到的字符串的数组
* 输入 : void
* 输出 : u8_Pro* 
* 作者 :Kim_alitle_star
*****************************************************************************/
void WIFi_Show_Recive_Stirng(void)
{
	String_Pro str;
	str = WIFi_Get_Rx_BUF();
	if(str!= NULL)
	{
		printf(str);
	}
}

/**********************************************************************************
* 函数名称: WIFI_Creat_AP
* 函数功能: 将wifi 作为热点设备 
* 函数输入: void input:SSID wifi名，password wifi密码
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void WIFI_Creat_AP(const String_Pro SSID,const String_Pro password)
{
  	char sage[35];
	  while(atk_8266_send_cmd("AT","OK",20))					/*!< 测试 wifi 响应函数 */
	{
		WIFI_prinft("+");
		DELAY_MS(30);
		WIFI_prinft("+");
		DELAY_MS(30);
		WIFI_prinft("+");
		DELAY_MS(500);
		atk_8266_send_cmd("AT+CIPMODE=0","OK",20);
		printf("defeat\r\n");
	}
	while(	WIFI_CMD("AT+CWMODE=2","OK",200))							/*!< 设置 wifi 模块为 AP 即 创建热点模式 */
	{
		if(WIFI_CMD("AT+CWMODE?","MODE=2",20))
		  break;
		else
		{
		  	
		}
	}
//	WIFI_CMD("AT+CWLAP=\"H\"","OK",2000);
//	WIFI_CMD("AT+CWLAP=4","OK",2000);
	while(WIFI_CMD("AT+RST","OK",50)) printf("RST_Defeat");									/*!< 重启wifi							 */
	DELAY_MS(2000);
	while(	WIFI_CMD("AT+CWMODE=2","OK",200))							/*!< 设置 wifi 模块为 AP 即 创建热点模式 */
	{
		if(WIFI_CMD("AT+CWMODE?","MODE=2",20))
		  break;
		else
		{
		  	
		}
	}
	while(WIFI_CMD("AT+RST","OK",50)) printf("RST_Defeat");									/*!< 重启wifi
	// WIFI_CMD("ATE0","OK",20);	*/
	sprintf(sage,"AT+CWSAP=\"%s\",\"%s\",1,4",SSID,password);							/*!< 关闭 回显 ，关闭 					 */
	DELAY_MS(2000);
	while(WIFI_CMD(sage,"OK",500))
	{
		DELAY_MS(1000);	/*!< 设置 wifi 热点 密码 通道  加密方式	 */
		printf(sage);
	}
	// WIFI_CMD("AT+CIFSR","OK",2000);								/*!< 获取 服务器的  IP mac 地址			*/
	// WIFI_CMD("AT+CIPMUX=1","OK",20);							/*!< 开启多连接						   */
	// WIFI_CMD("AT+CIPSERVER=1,8086","OK",20);
}

/**********************************************************************************
* 函数名称: WIFI_STA_Join
* 函数功能: 加入某个热点
* 函数输入: void input:热点名称 热点密码
* 函数输出: void output:成功加入 返回 ！0，失败 返回 0
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
u8_Pro WIFI_STA_Join(const String_Pro SSID,const String_Pro pwd)
{
	while(	WIFI_CMD("AT+CWMODE=1","OK",200))							/*!< 设置 wifi 模块为 AP 即 创建热点模式 */
	{
		if(WIFI_CMD("AT+CWMODE?","MODE=1",20))
		  break;
	}
	while(WIFI_CMD("AT+RST","OK",500)) printf("RST_Defeat");
	sprintf(mesage,"AT+CWLAP=\"%s\"",SSID);
	if(WIFI_CMD(mesage,"OK",800) != 0)
	{
	  	DELAY_MS(10);
		sprintf(mesage,"AT+CWJAP=\"%s\",\"%s\"",SSID,pwd);
		return WIFI_CMD(mesage,"OK",600);
	}
	else{
		return 0;
	}	
}











