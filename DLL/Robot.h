#include "MDH1K.h"
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<conio.h>
#include<locale.h>
#include<iostream.h>


////////20150605,在线式底层软件
///////////////////////////////////常量////////////////////////////////////
typedef unsigned char    U8;
typedef signed char	 I8;
typedef unsigned short   U16;
typedef signed short     I16;
typedef unsigned long    U32;
typedef signed long      I32;
//typedef signed long long I64;//20151226,编译不通过出错。
typedef float            F32;
typedef double           F64;


/////////////////
#define   E           extern
#define MAX_PULSE   100000
#define PI   3.1415926
#define Safety_Belt_L   10.0  //手动按操作下运动极限安全距离
#define Safety_Belt_A   10.0  //手动按操作下运动极限安全距离
#define RobotDLLVer     1708261355   //驱动版本号年月日时分

///////////////////////////////////////////////////////
//      轴名称  电机索引号
#define a_x		0
#define a_y		1
#define a_z		2
#define a_r		3
#define a_r1	5	  //姿态轴
#define a_r2	6	  //姿态轴
#define a_s		4	  //送锡轴
#define a_e		7     //该轴未定义，暂定为外部轴


///////////////////////////////////////////////////////
//一下定义最终设置依据电器信号对照表确定
#define key_Xp  12 //需要与电器定义一致ZZ,R2Z
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

#define BLOW_DO  6		//吹气
#define Cylinder_DO  0	//气缸
#define LED_START_DO  7 //指示灯启动按键指示灯信号_DO




//////////////////////////////////////////////////////////////
#define MOD_Teach 1 //手动模式，示教
#define MOD_AUTO 2  //自动模式，手柄操作按键禁用


///////////////////////////////////全局变量声明/////////////////////////////////////
E volatile  struct AXIS Axis_x,Axis_y,Axis_z,Axis_r,Axis_r1,Axis_r2,Axis_s,Axis_e;
E volatile  struct Forward_Tin HD_FT,HD_BT ;//声明手动送锡、退锡
E volatile  struct KEY KEY ;//声明手动送锡、退锡
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
E U8 ML_FF_Stop_Tin_Flag;//20150529，属于临时控制参数。执行"轨迹"时,关掉送锡驱动器。
E U8 SolderFace_style;//20150529,属于工程参数，在对应工程中是一致的。SolderFace_style
E U8 Do_Step_style;//201150529,执行的步骤的类型，0(起始项),1（中间项）,2（最后一项）
E U8 Set_DLL_Return;//201150529,当上位机需要立即停止下位机的某一线程时，可以通过请求来申请。
E U8 RobotStyle;//20150608
E U8 Set_Act_Y2;//  :20150611由上位机判断
E U8 Set_DINGWEI_HZ;//20170326设置定位函数是否启用提升到功能。
//当前启用的是Y1或者Y2，Set_Act_Y2初始化为0，默认XY执行，当需要激活Y2，通过该函数设置Set_Act_Y2 = 1;DLL将按XY2执行
E U8 Set_NOAct_R;////  :20160402由上位机机型设置中进行判断，如果启用R轴，则值为0，不启用R轴，则值为1；
E U8 Set_WorkLine_NUM;//  :20150805,U8 Set_WorkLine_NUM=0,普通拖焊，1，多段首项，2多段中间，3多段未段

E U8 HomingStyle;//回零方式，=0开机回零，1非开机回零
E U8 DouhanStyle;//抖焊方式，=0普通方式，1全定点送锡抖焊，2半定点送锡抖焊；20151119

E U8 F_Change_Position_ctrl;//F_Change_Position_ctrl==0,执行，F_Change_Position_ctrl==1不执行
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
/////////////////读写变量///////////////
E	DWORD	m_dwAddr;
E	int		m_iDValue;
E	CString	m_strValue;
E    unsigned long m_iDValuedata;
E	CString m_strValuedata;
E	CString	m_strTime;
E	CString			tempStr;

////////////////////////////////全局变量定义/////////////////////////

F64 HM_FT_MAX_Speed,HM_FT_Min_Speed;//单位MM,从软件面板设置。
U32 overtimesflag,count_time,Work_Point_step_num = 0;
U8	ML_V_Soldering_Status,v_Forward_Tin_Status_flag;//焊接状态标志1：焊接中，0无焊接，全局变量标志。
U8 Work_State_Type,ML_V_Soldering_Over_Time_Flag;
U32 Over_Time_count;
U8 Homing_Status_flag,MIming_Status_flag=0;
U32 Delywkl_chk_counter,Delywkp_chk_counter;//20151004
U32 ML_Check_Mima_Legal_fail_count = 0;
I32 FHZ = 0;

////////////子函数运行标志
U8 ML_MovePTP_Step_Num,ML_MovePTP_State;//ML_MovePTP()

U8 ML_FF_Stop_Tin_Flag =0;//20150529,初始化为送锡，0：送，1：不送
U8 SolderFace_style = 0;//20150529,在回零点后，初始烙铁上锡面方向：0=左侧，1为：右侧，2为前面，3为后面
U8 Do_Step_style = 0;//201150529,执行的步骤的类型，0(起始项),1（中间项）,2（最后一项），默认起始项
U8 Set_DLL_Return = 0;//201150529,当上位机需要立即停止下位机的某一线程时，可以通过请求来申请。0:不申请，1：申请
U8 RobotStyle = 0;//20150608,机器人本体结构形式，0：台式单轴（XYZR），1：在线式(Y,R反转)，2：台式双平台（Y1,Y2），3：待定
U8 Set_Act_Y2 = 0;//  :20150611由上位机判断
U8 Set_DINGWEI_HZ = 0;//20170326初始化为0，不启用。这样焊锡机不用修改程序。
//当前启用的是Y1或者Y2，Set_Act_Y2初始化为0，默认XY执行，当需要激活Y2，通过该函数设置Set_Act_Y2 = 1;DLL将按XY2执行
U8 Set_NOAct_R = 0;//  :20160402由上位机机型设置中进行判断，如果启用R轴，则值为0，不启用R轴，则值为1；
U8 Set_WorkLine_NUM = 0;//  :20150805,U8 Set_WorkLine_NUM=0,普通拖焊，1，多段首项，2多段中间，3多段未段

U8 HomingStyle = 0;
U8 DouhanStyle = 3;//抖焊方式，=0普通方式，1普通抖焊；2全定点送锡抖焊，3半定点送锡抖焊；20151119为保定设置的专用抖焊。

U8 F_Change_Position_ctrl = 0;
/////////////
//////////////安全变量////////////////////	
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
/////////////////读写变量///////////////
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
///////////////////////机构特性参数变量////////////////////////////////////////////////////
U32 PULSE_Per;//电机步数/转数，细分参数,当前为6400
U32 MAX_Speed;//设定范围：0~100000，以脉冲数设置。MAX_Speed<MAX_PULSE
F32 MAX_route;//最大行程，单位MM，或度，范围根据机械结构确定，
F32 MIN_route;//最小行程，单位MM，或度，范围根据机械结构确定，

F64 STEP_Length;//单位MM，或度，范围根据机械结构确定,STEP_Length=2*PI*R/PULSE_Per;=360/PULSE_Per

///////////////////////机器坐标系变量////////////////////////////////////////////
F32 Current_Length;//单位MM，或度，当前坐标值，MIN_route<Current_Length<MAX_route
F32 Target_Length;//单位MM，或度，目标坐标值，MIN_route<Target_Length<MAX_route
I32 CurrentPosition;//当前坐标，单位PPS，记录的脉冲数，对应我们的机构=对X,Y,Z而言，均为正值，CurrentPosition=Current_Length/STEP_Length
					//找到零点后，将该值设为0；具有绝对坐标值，
					//对R旋转而言，是一个正负值，角度，当前为176，-174，从零点正方向为正，反方向为负，具有绝对坐标值。
					//对R1,R2,对应也是角度，从零点正方向为正，反方向为负，具有绝对坐标值。
					//对S轴进行的是相对运动，没有行程的概念，采用相对移动。
I32 m_Target_Axis;//目标坐标，单位PPS，要达到的脉冲数(绝对值)或要偏移的脉冲数(相对值)。

///////////////////////用户（或工件坐标系）坐标系变量/////////////////////////////
F32 User_Current_Length;//单位MM，或度，是以用户设定的新原点形成的当前坐标的相对坐标值，相对坐标偏移，有利于直观展现。
F32 User_Target_Length;//单位MM，或度，目标坐标值，以用户坐标系为参考的目标坐标值。
I32 User_Org_Position;//当前坐标，单位PPS，记录用户设置的零点实际坐标脉冲数。User_Org_Position = 设置零点的CurrentPosition，仅对X,Y,Z.

////////////////////////单位转换变量/////////////////////////////////////////////////////

F32 Mil_Current_Length;//单位mil，对长度单位做变化，从mm转换为mil。1mil=0.0254mm,适合X,Y轴
F32 Mil_Target_Length;//单位mil。适合X,Y轴

////////////////////////手动模式下的速度参数/////////////////////////////////////////////////////

U32 Jog_MAX_Speed;//设定范围：0~100000，以脉冲数设置。响应轴的Jog_MAX_Speed<MAX_Speed
U32 Jog_MIN_Speed;//设定范围：0~100000，以脉冲数设置。
U32 Jog_Current_Speed;//设定范围：0~100000，以脉冲数设置。Jog_MIN_Speed<Jog_Current_Speed<Jog_MAX_Speed
U8  Jog_index_Speed;//设置手动下实际运行速度与最大速度的百分比，0,1,2,3。。。10，表示0%，10%，。。。。100%
F32 INCJogValue;//点动时
U8  SetJogMode;

////////////////////////自动模式下的速度参数///////////////////////////////////////////////////


////////////////////////速度曲线设置参数变量/////////////////////////////////////////////////

U32 G_TVH;//加减速曲线设置参数变量,小于MAX_Speed
U32 G_TVL;
U32 G_SVH;//小于MAX_Speed
U32 G_SVL;
U16 G_TTAcc;
U16 G_TTDec;

U16 G_STAcc;
U16 G_STDec;
U16 G_STsAcc;////应小于G_STAcc的一半
U16 G_STsDec;////应小于G_STDec的一半
////////////////////////org回原点的速度参数
U32 VL_HomePPS;
U32 VH_HomePPS;//小于MAX_Speed
U16 TAccDec_ms;

U32 Vhmax[22];//20150809,支持依据距离设置加减速参数。
};
volatile  struct AXIS Axis_x,Axis_y,Axis_z,Axis_r,Axis_r1,Axis_r2,Axis_s,Axis_e;//声明各轴的结构变量


struct Forward_Tin{
	////////////////////////送锡速度曲线设置参数变量/////////////////////////////////////////////////

U32 G_TVH_speed;//加减速曲线设置参数变量,小于MAX_Speed
U32 G_TVL;
F64 G_SVH_speed;//小于MAX_Speed
U32 G_SVL;
U16 G_TTAcc;
U16 G_TTDec;

U16 G_STAcc;
U16 G_STDec;
U16 G_STsAcc;////应小于G_STAcc的一半
U16 G_STsDec;////应小于G_STDec的一半


};

volatile  struct Forward_Tin HD_FT,HD_BT,AT_FT,AT_BT ;//声明手动送锡、退锡

/////////////////////////操作面板和手持器的输入///////////////////////////////////////
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
volatile  struct KEY KEY ;//声明手动送锡、退锡

///////////////////////////////////反调函数///////////////////////////////////////////////////////
extern "C" void _declspec(dllexport)ML_MDH1K_AttachErrorReport (ErrorHandle ErrorReportFunction);
//extern "C" I32 _declspec(dllexport)ML_MDH1K_RegisterGPICallBack(U8 Num, CallBack Event_PositiveEdge, PVOID pArg_PositiveEdge
//													 , CallBack Event_NegativeEdge, PVOID pArg_NegativeEdge);
//extern "C" I32 _declspec(dllexport)ML_MDH1K_DeRegisterGPICallBack(U8 Num);//取消已注册之回调函数。
//extern "C" I32 _declspec(dllexport)ML_MDH1K_EnableGPICallBack();
//extern "C" I32 _declspec(dllexport)ML_MDH1K_DisableGPICallBack();


///////////////////////////////////迈力机器人平台部分/////////////////////////////////////
/**********************************************************************************************/
////////////系统函数
extern "C" void _declspec(dllexport)ML_Sleep(U32 mscount);//延时,1MS为单位.
extern "C" I32 _declspec(dllexport)ML_FF_start_count_overtime(void);//超时计数函数。需要在一个固定较高频率的周期中调用。
extern "C" I32 _declspec(dllexport)ML_Set_RobotStyle(U8 vRobotStyle);//为了DLL的维护管理方便，使得多个机型采用同一个DLL，设置机型RobotStyle=0,01,10，11:0,1,2,3

///////////////////////////////////硬件板卡系统设置//////////////////////////////
////////////伺服参数设置
extern "C" I32 _declspec(dllexport)ML_Initial_MAXPULSE(U32 DriverSpec_PPS);//初始化函数，设置了伺服的最大输入脉冲,
extern "C" I32 _declspec(dllexport)ML_SetPulseOutputType_ALL(U8 v_PulseOutputType);//脉冲类型相同
extern "C" I32 _declspec(dllexport)ML_SetPulseOutputType_Axis(U8 AxisNo,U8 v_PulseOutputType);//设置各轴不同的脉冲类型
extern "C" I32 _declspec(dllexport)ML_Initial_All(void);//硬件初始化,DriverSpec_PPS=100000,v_PulseOutputType=0;
extern "C" I32 _declspec(dllexport)ML_ServoONOFF(U8 AxisNO, U8 onoff,U8 allsg);//关闭开启伺服，AxisNO：0~7；onoff：0关闭、1开启；allsg：0设置对应轴，1全部轴设置
extern "C" I32 _declspec(dllexport)ML_GetServoONStatus(U8 AxisNo, U8 *pServoOnStatus);//读取当前伺服状态，AxisNO：0~7；pServoOnStatus：0关闭、1开启
extern "C" I32 _declspec(dllexport)ML_ExitForm(void);//关闭运动板卡

////////////机械与电机参数设置
extern "C" I32 _declspec(dllexport)ML_SetHardSys_MAX_route(U8 AxisNo,F32 vMIN_route,F32 vMAX_route);//设置各轴的行程
extern "C" I32 _declspec(dllexport)ML_SetHardSys_STEP_Length(U8 AxisNo,F64 vSTEP_Length);//设置各轴的步距
extern "C" I32 _declspec(dllexport)ML_SetHardSys_PULSE_Per(U8 AxisNo,U32 vPULSE_Per);//设置各轴的细分脉冲数，与电机驱动器的设置有关，当前6400
extern "C" I32 _declspec(dllexport)ML_SetHardSys_MAX_Speed(U8 AxisNo,U32 vMAX_Speed);//设置各轴的最大限制速度
extern "C" I32 _declspec(dllexport)ML_GetVelocityLimit(U8 AxisNo,U32 *VLimit_pps);//读取限制的速度值
extern "C" I32 _declspec(dllexport)ML_GetCurrentVelocity(U8 AxisNo,I32 *pVelocity);//获得当前速度值,脉冲数。
extern "C" I32 _declspec(dllexport)ML_SetVPercentage(U32 VPercentage);//设置速度百分比。参数说明：VPercentage：10~200，
extern "C" I32 _declspec(dllexport)ML_SetSVelocityProfile_Para(U8 AxisNo, U32 vVLPPS, U32 vVHPPS, U16 vTaccMS, U16 vTdecMS, U16 vTsaccMS, U16 vTsdecMS);//设置系统加速度曲线参数。

////////////////////////////////////////机器人运动相关////////////////////////////////
///////////////回原点设置与操作
extern "C" I32 _declspec(dllexport)ML_SetHomingVelocityProfile(U8 AxisNo,U32 VL_HomePPS,U32 VH_HomePPS,U16 TAccDec_ms);//设置单轴回原点运动曲线
extern "C" I32 _declspec(dllexport)ML_StartHoming(U8 AxisNo, I8 HomeDir);//单轴回原点操作
extern "C" I32 _declspec(dllexport)ML_SetHomingVelocityProfile_Param(U8 AxisNo,U32 VL_HomePPS,U32 VH_HomePPS,U16 TAccDec_ms);//
extern "C" I32 _declspec(dllexport)ML_F_MovToORG_D(U8 ALL_OR_PART);//总体回零点操作,1=ALL，全部轴参与回原点；0=R1,R2不参与回原点
extern "C" I32 _declspec(dllexport)ML_GetORG(U8 AxisNo, U8 *pORG);//读取原点输入信号，AxisNo[0,7]，
extern "C" I32 _declspec(dllexport)ML_GetHoming_Status_flag(U8* mod);//读取回原点状态，1为完成，0为未完成。
///////////////JOG手动指令///////
extern "C" I32 _declspec(dllexport)ML_SetJogminmaxSpeed ( U8 AxisNo,U32 vJog_MIN_Speed ,U32 vJog_MAX_Speed) ;//设置手动方式是各个轴的速度，其中X,Y,Z,R,R1,R2,E单位为脉冲数，s轴为mm。
extern "C" I32 _declspec(dllexport)ML_SetINCJogValue ( I32 index ) ;//设置点动步距(长度或角度)，index:1 = 0.01,2 = 0.1 ,3 = 1,4=10
extern "C" I32 _declspec(dllexport)ML_Set_Axis_INCJogValue ( U8 axis,I32 index );//以单个轴为对象设定手动移动模式的吋动数值===点动步长index:1 = 0.01,2 = 0.1 ,3 = 1,4=10
extern "C" I32 _declspec(dllexport)ML_SetExtSpeed ( U8 AxisNo,U8 index ) ;//设置手动速度百分比，Jog_MAX_Speed*0%~100%，对应0--10
extern "C" I32 _declspec(dllexport)ML_SetJogMode ( U8 index );//设置手动移动模式 ，index:0连续，1为点动
extern "C" I32 _declspec(dllexport)ML_Set_Axis_JogMode ( U8 axis,U8 index );//以单个轴为对象设定手动移动模式，index:0连续，1为点动 
extern "C" I32 _declspec(dllexport)ML_SetJogINCMove ( U8 axis, I32 direction, I32 enable );//手动点动移动 
extern "C" I32 _declspec(dllexport)ML_SetJogMove ( U8 axis, I32 direction, I32 enable );//手动连续移动
//////////////坐标设置与读取
extern "C" I32 _declspec(dllexport)ML_SetCurrentPosition(U8 AxisNo,I32  vCurrentPositionPuls);//设置当前轴的绝对坐标，这里可能就是在设置零点时使用。
extern "C" I32 _declspec(dllexport)ML_GetPosJoint( U8 AxisNo,I32* pPositionPulse);//读取当前轴的当前坐标，绝对坐标,脉冲数.
extern "C" I32 _declspec(dllexport)ML_GetPosCartesion( U8 AxisNo, F64* pPosition_Length);//读取当前轴的当前空间坐标，长度或者角度
extern "C" I32 _declspec(dllexport)ML_GetMotionStatus(U8 AxisNo, U8 *pStatus);//获得当前运动状态 

//////////////移动功能函数
extern "C" I32 _declspec(dllexport)ML_MovPosition(U8 AxisNo,F64 vTarget_Position,F64 vSpeed_H);//单轴移动到目标绝对值坐标位置，单位为长度或角度。
extern "C" I32 _declspec(dllexport)ML_MovReDistance(U8 AxisNo,F64 vRe_Distance,F64 vSpeed_H);//单轴移动偏移的距离,单位为长度或角度
extern "C" I32 _declspec(dllexport)ML_SetPosCartesionP (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H); //ptp方式移动到目标点（xyzr）,单位MM或°
extern "C" I32 _declspec(dllexport)ML_Mov_Line_XY(F64 x, F64 y,F64 vSpeed_H);//在X,Y面上做直线插补移动到目标点。单位MM或°
extern "C" I32 _declspec(dllexport)ML_Mov_Line_XYZ(F64 x, F64 y,F64 z,F64 vSpeed_H);//在X,Y,z做直线插补移动到目标点。
extern "C" I32 _declspec(dllexport)ML_Mov_Line_XYR(F64 x, F64 y,F64 r,F64 vSpeed_H);//在X,Y,r做直线插补移动到目标点
///////////////////////////I/O输出，系统相关函数,支持16路开关量输入、输出，0~15/////////////////////////////////
/////////////////开关量输入输出相关
extern "C" I32 _declspec(dllexport)ML_GetGPI(U8 Num,U8 *pGPI);//Num：0`15 GPIO，开关输入、按键输入读取调用总函数。
extern "C" I32 _declspec(dllexport)ML_GetInput(U8 index,U8 *pGPI);//采用左右极限作为I/O输入，作为用户扩展I/O输入，index[0,15]
extern "C" I32 _declspec(dllexport)ML_SetOutput(U8 Num);//开关输出调用该函数。设置、清楚、回读Num[0,15]
extern "C" I32 _declspec(dllexport)ML_ClearOutput(U8 Num);//开关输出清除,Num[0,15]
extern "C" I32 _declspec(dllexport)ML_GetOutput(U8 index,U8 *pGPO);//index[0,15]
/////////////////////////////停止、复位、急停//////////////////////////////////////////////
///////////暂停、复位
extern "C" I32 _declspec(dllexport)ML_StopAll(U8 Mode);//暂时停止八轴脉冲输出，Mode 0：减速，1：立即
extern "C" I32 _declspec(dllexport)ML_Stop(U8 AxisNo, U8 Mode);//暂时停止指定轴脉冲输出AxisNo[0,7]，Mode 0：减速，1：立即
extern "C" I32 _declspec(dllexport)ML_Restart(U8 AxisNo);//启动指定轴脉冲输出AxisNo[0,7]
////////////Estop
extern "C" I32 _declspec(dllexport)ML_GetEstop(U8 *pEstop);//急停信号输入读取
extern "C" I32 _declspec(dllexport)ML_EnableAutoEstop();//使能自动急停
extern "C" I32 _declspec(dllexport)ML_DisableAutoEstop();//不使能自动急停

//////////////////////////////产品版本信息/////////////////////////////////////////////////
extern "C" I32 _declspec(dllexport)ML_GetHardwareType(U8 *pHW_Type);//硬件构架
extern "C" I32 _declspec(dllexport)ML_GetFirmwareVersion(U8 *pFW_Ver);//软件版本
extern "C" I32 _declspec(dllexport)ML_GetRobotDLLVersion(U32 *pFW_Ver);//驱动版本

//////////////////////////////////////工艺开发部分////////////////////////////////////////////////
/***********************************************************************************************/
extern "C" I32 _declspec(dllexport)ML_SetMode(U8 mod);//mod=1示教模式或管理员，2=自动或操作员模式
extern "C" I32 _declspec(dllexport)ML_GetMode(U8* mod);//读取模式。
//#define MOD_Teach 1
//#define MOD_AUTO 2
/////////////////输入之机器人相关
extern "C" I32 _declspec(dllexport)ML_READ_KEY_X_positive(U8 *pGPI);//X+,Z+,R1+正方向按键信号读取
extern "C" I32 _declspec(dllexport)ML_READ_KEY_X_negative(U8 *pGPI);//X-,Z-,R1-负方向按键信号读取
extern "C" I32 _declspec(dllexport)ML_READ_KEY_Y_positive(U8 *pGPI);//Y+,R+,R2+正方向按键信号读取
extern "C" I32 _declspec(dllexport)ML_READ_KEY_Y_negative(U8 *pGPI);//Y-，R-,R2-负方向按键信号读取
extern "C" I32 _declspec(dllexport)ML_READ_KEY_SEL_1(U8 *pGPI);//XY与ZR的选择
extern "C" I32 _declspec(dllexport)ML_READ_KEY_SEL_2(U8 *pGPI);//R1,R2的选择
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_FAST_SLOW(U8 *pGPI);//读手动”快慢速“按键


/////////////////输入之工艺相关
extern "C" I32 _declspec(dllexport)ML_READ_KEY_FIX_POSITION(U8 *pGPI);//读手动”定位“按键
extern "C" I32 _declspec(dllexport)ML_READ_KEY_NEXT(U8 *pGPI);//读手动”下一步“按键
extern "C" I32 _declspec(dllexport)ML_READ_KEY_UpData_POS(U8 *pGPI);//读手动”更新坐标“按键
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_START(U8 *pGPI);//读手动”启动“按键
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_STOP(U8 *pGPI);//读手动”停止“按键

extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_FORWARD_TIN(U8 *pGPI);//读手动”送锡“按键
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_BACK_TIN(U8 *pGPI);//读手动”退锡“按键
extern "C" I32 _declspec(dllexport)ML_READ_KEY_HM_BLOWER(U8 *pGPI);//读手动”清洗“按键



//I/O输出，输出之工艺相关
extern "C" I32 _declspec(dllexport)ML_Scan_Key(void);//手柄全面扫描。
extern "C" I32 _declspec(dllexport)ML_Set_Key_Doing(void);//功能：对按键等输入的响应功能调用该函数。
                                                          //应在与扫描按键函数在一个数量级的扫描频率中，确保能被快速扫描。
extern "C" I32 _declspec(dllexport)ML_FF_Manual_FT(U8 dir,U8 enable);//送锡或退锡输出，dir：1送，0退；enable：0停止，1有效
extern "C" I32 _declspec(dllexport)ML_Set_BLOW_Output(void);//吹气
extern "C" I32 _declspec(dllexport)ML_Clear_BLOW_Output(void);//停止吹气

//////////////////////////////////焊锡工艺相关///////////////////////////////////////
/////////////工艺系统函数
extern "C" I32 _declspec(dllexport)ML_MovePTP (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH) ;//按照一定的路线移动到目标点。
extern "C" I32 _declspec(dllexport)ML_MovePTP_Rapid (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH);//优化时调用的移动函数 
extern "C" I32 _declspec(dllexport)ML_MovePTPXYR (F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH); //轨迹：Z提升》X,Y,R启动》XYR均停止》Z轴下压。

extern "C" I32 _declspec(dllexport)ML_F_Go_fixposition_Point(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed);//定位函数,以atspeed速度定位到当前点xyzr,移动时提升偏移ZH。
extern "C" I32 _declspec(dllexport)ML_F_Go_Empty_Point(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,U32 Delay_time);//空点函数，可以作为一个过渡点，避免移动中的干涉，或者暂停时保留在一个安全的位置。
//extern "C" I32 _declspec(dllexport)ML_F_Go_Clear(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 CLear_H,U32 Delay_time);//插入清洗
extern "C" I32 _declspec(dllexport) ML_F_Go_Clear(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 CLear_H,U32 Delay_time,F64 FT_CL_Length,F64 FT_CL_Speed,U8 portno);

/////////////送锡工艺参数设置
extern "C" I32 _declspec(dllexport)ML_FF_SetAT_BT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);//
                                        //自动退锡参数设置 注意:v_G_SVH_speed是长度单位,是ms不可以太大;
extern "C" I32 _declspec(dllexport)ML_FF_SetAT_FT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);//自动送锡参数设置
										//注意:v_G_SVH_speed是长度单位,是ms不可以太大;
extern "C" I32 _declspec(dllexport)ML_FF_Forward_Tin(U8 dir,F64 Length,F64 speed);//自动模式下,dir=送锡0，退锡1									
extern "C" I32 _declspec(dllexport)ML_FF_SetManual_FT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);
															//注意:v_G_SVH_speed是长度单位,是ms不可以太大;
extern "C" I32 _declspec(dllexport)ML_FF_SetManual_BT_Speed(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);
	                                                        //注意:v_G_SVH_speed是长度单位,是ms不可以太大;
extern "C" I32 _declspec(dllexport)ML_FF_Manual_FT(U8 dir,U8 enable);//手动下送锡操作,dir=0,送锡;1退锡

extern "C" I32 _declspec(dllexport)ML_FF_Stop_Tin(U8 enable);//20150529；0,送锡;1，不送锡，只在“轨迹”功能时设置为1，其他方式全部为0
extern "C" I32 _declspec(dllexport)ML_FF_Set_TinFace(U8 vSolderFace_style);//20150529设置烙铁头送锡面方向。0=左侧，1为：右侧，2为前面，3为后面：
                                                         //为工程项目参数，在项目新建和登载时调用一次。
                             //U8 SolderFace_style  在回零点后，初始烙铁上锡面方向：0=左侧，1为：右侧，2为前面，3为后面
/////////////焊接动作轨迹运动函数
extern "C" I32 _declspec(dllexport)ML_FF_Set_Do_Step_style(U8 vDo_Step_style);//20150529设置当前执行项的类型，种类U8 Do_Step_style=0(起始项),1（中间项）,2（最后一项）

extern "C" I32 _declspec(dllexport)ML_FF_Get_Solder_pStatus_flag(U32 *V_Work_Point_step_num ,U8 *V_ML_V_Soldering_Status);//用于获得当前焊接状态，焊接步骤项、焊接状态标志。
//ML_V_Soldering_Status = 1焊接中,0焊接完成；
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_Point(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,U8 FT_Pre_Modle,U8 Soldering_style,F64 ShakeSolder_Length,F64 ShakeSolder_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S_speed,F64 S2_Length,F64 S2_T_Length,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style);
//Optimize_style==0,默认平稳，1：优化，2：高速。//20160316 新增Optimize_style==2
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_line(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,F64 TH_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S1_speed,F64 S2_Length,F64 S2_T_Length,F64 S2_speed,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style);
//Optimize_style==0,默认平稳，1：优化，2：高速。//20160316 新增Optimize_style==2
extern "C" I32 _declspec(dllexport)ML_FF_Set_DLL_Return(U8 vSet_DLL_Return);//U8 Set_DLL_Return;//201150529,当上位机需要立即停止下位机的某一线程时，可以通过请求来申请。vSet_DLL_Return=1有效；

///////////////////////双平台/////////
extern "C" I32 _declspec(dllexport)ML_FF_Set_Act_Y2(U8 vSet_Act_Y2);//U8 Set_Act_Y2  :20150611由上位机判断
//当前启用的是Y1或者Y2，Set_Act_Y2初始化为0，默认XY执行，当需要激活Y2，通过该函数设置Set_Act_Y2 = 1;DLL将按XY2执行
///////////////////////无旋转轴//////
extern "C" I32 _declspec(dllexport)ML_FF_Set_NOAct_R(U8 vSet_NOAct_R);
//U8 Set_NOAct_R = 0;//  :20160402由上位机机型设置中进行判断，如果启用R轴，则值为0，不启用R轴，则值为1；在初始化时进行
extern "C" I32 _declspec(dllexport)ML_FF_Set_DINGWEI_HZ(U8 vSet_DINGWEI_HZ);//20170326是否启用定位函数的提升到功能，0为不启用，1为启用。在焊锡机中设置为0，在螺丝机中设置为1，
//参数说明：在设备初始化时调用一次即可。
////////////////////////多段拖焊。
extern "C" I32 _declspec(dllexport)ML_FF_Set_WorkLine_NUM(U8 vSet_WorkLine_NUM);//U8 Set_WorkLine_NUM  :20150805由上位机判断并设置。
//U8 Set_WorkLine_NUM=0,普通拖焊，1，多段首项，2多段中间，3多段未段
//
extern "C" I32 _declspec(dllexport)ML_SetSVHmax_Longth(U8 AxisNo,U8 LongthNo, U32 VHPPS);//20150809
extern "C" I32 _declspec(dllexport)ML_GetSVHmax_Longth(U8 AxisNo,F64 Longth_Per_Cur, U32 *VHPPS);

extern "C" I32 _declspec(dllexport)ML_Set_HomingStyle(U8 vHomingStyle);//20151029,0为开机和急停回零方式，1为指令和复位回零方式。
//20160103增加了烙铁姿态调整功能函数。在“逻辑”中增加“姿态”功能。像创建“空点”类似。增加R1,R2坐标。
extern "C" I32 _declspec(dllexport)ML_Change_Position(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,U32 Delay_time,F64 r1,F64 r2);//Delay_time = 100
//20160103增加了烙铁姿态调整功能锁定函数，“锁定按钮”有效时锁定，姿态调整函数无效；
//“解锁”后可以进行姿态调整函数才会执行有效。
extern "C" I32 _declspec(dllexport)ML_Set_Change_PositionStyle(U8 vF_Change_Position_ctrl);//20151029F_Change_Position_ctrl
extern "C" I32 _declspec(dllexport)ML_Move_R1R2(F64 r1,F64 r2,F64 atspeed,U32 Delay_time);//20160130,可以进行R1R2的移动。atspeed《15
///////////////////////////////////
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_Point_2R(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,U8 FT_Pre_Modle,U8 Soldering_style,F64 ShakeSolder_Length,F64 ShakeSolder_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S_speed,F64 S2_Length,F64 S2_T_Length,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style,F64 r1,F64 r2,U32 Delay_time);
//Optimize_style==0,默认平稳，1：优化，2：高速。//20160316 新增Optimize_style==2
extern "C" I32 _declspec(dllexport)ML_F_Delay_Work_line_2R(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,F64 TH_speed,
					F64 S1_Length,F64 S1_T_Length,F64 S1_speed,F64 S2_Length,F64 S2_T_Length,F64 S2_speed,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style,F64 Rr1,F64 Rr2,U32 Delay_time);
//20180203坐标转换算法实验
/*功能：根据某一点的R=0时对应的轴坐标（F64 x0, F64 y0,F64 z0,F64 r0）和R=180时对应的轴坐标（F64 x180, F64 y180,F64 z180,F64 r180）
计算出初始角度INITANGLE（弧度）和旋转半径TURNING_RADIUS（mm）。
参数说明：输入变量关心x180,y180,x0,y0,其他不关心，单位MM或°
回传：计算出初始角度INITANGLE（弧度）和旋转半径TURNING_RADIUS（mm）。该两个值对应一种烙铁姿态（一个项目一般一组）。
*/
extern "C" I32 _declspec(dllexport)ML_F_Get_TRAD_IANG(F64 x0, F64 y0,F64 z0,F64 r0,F64 x180, F64 y180,F64 z180,F64 r180,F64 *TURNING_RADIUS,F64 *INITANGLE);
//已知：F64 TURNING_RADIUS,F64 INITANGLE,F64 和点的轴座标F64 x, F64 y,F64 z,F64 r，计算点归一化工件坐标。
extern "C" I32 _declspec(dllexport)ML_F_Get_Normalize(F64 x, F64 y,F64 z,F64 r,F64 TURNING_RADIUS,F64 INITANGLE,F64 *Xn, F64 *Yn,F64 *Zn,F64 *Rn);

/*功能：根据偏移前参考点1（F64 x0, F64 y0,F64 z0,F64 r0）和偏移前参考点2（F64 x1, F64 y1,F64 z1,F64 r1）。
以及刀具偏移长度TURNING_RADIUS，初始角度INITANGLE
 和偏移后参考点1（F64 x01, F64 y01,F64 z01,F64 r01）和偏移后参考点2（F64 x11, F64 y11,F64 z11,F64 r11）    
把F64 x, F64 y,F64 z,F64 r,进行坐标的转换输出F64 x2, F64 y2,F64 z2,F64 r2,，这里有一个约定，Z坐标与原值
相同,
*/
extern "C" I32 _declspec(dllexport)ML_F_Get_x2y2z2r2(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 x, F64 y,F64 z,F64 r,F64 TURNING_RADIUS,F64 INITANGLE,
				F64 x01, F64 y01,F64 z01,F64 r01,F64 x11, F64 y11,F64 z11,F64 r11,
					  F64 *x2, F64 *y2,F64 *z2,F64 *r2);//
