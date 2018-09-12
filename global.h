#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

#define APPLICATION_NAME "迈力机器人"

#define MAIN_WIDGET_COLOR 125,145,200
#define MID_WIDGET_COLOR 125,45,200

#define MAIN_STATUS_FONT_COLOR 250,180,100
#define MAIN_BUTTONS_FONT_COLOR 83,4,14
#define OPERATE_PROJECT_FONT_COLOR 20,95,120
#define OPERATE_BUTTONS_FONT_COLOR 20,25,20
#define EDIT_PROJECT_FONT_COLOR 200,195,120
#define TRACKS_COORDINATE_FONT_COLOR 20,95,120
#define DIALOG_BACKGROUND_COLOR 125,45,200

////以下为用户权限/////////
#define USER_RIGHT_OPERATOR     1       //操作员
#define USER_RIGHT_DESIGNER     2       //设计师
#define USER_RIGHT_ADMIN        3       //管理员
#define USER_RIGHT_VENDER       4       //厂家

/////////////////////////////////////////////////////////
//以下定义各种界面字体大小
#define MESSAGEBOX_FONT         10  //MessageBox的字体大小
#define LABEL_BUTTON_FONT       8  //界面Label及Button的字体大小
#define DLG_LABEL_BUTTON_FONT   10  //对话框的Label及Button的字体大小
#define WELD_DLG_BUTTON_FONT    12  //焊接对话框的Label及Button的字体大小
#define TMDLG_LABEL_BUTTON_FONT 15  //对话框的Label及Button的字体大小


///////////////以下为核心控制器动态库定义///////////***********************//////////////
///////////////////////////////////常量///////////////////
typedef unsigned char    U8;
typedef signed char      I8;
typedef unsigned short   U16;
typedef signed short     I16;
typedef unsigned long    U32;
typedef signed long      I32;
typedef float            F32;
typedef double           F64;


/////////////////
//#define   E             extern
#define MAX_PULSE       100000
#define PI              3.1415926
#define Safety_Belt_L   15.0  //手动按操作下运动极限安全距离
#define Safety_Belt_A   10.0  //手动按操作下运动极限安全距离

///////////////////////////////////////////////////////
//      轴名称  电机索引号
#define a_x		0
#define a_y		1
#define a_z		2
#define a_r		3
#define a_s		4	  //送锡轴
#define a_r1	5	  //姿态轴
#define a_r2	6	  //姿态轴
#define a_y2	7     //双平台第2个y轴
///////////////////////////////////////////////////////
//     系统 Input 配置

#define key_Xn          0       //X-
#define key_Yn          1       //Y-
#define key_Xp          2       //X+
#define key_Yp          3       //Y+
#define key_SEL_1		4       //手柄选择1：XY/ZR
#define key_SEL_2		5       //手柄选择2：R1R2 Y2
#define key_FT          6       //送锡
#define key_BT          7       //退锡

#define key_BL          8       //
#define key_FS          9       //
#define key_START       10      //
#define key_STOP        11      //启动/停止按钮
#define key_FIX_POS     12      //
#define key_NEXT		13      //
#define key_UP_POS      14      //
#define key_RESET       15      //复位

#define key_CLEAR       16      //清锡按钮（开入）
#define mac_TYPE0       17      //
#define mac_TYPE1       18      //
#define IRE1            19      //
#define IRE2            20      //
#define IRE3            21      //
#define IRE4            22      //
#define IRE5            23      //
///////////////////////////////////////////////////////
//     系统 Output 配置
#define CLEAR_O         0       //清锡输出吹嘴（开出）
#define LED_START_O     1       //启动/停止按钮灯
#define Cylinder_O      2       //
#define ORE1            3       //
#define ORE2            4       //
#define ORE3            5       //
#define ORE4            6       //
#define ORE5            7       //
///////////////////////////////////////////////////////
//     客户 Input 配置
#define IN0             0       //X-
#define IN1             1       //Y-
#define IN2             2       //X+
#define IN3             3       //Y+
#define IN4             4       //
#define IN5             5       //
#define IN6             6       //
#define IN7             7       //

#define IN8             8       //X-
#define IN9             9       //Y-
#define IN10            10      //X+
#define IN11            11      //Y+
#define IN12            12      //
#define IN13            13      //
#define IN14            14      //
#define IN15            15      //

#define IN16            16      //
#define IN17            17      //
#define IN18            18      //
#define IN19            19      //
#define IN20            20      //
#define IN21            21      //
#define IN22            22      //
#define IN23            23      //
///////////////////////////////////////////////////////
//     客户 Output 配置
#define OUT0            0       //
#define OUT1            1       //
#define OUT2            2       //
#define OUT3            3       //
#define OUT4            4       //
#define OUT5            5       //
#define OUT6            6       //
#define OUT7            7       //




/////////////////////////////////////////////////////////
//以下定义各轴手动移动模式使能
#define LINE_MOVE       0  //连续
#define POINT_MOVE      1  //点动

/////////////////////////////////////////////////////////
//以下定义各轴移动模式使能
#define DISABLE         0  //禁止
#define ENABLE          1  //使能

/////////////////////////////////////////////////////////
//以下定义各轴移动方向
#define POSITIVE        0  //正向
#define NEGATIVE        1  //负向

/////////////////////////////////////////////////////////
//以下定义焊接项目的执行方式
#define ROBOT_ALWAYS_EXEC_MODE     1       //连续执行模式
#define ROBOT_SINGLE_EXEC_MODE     2       //单条执行模式
#define ROBOT_STOP_EXEC_MODE       3       //停止执行模式

/////////////////////////////////////////////////////////
//以下定义机器人执行操作的函数结束的返回标志
#define ROBOT_POINT_WELD_END        1       //点焊结束
#define ROBOT_LINE_WELD_END         2       //拖焊结束
#define ROBOT_CLEAR_SOLDER_END      3       //清锡结束
#define ROBOT_RESET_ORG_END         4       //复位（回原点）结束
#define ROBOT_DELAY_TIME_END        5       //延时结束
#define ROBOT_GOTO_END              6       //跳转
#define ROBOT_BLANK_POINT_END       7       //移动到空点结束
#define ROBOT_ORG_END               8       //回原点结束
#define ROBOT_GO_FIX_POS_END        9       //定位结束
#define ROBOT_EMPTY_NOTHING_END     10      //空行，什么都不干
#define ROBOT_CHANGE_POSE_END       11      //调整姿态结束

///////////////////////////////////////////////////////
//以下定义机器人执行操作的边界最大值
#define ROBOT_MAX_ATSPEED           500     //移动速度最大值
///////////////////////////////////////////////////////
//一下定义最终设置依据电器信号对照表确定
/*#define key_Xp          12  //需要与电器定义一致ZZ,R2Z
#define key_Xn          13  //ZF,R2F
#define key_Yp          14  //RZ,R3Z
#define key_Yn          15  //RF,R3F
#define key_SEL_1       11
#define key_SEL_2       10
#define key_FT          2
#define key_BT          1
#define key_BL          8
#define key_FS          6
#define key_START       4
#define key_STOP        5
#define key_FIX_POS     7
#define key_NEXT        8
#define key_UP_POS      9
#define key_RESET       3
#define key_CLEAR       0

#define BLOW_DO         6	//吹气
#define Cylinder_DO     0	//气缸
#define LED_START_DO    7   //指示灯启动按键指示灯信号_DO
*/
#define MOD_Teach       1   //手动模式，示教
#define MOD_AUTO        2  //自动模式，手柄操作按键禁用
///////////////////////////////////////////////*************************///////////////
//以下定义系统设置参数的数据结构
typedef  struct
{
    int	highSpeed[8];       //轴手动高速脉冲数
    int	lowSpeed[8];        //轴手动低速脉冲数
    int	aHighSpeed[8];      //轴自动高速脉冲数
    int	aLowSpeed[8];       //轴自动低速脉冲数
    double	limitPosLen[9]; //轴行程极限长
    double	outSolderSpeed; //手动出锡速度
    double	backSolderSpeed;//手动退锡速度
} SYSTEM_PARAMS;    //系统参数
typedef  struct
{
    int maxPusle;           //伺服最大输入脉冲
    int stopStart;          //紧急停止启动

    //控制器参数
    int	Out_pulse_form[8];  //输出脉冲格式
    int	VLPPS[8];
    int	VH[8];
    int	Tacc[8];
    int	Tdec[8];
    int	Tsacc[8];
    int	Tsdec[8];

    //电机及机械参数
    int	PULSE_Per[8];
    int	maxSpeed[8];        //最高限速
    double	macStepLength[8];//机械步距

    //回原点参数
    int	VL_Home[8];
    int	VH_hm[8];
    int	TAcc[8];

    //机器类型
    int macType;
} SYSTEM_ADV_PARAMS;    //系统高级参数
typedef  struct
{
    int	maxSpeed[8][21];       //轴防冲击速度脉冲数,8轴20段
} SPEED_PARAMS;    //防冲击速度设置参数
//////////////////////////////////////////////////////////////////////////////////////
///
typedef struct
{//点焊
    //坐标参数，每个焊点不同。
    double x;//焊点坐标
    double y;
    double z;
    double r;
    double rr1; //姿态轴
    double rr2; //姿态轴
    double ZH;//Z提高到的安全高度(是绝对值，不是相对值，不同于M系列)，应该小于z
    //快速移动速度，一般由系统设置好不可改变。
    double atspeed;//快速移动速度
    //
    //一下为工艺参数，即焊接类型参数，一类焊点一组参数
    double Dlt_X;//补偿，这里Dlt_X与Dlt_Y应一致
    double Dlt_Y;//补偿
    double Dlt_Z;//补偿
    double Dlt_speed;//补偿速度
    double FT_Pre_Length;//预送锡长度
    double FT_Pre_Speed;//预送锡速度
    unsigned char FT_Pre_Modle;//预送锡方式0,1,2
    unsigned char Soldering_style;//是否抖焊标志，0不抖焊,1水平抖焊,2垂直抖焊
    double ShakeSolder_Length;//抖焊长度
    double ShakeSolder_speed;//抖焊速度
    double S1_Length;//一次送锡长度
    double S1_T_Length;//一次退锡长度
    double S_speed;//送锡速度
    double S2_Length;//二次送锡长度
    double S2_T_Length;//二次退锡长度
    double Dlt_TX;//退偿，这里TX与TY应一致。
    double Dlt_TY;//退偿
    double Dlt_TZ;//退偿
    unsigned long long T1_Delay_time;//预热时间
    unsigned long long T2_Delay_time;//一次滞留时间
    unsigned long long T3_Delay_time;//二次滞留时间，实际上没有使用。

    unsigned char Optimize;

    //超时时间计数
    unsigned long long v_Over_Time_count;//超时时间计数，根据实际情况设置，避免程序死循环。
} S_P;
typedef struct
{//拖焊
      double x0;//起点坐标
      double y0;
      double z0;
      double r0;
      double x1;//终点坐标
      double y1;
      double z1;
      double r1;

      double rr1; //姿态轴
      double rr2; //姿态轴

      double ZH;//Z提高到的安全高度(是绝对值，不是相对值，不同于M系列)，应该小于z1,z0

    //快速移动速度，一般由系统设置好不可改变。
      double atspeed;//快速移动速度

    //
    //一下为工艺参数，即焊接类型参数，一类焊点一组参数

      double Dlt_X;//补偿，这里Dlt_X与Dlt_Y应一致
      double Dlt_Y;//补偿，这里Dlt_X与Dlt_Y应一致
      double Dlt_Z;//补偿，
      double Dlt_speed;//补偿速度
      double FT_Pre_Length;//预送锡长度
      double FT_Pre_Speed;//预送锡速度
      double TH_speed;//拖焊移动速度1
      double S1_Length;//1次送锡长度
      double S1_T_Length;//1次退锡长度
      double S1_speed;//S_speed;//送锡速度1
      double S2_Length;//2次送锡长度
      double S2_T_Length;//2次退锡长度
      double S2_speed;//S_speed;//送锡速度2
      double Dlt_TX;//退偿，这里TX与TY应一致。
      double Dlt_TY;//退偿，这里TX与TY应一致。
      double Dlt_TZ;//退偿
      unsigned long long T1_Delay_time;//预热时间
      unsigned long long T2_Delay_time;//一次滞留时间
      unsigned long long T3_Delay_time;//二次滞留时间，实际上没有使用。

      unsigned char Optimize;

    //超时时间计数
      unsigned long long v_Over_Time_count;//超时时间计数，根据实际情况设置，避免程序死循环。
  } L_P;

typedef struct
{
    int commNumber;             //命令序号
    QString command;            //操作命令
    QString technics;           //工艺名称
    double pointCoordinate1[8]; //焊点坐标
    double pointCoordinate2[8]; //焊点坐标
    QString direction;          //镀锡面方向
    double Zh;                  //Z提升高度
    int optimize;               //是否优化

} PROJ_ITEM;

typedef struct
{
    //焊盘
    double HanPan_HD;       //厚度
    QString HanPan_BMCL;    //表面处理
    QString HanPan_FTXS;    //覆铜形式
    double HanPan_ZJ;       //直径
    double HanPan_KJ;       //孔径
    //焊锡丝
    double HanXiSi_ZJ;      //直径
    QString HanXiSi_ZL;     //种类
    double HanXiSi_ZHJ;     //助焊剂含量
    QString HanXiSi_FW;     //方位
    //烙铁
    double LaoTie_WD;       //温度
    QString LaoTie_ZL;      //种类

} ENV_PARA;

typedef struct
{
    QString projectName;        //项目名称
    double atspeed;             //下降速度(mm/s)
    double Zh;                  //提升高度(mm)
    double clear_H;             //清锡高度(mm)
    int clearTime;              //清锡时间(ms)
    double clearPos[8];         //清锡坐标

} CLEAR_SOLDER;

typedef struct
{
    unsigned char gpi[16];        //GPI开关量输入状态,0=断开，1=闭合
    unsigned char lmin[16];       //LMIN开关量输入状态,0=断开，1=闭合
    unsigned char org[8];         //ORG原点开关量输入状态,0=断开，1=闭合
    unsigned char gpo[16];        //GPI开关量输出读回的状态,0=断开，1=闭合
    unsigned char eStop;          //急停开关量输出读回的状态,0=断开，1=闭合
} IO_STATUS;

typedef struct
{
    unsigned char  sysIxb[24];   //给系统配置的开关量输入下标
    unsigned char  cusIxb[24];   //给客户配置的开关量输入下标
    unsigned char  sysOxb[8];    //给系统配置的开关量输出下标
    unsigned char  cusOxb[8];    //给客户配置的开关量输出下标
} IO_SET;
//////////////////////////////////////////////////////////////////////////////////////
//以下声明全局变量
extern int g_userRight;     //当前用户权限
extern QString g_userName;  //当前用户名
extern S_P g_PointWeldPara; //点焊参数
extern L_P g_LineWeldPara;  //拖焊参数
extern IO_STATUS g_ioStatus;    //IO状态
extern int g_currOperatePage;     //当前操作页，1=自动操作页，2=编辑页
extern int g_autoTempCtrlState;     //自动温控检查框的状态,0=手动温控 ; 1=自动温控

extern unsigned char  g_sysInput[24];   //给系统配置的开关量输入
extern unsigned char  g_cusInput[24];   //给客户配置的开关量输入
extern unsigned char  g_sysGpo[8];      //给系统配置的开关量输出反馈g_sysOutput[g_IOxb.sysOxb[]]
extern unsigned char  g_cusGpo[8];      //给客户配置的开关量输出反馈

extern IO_SET  g_IOxb;   //开关量配置的下标
extern unsigned char  g_MoveKeyPressed;   //movetool键盘按下，1=按下，0=未按下
extern unsigned char  g_MacType;   //机器类型：0=4轴单平台； 1=6轴在线式(Y,R反转)； 2=4轴双平台（Y1,Y2）;3=6轴单平台; 4=6轴双平台;5=3轴单平台; 6=3轴双平台
extern int g_moveMod;  //移动方式，1=点动，0=连续移动
extern int g_setpLen;  //点动步长,0=0.01mm,1=0.1mm,2=1mm,3=10mm
extern int g_lineWeldType; //拖焊类型：0=普通拖焊；1=多段拖焊首段；2=多段拖焊中段；3=多段拖焊末段；
extern short int g_NumLockSt; //小键盘的状态，0=关闭；非0=打开

#endif // GLOBAL_H
