/****************************************************************************
* �ļ����� : Oled ��Ļ ҳ����ʾ
* �ļ����� : ��д��Ļҳ�湦�� ��ҳ������Ϣ �޸Ĳ���
* ע������ :
*****************************************************************************/

#include "Debug_.h"
#include "OledDrive.h"
#include "KeyScan.h"
#include "MotorDrive.h"
#include "EncordDrive.h"
#include "PID_Process.h"
#include "Ov7725Drive.h"
#include "ServoDrive.h"
#include "WIFIDrive.h"
#include "include.h"
//#include "include.h"
u8_Pro Debug_OLED_PageNum = 0;				/* !< ȷ����ǰ��ʾ���ǵڼ�ҳ 					*/
u8_Pro Debug_OLED_PageCmd = 0;				/* !< ȷ����ǰ��ʹ���޸����� ����ѡ������ 	*/
u8_Pro OVST = 0x57;

const u8_Pro DEBUG_Rcicon[] = {0x00,0x00,0x03,0x07,0x0F,0x0F,0x0F,0x17,0xE7,0x17,0x0F,0x0F,0x07,0x07,0x03,0x00,
							   0x7C,0xFE,0xFE,0xFE,0xFC,0xFC,0xF8,0xF0,0xF0,0xF0,0xF8,0xFC,0xFC,0xFE,0xFE,0x7C};/*"D:\Picture\QQ????20180419091647.bmp",0*/

const u8_Pro DEBUG_Wificon[] = {0x04,0x0C,0x1D,0x19,0x33,0x33,0x37,0x76,0x76,0x76,0x37,0x33,0x33,0x19,0x19,0x1C,
								0x00,0x00,0x80,0x80,0x00,0x70,0x70,0x67,0x67,0x67,0x70,0x70,0x80,0x80,0x80,0x00};/*"D:\Picture\WIFI.bmp",0*/



const u8_Pro DEBUG_IMGicon[] = {0x1F,0x1F,0x16,0x16,0x13,0x7F,0x6C,0x7C,0x78,0x78,0x6C,0x77,0x13,0x10,0x10,0x1F,
								0xFC,0xFC,0x04,0x04,0xC4,0xF4,0x14,0x1C,0x1C,0x1C,0x34,0xF4,0xC4,0x04,0x04,0xFC};/*"D:\Picture\cammer.bmp",0*/

const u8_Pro DEBUG_SETicon[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x3F,0x7F,0xFF,0xFF,0xFF,0xE7,0xC7,0x0F,0x1F,
								0x0F,0x1F,0x3F,0x7F,0x7F,0xFF,0xFE,0xFC,0xF0,0xE0,0xC0,0xC0,0xC0,0xC0,0xC0,0x80/*"D:\Picture\SET.bmp",0*/};

/**********************************************************************************
* ��������: Debug_SetParam
* ��������: ����ģʽ�£���������������� ���� PID �ٶ�PID �ȵ�
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Debug_SetParam(void)
{
	Encord_GetSpeed_Prepare();
	Motor_div.setPoint = 0;
	Motor_div.const_D = 1000;
	Motor_div.const_P = 60;
	Motor_Default_Setpoint = 200;
	while(Key_Of_Confirm(KEY_MODE_Once) == 0)
	{
		OverFlag = MDL_ROAD_Straight;
		if(Key_Of_Up(KEY_MODE_Once) == 1)
		{
			Motor_Default_Setpoint+=10;
		}else if(Key_Of_Down(KEY_MODE_Once) ==1)
		{
			Motor_Default_Setpoint-=10;
		}
		OLED_ResetGRAM();

		OLED_Show_String(0,0,"SET:",charSize16);OLED_Show_Num(35,0,Motor_Default_Setpoint,charSize16);
		OLED_Show_String(0,16,"SPEED:",charSize16);OLED_Show_Num(48,16,(Motor_Default_Setpoint*1300/200000),charSize16);
		OLED_Show_Char(56,16,'.',charSize16,POINT_STATU_SET); OLED_Show_Num(64,16,(Motor_Default_Setpoint*1300/200)%1000,charSize16);

		OLED_Refrech_Gram();
		if(Key_Of_Cancel(KEY_MODE_Once) == 1)
		{
			break;
		}
	}
	if(Key_Of_Cancel(KEY_MODE_Continuous) == 0)
	{
		PID_SET_begin();
	}
	while(Key_Of_Cancel(KEY_MODE_Continuous) == 0)
	{
		camera_get_img();
		IMG_externd();
		
		IMG(MDL_array,0);
		Servo_div.setPoint = MDL_Slope;
		PID_Servo_AngleSet();
		OLED_ResetGRAM();
	}
	PID_SET_stop();
	Motor_Set_Speed(MOTOR_CH1_DUTY);
}

/**********************************************************************************
* ��������: Debug_RcMode
* ��������: ʹ���յ���Ƭ���ȵ� ң�أ���Ϊһ��ң�س�
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Debug_RcMode(void)
{
  unsigned int b=150*4;
  
	String_Pro RC_string;
	Encord_GetSpeed_Prepare();
	Motor_div.setPoint = 0;
	Motor_div.const_D = 1000;
	Motor_div.const_P = 60;
        
        while(Key_Of_Cancel(KEY_MODE_Once) == 0)
	{
          
		camera_get_img();
		IMG_externd();
                IMG_GetSign();
                Servo_Set_Angle(Servo_Default_Angle+slope);
                
                OLED_ResetGRAM();
		if(1) 
		{
			for(int i = 0;i<60;i++)
			{
				for(int j = 0;j<80;j++)
				{
					if(OV_Img[i][j])
					OLED_Show_Point(j,i,POINT_STATU_SET);
				} 
			}  
		}
                OLED_Show_String(80,0,"FPS",charSize12);OLED_Show_Num(100,0,OLED_Refrech_FPS,charSize12); 
		OLED_Show_String(80,12,"r",charSize12);OLED_Show_Num(100,12,IMG_Road_sign,charSize12);
		OLED_Show_String(80,24,"rl",charSize12);OLED_Show_Num(100,24,IMG_Road_sign_late,charSize12);
		
                OLED_Show_String(80,36,"s",charSize12);
                
                if(slope<0)
                {
                  OLED_Show_String(90,36,"-",charSize12);
                  OLED_Show_Num(100,36,-slope,charSize12);
                }
                else 
                {
                  OLED_Show_Num(100,36,slope,charSize12);
                }
		OLED_Refrech_Gram();
		
            if(Key_Of_Down(KEY_MODE_Once) == 1)
			{
			IMG_Road_sign=IMG_Road_Straight;
			IMG_Road_sign_late=IMG_Road_Straight;
			date=0;
			}
                
        }
        
        
        while(Key_Of_Cancel(KEY_MODE_Once) == 0)
	{
		OLED_ResetGRAM(); 
		OLED_Show_String(80,0,"FPS",charSize12);OLED_Show_Num(100,0,OLED_Refrech_FPS,charSize12); 
		OLED_Show_String(80,12,"s",charSize12);OLED_Show_Num(100,12,speedmax,charSize12); 
		OLED_Refrech_Gram();
                
                if(Key_Of_Down(KEY_MODE_Once) == 1)
		{
                  speedmin-=10;
                  speedmax-=10;
        }
                if(Key_Of_Up(KEY_MODE_Once) == 1)
		{
                  speedmin+=10;
                  speedmax+=10;
        }
                 
        }
         
        while(b-->0)
        {
	camera_get_img();
	IMG_externd();
        }
        
        
        PID_SET_begin(); 
        IMG_Road_sign=IMG_Road_Straight;
        IMG_Road_sign_late=IMG_Road_Straight;
		date=0;
	while(Key_Of_Cancel(KEY_MODE_Once) == 0)
	{
          
		camera_get_img();
		IMG_externd();
                IMG_GetSign();
                Servo_Set_Angle(Servo_Default_Angle+slope);
                
        }

//	 unsigned int b=150*5;
//  
//	String_Pro RC_string;
//	Encord_GetSpeed_Prepare();
//	Motor_div.setPoint = 0;
//	Motor_div.const_D = 1000;
//	Motor_div.const_P = 60;
//        while(Key_Of_Cancel(KEY_MODE_Once) == 0)
//	{
//		OLED_ResetGRAM(); 
//		OLED_Show_String(80,0,"FPS",charSize12);OLED_Show_Num(100,0,OLED_Refrech_FPS,charSize12); 
//		OLED_Show_String(80,12,"s",charSize12);OLED_Show_Num(100,12,speedmax,charSize12); 
//		OLED_Refrech_Gram();
//				
//		if(Key_Of_Down(KEY_MODE_Once) == 1)
//		{
//			speedmin-=10;
//			speedmax-=10;
//		}
//		if(Key_Of_Up(KEY_MODE_Once) == 1)
//		{
//			speedmin+=10;
//			speedmax+=10;
//		}
//					
//	}
//			
//		while(b-->0)
//		{
//	camera_get_img();
//	IMG_externd();
//		}
//		
//		
//        PID_SET_begin(); 
//        IMG_Road_sign=IMG_Road_Straight;
//        IMG_Road_sign_late=IMG_Road_Straight;
//	while(Key_Of_Cancel(KEY_MODE_Once) == 0)
//	{
//          
//		camera_get_img();
//		IMG_externd();
//                IMG_GetSign();
//                Servo_Set_Angle(Servo_Default_Angle+slope);
//                
//        }
}

/**********************************************************************************
* ��������: Debug_ImgAcq
* ��������: ͼ��ɼ�ģʽ ��ͼ��ɼ��� ������λ��
* ��������: void input:
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Debug_ImgAcq(void)
{
	u8_Pro i,j,k,m = 0;
		Encord_GetSpeed_Prepare();
	while(Key_Of_Cancel(KEY_MODE_Once) == 0)
	{
	  	if(Key_Of_Down(KEY_MODE_Once) == 1)
		{
			OVST--;
			while(0 == SCCB_WriteByte(OV7725_CNST,OVST))
			{}
		
		}
		
		if(Key_Of_Up(KEY_MODE_Once) == 1)
		{
			OVST++;
			while(0 == SCCB_WriteByte(OV7725_CNST,OVST))
			{};
		
		}
		if(Key_Of_Confirm(KEY_MODE_Once) == 1)
		{
			m = (m+1)%2;
		}
		camera_get_img();
		IMG_externd();
		
		if(m%2)
		{
			IMG(MDL_array,0);	
		}
		
		Servo_div.setPoint = MDL_Slope;
		PID_Servo_AngleSet();
		OLED_ResetGRAM();
		if(m)
		{
			OLED_Show_String(0,0,"up:",charSize12);OLED_Show_Num(19,0,pointUp.X,charSize12);OLED_Show_Num(35,0,pointUp.Y,charSize12);
			OLED_Show_String(0,12,"do:",charSize12);OLED_Show_Num(19,12,pointDown.X,charSize12);OLED_Show_Num(35,12,pointDown.Y,charSize12);
			OLED_Show_String(0,24,"le:",charSize12);OLED_Show_Num(19,24,pointLeft.X,charSize12);OLED_Show_Num(35,24,pointLeft.Y,charSize12);
			OLED_Show_String(0,36,"ri:",charSize12);OLED_Show_Num(19,36,pointRight.X,charSize12);OLED_Show_Num(35,36,pointRight.Y,charSize12);
			OLED_Show_String(0,48,"ce:",charSize12);OLED_Show_Num(19,48,pointCenter.X,charSize12);OLED_Show_Num(35,48,pointCenter.Y,charSize12);
			// OLED_Show_String(80,0,"slope",charSize12);
			// OLED_Show_Num(80,12,Servo_Angle_Now,charSize12);


			OLED_Show_String(64,0,"uL:",charSize12);OLED_Show_Num(83,0,pointUpLeft.X,charSize12);OLED_Show_Num(99,0,pointUpLeft.Y,charSize12);
			OLED_Show_String(64,12,"uR:",charSize12);OLED_Show_Num(83,12,pointUpRight.X,charSize12);OLED_Show_Num(99,12,pointUpRight.Y,charSize12);
			OLED_Show_String(64,24,"dL:",charSize12);OLED_Show_Num(83,24,pointDownLeft.X,charSize12);OLED_Show_Num(99,24,pointDownLeft.Y,charSize12);
			OLED_Show_String(64,36,"dR:",charSize12);OLED_Show_Num(83,36,pointDownRight.X,charSize12);OLED_Show_Num(99,36,pointDownRight.Y,charSize12);
			OLED_Show_String(64,48,"La:",charSize12);OLED_Show_Num(83,48,lastFlag,charSize12);OLED_Show_Num(99,48,last2Flag,charSize12);

//			OLED_Show_String(80,0,"slope",charSize12);
//			OLED_Show_Num(80,12,Servo_Angle_Now,charSize12);
			// for(i = 0;i<80;i++)
			// {
			// 	// for(j = 0;j<80;j++)
			// 	// {
			// 	// 	if(OV_Img[i*80+j])
			// 	// 	OLED_Show_Point(j,i,POINT_STATU_SET);
			// 	// }
			// 	OLED_Show_Point(i,Img_Pox[i],POINT_STATU_SET);
			// }
			// IMG_FlagJudgement();
			// MDL_Slope = MDL_FitSlope(MDL_Slope);
			// Servo_Set_Angle(Servo_Default_Angle+(MDL_Slope*4));
		}else
		{
			for(i = 0;i<60;i++)
			{
				for(j = 0;j<80;j++)
				{
					if(OV_Img[i][j])
					OLED_Show_Point(j,i,POINT_STATU_SET);
				}
			//	OLED_Show_Point(i,Img_Pox[i],POINT_STATU_SET);
			}
			OLED_Show_Point(pointCenter.X,pointCenter.Y,POINT_STATU_SET);
			OLED_Show_Num(84,0,(u8_Pro)roadFlag,charSize16);
			OLED_Show_Num(84 ,16,(u8_Pro)lastFlag,charSize12);OLED_Show_String(92,16,":la",charSize12);
			OLED_Show_Num(84 ,28,(u8_Pro)last2Flag,charSize12);OLED_Show_String(92,28,":la2",charSize12);
			OLED_Show_Num(84,40,(CircleFlag & 0x0FFF),charSize12);
			OLED_Show_Num(100,40,(OVST),charSize12);
		}
		
		if(k&0x01)
		{
			OLED_Show_Point(127,63,POINT_STATU_SET);
			Debug_IOTest_Star();
		//	k = 0;
		}
		else
		{
	//	  	k = 1;
			OLED_Show_Point(127,63,POINT_STATU_RESET);
			Debug_IOTest_Over();
		}
//		DELAY_MS(20);
		OLED_Show_String(80,52,"FPS",charSize12);OLED_Show_Num(100,52,OLED_Refrech_FPS,charSize12);

		// OLED_Show_Num(83,0,samplePoint_L[0],charSize12);OLED_Show_Num(100,0,samplePoint_L[1],charSize12);
		// OLED_Show_Num(83,12,samplePoint_L[2],charSize12);OLED_Show_Num(100,12,samplePoint_L[3],charSize12);
		// OLED_Show_Num(83,24,samplePoint_L[4],charSize12);OLED_Show_Num(100,24,samplePoint_L[5],charSize12);
		// OLED_Show_Num(83,36,samplePoint_L[6],charSize12);OLED_Show_Num(100,36,samplePoint_L[7],charSize12);
		// OLED_Show_Num(83,42,Servo_Angle_Now,charSize12);
		

		OLED_Refrech_Gram();
//		if(Key_Of_Confirm(KEY_MODE_Once))
		if((OV_Refresh_FPS%65) == 0)
		{
			/*!< sending  img to  computer */
			k = 0;
			OV_Sending(OV_Rx_ImgOne,(CAMERA_W*CAMERA_H)>>3);
		}else
		{
			k++;
		}
	}
	
	
}

u8_Pro Debug_SetPageNum = 0;		/*!< ˽��ȫ�ֱ��� ������ �� Debug_Set ������ʹ��*/
u8_Pro DEBUG_SET_ParFlag=0;
void Debug_Set(void)
{
	String_Pro msg;
	Encord_GetSpeed_Prepare();
	while(Key_Of_Cancel(KEY_MODE_Once)==0)
	{
		msg = WIFi_Get_Rx_BUF();
		if(msg != NULL)
		{
			switch(Debug_SetPageNum)
			{
				case 0:
						Servo_Default_Angle = Debug_getNumFormStr(msg,"DDIR",Servo_Default_Angle);
						Servo_MaxTurn_Angle = Debug_getNumFormStr(msg,"MDIR",Servo_MaxTurn_Angle);
						Motor_Default_Setpoint = Debug_getNumFormStr(msg,"DSET",Motor_Default_Setpoint);
					break;
				case 1:
						Motor_div.const_P = Debug_getNumFormStr(msg,"PIDP",Motor_div.const_P);
						Motor_div.const_I = Debug_getNumFormStr(msg,"PIDI",Motor_div.const_I);
						Motor_div.const_D = Debug_getNumFormStr(msg,"PIDD",Motor_div.const_D);

						Servo_div.const_P = Debug_getNumFormStr(msg,"ANGP",Servo_div.const_P);
						Servo_div.const_I = Debug_getNumFormStr(msg,"ANGI",Servo_div.const_I);
						Servo_div.const_D = Debug_getNumFormStr(msg,"ANGD",Servo_div.const_D);
					break;
			}
			

			
		}
		OLED_ResetGRAM();

		switch(Debug_SetPageNum)
		{
			case 0:	Debug_OLED_SecondPage();		break;		/*!< ������ �ȵ�һЩʵʱ�������� */
			case 1: Debug_OLED_ThirdPage();			break;		/*!< ��� ���PID���ڲ������� */
		}
#if 0
		/*!< ҳ��ģ�� */
		/*!< -------------------��һ��--------------------- */
		/*!< ���PID���� P���� */
		OLED_Show_String(0,0,"|PIDP",charSize12);	OLED_Show_Num(30,0,Motor_div.const_P,charSize12);
		/*!< ���PID���� I���� */
		OLED_Show_String(0,12,"|PIDI",charSize12);	OLED_Show_Num(30,12,Motor_div.const_I,charSize12);
		/*!< ���PID���� D���� */
		OLED_Show_String(0,24,"|PIDD",charSize12);	OLED_Show_Num(30,24,Motor_div.const_P,charSize12);
		/*!<   */
		OLED_Show_String(0,36,"|    ",charSize12);
		/*!< ��ص�ѹ */
		OLED_Show_String(0,48,"|VBAT",charSize12);	OLED_Show_Num(30,48,Cell_GetBatteryPer(),charSize12);


		/*!< -------------------�ڶ���--------------------- */

		/*!< ���PID���� P���� */
		OLED_Show_String(64,0,"|ANGP",charSize12);	OLED_Show_Num(94,0,Servo_div.const_P,charSize12);
		/*!< ���PID���� I���� */
		OLED_Show_String(64,12,"|ANGI",charSize12);	OLED_Show_Num(94,12,Servo_div.const_I,charSize12);
		/*!< ���PID���� D���� */
		OLED_Show_String(64,24,"|ANGD",charSize12);	OLED_Show_Num(94,24,Servo_div.const_D,charSize12);
		/*!<      */
		OLED_Show_String(64,36,"|    ",charSize12);	OLED_Show_Num(94,36,Motor_div.setPoint,charSize12);
		OLED_Show_String(64,48,"|    ",charSize12);
#endif 
		if(Key_Of_Up(KEY_MODE_Once))
		{
			Debug_OLED_SecondPage_Up_Press();
		}else if(Key_Of_Down(KEY_MODE_Once))
		{
			Debug_OLED_SecondPage_Down_Press();
		}
		/*!< ��ʾ ѡ���ĸ� ������Ҫ�޸� */
		OLED_Show_Char(((DEBUG_SET_ParFlag%10)/5)*64,((DEBUG_SET_ParFlag%10)%5)*12,'>',charSize12,POINT_STATU_SET);
		OLED_Refrech_Gram();
		// if(Key_Of_Down(KEY_MODE_Once)==1)	Debug_SetPageNum = (Debug_SetPageNum+1)%2;
		// else if(Key_Of_Up(KEY_MODE_Once)==1)	Debug_SetPageNum = (Debug_SetPageNum+1)%2;
		DELAY_MS(20);
		
	}
	Encord_GetSpeed_Close();
}

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

void Debug_OLED_ShowIcon(u8_Pro index_X,u8_Pro index_Y,u8_Pro* icon)
{
	u8_Pro i;
	String_Pro dat;
	dat = (String_Pro) icon;

	for(i = 0;i<16;i++)
	{
		OLED_Set_Byte(index_X+i,index_Y,*(dat+i),POINT_STATU_SET);
	}
	for(i = 0;i<16;i++)
	{
		OLED_Set_Byte(index_X+i,index_Y+8,*(dat+i+16),POINT_STATU_SET);
	}

}

/****************************************************************************
* ��������: ��һҳ��ʾ����
* �������� : ������һҳ��Ҫ��ʾʲô����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FristPage(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
	String_Pro icon[4];
	u8_Pro x = 0,y = 0;
	u8_Pro keyvalue;
	icon[0] = (u8_Pro*)DEBUG_Rcicon;
	icon[1] = (u8_Pro*)DEBUG_Wificon;
	icon[2] = (u8_Pro*)DEBUG_IMGicon;
	icon[3] = (u8_Pro*)DEBUG_SETicon;
	OLED_ResetGRAM();
	Debug_OLED_PageNum = 0;
	while(Key_Of_Confirm(KEY_MODE_Once)==0)
	{
		if(Key_Of_Down(KEY_MODE_Once))
		{
			Debug_OLED_FristPage_Down_Press();
			y = (y+16)%64;
			
		}else if(Key_Of_Up(KEY_MODE_Once))
		{
			Debug_OLED_FristPage_Up_Press();
			y = (y+32)%64;
		}
		OLED_ResetGRAM();
		Debug_OLED_ShowIcon(x,y,icon[Debug_OLED_PageNum]);
		OLED_Show_String(18,0,"|RC Mode",charSize16);
		OLED_Show_String(18,16,"|Wifi Mode",charSize16);
		OLED_Show_String(18,32,"|IMG Mode",charSize16);
		OLED_Show_String(18,48,"|SET Mode",charSize16);
		OLED_Refrech_Gram();
		
	}

}



/****************************************************************************
* ��������: ��һҳ �ϼ� ������
* �������� : ���� �ڵ�һҳʱ �����ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FristPage_Up_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif

	Debug_OLED_PageNum = (Debug_OLED_PageNum+2)%3;

}


/****************************************************************************
* ��������: ��һҳ �¼� ������
* �������� : ���� �ڵ�һҳʱ ���� �� ���Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FristPage_Down_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
	Debug_OLED_PageNum = (Debug_OLED_PageNum+1)%4;
}


/****************************************************************************
* ��������: ��һҳ ȡ���� ������
* �������� : ���� �ڵ�һҳʱ ���� ȡ�����Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FristPage_Cancel_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}



/****************************************************************************
* ��������: ��һҳ ȷ���� ������
* �������� : ���� �ڵ�һҳʱ ����ȷ�ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FristPage_Confirm_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}

//�ڶ�ҳ

/*!< �޸ĺ�����־λ */

/****************************************************************************
* ��������: �ڶ�ҳ��ʾ����
* �������� : �����ڶ�ҳ ��Ҫ��ʾʲô����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_SecondPage(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif

	/*!< -------------------��һ��--------------------- */
		/*!< Ĭ�϶��ת�� */
		OLED_Show_String(0,0,"|DDIR",charSize12);	OLED_Show_Num(33,0,Servo_Default_Angle,charSize12);
		/*!< ������ת�� */
		OLED_Show_String(0,12,"|MDIR",charSize12);	OLED_Show_Num(33,12,Servo_MaxTurn_Angle,charSize12);
		/*!< �����ǰת�� */
		OLED_Show_String(0,24,"|NDIR",charSize12);	OLED_Show_Num(33,24,Servo_Angle_Now,charSize12);
		/*!< Ĭ�ϵ��PID��������ֵ */
		OLED_Show_String(0,36,"|DSET",charSize12);	OLED_Show_Num(33,36,Motor_Default_Setpoint,charSize12);
		/*!< ��ص�ѹ */
		OLED_Show_String(0,48,"|VBAT",charSize12);	OLED_Show_Num(33,48,Cell_GetBatteryPer() * 3,charSize12);


		/*!< -------------------�ڶ���--------------------- */

		/*!< Ĭ��pwm ռ�ձ� */
		OLED_Show_String(64,0,"|DPWM",charSize12);	OLED_Show_Num(97,0,MOTOR_CH1_DUTY,charSize12);
		/*!< ���ڵ����ռ�ձ� */
		OLED_Show_String(64,12,"|NPWM",charSize12);	OLED_Show_Num(97,12,Motor_Speed_Now,charSize12);
		/*!< ��ʵ�ı������ٶ� */
		OLED_Show_String(64,24,"|RSET",charSize12);	OLED_Show_Num(97,24,Encord_GetMotorTime(),charSize12);
		/*!< ��ǰPID���ڵ���ʵֵ */
		OLED_Show_String(64,36,"|PSET",charSize12);	OLED_Show_Num(97,36,Motor_div.setPoint,charSize12);
	
		OLED_Show_String(64,48,"|OVST",charSize12); OLED_Show_Num(97,48,OVST,charSize12);


		// if(Key_Of_Up(KEY_MODE_Once))
		// {
		// 	Debug_OLED_SecondPage_Up_Press();
		// }else if(Key_Of_Down(KEY_MODE_Once))
		// {
		// 	Debug_OLED_SecondPage_Down_Press();
		// }
		// /*!< ��ʾ ѡ���ĸ� ������Ҫ�޸� */
		// OLED_Show_Char(((DEBUG_SET_ParFlag%10)/5)*64,((DEBUG_SET_ParFlag%10)%5)*16,'>',charSize16,POINT_STATU_SET);

}


/****************************************************************************
* ��������: �ڶ�ҳ �ϼ� ������
* �������� : ���� �ڵڶ�ҳʱ �����ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_SecondPage_Up_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
	DELAY_MS(350);
	if(Key_Of_Up(KEY_MODE_Continuous))
	{
		/*!< ���� ѡ������޸�ֵ������ */
		DEBUG_SET_ParFlag = (DEBUG_SET_ParFlag+19)%20;
		Debug_SetPageNum = DEBUG_SET_ParFlag/10;
	}else
	{
		Debug_ModifyPar(DEBUG_SET_ParFlag,0);
	}
}

/**********************************************************************************
* ��������: Debug_ModifyPar
* ��������: �����޸���Ҫ�޸ĵĲ�����ֵ
* ��������: void input:������־ �� ���(0 ��       ��0 ��)
* �������: void output:
* ���� :author:@Kim_alittle_star
**********************************************************************************/
void Debug_ModifyPar(u8_Pro DEBUG_set_Flag,u8_Pro Up_Down)
{
	DEBUG_set_Flag = DEBUG_set_Flag%20;
	switch(DEBUG_set_Flag)
	{
		/*!< Ĭ��ֱ�з���Ƕ� */
		case 0: Servo_Default_Angle = (Up_Down)?(Servo_Default_Angle+5):(Servo_Default_Angle-5); break;

		/*!< ���ת��ת��Ƕ� */
		case 1: Servo_MaxTurn_Angle = (Up_Down)?(Servo_MaxTurn_Angle+5):(Servo_MaxTurn_Angle-5); break;

		/*!< ��ǰ���ת�Ƿ��� */
		case 2: Servo_Set_Angle(
								(Up_Down)?
								(Servo_Angle_Now+5):(Servo_Angle_Now-5)
								); break;

		/*!< Ĭ�ϵ��PID ����ֵ */
		case 3: Motor_Default_Setpoint=(Up_Down)?(Motor_Default_Setpoint+10):(Motor_Default_Setpoint-10); break;

		/*!< ��ص�ѹ */
		case 4:  break;

		/*!< Ĭ��ֹͣ PWM */
		case 5: break;

		/*!< ���ڵ�PWM */
		case 6:  break;

		/*!< ��������ǰֵ */
		case 7: break;

		/*!< �����ǰ PID ����ֵ */
		case 8: Motor_div.setPoint = (Up_Down)?(Motor_div.setPoint+10):(Motor_div.setPoint-10); break;

		/*!< �� */
		case 9:(Up_Down)?(OVST++):(OVST--); 
				while(0 == SCCB_WriteByte(OV7725_CNST,OVST))
				{} break;

		/*!< ���PID���� P */
		case 10: Motor_div.const_P = (Up_Down)?(Motor_div.const_P+10):(Motor_div.const_P-10); break;

		/*!< ���PID���� I */
		case 11: Motor_div.const_I = (Up_Down)?(Motor_div.const_I+10):(Motor_div.const_I-10); break;

		/*!< ���PID���� D */
		case 12: Motor_div.const_D = (Up_Down)?(Motor_div.const_D+10):(Motor_div.const_D-10); break;

		/*!< �� */
		case 13: break;

		/*!< ��ص�ѹ */
		case 14: break;

		/*!< ���PID���� P */
		case 15: Servo_div.const_P = (Up_Down)?(Servo_div.const_P+10):(Servo_div.const_P-10); break;

		/*!< ���PID���� I */
		case 16: Servo_div.const_I = (Up_Down)?(Servo_div.const_I+10):(Servo_div.const_I-10); break;

		/*!< ���PID���� D */
		case 17: Servo_div.const_D = (Up_Down)?(Servo_div.const_D+10):(Servo_div.const_D-10); break;

		/*!< �� */
		case 18: break;
		/*!< �� */
		case 19: break;
		
	}
}

/****************************************************************************
* ��������: �ڶ�ҳ �¼� ������
* �������� : ���� �ڵڶ�ҳʱ ���� �¼��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_SecondPage_Down_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
DELAY_MS(350);
	if(Key_Of_Down(KEY_MODE_Continuous))
	{
		/*!< ���� ѡ������޸�ֵ������ */
		DEBUG_SET_ParFlag = (DEBUG_SET_ParFlag+1)%20;
		Debug_SetPageNum = DEBUG_SET_ParFlag/10;
	}else
	{
		Debug_ModifyPar(DEBUG_SET_ParFlag,1);
	}
}



/****************************************************************************
* ��������: �ڶ�ҳ ȡ���� ������
* �������� : ���� �ڵڶ�ҳʱ ���� ȡ�����Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_SecondPage_Cancel_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: �ڶ�ҳ ȷ�ϼ� ������
* �������� : ���� �ڵڶ�ҳʱ ���� ȷ�ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_SecondPage_Confirm_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}

//����ҳ

/****************************************************************************
* ��������: ����ҳ��ʾ����
* �������� : ��������ҳ��Ҫ��ʾʲô����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_ThirdPage(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
	/*!< -------------------��һ��--------------------- */
		/*!< ���PID���� P���� */
		OLED_Show_String(0,0,"|PIDP",charSize12);	OLED_Show_Num(33,0,Motor_div.const_P,charSize12);
		/*!< ���PID���� I���� */
		OLED_Show_String(0,12,"|PIDI",charSize12);	OLED_Show_Num(33,12,Motor_div.const_I,charSize12);
		/*!< ���PID���� D���� */
		OLED_Show_String(0,24,"|PIDD",charSize12);	OLED_Show_Num(33,24,Motor_div.const_D,charSize12);
		/*!<   */
		OLED_Show_String(0,36,"|    ",charSize12);
		/*!< ��ص�ѹ */
		OLED_Show_String(0,48,"|VBAT",charSize12);	OLED_Show_Num(33,48,Cell_GetBatteryPer(),charSize12);


		/*!< -------------------�ڶ���--------------------- */

		/*!< ���PID���� P���� */
		OLED_Show_String(64,0,"|ANGP",charSize12);	OLED_Show_Num(97,0,Servo_div.const_P,charSize12);
		/*!< ���PID���� I���� */
		OLED_Show_String(64,12,"|ANGI",charSize12);	OLED_Show_Num(97,12,Servo_div.const_I,charSize12);
		/*!< ���PID���� D���� */
		OLED_Show_String(64,24,"|ANGD",charSize12);	OLED_Show_Num(97,24,Servo_div.const_D,charSize12);
		/*!<      */
		OLED_Show_String(64,36,"|    ",charSize12);	
		OLED_Show_String(64,48,"|    ",charSize12);
		// if(Key_Of_Up(KEY_MODE_Once))
		// {
		// 	Debug_OLED_SecondPage_Up_Press();
		// }else if(Key_Of_Down(KEY_MODE_Once))
		// {
		// 	Debug_OLED_SecondPage_Down_Press();
		// }
		// /*!< ��ʾ ѡ���ĸ� ������Ҫ�޸� */
		// OLED_Show_Char(((DEBUG_SET_ParFlag%10)/5)*64,((DEBUG_SET_ParFlag%10)%5)*16,'>',charSize16,POINT_STATU_SET);

}


/****************************************************************************
* ��������: ����ҳ �ϼ� ������
* �������� : ���� �ڵ���ҳʱ ���� �ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_ThirdPage_Up_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ �¼� ������
* �������� : ���� �ڵ���ҳʱ ���� �¼��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_ThirdPage_Down_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ ȡ���� ������
* �������� : ���� �ڵ���ҳʱ ���� ȡ�����Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_ThirdPage_Cancel_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ ȷ�ϼ� ������
* �������� : ���� �ڵ���ҳʱ ���� ȷ�ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_ThirdPage_Confirm_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}

//����ҳ

/****************************************************************************
* ��������: ����ҳ��ʾ����
* �������� : ��������ҳ��Ҫ��ʾʲô����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FourthPage(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ �ϼ� ������
* �������� : ���� �ڵ���ҳʱ ���� �ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FourthPage_Up_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ �¼� ������
* �������� : ���� �ڵ���ҳʱ ���� �¼��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FourthPage_Down_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ ȡ���� ������
* �������� : ���� �ڵ���ҳʱ ���� ȡ�����Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FourthPage_Cancel_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: ����ҳ ȷ�ϼ� ������
* �������� : ���� �ڵ���ҳʱ ���� ȷ�ϼ��Ĵ�����
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_FourthPage_Confirm_Press(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
}


/****************************************************************************
* ��������: OLED ������������
* �������� : �������ж� ��Ҫ�����Ǹ�ҳ�棬���߽���׼�������ȵȡ�
* ���� :
* ��� :
* ���� :
*****************************************************************************/
void Debug_OLED_Main(void)
{
#if Debug_OLED_Function == 1
	#warning "����δ��д"		/*!< ��д����ɾ���˶� */
#endif
	u8_Pro Debug_Oled_Over_flag = 0;
	switch(Debug_OLED_PageNum)
	{
		case 1: Debug_OLED_FristPage(); break;
		case 2: Debug_OLED_SecondPage(); break;
		case 3: Debug_OLED_ThirdPage(); break;
		case 4: Debug_OLED_FourthPage(); break;
	}
//	while(1)	/* �����ɨ�谴�����а������� ���˳� ��ѭ�� */
//	{
//		if(Key_Of_Cancel() == 1)
//		{
//			switch(Debug_OLED_PageNum)
//		}
//	}
}
