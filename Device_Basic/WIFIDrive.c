/****************************************************************************
* �ļ����� : wifi ģ����� ģ��
* �ļ����� : ���wifi �����������ﱻʵ��
* ע������ :
*****************************************************************************/
#include "WIFIDrive.h"
#include"OledDrive.h"
#include"KeyScan.h"
u8_Pro WIFI_Tx_BUF[4800] = {0};
u8_Pro mesage[30];
u8_Pro WIFI_Rx_BUF[Rx_BUF_MAX_SIZE] = {0};
u8_Pro WIFI_RX_STA = 0;		/*<! Ҫ�� MAX SIZE С�ڵ��� 63 */


/****************************************************************************
* ��������: WIFI ģ���ʼ�� 
* �������� : WIFI ģ���ʼ�����������ﱻִ��
* ���� :void(������ʱ���������������ܲ���)
* ��� :��ʼ��״̬ ���ɹ� 0��ʧ�� 1
* ���� :
*****************************************************************************/
u8_Pro WIFI_Init(void)
{
// #if WIFI_FUNCTION == 1
// 	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
// #endif
	u8_Pro i = 2;
	uart_init(WIFI_Uart,256000);							/*!< ��ʼ�� WiFi ���ʹ��� */
	uart_rx_irq_en(WIFI_Uart);								/*!< ʹ�� ���ڽ����ж� */
	pit_init_ms(PIT3,10);									/*!< ��ʼ�� ��ʱ�� 10ms ǿ�Ʊ�־һ�� ������� */
	pit_close(PIT3);										/*!< �رն�ʱ�� */
	set_vector_handler(PIT3_VECTORn,WIFI_PITIQR_Hander);	/*!< �ض��� �жϷ����� */
	set_vector_handler(UART1_RX_TX_VECTORn,WIFI_Rx_IRQHander);
	enable_irq(UART3_RX_TX_IRQn);
	enable_irq(PIT3_IRQn);
	while(atk_8266_send_cmd("AT","OK",20))					/*!< ���� wifi ��Ӧ���� */
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

// 	while(	WIFI_CMD("AT+CWMODE=2","OK",200))							/*!< ���� wifi ģ��Ϊ AP �� �����ȵ�ģʽ */
// 	{
// 		if(WIFI_CMD("AT+CWMODE?","MODE=2",20))
// 		  break;
// 	}
// //	WIFI_CMD("AT+CWLAP=\"H\"","OK",2000);
// //	WIFI_CMD("AT+CWLAP=4","OK",2000);
// 	while(WIFI_CMD("AT+RST","OK",20)) printf("RST_Defeat");									/*!< ����wifi							 */
// 	// WIFI_CMD("ATE0","OK",20);								/*!< �ر� ���� ���ر� 					 */
// 	while(WIFI_CMD("AT+CWSAP=\"ATK_ESP\",\"123456789\",1,4","OK",500))
// 		DELAY_MS(3000);	/*!< ���� wifi �ȵ� ���� ͨ��  ���ܷ�ʽ	 */
	WIFI_CMD("AT+CIFSR","OK",800);							
	WIFI_CMD("AT+CIPMUX=1","OK",20);							/*!< ����������						   */
	DELAY_MS(500);
	WIFI_CMD("AT+CIPSERVER=1,8086","OK",20);					/*!< ���� Ϊ TCP ������ģʽ ��Ӧ��PC Android ����Ϊ TCP �ͻ���ģʽ						*/
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
	return 0;													/*!< ��ȡ ��������  IP mac ��ַ			*/
}


/****************************************************************************
* ��������: �������е����ݷ��͵� ĳ��������
* �������� : �������е����ݷ��͵�������
* ���� :������  ���鳤��
* ��� :void
* ���� :
*****************************************************************************/
void WIFI_SentArrayToComputer(u8_Pro* ArrayList,u16_Pro ListLength)
{
	u8_Pro temp[20];
// #if WIFI_FUNCTION == 1
// 	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
// #endif
	sprintf((char*)temp,"AT+CIPSEND=0,%d",ListLength);
	WIFI_CMD(temp,"OK",20);
        WIFI_prinft("%s\r\n",ArrayList);

}

/****************************************************************************
* ��������: WIFI ���ڴ�ӡ����
* �������� : ʹ�� printf ��ʽ ��������
* ���� :�� Printf ��ͬ��ʽ
* ��� :void
* ���� :kim_alittle_star
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
* ��������: WiFi ģ�鷢�� wifi  ָ�� ����
* �������� :���� ָ�� CMD 
* ���� :u8_Pro * cmd�� Ack �����ش� ��waittime �ȴ�ʱ��
* ��� :�ɹ� ���� 0 ��ʧ�� ���� 1
* ���� :Kim_alitle_star
*****************************************************************************/
u8_Pro atk_8266_send_cmd(u8_Pro *cmd,u8_Pro* ack,u16_Pro waittime )
{
	u8_Pro res=0; 
	u8_Pro * msg;
	WIFI_prinft("%s\r\n",cmd);
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
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
* ��������: WiFI���� ���� 
* �������� :
* ���� :Ҫ���͵����� ����
* ��� :0 �� �ɹ��� ���� ���ɹ�
* ���� :Kim_alitle_star
*****************************************************************************/
u8_Pro atk_8266_send_data(u8_Pro *data,u8_Pro *ack,u16_Pro waittime)
{
	u8_Pro res=0;
	u8_Pro* msg; 
	WIFI_prinft("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			DELAY_MS(10);
			msg = WIFi_Get_Rx_BUF();
			if(msg != NULL)//���յ��ڴ���Ӧ����
			{
				if(strstr((const char*)msg,(const char*)ack))
				break;//�õ���Ч���� 				
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}

/****************************************************************************
* ��������: WIFI_Rx_IRQHander
* �������� :ʵ�� WIFi ���ڵ� �����ж�
* ���� : void
* ��� : void 
* ���� :Kim_alitle_star
*****************************************************************************/
void WIFI_Rx_IRQHander(void)
{
	u8_Pro res;
//	res =   UART_D_REG(UARTN[WIFI_Uart]);
    if(UART_S1_REG(UARTN[WIFI_Uart]) & UART_S1_RDRF_MASK)   //�������ݼĴ�����
    {
        //�û���Ҫ�����������
		res =   UART_D_REG(UARTN[WIFI_Uart]);
		if((WIFI_RX_STA & 0x80) != 1)		/*<! ��һ�ν��յ����� ��û�б�ȡ�ߣ� */
		{
			if((WIFI_RX_STA & 0x3f) < Rx_BUF_MAX_SIZE)   /*<! ���� ��� */
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
* ��������: WIFI_PITIQR_Hander
* �������� :Wifi ר����ʱ�� �жϴ�����
* ���� : void
* ��� : void 
* ���� :Kim_alitle_star
*****************************************************************************/
void WIFI_PITIQR_Hander(void)
{
	PIT_Flag_Clear(PIT3);
	pit_close(PIT3);
        printf("PIt\n");
	WIFI_RX_STA |= 0x80;
}

/****************************************************************************
* ��������: WIFi_Get_Rx_BUF
* �������� :���� ���� WIFI �յ����ַ���������
* ���� : void
* ��� : u8_Pro* 
* ���� :Kim_alitle_star
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
* ��������: WIFi_Show_Recive_Stirng
* �������� :���� ���� WIFI �յ����ַ���������
* ���� : void
* ��� : u8_Pro* 
* ���� :Kim_alitle_star
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
* ��������: WIFI_Creat_AP
* ��������: ��wifi ��Ϊ�ȵ��豸 
* ��������: void input:SSID wifi����password wifi����
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void WIFI_Creat_AP(const String_Pro SSID,const String_Pro password)
{
  	char sage[35];
	  while(atk_8266_send_cmd("AT","OK",20))					/*!< ���� wifi ��Ӧ���� */
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
	while(	WIFI_CMD("AT+CWMODE=2","OK",200))							/*!< ���� wifi ģ��Ϊ AP �� �����ȵ�ģʽ */
	{
		if(WIFI_CMD("AT+CWMODE?","MODE=2",20))
		  break;
		else
		{
		  	
		}
	}
//	WIFI_CMD("AT+CWLAP=\"H\"","OK",2000);
//	WIFI_CMD("AT+CWLAP=4","OK",2000);
	while(WIFI_CMD("AT+RST","OK",50)) printf("RST_Defeat");									/*!< ����wifi							 */
	DELAY_MS(2000);
	while(	WIFI_CMD("AT+CWMODE=2","OK",200))							/*!< ���� wifi ģ��Ϊ AP �� �����ȵ�ģʽ */
	{
		if(WIFI_CMD("AT+CWMODE?","MODE=2",20))
		  break;
		else
		{
		  	
		}
	}
	while(WIFI_CMD("AT+RST","OK",50)) printf("RST_Defeat");									/*!< ����wifi
	// WIFI_CMD("ATE0","OK",20);	*/
	sprintf(sage,"AT+CWSAP=\"%s\",\"%s\",1,4",SSID,password);							/*!< �ر� ���� ���ر� 					 */
	DELAY_MS(2000);
	while(WIFI_CMD(sage,"OK",500))
	{
		DELAY_MS(1000);	/*!< ���� wifi �ȵ� ���� ͨ��  ���ܷ�ʽ	 */
		printf(sage);
	}
	// WIFI_CMD("AT+CIFSR","OK",2000);								/*!< ��ȡ ��������  IP mac ��ַ			*/
	// WIFI_CMD("AT+CIPMUX=1","OK",20);							/*!< ����������						   */
	// WIFI_CMD("AT+CIPSERVER=1,8086","OK",20);
}

/**********************************************************************************
* ��������: WIFI_STA_Join
* ��������: ����ĳ���ȵ�
* ��������: void input:�ȵ����� �ȵ�����
* �������: void output:�ɹ����� ���� ��0��ʧ�� ���� 0
* ���� :author:@Kim_alittle_star
**********************************************************************************/
u8_Pro WIFI_STA_Join(const String_Pro SSID,const String_Pro pwd)
{
	while(	WIFI_CMD("AT+CWMODE=1","OK",200))							/*!< ���� wifi ģ��Ϊ AP �� �����ȵ�ģʽ */
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











