/****************************************************************************
* 文件名称 : MidLine_Process.c
* 文件功能 : 将所提取到的中线 数组 进行滤波，元整，近似得到中线的曲线或直线函数。
			由此函数得到 的 斜率由此来调整 舵机 电机的斜率
* 注意事项 :
*****************************************************************************/
#include "MidLine_Process.h"
#include "artan.h"
#include "include.h"
#define MDL_GET_TAN(a,b) ((a)/(b))
#define DELAY_ROAD 0x808F

Float_Pro MidLine_Dir_Near = 0.0;
Float_Pro MidLine_Dir_Mid=0.0;
Float_Pro MidLine_Dir_Far=0.0;

u16_Pro CircleFlag = 0;          /*!< 三态标志 未进圆环 圆环ing 出圆环 */
u16_Pro BlockFlag = 0;



IMG_FIT Fit_div;
//extern struct PID_DIV Motor_div;
u8_Pro samplePoint_L[8];
u8_Pro samplePoint_R[8];
u8_Pro Img_Pox[80];
int16_Pro MDL_Slope  = 0;
int16_Pro MDL_SlopeS_[3];
int16_Pro MDL_SlopeEvg;
int16_Pro MDL_SlopeErr;

volatile int16_Pro lastSlope;


volatile MDL_ROADFLAG roadFlag;
volatile MDL_ROADFLAG  lastFlag;
volatile MDL_ROADFLAG last2Flag;
volatile MDL_ROADFLAG OverFlag = MDL_ROAD_Straight;
POINT pointCenter = {39,59};
POINT pointBase = {39,59};
POINT pointLeft;
POINT pointUp;
POINT pointDown;
POINT pointRight;
POINT pointUpRight;
POINT pointUpLeft;
POINT pointDownRight;
POINT pointDownLeft;
volatile int16_Pro MDL_Slopes;


int16_Pro MDL_FitSlope(int16_Pro slope)
{
    MDL_SlopeEvg = (MDL_SlopeS_[2] + MDL_SlopeS_[1] + MDL_SlopeS_[0])/3;
    if(1)/*!< 存储错误数据 和下一次错误数据相比较 */
    {
        MDL_SlopeS_[2] = MDL_SlopeS_[1];
        MDL_SlopeS_[1] = MDL_SlopeS_[0];
        MDL_SlopeS_[0] = slope;
    }
    //else
    // {
    //     if(MDL_SlopeErr < slope +150 && MDL_SlopeErr > slope -150)
    //     {
    //         MDL_SlopeS[2] = MDL_SlopeS[1];
    //         MDL_SlopeS[1] = MDL_SlopeS[0];
    //         MDL_SlopeS[0] = slope;
    //     }else
    //     {
    //         MDL_SlopeErr = slope;
    //     }
        
    // }
    return MDL_SlopeEvg;
}


/****************************************************************************
* 函数名称: 直道中线拟合算法
* 函数功能 : 在直道 十字路口的 时候 对中线的斜率提取
*			提取到 近处斜率 中部斜率	远处斜率
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void MidLine_GetRoad_Go(void)
{
#if MIDLINE_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
	MidLine_Dir_Far = 0;
	MidLine_Dir_Mid = 0;
	MidLine_Dir_Near = 0;
}


/****************************************************************************
* 函数名称: 直道中线拟合算法
* 函数功能 : 在弯道 圆环处的 时候 对中线的斜率提取
*			提取到 近处斜率 中部斜率	远处斜率
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void MidLine_GetRoad_Turn(void)
{
#if MIDLINE_FUNCTION == 1
	#warning "函数未重写"		/*!< 重写后请删除此段 */
#endif
	MidLine_Dir_Far = 0;
	MidLine_Dir_Mid = 0;
	MidLine_Dir_Near = 0;

	
}

/****************************************************************************
* 函数名称: 刷新中线方向函数
* 函数功能 : 根据当前道路，选择不同的 中线拟合方案
* 输入 :void
* 输出 :void
* 作者 :
*****************************************************************************/
void MidLine_Refrash_MidDir(void)
{
	// if(ROAD_IMG_FLAG != IMG_OUT_FLAG)
	// {
	// 	if(ROAD_IMG_FLAG == IMG_ACROSS_FLAG || ROAD_IMG_FLAG == IMG_GO_FLAG)
	// 	{
	// 		MidLine_GetRoad_Go();
	// 	}else
	// 	{
	// 		MidLine_GetRoad_Turn();
	// 	}
	// }
}


u8_Pro* IMG(u8_Pro* img_data,u8_Pro roadFlag)
{
    MDL_JudgeRoad();
#ifdef mex_h
    printf("center(%d,%d)\n",pointCenter.X,pointCenter.Y);
    MDL_array[pointCenter.X + pointCenter.Y*80] = 0X08;
#endif
    MDL_GetSamplePoint(samplePoint_L,0);
	return img_data;
}

/**********************************************************************************
* 函数名称: MDL_JudgeRoad
* 函数功能: 判断道路类型 将道路类型写入到 roadType 中
* 函数输入: void input: 上一个道路的中心点坐标
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
void MDL_JudgeRoad(void)
{
    int8_Pro i=0;
    int8_Pro tx,ty;
    POINT leftarray[6],rightarray[6];
    Float_Pro leftslope[4];
    Float_Pro rightslope[4];
//    int8_Pro cir_y1,cir_y2;
    MDL_FindCenterPoint();
    if((pointLeft.X == 0 || pointRight.X == 79) && (pointLeft.X != 0 || pointRight.X != 79))
    {/*!< 两个点有一个为边缘 一个不为边缘 */
        i = 1;
        if(pointLeft.X == 0)
        {/*!< 左边为  0  寻找右边的黑点处 上下 哪边先出现 白点 上面 出现白点 重新取一次 中心 下面出现白点跳过 */
            
            while(JUDEG_GET(pointRight.X,pointRight.Y-i) == JUDEG_GET(pointRight.X,pointRight.Y+i))
            {
                i++;
            }
            if(JUDEG_GET(pointRight.X,pointRight.Y+i) == BLACk )
            {
                MDL_FindCenterPoint();
            }
        }else
        {/*!< 右边为 79  寻找左边黑点出 上下哪边先出现白点 上面出现白点 就重新去一次点 */
            while(JUDEG_GET(pointLeft.X,pointLeft.Y+i) == JUDEG_GET(pointLeft.X,pointLeft.Y-i))
            {
                i++;
            }
            if(JUDEG_GET(pointLeft.X,pointLeft.Y+i) == BLACk)
            {
                MDL_FindCenterPoint();
            }
        }
    }


    /*!< 确定 上左点 x坐标 */
        ty = pointUp.Y;
        tx = pointUp.X;
        if(JUDEG_GET(pointUp.X,pointUp.Y) == BLACk)
        {
            #ifdef mex_h
            printf("********************error**********************\n");
            #endif
            i = 0;
            while(JUDEG_XOR(tx-i,ty,i*2))
            {
                i++;
            }
            if(JUDEG_GET(tx-i,ty) == WHITE)
            {
                tx -= i;
            }else
            {
                tx += i;
            }
        }
        i = tx;/*!< 中间变量 存储 寻找到的 tx 点  */

#if MDL_UpdataSwitch == 0
        while(tx>1 && JUDGE_BLACK(tx,ty,-2))
        {
            tx -= 4;
        }
         tx = (tx<2)?
            ((JUDEG_GET(0,ty) == WHITE)?(0):((JUDEG_GET(1,ty)== WHITE)?(1):(((JUDEG_GET(2,ty)== WHITE))?(2):(3)))):
            ((JUDEG_XOR(tx+1,ty,-2))?((JUDEG_GET(tx,ty)== WHITE)?(tx-1):(tx+2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx+1)));
        pointUpLeft.X = tx;
        pointUpLeft.Y = ty;
    /*!< 确定 上右点 x坐标 */
//        ty = pointUp.Y;
        tx = i;
        while(tx<78 && JUDGE_BLACK(tx,ty,2))
        {
            tx += 4;
        }
        tx = (tx<78)?
            ((JUDEG_XOR(tx-1,ty,2))?((JUDEG_GET(tx,ty)== WHITE)?(tx+1):(tx-2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx-1))):
            ((JUDEG_GET(79,ty) == WHITE)?(79):((JUDEG_GET(78,ty)== WHITE)?(78):(((JUDEG_GET(77,ty)== WHITE))?(77):(76))));
        pointUpRight.X = tx;
        pointUpRight.Y = ty;



    /*!< 确定 下左点 x坐标 */
    ty = pointDown.Y;
    tx = pointDown.X;
    while(tx>1 && JUDGE_BLACK(tx,ty,-2))
    {
        tx -= 4;
    }
     tx = (tx<2)?
            ((JUDEG_GET(0,ty) == WHITE)?(0):((JUDEG_GET(1,ty)== WHITE)?(1):(((JUDEG_GET(2,ty)== WHITE))?(2):(3)))):
            ((JUDEG_XOR(tx+1,ty,-2))?((JUDEG_GET(tx,ty)== WHITE)?(tx-1):(tx+2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx+1)));
    pointDownLeft.X = tx;
    pointDownLeft.Y = ty ;

    /*!< 确定 下右点 x坐标 */
    tx = pointDown.X;
    while(tx<78 && JUDGE_BLACK(tx,ty,2))
    {
        tx += 4;
    }
    tx = (tx<78)?
            ((JUDEG_XOR(tx-1,ty,2))?((JUDEG_GET(tx,ty)== WHITE)?(tx+1):(tx-2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx-1))):
            ((JUDEG_GET(79,ty) == WHITE)?(79):((JUDEG_GET(78,ty)== WHITE)?(78):(((JUDEG_GET(77,ty)== WHITE))?(77):(76))));
    pointDownRight.X = tx;
    pointDownRight.Y = ty;

#else
    /*!< 上左点坐标  */
    pointUpLeft.X = tx;
    pointUpLeft.Y = pointUp.Y;
    MDL_FindLeftPoint(&pointUpLeft);
    /*!< 上右点坐标 */
    pointUpRight.X = tx;
    pointUpRight.Y = pointUp.Y;
    MDL_FindRightPoint(&pointUpRight);
    /*!< 下左 */
    pointDownLeft.X = pointDown.X;
    pointDownLeft.Y = pointDown.Y;
    MDL_FindLeftPoint(&pointDownLeft);
    /*!< 下右 */
    pointDownRight.X = pointDown.X;
    pointDownRight.Y = pointDown.Y;
    MDL_FindRightPoint(&pointDownRight);



#endif



    /*!< 检测圆环时  首要条件 在于 中心 在 30-35 之间 左右两侧 一个为零 一个不为零 、
    向上检测 发现起点 测到中心的距离一定大于5-7 且另一侧一定不为 0 且 林一侧边沿向内倾斜 */
    /*!< 在圆环标志里面 error 直线 都要按照上一个 道路标志进行打舵 */
    /*!< take  the road judegment  to  next code  */
    /*!< first was  cross  secend was straight  */
    if((pointDownLeft.X+10)>pointDownRight.X)
    {
        OverFlag = MDL_ROAD_Over;
    }
    if(pointLeft.X == 0 && pointRight.X == 79)
    {
        roadFlag = MDL_ROAD_Cross;
    }else
    {
        if(pointCenter.Y>39) /*!< 中心点 在 坐标40以下  横坐标 在 左右两边即为 转向 */
        {
            if(pointCenter.X<39)
            {
                roadFlag = MDL_ROAD_Left;
            }else
            {
                if(pointCenter.X>40)
                {
                    roadFlag = MDL_ROAD_Right;
                }else
                {
                    roadFlag = MDL_ROAD_Straight;
                }
            }
            
        }else
        {
            // if((pointDownLeft.X > (pointLeft.X+8) || pointDownRight.X < (pointRight.X-8)) && 
            //                         (pointRight.X!= 79 || pointLeft.X != 0) && 
            //                         OverFlag != MDL_ROAD_Over)
            // {
            //     roadFlag = MDL_ROAD_Block;
            // }else
            
                roadFlag = MDL_ROAD_Straight;
            
            
            if(pointCenter.X>30 && pointCenter.X<49 &&pointCenter.Y<33 )
//            if(1)/*!< 实验性 */                && pointRight.X!= 79 && pointLeft.X != 0 
            {
                // if(pointUp.Y == 0 && pointUp.X>(pointUpLeft.X+2) && pointUp.X<(pointUpRight.X-2))
                // {
                //         leftarray[0].X = pointLeft.X + (pointUpLeft.X-pointLeft.X)/3;
                //         rightarray[0].X = pointRight.X - (pointRight.X-pointUpRight.X)/3;
                // }else
                // {
                    leftarray[0].X = pointLeft.X+3;
                    rightarray[0].X = pointRight.X-3;

                // }
                leftarray[0].Y = pointLeft.Y;
                rightarray[0].Y = pointRight.Y;
                /*!< 两边同时向上搜索 */
                MDL_FindUPPoint(&leftarray[0]);
                MDL_FindUPPoint(&rightarray[0]);
                /*!< 如果两边相差大于 */
                if((leftarray[0].Y-rightarray[0].Y) > 7 && rightarray[0].Y>(pointUp.Y+5))/*!< 右圆环情况 */
                {/*!< 右环判断 */
                    /*!< 判断步骤：1. 找到右边边缘白点的点 */
                    /*!<          2. 向下寻找，确定期间的距离 */
                    /*!<          3. 从中线开始寻找对面的点 确保对面的点都不是通路（排除十字路口的可能） */ 
                    for(i = 0;i<3;i++)
                    {
                        rightarray[1].X = rightarray[0].X;
                        rightarray[1].Y = rightarray[0].Y+1+i;
                        MDL_FindRightPoint(&rightarray[1]);
                        if(rightarray[1].X == 79)
                        {
                            break;
                        }   
                    }
                    if(rightarray[1].X ==79)
                    {/*!< scan to down */
                        rightarray[3].X = rightarray[1].X;
                        rightarray[3].Y = rightarray[1].Y;
                        MDL_FindDownPoint(&rightarray[3]);
                        rightarray[2].X = rightarray[3].X;
                        rightarray[2].Y = (rightarray[3].Y+rightarray[1].Y)>>1;
                        rightarray[4].X = (rightarray[0].X+rightarray[1].X)>>1;
                        rightarray[4].Y = rightarray[2].Y;
                        MDL_FindUPPoint(&rightarray[4]);
                        rightarray[5].X = pointCenter.X;
                        rightarray[5].Y = rightarray[4].Y-1;
                        MDL_FindRightPoint(&rightarray[5]);
//                        if(rightarray[4].Y <= rightarray[0].Y && rightarray[5].X<35)
                        if(rightarray[5].X<(rightarray[4].X-5))
                        {
                            leftarray[1].X = pointCenter.X;
                            leftarray[1].Y = rightarray[1].Y;
                            MDL_FindLeftPoint(&leftarray[1]);

                            leftarray[2].X = pointCenter.X;
                            leftarray[2].Y = rightarray[2].Y;
                            MDL_FindLeftPoint(&leftarray[2]);

                            leftarray[3].X = pointCenter.X;
                            leftarray[3].Y = rightarray[3].Y;
                            MDL_FindLeftPoint(&leftarray[3]);
                            if(leftarray[1].X>=leftarray[2].X && leftarray[2].X>=leftarray[3].X && leftarray[3].X > 0)
                            {
                                roadFlag = MDL_ROAD_CircleRight;
                            }
                        }else
                        {
                            lastFlag = MDL_ROAD_Straight;
                            lastSlope = 0;
                        }
                    }




                }else 
                    if((leftarray[0].Y-rightarray[0].Y) <(-7)&& leftarray[0].Y>(pointUp.Y+5))/*!< 左圆环情况 */
                    {/*!< 左环判断 */
                        /*!< 参考上面 右环判断 */
                         for(i = 0;i<3;i++)
                        {
                            leftarray[1].X = leftarray[0].X;
                            leftarray[1].Y = leftarray[0].Y+1+i;
                            MDL_FindLeftPoint(&leftarray[1]);
                            
                            if(leftarray[1].X == 0)
                            {
                                break;
                            }   
                        }
                        if(leftarray[1].X ==0)
                        {/*!< scan to down */
                            leftarray[3].X = leftarray[1].X;
                            leftarray[3].Y = leftarray[1].Y;
                            MDL_FindDownPoint(&leftarray[3]);
                            leftarray[2].X = leftarray[3].X;
                            leftarray[2].Y = (leftarray[3].Y+leftarray[1].Y)>>1;
                            leftarray[4].X = (leftarray[0].X+leftarray[1].X)>>1;
                            leftarray[4].Y = leftarray[2].Y;
                            MDL_FindUPPoint(&leftarray[4]);
                            leftarray[5].X = pointCenter.X;
                            leftarray[5].Y = leftarray[4].Y-1;
                            MDL_FindLeftPoint(&leftarray[5]);
//                            if(leftarray[4].Y <= leftarray[0].Y && leftarray[5].X>45)
                            if(leftarray[5].X>(leftarray[4].X+5))
                            {
                                rightarray[1].X = pointCenter.X;
                                rightarray[1].Y = leftarray[1].Y;
                                MDL_FindRightPoint(&rightarray[1]);

                                rightarray[2].X = pointCenter.X;
                                rightarray[2].Y = leftarray[2].Y;
                                MDL_FindRightPoint(&rightarray[2]);

                                rightarray[3].X = pointCenter.X;
                                rightarray[3].Y = leftarray[3].Y;
                                MDL_FindRightPoint(&rightarray[3]);
                                if(rightarray[1].X<=rightarray[2].X && rightarray[2].X<=rightarray[3].X && rightarray[3].X <79)
                                {
                                    roadFlag = MDL_ROAD_CircleLeft;
                                }
                            }else
                            {
                                lastFlag = MDL_ROAD_Straight;
                                lastSlope = 0;
                            }
                        }
                    }
                /*!< 是否圆环 判断结束 */
#ifdef mex_h
    printf("#########left-right = %d ##############",leftarray[0].Y-rightarray[0].Y);
#endif



            }

        }
    }

 #ifdef mex_h   
    MDL_array[pointUp.X+pointUp.Y*80] = 0x4f;
    MDL_array[pointDown.X+pointDown.Y*80] = 0x4f;
    MDL_array[pointLeft.X+pointLeft.Y*80] = 0x4f;
    MDL_array[pointRight.X+pointRight.Y*80] = 0x4f;
    MDL_array[pointBase.X+pointBase.Y*80] = 0x4f;
    MDL_array[pointCenter.X+pointCenter.Y*80] = 0x4f;
    MDL_array[pointDownLeft.X+pointDownLeft.Y*80] = 0x4f;
    MDL_array[pointDownRight.X+pointDownRight.Y*80] = 0x4f;
    MDL_array[pointUpLeft.X+pointUpLeft.Y*80] = 0x4f;
    MDL_array[pointUpRight.X+pointUpRight.Y*80] = 0x4f;
#endif
    if(lastFlag == MDL_ROAD_CircleLeft || lastFlag == MDL_ROAD_CircleRight)
    {
//        roadFlag = MDL_ROAD_Straight;
        Cell_BEEP_ON;
    }else
    {
        Cell_BEEP_OFF;    
    }




}


inline void MDL_FindCenterPoint(void)
{

    int8_Pro tx = pointCenter.X,ty = pointCenter.Y;
   
//     /*!< 确定 左点 x坐标 */
// //    ty = pointLeft.Y;
//     while(tx>1 && JUDGE_BLACK(tx,ty,-2))
//     {
//         tx -= 4;
//     }
//     tx = (tx<2)?(0):(((JUDEG_GET(tx-1,ty)== WHITE)?(tx-1):(tx+1)));
//     pointLeft.X = tx;

// /*!< 确定 右点 x坐标 */
//     tx = pointCenter.X;
//     while(tx<78 && JUDGE_BLACK(tx,ty,2))
//     {
//         tx += 4;
//     }
//     tx = (tx<78)?((JUDEG_GET(tx+1,ty)== WHITE)?(tx+1):(tx-1)):(79);
//     pointRight.X = tx;
//     /*!< 根据左右求中间 x */
//     pointCenter.X = (pointRight.X+pointLeft.X)>>1;
    
    /*!< 判断最下方 是否有黑点 */

    tx = pointCenter.X;
    pointUp.X = tx;
    pointDown.X = tx;
    if(JUDGE_BLACK(tx,58,80) == 0)
    {
        roadFlag = MDL_ROAD_Error; 
       return;
    }
    pointDown.Y = 59;


    /*!< 判断最上方 是否通路 */
    

#if MDL_UpdataSwitch == 0
    ty = pointCenter.Y;
    while(ty>1 && JUDGE_BLACK(tx,ty,-160))
    {
        ty -= 4;
    }               /*!< 两者有一点 为黑 */
//    ty =(JUDEG_XOR(tx,ty,-160))?(ty):(ty-2);/*!< 相同 则最下点为起始黑 不想通 则上点为起始黑  */
    ty = (ty<2)?
            ((JUDEG_GET(tx,0) == WHITE)?(0):((JUDEG_GET(tx,1)== WHITE)?(1):(((JUDEG_GET(tx,2)== WHITE))?(2):(3)))):
            ((JUDEG_XOR(tx,ty+1,-160))?((JUDEG_GET(tx,ty)== WHITE)?(ty-1):(ty+2)):((JUDEG_GET(tx,ty) == WHITE)?(ty):(ty+1)));
    pointUp.Y = ty;
#else
    pointUp.X = pointCenter.X;
    pointUp.Y = pointCenter.Y;
    MDL_FindUPPoint(&pointUp);
#endif
   

    /*!< 上下长度确定 随之 左右的 Y 就确定 */
    pointLeft.Y = ((pointUp.Y + pointDown.Y)>>1);
    pointRight.Y = pointLeft.Y;

    /*!< 确定左右的 两个 点坐标 */

/*!< 确定 左点 x坐标 */
  
#if MDL_UpdataSwitch == 0
    ty = pointLeft.Y;
    while(tx>1 && JUDGE_BLACK(tx,ty,-2))
    {
        tx -= 4;
    }
    tx = (tx<2)?
            ((JUDEG_GET(0,ty) == WHITE)?(0):((JUDEG_GET(1,ty)== WHITE)?(1):(((JUDEG_GET(2,ty)== WHITE))?(2):(3)))):
            ((JUDEG_XOR(tx+1,ty,-2))?((JUDEG_GET(tx,ty)== WHITE)?(tx-1):(tx+2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx+1)));
    pointLeft.X = tx;
#else
    pointLeft.X = pointCenter.X;
    MDL_FindLeftPoint(&pointLeft);
#endif
/*!< 确定 右点 x坐标 */
#if MDL_UpdataSwitch ==0
    tx = pointCenter.X;
    while(tx<78 && JUDGE_BLACK(tx,ty,2))
    {
        tx += 4;
    }
    tx = (tx<78)?
            ((JUDEG_XOR(tx-1,ty,2))?((JUDEG_GET(tx,ty)== WHITE)?(tx+1):(tx-2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx-1))):
            ((JUDEG_GET(79,ty) == WHITE)?(79):((JUDEG_GET(78,ty)== WHITE)?(78):(((JUDEG_GET(77,ty)== WHITE))?(77):(76))));
    pointRight.X = tx;
#else
    pointRight.X = pointCenter.X;
    MDL_FindRightPoint(&pointRight);
#endif
#ifdef mex_h
    printf("Left(%d,%d)\tRight(%d,%d)\tUp(%d,%d)\t down(%d,%d)\n",pointLeft.X,pointLeft.Y,pointRight.X,pointRight.Y,pointUp.X,pointUp.Y,pointDown.X,pointDown.Y);

#endif


    pointCenter.X = (pointRight.X + pointLeft.X)>>1;
    pointCenter.Y = pointLeft.Y;
    
}

/**********************************************************************************
* 函数名称: MDL_GetSamplePoint
* 函数功能: 读取图像获得样本点坐标
* 函数输入: void input:样本点坐标数组 ，寻找方向 左  0    右 ！0
* 函数输出: void output:是否找到边界线 找到 ！0  没有找到 0
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
#ifdef mex_h
u8_Pro MDL_GetSamplePoint(u8_Pro *PointArray,u8_Pro scanDir,u8_Pro roadFlag)
#else
u8_Pro MDL_GetSamplePoint(u8_Pro *samplePoint,u8_Pro scanDir)
#endif
{
    POINT pointtemp;
    int16_Pro nextspeed;
    pointtemp.X = (pointUpLeft.X+pointUpRight.X)>>1;
    pointtemp.Y = pointUp.Y;
    int32_Pro speed;
    speed = MDL_GetSlop(&pointtemp,&pointCenter);
//  	u8_Pro i = (u8_Pro) roadFlag;
    pointBase.X = 39;
    pointBase.Y = 59;
    switch(roadFlag)
    {
        case (MDL_ROAD_Block):    /*!< 路障 */
            MDL_Slope = MDL_BlockProcess();
            break;
        case (MDL_ROAD_Cross):    /*!< 十字 */
            MDL_Slope = MDL_CrossProcess(); 			
             Servo_div.const_P = 215;
            break;
        case (MDL_ROAD_Left):    /*!< 左弯 */
            MDL_Slope = MDL_LeftProcess();
            Servo_div.const_P = 230;
            break;
        case (MDL_ROAD_Right):    /*!< 右弯 */
            MDL_Slope = MDL_RightProcess();

            Servo_div.const_P = 230;
            break;
        case (MDL_ROAD_Over):    /*!< 终点 */
            break;
        case (MDL_ROAD_Straight):    /*!< 直道 */
            MDL_Slope = MDL_StraightProcess();
            Servo_div.const_P = 215;
            break;
        case (MDL_ROAD_CircleLeft):
            MDL_Slope = MDL_CircleProcess(); 
            Servo_div.const_P = 230;
            break;
	case (MDL_ROAD_CircleRight):
            MDL_Slope = MDL_CircleProcess();
            Servo_div.const_P = 230;
            break;
        case (MDL_ROAD_Error):
            break;
        default: break;
    }
    CircleFlag = (CircleFlag >= 0x8000)?(CircleFlag+1):(CircleFlag);
    BlockFlag = (BlockFlag>=0x8000)?(BlockFlag+1):(BlockFlag);
    
    nextspeed = Motor_Default_Setpoint-((speed>0)?(speed/10):((0-speed)/10));
    last2Flag = (CircleFlag >= DELAY_ROAD)?(MDL_ROAD_Straight):(last2Flag); 


    CircleFlag = (CircleFlag >= DELAY_ROAD)?(0):(CircleFlag);
    BlockFlag = (BlockFlag>=0x8040)?(0):(BlockFlag);  

    if(OverFlag !=MDL_ROAD_Over)
   { 
       if(last2Flag == MDL_ROAD_CircleLeft || last2Flag == MDL_ROAD_CircleRight || lastFlag == MDL_ROAD_CircleLeft || lastFlag == MDL_ROAD_CircleRight)
        {
            Motor_div.setPoint = (Motor_Default_Setpoint - 40);
    //        (Motor_div.setPoint>nextspeed)?(nextspeed):(Motor_div.setPoint)
        }else
        {
            Motor_div.setPoint = (nextspeed > Motor_Default_Setpoint - 5)?(Motor_Default_Setpoint)
                                                                        :((nextspeed > Motor_Default_Setpoint - 30)?
                                                                            (Motor_Default_Setpoint - 20):
                                                                            (Motor_Default_Setpoint - 40));
        } 
    }else
    {
        Motor_div.setPoint = 0;
    }
    pointBase.X = 39;
    pointBase.Y = 59;
	return 0;
}

/**********************************************************************************
* 函数名称: MDL_BlockProcess
* 函数功能: void
* 函数输入: void input:
* 函数输出: void output: 舵机返回值
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro MDL_BlockProcess(void)
{
    // POINT temppoint1,temppoint2;
    // BlockFlag = 0x8000;
    // temppoint1.Y = pointCenter.Y;
    // temppoint1.X = (pointDownLeft.X>pointLeft.X)?(pointRight.X-15):((pointDownRight.X<pointRight.X)?(pointLeft.X+15):(pointCenter.X));
    // temppoint2.Y = pointBase.Y;
    // temppoint2.Y = pointBase.Y;
    // temppoint2.X = (pointDownLeft.X+ pointDownRight.X+pointCenter.X)/3;
    // temppoint2.X = (pointDownLeft.X+ pointDownRight.X+pointCenter.X)/3;
    if(Motor_GetSpeed_Of_Now()>200)
   return MDL_GetSlop(&pointCenter,&pointBase);
    else
     return 0;
    
}

/**********************************************************************************
* 函数名称: MDL_CircleProcess
* 函数功能: 圆环判断处理函数
* 函数输入: void input:
* 函数输出: void output:舵机打舵角度
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro MDL_CircleProcess(void)
{
    POINT pointtemp;
    volatile int16_Pro slope;
    pointtemp.X = pointCenter.X;
    pointtemp.Y = pointCenter.Y;
    if(roadFlag == MDL_ROAD_CircleLeft)
    {
        pointtemp.X = 22; 
    }else{
        pointtemp.X= 58;
    }
    if(lastFlag != roadFlag)
    {
        last2Flag = lastFlag;
        lastFlag = roadFlag;
    }
    lastSlope =  MDL_GetSlop(&pointtemp,&pointBase);
     pointtemp.X = (roadFlag == MDL_ROAD_CircleLeft)?(pointCenter.X-5)
                                                     :(pointCenter.X+5);
     pointtemp.Y = pointCenter.Y;
//    pointBase.Y = pointCenter.Y;
    slope = MDL_GetSlop(&pointtemp,&pointBase);
//    pointBase.Y =59;
    return slope;
}

/**********************************************************************************
* 函数名称: MDL_CrossProcess
* 函数功能:十字路口的处理函数
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro  MDL_CrossProcess(void)
{
    volatile int16_Pro slope;
    POINT pointTemp;
    volatile int8_Pro tempx;
    tempx = (pointDownLeft.X+pointDownRight.X);
    pointBase.X = (tempx>79)?(39-tempx+79):(39+79-tempx);
    if(pointUp.Y == 0 && pointDownLeft.X == 0&& pointDown.X == 79)
    {
        pointTemp.X = (pointUpLeft.X+pointUpRight.X)>>1;
        pointTemp.Y = 0;
        slope = MDL_GetSlop(&pointTemp,&pointBase);
		if(lastFlag == MDL_ROAD_CircleLeft || lastFlag == MDL_ROAD_CircleRight)
            {
                slope = lastSlope/3*4;
            }
    }else
    {
        if(pointDownLeft.X != 0 || pointDownRight.X != 79)
        {
            slope = MDL_GetSlop(&pointCenter,&pointBase);            
        }else 
        {
            slope = (lastSlope * pointUp.Y) / 39;
        }
        if((CircleFlag & 0x8000) && pointUp.Y <24)
        {
            if(
                (last2Flag == MDL_ROAD_CircleLeft || last2Flag == MDL_ROAD_CircleRight) && 
                CircleFlag<DELAY_ROAD)
            {
                 pointTemp.X = (last2Flag == MDL_ROAD_CircleLeft)?(pointCenter.X+4)
                                                               :(pointCenter.X-4);
                 pointTemp.Y = pointCenter.Y;
                 pointCenter.X  = (last2Flag == MDL_ROAD_CircleLeft)?(pointCenter.X+6):(pointCenter.X-6);
                slope = MDL_GetSlop(&pointTemp,&pointBase);
//                slope = -lastSlope/4;
    //            pointBase.Y = 59;
                
            }
        }else if(lastFlag == MDL_ROAD_CircleLeft || lastFlag == MDL_ROAD_CircleRight)
            {
                slope = lastSlope/3*4;
                pointCenter.X  = (last2Flag == MDL_ROAD_CircleLeft)?(pointCenter.X-8):(pointCenter.X+8);
            }
        if((last2Flag == MDL_ROAD_CircleLeft || last2Flag == MDL_ROAD_CircleRight))
        {
            if(pointUp.Y > 25 && (CircleFlag&0x8000) == 0)
            {
    //            slope = MDL_GetSlop(&pointCenter,&pointBase);
                CircleFlag |= 0x8000; 
            }
            slope = ((lastSlope * pointUp.Y) / 28)+(lastSlope/4);
        }
        

        pointBase.X = 39;
    }    
    return slope;
    
}

/**********************************************************************************
* 函数名称: MDL_LeftProcess
* 函数功能: 左转弯时候 舵机的期望斜率 通过最小二乘法线性拟合 与重心斜率的均值作为期望斜率
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro MDL_LeftProcess(void)
{
    volatile int16_Pro slope;
//    POINT pointStar,pointStop;
//    int8_Pro ty;
//    pointStar.X = pointRight.X;
//    pointStar.Y = 59;
    // while(pointStar.X<79 && JUDGE_BLACK(pointStar.X,pointStar.Y,1))
    // {
    //     pointStar.X+=2;
    // }
    // if(pointStar.X>76 && JUDGE_BLACK(78,pointStar.Y,1) )
    // {/*!< 认为到达上边界 */
    //     while(pointStar.Y>pointCenter.Y && JUDGE_BLACK(pointStar.X,pointStar.Y,-160))
    //     {
    //         pointStar.Y-=4;
    //     }
    //     if(pointStar.Y <= pointCenter.Y)
    //     {
    //         pointStar.X = pointRight.X;
    //         pointStar.Y = pointRight.Y;
    //     }else
    //     {
    //         pointStar.Y = (JUDEG_XOR(pointStar.X,pointStar.Y,-160))?(pointStar.Y):(pointStar.Y-2);
    //     }
    // }else
    // {
    //     pointStar.X = (JUDGE_BLACK(pointStar.X+1,pointStar.Y,1))?(pointStar.X):(pointStar.X+1);
    // }
    // /*!< stop  点 */
    // ty =  pointUp.Y;
    // pointStop.X = pointLeft.X;
    // while(ty>1 && JUDGE_BLACK(pointStop.X,ty,-160))
    // {
    //     ty -=4;
    // }
    // if(ty<2)
    // {/*!< ERROR   */
    //     pointStop.Y = pointUp.Y;
    //     pointStop.X = pointUp.X;
    // }else
    // {
    //     pointStop.Y = (JUDEG_XOR(pointStop.X,ty,-160))?(ty-2):(ty);
    // }  /*!< 矩阵填充 求算邪率 */
    // slope[0] = MDL_GetSlop(&pointUp,&pointRight);
    // slope[1] = MDL_GetSlop(&pointStar,&pointStop);
    slope = MDL_GetSlop(&pointCenter,&pointBase);
#ifdef mex_h

    printf("left: pointbase (%d,%d)\n", pointBase.X,pointBase.Y);
    printf("left Star (%d,%d),stop:(%d,%d)\n" ,pointStar.X,pointStar.Y ,pointStop.X,pointStop.Y);
    printf("left: Left(%d,%d)\tRight(%d,%d)\tUp(%d,%d)\t down(%d,%d)\n",pointLeft.X,pointLeft.Y,pointRight.X,pointRight.Y,pointUp.X,pointUp.Y,pointDown.X,pointDown.Y);
    printf("left slope %d , %d ,%d ,%d ", slope[0] ,slope[1],slope[2],(slope[0] +slope[1]+slope[2])/3);

#endif
    if(lastFlag != roadFlag)
    {
        last2Flag = lastFlag;
        lastFlag = roadFlag;
    }
    lastSlope = slope;
//	MDL_Slopes= slope[1] ;
	return slope;
}

/**********************************************************************************
* 函数名称: MDL_RightProcess
* 函数功能: 左转弯时候 舵机的期望斜率 通过最小二乘法线性拟合 与重心斜率的均值作为期望斜率
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro MDL_RightProcess(void)
{
    volatile int16_Pro slope;
    // int8_Pro tx,ty;
    // POINT pointStar,pointStop;
    // tx = pointLeft.X;
    // pointStar.Y = 59;
    // while(tx > 1 && JUDGE_BLACK(tx,pointStar.Y,-1))
    // {
    //     tx-=2;
    // }
    // if(tx<2 && JUDGE_BLACK(1,pointStar.Y,-1) )
    // {/*!< 认为到达上边界 */
    //     while(pointStar.Y>pointCenter.Y && JUDGE_BLACK(tx,pointStar.Y,-160))
    //     {
    //         pointStar.Y-=4;
    //     }
    //     if(pointStar.Y <= pointCenter.Y)
    //     {
    //         pointStar.X = pointLeft.X;
    //         pointStar.Y = pointLeft.Y;
    //     }else
    //     {
    //         pointStar.X = tx;
    //         pointStar.Y = (JUDEG_XOR(tx,pointStar.Y,-160))?(pointStar.Y):(pointStar.Y-2);
    //     }
    // }else
    // {
    //     pointStar.X = (JUDGE_BLACK(tx-1,pointStar.Y,1))?(tx):(tx-1);
    // }
    // /*!< stop  点 */
    // pointStop.Y = pointUp.Y;
    // ty =  pointUp.Y;
    // pointStop.X = 79;
    // while(ty>3 && JUDGE_BLACK(pointStop.X,ty,-160))
    // {
    //     ty -=4;
    // }
    // if(ty<4)
    // {/*!< ERROR   */
    //     pointStop.Y = pointUp.Y;
    //     pointStop.X = pointUp.X;
    // }else
    // {
    //     pointStop.Y = (JUDEG_XOR(pointStop.X,ty,-160))?(ty-2):(ty);
    // }
    // /*!< 矩阵填充 求算邪率 */
    // slope[0] = MDL_GetSlop(&pointUp,&pointLeft);
    // slope[1] = MDL_GetSlop(&pointStar,&pointStop);
    slope = MDL_GetSlop(&pointCenter,&pointBase);
#ifdef mex_h

    printf("right slope %d , %d ,%d ,%d ", slope[0] ,slope[1],slope[2],(slope[0] +slope[1]+slope[2])/3);

#endif
//	MDL_Slopes= slope[1] ;
    lastSlope = slope;
    if(lastFlag != roadFlag)
    {
        last2Flag = lastFlag;
        lastFlag = roadFlag;
    }
	return slope;
}



/**********************************************************************************
* 函数名称: MDL_StraightProcess
* 函数功能: 直道下舵机输出斜率 通过观察其图片的重心来获取当前应当要的角度
* 函数输入: void input:
* 函数输出: void output:
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro MDL_StraightProcess(void)
{
    volatile int16_Pro slope;
    POINT tempPoint;
    tempPoint.Y = pointCenter.Y;
    slope = MDL_GetSlop(&pointCenter,&pointBase);
   if((lastFlag == MDL_ROAD_CircleLeft) && (pointLeft.X == 0))
    {
        // tempPoint.X = (pointCenter.X<39)?(pointCenter.X):((pointUp.Y>8)?(36):(25));
		// slope = 0;
        
        slope = (pointUp.Y>8)?(slope):(lastSlope/3*2);
    }else if((lastFlag == MDL_ROAD_CircleRight) &&(pointRight.X ==79))
    {
        // tempPoint.X = (pointCenter.X>39)?(pointCenter.X):((pointUp.Y>8)?(42):(53));
		// slope = 0;
        slope =(pointUp.Y>8)?(slope):(lastSlope/3*2);
        
//        pointRight.X -= 4;
    }else if(CircleFlag & 0x8000)
            {
                if((last2Flag == MDL_ROAD_CircleLeft || last2Flag == MDL_ROAD_CircleRight) && CircleFlag<DELAY_ROAD)
                {
                    tempPoint.X = (last2Flag == MDL_ROAD_CircleLeft)?(((pointRight.X>50) && (pointLeft.X == 0))?
                                                                            (pointRight.X-20):(pointCenter.X) )
                                                                    :((pointLeft.X<30 && (pointRight.X == 0))?
                                                                            (pointLeft.X+20):(pointCenter.X));
                    tempPoint.Y = pointCenter.Y;
        //            pointBase.Y = pointCenter.Y;
                    slope = MDL_GetSlop(&tempPoint,&pointBase);
        //            pointBase.Y = 59;
                    
                }
            }else if(BlockFlag & 0x8000)
            {
                tempPoint.Y = pointCenter.Y;
    
                if(pointCenter.X>39 && (pointDownLeft.X+pointDownRight.X)>80)
                {
                    tempPoint.X = pointCenter.X-9+((BlockFlag&0x00FF)/7);
                }else
                {
                    tempPoint.X = pointCenter.X+9-((BlockFlag&0x00FF)/7);
                }
                slope = MDL_GetSlop(&tempPoint,&pointBase);
                
            }else
            {
                tempPoint.X = (pointDownLeft.X+pointDownRight.X)>>1;
                tempPoint.Y = pointCenter.Y;
                pointBase.X = (79-tempPoint.X+39)/2;
                slope = MDL_GetSlop(&pointCenter,&pointBase);
                pointBase.X = 39;
            }
    /*!< 重心判断方向 */
       
    
#ifdef mex_h
    printf("straight : %d " ,slope);
#endif
    
    
//    MDL_Slopes= slope ;
	return slope;

}


/**********************************************************************************
* 函数名称: MDL_FindUPPoint
* 函数功能: 根据传输进来的基准点向上寻找最靠近黑点的白点坐标 以写进结构体指针中
* 函数输入: POINT 结构体 input:
* 函数输出:  output:void
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
inline void MDL_FindUPPoint(POINT* targetPoint)
{
    int8_Pro tx,ty;
    tx = targetPoint->X;
    ty = targetPoint->Y;
    while(ty>1 && JUDGE_BLACK(tx,ty,-160))
    {
        ty -= 4;
    }               /*!< 两者有一点 为黑 */
//    ty =(JUDEG_XOR(tx,ty,-160))?(ty):(ty-2);/*!< 相同 则最下点为起始黑 不想通 则上点为起始黑  */
    ty = (ty<2)?
            ((JUDEG_GET(tx,0) == WHITE)?(0):((JUDEG_GET(tx,1)== WHITE)?(1):(((JUDEG_GET(tx,2)== WHITE))?(2):(3)))):
            ((JUDEG_XOR(tx,ty+1,-160))?((JUDEG_GET(tx,ty)== WHITE)?(ty-1):(ty+2)):((JUDEG_GET(tx,ty) == WHITE)?(ty):(ty+1)));
    targetPoint->X = tx;
    targetPoint->Y = ty;
}

/**********************************************************************************
* 函数名称: MDL_FindDownPoint
* 函数功能: 根据传输进来的基准点向下寻找最靠近黑点的白点坐标 以写进结构体指针中
* 函数输入: POINT 结构体 input:
* 函数输出:  output:void
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
inline void MDL_FindDownPoint(POINT* targetPoint)
{
    int8_Pro tx,ty;
    tx = targetPoint->X;
    ty = targetPoint->Y;
    while(ty<58 && JUDGE_BLACK(tx,ty,160))
    {
        ty += 4;
    }               /*!< 两者有一点 为黑 */
//    ty =(JUDEG_XOR(tx,ty,-160))?(ty):(ty-2);/*!< 相同 则最下点为起始黑 不想通 则上点为起始黑  */
    ty = (ty>57)?
            ((JUDEG_GET(tx,59) == WHITE)?(59):((JUDEG_GET(tx,58)== WHITE)?(58):(((JUDEG_GET(tx,57)== WHITE))?(57):(56)))):
            ((JUDEG_XOR(tx,ty-1,160))?((JUDEG_GET(tx,ty)== WHITE)?(ty+1):(ty-2)):((JUDEG_GET(tx,ty) == WHITE)?(ty):(ty-1)));
    targetPoint->X = tx;
    targetPoint->Y = ty;
}

/**********************************************************************************
* 函数名称: MDL_FindLeftPoint
* 函数功能: 根据传输进来的基准点向左寻找最靠近黑点的白点坐标 以写进结构体指针中
* 函数输入: POINT 结构体 input:
* 函数输出:  output:void
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
inline void MDL_FindLeftPoint(POINT* targetPoint)
{
    int8_Pro tx,ty;
    tx = targetPoint->X;
    ty = targetPoint->Y;
    while(tx>1 && JUDGE_BLACK(tx,ty,-2))
    {
        tx -= 4;
    }
    tx = (tx<2)?
        ((JUDEG_GET(0,ty) == WHITE)?(0):((JUDEG_GET(1,ty)== WHITE)?(1):(((JUDEG_GET(2,ty)== WHITE))?(2):(3)))):
        ((JUDEG_XOR(tx+1,ty,-2))?((JUDEG_GET(tx,ty)== WHITE)?(tx-1):(tx+2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx+1)));
    targetPoint->X = tx;
    targetPoint->Y = ty;    
}

/**********************************************************************************
* 函数名称: MDL_FindRightPoint
* 函数功能: 根据传输进来的基准点向右寻找最靠近黑点的白点坐标 以写进结构体指针中
* 函数输入: POINT 结构体 input:
* 函数输出:  output:void
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
inline void MDL_FindRightPoint(POINT* targetPoint)
{
    int8_Pro tx,ty;
    tx = targetPoint->X;
    ty = targetPoint->Y;
    while(tx<78 && JUDGE_BLACK(tx,ty,2))
    {
        tx += 4;
    }
    tx = (tx<78)?
        ((JUDEG_XOR(tx-1,ty,2))?((JUDEG_GET(tx,ty)== WHITE)?(tx+1):(tx-2)):((JUDEG_GET(tx,ty) == WHITE)?(tx):(tx-1))):
        ((JUDEG_GET(79,ty) == WHITE)?(79):((JUDEG_GET(78,ty)== WHITE)?(78):(((JUDEG_GET(77,ty)== WHITE))?(77):(76))));
    targetPoint->X = tx;
    targetPoint->Y = ty;       
}


/**********************************************************************************
* 函数名称: MDL_GetSlop
* 函数功能: 给出两点求出斜率
* 函数输入: void input:点1  点2 
* 函数输出: void output:斜率
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro MDL_GetSlop(POINT* point1,POINT* point2)
{
    Float_Pro slop;
    slop = ((Float_Pro)(point1->X-point2->X))/((Float_Pro)(point1->Y-point2->Y));
#ifdef mex_h
    printf("point1(%d,%d),point2(%d,%d),dY: %d dX:%d slope %f \n",point1->X,point1->Y,point2->X,point2->Y,point1->Y-point2->Y,point1->X-point2->X,slop);
#endif

    return findingTable(slop);
}


/**********************************************************************************
* 函数名称: sort_finding
* 函数功能: 递归快速寻找算法 
* 函数输入: void input:开始数组下标 结束 数组下标 寻找值
* 函数输出: void output:当先 tan 值 对应的角度 0-50度之间
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro sort_finding(int16_Pro star,int16_Pro stop,Float_Pro value)
{
	int16_Pro mid = (star+stop)/2;
	if(star+1 == stop)
	{
		return angTable[mid].angle;
	}else
	{
		if(angTable[mid].value > value)
		{
			stop = mid;
			return sort_finding(star,stop,value);
		}else
		{
			star = mid;
			return sort_finding(star,stop,value);
		}
	}
}

/**********************************************************************************
* 函数名称: findingTable
* 函数功能: 根据 tan 值 查表求算其 角度值
* 函数输入: void input:tan值
* 函数输出: void output:角度值
* 作者 :author:@Kim_alittle_star
**********************************************************************************/
int16_Pro findingTable(Float_Pro value)
{
	if(value < 0)
	return -(sort_finding(0,500,-value));
	else
	return sort_finding(0,500,value);
}
