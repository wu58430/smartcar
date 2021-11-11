#ifndef _PID_H_
#define _PID_H_

#include "headfile.h"

extern float BalancePIDParam[][7];

extern float SpeedPIDParam[][5];
extern float LeftMotorSpeedPIDParam[1][7];
extern float RightMotorSpeedPIDParam[1][7];
extern float ServoPIDParam[][8];
extern float TurnPIDParam[][9];


//��ͨ�ٶȻ� �����ٶȿ���
//��������ʽPID
typedef struct
{
    int16 exval;
    int16 nowval;
    float kp;
    float ki;
    float kd;
    int16 nowError;
    float increase;
    int16 lastError;
    int16 beforeError;
    int16 out;
    float maxout;
    float minout;
} SpeedPIDTypedef;

extern SpeedPIDTypedef SpeedPID, SpeedPIDLeft, SpeedPIDRight;
int16 SimpleIncremental_PID(float *pid_param, SpeedPIDTypedef *pid, int16 _exval, int16 _nowval);


//������� ���

typedef struct
{
    int16 exval;
    int16 nowval;
    float kp;
    float kp1;
    float kp2;
    float kd;
    float ki;
    int16 nowError;
    int16 lastError;
    int16 beforeError;
    int32 sumError;
    float sumErrormax;
    float sumErrormin;
    int16 out;
    float maxout;
    float minout;
} ServoControlCamTypedef;

extern ServoControlCamTypedef ServoControlCam;
int16 ServoPosition_PID(float *pid_param, ServoControlCamTypedef *pid, int16 _exval, int16 _nowval);


//ת�� ������λ��ʽpid ��������ͷ����
typedef struct
{
    int16 exval;
    int16 nowval;
    float kp;
    float kp1;
    float kp2;
    float kp2Real;
    float kp2_K;
    float kp3;
    float ki;
    float kd;
    int16 nowError;
    int16 lastError;
    int16 beforeError;
    int32 sumError;
    float sumErrormax;
    float sumErrormin;
    float Ud;//��ǰ΢��ֵ
    float Ud_last;//��һ��΢��ֵ
    float Ud_a;//�˲�ϵ��
    int16 out;
    float maxout;
    float minout;
} TurnLoopCamTypedef;

extern TurnLoopCamTypedef TurnLoopCam, BalancePID;
int16 Position_PID(float *pid_param, TurnLoopCamTypedef *pid, int16 _exval, int16 _nowval);

float Float_SimpleIncremental_PID(float *pid_param, TurnLoopCamTypedef *pid, float _exval, float _nowval);

//function
void  PID_Init(void);




//----------------------------------------ģ��PID------------------------------------------//
/*����ṹ��͹�����*/
typedef struct
{
    float nowval;                 /*��ǰֵ*/
    float exval;                  /*����ֵ*/
    float nowError;               /*��ǰƫ��*/
    float E_Quantized;            /*����������*/
    int   E_Index[2];             /*��������*/
    float E_Membership[2];        /*������*/
    float ErrorInc;               /*ƫ������*/
    float EC_Quantized;           /*����������仯*/
    int   EC_Index[2];            /*��������*/
    float EC_Membership[2];       /*������*/

    float kp1;                    /*����ϵ��*/
    float ki1;                    /*����ϵ��*/
    float kd1;                    /*΢��ϵ��*/
    float kp2;                    /*����ϵ��*/
    float ki2;                    /*����ϵ��*/
    float kd2;                    /*΢��ϵ��*/
    float kp2k;                   /*kp2ģ������ϵ��*/
    float ki2k;                   /*ki2ģ������ϵ��*/
    float kd2k;                   /*kd2ģ������ϵ��*/
    float kp;                    /*����ϵ��*/
    float ki;                    /*����ϵ��*/
    float kd;                    /*΢��ϵ��*/
    float sumError;               /*���ֺ�*/
    float sumErrormax;            /*����*/
    float sumErrormin;            /*����*/

    float nowErrmax;              /*����*/
    float nowErrmin;              /*����*/
    float lastError;              /*ǰһ��ƫ��*/
    float preError;               /*ǰ����ƫ��*/

    float ECmax;                  /*����*/
    float ECmin;                  /*����*/

    float out;                    /*���ֵ*/
    float outmax;                 /*���ֵ������*/
    float outmin;                 /*���ֵ������*/
} FuzzyPID_Typedef;

extern  FuzzyPID_Typedef  AngleLoop_Fuzzy;


//
void FuzzyPID_Init(void);
void Fuzzy_PID_Test(void);
float FuzzyPID(FuzzyPID_Typedef *pid, float _nowval, float _exval);
#endif
