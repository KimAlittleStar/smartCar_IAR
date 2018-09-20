/****************************************************************************
* 文件名称 : Img_Process.c
* 文件功能 : 图像识别，将摄像头识别到的图像根据不同的赛道元素 采用不同的算法情况
*			进行处理，最后得到一条中线
* 注意事项 :
*****************************************************************************/

#include "IMG_Process.h"
#include "WIFIDrive.h"
#include "PID_Process.h"


int speedmin=200,speedmax=230;
int slope,speed;//舵机增量  
enum IMG_Road_Sign IMG_Road_sign=IMG_Road_Straight;//处于赛道的类型  
enum IMG_Road_Sign IMG_Road_sign_late=IMG_Road_Straight;//上次处于赛道的类型（不包括直道）


enum IMG_Road_Sign IMGsign;
char IMGend=0;
char date=0; 











 

/************************************************************************  
*函数名：IMG_GetSign
*作用：计算出数据
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
    
void IMG_GetSign(){
    /********判断********/ 
    char x,y=0,i;
    // PTE24_OUT=1; 
   // PTE24_OUT=0;
     
    IMG_Forecast(); 
   if(IMG_FindPoint(0,59,1,79,WITHE)==79)
   {
            IMG_Road_sign=IMG_Road_End;
   }
    switch(IMG_Road_sign)
    {
        case IMG_Road_Straight:
        PTE24_OUT=0;
            IMG_Straight();

        if(IMGend==1 && IMG_End(50)==IMG_Road_End  || IMGend==1 && IMG_End(59)==IMG_Road_End)
        {
            speed=0;
            IMG_Road_sign=IMG_Road_End;
        }else{
            y=IMG_FindPoint(40,30,0,0,BLACK);

            if(y>3)
            {
            speed=speedmin;
            }
            else
            {
                    speed=speedmax+30;
            }   
        }
             
 
        break;
        case IMG_Road_Left:
          //  PTE24_OUT=1;
             IMG_Left();
             speed=speedmin ;
            
        break;
        
        
        case IMG_Road_Right:
           // PTE24_OUT=1;
             IMG_Right();
                    speed=speedmin; 
        break;

        
        case IMG_Road_Cross:
        
            IMG_Cross();
                    speed=speedmin; 
        break;
        

        case IMG_Road_CirqueLeft: 
            IMGend=1;
            IMG_CirqueLeft();
                    speed=speedmin; 
        break;
        
        
        case IMG_Road_CirqueRight:
        
            IMGend=1;
            IMG_CirqueRight();
            speed=speedmin; 
        break; 
        
        case IMG_Road_End:
            speed=0; 
            PTE24_OUT=1; 
            IMG_Straight();
        break;
    }

    if(slope>450){
        slope=4500;
        
    }else if(slope<-450)
    {
        slope=-450; 
    }
    i=slope;
    if(slope<0)i=-slope;

     if(IMG_Road_sign!=IMG_Road_Straight && IMG_Road_sign!=IMG_Road_End)
     {
         speed=speedmin+i/45*(speedmax-speedmin)/100;
     }
     Motor_div.setPoint=speed; 

   
}
/************************************************************************  
*函数名：IMG_Forecast_Straight
*作用：判断直道道路函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
enum IMG_Road_Sign IMG_Forecast_Straight(char lenth)
{
    char x,y,i;
    int pointleft=0,pointRight=79;
    int pointx[2],pointy[2];
    
    i=1;
    for(y=59;y>lenth;y-=3)
    {
        if(BMP(40,y)==BLACK)
        {
        i=0; 
        break; 
        }
    }
    if(i)  return IMG_Road_Straight;

return IMG_Road_sign;
}
/************************************************************************  
*函数名：IMG_Forecast_LeftRight
*作用：判断左、右弯道道路函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/

enum IMG_Road_Sign IMG_Forecast_LeftRight()
{ 
    char x,y=0 ;
    int pointleft=0,pointRight=79;
    int pointx[2],pointy[2];
        for(y=50;y>=25;y--)
        {
             if(BMP(40,y)==BLACK)
             {
                 break;
             }
            
        }
        if(y>25)
        {//中间检测到黑点，判断是否为左、右弯道
            y+=2;
            for(x=40;x<79;x++)
            {
                if(BMP(x,y)==BLACK)
                {
                    pointRight=x;
                    break;
                }
            }
            for(x=40;x>0;x--)
            {
                if(BMP(x,y)==BLACK){
                    pointleft=x;
                    break;
                }
            }

            if(pointleft>0&&pointRight==79 ){
              return IMG_Road_Right; 
            }
            else if(pointleft==0&&pointRight<79 )
            {
               return IMG_Road_Left;
            } 
        }

return IMG_Road_sign;
}
 /************************************************************************  
*函数名：IMG_Forecast_Cross
*作用：判断十字道道路函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/

enum IMG_Road_Sign IMG_Forecast_Cross()
{ 
    char x,y=0,i;
    /*int pointleft=0,pointRight=79;
    int pointx[2],pointy[2];*/

    for(x=0;x<40;x++)
        {

            for(y=35;y<45;y+=2)
            {
                if(BMP(39-x,y)==BLACK)
                { 
                    i=0;
                    goto loop;
                    break;
                }
                if(BMP(40+x,y)==BLACK)
                {
                    i=1;
                    goto loop;
                    break;
                }
            }
            
        }
        loop:
        if(x==40)
        {
             return IMG_Road_Cross; 
        }
return IMG_Road_sign;
}

 /************************************************************************  
*函数名：IMG_Forecast_CirqueLeft
*作用：判断左环岛道路函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/

enum IMG_Road_Sign  IMG_Forecast_CirqueLeft() 
{
    char x,y=0;
    char  pointleft=0,pointRight=79;
    char  pointx[5],pointy[5];
            pointleft=1;
            for(y=59;y>50;y--)
            {
                if(BMP(0,y)==BLACK)pointleft=0; 
            }
            if(pointleft)
            {
                pointy[0]=IMG_FindPoint(0,50,0,0,BLACK); 
                pointy[1]=IMG_FindPoint(0,pointy[0],0,0,WITHE); 
                pointy[2]=IMG_FindPoint(0,pointy[1],0,0,BLACK); 

                if(pointy[1]>3 && IMG_FindPoint(0,pointy[0]+1,1,40,BLACK)==40 && IMG_FindPoint(0,pointy[1]-1,1,40,BLACK)==40 && pointy[2]!=0  )
                {
                             pointy[3]=(pointy[0]-pointy[1])/2+pointy[1];//圆形中间的点
                             pointy[0]-=2;
                             pointy[1]+=2;//消除边缘影响
                                    pointx[0]=IMG_FindPoint(0,pointy[0],1,79,WITHE);
                                    pointx[1]=IMG_FindPoint(0,pointy[1],1,79,WITHE);
                                    pointx[3]=IMG_FindPoint(0,pointy[3],1,79,WITHE);
                                                if(pointx[3]>5 && pointx[1]+2<pointx[3] && pointy[3]>10 && pointx[0]<10 )
                                                {
                                                    pointx[4]=IMG_FindPoint(pointx[3],pointy[3],1,79,BLACK);
                                                    if(IMG_FindPoint(pointx[3]+(pointx[4]-pointx[3])/2,30,0,3,BLACK)==3  && IMG_FindPoint(pointx[4]-2,pointy[3],0,59,BLACK)==59)
                                                    {
                                                        return IMG_Road_CirqueLeft;
                                                    }
                                                }
                 }
            }
return IMG_Road_sign;
}



 /************************************************************************  
*函数名：IMG_Forecast_CirqueRight
*作用：判断右环岛道路函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/

enum IMG_Road_Sign IMG_Forecast_CirqueRight()
{
    char x,y=0 ;
    char  pointleft=0,pointRight=79;
    char pointx[5],pointy[5]; 
            pointRight=1;
            for(y=59;y>50;y--)
            {
                if(BMP(79,y)==BLACK)pointRight=0; 
            }
            if(pointRight)
            {
                pointy[0]=IMG_FindPoint(79,50,0,0,BLACK);
                pointy[1]=IMG_FindPoint(79,pointy[0],0,0,WITHE);
                pointy[2]=IMG_FindPoint(79,pointy[1],0,0,BLACK);
                if(pointy[1]>3 && IMG_FindPoint(79,pointy[0]+1,1,40,BLACK)==40 && IMG_FindPoint(79,pointy[1]-1,1,40,BLACK)==40 && pointy[2]!=0)
                {
                             pointy[3]=(pointy[0]-pointy[1])/2+pointy[1];
                             pointy[0]-=2;
                             pointy[1]+=2;//消除边缘影响

                                    pointx[0]=IMG_FindPoint(79,pointy[0],1,0,WITHE);
                                    pointx[1]=IMG_FindPoint(79,pointy[1],1,0,WITHE);
                                    pointx[3]=IMG_FindPoint(79,pointy[3],1,0,WITHE);
                                                if(pointx[1]+2>pointx[3] && pointx[3]<75 && pointy[3]>10 && pointx[0]>70)
                                                {
                                                    pointx[4]=IMG_FindPoint(pointx[3],pointy[3],1,0,BLACK);
                                                    if(IMG_FindPoint(pointx[4]+(pointx[3]-pointx[4])/2,30,0,3,BLACK)==3 && IMG_FindPoint(pointx[4]+2,pointy[3],0,59,BLACK)==59)
                                                    {
                                                    return IMG_Road_CirqueRight;
                                                    }
                                                }  
                }
            }

return IMG_Road_sign;
}


/************************************************************************  
*函数名：IMG_End
*作用：判断终点函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年07月12日 
* 修改人：
* 修改日期：
************************************************************************/
enum IMG_Road_Sign IMG_End(int y)
{
char x,i,wstart,bx1=20,bx2=20,bx3=20;
        
        if(BMP(20,y)==WITHE)
        {
            wstart=IMG_FindPoint(20,y,1,79,BLACK);
            
            bx1=IMG_FindPoint(wstart,y,1,79,WITHE); 
            bx2=IMG_FindPoint(bx1,y,1,79,BLACK);
            bx3=IMG_FindPoint(bx2,y,1,79,WITHE);
        }else{
            wstart=IMG_FindPoint(20,y,1,79,WITHE);

            bx1=IMG_FindPoint(wstart,y,1,79,BLACK); 
            bx2=IMG_FindPoint(bx1,y,1,79,WITHE);
            bx3=IMG_FindPoint(bx2,y,1,79,BLACK);
        }

        if(bx1!=bx2 && bx3!=bx2 && bx1 && bx3<60 && IMG_Parity(bx1,bx2)>3 && IMG_Parity(bx2,bx3)>3 &&  IMG_Parity(IMG_Parity(bx1,bx2),IMG_Parity(bx2,bx3))<3 )
        {
            return IMG_Road_End;
        }


return IMG_Road_sign;   
}






















/************************************************************************  
*函数名：IMG_Forecast
*作用：判断道路函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
void  IMG_Forecast(){ 
    char x,y=0;
    enum IMG_Road_Sign dateSign[5];
    int pointleft=0,pointRight=79;
    int pointx[2],pointy[2];
    
        /********* IMG_Road_sign=直道 **************/ 
    if(IMG_Road_sign==IMG_Road_Straight)
    {
        IMG_Road_sign_late=IMG_Road_Straight;
         IMG_Road_sign= IMG_Forecast_LeftRight();//左右弯道判断 

         IMG_Road_sign= IMG_Forecast_Cross();//十字判断 

         IMG_Road_sign= IMG_Forecast_CirqueLeft();//左环岛判断  

         IMG_Road_sign= IMG_Forecast_CirqueRight();//右环岛判断
        /* if( dateSign[2]==IMG_Road_CirqueLeft  &&  dateSign[3]==IMG_Road_CirqueRight)
         {

         }*/

    }

    else if(IMG_Road_sign==IMG_Road_Cross)
    {
         if(IMG_Road_sign_late==IMG_Road_Straight)
         {
                IMG_Road_sign = IMG_Forecast_CirqueLeft();//左环岛判断  
                IMG_Road_sign = IMG_Forecast_CirqueRight();//右环岛判断 
                if(IMG_Road_sign!=IMG_Road_CirqueLeft && IMG_Road_sign!=IMG_Road_CirqueRight)
                {
                    IMG_Road_sign=IMG_Forecast_LeftRight();//左右弯道判断
                    if(IMG_Road_sign==IMG_Road_Left )
                    {
                        IMG_Road_sign=IMG_Road_Cross;
                        IMG_Road_sign_late=IMG_Road_Left;
                    }
                     if(  IMG_Road_sign==IMG_Road_Right)
                    {
                        IMG_Road_sign=IMG_Road_Cross;
                        IMG_Road_sign_late=IMG_Road_Right;
                    }
                }
         } 
    }
    
     /********* IMG_Road_sign=左、右弯道    **************/
    else if(IMG_Road_sign==IMG_Road_Left||IMG_Road_sign==IMG_Road_Right)
    {
        if(IMG_Road_Straight==IMG_Forecast_Straight(30))
        {
            IMG_Road_sign=IMG_Road_Straight;
        }
        /*else
        {
        IMG_Road_sign=IMG_Forecast_LeftRight();
        }*/
    }

    /********* IMG_Road_sign=左、右环岛道     **************/  
    else if(IMG_Road_sign==IMG_Road_CirqueLeft )
    {
        
        pointy[0]=-1;
        pointy[1]=-1;
        pointy[2]=-1;
        if(IMG_Road_sign_late!=IMG_Road_Left)
        {
             if(IMG_Road_CirqueLeft!=IMG_Forecast_LeftRight() || IMG_Road_Straight!=IMG_Forecast_Straight(8))
            {
                IMG_Road_sign=IMG_Road_Straight;
            }
            else
            {
                for(y=59;y>0;y--)
                {
                    if(BMP(0,y)==BLACK)
                    {
                        pointy[0]=y;
                        for(;y>0;y--)
                        {
                            if(BMP(0,y)==WITHE)
                            {
                                pointy[1]=y;
                                for(;y>0;y--)
                                {
                                    if(BMP(0,y)==BLACK )
                                    {
                                        pointy[2]=y;
                                        break;
                                    }
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                if(pointy[0]>pointy[1] && pointy[1]>pointy[2] )
                {
                    pointy[2]=pointy[1]+(pointy[0]-pointy[1])/2;
                    y=pointy[2];
                    for(x=0;x<79;x++)
                    {
                        if(BMP(x,y)==WITHE)
                        {
                            pointx[2]=x-1;
                            for(;x<79;x++)
                            {
                                if(BMP(x,y)==BLACK || x==78)
                                {
                                    pointRight=79-x; 
                                    slope=(pointx[2]-pointRight)*8;  
                                break;
                                }
                            }
                            break;
                        }
                    }
                } 
            }
        }
        if(IMG_Road_sign_late==IMG_Road_Straight)
        {
            IMG_Road_sign=0;
           /*IMG_Road_sign= IMG_Forecast_Cross();//十字判断
              dateSign[0] =IMG_Road_sign;*/
             IMG_Road_sign= IMG_Forecast_CirqueLeft(); 
            dateSign[1] =IMG_Road_sign;
            if(/*dateSign[0]==IMG_Road_Cross && */dateSign[1]==IMG_Road_CirqueLeft )
            {
                IMG_Road_sign_late=IMG_Road_Cross;
            }
            else
            {
                IMG_Road_sign=IMG_Road_Straight;
            }
        }
       else if(IMG_Road_sign_late==IMG_Road_Cross && pointy[0]>35)
        {
                    IMG_Road_sign_late=IMG_Road_CirqueLeft;
        }
        else if(pointy[0] >10 && pointy[0] <30 && IMG_Road_sign_late==IMG_Road_CirqueLeft)//进入左环岛
        {
            IMG_Road_sign=IMG_Road_Cross;
            if(IMG_Road_CirqueLeft!=IMG_Forecast_CirqueLeft() && IMG_Forecast_Straight(8)==IMG_Road_Straight )
            {
            IMG_Road_sign_late=IMG_Road_Left; 
            }
            else 
            {
            IMG_Road_sign_late=IMG_Road_Straight;
            } 
            IMG_Road_sign=IMG_Road_CirqueLeft;
        }
        
    }
     /********* IMG_Road_sign=左、右环岛道     **************/  
    else if(IMG_Road_sign==IMG_Road_CirqueRight )
    {
        pointy[0]=59;
        pointy[1]=59;
        pointy[2]=59;//计算角度     
    if(IMG_Road_sign_late!=IMG_Road_Right)
    {
        if(IMG_Road_CirqueRight!=IMG_Forecast_LeftRight() || IMG_Road_Straight!=IMG_Forecast_Straight(8))
        {
            IMG_Road_sign=IMG_Road_Straight;
        }
        else
        {
            for(y=59;y>=0;y--)
            {
                if(BMP(79,y)==BLACK)
                {
                    pointy[0]=y;
                    for(;y>0;y--)
                    {
                        if(BMP(79,y)==WITHE )
                        {
                            pointy[1]=y;
                            for(;y>0;y--)
                            {
                                if(BMP(79,y)==BLACK)
                                {
                                    pointy[2]=y;
                                    break;
                                }
                            }
                            break;
                        }
                    }
                    break;
                }
            }
            if(pointy[0]>pointy[1] && pointy[1]>pointy[2] )
            {
                pointy[2]=pointy[1]+(pointy[0]-pointy[1])/2;
                y=pointy[2];
                for(x=79;x>=0;x--)
                {
                    if(BMP(x,y)==WITHE)
                    {
                        pointx[2]=x+1;
                        for(;x>=0;x--)
                        {
                            if(BMP(x,y)==BLACK || x==0)
                            {
                            pointleft=79-pointx[2];
                            slope=(x-pointleft)*8;
                            break;
                            }
                        }
                        break;
                    }

                }
            }
        }
    } 
        //计算角度


        //date确定是否为环岛    
        
        if(IMG_Road_sign_late==IMG_Road_Straight)
        {
            IMG_Road_sign=IMG_Road_Straight;
            
            IMG_Road_sign=IMG_Forecast_CirqueRight();
            dateSign[1]=IMG_Road_sign;
            if( dateSign[1]==IMG_Road_CirqueRight )
            {
                IMG_Road_sign_late=IMG_Road_Cross;
            }
            else
            {
                IMG_Road_sign=IMG_Road_Straight;
            }
        }
        else if(IMG_Road_sign_late==IMG_Road_Cross && pointy[0]>35)
        {
                IMG_Road_sign_late=IMG_Road_CirqueRight;
                IMG_Road_sign=IMG_Forecast_LeftRight();
        }
        else if(pointy[0]<30 && pointy[0]>10 && IMG_Road_sign_late==IMG_Road_CirqueRight )//进入右环岛
        {
            IMG_Road_sign=IMG_Road_Cross;
            if(IMG_Forecast_CirqueRight()!=IMG_Road_CirqueRight && IMG_Forecast_Straight(8)==IMG_Road_Straight)
            {

                IMG_Road_sign_late=IMG_Road_Right;
            }
            else
            {
            IMG_Road_sign_late=IMG_Road_Straight;
            }

            IMG_Road_sign=IMG_Road_CirqueRight;
        }
    }
}





















































 
/************************************************************************  
*函数名：IMG_Straight
*作用：直道处理函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
void IMG_Straight(){
    char x,y,i;
    int pointLeft,pointRight; 
    pointRight=0;
    pointLeft=0;

     if(BMP(30,40)==WITHE)
    {
    pointLeft=IMG_FindPoint(30,40,1,0,BLACK);
    pointRight=79-IMG_FindPoint(30,40,1,79,BLACK); 
    slope=(pointLeft-pointRight)*10; 
    }else if(BMP(50,40)==WITHE)
    {
    pointLeft=IMG_FindPoint(50,40,1,0,BLACK);
    pointRight=79-IMG_FindPoint(50,40,1,79,BLACK); 
    slope=(pointLeft-pointRight)*10; 
    }else{ 
    pointLeft=IMG_FindPoint(40,40,1,0,BLACK);
    pointRight=79-IMG_FindPoint(40,40,1,79,BLACK); 
    slope=(pointLeft-pointRight)*10; 
    }


    if(BMP(0,40)==WITHE  && BMP(79,40)==WITHE)
    {
    pointLeft=IMG_FindPoint(40,55,1,0,BLACK); 
    pointRight=79-IMG_FindPoint(40,55,1,79,BLACK);     
    
    slope+=(pointLeft-pointRight)*11;
     }
    
   
}

  
/************************************************************************  
*函数名：IMG_Right
*作用：右赛道处理函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
void IMG_Right(){
    char x=79,y=59;
    char middley;
    
     
    
    /*********计算角度*********/
    
    if(BMP(x,y)==BLACK)
    {
        x=IMG_FindPoint(x,y,1,0,WITHE);
    }
    
    x=IMG_FindPoint(x,y,1,0,BLACK);
    middley=IMG_FindPoint(x+(79-x)/2,y,0,0,BLACK);
    y=IMG_FindPoint(79,y,0,0,BLACK);
    if(y>middley && y>40)
    {
        y=middley*2/3;
    }

 
    
    slope=(y+x)*10; 

}
 
/************************************************************************  
*函数名：IMG_Left
*作用：左赛道处理函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
void IMG_Left(){ 
    char x=0,y=59;
    char middley;
    
     
    
    /*********计算角度*********/
    
    if(BMP(x,y)==BLACK)
    {
        x=IMG_FindPoint(x,y,1,79,WITHE);
    }
    
    x=IMG_FindPoint(x,y,1,79,BLACK);
    middley=IMG_FindPoint(x/2,y,0,0,BLACK);
    y=IMG_FindPoint(0,y,0,0,BLACK);
    if(y>middley  && y>40)
    {
        y=middley*2/3;
    }

    x=79-x; 

    slope=-(y+x)*10; 
}
/************************************************************************  
*函数名：IMG_Cross
*作用：十字赛道处理函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月08日 
* 修改人：
* 修改日期：
************************************************************************/
void IMG_Cross(){
    char x,y;
    unsigned char pointleft=0,pointRight=0; 
    if(IMG_Road_sign_late==IMG_Road_Straight)
    {
        
      for(y=40;y>15;y--)
            {
                if(BMP(5,y)==BLACK)
                {
                    y-=2;
                    pointleft=0;
                    pointRight=0;


                    pointleft=IMG_FindPoint(40,y,1,0,BLACK); 
                    pointRight=79-IMG_FindPoint(40,y,1,79,BLACK);
                    

                    slope=(pointleft-pointRight)*9;
                    break;
                }
                if(BMP(75,y)==BLACK)
                {
                    y-=2;
                    pointleft=0;
                    pointRight=0;

                    pointleft=IMG_FindPoint(40,y,1,0,BLACK);
                    pointRight=IMG_FindPoint(40,y,1,79,BLACK); 

                    pointRight=79-pointRight;
                    slope=(pointleft-pointRight)*9; 
                    break;
                }
            }
            if(slope>20)slope=20;
            if(slope<-20)slope=-20;
    }
    else if(IMG_Road_sign_late==IMG_Road_Left)
    {
        IMG_Left();
        if(IMG_Forecast_Straight(10)==IMG_Road_Straight)
        {
            IMG_Road_sign =IMG_Road_Cross;
            IMG_Road_sign_late=IMG_Road_Cross;
        }
    }
    else if(IMG_Road_sign_late==IMG_Road_Right)
    {
         IMG_Right();
        if(IMG_Forecast_Straight(10)==IMG_Road_Straight)
        {
            IMG_Road_sign =IMG_Road_Cross;
            IMG_Road_sign_late=IMG_Road_Cross;
        }
    }
    else if(IMG_Road_sign_late==IMG_Road_Cross)
    {
        IMG_Road_sign_late=IMG_Road_Straight;
        IMG_Cross();
        IMG_Road_sign_late=IMG_Road_Cross;
        IMG_Road_sign=0;
        if(IMG_Forecast_Cross()!=IMG_Road_Cross)
        {
            IMG_Straight();
            IMG_Road_sign=IMG_Road_Straight; 
            IMG_Road_sign_late=IMG_Road_Straight;
        }
        else{
            IMG_Road_sign=IMG_Road_Cross;
        }
    }
 
              
}

/************************************************************************  
*函数名：IMG_CirqueLeft
*作用：左圆环赛道处理函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月09日 
* 修改人：
* 修改日期：
************************************************************************/
void IMG_CirqueLeft()
{  
    char x,y,pointleft,pointRight;
    if(IMG_Road_sign_late==IMG_Road_Left)
    {
        if(date==0)
        {
            if(IMG_Forecast_LeftRight()==IMG_Road_Left)
            {
                date=1; 
            }
            else
            {
                IMG_Left(); 
                 //slope/=2;
                //可适当加大角度
                     slope-=80;
            }
        }
        if(date==1)
        {
            if(IMG_Road_Straight==IMG_Forecast_Straight(10) &&   IMG_FindPoint(40,30,1,0,BLACK)<IMG_FindPoint(40,25,1,0,BLACK) 
                    &&  IMG_FindPoint(40,30,1,79,BLACK)>IMG_FindPoint(40,25,1,79,BLACK))
            {
                    date=2;  
            }
            else
            { 
             IMG_Left(); 
                    if(IMG_FindPoint(0,50,1,79,BLACK)>=77)
                    {
                     slope-=100;
                    }
            }
            
        }
        if(date==2)
        {
            y=IMG_FindPoint(0,59,0,0,BLACK);
                if(y>40)
                {
                        IMG_Road_sign=IMG_Road_Straight;
                        IMG_Road_sign_late=IMG_Road_Straight;
                        IMG_Straight();
                        date=0;
                }else{
                    pointleft=IMG_FindPoint(40,y,1,0,BLACK);
                    pointRight=79-IMG_FindPoint(40,y,1,79,BLACK);
                    slope=(pointleft-pointRight)*7;
                }

        }
    }

}
/************************************************************************  
*函数名：IMG_CirqueRight
*作用：右圆环赛道处理函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年05月09日 
* 修改人：
* 修改日期：
************************************************************************/
void IMG_CirqueRight()
{ 
    char x,y,pointleft,pointRight;
 
        if(IMG_Road_sign_late==IMG_Road_Right)
        {
            if(date==0)
            {
                 if(IMG_Forecast_LeftRight()==IMG_Road_Right)
                {
                    date=1; 
                }
                else{ 
                     IMG_Right();
                     //slope/=2;
                     slope+=80;
                     //可适当加大角度
                }
            }
            if(date==1)
            {
                if(IMG_Road_Straight==IMG_Forecast_Straight(10) && IMG_FindPoint(40,30,1,0,BLACK)<IMG_FindPoint(40,25,1,0,BLACK)
                &&  IMG_FindPoint(40,30,1,79,BLACK)>IMG_FindPoint(40,25,1,79,BLACK))
                {
                        date=2;
                }
                else
                {
                    IMG_Right(); 
                    if(IMG_FindPoint(0,50,1,79,BLACK)>=77)
                    {
                     slope+=100;
                    }
                }
            }
            if(date==2)
            {
                y=IMG_FindPoint(79,59,0,0,BLACK);
                if(y>40)
                {
                        IMG_Road_sign=IMG_Road_Straight;
                        IMG_Road_sign_late=IMG_Road_Straight;
                        IMG_Straight();
                        date=0;
                }else{
                    pointleft=IMG_FindPoint(40,y,1,0,BLACK);
                    pointRight=79-IMG_FindPoint(40,y,1,79,BLACK);
                    slope=(pointleft-pointRight)*7;

                }
            }
        }

}

/************************************************************************  
*       IMG_FindPoint
*作用：
*输入： 从px,py点开始需找
        dir: 
            0纵向寻找 
            1横向寻找
        len寻找的位置
        colo寻找的颜色
*输出：寻找到的点的x坐标或y坐标    
*      -1未找到      
*  
* 作    者：墨水瓶
* 完成日期：2018年05月09日 
* 修改人：
* 修改日期：
************************************************************************/

char IMG_FindPoint(char px,char py,char dir,char len,unsigned char color  )
{
    
    if(dir==0)
    {
        if(py>len)
        {
            for(;py>len;py--)
            {
                if( BMP(px,py)==color)
                {
                    return py;
                }
            }
        }else if(py<len){
            for(;py<len;py++)
            {
                if(BMP(px,py)==color)
                {
                    return py;
                }
            } 


        }
        if(len==py)
        {
            return py;
        }

    }
    else
    {
        if(px>len)
        {
            for(;px>len;px--)
            {
                if(BMP(px,py)==color)
                {
                    return px;
                }
            } 
        }else if(px<len)
        {
            for(;px<len;px++)
            {
                if( BMP(px,py)==color)
                {
                    return px;
                }
            }
        }
        if(len==px)
        {
            return px;
        }

    }
    return len;
}
 


/************************************************************************  
*函数名：IMG_Parity
*作用：计算两数绝对值函数
*输入：无
*输出：无            
*  
* 作    者：墨水瓶
* 完成日期：2018年07月12日 
* 修改人：
* 修改日期：
************************************************************************/
int IMG_Parity(int a,int b)
{
    if(a-b>0)
    return a-b;
    else 
    return b-a;
}








