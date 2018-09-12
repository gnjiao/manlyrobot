#ifndef MANLYROBOT_H
#define MANLYROBOT_H

#include <QObject>
#include <QThread>
#include <QMessageBox>
#include <QTime>
#include <QTimer>
#include <QCoreApplication>
#include <QEventLoop>

#include "global.h"

typedef I32 (*Fun_ML_GetHardwareType)(unsigned char *); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_GetFirmwareVersion)(unsigned char *); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_Initial_All)(); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_EnableAutoEstop)(); //使能自动急停
typedef I32 (*Fun_ML_SetHardSys_PULSE_Per)(U8 AxisNo,U32 vPULSE_Per); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetHardSys_MAX_route)(U8 AxisNo,F32 vMIN_route,F32 vMAX_route); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetHardSys_MAX_Speed)(U8 AxisNo,U32 vMAX_Speed); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetHardSys_STEP_Length)(U8 AxisNo,F64 vSTEP_Length); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetSVelocityProfile_Para)(U8 AxisNo, U32 vVLPPS, U32 vVHPPS, U16 vTaccMS, U16 vTdecMS, U16 vTsaccMS, U16 vTsdecMS); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetPulseOutputType_ALL)(U8 v_PulseOutputType); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetJogminmaxSpeed)(U8 AxisNo,U32 vJog_MIN_Speed ,U32 vJog_MAX_Speed); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetExtSpeed)(U8 AxisNo,U8 index); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetJogMode)(U8 index); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_SetHomingVelocityProfile_Param)(U8 AxisNo,U32 VL_HomePPS,U32 VH_HomePPS,U16 TAccDec_ms); //定义与DLL对应的函数指针，以备调用
typedef I32 (*Fun_ML_F_MovToORG_D)(U8 ALL_OR_PART); //定义与DLL对应的函数指针，以备调用

typedef I32 (*Fun_ML_SetINCJogValue)( I32 index ) ;//设置点动步距(长度或角度)，index:1 = 0.01,2 = 0.1 ,3 = 1,4=10
typedef I32 (*Fun_ML_Set_Axis_INCJogValue)( U8 axis,I32 index );//以单个轴为对象设定手动移动吋的点动步长index:1 = 0.01,2 = 0.1 ,3 = 1,4=10

typedef I32 (*Fun_ML_StopAll)(U8 Mode);//暂时停止八轴脉冲输出，Mode 0：减速，1：立即
typedef I32 (*Fun_ML_Stop)(U8 AxisNo, U8 Mode);//暂时停止指定轴脉冲输出AxisNo[0,7]，Mode 0：减速，1：立即
typedef I32 (*Fun_ML_SetJogMove)( U8 axis, I32 direction, I32 enable );//手动连续移动,direction:0=正向，1=负向；enable：0=禁止，1=使能
typedef I32 (*Fun_ML_SetJogINCMove)( U8 axis, I32 direction, I32 enable );//手动点动移动,direction:0=正向，1=负向；enable：0=禁止，1=使能

typedef I32 (*Fun_ML_GetPosCartesion)( U8 AxisNo, F64* pPosition_Length);//读取当前轴的当前空间坐标，长度或者角度
typedef I32 (*Fun_ML_SetMode)( U8 mod);//
typedef I32 (*Fun_ML_ExitForm)();// 关闭板卡
typedef I32 (*Fun_ML_Initial_MAXPULSE)(U32 DriverSpec_PPS);//初始化函数，设置了伺服的最大输入脉冲
typedef I32 (*Fun_ML_F_Delay_Work_Point)(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,U8 FT_Pre_Modle,U8 Soldering_style,F64 ShakeSolder_Length,F64 ShakeSolder_speed,\
                                         F64 S1_Length,F64 S1_T_Length,F64 S_speed,F64 S2_Length,F64 S2_T_Length,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style);
typedef I32 (*Fun_ML_F_Delay_Work_line)(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed,F64 FT_Pre_Length,F64 FT_Pre_Speed,F64 TH_speed,\
                                        F64 S1_Length,F64 S1_T_Length,F64 S1_speed,F64 S2_Length,F64 S2_T_Length,F64 S2_speed,F64 Dlt_TX,F64 Dlt_TY,F64 Dlt_TZ,U32 T1_Delay_time,U32 T2_Delay_time,U32 T3_Delay_time,U32 v_Over_Time_count,U8 Optimize_style);

typedef I32 (*Fun_ML_GetGPI)(U8 Num,U8 *pGPI);//Num：0`15 GPIO，开关输入、按键输入读取调用总函数
typedef I32 (*Fun_ML_GetInput)(U8 index,U8 *pGPI);//采用左右极限作为I/O输入，作为用户扩展I/O输入，index[0,15]
typedef I32 (*Fun_ML_SetOutput)(U8 Num);//开关输出调用该函数。设置、清楚、回读Num[0,15]
typedef I32 (*Fun_ML_ClearOutput)(U8 Num);//开关输出清除,Num[0,15]
typedef I32 (*Fun_ML_GetOutput)(U8 index,U8 *pGPO);//index[0,15]
typedef I32 (*Fun_ML_GetORG)(U8 AxisNo, U8 *pORG);//读取原点输入信号，AxisNo[0,7]，

typedef I32 (*Fun_ML_FF_Manual_FT)(U8 dir,U8 enable);//手动下送锡操作,dir=0,送锡;1退锡
typedef I32 (*Fun_ML_MovPosition)(U8 AxisNo,F64 vTarget_Position,F64 vSpeed_H);//单轴移动到目标绝对值坐标位置，单位为长度或角度。
typedef I32 (*Fun_ML_SetPosCartesionP)(F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H); //ptp方式移动到目标点（xyzr）,单位MM或°
typedef I32 (*Fun_ML_Mov_Line_XY)(F64 x, F64 y,F64 vSpeed_H);//在X,Y面上做直线插补移动到目标点。单位MM或°
typedef I32 (*Fun_ML_Mov_Line_XYZ)(F64 x, F64 y,F64 z,F64 vSpeed_H);//在X,Y,z做直线插补移动到目标点。
typedef I32 (*Fun_ML_DisableAutoEstop)();   //不使能自动急停

typedef I32 (*Fun_ML_MovePTP)(F64 x, F64 y, F64 z, F64 r,F64 vSpeed_H,F64 ZH) ;//按照一定的路线移动到目标点。
typedef I32 (*Fun_ML_F_Go_fixposition_Point)(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 Dlt_X,F64 Dlt_Y,F64 Dlt_Z,F64 Dlt_speed);//定位函数,以atspeed速度定位到当前点xyzr,移动时提升偏移ZH。
typedef I32 (*Fun_ML_F_Go_Empty_Point)(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,U32 Delay_time);//空点函数，可以作为一个过渡点，避免移动中的干涉，或者暂停时保留在一个安全的位置。
typedef I32 (*Fun_ML_F_Go_Clear)(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,F64 CLear_H,U32 Delay_time);//插入清洗

typedef I32 (*Fun_ML_FF_SetManual_FT_Speed)(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);  //注意:v_G_SVH_speed是长度单位,是ms不可以太大;
typedef I32 (*Fun_ML_FF_SetManual_BT_Speed)(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);  //注意:v_G_SVH_speed是长度单位,是ms不可以太大;
typedef I32 (*Fun_ML_FF_Set_TinFace)(U8 vSolderFace_style);//设置烙铁头送锡面方向。0=左侧，1为：右侧，2为前面，3为后面
typedef I32 (*Fun_ML_Set_RobotStyle)(U8 vRobotStyle);//为了DLL的维护管理方便，使得多个机型采用同一个DLL，设置机型RobotStyle=0,01,10，11:0,1,2,3
typedef I32 (*Fun_ML_FF_Stop_Tin)(U8 enable);//0,送锡;1，不送锡，只在“轨迹”功能时设置为1，其他方式全部为0
typedef I32 (*Fun_ML_FF_Set_Do_Step_style)(U8 vDo_Step_style);//设置当前执行项的类型，种类U8 Do_Step_style=0(起始项),1（中间项）,2（最后一项）
typedef I32 (*Fun_ML_GetMotionStatus)(U8 AxisNo, U8 *pStatus);//获得当前运动状态 ,0=不运动，8=回原点运动
typedef I32 (*Fun_ML_FF_Set_Act_Y2)(U8 vSet_Act_Y2);//当需要激活Y2，通过该函数设置Set_Act_Y2 = 1;DLL将按XY2执行
typedef I32 (*Fun_ML_FF_Set_WorkLine_NUM)(U8 vSet_WorkLine_NUM);//U8 Set_WorkLine_NUM=0,普通拖焊，1，多段首项，2多段中间，3多段未段
typedef I32 (*Fun_ML_GetEstop)(U8 *pEstop);//急停信号输入读取
typedef I32 (*Fun_ML_SetSVHmax_Longth)(U8 AxisNo,U8 LongthNo, U32 VHPPS);//防冲击最大速度设置
typedef I32 (*Fun_ML_FF_Set_DLL_Return)(U8 vSet_DLL_Return);//U8 Set_DLL_Return;//201150529,当上位机需要立即停止下位机的某一线程时，可以通过请求来申请。vSet_DLL_Return=1有效；
//typedef I32 (*Fun_ML_MovPosition)(U8 AxisNo,F64 vTarget_Position,F64 vSpeed_H);//单轴移动到目标绝对值坐标位置，单位为长度或角度；
typedef I32 (*Fun_ML_Change_Position)(F64 x, F64 y,F64 z,F64 r,F64 atspeed,F64 ZH,U32 Delay_time,F64 r1,F64 r2);//烙铁姿态调整功能函数
typedef I32 (*Fun_ML_Set_Change_PositionStyle)(U8 vF_Change_Position_ctrl);//20151029F_Change_Position_ctrl==0,执行，F_Change_Position_ctrl==1不执行
typedef I32 (*Fun_ML_Move_R1R2)(F64 r1,F64 r2,F64 atspeed,U32 Delay_time);
typedef I32 (*Fun_ML_FF_SetAT_FT_Speed)(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);  //注意:v_G_SVH_speed是长度单位,是ms不可以太大;
typedef I32 (*Fun_ML_FF_SetAT_BT_Speed)(U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);  //注意:v_G_SVH_speed是长度单位,是ms不可以太大;
typedef I32 (*Fun_ML_Set_HomingStyle)(U8 vHomingStyle);
typedef I32 (*Fun_ML_F_Get_TRAD_IANG)(F64 x0, F64 y0,F64 z0,F64 r0,F64 x180, F64 y180,F64 z180,F64 r180,F64 *TURNING_RADIUS,F64 *INITANGLE);  //根据某一点的R=0时对应的轴坐标,和R=180时对应的轴坐标,计算出初始角度INITANGLE（弧度）和旋转半径TURNING_RADIUS（mm）
typedef I32 (*Fun_ML_F_Get_Normalize)(F64 x, F64 y,F64 z,F64 r,F64 TURNING_RADIUS,F64 INITANGLE,F64 *Xn, F64 *Yn,F64 *Zn,F64 *Rn);  //根据某一点的R=0时对应的轴坐标,和R=180时对应的轴坐标,计算出初始角度INITANGLE（弧度）和旋转半径TURNING_RADIUS（mm）
/*功能：根据偏移前参考点1（F64 x0, F64 y0,F64 z0,F64 r0）和偏移前参考点2（F64 x1, F64 y1,F64 z1,F64 r1）。
以及刀具偏移长度TURNING_RADIUS，初始角度INITANGLE 和偏移后参考点1（F64 x01, F64 y01,F64 z01,F64 r01）和偏移后参考点2（F64 x11, F64 y11,F64 z11,F64 r11）
把F, F64 x,F64 y,F64 z,F64 r,进行坐标的转换输出F64 x2, F64 y2,F64 z2,F64 r2,，这里有一个约定，Z坐标与原值相同,
*/
typedef I32 (*Fun_ML_F_Get_x2y2z2r2)(F64 x0, F64 y0,F64 z0,F64 r0,F64 x1, F64 y1,F64 z1,F64 r1,
                                     F64 x, F64 y,F64 z,F64 r,F64 TURNING_RADIUS,F64 INITANGLE,
                                     F64 x01, F64 y01,F64 z01,F64 r01,F64 x11,F64 y11,F64 z11,F64 r11,
                                     F64 *x2, F64 *y2,F64 *z2,F64 *r2);

class ManlyRobot : public QObject
{
    Q_OBJECT
public:
    explicit ManlyRobot(QObject *parent = 0);
    virtual ~ManlyRobot();

    Fun_ML_GetHardwareType ML_GetHardwareType;
    Fun_ML_GetFirmwareVersion ML_GetFirmwareVersion;
    Fun_ML_Initial_All ML_Initial_All;
    Fun_ML_EnableAutoEstop ML_EnableAutoEstop;              //使能自动急停
    Fun_ML_SetHardSys_PULSE_Per ML_SetHardSys_PULSE_Per;    //设置各轴的细分脉冲数
    Fun_ML_SetHardSys_MAX_route ML_SetHardSys_MAX_route;    //设置各轴的行程
    Fun_ML_SetHardSys_MAX_Speed ML_SetHardSys_MAX_Speed;    //设置各轴的最大速度
    Fun_ML_SetHardSys_STEP_Length ML_SetHardSys_STEP_Length;    //设置各轴的步距
    Fun_ML_SetSVelocityProfile_Para ML_SetSVelocityProfile_Para;    //设置系统加速度曲线参数
    Fun_ML_SetPulseOutputType_ALL ML_SetPulseOutputType_ALL;    //设置控制脉冲形式
    Fun_ML_SetJogminmaxSpeed ML_SetJogminmaxSpeed;    //设置手动方式是各个轴的速度，其中X,Y,Z,R,R1,R2,E单位为脉冲数，s轴为mm。
    Fun_ML_SetExtSpeed ML_SetExtSpeed;    //设置手动速度百分比，Jog_MAX_Speed*0%~100%，对应0--10
    Fun_ML_SetJogMode ML_SetJogMode;    //设置手动移动模式 ，0连续，1为点动
    Fun_ML_SetHomingVelocityProfile_Param ML_SetHomingVelocityProfile_Param;    //0,1,2,3R,4，设置回零点速度
    Fun_ML_F_MovToORG_D ML_F_MovToORG_D;    //总体回零点操作,1=ALL，全部轴参与回原点；0=R1,R2不参与回原点

    Fun_ML_SetINCJogValue ML_SetINCJogValue;    //设置点动步距(长度或角度)，1 = 0.01mm,2 = 0.1mm ,3 = 1mm,4=10mm
    Fun_ML_Set_Axis_INCJogValue ML_Set_Axis_INCJogValue;    //以单个轴为对象设定手动移动吋的点动步长，1 = 0.01mm,2 = 0.1mm ,3 = 1mm,4=10mm

    Fun_ML_StopAll ML_StopAll;    //暂时停止八轴脉冲输出
    Fun_ML_Stop ML_Stop;    //暂时停止指定轴脉冲输出
    Fun_ML_SetJogMove ML_SetJogMove;    //手动连续移动
    Fun_ML_SetJogINCMove ML_SetJogINCMove;    //手动点动移动
    Fun_ML_ExitForm ML_ExitForm;    //关闭板卡

    Fun_ML_GetPosCartesion ML_GetPosCartesion;    //读取当前轴的当前空间坐标，长度或者角度
    Fun_ML_SetMode ML_SetMode;    //
    Fun_ML_Initial_MAXPULSE ML_Initial_MAXPULSE;    //初始化函数，设置了伺服的最大输入脉冲
    Fun_ML_F_Delay_Work_Point ML_F_Delay_Work_Point;    //点焊
    Fun_ML_F_Delay_Work_line ML_F_Delay_Work_line;    //拖焊

    Fun_ML_GetGPI ML_GetGPI;
    Fun_ML_GetInput ML_GetInput;
    Fun_ML_SetOutput ML_SetOutput;
    Fun_ML_ClearOutput ML_ClearOutput;
    Fun_ML_GetOutput ML_GetOutput;
    Fun_ML_GetORG ML_GetORG;

    Fun_ML_FF_Manual_FT ML_FF_Manual_FT;            //手动下送锡操作,dir=0,送锡;1退锡
    Fun_ML_MovPosition ML_MovPosition;              //单轴移动到目标绝对值坐标位置，单位为长度或角度。
    Fun_ML_SetPosCartesionP ML_SetPosCartesionP;    //ptp方式移动到目标点（xyzr）,单位MM或°
    Fun_ML_Mov_Line_XY ML_Mov_Line_XY;              //在X,Y面上做直线插补移动到目标点。单位MM或°
    Fun_ML_Mov_Line_XYZ ML_Mov_Line_XYZ;            //在X,Y,z做直线插补移动到目标点。
    Fun_ML_DisableAutoEstop ML_DisableAutoEstop;    //不使能自动急停

    Fun_ML_MovePTP ML_MovePTP;                      //按照一定的路线移动到目标点。
    Fun_ML_F_Go_fixposition_Point ML_F_Go_fixposition_Point;    //定位函数,以atspeed速度定位到当前点xyzr,移动时提升偏移ZH。
    Fun_ML_F_Go_Empty_Point ML_F_Go_Empty_Point;    //空点函数，可以作为一个过渡点，避免移动中的干涉，或者暂停时保留在一个安全的位置。
    Fun_ML_F_Go_Clear ML_F_Go_Clear;                //插入清洗
    Fun_ML_FF_SetManual_FT_Speed ML_FF_SetManual_FT_Speed;                //手动送锡速度
    Fun_ML_FF_SetManual_BT_Speed ML_FF_SetManual_BT_Speed;                //手动退锡速度
    Fun_ML_FF_Set_TinFace ML_FF_Set_TinFace;        //设置烙铁头送锡面方向
    Fun_ML_Set_RobotStyle ML_Set_RobotStyle;        //为了DLL的维护管理方便，使得多个机型采用同一个DLL，设置机型RobotStyle=0,01,10，11:0,1,2,3
    Fun_ML_FF_Stop_Tin ML_FF_Stop_Tin;              //0,送锡;1，不送锡，只在“轨迹”功能时设置为1，其他方式全部为0
    Fun_ML_FF_Set_Do_Step_style ML_FF_Set_Do_Step_style;  //设置当前执行项的类型，种类U8 Do_Step_style=0(起始项),1（中间项）,2（最后一项）
    Fun_ML_GetMotionStatus ML_GetMotionStatus;      //获得当前运动状态
    Fun_ML_FF_Set_Act_Y2 ML_FF_Set_Act_Y2;          //激活Y2
    Fun_ML_FF_Set_WorkLine_NUM ML_FF_Set_WorkLine_NUM;          //设置拖焊类型
    Fun_ML_GetEstop ML_GetEstop;                    //急停信号输入读取
    Fun_ML_SetSVHmax_Longth ML_SetSVHmax_Longth;    //防冲击最大速度设置
    Fun_ML_FF_Set_DLL_Return ML_FF_Set_DLL_Return;  //当上位机需要立即停止下位机的某一线程时，可以通过请求来申请
    Fun_ML_Change_Position ML_Change_Position;      //烙铁姿态调整功能函数
    Fun_ML_Set_Change_PositionStyle ML_Set_Change_PositionStyle;      //姿态锁定，Change_Position_ctrl==0,执行，F_Change_Position_ctrl==1不执行
    Fun_ML_Move_R1R2 ML_Move_R1R2;      //移动R1R2
    Fun_ML_FF_SetAT_FT_Speed ML_FF_SetAT_FT_Speed;  //自动送锡速度
    Fun_ML_FF_SetAT_BT_Speed ML_FF_SetAT_BT_Speed;  //自动退锡速度
    Fun_ML_Set_HomingStyle ML_Set_HomingStyle;      //回零方式
    Fun_ML_F_Get_TRAD_IANG ML_F_Get_TRAD_IANG;      //根据某一点的R=0时对应的轴坐标,和R=180时对应的轴坐标,计算出初始角度INITANGLE（弧度）和旋转半径TURNING_RADIUS（mm）
    Fun_ML_F_Get_Normalize ML_F_Get_Normalize;      //计算点归一化工件坐标
    Fun_ML_F_Get_x2y2z2r2 ML_F_Get_x2y2z2r2;


    U8 speedValue;
    CLEAR_SOLDER clearItem;
    F64 coordinate[8];
    U32 statusLoadDll1;
    U32 statusLoadDll2;
    U32 statusLoadDll3;
    bool isReseting;        //是否正在复位

    bool keySelPressed1;    //选择键1按下
    bool keySelPressed2;    //选择键2按下
    int keySelFlag1;        //手柄按键选择按钮的状态标志
    int keySelFlag2;        //手柄按键选择按钮的状态标志,keySelFlag1,keySelFlag2: (0,0)=x,y轴；(1,0)=z,r轴；(0,1)=y2轴；(1,1)=r1,r2轴；

    int initRobot();
    int getLoadDllStatus();
    int getMoveStatus();

signals:
    void haveAxisCoordinate(double *coordinate);//有8轴坐标数据，发信号
    void haveIoStatus();                        //有IO状态信号
    void operateFinished(int type);             //操作动作完成，发信号   type=1,点焊，2,拖焊
    void resetRobotSig();                            //复位信号
    void macTypeSig();                          //获得机器型号
    void activeError(int errCodeType,int dllErrCode);               //出错信号,errCodeType错误类型标志，以每一位来区分，dllErrCode动态库返回代码
    void resetOperate(int st);  //复位信号，1=正在复位，2=复位结束

public slots:
    int Axis_P_Move(unsigned char axis,unsigned char moveMod);  //轴手动正向移动，axis为轴的编号，moveMod为移动模式，0=连续，1=点动
    int Axis_N_Move(unsigned char axis,unsigned char moveMod);  //轴手动负向移动，axis为轴的编号，moveMod为移动模式，0=连续，1=点动
    void stopMove(unsigned char axis,unsigned char stopMod);
    void start_P_Move(unsigned char axis);      //在采用按键手柄操作时，轴正向移动
    void start_N_Move(unsigned char axis);      //在采用按键手柄操作时，轴负向移动
    int MoveToBlankPoint();                     //移动到空点
    int ChangeIronPose();                       //烙铁姿态调整功能函数

    int pointWeld();    //点焊操作S_P pointWeldPara
    int lineWeld();     //拖焊操作
    void clearSolder(); //清锡操作CLEAR_SOLDER clearItem

    void exitForm();    //退出时关闭控制器板卡的操作
    //void resetSlot(); //复位槽
    void goFixPosSlot(int weldType);//定位功能 weldType=1,点焊 ;  weldType=2,拖焊
    void resetRobot();  //复位操作
    void moveToOrg();   //机器回原点

    //void startReadCoordinate(); //开始读取各轴坐标
    //void stopReadCoordinate();  //停止读取各轴坐标
    //void startReadIoStatus();   //开始读取IO状态
    int getAxisCoordinate();    //获得轴坐标
    void getIoStatus();         //获得IO的状态,与定时器配合的槽函数
    void processLimitValue(unsigned char axis,unsigned char mod);  //轴运行到极限边界的处理
    void gpoSet(unsigned char  Num);          //开关量输出1,Num:通道号
    void gpoClear(unsigned char  Num);        //开关量输出0,Num:通道号

    void mySleep(unsigned int msec);

private:
    ////////////系统基础参数变量数组声明
    U32 vPULSE_Per[8];//各轴的每转脉冲数当前基本为6400
    U8 v_PulseOutputType[8];//脉冲类型
    F32 vMIN_route[8];//最小行程，单位MM，或度，范围根据机械结构确定
    F32 vMAX_route[8];//最大行程，单位MM，或度，范围根据机械结构确定
    U32 vMAX_Speed[8];//设定范围：0~100000，以脉冲数设置。MAX_Speed<MAX_PULSE
    F64 vSTEP_Length[8];//单位MM，或度，范围根据机械结构确定,STEP_Length=2*PI*R/PULSE_Per;=360/PULSE_Per
    U32 VLPPS[8];//S曲线运动方式的启动速度
    U32 VHPPS[8];//S曲线运动方式的平稳段速度
    U16 TaccMS[8];//S曲线运动方式的加速速度时间
    U16 TdecMS[8];//S曲线运动方式的减速速度时间
    U16 TsaccMS[8];//S曲线运动方式的加加速速度时间
    U16 TsdecMS[8];//S曲线运动方式的减减速速度时间
    U32 vJog_MIN_Speed[8];//手动速度最小值
    U32 vJog_MAX_Speed[8];//手动速度最大值

    //////////手动移动参数变量
    I32 INCJogValue_index;//手动步距索引
    U8 ExtSpeed_index;//设置手动速度百分比，Jog_MAX_Speed*0%~100%，对应0--10
    U8 JogMode_index;//手动连续或点动

    //////////各轴当前坐标值数组声明
    I32 pPositionPulse[8];//获得当前脉冲数表示的坐标值，单位是脉冲数
    F64 pPosition_Length[8];//获得当前坐标值，单位是mm
    F64 pPosition_Length_PTP[8];//记录点焊当前点坐标
    F64 pPosition_Length_L_S[8];//记录拖焊当前起点坐标
    F64 pPosition_Length_L_E[8];//记录拖焊当前终点坐标

    int moveStatus;
    int statusLoadDll;
    QTimer *coodTimer;
    QTimer *ioTimer;
    U8  vGetGPI[16];//常规输入数组，对应16路开关量输入
    double upValue[8];      //上限值,轴连续运行到该位置时要停止，变为点动模式
    double downValue[8];    //下限值,轴连续运行到该位置时要停止，变为点动模式
    bool flagLimit[8];      //下限标志
    bool flagLimit2[8];     //上限标志
    int axisStatus[8];      //轴移动的状态，0=静止，1=正在移动
    int axisPointMove[8];      //轴移动的状态，0=非点动，1=正在点动移动
    int xyKeyPressedCnt[4]; //轴移动的4个方向键按下的次数
    int resetFlag;          //复位标志，1=正在复位，0=结束或没开始

    bool isFirstReadIo;     //是否第一次读取IO状态
    bool isAutoClearSolder; //是否自动清锡
    bool isWelding;         //是否正在执行焊接操作
    bool isMove_R_Reset;    //是正在复位中否移动R轴

    int stEStop;            //急停按钮的状态,1=有效(按下)；2=无效(抬起释放)
    int isEStopReset;       //是否急停复位：1=是；0=否
};

class RobotThread : public QThread
{
    Q_OBJECT
public:
    explicit RobotThread(QObject *parent = 0);
    virtual ~RobotThread();

    ManlyRobot *robot;

    void run();

signals:
    void robotCreated();    //机器人控制器创建完毕

public slots:

private:
};

class RobotThread2 : public QThread
{
    Q_OBJECT
public:
    explicit RobotThread2(QObject *parent = 0);
    virtual ~RobotThread2();

    ManlyRobot *robot;

    void run();

signals:
    void haveAxisCoordinate(double *coordinate);//有8轴坐标数据，发信号
    //void robotCreated2();    //机器人控制器创建完毕

public slots:
    int getAxisCoordinate();    //获得轴坐标
    int getIoStatus();    //获得IO
private:
    QTimer *coodTimer;
    F64 coordinate[8];
};

class RobotThread3 : public QThread
{
    Q_OBJECT
public:
    explicit RobotThread3(QObject *parent = 0);
    virtual ~RobotThread3();

    ManlyRobot *robot;

    void run();

signals:


public slots:
    int getIoStatus();    //获得IO

private:
    QTimer *ioTimer;
};

#endif // MANLYROBOT_H
