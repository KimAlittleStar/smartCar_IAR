
#ifndef _IMG_PROCESS_H_
#define _IMG_PROCESS_H_
#include "Global_Var.h"


#include "Ov7725Drive.h"
/*------------------------�궨��  ��----------------------*/
 

enum IMG_Road_Sign{
    IMG_Road_Straight,//0ֱ��
    IMG_Road_Left,//1�����
    IMG_Road_Right,//2�����
    IMG_Road_Cross,//3ʮ��
    IMG_Road_CirqueLeft,//4��Բ��
    IMG_Road_CirqueRight,//5��Բ��
    IMG_Road_End
};
 

extern enum IMG_Road_Sign IMG_Road_sign;//��������������  
extern enum IMG_Road_Sign IMG_Road_sign_late;//�ϴδ������������ͣ�������ֱ����




#define WITHE 0xff
#define BLACK 0
#define BMP(x,y) (OV_Img[y][x])
 
extern int slope,speed ;//�������  
extern char date; 

extern int speedmin,speedmax;
/*------------------------����������----------------------*/
 



void IMG_Forecast();//�жϵ�·����
void IMG_GetSign();//���������õ�����
void IMG_Straight();//ֱ��������
void IMG_Left();//����������
void IMG_Right();//���������� 
void IMG_Cross();//ʮ���������� 
void IMG_CirqueLeft();//��Բ���������� 
void IMG_CirqueRight();//��Բ���������� 
enum IMG_Road_Sign IMG_End(int y);

enum IMG_Road_Sign IMG_Forecast_Straight(char lenth);//ֱ���ж�
enum IMG_Road_Sign  IMG_Forecast_LeftRight();//��������ж�
enum IMG_Road_Sign  IMG_Forecast_Cross();//ʮ���ж�
enum IMG_Road_Sign  IMG_Forecast_CirqueLeft();//�󻷵��ж�
enum IMG_Road_Sign  IMG_Forecast_CirqueRight();//�һ����ж�

char IMG_FindPoint(char px,char py,char dir,char len,unsigned char  color);
int IMG_Parity(int a,int b);


#endif

