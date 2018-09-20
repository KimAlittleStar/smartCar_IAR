/****************************************************************************
* 文件名称：main.c
* 飞思卡尔 主函数文件
* 
*****************************************************************************/

#include "include.h"
#include "_NVIC.h"
extern u8_Pro *BBmp;

/****************************************************************************
* 函数名称: main(void)
* 函数功能 : 系统主函数
* 输入 :void
* 输出 :void	
* 作者 : kim_alittle_star
*****************************************************************************/
int main(void)
{
	Project_Init();		/* !< 所有函数初始化将在这里被执行 */
	Project_StarUp();	/* !< 项目真正 执行函数			 */
	while(1)
	{}
}


/****************************************************************************
* 函数名称: Project_Init()
* 函数功能 : 所有的函数初始化将在这里被执行
* 输入 :
* 输出 :
* 作者 :
*****************************************************************************/
void Project_Init(void)
{
	
	/* !< 所有的函数初始化将在这里被执行 */
	NVIC_SET_init();
	Debug_IOTest_Init();
	OLED_Init();
	Motor_Init();
	Servo_Init();
	Encord_Init();
	KeyScan_Init();
	Cell_Init();
	uart_init(Kim_UsartPort,Kim_BAUD);
	printf("Uart OK \n");
	Debug_IOTest_Star();
	PID_NVIC_init();
	OV_Init();
//	Debug_ImgAcq();
	while(WIFI_Init())
	{
		printf("Wifi_defeat\n");
		DELAY_MS(500);
		
	}

	Encord_GetSpeed_Close();

	Motor_div.const_D = 1000;
	Motor_div.const_I = 0;
	Motor_div.const_P = 60;
	Motor_div.error_s[0] = 0;
	Motor_div.error_s[1] = 0;
	Motor_div.error_s[2] = 0;
	Motor_div.error_sum = 0;
	Motor_div.realPoint = 0;
	Motor_div.setPoint = 0;

	Servo_div.const_D = 0;
	Servo_div.const_I = 0;
	Servo_div.const_P = 240;
	Servo_div.error_s[0] = 0;
	Servo_div.error_s[1] = 0;
	Servo_div.error_s[2] = 0;
	Servo_div.error_sum = 0;
	Servo_div.realPoint = 0;
	Servo_div.setPoint = 0;


	
	printf("Init_ok\n");
}

/****************************************************************************
* 函数名称: Project_StarUp()
* 函数功能 : 项目真正的执行函数入口
* 输入 :
* 输出 :
* 作者 :
*****************************************************************************/
void Project_StarUp(void)
{
  
#if RC_MODE==0
	String_Pro wifimsg;
	String_Pro pp;
	u32_Pro temp = 0;
	u32_Pro const_p = 0;
	u16_Pro Motortmep = 500;
	u32_Pro databuff[1000] = 0;
//	Motor_Set_Speed(400);
	OLED_ResetGRAM();
	OLED_Show_String(0,0,"OK!",charSize12);
	OLED_Refrech_Gram();
	DELAY_MS(1000);
	Motor_div.setPoint = 400;
	// while(Key_Of_Cancel(KEY_MODE_Once) != 1)
	// {
	// 	camera_get_img();
	// 	OV_Sending(OV_7725Rx_Img,600);
	// }
	while(1)
	{
		wifimsg = WIFi_Get_Rx_BUF();
		if(wifimsg!= NULL)
		{
		 	printf(wifimsg);
		}
		if(wifimsg != NULL)
		{

			/*!< 执行测速 惯性数据测量 */
			Encord_GetSpeed_Prepare();
			if(strstr(wifimsg,"stst"))
			{
				Motor_Set_Speed(600);
				while(Encord_GetMotorTime() < 30)
				{
					Motortmep = Motor_GetSpeed_Of_Now();
					Motortmep+=5;
					Motor_Set_Speed(MOTOR_CH1_DUTY);
					DELAY_MS(400);
					Motor_Set_Speed(Motortmep);
					DELAY_MS(500);
					temp = Encord_GetMotorTime();
					OLED_ResetGRAM();
					OLED_Show_Num(0,0,Motor_GetSpeed_Of_Now(),24);
					OLED_Show_Num(0,24,temp,charSize24);
					OLED_Refrech_Gram();
				}
			
			
				for(temp = 0;temp<500;temp++)
				{
					databuff[temp] = Encord_GetMotorTime();
					PID_Motor_SpeedSet();
					DELAY_MS(10);
				}
	//			while(keyof)
				OLED_ResetGRAM();
				OLED_Show_String(0,0,"OK! 5 S well sending data",charSize16);
				OLED_Refrech_Gram();
				DELAY_MS(1000);
				Motor_Set_Speed(MOTOR_CH1_DUTY);
				// for(temp = 0;temp < 500;temp++)
				// {
				// 	senddata(temp,databuff[temp]);
				// 	DELAY_MS(50);
				// }	
				senddata(500,databuff);
			}else
				{
					Motor_div.const_P = Debug_getNumFormStr((const String_Pro)wifimsg,
										"PIDP",
										Motor_div.const_P		);
					Servo_Set_Angle(
						Debug_getNumFormStr(
							(const String_Pro)wifimsg,
							"DIR",
							Servo_Angle_Now
						)
					);
					Motor_div.setPoint = Debug_getNumFormStr(
										(const String_Pro)wifimsg,
										"SET",
										Motor_div.setPoint
															  );
					Motor_div.const_D = Debug_getNumFormStr(
										(const String_Pro)wifimsg,
										"PIDD",
										Motor_div.const_D
															  );
				}
		}
		DELAY_MS(50);
		OLED_ResetGRAM();
		/*!< P 参数 */
		OLED_Show_String(0,0,"P:",charSize12);
		OLED_Show_Num(12,0,Motor_div.const_P,charSize12);
		/*!<  期望 编码器转速 */
		OLED_Show_String(48,0,"R",charSize12);
		OLED_Show_Num(54,0,Motor_div.setPoint,charSize12);
		/*!< 编码器参数 */
		OLED_Show_String(0,12,"Bma:",charSize12);
		OLED_Show_Num(24,12,Encord_GetMotorTime(),charSize12);
		/*!< 舵机参数 */
		OLED_Show_String(0,24,"Dir:",charSize12);
		OLED_Show_Num(24,24,Servo_Angle_Now,charSize12);
		/*!< D 参数 */
		OLED_Show_String(48,24,"D:",charSize12);
		OLED_Show_Num(60,24,Motor_div.const_D,charSize12);
		/*!< 电机参数 */
		OLED_Show_String(0,36,"PWM:",charSize12);
		OLED_Show_Num(24,36,Motor_Speed_Now,charSize12);
		/*!< ADC 参数 */
		OLED_Show_String(0,48,"ADC:",charSize12);
		OLED_Show_Num(24,48,Cell_GetBatteryPer() * 3,charSize12);
		OLED_Refrech_Gram();
		// temp = Encord_GetMotorTime();
		// OLED_ResetGRAM();
		// OLED_Show_Num(0,0,Motor_GetSpeed_Of_Now(),24);
		// OLED_Show_Num(0,24,temp,charSize24);
		// OLED_Refrech_Gram();
// 		if(temp>500)
// 		{
// //			OLED_Show_Num(48,0,Motor_GetSpeed_Of_Now(),charSize16);
// 			if(temp>15000)
// 			{
// 				Motor_Reduce_Speed(10);
// 			}else if(temp <10000) {
// 				Motor_Increase_Speed(10);
// 			}
// 		}
// 		else
// 		{
// 			Motortmep = Motor_GetSpeed_Of_Now();
// 			Motor_Set_Speed(300);
// 			DELAY_MS(1000);
// 			Motor_Set_Speed(Motortmep+20);
// 		}
// 	//	Motor_Increase_Speed(50);
// 		DELAY_MS(1000);
// 		// camera_get_img();
// 		// OV_Sending(OV_7725Rx_Img,600);
	}
#elif RC_MODE==1
//	Project_RC();
#endif

		/*!< 即将书写的函数  */
	while(1)
	{
	  	Debug_OLED_FristPage();
		switch(Debug_OLED_PageNum)
		{
			case 0:	Debug_RcMode();	 break ;/*!<  RC_MODE 模式 */
			case 1: Debug_SetParam();break ;/*!< wifi调试模式 */
			case 2: Debug_ImgAcq();  break ;/*!< 图像采集模式 */
			case 3: Debug_Set(); 	break;	/*!< 参数设置模式 */
		}
	}
		
}
#if RC_MODE == 1

void Project_RC(void)
{
	String_Pro RC_string;
	Encord_GetSpeed_Prepare();
	Motor_div.setPoint = 0;
	Motor_div.const_D = 1000;
	Motor_div.const_P = 60;
	while(1)
	{
		Key_Of_Cancel(KEY_MODE_Once);
		Key_Of_Confirm(KEY_MODE_Once);
		Key_Of_Down(KEY_MODE_Once);
		Key_Of_Up(KEY_MODE_Once);
		/*!< 按键扫描是否修改PID参数 */

		RC_string = WIFi_Get_Rx_BUF();

		if(RC_string != NULL)
		{
			/*!< 执行加速减速摆舵等处理 */
			if(strstr(RC_string,GOHADE))
			{
				/*!< 前进 */
				
				if(Encord_GetMotorTime() < 30)
				{
					Motor_Set_Speed(600);
					DELAY_MS(200);
				}
				Motor_div.setPoint = 150;
				
			}else if(strstr(RC_string ,"bb")){
				/*!< 停止 */
				Motor_Set_Speed(MOTOR_CH1_DUTY);
				Motor_div.setPoint = 0;
			}
			if(strstr(RC_string,LEFT))
			{
				/*!< 左转 */
				
				Servo_Set_Angle(Servo_Default_Angle-30);
			}else if(strstr(RC_string,RIGHT)){
				/*!< 右转 */
				Servo_Set_Angle(Servo_Default_Angle+30);
			}else if(strstr(RC_string,"dd")){
				/*!< 直行 */
				Servo_Set_Angle(Servo_Default_Angle);
			}

		}else
		{
			PID_Motor_SpeedSet();
			/*!< 执行匀速或者停止处理 */
			// Motor_Set_Speed(MOTOR_CH1_DUTY);
			// Servo_Set_Angle(Servo_Default_Angle);
		}

		/*!< 整定正方向 */
		Servo_Default_Angle = Debug_getNumFormStr(RC_string,"DDIR",Servo_Default_Angle);

		/*!< 显示数据到 OLED */
		OLED_Show_Num(0,0,Motor_GetSpeed_Of_Now(),charSize16);
		OLED_Show_Num(0,16,Servo_Angle_Now,charSize16);
		OLED_Show_Num(0,32,Encord_GetMotorTime(),charSize16);
		OLED_Show_Num(50,0,Motor_div.const_D,charSize16);
		OLED_Show_Num(16,50,Motor_div.const_P,charSize12);
		OLED_Refrech_Gram();
		OLED_ResetGRAM();
		DELAY_MS(20);

	}



#if 0
	u16_Pro angleTmmp;
	String_Pro rc_cmd;
	while(1)
	{
		rc_cmd = WIFi_Get_Rx_BUF();
//		
//		WIFi_Show_Recive_Stirng();
		if(rc_cmd != NULL)
		{
			printf(rc_cmd);
//		  	WIFI_SentArrayToComputer(rc_cmd,30);
			if(strstr((const char*) rc_cmd,(const char*) GOHADE ))
			{
				/*!< 向前运动 */
				RC_Motor_go();
			}else if(strstr((const char*) rc_cmd,(const char*)BACK))
				{
					/*!< 向后运动 */
					Motor_Set_Speed(300);
				}else
				{
					
				}
			if(strstr((const char*) rc_cmd,(const char*) RIGHT))
			{
				/*!< 向右运动 */
				Servo_Set_Angle(Servo_Default_Angle);
				Servo_SetRight_Angle(38);
			}else if(strstr((const char*) rc_cmd,(const char*) LEFT))
				{
					/*!< 向左运动 */
					Servo_Set_Angle(Servo_Default_Angle);
					Servo_SetLeft_Angle(38);
				}else
				{
		//			Servo_Set_Angle(Servo_Default_Angle);
				}

		}else 
		{
			if(angleTmmp>=1000)
			{
				angleTmmp = 0;
				Servo_Set_Angle((Servo_Angle_Now<<1)/3+Servo_Default_Angle/3);
			}else
			{
				angleTmmp++;
			}
			if(Encord_GetMotorTime() > 500)
			{
			if(Encord_GetMotorTime()>15000)
			{
				if(Encord_GetMotorTime()>30000)
				{
					Motor_Reduce_Speed(3);
				}else
				{
					Motor_Reduce_Speed(1);
				}
				
			}else if(Encord_GetMotorTime() <10000) 
			{
				if(Encord_GetMotorTime()<5000)
				{
					Motor_Increase_Speed(3);
				}else
				{
					Motor_Increase_Speed(1);
					Motor_FeedBack_SpeedTime
				}
			}
			}
		}
#endif				/*!< 折叠这一段BUG 代码  */
		
				
	

}

void RC_Motor_go(void)
{
	u8_Pro i;
	while(Encord_GetMotorTime() < 500)
	{
		Motor_Increase_Speed(10);
		DELAY_MS(100);
	}
	for(i = 0;i<100;i++)
	{
		if(Encord_GetMotorTime()>15000)
		{
			if(Encord_GetMotorTime()>30000)
			{
				Motor_Reduce_Speed(10);
			}else
			{
				Motor_Reduce_Speed(3);
			}
			
		}else if(Encord_GetMotorTime() <10000) 
		{
			if(Encord_GetMotorTime()<5000)
			{
				Motor_Increase_Speed(10);
			}else
			{
				Motor_Increase_Speed(3);
			}
		}
		DELAY_MS(500);
	}
	
}

#elif RC_MODE ==0

void senddata(u16_Pro count,u32_Pro* Value_Array)
{
	u8_Pro mess[25];
	u16_Pro i;
	sprintf((char*)mess,"AT+CIPSEND=0,%d",4*count+1);
	WIFI_CMD(mess,"OK",20);
//    WIFI_prinft("%s\r\n",ArrayList);
	WIFI_prinft("[");
	if(*Value_Array < 10)
		{
			sprintf(mess,"00%d",*Value_Array);
		}else if (*Value_Array <100)
		{
			sprintf(mess,"0%d",*Value_Array);
		}else if(*Value_Array <1000)
		{
			sprintf(mess,"%d",*Value_Array);
		}else
		{
		  	sprintf(mess,"%s","000");
		}
		Value_Array++;
		WIFI_prinft("%s",mess);
	for(i = 1;i<count;i++)
	{
		if(*Value_Array < 10)
		{
			sprintf(mess,",00%d",*Value_Array);
		}else if (*Value_Array <100)
		{
			sprintf(mess,",0%d",*Value_Array);
		}else if(*Value_Array <1000)
		{
			sprintf(mess,",%d",*Value_Array);
		}else
		{
		  	sprintf(mess,",%s","000");
		}
		Value_Array++;
		WIFI_prinft("%s",mess);
	}
	WIFI_prinft("]/r/n");
	// sprintf(mess,"%d,",Encord_Time_Value);
	// WIFI_SentArrayToComputer(mess,strlen((const char*)mess));
	DELAY_MS(10);
}


#endif