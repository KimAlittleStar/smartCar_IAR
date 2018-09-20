
#ifndef _MIDLINE_PROCESS_H_
#define _MIDLINE_PROCESS_H_


#include "Global_Var.h"
#include "IMG_Process.h"

#define BLACk 0x00
#define WHITE 0xFF
#define MDL_array (*OV_Img)
#define JUDGE_BLACK(X,Y,SEPT) ((MDL_array[(X)+(Y)*80]==BLACk || MDL_array[(X)+(Y)*80+(SEPT)]==BLACk)?(0):(1)) /*!< 或门判断 */
#define JUDEG_XOR(X,Y,SEPT) ((MDL_array[(X)+(Y)*80]==MDL_array[(X)+(Y)*80+(SEPT)])?(1):(0)) /*!< 异或门 判断 */
#define JUDEG_GET(X,Y) (OV_Img[(Y)][(X)])
#define MDL_UpdataSwitch 1
/*!< Curve fitting */
typedef struct FIT
{
    Float_Pro IMG_FitA[3];         /*!< a[0] b[0] c[0]  = d[0] */
    Float_Pro IMG_FitB[3];
    Float_Pro IMG_FitC[3];
    Float_Pro IMG_FitD[3];
    Float_Pro IMG_FitPr0_L;      /*!< 矩阵求值时的第一列 */
    Float_Pro IMG_FitPr1_L;      /*!< 矩阵求值时的第二列 */
    Float_Pro IMG_FitPr2_L;      /*!< 矩阵求值时的第三列 */
    Float_Pro IMG_FitPr0_R;      /*!< 右边 曲线 的系数 */
    Float_Pro IMG_FitPr1_R;      /*!< 右边 曲线 的系数 */
    Float_Pro IMG_FitPr2_R;      /*!< 右边 曲线 的系数 */
    Float_Pro IMG_LineX_0;      /*!< 边界曲线函数中 x^0的系数 */
    Float_Pro IMG_LineX_1;      /*!< 边界曲线函数中 x^1 的系数 */
    Float_Pro IMG_LineX_2;      /*!< 边界曲线函数中 x^2 的系数 */      
} IMG_FIT;

typedef enum 
{
MDL_ROAD_Straight = 0,
MDL_ROAD_Left = 1,
MDL_ROAD_Right = 2,
MDL_ROAD_Cross = 3, 
MDL_ROAD_Over = 4,
MDL_ROAD_Block = 5, 
MDL_ROAD_Error = 6,
MDL_ROAD_CircleLeft = 7,
MDL_ROAD_CircleRight = 8
} MDL_ROADFLAG; 



typedef struct
{
    int8_Pro X;
    int8_Pro Y;
} POINT;

extern POINT pointCenter;
extern POINT pointBase;
extern POINT pointLeft;
extern POINT pointUp;
extern POINT pointDown;
extern POINT pointRight;
extern POINT pointUpRight;
extern POINT pointUpLeft;
extern POINT pointDownRight;
extern POINT pointDownLeft;
extern u16_Pro  CircleFlag;
extern u16_Pro BlockFlag;
extern volatile MDL_ROADFLAG  roadFlag;
extern volatile MDL_ROADFLAG  lastFlag;
extern volatile MDL_ROADFLAG  last2Flag;
extern volatile MDL_ROADFLAG OverFlag;
extern volatile int16_Pro  MDL_Slopes;

extern IMG_FIT Fit_div;

extern u8_Pro Img_Pox[80];

void MDL_JudgeRoad(void);


u8_Pro* IMG(u8_Pro* img_data,u8_Pro roadFlag);
u8_Pro MDL_GetSamplePoint(u8_Pro *samplePoint,u8_Pro scanDir);
int16_Pro MDL_CrossProcess(void);
int16_Pro MDL_LeftProcess(void);
int16_Pro MDL_RightProcess(void);
int16_Pro MDL_StraightProcess(void);
int16_Pro MDL_CircleProcess(void);
int16_Pro MDL_BlockProcess(void);
void MDL_FindCenterPoint(void);
void MDL_FindUPPoint(POINT* targetPoint);
void MDL_FindDownPoint(POINT* targetPoint);
void MDL_FindLeftPoint(POINT* targetPoint);
void MDL_FindRightPoint(POINT* targetPoint);




int16_Pro MDL_GetSlop(POINT* point1,POINT* point2);
int16_Pro findingTable(Float_Pro value);
int16_Pro MDL_FitSlope(int16_Pro slope);

void paintLine(u8_Pro star,u8_Pro stop,u8_Pro* Img_Pox);





//u8_Pro IMG_array[4800];
/*------------------------宏定义  区----------------------*/



/*------------------------函数声明区----------------------*/
void MidLine_GetRoad_Go(void);

void MidLine_GetRoad_Turn(void);

void MidLine_Refrash_MidDir(void);

#endif