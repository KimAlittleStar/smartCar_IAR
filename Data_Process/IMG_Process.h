
#ifndef _IMG_PROCESS_H_
#define _IMG_PROCESS_H_
#include "Global_Var.h"


#include "Ov7725Drive.h"
/*------------------------宏定义  区----------------------*/
 

enum IMG_Road_Sign{
    IMG_Road_Straight,//0直道
    IMG_Road_Left,//1左弯道
    IMG_Road_Right,//2右弯道
    IMG_Road_Cross,//3十字
    IMG_Road_CirqueLeft,//4左圆环
    IMG_Road_CirqueRight,//5右圆环
    IMG_Road_End
};
 

extern enum IMG_Road_Sign IMG_Road_sign;//处于赛道的类型  
extern enum IMG_Road_Sign IMG_Road_sign_late;//上次处于赛道的类型（不包括直道）




#define WITHE 0xff
#define BLACK 0
#define BMP(x,y) (OV_Img[y][x])
 
extern int slope,speed ;//舵机增量  
extern char date; 

extern int speedmin,speedmax;
/*------------------------函数声明区----------------------*/
 



void IMG_Forecast();//判断道路函数
void IMG_GetSign();//处理赛道得到数据
void IMG_Straight();//直道处理函数
void IMG_Left();//左赛道处理
void IMG_Right();//右赛道处理 
void IMG_Cross();//十字赛道处理 
void IMG_CirqueLeft();//左圆环赛道处理 
void IMG_CirqueRight();//右圆环赛道处理 
enum IMG_Road_Sign IMG_End(int y);

enum IMG_Road_Sign IMG_Forecast_Straight(char lenth);//直道判断
enum IMG_Road_Sign  IMG_Forecast_LeftRight();//左右弯道判断
enum IMG_Road_Sign  IMG_Forecast_Cross();//十字判断
enum IMG_Road_Sign  IMG_Forecast_CirqueLeft();//左环岛判断
enum IMG_Road_Sign  IMG_Forecast_CirqueRight();//右环岛判断

char IMG_FindPoint(char px,char py,char dir,char len,unsigned char  color);
int IMG_Parity(int a,int b);


#endif

