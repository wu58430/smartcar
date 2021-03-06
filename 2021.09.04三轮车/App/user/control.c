#include "control.h"

int Ackerman_kl = 220;//左右差速系数
int Ackerman_kr = 220;

int Speed_diff_l = 0;//左差速
int Speed_diff_r = 0;//右差速

CarInfoTypedef   CarInfo;
//初始化一些设置
uint8 CarSystem_Init(void)
{
    //设置标志
    CarInfo.Protect = 'F';
    CarInfo.ControlMode = CAMMODE;
    CarInfo.ClosedLoopMode = AllLoop;
    CarInfo.CAR_PROTECT_MODE = 'F';//开启电磁信号削减保护
    CarInfo.StopTime = 0.000f;
    CarInfo.PWM_ENABLE = 'T';
    CarInfo.ReadFlashParams = 'F';
    CarInfo.Camera_FPS = 150;
    //ret
    return 1;
}


//中线计算
int32 point_center;
int32 point_center0;
int32 point_center1;
int32 point_center2;
uint8 Center_Cal(void)
{
    //计算当前行的偏移量
    float excursion[60];
    float sumcenter = 0;
    //三岔出入中线计算
    if(Fork.FindFlag == 'T' && (Fork.state == ForkInTurn || Fork.state == ForkOutTurn))
    {
        if(Fork.Peak.y <= 15)
        {
            if(Fork.In_Direct == 'R')
                point_center = 60;
            else
                point_center = -60;
            return 1;
        }
        for(Ysite = 5; Ysite < 15; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[5] = 0.000f;
        excursion[6] = 0.000f;
        excursion[7] = 0.000f;
        for(Ysite = 5; Ysite < 15 ; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        if(Fork.state == ForkInTurn)
            point_center = (int32)(sumcenter / 10 * 3.0f);
        else if(Fork.state == ForkOutTurn)
            point_center = (int32)(sumcenter / 10 * 2.2f);
        if(Fork.In_Direct == 'R')
            point_center = range_protect(point_center, -100, 300);
        else
            point_center = range_protect(point_center, -300, 100);
    }
    //出入库中线计算
    else if(Barn.FindFlag == 'T' && (Barn.state == BarnOutTurn || Barn.state == BarnInTurn))
    {
        for(Ysite = 10; Ysite < 25; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[10] = 0.000f;
        excursion[11] = 0.000f;
        excursion[12] = 0.000f;
        for(Ysite = 10; Ysite < 25 ; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        if(Barn.state == BarnOutTurn)
            point_center = (int32)(sumcenter / 15 * 1.0f);
        else if(Barn.state == BarnInTurn)
        {
            if(Barn.OutDir == 'R')
                point_center = 200;
            else if(Barn.OutDir == 'L')
                point_center = -200;
        }

    }
    //入环岛转向
    else if(Circle.FindFlag == 'T' && Circle.state == CircleInTurn )
    {
        uint8 StartCalLine = 0, EndCalLine = 0;
        if(Circle.RightUpInflexion.y >= 16 && Circle.dir == 'R')
        {
            EndCalLine = Circle.RightUpInflexion.y;
            StartCalLine = EndCalLine - 10;
        }
        else if(Circle.LeftUpInflexion.y >= 16 && Circle.dir == 'L')
        {
            EndCalLine = Circle.LeftUpInflexion.y;
            StartCalLine = EndCalLine - 10;
        }
        else if(Circle.state == CircleInTurn)
        {
            if(Circle.dir == 'R')
                point_center = 100;
            else
                point_center = -100;
            return 1;
        }
        //计算
        for(Ysite = StartCalLine; Ysite < EndCalLine; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[StartCalLine] = 0.000f;
        excursion[StartCalLine + 1] = 0.000f;
        excursion[StartCalLine + 2] = 0.000f;
        for(Ysite = StartCalLine; Ysite < EndCalLine; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        point_center = (int32)(sumcenter / 10 * Circle.InTurnK);
        if(Circle.dir == 'R')
        {
            point_center = range_protect(point_center, 0, 300);
        }
        if(Circle.dir == 'L')
        {
            point_center = range_protect(point_center, -300, 0);
        }
    }
    //出环岛
    else if(Circle.FindFlag == 'T' && Circle.state == CircleOutTurn)
    {
        for(Ysite = 10; Ysite < 20; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[10] = 0.000f;
        excursion[11] = 0.000f;
        excursion[12] = 0.000f;
        for(Ysite = 10; Ysite < 20 ; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        point_center = (int32)(sumcenter / 10 * Circle.OutTurnK);
        if(Circle.dir == 'R')
        {
            point_center = range_protect(point_center, -100, 300);
        }
        if(Circle.dir == 'L')
        {
            point_center = range_protect(point_center, -300, 100);
        }
    }
    //环内巡线
    else if(Circle.FindFlag == 'T' && Circle.state == CircleIn)
    {
        for(Ysite = 5; Ysite < 15; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[5] = 0.000f;
        excursion[6] = 0.000f;
        for(Ysite = 5; Ysite < 15 ; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        point_center = (int32)(sumcenter / 10 * 2.0f);
    }
    //入环直道
    else if(Circle.FindFlag == 'T' && \
            (Circle.state == CircleInStraight || Circle.state == CircleOutCenter || Circle.state == CircleOutStraight))/* 前瞻 15 - 25 */
    {
        for(Ysite = 15; Ysite < 25; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[15] = 0.000f;
        excursion[16] = 0.000f;
        excursion[17] = 0.000f;
        for(Ysite = 15; Ysite < 25 ; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        point_center0 = (int32)(sumcenter / 10 * 2.0f);
        point_center1 = point_center0;
        point_center2 = point_center1;
        point_center = point_center0 * 0.7f + point_center1 * 0.2f + point_center2 * 0.1f;
        point_center *= 0.5f;
        //		for(Ysite = 10;Ysite < 20;Ysite ++)
        //		{
        //			excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        //		}
        //		excursion[10] = 0.000f;
        //		excursion[11] = 0.000f;
        //		excursion[12] = 0.000f;
        //		for(Ysite = 10;Ysite < 20 ;Ysite ++)
        //		{
        //			sumcenter += excursion[Ysite];
        //		}
        //		point_center0 = (int32)(sumcenter / 10 * 2.0f);
        //		point_center1 = point_center0;
        //		point_center2 = point_center1;
        //		point_center = point_center0 * 0.7f + point_center1 * 0.2f + point_center2 * 0.1f;
    }
    //普通赛道计算
    else/* 前瞻 15 - 25 *///15-30
    {
        for(Ysite = 20; Ysite < 55; Ysite ++)
        {
            excursion[Ysite] = (float)(Img.Center_Repair[Ysite] - 64) / (float)(RoadWide0[Ysite] / 2) * 100;
        }
        excursion[15] = 0.000f;
        excursion[16] = 0.000f;
        excursion[17] = 0.000f;
        for(Ysite = 20; Ysite < 55 ; Ysite ++)
        {
            sumcenter += excursion[Ysite];
        }
        point_center0 = (int32)(sumcenter / 10 * 2.0f);
        point_center1 = point_center0;
        point_center2 = point_center1;
        point_center = point_center0 * 0.7f + point_center1 * 0.2f + point_center2 * 0.1f;
    }
    return 1;
}


//定时器中断 1ms
static int16 TIMx_Cnt = 0;
int16 DirectOut = 0;
int16 SpeedOut =  0;
int16 ExSpeed = NormalSpeed;
int16 L_ExSpeed = 0;
int16 R_ExSpeed = 0;
int16 L_SingleControlOut = 0, R_SingleControlOut = 0;
uint8 WaveWriteOK = 0;
float Gyro_Z_out = 0;
float GYRO_Z_Last = 0.000f;
int16 RiseHeadOut = 0;

void Timer_IT(void)
{
    //time
    TIMx_Cnt ++;
    CarInfo.UpTime += 0.001f;
    BeepInt();
    KeyScanInt();
    //陀螺仪数值获取
    Data_Filter();
    if(TIMx_Cnt % 5 == 0 )
    {
        //角度积分
        ICM_I_Int();
        //更新角度
        IMUupdate(ICM_Treated.gyro.x * 0.01745, ICM_Treated.gyro.y * 0.01745, ICM_Treated.gyro.z * 0.01745, \
                  ICM_Treated.accdata.x , ICM_Treated.accdata.y, ICM_Treated.accdata.z);
    }
    //----------------------------摄像头控制舵机转向-----------------------------------//


    if(CarInfo.ControlMode == CAMMODE && CarInfo.Protect == 'F' /*&&
		(CarInfo.ClosedLoopMode == AllLoop || CarInfo.ClosedLoopMode == DirectLoop)*/)
    {
        Servo_add = ServoPosition_PID(&ServoPIDParam[0][0], &ServoControlCam, 0, point_center);
        //		Servo_add = Position_PID(&TurnPIDParam[0][0],&TurnLoopCam,0,point_center );
        //		Servo_add = FuzzyPID(&AngleLoop_Fuzzy, point_center,0);

    }
    //----------------------------电磁控制舵机转向-----------------------------------//
    if(TIMx_Cnt % 5 == 0 && CarInfo.ControlMode == ELCMODE && CarInfo.Protect == 'F' &&
            (CarInfo.ClosedLoopMode == AllLoop || CarInfo.ClosedLoopMode == DirectLoop) )
    {
        DirectOut = Position_PID(&TurnPIDParam[0][0], &TurnLoopCam, 0, ELC_Center_14);
    }
    //------------------------------------差速控制------------------------------------//
    if(TIMx_Cnt % 10 == 0 && CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == AllLoop )
    {
        if(Servo_add <= -100) //右转
        {
            Speed_diff_l = ExSpeed;
            Speed_diff_r = (int)(ExSpeed * (1 + ((float)Ackerman_kl / 1000) * tan((float)Servo_add * 3.14 / 4600) / 0.396));
        }
        else if(Servo_add >= 100) //左转
        {
            Speed_diff_l = (int)(ExSpeed * (1 - ((float)Ackerman_kl / 1000) * tan((float)Servo_add * 3.14 / 4600) / 0.396));
            Speed_diff_r = ExSpeed;
        }
        else
        {
            Speed_diff_l = ExSpeed;
            Speed_diff_r = ExSpeed; //内轮减速外轮加速确保中轴线速度不变
        }
    }
    //-----------------------------------速度控制------------------------------------//
    //速度控制 双轮
    if(TIMx_Cnt % 10 == 0 && CarInfo.Protect == 'F' &&
            (CarInfo.ClosedLoopMode == AllLoop || CarInfo.ClosedLoopMode == SpeedLoop))
    {
        Encoder_Read();
        if(CarInfo.UpTime <= 0.500f)//起步
        {
            L_SingleControlOut = SimpleIncremental_PID(&SpeedPIDParam[0][0], &SpeedPIDLeft, (float)(ExSpeed * (CarInfo.UpTime + 0.500f)), Wheel.LSpeed);
            R_SingleControlOut = SimpleIncremental_PID(&SpeedPIDParam[0][0], &SpeedPIDRight, (float)(ExSpeed * (CarInfo.UpTime + 0.500f)), Wheel.RSpeed);
            //			SpeedOut = SimpleIncremental_PID(&SpeedPIDParam[0][0],&SpeedPID,(float)(ExSpeed * (CarInfo.UpTime + 0.500f)),Wheel.now);
        }
        else
        {
            L_SingleControlOut = SimpleIncremental_PID(&SpeedPIDParam[0][0], &SpeedPIDLeft, (float)(ExSpeed), Wheel.LSpeed);
            R_SingleControlOut = SimpleIncremental_PID(&SpeedPIDParam[0][0], &SpeedPIDRight, (float)(ExSpeed), Wheel.RSpeed);
            //			SpeedOut = SimpleIncremental_PID(&SpeedPIDParam[0][0],&SpeedPID, \
            //			(float)(ExSpeed),Wheel.now);
        }
    }
    //速度控制 单轮
    if(TIMx_Cnt % 10 == 0 && CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == LR_SingleControl)
    {
        Encoder_Read();
        L_SingleControlOut = SimpleIncremental_PID(&LeftMotorSpeedPIDParam[0][0], &SpeedPIDLeft, L_ExSpeed, L_Wheel.now);
        R_SingleControlOut = SimpleIncremental_PID(&RightMotorSpeedPIDParam[0][0], &SpeedPIDRight, R_ExSpeed, R_Wheel.now);
    }
    //速度控零保护
    if(TIMx_Cnt % 10 == 0 && CarInfo.Protect == 'T')
    {
        Encoder_Read();
        pwm_l = SimpleIncremental_PID(&LeftMotorSpeedPIDParam[0][0], &SpeedPIDLeft, 0, L_Wheel.now);
        pwm_r = SimpleIncremental_PID(&RightMotorSpeedPIDParam[0][0], &SpeedPIDRight, 0, R_Wheel.now);
    }
    //波形发送标志
    if(TIMx_Cnt % 50 == 0 )
    {
        WaveWriteOK = 1;
    }
    //清零计数
    if(TIMx_Cnt >= 1000)
    {
        LED0_TOGGLE;
        TIMx_Cnt = 0;
        CarOutProtectCnt = 0;
    }
    //限幅
    if(CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == AllLoop)
    {
        pwm_l = Speed_diff_l + L_SingleControlOut;
        pwm_r = Speed_diff_r + R_SingleControlOut;
    }
    else if(CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == SpeedLoop)
    {
        pwm_l = + L_SingleControlOut;
        pwm_r = + R_SingleControlOut;
    }
    else if(CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == DirectLoop)
    {
        pwm_l = 0;
        pwm_r = 0;
    }
    else if(CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == LR_SingleControl)
    {
        pwm_l = L_SingleControlOut;
        pwm_r = R_SingleControlOut;
    }
    //	else if(CarInfo.Protect == 'F' && CarInfo.ClosedLoopMode == NOLoop && \
    //		CarInfo.RiseHeadLoopEnable == 'T')
    //	{
    //		pwm_l = RiseHeadOut;
    //		pwm_r = RiseHeadOut;
    //	}
    SERVO_Write();
    pwm_l = range_protect(pwm_l, -1000, 1000);
    pwm_r = range_protect(pwm_r, -1000, 1000);
    if(CarInfo.PWM_ENABLE == 'T' && CarInfo.UpTime >= 1.000f)
        PWM_Write();
    else if(CarInfo.PWM_ENABLE == 'F')
    {
        pwm_l = 0;
        pwm_r = 0;
        PWM_Write();
    }
}


//冲出赛道电磁削减保护
int16 CarOutProtectCnt = 0;
void Car_Protect(void)
{
    if((CarInfo.UpTime >= 2.000f) && ((CarInfo.ClosedLoopMode == AllLoop) || \
                                      (CarInfo.ClosedLoopMode == SpeedLoop)))
    {
        //---------------电磁保护--------------//
        if(ELC_PROTECT_FLAG == 'T' && CarInfo.CAR_PROTECT_MODE == 'T')
        {
            CarOutProtectCnt ++;
        }
        if(CarOutProtectCnt >= 5)
        {
            CarInfo.Protect = 'T';
        }
    }
    if(CarInfo.StopTime != 0.000f && CarInfo.UpTime >= CarInfo.StopTime)
    {
        CarInfo.Protect = 'T';
        Base.element_check = 'F';
    }
}

//显示界面调整参数
const uint8 CarInfoItem0[] = "Protect En:";
const uint8 CarInfoItem1[] = "Control Loop:";
const uint8 CarInfoItem2[] = "Params Read:";

#define CarInfo_MENU_NUM 2
//#define ControlMood_MENU_NUM 4

uint8 CarInfo_Show(void)
{
    while(KeyPageUp != onepress)
    {
        static int8 NowChooseControlMood = 1;
        static int8 NowCarInfoItem = 0;
        Menu_Choose(&NowCarInfoItem, CarInfo_MENU_NUM);
        //显示条目
        IPS114_PENCOLOR = RED;
        ips114_showstr(16, 0, CarInfoItem0);
        ips114_showstr(16, 1, CarInfoItem1);
        ips114_showstr(16, 2, CarInfoItem2);
        //显示参数
        IPS114_PENCOLOR = BLUE;
        if(CarInfo.CAR_PROTECT_MODE == 'T')
            ips114_showstr(128, 0, "T");
        else
            ips114_showstr(128, 0, "F");

        if(CarInfo.ClosedLoopMode == AllLoop)
            ips114_showstr(128, 1, "Allloop");
        else if(CarInfo.ClosedLoopMode == SpeedLoop)
            ips114_showstr(128, 1, "SpeedLoop");
        else if(CarInfo.ClosedLoopMode == DirectLoop)
            ips114_showstr(128, 1, "DirectLoop");
        else if(CarInfo.ClosedLoopMode == NOLoop)
            ips114_showstr(128, 1, "NOLoop");

        if(CarInfo.ReadFlashParams == 'T')
            ips114_showstr(128, 2, "T");
        else
            ips114_showstr(128, 2, "F");
        //调参确认
        if(KeyPageDown == onepress)
        {
            KeyPageDown = nopress;
            switch(NowCarInfoItem)
            {
            case(0):
            {
                StateAdjust(&CarInfo.CAR_PROTECT_MODE, 128, 0);
                break;
            }
            case(1):
            {
                //					Menu_Choose(&NowChooseControlMood,ControlMood_MENU_NUM);
                ParamsAdjustShort(&CarInfo.ClosedLoopMode, 190, 1, 1);
                //					switch(NowChooseControlMood)
                //					{
                //						case (0):CarInfo.ClosedLoopMode = AllLoop;
                //						case (1):CarInfo.ClosedLoopMode = SpeedLoop;
                //						case (2):CarInfo.ClosedLoopMode = DirectLoop;
                //						case (3):CarInfo.ClosedLoopMode = NOLoop;
                //					}
                break;
            }
            case(2):
            {
                StateAdjust(&CarInfo.ReadFlashParams, 128, 2);
                break;
            }
            }
        }
        delay_ms(10);
    }
    KeyPageUp = nopress;
    //
    return 1;
}








