#ifndef WELDPROJECT_H
#define WELDPROJECT_H

#include <QObject>
#include <QWidget>
#include <QStringList>
#include <QVector>
#include <QMap>
#include <QMessageBox>

#include "global.h"
#include "manlyrobot.h"
#include "serialport.h"


class WeldProject : public QObject
{
    Q_OBJECT
public:
    explicit WeldProject(QObject *parent = 0);
    virtual ~WeldProject();

    QStringList listCommand;        //焊接指令序列
    QStringList listTechnics;       //工艺名称序列
    QVector<double> Zh;             //Z提升高度序列QMap<QString, ENV_PARA> map;
    QMap<QString, ENV_PARA> envPrarMap;      //环境工艺参数
    QVector<int> optimize;          //优化
    QVector<double> curPointPos;    //当前焊点坐标,对拖焊来说，weldProject->curPointPos[i]，0~3为起点坐标，4~7为终点坐标
    double ironPos[2];              //烙铁姿态
    QList<QVector<double> > pointsAxisPos;//所有焊点坐标位置
    QList<QVector<double> > pointsAxisPos2;//拖焊焊点姿态坐标位置
    int currCommNum;                //当前指令序号
    //QString currTechnicsName;     //
    QString currPointTechnicsName;  //当前点焊工艺名称
    QString currLineTechnicsName;   //当前拖焊工艺名称
    double currZh;                  //当前Z提升高度
    ManlyRobot *robot;              //机器人控制器
    double movetoolMatrixPos[2][8]; //movetool传来的阵列两点坐标
    int execMode;           //执行方式,1=连续执行,  2=单条执行
    double atspeed;         //空点使用,移动速度
    int delayTime;          //空点使用,延时时间
    int stopWeldFlag;       //停止执行项目标志，1=停止；0=可以执行
    int exeChangeIronPose;  //是否执行了调整烙铁姿态，1=执行；0=没执行

    int flagInWelding;      //正在进行正式焊接
    int currCardCnt;        //当前项目已焊完的板子数量
    int currWeldPointCnt;   //当前项目已焊完的焊点数量
    int currWeldTime;       //当前项目进行的焊接时间
    int oldWeldPointCnt;    //当前项目已焊完的焊点数量

    S_P currPointWeldPara;              //当前点焊工艺参数
    L_P currLineWeldPara;               //当前拖焊工艺参数

    SerialPort* serialPort; //温控器串口

    void initConnect();
    bool loadProject(QString projectName);  //加载项目
    void saveProject();                     //保存当前项目
    void saveCurrItem();                    //保存当前命令条目
    void execProject(int index=0);          //连续执行项目
    void execItem();                        //执行项目中的当前项
    QString getProjName();      //获得焊接项目名称
    QString getDirection();     //获得焊点锡面方向
    void setProjName(QString wName);    //设置焊接项目名称
    void setDirection(QString direct);  //设置焊点锡面方向
    void insertCommand(int index);      //在项目中index位置插入一条空指令
    void appendCommand();               //在项目对象中最后位置添加一条空指令
    void deleteCommand(int index);      //在项目对象中删除一条指令
    void setCurrCommand(int index);     //设置当前指令
    int getExecMode();
    void setExecMode(int mode);
    void clear();
    void stopExeProj();
    void continueExeProj(int index);

signals:
    void startPointWeld();  //开始点焊
    void startLineWeld();   //开始拖焊
    void currCmdChange(int index); //当前执行的指令改变
    void clearSolderSig();  //触发清锡功能
    void weldfinished();    //焊接工程完成
    void moveToBlankPoint();//移动到空点
    void changeIronPose();  //调整烙铁姿态信号
    void resetSig();        //复位机器人
    void toOrgSig();        //机器回原点
    void reloadTableSig();  //刷新数据表

public slots:
    void execStatus(int type);  //对象执行焊接操作的结果状态,type:1=点焊, 2=拖焊
    void execReset();   //执行复位

private:
    QString currCommand;    //当前指令
    QString direction;      //焊点锡面方向
    QString name;           //项目名称
    QTime *mytime;          //时间计时器
    int weldTime;           //每一步焊接所用时间

    void mySleep(unsigned int msec);
};

#endif // WELDPROJECT_H
