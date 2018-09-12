#include "manlyrobot.h"
#include "qlibrary.h"
#include "myrwxml.h"
#include <math.h>

////////////////////////////////////////////class ManlyRobot //////////////////////////////////////////////////////

ManlyRobot::ManlyRobot(QObject *parent) :
    QObject(parent)
{
    moveStatus=0;
    statusLoadDll=0;
    statusLoadDll1=0;
    statusLoadDll2=0;
    statusLoadDll3=0;
    speedValue=2;
    resetFlag=0;
    stEStop=0;
    isEStopReset=0;
    isFirstReadIo=true;
    isReseting=false;
    isWelding=false;
    isMove_R_Reset=false;
    keySelFlag1=0;
    keySelFlag2=0;
    keySelPressed1=false;
    keySelPressed2=false;
    isAutoClearSolder=false;
    for(int i=0;i<8;i++)
        coordinate[i]=0.0;
    for(int i=0;i<16;i++)
    {
        g_ioStatus.gpi[i]=0;
        g_ioStatus.lmin[i]=0;
        g_ioStatus.gpo[i]=0;
        if(i<8)
            g_ioStatus.org[i]=0;
    }
    for(int i=0;i<4;i++)
        xyKeyPressedCnt[i]=0;

    for(int i=0;i<8;i++)
        flagLimit[i]=false;
    for(int i=0;i<8;i++)
        flagLimit2[i]=false;
    for(int i=0;i<8;i++)
        axisStatus[i]=0;
    for(int i=0;i<8;i++)
        axisPointMove[i]=0;

    QLibrary mylib("Robot_DDll.dll");   //声明所用到的dll文件
    //int result;
    //QString sresult;
    if (mylib.load())              //判断是否正确加载
    {
        ML_GetHardwareType=(Fun_ML_GetHardwareType)mylib.resolve("ML_GetHardwareType");
        ML_GetFirmwareVersion=(Fun_ML_GetFirmwareVersion)mylib.resolve("ML_GetFirmwareVersion");
        ML_Initial_All=(Fun_ML_Initial_All)mylib.resolve("ML_Initial_All");
        ML_EnableAutoEstop=(Fun_ML_EnableAutoEstop)mylib.resolve("ML_EnableAutoEstop");
        ML_SetHardSys_PULSE_Per=(Fun_ML_SetHardSys_PULSE_Per)mylib.resolve("ML_SetHardSys_PULSE_Per");
        ML_SetHardSys_MAX_route=(Fun_ML_SetHardSys_MAX_route)mylib.resolve("ML_SetHardSys_MAX_route");
        ML_SetHardSys_MAX_Speed=(Fun_ML_SetHardSys_MAX_Speed)mylib.resolve("ML_SetHardSys_MAX_Speed");
        ML_SetHardSys_STEP_Length=(Fun_ML_SetHardSys_STEP_Length)mylib.resolve("ML_SetHardSys_STEP_Length");
        ML_SetSVelocityProfile_Para=(Fun_ML_SetSVelocityProfile_Para)mylib.resolve("ML_SetSVelocityProfile_Para");
        ML_SetPulseOutputType_ALL=(Fun_ML_SetPulseOutputType_ALL)mylib.resolve("ML_SetPulseOutputType_ALL");
        ML_SetJogminmaxSpeed=(Fun_ML_SetJogminmaxSpeed)mylib.resolve("ML_SetJogminmaxSpeed");
        ML_SetExtSpeed=(Fun_ML_SetExtSpeed)mylib.resolve("ML_SetExtSpeed");
        ML_SetJogMode=(Fun_ML_SetJogMode)mylib.resolve("ML_SetJogMode");
        ML_SetINCJogValue=(Fun_ML_SetINCJogValue)mylib.resolve("ML_SetINCJogValue");
        ML_SetHomingVelocityProfile_Param=(Fun_ML_SetHomingVelocityProfile_Param)mylib.resolve("ML_SetHomingVelocityProfile_Param");
        ML_F_MovToORG_D=(Fun_ML_F_MovToORG_D)mylib.resolve("ML_F_MovToORG_D");
        ML_Stop=(Fun_ML_Stop)mylib.resolve("ML_Stop");
        ML_SetJogMove=(Fun_ML_SetJogMove)mylib.resolve("ML_SetJogMove");
        ML_SetJogINCMove=(Fun_ML_SetJogMove)mylib.resolve("ML_SetJogINCMove");
        ML_GetPosCartesion=(Fun_ML_GetPosCartesion)mylib.resolve("ML_GetPosCartesion");
        ML_StopAll=(Fun_ML_StopAll)mylib.resolve("ML_StopAll");
        ML_Set_Axis_INCJogValue=(Fun_ML_Set_Axis_INCJogValue)mylib.resolve("ML_Set_Axis_INCJogValue");
        ML_SetMode=(Fun_ML_SetMode)mylib.resolve("ML_SetMode");
        ML_Initial_MAXPULSE=(Fun_ML_Initial_MAXPULSE)mylib.resolve("ML_Initial_MAXPULSE");
        ML_F_Delay_Work_Point=(Fun_ML_F_Delay_Work_Point)mylib.resolve("ML_F_Delay_Work_Point");
        ML_F_Delay_Work_line=(Fun_ML_F_Delay_Work_line)mylib.resolve("ML_F_Delay_Work_line");
        ML_ExitForm=(Fun_ML_ExitForm)mylib.resolve("ML_ExitForm");
        ML_GetGPI=(Fun_ML_GetGPI)mylib.resolve("ML_GetGPI");
        ML_GetInput=(Fun_ML_GetInput)mylib.resolve("ML_GetInput");
        ML_SetOutput=(Fun_ML_SetOutput)mylib.resolve("ML_SetOutput");
        ML_ClearOutput=(Fun_ML_ClearOutput)mylib.resolve("ML_ClearOutput");
        ML_GetOutput=(Fun_ML_GetOutput)mylib.resolve("ML_GetOutput");
        ML_GetORG=(Fun_ML_GetORG)mylib.resolve("ML_GetORG");
        ML_FF_Manual_FT=(Fun_ML_FF_Manual_FT)mylib.resolve("ML_FF_Manual_FT");
        ML_MovPosition=(Fun_ML_MovPosition)mylib.resolve("ML_MovPosition");
        ML_SetPosCartesionP=(Fun_ML_SetPosCartesionP)mylib.resolve("ML_SetPosCartesionP");
        ML_Mov_Line_XY=(Fun_ML_Mov_Line_XY)mylib.resolve("ML_Mov_Line_XY");
        ML_Mov_Line_XYZ=(Fun_ML_Mov_Line_XYZ)mylib.resolve("ML_Mov_Line_XYZ");
        ML_DisableAutoEstop=(Fun_ML_DisableAutoEstop)mylib.resolve("ML_DisableAutoEstop");
        ML_MovePTP=(Fun_ML_MovePTP)mylib.resolve("ML_MovePTP");
        ML_F_Go_fixposition_Point=(Fun_ML_F_Go_fixposition_Point)mylib.resolve("ML_F_Go_fixposition_Point");
        ML_F_Go_Empty_Point=(Fun_ML_F_Go_Empty_Point)mylib.resolve("ML_F_Go_Empty_Point");
        ML_F_Go_Clear=(Fun_ML_F_Go_Clear)mylib.resolve("ML_F_Go_Clear");
        ML_FF_SetManual_FT_Speed=(Fun_ML_FF_SetManual_FT_Speed)mylib.resolve("ML_FF_SetManual_FT_Speed");
        ML_FF_SetManual_BT_Speed=(Fun_ML_FF_SetManual_BT_Speed)mylib.resolve("ML_FF_SetManual_BT_Speed");
        ML_FF_Set_TinFace=(Fun_ML_FF_Set_TinFace)mylib.resolve("ML_FF_Set_TinFace");
        ML_Set_RobotStyle=(Fun_ML_Set_RobotStyle)mylib.resolve("ML_Set_RobotStyle");
        ML_FF_Stop_Tin=(Fun_ML_FF_Stop_Tin)mylib.resolve("ML_FF_Stop_Tin");
        ML_FF_Set_Do_Step_style=(Fun_ML_FF_Set_Do_Step_style)mylib.resolve("ML_FF_Set_Do_Step_style");
        ML_GetMotionStatus=(Fun_ML_GetMotionStatus)mylib.resolve("ML_GetMotionStatus");
        ML_FF_Set_Act_Y2=(Fun_ML_FF_Set_Act_Y2)mylib.resolve("ML_FF_Set_Act_Y2");
        ML_FF_Set_WorkLine_NUM=(Fun_ML_FF_Set_WorkLine_NUM)mylib.resolve("ML_FF_Set_WorkLine_NUM");
        ML_GetEstop=(Fun_ML_GetEstop)mylib.resolve("ML_GetEstop");
        ML_SetSVHmax_Longth=(Fun_ML_SetSVHmax_Longth)mylib.resolve("ML_SetSVHmax_Longth");
        ML_FF_Set_DLL_Return=(Fun_ML_FF_Set_DLL_Return)mylib.resolve("ML_FF_Set_DLL_Return");
        ML_Change_Position=(Fun_ML_Change_Position)mylib.resolve("ML_Change_Position");
        ML_Set_Change_PositionStyle=(Fun_ML_Set_Change_PositionStyle)mylib.resolve("ML_Set_Change_PositionStyle");
        ML_Move_R1R2=(Fun_ML_Move_R1R2)mylib.resolve("ML_Move_R1R2");
        ML_FF_SetAT_FT_Speed=(Fun_ML_FF_SetAT_FT_Speed)mylib.resolve("ML_FF_SetAT_FT_Speed");
        ML_FF_SetAT_BT_Speed=(Fun_ML_FF_SetAT_BT_Speed)mylib.resolve("ML_FF_SetAT_BT_Speed");
        ML_Set_HomingStyle=(Fun_ML_Set_HomingStyle)mylib.resolve("ML_Set_HomingStyle");
        ML_F_Get_TRAD_IANG=(Fun_ML_F_Get_TRAD_IANG)mylib.resolve("ML_F_Get_TRAD_IANG");
        ML_F_Get_Normalize=(Fun_ML_F_Get_Normalize)mylib.resolve("ML_F_Get_Normalize");
        ML_F_Get_x2y2z2r2=(Fun_ML_F_Get_x2y2z2r2)mylib.resolve("ML_F_Get_x2y2z2r2");

        if (!ML_GetHardwareType)  {statusLoadDll1 |= 0x01;statusLoadDll++;}             //没有成功连接上第1个Dll函数
        if (!ML_GetFirmwareVersion)  {statusLoadDll1 |= 0x2;statusLoadDll++;}           //没有成功连接上第2个Dll函数
        if (!ML_Initial_All)  {statusLoadDll1 |= 0x4;statusLoadDll++;}                  //没有成功连接上第3个Dll函数
        if (!ML_EnableAutoEstop)  {statusLoadDll1 |= 0x08;statusLoadDll++;}             //没有成功连接上第4个Dll函数
        if (!ML_SetHardSys_PULSE_Per)  {statusLoadDll1 |=0x10;statusLoadDll++;}         //没有成功连接上第5个Dll函数
        if (!ML_SetHardSys_MAX_route)  {statusLoadDll1 |= 0x20;statusLoadDll++;}        //没有成功连接上第6个Dll函数
        if (!ML_SetHardSys_MAX_Speed)  {statusLoadDll1 |= 0x40;statusLoadDll++;}        //没有成功连接上第7个Dll函数
        if (!ML_SetHardSys_STEP_Length)  {statusLoadDll1 |= 0x80;statusLoadDll++;}      //没有成功连接上第8个Dll函数
        if (!ML_SetSVelocityProfile_Para)  {statusLoadDll1 |= 0x100;statusLoadDll++;}   //没有成功连接上第9个Dll函数
        if (!ML_SetPulseOutputType_ALL)  {statusLoadDll1 |= 0x200;statusLoadDll++;}     //没有成功连接上第10个Dll函数
        if (!ML_SetJogminmaxSpeed)  {statusLoadDll1 |= 0x400;statusLoadDll++;}          //没有成功连接上第11个Dll函数
        if (!ML_SetExtSpeed)  {statusLoadDll1 |= 0x800;statusLoadDll++;}                //没有成功连接上第12个Dll函数
        if (!ML_SetJogMode)  {statusLoadDll1 |= 0x1000;statusLoadDll++;}                //没有成功连接上第13个Dll函数
        if (!ML_SetINCJogValue)  {statusLoadDll1 |= 0x2000;statusLoadDll++;}            //没有成功连接上第14个Dll函数
        if (!ML_SetHomingVelocityProfile_Param)  {statusLoadDll1 |= 0x4000;statusLoadDll++;}          //没有成功连接上第15个Dll函数
        if (!ML_F_MovToORG_D)  {statusLoadDll1 |= 0x8000;statusLoadDll++;}                //没有成功连接上第16个Dll函数
        if (!ML_Stop)  {statusLoadDll1 |= 0x10000;statusLoadDll++;}                //没有成功连接上第17个Dll函数
        if (!ML_SetJogMove)  {statusLoadDll1 |= 0x20000;statusLoadDll++;}                //没有成功连接上第18个Dll函数
        if (!ML_SetJogINCMove)  {statusLoadDll1 |= 0x40000;statusLoadDll++;}                //没有成功连接上第19个Dll函数
        if (!ML_GetPosCartesion)  {statusLoadDll1 |= 0x80000;statusLoadDll++;}                //没有成功连接上第20个Dll函数
        if (!ML_StopAll)  {statusLoadDll1 |= 0x100000;statusLoadDll++;}                //没有成功连接上第21个Dll函数
        if (!ML_Set_Axis_INCJogValue)  {statusLoadDll1 |= 0x200000;statusLoadDll++;}                //没有成功连接上第22个Dll函数
        if (!ML_SetMode)  {statusLoadDll1 |= 0x400000;statusLoadDll++;}                //没有成功连接上第23个Dll函数
        if (!ML_Initial_MAXPULSE)  {statusLoadDll1 |= 0x800000;statusLoadDll++;}                //没有成功连接上第24个Dll函数
        if (!ML_F_Delay_Work_Point)  {statusLoadDll1 |= 0x1000000;statusLoadDll++;}                //没有成功连接上第25个Dll函数
        if (!ML_F_Delay_Work_line)  {statusLoadDll1 |= 0x2000000;statusLoadDll++;}                //没有成功连接上第26个Dll函数
        if (!ML_ExitForm)  {statusLoadDll1 |= 0x4000000;statusLoadDll++;}                //没有成功连接上第27个Dll函数
        if (!ML_GetGPI)  {statusLoadDll1 |= 0x8000000;statusLoadDll++;}                //没有成功连接上第28个Dll函数
        if (!ML_GetInput)  {statusLoadDll1 |= 0x10000000;statusLoadDll++;}                //没有成功连接上第29个Dll函数
        if (!ML_SetOutput)  {statusLoadDll1 |= 0x20000000;statusLoadDll++;}                //没有成功连接上第30个Dll函数
        if (!ML_ClearOutput)  {statusLoadDll1 |= 0x40000000;statusLoadDll++;}                //没有成功连接上第31个Dll函数
        if (!ML_GetOutput)  {statusLoadDll1 |= 0x80000000;statusLoadDll++;}                //没有成功连接上第32个Dll函数

        if (!ML_GetORG)  {statusLoadDll2 |= 0x00000001;statusLoadDll++;}                //没有成功连接上第33个Dll函数
        if (!ML_FF_Manual_FT)  {statusLoadDll2 |= 0x00000002;statusLoadDll++;}                //没有成功连接上第34个Dll函数
        if (!ML_MovPosition)  {statusLoadDll2 |= 0x00000004;statusLoadDll++;}                //没有成功连接上第35个Dll函数
        if (!ML_SetPosCartesionP)  {statusLoadDll2 |= 0x00000008;statusLoadDll++;}                //没有成功连接上第36个Dll函数
        if (!ML_Mov_Line_XY)  {statusLoadDll2 |= 0x00000010;statusLoadDll++;}                //没有成功连接上第37个Dll函数
        if (!ML_Mov_Line_XYZ)  {statusLoadDll2 |= 0x00000020;statusLoadDll++;}                //没有成功连接上第38个Dll函数
        if (!ML_DisableAutoEstop)  {statusLoadDll2 |= 0x00000040;statusLoadDll++;}                //没有成功连接上第39个Dll函数
        if (!ML_MovePTP)  {statusLoadDll2 |= 0x00000080;statusLoadDll++;}                //没有成功连接上第40个Dll函数
        if (!ML_F_Go_fixposition_Point)  {statusLoadDll2 |= 0x00000100;statusLoadDll++;}                //没有成功连接上第41个Dll函数
        if (!ML_F_Go_Empty_Point)  {statusLoadDll2 |= 0x00000200;statusLoadDll++;}                //没有成功连接上第42个Dll函数
        if (!ML_F_Go_Clear)  {statusLoadDll2 |= 0x00000400;statusLoadDll++;}                //没有成功连接上第43个Dll函数
        if (!ML_FF_SetManual_FT_Speed)  {statusLoadDll2 |= 0x00000800;statusLoadDll++;}                //没有成功连接上第44个Dll函数
        if (!ML_FF_SetManual_BT_Speed)  {statusLoadDll2 |= 0x00001000;statusLoadDll++;}                //没有成功连接上第45个Dll函数
        if (!ML_FF_Set_TinFace)  {statusLoadDll2 |= 0x00002000;statusLoadDll++;}                //没有成功连接上第46个Dll函数
        if (!ML_Set_RobotStyle)  {statusLoadDll2 |= 0x00004000;statusLoadDll++;}                //没有成功连接上第47个Dll函数
        if (!ML_FF_Stop_Tin)  {statusLoadDll2 |= 0x00008000;statusLoadDll++;}                //没有成功连接上第48个Dll函数
        if (!ML_FF_Set_Do_Step_style)  {statusLoadDll2 |= 0x00010000;statusLoadDll++;}                //没有成功连接上第49个Dll函数
        if (!ML_GetMotionStatus)  {statusLoadDll2 |= 0x00020000;statusLoadDll++;}                //没有成功连接上第50个Dll函数
        if (!ML_FF_Set_Act_Y2)  {statusLoadDll2 |= 0x00040000;statusLoadDll++;}                //没有成功连接上第51个Dll函数
        if (!ML_FF_Set_WorkLine_NUM)  {statusLoadDll2 |= 0x00080000;statusLoadDll++;}                //没有成功连接上第52个Dll函数
        if (!ML_GetEstop)  {statusLoadDll2 |= 0x00100000;statusLoadDll++;}                //没有成功连接上第53个Dll函数
        if (!ML_SetSVHmax_Longth)  {statusLoadDll2 |= 0x00200000;statusLoadDll++;}                //没有成功连接上第54个Dll函数
        if (!ML_FF_Set_DLL_Return)  {statusLoadDll2 |= 0x00400000;statusLoadDll++;}                //没有成功连接上第55个Dll函数
        if (!ML_Change_Position)  {statusLoadDll2 |= 0x00800000;statusLoadDll++;}                //没有成功连接上第56个Dll函数
        if (!ML_Move_R1R2)  {statusLoadDll2 |= 0x01000000;statusLoadDll++;}                //没有成功连接上第57个Dll函数
        if (!ML_FF_SetAT_FT_Speed)  {statusLoadDll2 |= 0x02000000;statusLoadDll++;}                //没有成功连接上第58个Dll函数
        if (!ML_FF_SetAT_BT_Speed)  {statusLoadDll2 |= 0x04000000;statusLoadDll++;}                //没有成功连接上第59个Dll函数
        if (!ML_Set_HomingStyle)  {statusLoadDll2 |= 0x08000000;statusLoadDll++;}                //没有成功连接上第60个Dll函数
        if (!ML_F_Get_TRAD_IANG)  {statusLoadDll2 |= 0x10000000;statusLoadDll++;}                //没有成功连接上第61个Dll函数
        if (!ML_F_Get_Normalize)  {statusLoadDll2 |= 0x20000000;statusLoadDll++;}                //没有成功连接上第62个Dll函数
        if (!ML_F_Get_x2y2z2r2)  {statusLoadDll2 |= 0x40000000;statusLoadDll++;}                //没有成功连接上第63个Dll函数
    }
    else
    {
        statusLoadDll=1000;
        //QMessageBox::warning(NULL,APPLICATION_NAME,"核心控制动态库RobotDll.dll装载失败!",QMessageBox::Ok);
    }
    printf("new ManlyRobot, statusLoadDll=%d ,statusLoadDll1=0x%lx, statusLoadDll2=0x%lx, statusLoadDll3=0x%lx\n",statusLoadDll,statusLoadDll1,statusLoadDll2,statusLoadDll3);
}

ManlyRobot::~ManlyRobot()
{
    delete coodTimer;
}

int ManlyRobot::getLoadDllStatus()
{
    return statusLoadDll;
}

int ManlyRobot::getMoveStatus()
{
    return moveStatus;
}

int ManlyRobot::initRobot()
{
    if(statusLoadDll1>0 || statusLoadDll2>0 || statusLoadDll3>0)
    {
        return -999999;
    }
                    //coodTimer=new QTimer(this);
                    //connect(coodTimer, SIGNAL(timeout()), this, SLOT(getAxisCoordinate()));
    //ioTimer=new QTimer(this);
    //connect(ioTimer, SIGNAL(timeout()), this, SLOT(getIoStatus()));
    //connect(this, SIGNAL(resetSig()), this, SLOT(execReset()));

    MyRWxml xml;
    SYSTEM_PARAMS systemPara;
    SYSTEM_ADV_PARAMS systemAdvPara;
    SPEED_PARAMS speedPara;
    xml.readSysPara("robot.xml",systemPara);
    xml.readSysAdvPara("robota.xml",systemAdvPara);
    xml.readSpeedPara("robotspeed.xml",speedPara);

    upValue[a_x]=systemPara.limitPosLen[0]-10 ; //290
    upValue[a_y]=systemPara.limitPosLen[1]-10; //290;
    upValue[a_z]=systemPara.limitPosLen[2]-10; //80;
    upValue[a_r]=systemPara.limitPosLen[3]-10; //170;
    upValue[a_s]=290;
    upValue[a_r1]=systemPara.limitPosLen[5]-5; //55
    upValue[a_r2]=systemPara.limitPosLen[7]-5; //55;
    upValue[a_y2]=upValue[a_y];
    downValue[a_x]=10;
    downValue[a_y]=10;
    downValue[a_z]=10;
    downValue[a_r]=systemPara.limitPosLen[4]+10; //-170;
    downValue[a_s]=0;
    downValue[a_r1]=systemPara.limitPosLen[6]+5; //-55;
    downValue[a_r2]=systemPara.limitPosLen[8]+5; //-55;
    downValue[a_y2]=10;

    int i=0;
    unsigned char v_pHW_Type = 20;
    unsigned char v_pFW_Ver = 20;
    I32 state_re = 0;
    state_re = ML_GetHardwareType(&v_pHW_Type);
    state_re = ML_GetFirmwareVersion(&v_pFW_Ver);
    //state_re = ML_SetMode(5);
    printf("ManlyRobot::initRobot(), hardware type=%u, software ver=%u\n",v_pHW_Type,v_pFW_Ver);
////////////////////设置急停功能有效//////////////////////////////////
                                //state_re = ML_Initial_All();//
    state_re =ML_Initial_MAXPULSE(systemAdvPara.maxPusle);
    if(systemAdvPara.stopStart==1)
        ML_EnableAutoEstop();//使能紧急停止,注意

    int typ=g_MacType;
    if(g_MacType==3) typ=0;
    if(g_MacType==4 || g_MacType==6) typ=2;
    ML_Set_RobotStyle(typ);   //机器类型：macType：0=台式单平台； 1=在线式(Y,R反转)； 2=台式双平台（Y1,Y2）

////////////////////进行系统参数的初始赋值///////////////////////////
    ExtSpeed_index=2;
    for(i=0;i<8;i++)
    {
        vJog_MIN_Speed[i] = systemPara.lowSpeed[i];   //界面系统设置的"手动高速\低速"
        vJog_MAX_Speed[i] = systemPara.highSpeed[i];	//界面系统设置的"手动高速\低速"
        vPULSE_Per[i] = systemAdvPara.PULSE_Per[i];   //电机及机械参数,6400,200,1600,3200,12800
        vMAX_Speed[i] = systemAdvPara.maxSpeed[i];//电机及机械参数的"最高限速"
        vSTEP_Length[i] = systemAdvPara.macStepLength[i];//电机及机械参数的"机械步距"

        VLPPS[i] = systemAdvPara.VLPPS[i];
        VHPPS[i] = systemAdvPara.VH[i];
        TaccMS[i] = systemAdvPara.Tacc[i];
        TdecMS[i] = systemAdvPara.Tdec[i];
        TsaccMS[i] = systemAdvPara.Tsacc[i];
        TsdecMS[i] = systemAdvPara.Tsdec[i];
        pPositionPulse[i] = 100;
    }
    vMIN_route[0] = 0;//xyz最小行程0,
    vMIN_route[1] = 0;//xyz最小行程0,
    vMIN_route[2] = 0;//xyz最小行程0,
    vMIN_route[3] = systemPara.limitPosLen[4];//r最小行程
    vMIN_route[5] = systemPara.limitPosLen[6];//r1最小行程
    vMIN_route[6] = systemPara.limitPosLen[8];//r2最小行程
    vMIN_route[7] = 0;//y2最小行程0,
    vMIN_route[4] = -1000;//s最小行程

    vMAX_route[0] = systemPara.limitPosLen[0];//x最大行程
    vMAX_route[1] = systemPara.limitPosLen[1];//y最大行程
    vMAX_route[2] = systemPara.limitPosLen[2];//z最大行程
    vMAX_route[3] = systemPara.limitPosLen[3];//r最大行程
    vMAX_route[5] = systemPara.limitPosLen[5];//r1最大行程
    vMAX_route[6] = systemPara.limitPosLen[7];//r2最大行程
    vMAX_route[7] = systemPara.limitPosLen[1];//y2最大行程
    vMAX_route[4] = 1000;//s最大行程


////////////////////////////////////////////////////////////////
    //a = sprintf(ErrMsg,"ML_Initial_All = %d",state_re);
    //MessageBox(ErrMsg,"sys_init",1);//



//////////////////////////////设置系统参数//////////////////////////////////
    for(i=0;i<8;i++)
    {
        ML_SetHardSys_PULSE_Per(i,vPULSE_Per[i]);//设置各轴的细分脉冲数
        ML_SetHardSys_MAX_route(i,vMIN_route[i],vMAX_route[i]);//设置各轴的行程
        ML_SetHardSys_MAX_Speed(i,vMAX_Speed[i]);//设置各轴的最大速度
        ML_SetHardSys_STEP_Length(i,vSTEP_Length[i]);//电机及机械参数的"机械步距"
        state_re = ML_SetSVelocityProfile_Para(i, VLPPS[i], VHPPS[i],TaccMS[i],TdecMS[i],TsaccMS[i],TsdecMS[i]);//设置系统加速度曲线参数。
        //m_Position_Axis = state_re;//state_re;
        //ML_SetPulseOutputType_Axis(i,systemAdvPara.Out_pulse_form[i]);//设置各轴不同的脉冲类型/////////////////////////
    }

    ML_SetPulseOutputType_ALL(0);//控制器参数:输出脉冲格式: Pulsel/Dir(0),CW/CCW(1),AB-phase(2)。

    //a = sprintf(ErrMsg,"vPULSE_Per,PulseOutputType,MAX_route,MAX_Speed,vSTEP_Length,SVelocityProfile_Para",1);
    //MessageBox(ErrMsg,"ML_Set_sys_harh",1);//

/////////////////////////////设置手动操作的参数///////////////////////////////////////////////////

    for(i=0;i<8;i++)
    {
        state_re = ML_SetJogminmaxSpeed ( i,vJog_MIN_Speed[i] ,vJog_MAX_Speed[i]) ;//vJog_MIN_Speed[i] ,vJog_MAX_Speed[i]设置手动方式是各个轴的速度，其中X,Y,Z,R,R1,R2,E单位为脉冲数，s轴为mm。
        state_re = ML_SetExtSpeed ( i, ExtSpeed_index ) ;//设置手动速度百分比，Jog_MAX_Speed*0%~100%，对应0--10
    }

    //a = sprintf(ErrMsg,"OnSetJOGPra success",1);
    //MessageBox(ErrMsg,"OnSetJOGPra",1);//

    JogMode_index =1;//点动模式。
    ML_SetJogMode ( JogMode_index );//设置手动移动模式 ，0连续，1为点动
    ML_SetINCJogValue ( INCJogValue_index ) ;//movetool设置点动步距(长度或角度)，1 = 0.01,2 = 0.1 ,3 = 1,4=10

    //手动送锡退锡  (U32 v_G_SVL,F64 v_G_SVH_speed,U16 v_G_STAcc,U16 v_G_STDec,U16 v_G_STsAcc ,U16 v_G_STsDec);
    ML_FF_SetManual_FT_Speed(systemAdvPara.VLPPS[4],systemPara.outSolderSpeed,systemAdvPara.Tacc[4],systemAdvPara.Tdec[4],systemAdvPara.Tsacc[4],systemAdvPara.Tsdec[4]);
    ML_FF_SetManual_BT_Speed(systemAdvPara.VLPPS[4],systemPara.backSolderSpeed,systemAdvPara.Tacc[4],systemAdvPara.Tdec[4],systemAdvPara.Tsacc[4],systemAdvPara.Tsdec[4]);
    ML_FF_SetAT_FT_Speed(systemAdvPara.VLPPS[4],systemPara.outSolderSpeed,systemAdvPara.Tacc[4],systemAdvPara.Tdec[4],systemAdvPara.Tsacc[4],systemAdvPara.Tsdec[4]);
    ML_FF_SetAT_BT_Speed(systemAdvPara.VLPPS[4],systemPara.backSolderSpeed,systemAdvPara.Tacc[4],systemAdvPara.Tdec[4],systemAdvPara.Tsacc[4],systemAdvPara.Tsdec[4]);

    ML_FF_Stop_Tin(0);      //正常送锡

/////////////////////////////设置防冲击速度///////////////////////////////////////////////////

    for(i=0;i<8;i++)
    {
        for(int j=0;j<21;j++)
        {
            ML_SetSVHmax_Longth(i,j,speedPara.maxSpeed[i][j]);
        }
    }

////////////////////////////H回原点/////////////////////////////////////////////////////

    for(i=0;i<8;i++)
    {
        state_re =ML_SetHomingVelocityProfile_Param(i,systemAdvPara.VL_Home[i],systemAdvPara.VH_hm[i],systemAdvPara.TAcc[i]);//参数:(轴号,VL_Home,VH_hm,TAcc),，设置回零点速度
    }

    int timeout=0;
    unsigned char st_Z=1;
    do
    {
        Axis_N_Move(a_z,LINE_MOVE);         //如果z轴原点信号有效，提升z轴
        ML_GetORG(a_z,&st_Z);    //读z轴原点的状态
        QThread::msleep(5);    //延时5ms
        timeout++;
    }
    while((st_Z==0) && (timeout<12000));
    ML_Stop(a_z, 1);    //停止Z轴移动，第2个参数，0=减速，1=立即停止
    QThread::msleep(5);    //延时5ms
    if(timeout>=12000)
    {
        emit activeError(0x4000,-6000);                //r轴原点信号出错
        return -5000;
    }
    ML_GetORG(a_r,&g_ioStatus.org[a_r]);    //读r轴原点的状态
    if(g_ioStatus.org[a_r]==1)
        Axis_P_Move(a_r,LINE_MOVE);         //如果r轴原点信号有效，正转
    timeout=0;
    while(g_ioStatus.org[a_r] && (timeout<12000)) //读原点信号，直到无效
    {
        Axis_P_Move(a_r,LINE_MOVE);         //如果r轴原点信号有效，正转
        QThread::msleep(5);    //延时5ms
        ML_GetORG(a_r,&g_ioStatus.org[a_r]);
        timeout++;
    }
    ML_Stop(a_r, 0);    //停止r轴移动，第2个参数，0=减速，1=立即停止
    if(timeout>=12000)
    {
        emit activeError(0x4000,-7000);                //r轴原点信号出错
        return -5000;
    }
    timeout=0;
    QThread::msleep(5);    //延时5ms
    //ML_Set_HomingStyle(0);
    if(g_MacType==3 || g_MacType==1 || g_MacType==4)    //
    {
        state_re=ML_F_MovToORG_D(1);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    }
    else
    {
        state_re=ML_F_MovToORG_D(0);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    }

    if(state_re != 0)
    {
        emit activeError(0x1000,state_re);                //函数执行出错
    }

    resetFlag=0;
    stEStop=0;
    isEStopReset=0;
    isFirstReadIo=true;
    isReseting=false;
    isWelding=false;

    return state_re;
}

/*
void ManlyRobot::startReadCoordinate()  //开始读取各轴坐标
{
    if(speedValue<5)
        coodTimer->start(100);
    else if(speedValue<8)
        coodTimer->start(50);
    else
        coodTimer->start(20);
}

void ManlyRobot::stopReadCoordinate()  //停止读取各轴坐标
{
    coodTimer->stop();
}
*/
int ManlyRobot::getAxisCoordinate()     //读取各轴的坐标
{

    int st=0;
    for(int i=0;i<8;i++)
    {
        st=ML_GetPosCartesion(i,&coordinate[i]);
        if(st !=0 ) break;
    }

    if(st==0)
        emit haveAxisCoordinate(coordinate);

    if(isReseting) return st;    //如果正在复位，不进行极限判断

    for(int i=0;i<8;i++)
    {
        if(!flagLimit[i])
        {
            if(coordinate[i]<downValue[i])
            {
                ML_Stop(i, 0);
                ML_SetJogMode(POINT_MOVE);
                ML_SetJogMove ( i, NEGATIVE, DISABLE );
                flagLimit[i]=true;
            }
        }
        if(!flagLimit2[i])
        {
            if(coordinate[i]>upValue[i])
            {
                ML_Stop(i, 0);
                ML_SetJogMode(POINT_MOVE);
                ML_SetJogMove ( i, POSITIVE, DISABLE );
                flagLimit2[i]=true;
            }
        }
    }

    return st;
}
/*
void ManlyRobot::startReadIoStatus()  //开始读取IO状态
{
    //ioTimer->start(30);
}
*/
void ManlyRobot::getIoStatus()      //读取IO状态
{
    if(isReseting) return ;    //如果正在复位，不读取IO状态
    for(int i=0;i<16;i++)
    {
        ML_GetGPI(i,&g_ioStatus.gpi[i]);
        if(g_ioStatus.gpi[i])
            g_ioStatus.gpi[i]=0;
        else
            g_ioStatus.gpi[i]=1;
        ML_GetInput(i,&g_ioStatus.lmin[i]);
        ML_GetOutput(i,&g_ioStatus.gpo[i]);
        if(i<8)
            ML_GetORG(i,&g_ioStatus.org[i]);
    }
    emit haveIoStatus();   //触发有IO状态信号

    if(isEStopReset==1) return; //已经急停复位，返回
    unsigned char eStop=0;
    ML_GetEstop(&eStop);    //急停信号输入读取
    g_ioStatus.eStop=eStop;
    if((eStop == 0) && (stEStop == 0))  // 急停按钮按下，信号=0
    {
        stEStop++;  //stEStop=1,急停按钮按下
    }
    else  if((eStop == 1) && (stEStop == 1))  // 急停按钮抬起释放，信号=1
    {
        stEStop++;  //stEStop=2,急停按钮抬起释放
    }
    else  if((eStop == 1) && (stEStop == 2))
    {
        /*if(isWelding)
        {
           ML_FF_Set_DLL_Return(1);    //立即退出任何焊接动作
           QThread::msleep(50);
           ML_FF_Set_DLL_Return(0);
        }*/
        if(!isReseting)
        {
            stEStop=0;
            resetFlag=1;
            isEStopReset=1;
            /*int timeout=0;
            unsigned char st_Z=1;
            do
            {
                Axis_N_Move(a_z,LINE_MOVE);         //如果z轴原点信号有效，提升z轴
                QThread::msleep(5);    //延时5ms
                ML_GetORG(a_z,&st_Z);    //读z轴原点的状态
                timeout++;
            }
            while((st_Z==0) && (timeout<4000));
            ML_Stop(a_z, 1);    //停止Z轴移动，第2个参数，0=减速，1=立即停止
            QThread::msleep(5);*/
            emit resetRobotSig();
        }
        return;
    }

    for(int i=0;i<24;i++)
    {
        //读取系统所用IN
        if(g_IOxb.sysIxb[i]<16)
        {
            g_sysInput[i]=g_ioStatus.gpi[g_IOxb.sysIxb[i]];
        }
        else if(g_IOxb.sysIxb[i]<32)
        {
            g_sysInput[i]=g_ioStatus.lmin[g_IOxb.sysIxb[i]-16];
        }
        //读取客户所用IN
        if(g_IOxb.cusIxb[i]<16)
        {
            g_cusInput[i]=g_ioStatus.gpi[g_IOxb.cusIxb[i]];
        }
        else if(g_IOxb.cusIxb[i]<32)
        {
            g_cusInput[i]=g_ioStatus.lmin[g_IOxb.cusIxb[i]-16];
        }
        if(i<8)
        {
            //读取系统所用OUT反馈
            if(g_IOxb.sysOxb[i]<8)
            {
                g_sysGpo[i]=g_ioStatus.gpo[g_IOxb.sysOxb[i]];
            }
            //读取客户所用OUT反馈
            else if(g_IOxb.sysOxb[i]<16)
            {
                g_cusGpo[i]=g_ioStatus.gpo[g_IOxb.cusOxb[i]];
            }
        }
    }

    if(isFirstReadIo)       //如果是第一次读取IO
    {
    }


    if(g_sysInput[key_RESET]==1)    //复位
    {
        if(resetFlag==1)
            return;
        /*if(isWelding)
        {
            ML_FF_Set_DLL_Return(1);    //立即退出任何焊接动作
            QThread::msleep(50);
            ML_FF_Set_DLL_Return(0);
        }*/
        if(!isReseting)
        {
            //emit resetOperate(1);   //准备复位
            //QThread::msleep(5);
            resetFlag=1;
            emit resetRobotSig();
        }
        return;
    }

    if(isWelding) return;   //正在执行焊接操作,退出

    if((g_sysInput[key_FT]==1) && (g_sysInput[key_BT]==0))       //送锡
        ML_FF_Manual_FT(0,1);       //手动下送锡操作,第一个参数dir=0,送锡,1退锡； 第二个参数1=有效,0=无效
    else if((g_sysInput[key_FT]==0) && (g_sysInput[key_BT]==1)) //退锡
        ML_FF_Manual_FT(1,1);       //手动下送锡操作,第一个参数dir=0,送锡,1退锡； 第二个参数1=有效,0=无效
    else
        ML_FF_Manual_FT(0,0);

    if(!isAutoClearSolder)
    {
        if((g_sysInput[key_CLEAR]==1))       //清锡
            ML_SetOutput(g_IOxb.sysOxb[CLEAR_O]);    //清锡输出
        else
            ML_ClearOutput(g_IOxb.sysOxb[CLEAR_O]);    //清锡输出停止
    }

    if(g_MoveKeyPressed==1) return;     //如果用屏幕的movetool键盘，就不进行手柄按键操作

    if(g_sysInput[key_Xn]==1)
        xyKeyPressedCnt[0]++;
    else
        xyKeyPressedCnt[0]=0;
    if(g_sysInput[key_Yn]==1)
        xyKeyPressedCnt[1]++;
    else
        xyKeyPressedCnt[1]=0;
    if(g_sysInput[key_Xp]==1)
        xyKeyPressedCnt[2]++;
    else
        xyKeyPressedCnt[2]=0;
    if(g_sysInput[key_Yp]==1)
        xyKeyPressedCnt[3]++;
    else
        xyKeyPressedCnt[3]=0;

    if(g_userRight<USER_RIGHT_DESIGNER) return; //权限不足,不能进行movetool操作

    unsigned char status=0;
    for(int i=0;i<8;i++)
    {
        ML_GetMotionStatus(i,&status);
        if(status==0)
            axisPointMove[i]=0;
        if(axisPointMove[i]==1) //正在点动移动
            return;
    }

    if(g_sysInput[key_SEL_1]==1)    //手柄按钮选择键1按下
    {
        keySelPressed1=true;
    }
    else
    {
        if(keySelPressed1)
        {
            if(keySelFlag1==0)
            {
                keySelFlag1=1;
                gpoSet(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
            }
            else
            {
                keySelFlag1=0;
                gpoClear(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
            }
            keySelPressed1=false;
        }
    }

    if(g_sysInput[key_SEL_2]==1)    //手柄按钮选择键2按下
    {
        keySelPressed2=true;
    }
    else
    {
        if(keySelPressed2)
        {
            if(keySelFlag2==0)
            {
                keySelFlag2=1;
                gpoSet(g_IOxb.sysOxb[ORE2]);    //点亮选择键1的灯
            }
            else
            {
                keySelFlag2=0;
                gpoClear(g_IOxb.sysOxb[ORE2]);  //熄灭选择键1的灯
            }
            keySelPressed2=false;
        }
    }

    if((keySelFlag1==0) && (keySelFlag2==0))    //选择x,y轴移动
    {
        /*if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Xp]==0))
            stopMove(a_x,0);
        if((g_sysInput[key_Yn]==0) && (g_sysInput[key_Yp]==0))
            stopMove(a_y,0);*/
        if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择x轴负向移动
        {
            stopMove(a_y,0);
            start_N_Move(a_x);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择y轴负向移动
        {
            stopMove(a_x,0);
            start_N_Move(a_y);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==0))    //选择x轴正向移动
        {
            stopMove(a_y,0);
            start_P_Move(a_x);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==1))    //选择y轴正向移动
        {
            stopMove(a_x,0);
            start_P_Move(a_y);
        }
        else if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择x,y轴同时负向移动
        {
            start_N_Move(a_x);
            start_N_Move(a_y);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==1))    //选择x,y轴同时正向移动
        {
            start_P_Move(a_x);
            start_P_Move(a_y);
        }
        else if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==1))    //选择x,y轴同时负向移动
        {
            start_N_Move(a_x);
            start_P_Move(a_y);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==0))    //选择x,y轴同时正向移动
        {
            start_P_Move(a_x);
            start_N_Move(a_y);
        }
        else
        {
            if(isMove_R_Reset) return;
            stopMove(a_x,0);
            stopMove(a_y,0);
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==0))    //选择r,z轴移动
    {
        if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择r轴负向移动
        {
            stopMove(a_z,0);
            start_N_Move(a_r);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==1))    //选择z轴负向移动
        {
            stopMove(a_r,0);
            start_N_Move(a_z);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==0))    //选择r轴正向移动
        {
            stopMove(a_z,0);
            start_P_Move(a_r);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择z轴正向移动
        {
            stopMove(a_r,0);
            start_P_Move(a_z);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==0))    //选择r,z轴正向移动
        {
            //start_N_Move(a_r);
            start_P_Move(a_r);
            start_P_Move(a_z);
        }
        else if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==1))    //选择r,z轴负向移动
        {
            //start_N_Move(a_r);
            start_N_Move(a_r);
            start_N_Move(a_z);
        }
        else if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择r负向移动,z轴正向移动
        {
            //start_N_Move(a_r);
            start_N_Move(a_r);
            start_P_Move(a_z);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==1))    //选择r正向移动,z轴负向移动
        {
            //start_N_Move(a_r);
            start_P_Move(a_r);
            start_N_Move(a_z);
        }
        else
        {
            if(isMove_R_Reset) return;
            stopMove(a_r,0);
            stopMove(a_z,0);
        }
    }
    else if((keySelFlag1==0) && (keySelFlag2==1))    //选择x,y2轴移动
    {
        //if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Xp]==0))
        //    stopMove(a_r,0);
        //if((g_sysInput[key_Yn]==0) && (g_sysInput[key_Yp]==0))
        //    stopMove(a_y2,0);
        if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择y2轴负向移动
        {
            //stopMove(a_y2,0);
            start_N_Move(a_y2);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==1))    //选择y2轴正向移动
        {
            //stopMove(a_y2,0);
            start_P_Move(a_y2);
        }
        else if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择x轴负向移动
        {
            stopMove(a_y2,0);
            start_N_Move(a_x);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==0))    //选择x轴正向移动
        {
            stopMove(a_y2,0);
            start_P_Move(a_x);
        }
        else
        {
            if(isMove_R_Reset) return;
            stopMove(a_x,0);
            stopMove(a_y2,0);
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==1))    //选择r1,r2轴移动
    {
        if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Xp]==0))
            stopMove(a_r1,0);
        if((g_sysInput[key_Yn]==0) && (g_sysInput[key_Yp]==0))
            stopMove(a_r2,0);
        if((g_sysInput[key_Xn]==1) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择r1轴负向移动
        {
            stopMove(a_r2,0);
            start_N_Move(a_r1);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==1) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==0))    //选择r2轴负向移动
        {
            stopMove(a_r1,0);
            start_N_Move(a_r2);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==1) && (g_sysInput[key_Yp]==0))    //选择r1轴正向移动
        {
            stopMove(a_r2,0);
            start_P_Move(a_r1);
        }
        else if((g_sysInput[key_Xn]==0) && (g_sysInput[key_Yn]==0) && (g_sysInput[key_Xp]==0) && (g_sysInput[key_Yp]==1))    //选择r2轴正向移动
        {
            stopMove(a_r1,0);
            start_P_Move(a_r2);
        }
        else
        {
            if(isMove_R_Reset) return;
            stopMove(a_r1,0);
            stopMove(a_r2,0);
        }
    }
}

void ManlyRobot::gpoSet(unsigned char  Num)          //开关量输出1,Num:通道号
{
    ML_SetOutput(Num);
}

void ManlyRobot::gpoClear(unsigned char  Num)        //开关量输出0,Num:通道号
{
    ML_ClearOutput(Num);
}
/*
void ManlyRobot::resetSlot()                            //复位槽
{
    ML_F_MovToORG_D(0);
    resetFlag=0;
}
*/
int ManlyRobot::Axis_P_Move(U8 axis,U8 moveMod)  //轴正向移动，axis为轴的编号，moveMod为移动模式，0=连续，1=点动
{
    if(moveMod==POINT_MOVE) //移动模式 ，点动
    {
        moveStatus=ML_SetJogMode(POINT_MOVE);
        if(moveStatus) return moveStatus;
        moveStatus=ML_SetJogMove ( axis, POSITIVE, DISABLE );//禁止手动连续移动  ,POSITIVE=正向移动
        if(moveStatus) return moveStatus;
        axisPointMove[axis]=1;     //标记轴正在点动移动
        moveStatus=ML_SetJogINCMove ( axis, POSITIVE, ENABLE );//允许手动点动移动
    }
    else //移动模式 ，0连续
    {
        moveStatus=0;
        moveStatus=ML_Stop(axis, 0);    //停止轴移动，第2个参数，0=减速，1=立即停止
        if(moveStatus) return moveStatus;

        moveStatus=ML_SetJogMode(LINE_MOVE);
        if(moveStatus) return moveStatus;
        moveStatus=ML_SetJogINCMove ( axis, POSITIVE, DISABLE );//禁止手动点动移动
        if(moveStatus) return moveStatus;
        moveStatus=ML_SetJogMove ( axis, POSITIVE, ENABLE );//允许手动连续移动
    }
    return moveStatus;
}

int ManlyRobot::Axis_N_Move(U8 axis,U8 moveMod)  //轴负向移动，axis为轴的编号，moveMod为移动模式，0=连续，1=点动
{
    if(moveMod==POINT_MOVE) //移动模式 ，0连续，1为点动(LINE_MOVE=0,POINT_MOVE=1)
    {
        moveStatus=ML_SetJogMode(POINT_MOVE);
        if(moveStatus) return moveStatus;
        moveStatus=ML_SetJogMove ( axis, NEGATIVE, DISABLE );//禁止手动连续移动  ,NEGATIVE=负向移动
        if(moveStatus) return moveStatus;
        axisPointMove[axis]=1;     //标记轴正在点动移动
        moveStatus=ML_SetJogINCMove ( axis, NEGATIVE, ENABLE );//允许手动点动移动
    }
    else
    {
        moveStatus=0;
        moveStatus=ML_Stop(axis, 0);    //停止轴移动，第2个参数，0=减速，1=立即停止
        if(moveStatus) return moveStatus;

        moveStatus=ML_SetJogMode(LINE_MOVE);
        if(moveStatus) return moveStatus;
        moveStatus=ML_SetJogINCMove ( axis, NEGATIVE, DISABLE );//禁止手动点动移动
        if(moveStatus) return moveStatus;
        moveStatus=ML_SetJogMove ( axis, NEGATIVE, ENABLE );//允许手动连续移动
    }
    return moveStatus;
}

void ManlyRobot::stopMove(unsigned char axis,unsigned char stopMod)
{
    ML_Stop(axis, stopMod);
    axisStatus[axis]=0;     //标记轴停止移动
}

int ManlyRobot::pointWeld()  //点焊操作S_P &pointWeldPara
{
    double x=g_PointWeldPara.x;
    double y=g_PointWeldPara.y;
    double z=g_PointWeldPara.z;
    double r=g_PointWeldPara.r;
    double ZH=g_PointWeldPara.ZH;
    double atspeed=g_PointWeldPara.atspeed;
    double Dlt_X=g_PointWeldPara.Dlt_X ;
    double Dlt_Y=g_PointWeldPara.Dlt_Y ;
    double Dlt_Z=g_PointWeldPara.Dlt_Z ;
    double Dlt_speed=g_PointWeldPara.Dlt_speed ;
    double FT_Pre_Length=g_PointWeldPara.FT_Pre_Length ;
    double FT_Pre_Speed=g_PointWeldPara.FT_Pre_Speed ;
    unsigned char FT_Pre_Modle=g_PointWeldPara.FT_Pre_Modle ;
    unsigned char Soldering_style=g_PointWeldPara.Soldering_style ;
    double ShakeSolder_Length=g_PointWeldPara.ShakeSolder_Length ;
    double ShakeSolder_speed=g_PointWeldPara.ShakeSolder_speed ;
    double S1_Length=g_PointWeldPara.S1_Length ;
    double S1_T_Length=g_PointWeldPara.S1_T_Length ;
    double S_speed=g_PointWeldPara.S_speed ;
    double S2_Length=g_PointWeldPara.S2_Length ;
    double S2_T_Length=g_PointWeldPara.S2_T_Length ;
    double Dlt_TX=g_PointWeldPara.Dlt_TX ;
    double Dlt_TY=g_PointWeldPara.Dlt_TY ;
    double Dlt_TZ=g_PointWeldPara.Dlt_TZ ;
    unsigned long T1_Delay_time=g_PointWeldPara.T1_Delay_time ;
    unsigned long T2_Delay_time=g_PointWeldPara.T2_Delay_time ;
    unsigned long T3_Delay_time=g_PointWeldPara.T3_Delay_time ;
    unsigned long v_Over_Time_count=g_PointWeldPara.v_Over_Time_count ;
    unsigned char Optimize=g_PointWeldPara.Optimize ;

    double rr1=g_PointWeldPara.rr1;
    double rr2=g_PointWeldPara.rr2;

    double xx=x+Dlt_X*cos(PI*r/180);
    double yy=y+Dlt_Y*sin(PI*r/180);
    double zz=z-Dlt_Z;
    double zz_ZH=z-ZH;

    int errInt=0;
    if(xx<0 || xx>vMAX_route[0])
    {
        errInt |= 0x01;
    }
    if(yy<0 || yy>vMAX_route[1])
    {
        errInt |= 0x02;
    }

    if(zz<0 || zz>vMAX_route[2])
    {
        errInt |= 0x04;
    }
    else if(zz_ZH<0 || zz_ZH>vMAX_route[2])
    {
        errInt |= 0x08;
    }

    if(r<vMIN_route[3] || r>vMAX_route[3])
    {
        errInt |= 0x10;
    }
    if(atspeed<0 || atspeed>ROBOT_MAX_ATSPEED)
    {
        errInt |= 0x20;
    }
    if(errInt>0)
    {
        emit activeError(errInt,0);
        return errInt;
    }

    isWelding=true; //禁止使用手柄，以免影响各轴移动
    //调整姿态
    if(g_MacType==3 || g_MacType==4 || g_MacType==1)
    {
        ML_MovPosition(a_z,ZH,atspeed);  //将Z轴提升到ZH的位置
        ML_Move_R1R2(rr1,rr2,atspeed,100);
    }

    if((g_MacType==2 || g_MacType==4 ||g_MacType==6) && (x>vMAX_route[0]/2)) //
        ML_FF_Set_Act_Y2(1);    //双平台，走Y2
    else
        ML_FF_Set_Act_Y2(0);    //走Y
    int st=0;
    st=ML_F_Delay_Work_Point(x,   y,  z,  r,  atspeed,  ZH,  Dlt_X,  Dlt_Y,  Dlt_Z,  Dlt_speed,  FT_Pre_Length,  FT_Pre_Speed, FT_Pre_Modle, Soldering_style,  ShakeSolder_Length,  ShakeSolder_speed,\
                          S1_Length,  S1_T_Length,  S_speed,  S2_Length,  S2_T_Length,  Dlt_TX,  Dlt_TY,  Dlt_TZ, T1_Delay_time, T2_Delay_time, T3_Delay_time, v_Over_Time_count,Optimize);
    isWelding=false; //可以使用手柄
    if(st==0)
    {
        emit operateFinished(ROBOT_POINT_WELD_END);  //完成操作
        return 0;
    }
    else
    {
        errInt = 0x100;
        emit activeError(errInt,st);                //函数执行出错
        return errInt;
    }
/*
    mySleep(500);
    emit operateFinished(ROBOT_POINT_WELD_END);  //完成操作
    return 0;*/
}

int ManlyRobot::MoveToBlankPoint()  //移动到空点
{
    double x=g_PointWeldPara.x;
    double y=g_PointWeldPara.y;
    double z=g_PointWeldPara.z;
    double r=g_PointWeldPara.r;
    double ZH=g_PointWeldPara.ZH;
    double atspeed=g_PointWeldPara.atspeed;
    unsigned long T3_Delay_time=g_PointWeldPara.T3_Delay_time ;

    double rr1=g_PointWeldPara.rr1;
    double rr2=g_PointWeldPara.rr2;

    double xx=x;
    double yy=y;
    double zz=z;
    double zz_ZH=z-ZH;

    int errInt=0;
    if(xx<0 || xx>vMAX_route[0])
    {
        errInt |= 0x01;
    }
    if(yy<0 || yy>vMAX_route[1])
    {
        errInt |= 0x02;
    }
    if(zz<0 || zz>vMAX_route[2])
    {
        errInt |= 0x04;
    }
    else if(zz_ZH<0 || zz_ZH>vMAX_route[2])
    {
        errInt |= 0x08;
    }
    if(r<vMIN_route[3] || r>vMAX_route[3])
    {
        errInt |= 0x10;
    }
    if(atspeed<0 || atspeed>ROBOT_MAX_ATSPEED)
    {
        errInt |= 0x20;
    }
    if(errInt>0)
    {
        emit activeError(errInt,0);
        return errInt;
    }

    isWelding=true; //禁止使用手柄，以免影响各轴移动
    //调整姿态
    if(g_MacType==3 || g_MacType==4 || g_MacType==1)
    {
        ML_MovPosition(a_z,ZH,atspeed);  //将Z轴提升到ZH的位置
        ML_Move_R1R2(rr1,rr2,atspeed,100);
    }

    if((g_MacType==2 || g_MacType==4 ||g_MacType==6) && (x>vMAX_route[0]/2)) //
        ML_FF_Set_Act_Y2(1);    //双平台，走Y2
    else
        ML_FF_Set_Act_Y2(0);    //走Y
    int st=0;
    st=ML_F_Go_Empty_Point(x,   y,  z,  r,  atspeed,  ZH, T3_Delay_time);
    isWelding=false; //使用手柄
    if(st==0)
    {
        emit operateFinished(ROBOT_BLANK_POINT_END);  //完成操作
        return 0;
    }
    else
    {
        errInt = 0x400;
        emit activeError(errInt,st);                //函数执行出错
        return errInt;
    }
}

int ManlyRobot::ChangeIronPose()  //烙铁姿态调整功能函数
{
    double x=g_PointWeldPara.x;
    double y=g_PointWeldPara.y;
    double z=g_PointWeldPara.z;
    double r=g_PointWeldPara.r;
    double r1=g_PointWeldPara.S2_Length;
    double r2=g_PointWeldPara.S2_T_Length;
    double ZH=g_PointWeldPara.ZH;
    double atspeed=g_PointWeldPara.atspeed;
    unsigned long T3_Delay_time=g_PointWeldPara.T3_Delay_time ;

    double xx=x;
    double yy=y;
    double zz=z;
    double zz_ZH=z-ZH;

    int errInt=0;
    if(xx<0 || xx>vMAX_route[0])
    {
        errInt |= 0x01;
    }
    if(yy<0 || yy>vMAX_route[1])
    {
        errInt |= 0x02;
    }
    if(zz<0 || zz>vMAX_route[2])
    {
        errInt |= 0x04;
    }
    else if(zz_ZH<0 || ZH<5.0 || zz_ZH>vMAX_route[2])
    {
        errInt |= 0x08;
    }
    if(r<vMIN_route[3] || r>vMAX_route[3])
    {
        errInt |= 0x10;
    }
    if(atspeed<0 || atspeed>ROBOT_MAX_ATSPEED)
    {
        errInt |= 0x20;
    }
    if(errInt>0)
    {
        emit activeError(errInt,0);
        return errInt;
    }

    isWelding=true; //禁止使用手柄，以免影响各轴移动
    if((g_MacType==2 || g_MacType==4 ||g_MacType==6) && (x>vMAX_route[0]/2)) //
        ML_FF_Set_Act_Y2(1);    //双平台，走Y2
    else
        ML_FF_Set_Act_Y2(0);    //走Y
    int st=0;
    ML_Set_Change_PositionStyle(0); //解锁姿态调整
    st=ML_Change_Position(x,   y,  z,  r,  atspeed,  ZH, T3_Delay_time,r1,r2);  //执行姿态调整
    isWelding=false; //使用手柄
    if(st==0)
    {
        emit operateFinished(ROBOT_CHANGE_POSE_END);  //完成操作
        return 0;
    }
    else
    {
        errInt = 0x10000;
        emit activeError(errInt,st);                //函数执行出错
        return errInt;
    }
}

int ManlyRobot::lineWeld()    //拖焊操作
{
    double x0=g_LineWeldPara.x0;
    double y0= g_LineWeldPara.y0;
    double z0= g_LineWeldPara.z0;
    double r0= g_LineWeldPara.r0;
    double x1=g_LineWeldPara.x1;
    double y1= g_LineWeldPara.y1;
    double z1= g_LineWeldPara.z1;
    double r1= g_LineWeldPara.r1;
    double ZH= g_LineWeldPara.ZH;
    double atspeed= g_LineWeldPara.atspeed;
    double Dlt_X= g_LineWeldPara.Dlt_X ;
    double Dlt_Y= g_LineWeldPara.Dlt_Y ;
    double Dlt_Z= g_LineWeldPara.Dlt_Z ;
    double Dlt_speed= g_LineWeldPara.Dlt_speed ;
    double FT_Pre_Length= g_LineWeldPara.FT_Pre_Length ;
    double FT_Pre_Speed= g_LineWeldPara.FT_Pre_Speed ;
    double TH_speed= g_LineWeldPara.TH_speed ;
    double S1_Length= g_LineWeldPara.S1_Length ;
    double S1_T_Length= g_LineWeldPara.S1_T_Length ;
    double S1_speed= g_LineWeldPara.S1_speed ;
    double S2_Length= g_LineWeldPara.S2_Length ;
    double S2_T_Length= g_LineWeldPara.S2_T_Length ;
    double S2_speed= g_LineWeldPara.S2_speed ;
    double Dlt_TX= g_LineWeldPara.Dlt_TX ;
    double Dlt_TY= g_LineWeldPara.Dlt_TY ;
    double Dlt_TZ= g_LineWeldPara.Dlt_TZ ;
    unsigned long T1_Delay_time= g_LineWeldPara.T1_Delay_time ;
    unsigned long T2_Delay_time= g_LineWeldPara.T2_Delay_time ;
    unsigned long T3_Delay_time= g_LineWeldPara.T3_Delay_time ;
    unsigned long v_Over_Time_count= g_LineWeldPara.v_Over_Time_count ;
    unsigned char Optimize=g_LineWeldPara.Optimize ;

    double xx0=x0+Dlt_X*cos(PI*r0/180);
    double yy0=y0+Dlt_Y*sin(PI*r0/180);
    double zz0=z0-Dlt_Z;
    double zz_ZH0=z0-ZH;
    double xx1=x1+Dlt_X*cos(PI*r1/180);
    double yy1=y1+Dlt_Y*sin(PI*r1/180);
    double zz1=z1-Dlt_Z;
    double zz_ZH1=z1-ZH;

    int errInt=0;
    if(xx0<0 || xx0>vMAX_route[0] || xx1<0 || xx1>vMAX_route[0])
    {
        errInt |= 0x01;
    }
    if(yy0<0 || yy0>vMAX_route[1] || yy1<0 || yy1>vMAX_route[1])
    {
        errInt |= 0x02;
    }
    if(zz0<0 || zz0>vMAX_route[2] || zz1<0 || zz1>vMAX_route[2])
    {
        errInt |= 0x04;
    }
    else if(zz_ZH0<0 || zz_ZH0>vMAX_route[2] || zz_ZH1<0 || zz_ZH1>vMAX_route[2])
    {
        errInt |= 0x08;
    }
    if(r0<vMIN_route[3] || r0>vMAX_route[3] || r1<vMIN_route[3] || r1>vMAX_route[3])
    {
        errInt |= 0x10;
    }
    if(atspeed<0 || atspeed>ROBOT_MAX_ATSPEED)
    {
        errInt |= 0x20;
    }
    if(errInt>0)
    {
        emit activeError(errInt,0);
        return errInt;
    }

    isWelding=true; //禁止使用手柄，以免影响各轴移动
    //调整姿态
    if(g_MacType==3 || g_MacType==4 || g_MacType==1)
    {
        double rr1=g_LineWeldPara.rr1;
        double rr2=g_LineWeldPara.rr2;

        ML_MovPosition(a_z,ZH,atspeed);  //将Z轴提升到ZH的位置
        ML_Move_R1R2(rr1,rr2,atspeed,100);
    }

    if((g_MacType==2 || g_MacType==4 ||g_MacType==6) && (x0>vMAX_route[0]/2)) //
        ML_FF_Set_Act_Y2(1);    //双平台，走Y2
    else
        ML_FF_Set_Act_Y2(0);    //走Y

    ML_FF_Set_WorkLine_NUM(g_lineWeldType); //g_lineWeldType:拖焊类型：0=普通拖焊；1=多段拖焊首段；2=多段拖焊中段；3=多段拖焊末段；
    int st=0;
    st=ML_F_Delay_Work_line( x0,  y0, z0, r0, x1,  y1, z1, r1, atspeed, ZH, Dlt_X, Dlt_Y, Dlt_Z, Dlt_speed, FT_Pre_Length, FT_Pre_Speed, TH_speed,
                         S1_Length, S1_T_Length, S1_speed, S2_Length, S2_T_Length, S2_speed, Dlt_TX, Dlt_TY, Dlt_TZ, T1_Delay_time, T2_Delay_time, T3_Delay_time, v_Over_Time_count,Optimize);
    isWelding=false; //使用手柄
    if(st==0)
    {
        emit operateFinished(ROBOT_LINE_WELD_END);  //完成操作
        return 0;
    }
    else
    {
        errInt = 0x200;
        emit activeError(errInt,st);                //函数执行出错
        return errInt;
    }
}

void ManlyRobot::clearSolder()   //清锡操作
{
    isAutoClearSolder=true;
    double x=clearItem.clearPos[0];
    double y=clearItem.clearPos[1];
    double z=clearItem.clearPos[2];
    double r=clearItem.clearPos[3];

    double xx=x;
    double yy=y;
    double zz=z;
    double zz_ZH=z-clearItem.Zh;

    int errInt=0;
    if(xx<0 || xx>vMAX_route[0])
    {
        errInt |= 0x01;
    }
    if(yy<0 || yy>vMAX_route[1])
    {
        errInt |= 0x02;
    }
    if(zz<0 || zz>vMAX_route[2])
    {
        errInt |= 0x04;
    }
    else if(zz_ZH<0 || zz_ZH>vMAX_route[2])
    {
        errInt |= 0x08;
    }
    if(r<vMIN_route[3] || r>vMAX_route[3])
    {
        errInt |= 0x10;
    }
    if(clearItem.atspeed<0 || clearItem.atspeed>ROBOT_MAX_ATSPEED)
    {
        errInt |= 0x20;
    }
    if(errInt>0)
    {
        emit activeError(errInt,0);
        return ;
    }

    isWelding=true; //禁止使用手柄，以免影响各轴移动
    if((g_MacType==2 || g_MacType==4 ||g_MacType==6) && (x>vMAX_route[0]/2)) //
        ML_FF_Set_Act_Y2(1);    //双平台，走Y2
    else
        ML_FF_Set_Act_Y2(0);    //走Y
    int st=0;
    //st=ML_F_Go_Clear(x,y,z,r,clearItem.atspeed,clearItem.Zh,clearItem.clear_H,clearItem.clearTime);
    //st=ML_F_Go_Empty_Point(x,y,10,r,clearItem.atspeed,0, 1);  //移动到清锡点
    //调整姿态
    if(g_MacType==3 || g_MacType==4 || g_MacType==1)
    {
        double rr1=clearItem.clearPos[5];
        double rr2=clearItem.clearPos[6];

        ML_MovPosition(a_z,clearItem.Zh,clearItem.atspeed);  //将Z轴提升到ZH的位置
        ML_Move_R1R2(rr1,rr2,clearItem.atspeed,100);
    }
    st=ML_F_Go_Empty_Point(x,y,z,r,clearItem.atspeed,clearItem.Zh, 1);  //移动到清锡点
    if(st==0)
    {
        if(clearItem.clear_H>0)
        {
            ML_FF_Manual_FT(0,1);       //手动下送锡操作,第一个参数dir=0,送锡,1退锡； 第二个参数1=有效,0=无效
            QThread::msleep(clearItem.clear_H);    //延时
        }
        ML_FF_Manual_FT(0,0);
        //清锡输出
        QThread::msleep(500);
        ML_SetOutput(g_IOxb.sysOxb[CLEAR_O]);    //清锡输出
        QThread::msleep(clearItem.clearTime);    //延时
        ML_ClearOutput(g_IOxb.sysOxb[CLEAR_O]);    //清锡输出停止
        emit operateFinished(ROBOT_CLEAR_SOLDER_END);  //完成操作
    }
    else
    {
        errInt = 0x800;
        emit activeError(errInt,st);                //函数执行出错
    }
    isWelding=false; //使用手柄
    isAutoClearSolder=false;
    ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
}

void ManlyRobot::resetRobot()  //复位回原点操作
{
    ML_FF_Stop_Tin(0);   //设置为可以送锡
    emit resetOperate(1);   //开始复位
    isMove_R_Reset=true;
    //ML_ClearOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯灭
    //机器回零
    isReseting=true;
    int st=0;


    if(isEStopReset==1)
    {
        isEStopReset=0;
        int timeout=0;
        unsigned char st_Z=1;
        do
        {
            Axis_N_Move(a_z,LINE_MOVE);         //如果z轴原点信号有效，提升z轴
            ML_GetORG(a_z,&st_Z);    //读z轴原点的状态
            QThread::msleep(5);    //延时5ms
            timeout++;
        }
        while((st_Z==0) && (timeout<12000));
        ML_Stop(a_z, 1);    //停止Z轴移动，第2个参数，0=减速，1=立即停止
        QThread::msleep(5);    //延时5ms
        if(timeout>=12000)
        {
            emit activeError(0x4000,-8000);                //r轴原点信号出错
            return ;
        }
 /*       ML_GetORG(a_r,&g_ioStatus.org[a_r]);    //读r轴原点的状态
        if(g_ioStatus.org[a_r]==1)
            Axis_P_Move(a_r,LINE_MOVE);         //如果r轴原点信号有效，正转
        timeout=0;
        while(g_ioStatus.org[a_r] && (timeout<12000)) //读原点信号，直到无效
        {
            Axis_P_Move(a_r,LINE_MOVE);         //如果r轴原点信号有效，正转
            QThread::msleep(5);    //延时5ms
            ML_GetORG(a_r,&g_ioStatus.org[a_r]);
            timeout++;
        }
        ML_Stop(a_r, 0);    //停止r轴移动，第2个参数，0=减速，1=立即停止
        if(timeout>=12000)
        {
            emit activeError(0x4000,-9000);                //r轴原点信号出错
            return ;
        }
        timeout=0;
        QThread::msleep(5);    //延时5ms
*/
        ML_Set_HomingStyle(0);
        if((g_MacType==3 || g_MacType==1 || g_MacType==4)) //&& (isEStopReset==1)
        {
            st=ML_F_MovToORG_D(1);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
        }
        else
        {
            st=ML_F_MovToORG_D(0);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
        }
    }
    ML_Set_HomingStyle(1);
    if((g_MacType==3 || g_MacType==1 || g_MacType==4)) //&& (isEStopReset==1)
    {
        st=ML_F_MovToORG_D(1);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    }
    else
    {
        st=ML_F_MovToORG_D(0);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    }
    resetFlag=0 ;
    isReseting=false;
    isMove_R_Reset=false;
    if(st==0)
    {
        //QThread::msleep(100);    //延时
        ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
        //emit operateFinished(ROBOT_RESET_ORG_END);  //完成操作
        emit resetOperate(2);   //复位结束
        return ;
    }
    else
    {
        ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
        emit activeError(0x1000,st);                //函数执行出错
        return ;
    }
}

void ManlyRobot::moveToOrg()  //在命令中复位，机器回原点
{
    emit resetOperate(3);   //开始复位
    isMove_R_Reset=true;

    //R轴回正
    //机器回零
    isReseting=true;
    int st=0;

    //mySleep(5);

    //ML_MovPosition(a_z,10,40);  //将Z轴提升到10的位置
    //if(g_MacType==3 || g_MacType==4 || g_MacType==1)
    //    st=ML_F_MovToORG_D(1);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    //else
    /*if(isEStopReset==1)
    {
        isEStopReset=0;
        ML_Set_HomingStyle(0);
    }*/
    if((g_MacType==3 || g_MacType==1 || g_MacType==4))
    {
        st=ML_F_MovToORG_D(1);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    }
    else
    {
        st=ML_F_MovToORG_D(0);//XYZR==HOM，回零点,参数:1=全部轴回零点,0=xyzr回零.
    }
    resetFlag=0 ;
    isReseting=false;
    isMove_R_Reset=false;
    if(st==0)
    {
        ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
        emit operateFinished(ROBOT_ORG_END);  //完成操作
        emit resetOperate(4);   //复位结束
        return ;
    }
    else
    {
        ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
        emit activeError(0x1000,st);                //函数执行出错
        return ;
    }
}

void ManlyRobot::exitForm() //退出关闭板卡
{
    int st=0;
    st=ML_ExitForm();
    if(st!=0)
    {
        emit activeError(0x2000,st);                //函数执行出错
        return ;
    }
}
void ManlyRobot::processLimitValue(unsigned char axis,unsigned char mod)  //轴运行到极限边界的处理, mod: 1=上限, 0=下限
{
    if(isReseting) return;    //如果正在复位，不执行边界处理
    ML_Stop(axis, 1);    //停止X轴移动，第2个参数，0=减速，1=立即停止
    mySleep(1);
    ML_Set_Axis_INCJogValue(axis,2);   //设置步距    0.1mm

    if((xyKeyPressedCnt[0]>1) || (xyKeyPressedCnt[1]>1) || (xyKeyPressedCnt[2]>1) || (xyKeyPressedCnt[3]>1))    //手柄操作时边界处理只做一次点动，大于1次则停止返回
    {
        ML_StopAll(1);
        return;
    }

    if(mod==1)
    {
        if(coordinate[axis]>=vMAX_route[axis]-1)
        {
            ML_Stop(axis, 0);
        }
        else
        {
            ML_SetJogMode(POINT_MOVE);
            ML_SetJogMove ( axis, POSITIVE, DISABLE );//禁止手动连续移动  ,POSITIVE=正向移动
            ML_SetJogINCMove ( axis, POSITIVE, ENABLE );//允许手动点动移动
        }
    }
    else
    {
        if(coordinate[axis]<=vMIN_route[axis]+1)
        {
            ML_Stop(axis, 0);
        }
        else
        {
            ML_SetJogMode(POINT_MOVE);
            ML_SetJogMove ( axis, NEGATIVE, DISABLE );//禁止手动连续移动  ,NEGATIVE=负向移动
            ML_SetJogINCMove ( axis, NEGATIVE, ENABLE );//允许手动点动移动
        }
    }

}

void ManlyRobot::start_P_Move(unsigned char axis)      //在采用按键手柄操作时，轴正向移动
{
    //int st=0;
    if(coordinate[axis]>upValue[axis])
    {
        processLimitValue(axis,1);
        return;
    }
    flagLimit2[axis]=false;
    if(axisStatus[axis]==1)     //如果轴正在移动
        return;
    axisStatus[axis]=1;     //标记轴正在移动
    if(g_moveMod==POINT_MOVE)
        ML_Set_Axis_INCJogValue(axis,g_setpLen+1);   //设置步距
    Axis_P_Move(axis,g_moveMod);
    /*if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(axis,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(axis,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(axis,POINT_MOVE);
    }*/
}

void ManlyRobot::start_N_Move(unsigned char axis)      //在采用按键手柄操作时，轴负向移动.  axis=轴号
{
    //int st=0;
    if(coordinate[axis]<downValue[axis])
    {
        processLimitValue(axis,0);
        return;
    }
    flagLimit[axis]=false;
    if(axisStatus[axis]==1)     //如果轴正在移动
        return;
    axisStatus[axis]=1;     //标记轴正在移动
    if(g_moveMod==POINT_MOVE)   //如果是点动
        ML_Set_Axis_INCJogValue(axis,g_setpLen+1);   //设置步距
    Axis_N_Move(axis,g_moveMod);
    /*if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_N_Move(axis,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(axis,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            return;
        }
        emit signal_Axis_N_Move(axis,POINT_MOVE);
    }*/
}

void ManlyRobot::goFixPosSlot(int weldType)//定位功能 weldType=1,点焊 ;  weldType=2,拖焊
{
    double x,y,z,r,ZH,atspeed,Dlt_X,Dlt_Y,Dlt_Z,Dlt_speed;
    double rr1;
    double rr2;

    if(weldType==1)
    {
        x=g_PointWeldPara.x;
        y=g_PointWeldPara.y;
        z=g_PointWeldPara.z;
        r=g_PointWeldPara.r;
        ZH=g_PointWeldPara.ZH;
        atspeed=g_PointWeldPara.atspeed;
        Dlt_X=g_PointWeldPara.Dlt_X;
        Dlt_Y=g_PointWeldPara.Dlt_Y;
        Dlt_Z=g_PointWeldPara.Dlt_Z;
        Dlt_speed=g_PointWeldPara.Dlt_speed;
        //姿态
        rr1=g_PointWeldPara.rr1;
        rr2=g_PointWeldPara.rr2;
    }
    else
    {
        x=g_LineWeldPara.x0;
        y=g_LineWeldPara.y0;
        z=g_LineWeldPara.z0;
        r=g_LineWeldPara.r0;
        ZH=g_LineWeldPara.ZH;
        atspeed=g_LineWeldPara.atspeed;
        Dlt_X=g_LineWeldPara.Dlt_X;
        Dlt_Y=g_LineWeldPara.Dlt_Y;
        Dlt_Z=g_LineWeldPara.Dlt_Z;
        Dlt_speed=g_LineWeldPara.Dlt_speed;
        //姿态
        rr1=g_LineWeldPara.rr1;
        rr2=g_LineWeldPara.rr2;
    }

    isWelding=true; //禁止使用手柄，以免影响各轴移动
    ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）

    //调整姿态
    if(g_MacType==3 || g_MacType==4 || g_MacType==1)
    {
        ML_MovPosition(a_z,ZH,atspeed);  //将Z轴提升到ZH的位置
        ML_Move_R1R2(rr1,rr2,atspeed,100);
    }

    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (x>vMAX_route[0]/2))
        ML_FF_Set_Act_Y2(1);    //双平台，走Y2
    else
        ML_FF_Set_Act_Y2(0);    //走Y
    ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
    int st=0;
    st=ML_F_Go_fixposition_Point(x,y,z,r,atspeed,ZH,Dlt_X,Dlt_Y,Dlt_Z,Dlt_speed);
    isWelding=false; //可以使用手柄
    int errInt=0;
    if(st==0)
    {
        emit operateFinished(ROBOT_GO_FIX_POS_END);  //完成操作
        return ;
    }
    else
    {
        errInt = 0x10000;
        emit activeError(errInt,st);                //函数执行出错
        return ;
    }
}

void ManlyRobot::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
////////////////////////////////////////////class RobotThread //////////////////////////////////////////////////////

RobotThread::RobotThread(QObject *parent) :
    QThread(parent)
{
}

RobotThread::~RobotThread()
{
}

void RobotThread::run()
{
    robot=new ManlyRobot;
    int st=robot->initRobot();

    emit robotCreated();

    if(st==-999999)
    {
        robot->mySleep(300);
        if(robot->statusLoadDll1>0)
            emit robot->activeError(0x8000,robot->statusLoadDll1);                //加载动态库出错
        if(robot->statusLoadDll2>0)
            emit robot->activeError(0x8000,robot->statusLoadDll2);                //加载动态库出错
    }

    exec();
}

////////////////////////////////////////////class RobotThread2 //////////////////////////////////////////////////////

RobotThread2::RobotThread2(QObject *parent) :
    QThread(parent)
{
}

RobotThread2::~RobotThread2()
{
}

void RobotThread2::run()
{
    //robot=new ManlyRobot;
    //robot->initRobot();this

    coodTimer=new QTimer();
    connect(coodTimer, SIGNAL(timeout()), this, SLOT(getAxisCoordinate()));
    coodTimer->start(50);
    //ioTimer=new QTimer(this);
    //connect(ioTimer, SIGNAL(timeout()), this, SLOT(getIoStatus()));
    //ioTimer->start(50);
    //startReadCoordinate();
    //robot->startReadIoStatus();
    //emit robotCreated();
    exec();
}
int RobotThread2::getAxisCoordinate()    //获得轴坐标
{
    robot->getAxisCoordinate();

    return 0;
}
int RobotThread2::getIoStatus()    //获得IO
{
    robot->getIoStatus();
    return 0;
}

////////////////////////////////////////////class RobotThread3 //////////////////////////////////////////////////////

RobotThread3::RobotThread3(QObject *parent) :
    QThread(parent)
{
}

RobotThread3::~RobotThread3()
{
}

void RobotThread3::run()
{
    //robot=new ManlyRobot;
    //robot->initRobot();this

    ioTimer=new QTimer();
    connect(ioTimer, SIGNAL(timeout()), this, SLOT(getIoStatus()));
    ioTimer->start(30);

    exec();
}

int RobotThread3::getIoStatus()    //获得IO
{
    robot->getIoStatus();
    return 0;
}
