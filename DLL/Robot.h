#include "MDH1K.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<conio.h>
#include<locale.h>
#include<iostream.h>


////////20150605,����ʽ�ײ����
///////////////////////////////////����////////////////////////////////////
typedef unsigned char    U8;
typedef signed char	 I8;
typedef unsigned short   U16;
typedef signed short     I16;
typedef unsigned long    U32;
typedef signed long      I32;
//typedef signed long long I64;//20151226,���벻ͨ������
typedef float            F32;
typedef double           F64;


/////////////////
#define   E           extern
#define MAX_PULSE   100000
#define PI   3.1415926
#define Safety_Belt_L   10.0  //�ֶ����������˶����ް�ȫ����
#define Safety_Belt_A   10.0  //�ֶ����������˶����ް�ȫ����
#define RobotDLLVer     1708261355   //�����汾��������ʱ��

///////////////////////////////////////////////////////
//      ������  ���������
#define a_x		0
#define a_y		1
#define a_z		2
#define a_r		3
#define a_r1	5	  //��̬��
#define a_r2	6	  //��̬��
#define a_s		4	  //������
#define a_e		7     //����δ���壬�ݶ�Ϊ�ⲿ��


///////////////////////////////////////////////////////
//һ�¶��������������ݵ����źŶ��ձ�ȷ��
#define key_Xp  12 //��Ҫ���������һ��ZZ,R2Z
#define key_Xn  13 //ZF,R2F
#define key_Yp  14  //RZ,R3Z
#define key_Yn  15 //RF,R3F
#define key_SEL_1  11
#define key_SEL_2  10
#define key_FT  2
#define key_BT  1
#define key_BL  8
#define key_FS  6
#define key_START  4
#define key_STOP  5
#define key_FIX_POS  7
#define key_NEXT  8
#define key_UP_POS  9
#define key_RESET 3
#define key_CLEAR 0

#define BLOW_DO  6		//����
#define Cylinder_DO  0	//����
#define LED_START_DO  7 //ָʾ����������ָʾ���ź�_DO




//////////////////////////////////////////////////////////////
#define MOD_Teach 1 //�ֶ�ģʽ��ʾ��
#define MOD_AUTO 2  //�Զ�ģʽ���ֱ�������������


///////////////////////////////////ȫ�ֱ�������/////////////////////////////////////
E volatile  struct AXIS Axis_x,Axis_y,Axis_z,Axis_r,Axis_r1,Axis_r2,Axis_s,Axis_e;
E volatile  struct Forward_Tin HD_FT,HD_BT ;//�����ֶ�����������
E volatile  struct KEY KEY ;//�����ֶ�����������
/////////////



E F64 HM_FT_MAX_Speed,HM_FT_Min_Speed;
E U32 overtimesflag,count_time,Work_Point_step_num;
E U8 ML_V_Soldering_Status,v_Forward_Tin_Status_flag ;
E U8 Work_State_Type,ML_V_Soldering_Over_Time_Flag;
E U32 Over_Time_count;
E U8 Homing_Status_flag,MIming_Status_flag;
E U32 Delywkl_chk_counter,Delywkp_chk_counter;
E U32 ML_Check_Mima_Legal_fail_count;
E I32 FHZ;
/////////////
E U8 ML_MovePTP_Step_Num,ML_MovePTP_State;//ML_MovePTP()
E U8 ML_FF_Stop_Tin_Flag;//20150529��������ʱ���Ʋ�����ִ��"�켣"ʱ,�ص�������������
E U8 SolderFace_style;//20150529,���ڹ��̲������ڶ�Ӧ��������һ�µġ�SolderFace_style
E U8 Do_Step_style;//201150529,ִ�еĲ�������ͣ�0(��ʼ��),1���м��,2�����һ�
E U8 Set_DLL_Return;//201150529,����λ����Ҫ����ֹͣ��λ����ĳһ�߳�ʱ������ͨ�����������롣
E U8 RobotStyle;//20150608
E U8 Set_Act_Y2;//  :20150611����λ���ж�
E U8 Set_DINGWEI_HZ;//20170326���ö�λ�����Ƿ��������������ܡ�
//��ǰ���õ���Y1����Y2��Set_Act_Y2��ʼ��Ϊ0��Ĭ��XYִ�У�����Ҫ����Y2��ͨ���ú�������Set_Act_Y2 = 1;DLL����XY2ִ��
E U8 Set_NOAct_R;////  :20160402����λ�����������н����жϣ��������R�ᣬ��ֵΪ0��������R�ᣬ��ֵΪ1��
E U8 Set_WorkLine_NUM;//  :20150805,U8 Set_WorkLine_NUM=0,��ͨ�Ϻ���1��������2����м䣬3���δ��

E U8 HomingStyle;//���㷽ʽ��=0�������㣬1�ǿ�������
E U8 DouhanStyle;//������ʽ��=0��ͨ��ʽ��1ȫ��������������2�붨������������20151119

E U8 F_Change_Position_ctrl;//F_Change_Position_ctrl==0,ִ�У�F_Change_Position_ctrl==1��ִ��
////////////////////////////////////
E	DWORD	m_dwSerial;
E	DWORD	m_dwMini;
E	CString	m_strPsd;
E	CString	m_strDisplay;
E	DWORD	m_dwKey1,K1A,K1B,K1C,K1D,K1E;
E	DWORD	m_dwKey2,K2A,K2B,K2C,K2D,K2E;
E	DWORD	m_dwKey3,K3A,K3B,K3C,K3D,K3E;
E	DWORD	m_dwKey4,K4A,K4B,K4C,K4D,K4E;
E	long	m_dwRand;
E	CString	m_strRet;
E	long	m_dwSPC;
E	long	m_dwLF32;
/////////////////��д����///////////////
E	DWORD	m_dwAddr;
E	int		m_iDValue;
E	CString	m_strValue;
E    unsigned long m_iDValuedata;
E	CString m_strValuedata;
E	CString	m_strTime;
E	CString			tempStr;

////////////////////////////////ȫ�ֱ�������/////////////////////////

F64 HM_FT_MAX_Speed,HM_FT_Min_Speed;//��λMM,�����������á�
U32 overtimesflag,count_time,Work_Point_step_num = 0;
U8	ML_V_Soldering_Status,v_Forward_Tin_Status_flag;//����״̬��־1�������У�0�޺��ӣ�ȫ�ֱ�����־��
U8 Work_State_Type,ML_V_Soldering_Over_Time_Flag;
U32 Over_Time_count;
U8 Homing_Status_flag,MIming_Status_flag=0;
U32 Delywkl_chk_counter,Delywkp_chk_counter;//20151004
U32 ML_Check_Mima_Legal_fail_count = 0;
I32 FHZ = 0;

////////////�Ӻ������б�־
U8 ML_MovePTP_Step_Num,ML_MovePTP_State;//ML_MovePTP()

U8 ML_FF_Stop_Tin_Flag =0;//20150529,��ʼ��Ϊ������0���ͣ�1������
U8 SolderFace_style = 0;//20150529,�ڻ����󣬳�ʼ���������淽��0=��࣬1Ϊ���Ҳ࣬2Ϊǰ�棬3Ϊ����
U8 Do_Step_style = 0;//201150529,ִ�еĲ�������ͣ�0(��ʼ��),1���м��,2�����һ���Ĭ����ʼ��
U8 Set_DLL_Return = 0;//201150529,����λ����Ҫ����ֹͣ��λ����ĳһ�߳�ʱ������ͨ�����������롣0:�����룬1������
U8 RobotStyle = 0;//20150608,�����˱���ṹ��ʽ��0��̨ʽ���ᣨXYZR����1������ʽ(Y,R��ת)��2��̨ʽ˫ƽ̨��Y1,Y2����3������
U8 Set_Act_Y2 = 0;//  :20150611����λ���ж�
U8 Set_DINGWEI_HZ = 0;//20170326��ʼ��Ϊ0�������á����������������޸ĳ���
//��ǰ���õ���Y1����Y2��Set_Act_Y2��ʼ��Ϊ0��Ĭ��XYִ�У�����Ҫ����Y2��ͨ���ú�������Set_Act_Y2 = 1;DLL����XY2ִ��
U8 Set_NOAct_R = 0;//  :20160402����λ�����������н����жϣ��������R�ᣬ��ֵΪ0��������R�ᣬ��ֵΪ1��
U8 Set_WorkLine_NUM = 0;//  :20150805,U8 Set_WorkLine_NUM=0,��ͨ�Ϻ���1��������2����м䣬3���δ��

U8 HomingStyle = 0;
U8 DouhanStyle = 3;//������ʽ��=0��ͨ��ʽ��1��ͨ������2ȫ��������������3�붨������������20151119Ϊ�������õ�ר�ö�����

U8 F_Change_Position_ctrl = 0;
/////////////
//////////////��ȫ����////////////////////	
	DWORD	m_dwSerial;
	DWORD	m_dwMini;
	CString	m_strPsd;
	CString	m_strDisplay;
	DWORD	m_dwKey1,K1A,K1B,K1C,K1D,K1E;
	DWORD	m_dwKey2,K2A,K2B,K2C,K2D,K2E;
	DWORD	m_dwKey3,K3A,K3B,K3C,K3D,K3E;
	DWORD	m_dwKey4,K4A,K4B,K4C,K4D,K4E;

	long	m_dwRand;
	CString	m_strRet;
	long	m_dwSPC;
	long	m_dwLF32;
/////////////////��д����///////////////
	DWORD	m_dwAddr;
	int		m_iDValue;
	CString	m_strValue;
    unsigned long m_iDValuedata;
	CString m_strValuedata;
	CString	m_strTime;
	CString			tempStr;

   // time_t	*tLockTime = new time_t[2];


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
struct AXIS{
///////////////////////�������Բ�������////////////////////////////////////////////////////
U32 PULSE_Per;//�������/ת����ϸ�ֲ���,��ǰΪ6400
U32 MAX_Speed;//�趨��Χ��0~100000�������������á�MAX_Speed<MAX_PULSE
F32 MAX_route;//����г̣���λMM����ȣ���Χ���ݻ�е�ṹȷ����
F32 MIN_route;//��С�г̣���λMM����ȣ���Χ���ݻ�е�ṹȷ����

F64 STEP_Length;//��λMM����ȣ���Χ���ݻ�е�ṹȷ��,STEP_Length=2*PI*R/PULSE_Per;=360/PULSE_Per

///////////////////////��������ϵ����////////////////////////////////////////////
F32 Current_Length;//��λMM����ȣ���ǰ����ֵ��MIN_route<Current_Length<MAX_route
F32 Target_Length;//��λMM����ȣ�Ŀ������ֵ��MIN_route<Target_Length<MAX_route
I32 CurrentPosition;//��ǰ���꣬��λPPS����¼������������Ӧ���ǵĻ���=��X,Y,Z���ԣ���Ϊ��ֵ��CurrentPosition=Current_Length/STEP_Length
					//�ҵ����󣬽���ֵ��Ϊ0�����о�������ֵ��
					//��R��ת���ԣ���һ������ֵ���Ƕȣ���ǰΪ176��-174�������������Ϊ����������Ϊ�������о�������ֵ��
					//��R1,R2,��ӦҲ�ǽǶȣ������������Ϊ����������Ϊ�������о�������ֵ��
					//��S����е�������˶���û���г̵ĸ����������ƶ���
I32 m_Target_Axis;//Ŀ�����꣬��λPPS��Ҫ�ﵽ��������(����ֵ)��Ҫƫ�Ƶ�������(���ֵ)��

///////////////////////�û����򹤼�����ϵ������ϵ����/////////////////////////////
F32 User_Current_Length;//��λMM����ȣ������û��趨����ԭ���γɵĵ�ǰ������������ֵ���������ƫ�ƣ�������ֱ��չ�֡�
F32 User_Target_Length;//��λMM����ȣ�Ŀ������ֵ�����û�����ϵΪ�ο���Ŀ������ֵ��
I32 User_Org_Position;//��ǰ���꣬��λPPS����¼�û����õ����ʵ��������������User_Org_Position = ��������CurrentPosition������X,Y,Z.

////////////////////////��λת������/////////////////////////////////////////////////////

F32 Mil_Current_Length;//��λmil���Գ��ȵ�λ���仯����mmת��Ϊmil��1mil=0.0254mm,�ʺ�X,Y��
F32 Mil_Target_Length;//��λmil���ʺ�X,Y��

////////////////////////�ֶ�ģʽ�µ��ٶȲ���/////////////////////////////////////////////////////

U32 Jog_MAX_Speed;//�趨��Χ��0~100000�������������á���Ӧ���Jog_MAX_Speed<MAX_Speed
U32 Jog_MIN_Speed;//�趨��Χ��0~100000�������������á�
U32 Jog_Current_Speed;//�趨��Χ��0~100000�������������á�Jog_MIN_Speed<Jog_Current_Speed<Jog_MAX_Speed
U8  Jog_index_Speed;//�����ֶ���ʵ�������ٶ�������ٶȵİٷֱȣ�0,1,2,3������10����ʾ0%��10%����������100%
F32 INCJogValue;//�㶯ʱ
U8  SetJogMode;

////////////////////////�Զ�ģʽ�µ��ٶȲ���///////////////////////////////////////////////////


////////////////////////�ٶ��������ò�������/////////////////////////////////////////////////

U32 G_TVH;//�Ӽ����������ò�������,С��MAX_Speed
U32 G_TVL;
U32 G_SVH;//С��MAX_Speed
U32 G_SVL;
U16 G_TTAcc;
U16 G_TTDec;

U16 G_STAcc;
U16 G_STDec;
U16 G_STsAcc;////ӦС��G_STAcc��һ��
U16 G_STsDec;////ӦС��G_STDec��һ��
////////////////////////org��ԭ����ٶȲ���
U32 VL_HomePPS;
U32 VH_HomePPS;//С��MAX_Speed
U16 TAccDec_ms;

U32 Vhmax[22];//20150809,֧�����ݾ������üӼ��ٲ�����
};
volatile  struct AXIS Axis_x,Axis_y,Axis_z,Axis_r,Axis_r1,Axis_r2,Axis_s,Axis_e;//��������Ľṹ����


struct Forward_Tin{
	////////////////////////�����ٶ��������ò�������/////////////////////////////////////////////////

U32 G_TVH_speed;//�Ӽ����������ò�������,С��MAX_Speed
U32 G_TVL;
F64 G_SVH_speed;//С��MAX_Speed
U32 G_SVL;
U16 G_TTAcc;
U16 G_TTDec;

U16 G_STAcc;
U16 G_STDec;
U16 G_STsAcc;////ӦС��G_STAcc��һ��
U16 G_STsDec;////ӦС��G_STDec��һ��


};

volatile  struct Forward_Tin HD_FT,HD_BT,AT_FT,AT_BT ;//�����ֶ�����������

/////////////////////////���������ֳ���������///////////////////////////////////////
struct KEY{
U8 HM_RESET;

U8 KEY_SEL_1;
U8 KEY_SEL_2;

U8 X_positive;
U8 Z_positive;
U8 R1_positive;
U8 X_negative;
U8 Z_negative;
U8 R1_negative;


U8 Y_positive;
U8 R_positive;
U8 R2_positive;
U8 Y_negative;
U8 R_negative;
U8 R2_negative;

U8 HM_FORWARD_TIN;
U8 HM_BACK_TIN;
U8 HM_BLOWER;
U8 HM_FAST_SLOW;
U8 HM_START;
U8 HM_STOP;
U8 FIX_POSITION;
U8 NEXT;
U8 UpData_POS;


};
volatile  struct KEY KEY ;//�����ֶ�����������

///////////////////////////////////��������///////////////////////////////////////////////////////
extern "C" void _declspec(dllexport)ML_MDH1K_AttachErrorReport (ErrorHandle ErrorReportFunction);
//extern "C" I32 _declspec(dllexport)ML_MDH1K_RegisterGPICallBack(U8 Num, CallBack Event_PositiveEdge, PVOID pArg_PositiveEdge
//													 , CallBack Event_NegativeEdge, PVOID pArg_NegativeEdge);
//extern "C" I32 _declspec(dllexport)ML_MDH1K_DeRegisterGPICallBack(U8 Num);//ȡ����ע��֮�ص�������
//extern "C" I32 _declspec(dllexport)ML_MDH1K_EnableGPICallBack();
//extern "C" I32 _declspec(dllexport)ML_MDH1K_DisableGPICallBack();


///////////////////////////////////����������ƽ̨����/////////////////////////////////////
/**********************************************************************************************/
////////////ϵͳ����
extern "C" void _declspec(dllexport)ML_Sleep(U32 mscount);//��ʱ,1MSΪ��λ.
extern "C" I32 _declspec(dllexport)ML_FF_start_count_overtime(void);//��ʱ������������Ҫ��һ���̶��ϸ�Ƶ�ʵ������е��á�
extern "C" I32 _declspec(dllexport)ML_Set_RobotStyle(U8 vRobotStyle);//Ϊ��DLL��ά�������㣬ʹ�ö�����Ͳ���ͬһ��DLL�����û���RobotStyle=0,01,10��11:0,1,2,3

///////////////////////////////////Ӳ���忨ϵͳ����//////////////////////////////
////////////�ŷ���������
extern "C" I32 _declspec(dllexport)ML_Initial_MAXPULSE(U32 DriverSpec_PPS);//��ʼ���������������ŷ��������������,
extern "C" I32 _declspec(dllexport)ML_SetPulseOutputType_ALL(U8 v_PulseOutputType);//����������ͬ
extern "C" I32 _declspec(dllexport)ML_SetPulseOutputType_Axis(U8 AxisNo,U8 v_PulseOutputType);//���ø��᲻ͬ����������
extern "C" I32 _declspec(dllexport)ML_Initial_All(void);//Ӳ����ʼ��,DriverSpec_PPS=100000,v_PulseOutputType=0;
extern "C" I32 _declspec(dllexport)ML_ServoONOFF(U8 AxisNO, U8 onoff,U8 allsg);//�رտ����ŷ���AxisNO��0~7��onoff��0�رա�1������allsg��0���ö�Ӧ�ᣬ1ȫ��������
extern "C" I32 _declspec(dllexport)ML_GetServoONStatus(U8 AxisNo, U8 *pServoOnStatus);//��ȡ��ǰ�ŷ�״̬��AxisNO��0~7��pServoOnStatus��0�رա�1����
extern "C" I32 _declspec(dllexport)ML_ExitForm(void);//�ر��˶��忨

////////////��е������������
extern "C" I32 _declspec(dllexport)ML_SetHardSys_MAX_route(U8 AxisNo,F32 vMIN_route,F32 vMAX_route);//���ø�����г�
extern "C" I32 _declspec(dllexport)ML_SetHardSys_STEP_Length(U8 AxisNo,F64 vSTEP_Length);//���ø���Ĳ���
extern "C" I32 _declspec(dllexport)ML_SetHardSys_PULSE_Per(U8 AxisNo,U32 vPULSE_Per);//���ø����ϸ���������������������������йأ���ǰ6400
extern "C" I32 _declspec(dllexport)ML_SetHardSys_MAX_Speed(U8 AxisNo,U32 vMAX_Speed);//���ø������������ٶ�
extern "C" I32 _declspec(dllexport)ML_GetVelocityLimit(U8 AxisNo,U32 *VLimit_pps);//��ȡ���Ƶ��ٶ�ֵ
extern "C" I32 _declspec(dllexport)ML_GetCurrentVelocity(U8 AxisNo,I32 *pVelocity);//��õ�ǰ�ٶ�ֵ,��������
extern "C" I32 _declspec(dllexport)ML_SetVPercentage(U32 VPercentage);//�����ٶȰٷֱȡ�����˵����VPercentage��10~200��
extern "C" I32 _declspec(dllexport)ML_SetSVelocityProfile_Para(U8 AxisNo, U32 vVLPPS, U32 vVHPPS, U16 vTaccMS, U16 vTdecMS, U16 vTsaccMS, U16 vTsdecMS);//����ϵͳ���ٶ����߲�����

////////////////////////////////////////�������˶����////////////////////////////////
///////////////��ԭ�����������
extern "C" I32 _declspec(dllexport)ML_SetHomingVelocityProfile(U8 AxisNo,U32 VL_HomePPS,U32 VH_HomePPS,U16 TAccDec_ms);//���õ����ԭ���˶�����
extern "C" I32 _declspec(dllexport)ML_StartHoming(U8 AxisNo, I8 HomeDir);//�����ԭ�����
extern "C" I32 _declspec(dllexport)ML_SetHomingVelocityProfile_Param(U8 AxisNo,U32 VL_HomePPS,U32 VH_HomePPS,U16 TAccDec_ms);//
extern "C" I32 _declspec(dllexport)ML_F_MovToORG_D(U8 ALL_OR_PART);//�����������,1=ALL��ȫ��������ԭ�㣻0=R1,R2�������ԭ��
extern "C" I32 _declspec(dllexport)ML_GetORG(U8 AxisNo, U8 *pORG);//��ȡԭ�������źţ�AxisNo[0,7]��
extern "C" I32 _declspec(dllexport)ML_GetHoming_Status_flag(U8* mod);//��ȡ��ԭ��״̬��1Ϊ��ɣ�0Ϊδ��ɡ�
///////////////JOG�ֶ�ָ��///////
extern "C" I32 _declspec(dllexport)ML_SetJogminmaxSpeed ( U8 AxisNo,U32 vJog_MIN_Speed ,U32 vJog_MAX_Speed) ;//�����ֶ���ʽ�Ǹ�������ٶȣ�����X,Y,Z,R,R1,R2,E��λΪ��������s��Ϊmm��
extern "C" I32 _declspec(dllexport)ML_SetINCJogValue ( I32 index ) ;//���õ㶯����(���Ȼ�Ƕ�)��index:1 = 0.01,2 = 0.1 ,3 = 1,4=10
extern "C" I32 _declspec(dllexport)ML_Set_Axis_INCJogValue ( U8 axis,I32 index );//�Ե�����Ϊ�����趨�ֶ��ƶ�ģʽ�ą�����ֵ===�㶯����index:1 = 0.01,2 = 0.1 ,3 = 1,4=10
extern "C" I32 _declspec(dllexport)ML_SetExtSpeed ( U8 AxisNo,U8 index ) ;//�����ֶ��ٶȰٷֱȣ�Jog_MAX_Speed*0%~100%����Ӧ0--10
extern "C" I32 _declspec(dllexport)ML_SetJogMode ( U8 index );//�����ֶ��ƶ�ģʽ ��index:0������1Ϊ�㶯
extern "C" I32 _declspec(dllexport)ML_Set_Axis_JogMode ( U8 axis,U8 index );//�Ե�����Ϊ�����趨�ֶ��ƶ�ģʽ��index:0������1Ϊ�㶯 
extern "C" I32 _declspec(dllexport)ML_SetJogINCMove ( U8 axis, I32 direction, I32 enable );//�ֶ��㶯�ƶ� 
extern "C" I32 _declspec(dllexport)ML_SetJogMove ( U8 axis, I32 direction, I32 enable );//�ֶ������ƶ�
//////////////�����������ȡ
extern "C" I32 _declspec(dllexport)ML_SetCurrentPosition(U8 AxisNo,I32  vCurrentPositionPuls);//���õ�ǰ��ľ������꣬������ܾ������������ʱʹ�á�
extern "C" I32 _declspec(dllexport)ML_GetPosJoint( U8 AxisNo,I32* pPositionPulse);//��ȡ��ǰ��ĵ�ǰ���꣬��������,������.
extern "C" I32 _declspec(dllexport)ML_GetPosCartesion( U8 AxisNo, F64* pPosition_Length);//��ȡ��ǰ��ĵ�ǰ�ռ����꣬���Ȼ��߽Ƕ�
extern "C" I32 _declspec(dllexport)ML_GetMotionStatus(U8 AxisNo, U8 *pStatus);//��õ�ǰ�˶�״̬ 

//////////////�ƶ����ܺ���
extern "C" I32 _declspec(dllexport)ML_MovPosition(U8 AxisNo,F64 vTarget_Position,F64 vSpeed_H);//�����ƶ���Ŀ�����ֵ����λ�ã���λΪ���Ȼ�Ƕȡ�
extern "C" I32 _declspec(dllexport)ML_MovReDistance(U8 AxisNo,F64 vRe_Distance,F64 vSpeed_H);//�����ƶ�ƫ�Ƶľ���,��λΪ���Ȼ�Ƕ�
extern "C" I32 _declspec(dllexport)ML_SetPosCartesionP (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H); //ptp��ʽ�ƶ���Ŀ��㣨xyzr��,��λMM���
extern "C" I32 _declspec(dllexport)ML_Mov_Line_XY(F64 x, F64 y,F64 vSpeed_H);//��X,Y������ֱ�߲岹�ƶ���Ŀ��㡣��λMM���
extern "C" I32 _declspec(dllexport)ML_Mov_Line_XYZ(F64 x, F64 y,F64 z,F64 vSpeed_H);//��X,Y,z��ֱ�߲岹�ƶ���Ŀ��㡣
extern "C" I32 _declspec(dllexport)ML_Mov_Line_XYR(F64 x, F64 y,F64 r,F64 vSpeed_H);//��X,Y,r��ֱ�߲岹�ƶ���Ŀ���
///////////////////////////I/O�����ϵͳ��غ���,֧��16·���������롢�����0~15/////////////////////////////////
/////////////////����������������
extern "C" I32 _declspec(dllexport)ML_GetGPI(U8 Num,U8 *pGPI);//Num��0`15 GPIO���������롢���������ȡ�����ܺ�����
extern "C" I32 _declspec(dllexport)ML_GetInput(U8 index,U8 *pGPI);//�������Ҽ�����ΪI/O���룬��Ϊ�û���չI/O���룬index[0,15]
extern "C" I32 _declspec(dllexport)ML_SetOutput(U8 Num);//����������øú��������á�������ض�Num[0,15]
extern "C" I32 _declspec(dllexport)ML_ClearOutput(U8 Num);//����������,Num[0,15]
extern "C" I32 _declspec(dllexport)ML_GetOutput(U8 index,U8 *pGPO);//index[0,15]
/////////////////////////////ֹͣ����λ����ͣ//////////////////////////////////////////////
///////////��ͣ����λ
extern "C" I32 _declspec(dllexport)ML_StopAll(U8 Mode);//��ʱֹͣ�������������Mode 0�����٣�1������
extern "C" I32 _declspec(dllexport)ML_Stop(U8 AxisNo, U8 Mode);//��ʱָֹͣ�����������AxisNo[0,7]��Mode 0�����٣�1������
extern "C" I32 _declspec(dllexport)ML_Restart(U8 AxisNo);//����ָ�����������AxisNo[0,7]
////////////Estop
extern "C" I32 _declspec(dllexport)ML_GetEstop(U8 *pEstop);//��ͣ�ź������ȡ
extern "C" I32 _declspec(dllexport)ML_EnableAutoEstop();//ʹ���Զ���ͣ
extern "C" I32 _declspec(dllexport)ML_DisableAutoEstop();//��ʹ���Զ���ͣ

//////////////////////////////��Ʒ�汾��Ϣ/////////////////////////////////////////////////
extern "C" I32 _declspec(dllexport)ML_GetHardwareType(U8 *pHW_Type);//Ӳ������
extern "C" I32 _declspec(dllexport)ML_GetFirmwareVersion(U8 *pFW_Ver);//����汾
extern "C" I32 _declspec(dllexport)ML_GetRobotDLLVersion(U32 *pFW_Ver);//�����汾

//////////////////////////////////////���տ�������////////////////////////////////////////////////
/***********************************************************************************************/
extern "C" I32 _declspec(dllexport)ML_SetMode(U8 mod);//mod=1ʾ��ģʽ�����Ա��2=�Զ������Աģʽ
extern "C" I32 _declspec(dllexport)ML_GetMode(U8* mod);//��ȡģʽ��
//#define MOD_Teach 1
//#define MOD_AUTO 2
/////////////////����֮���������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_X_positive(U8 *pGPI);//X+,Z+,R1+�����򰴼��źŶ�ȡ
extern "C" I32 _declspec(dllexport)ML_READ_KEY_X_negative(U8 *pGPI);//X-,Z-,R1-�����򰴼��źŶ�ȡ
extern "C" I32 _declspec(dllexport)ML_READ_KEY_Y_positive(U8 *pGPI);//Y+,R+,R2+�����򰴼��źŶ�ȡ
extern "C" I32 _declspec(dllexport)ML_READ_KEY_Y_negative(U8 *pGPI);//Y-��R-,R2-�����򰴼��źŶ�ȡ
extern "C" I32 _declspec(dllexport)ML_READ_KEY_SEL_1(U8 *pGPI);//XY��ZR��ѡ��
extern "C" I32 _declspec(dllexport)ML_READ_KEY_SEL_2(U8 *pGPI);//R1,R2��ѡ��
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_FAST_SLOW(U8 *pGPI);//���ֶ��������١�����


/////////////////����֮�������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_FIX_POSITION(U8 *pGPI);//���ֶ�����λ������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_NEXT(U8 *pGPI);//���ֶ�����һ��������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_UpData_POS(U8 *pGPI);//���ֶ����������ꡰ����
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_START(U8 *pGPI);//���ֶ�������������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_STOP(U8 *pGPI);//���ֶ���ֹͣ������

extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_FORWARD_TIN(U8 *pGPI);//���ֶ�������������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_BACK_TIN(U8 *pGPI);//���ֶ�������������
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_BLOWER(U8 *pGPI);//���ֶ�����ϴ������



//I/O��������֮�������
extern "C" I32 _declspec(dllexport)ML_Scan_Key(void);//�ֱ�ȫ��ɨ�衣
extern "C" I32 _declspec(dllexport)ML_Set_Key_Doing(void);//���ܣ��԰������������Ӧ���ܵ��øú�����
                                                          //Ӧ����ɨ�谴��������һ����������ɨ��Ƶ���У�ȷ���ܱ�����ɨ�衣
extern "C" I32 _declspec(dllexport)ML_FF_Manual_FT(U8 dir,U8 enable);//���������������dir��1�ͣ�0�ˣ�enable��0ֹͣ��1��Ч
extern "C" I32 _declspec(dllexport)ML_Set_BLOW_Output(void);//����
extern "C" I32 _declspec(dllexport)ML_Clear_BLOW_Output(void);//ֹͣ����

//////////////////////////////////�����������///////////////////////////////////////
/////////////����ϵͳ����
extern "C" I32 _declspec(dllexport)ML_MovePTP (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH) ;//����һ����·���ƶ���Ŀ��㡣
extern "C" I32 _declspec(dllexport)ML_MovePTP_Rapid (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH);//�Ż�ʱ���õ��ƶ����� 
extern "C" I32 _declspec(dllexport)ML_MovePTPXYR (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH); //�켣��Z������X,Y,R������XYR��ֹͣ��Z����ѹ��

extern "C" I32 _declspec(dllexport)ML_F_Go_fixposition_Point(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed);//��λ����,��atspeed�ٶȶ�λ����ǰ��xyzr,�ƶ�ʱ����ƫ��ZH��
extern "C" I32 _declspec(dllexport)ML_F_Go_Empty_Point(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,U32 Delay_time);//�յ㺯����������Ϊһ�����ɵ㣬�����ƶ��еĸ��棬������ͣʱ������һ����ȫ��λ�á�
//extern "C" I32 _declspec(dllexport)ML_F_Go_Clear(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 CLear_H,U32 Delay_time);//������ϴ
extern "C" I32 _declspec(dllexport) ML_F_Go_Clear(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 CLear_H,U32 Delay_time,F64 FT_CL_Length,F64 FT_CL_Speed,U8 portno);

/////////////�������ղ�������
extern "C" I32 _declspec(dllexport)ML_FF_SetAT_BT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);//
                                        //�Զ������������� ע��:v_G_SVH_speed�ǳ��ȵ�λ,��ms������̫��;
extern "C" I32 _declspec(dllexport)ML_FF_SetAT_FT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);//�Զ�������������
										//ע��:v_G_SVH_speed�ǳ��ȵ�λ,��ms������̫��;
extern "C" I32 _declspec(dllexport)ML_FF_Forward_Tin(U8 dir,F64 Length,F64 speed);//�Զ�ģʽ��,dir=����0������1									
extern "C" I32 _declspec(dllexport)ML_FF_SetManual_FT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);
															//ע��:v_G_SVH_speed�ǳ��ȵ�λ,��ms������̫��;
extern "C" I32 _declspec(dllexport)ML_FF_SetManual_BT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);
	                                                        //ע��:v_G_SVH_speed�ǳ��ȵ�λ,��ms������̫��;
extern "C" I32 _declspec(dllexport)ML_FF_Manual_FT(U8 dir,U8 enable);//�ֶ�����������,dir=0,����;1����

extern "C" I32 _declspec(dllexport)ML_FF_Stop_Tin(U8 enable);//20150529��0,����;1����������ֻ�ڡ��켣������ʱ����Ϊ1��������ʽȫ��Ϊ0
extern "C" I32 _declspec(dllexport)ML_FF_Set_TinFace(U8 vSolderFace_style);//20150529��������ͷ�����淽��0=��࣬1Ϊ���Ҳ࣬2Ϊǰ�棬3Ϊ���棺
                                                         //Ϊ������Ŀ����������Ŀ�½��͵���ʱ����һ�Ρ�
                             //U8 SolderFace_style  �ڻ����󣬳�ʼ���������淽��0=��࣬1Ϊ���Ҳ࣬2Ϊǰ�棬3Ϊ����
/////////////���Ӷ����켣�˶�����
extern "C" I32 _declspec(dllexport)ML_FF_Set_Do_Step_style(U8 vDo_Step_style);//20150529���õ�ǰִ��������ͣ�����U8 Do_Step_style=0(��ʼ��),1���м��,2�����һ�

extern "C" I32 _declspec(dllexport)ML_FF_Get_Solder_pStatus_flag(U32 *V_Work_Point_step_num ,U8 *V_ML_V_Soldering_Status);//���ڻ�õ�ǰ����״̬�����Ӳ��������״̬��־��
//ML_V_Soldering_Status = 1������,0������ɣ�
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_Point(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,U8 FT_Pre_Modle,U8 Soldering_style,F64 ShakeSolder_Length,F64 ShakeSolder_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S_speed,F64 S2_Length,F64 S2_T_Length,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style);
//Optimize_style==0,Ĭ��ƽ�ȣ�1���Ż���2�����١�//20160316 ����Optimize_style==2
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_line(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,F64 TH_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S1_speed,F64 S2_Length,F64 S2_T_Length,F64 S2_speed,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style);
//Optimize_style==0,Ĭ��ƽ�ȣ�1���Ż���2�����١�//20160316 ����Optimize_style==2
extern "C" I32 _declspec(dllexport)ML_FF_Set_DLL_Return(U8 vSet_DLL_Return);//U8 Set_DLL_Return;//201150529,����λ����Ҫ����ֹͣ��λ����ĳһ�߳�ʱ������ͨ�����������롣vSet_DLL_Return=1��Ч��

///////////////////////˫ƽ̨/////////
extern "C" I32 _declspec(dllexport)ML_FF_Set_Act_Y2(U8 vSet_Act_Y2);//U8 Set_Act_Y2  :20150611����λ���ж�
//��ǰ���õ���Y1����Y2��Set_Act_Y2��ʼ��Ϊ0��Ĭ��XYִ�У�����Ҫ����Y2��ͨ���ú�������Set_Act_Y2 = 1;DLL����XY2ִ��
///////////////////////����ת��//////
extern "C" I32 _declspec(dllexport)ML_FF_Set_NOAct_R(U8 vSet_NOAct_R);
//U8 Set_NOAct_R = 0;//  :20160402����λ�����������н����жϣ��������R�ᣬ��ֵΪ0��������R�ᣬ��ֵΪ1���ڳ�ʼ��ʱ����
extern "C" I32 _declspec(dllexport)ML_FF_Set_DINGWEI_HZ(U8 vSet_DINGWEI_HZ);//20170326�Ƿ����ö�λ���������������ܣ�0Ϊ�����ã�1Ϊ���á��ں�����������Ϊ0������˿��������Ϊ1��
//����˵�������豸��ʼ��ʱ����һ�μ��ɡ�
////////////////////////����Ϻ���
extern "C" I32 _declspec(dllexport)ML_FF_Set_WorkLine_NUM(U8 vSet_WorkLine_NUM);//U8 Set_WorkLine_NUM  :20150805����λ���жϲ����á�
//U8 Set_WorkLine_NUM=0,��ͨ�Ϻ���1��������2����м䣬3���δ��
//
extern "C" I32 _declspec(dllexport)ML_SetSVHmax_Longth(U8 AxisNo,U8 LongthNo, U32 VHPPS);//20150809
extern "C" I32 _declspec(dllexport)ML_GetSVHmax_Longth(U8 AxisNo,F64 Longth_Per_Cur, U32 *VHPPS);

extern "C" I32 _declspec(dllexport)ML_Set_HomingStyle(U8 vHomingStyle);//20151029,0Ϊ�����ͼ�ͣ���㷽ʽ��1Ϊָ��͸�λ���㷽ʽ��
//20160103������������̬�������ܺ������ڡ��߼��������ӡ���̬�����ܡ��񴴽����յ㡱���ơ�����R1,R2���ꡣ
extern "C" I32 _declspec(dllexport)ML_Change_Position(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,U32 Delay_time,F64 r1,F64 r2);//Delay_time = 100
//20160103������������̬��������������������������ť����Чʱ��������̬����������Ч��
//������������Խ�����̬���������Ż�ִ����Ч��
extern "C" I32 _declspec(dllexport)ML_Set_Change_PositionStyle(U8 vF_Change_Position_ctrl);//20151029F_Change_Position_ctrl
extern "C" I32 _declspec(dllexport)ML_Move_R1R2(F64 r1,F64 r2,F64 atspeed,U32 Delay_time);//20160130,���Խ���R1R2���ƶ���atspeed��15
///////////////////////////////////
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_Point_2R(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,U8 FT_Pre_Modle,U8 Soldering_style,F64 ShakeSolder_Length,F64 ShakeSolder_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S_speed,F64 S2_Length,F64 S2_T_Length,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style,F64 r1,F64 r2,U32 Delay_time);
//Optimize_style==0,Ĭ��ƽ�ȣ�1���Ż���2�����١�//20160316 ����Optimize_style==2
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_line_2R(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,F64 TH_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S1_speed,F64 S2_Length,F64 S2_T_Length,F64 S2_speed,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style,F64 Rr1,F64 Rr2,U32 Delay_time);
//20180203����ת���㷨ʵ��
/*���ܣ�����ĳһ���R=0ʱ��Ӧ�������꣨F64 x0, F64 y0,F64 z0,F64 r0����R=180ʱ��Ӧ�������꣨F64 x180, F64 y180,F64 z180,F64 r180��
�������ʼ�Ƕ�INITANGLE�����ȣ�����ת�뾶TURNING_RADIUS��mm����
����˵���������������x180,y180,x0,y0,���������ģ���λMM���
�ش����������ʼ�Ƕ�INITANGLE�����ȣ�����ת�뾶TURNING_RADIUS��mm����������ֵ��Ӧһ��������̬��һ����Ŀһ��һ�飩��
*/
extern "C" I32 _declspec(dllexport)ML_F_Get_TRAD_IANG(F64 x0, F64 y0,F64 z0,F64 r0,F64 x180, F64 y180,F64 z180,F64 r180,F64 *TURNING_RADIUS,F64 *INITANGLE);
//��֪��F64 TURNING_RADIUS,F64 INITANGLE,F64 �͵��������F64 x, F64 y,F64 z,F64 r��������һ���������ꡣ
extern "C" I32 _declspec(dllexport)ML_F_Get_Normalize(F64 x, F64 y,F64 z,F64 r,F64 TURNING_RADIUS,F64 INITANGLE,F64 *Xn, F64 *Yn,F64 *Zn,F64 *Rn);

/*���ܣ�����ƫ��ǰ�ο���1��F64 x0, F64 y0,F64 z0,F64 r0����ƫ��ǰ�ο���2��F64 x1, F64 y1,F64 z1,F64 r1����
�Լ�����ƫ�Ƴ���TURNING_RADIUS����ʼ�Ƕ�INITANGLE
 ��ƫ�ƺ�ο���1��F64 x01, F64 y01,F64 z01,F64 r01����ƫ�ƺ�ο���2��F64 x11, F64 y11,F64 z11,F64 r11��    
��F64 x, F64 y,F64 z,F64 r,���������ת�����F64 x2, F64 y2,F64 z2,F64 r2,��������һ��Լ����Z������ԭֵ
��ͬ,
*/
extern "C" I32 _declspec(dllexport)ML_F_Get_x2y2z2r2(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 x, F64 y,F64 z,F64 r,F64 TURNING_RADIUS,F64 INITANGLE,
				F64 x01, F64 y01,F64 z01,F64 r01,F64 x11, F64 y11,F64 z11,F64 r11,
					  F64 *x2, F64 *y2,F64 *z2,F64 *r2);//
