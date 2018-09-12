#ifndef SQLDB_H
#define SQLDB_H

#include <QObject>
#include <QSqlDatabase>
#include <QDateTime>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>

#include "global.h"

#define NOT_PCCTRL      0
#define PC_CONTROL      1

#define WELCOME         0
#define TESTDIALOG      1
#define INQUIRYDIALOG   2
#define RESULTDIALOG    3

#define READY_OK        0
#define WEIGHT_TEST     1
#define WEIGHT_OK       2
#define BCA_TEST        3
#define BCA_TEST_OK     4
#define BCA_INQU        5
#define BCA_INQU_OK     6
#define DATA_TRANS_OK   7

const QString SQL_CREATE_USERS_TABLE	= "CREATE TABLE Users (UserName NVARCHAR(20) NOT NULL, UserPassword NVARCHAR(20), Right INTEGER,PRIMARY KEY(UserName))";//
const QString SQL_CREATE_USERS_INDEX	= "CREATE INDEX PK_Users on Users (UserName ASC)";
const QString SQL_DROP_USERS			= "DROP TABLE Users";

const QString SQL_CREATE_MACHINE    	= "CREATE TABLE Machine (ID NVARCHAR(20) NOT NULL, Model NVARCHAR(20), Production INTEGER, WeldPoint INTEGER, Reserve1 NVARCHAR(20), Reserve2 NVARCHAR(20))";//
const QString SQL_CREATE_MACHINE_INDEX	= "CREATE INDEX PK_Machine on Machine (ID ASC)";
const QString SQL_DROP_MACHINE			= "DROP TABLE Machine";

const QString SQL_CREATE_PROJFILE_TABLE	= "CREATE TABLE ProjFile (ProjectName NVARCHAR(30) NOT NULL, OperateNum INTEGER,Command NVARCHAR(20),Technics NVARCHAR(20), Zh double, Optimize INTEGER,Direction NVARCHAR(20),\
                                               X double, Y double, Z double, R double, S double, R1 double, R2 double, E double,\
                                               eX double, eY double, eZ double, eR double, eS double, eR1 double, eR2 double, eE double, PRIMARY KEY(ProjectName,OperateNum))";
const QString SQL_CREATE_PROJFILE_INDEX	= "CREATE INDEX PK_ProjFile on ProjFile (ProjectName,OperateNum ASC)";
const QString SQL_DROP_PROJFILE			= "DROP TABLE ProjFile";

const QString SQL_CREATE_POINTWELD_TABLE	= "CREATE TABLE PointWeld (Name NVARCHAR(20) NOT NULL,ZH double, atspeed double, Dlt_X double, Dlt_Y double, Dlt_Z double,\
                                            Dlt_speed double, FT_Pre_Length double, FT_Pre_Speed double,FT_Pre_Modle INTEGER,Soldering_style INTEGER,\
                                            ShakeSolder_Length double,ShakeSolder_speed double,S1_Length double,S1_T_Length double,S_speed double,S2_Length double,S2_T_Length double,\
                                            Dlt_TX double,Dlt_TY double,Dlt_TZ double,T1_Delay_time INTEGER,T2_Delay_time INTEGER,T3_Delay_time INTEGER,v_Over_Time_count INTEGER,\
                                            PRIMARY KEY(Name))";
const QString SQL_CREATE_POINTWELD_INDEX	= "CREATE INDEX PK_PointWeld on PointWeld (Name ASC)";
const QString SQL_DROP_POINTWELD			= "DROP TABLE PointWeld";

const QString SQL_CREATE_LINEWELD_TABLE	= "CREATE TABLE LineWeld (Name NVARCHAR(20) NOT NULL,ZH double, atspeed double, Dlt_X double, Dlt_Y double, Dlt_Z double,\
                                            Dlt_speed double, FT_Pre_Length double, FT_Pre_Speed double,TH_speed double,S1_Length double,S1_T_Length double,S1_speed double,\
                                            S2_Length double,S2_T_Length double,S2_speed double,Dlt_TX double,Dlt_TY double,Dlt_TZ double,\
                                            T1_Delay_time INTEGER,T2_Delay_time INTEGER,T3_Delay_time INTEGER,v_Over_Time_count INTEGER,PRIMARY KEY(Name))";
const QString SQL_CREATE_LINEWELD_INDEX	= "CREATE INDEX PK_LineWeld on LineWeld (Name ASC)";
const QString SQL_DROP_LINEWELD			= "DROP TABLE LineWeld";

const QString SQL_CREATE_DELAY_TABLE	= "CREATE TABLE Delay (Name NVARCHAR(20) NOT NULL,Time INTEGER, PRIMARY KEY(Name))";
const QString SQL_CREATE_DELAY_INDEX	= "CREATE INDEX PK_Delay on Delay (Name ASC)";
const QString SQL_DROP_DELAY			= "DROP TABLE Delay";

const QString SQL_CREATE_SKIP_TABLE	= "CREATE TABLE Skip (Name NVARCHAR(20) NOT NULL,Number INTEGER, PRIMARY KEY(Name))";
const QString SQL_CREATE_SKIP_INDEX	= "CREATE INDEX PK_Skip on Skip (Name ASC)";
const QString SQL_DROP_SKIP			= "DROP TABLE Skip";

const QString SQL_CREATE_CONDITION_TABLE	= "CREATE TABLE Condition (Name NVARCHAR(20) NOT NULL,Number INTEGER,Time INTEGER,io INTEGER, PRIMARY KEY(Name))";
const QString SQL_CREATE_CONDITION_INDEX	= "CREATE INDEX PK_Condition  on Condition  (Name ASC)";
const QString SQL_DROP_CONDITION			= "DROP TABLE Condition ";

const QString SQL_CREATE_BLANKPOINT_TABLE	= "CREATE TABLE BlankPoint (Name NVARCHAR(20) NOT NULL,X double, Y double, Z double, R double, S double, R1 double, R2 double, E double,atspeed double,Delay_time INTEGER, PRIMARY KEY(Name))";
const QString SQL_CREATE_BLANKPOINT_INDEX	= "CREATE INDEX PK_BlankPoint  on BlankPoint  (Name ASC)";
const QString SQL_DROP_BLANKPOINT			= "DROP TABLE BlankPoint ";

const QString SQL_CREATE_CLEAR_TABLE	= "CREATE TABLE Clear (Name NVARCHAR(20) NOT NULL,atspeed double,Zh double,CLear_H double,Delay_time INTEGER,X double, Y double, Z double, R double, S double, R1 double, R2 double, E double, PRIMARY KEY(Name))";
const QString SQL_CREATE_CLEAR_INDEX	= "CREATE INDEX PK_Clear  on Clear  (Name ASC)";
const QString SQL_DROP_CLEAR			= "DROP TABLE Clear ";

const QString SQL_CREATE_IO_TABLE	= "CREATE TABLE Io (Name NVARCHAR(20) NOT NULL,Address INTEGER, PRIMARY KEY(Name))";
const QString SQL_CREATE_IO_INDEX	= "CREATE INDEX PK_Io  on Io  (Name ASC)";
const QString SQL_DROP_IO			= "DROP TABLE Io ";

const QString SQL_CREATE_PROJLOG_TABLE	= "CREATE TABLE ProjLog (Name NVARCHAR(20) NOT NULL,CardCount INTEGER,PointCount INTEGER,WeldTime INTEGER, PRIMARY KEY(Name))";
const QString SQL_CREATE_PROJLOG_INDEX	= "CREATE INDEX PK_ProjLog  on ProjLog  (Name ASC)";
const QString SQL_DROP_PROJLOG			= "DROP TABLE ProjLog ";

const QString SQL_CREATE_ENVPARA_TABLE	= "CREATE TABLE EnvPara (Name NVARCHAR(20) NOT NULL,HanPan_HD double, HanPan_BMCL NVARCHAR(20), HanPan_FTXS NVARCHAR(20), HanPan_ZJ double, HanPan_KJ double,\
                                            HanXiSi_ZJ double,HanXiSi_ZL NVARCHAR(20),HanXiSi_ZHJ double,HanXiSi_FW NVARCHAR(20),\
                                            LaoTie_WD double,LaoTie_ZL NVARCHAR(20), PRIMARY KEY(Name))";
const QString SQL_CREATE_ENVPARA_INDEX	= "CREATE INDEX PK_EnvPara on EnvPara (Name ASC)";
const QString SQL_DROP_ENVPARA			= "DROP TABLE EnvPara";

const QString SQL_CREATE_CORRECTPARA_TABLE	= "CREATE TABLE CorrectPara (Name NVARCHAR(20) NOT NULL,Initangle double,Turning_Radius double, PRIMARY KEY(Name))";
const QString SQL_CREATE_CORRECTPARA_INDEX	= "CREATE INDEX PK_CorrectPara on CorrectPara (Name ASC)";

class SqlDB : public QObject
{
    Q_OBJECT
public:
    explicit SqlDB(QObject *parent = 0);
    bool openDB(QString dbName); //创建连接函数
    void closeDB(); //创建关闭函数
    bool ReadProjNameFromDb(QStringList &listProjName);     //读所有的项目名称
    bool SaveProjItemToDb(QString projectName,PROJ_ITEM item);//保存焊接项目单条操作内容
    bool ReadProjItemFromDb(QString projectName,int commNumber,PROJ_ITEM &item);//读焊接项目单条操作内容
    bool SaveProjAllItemToDb(QString projectName,QList <PROJ_ITEM> AllItem);//保存焊接项目所有命令操作内容
    bool ReadProjAllItemFromDb(QString projectName,QList <PROJ_ITEM> &AllItem);//读焊接项目所有命令操作内容
    bool SaveAllClearItemToDb(QString projectName,QList <CLEAR_SOLDER> AllItem);//保存焊接项目所有命令操作内容
    bool ReadAllClearItemFromDb(QString projectName,QList <CLEAR_SOLDER> &AllItem);//读焊接项目所有命令操作内容
    bool DeleteProjItemFromDb(QString projectName,int commNumber); //删除焊接项目某一条
    bool DeleteProjectFromDb(QString projectName); //删除整个焊接项目
    bool UpdateProjectFromDb(QString oldProjectName,QString newProjectName,QString direction); //更新整个焊接项目的名称、焊锡面朝向
    bool UpdateTechnicsFromDb(QString projectName,QString oldTechnics,QString newTechnics,QString command); //将项目中的oldTechnics工艺参数更新为newTechnics工艺参数
    bool UpdateZhFromDb(QString projectName,double old_Zh,double new_Zh,QString command); //将项目中的old_Zh提升高度更新为new_Zh提升高度
    int getProjItemCount(QString projectName);  //获得焊接项目里操作命令条数
    bool SavePointWeldToDb(QString pointWeldName,S_P pointWeldPara);   //保存点焊工艺参数
    bool ReadPointWeldFromDb(QString pointWeldName,S_P &pointWeldPara); //读点焊工艺参数
    bool ReadPointTechNameFromDb(QStringList &pointWeldName);   //读点焊工艺名称
    bool DeletePointTechFromDb(QString pointTechnicsName);      //删除点焊工艺参数
    bool SaveLineWeldToDb(QString lineWeldName,L_P lineWeldPara);   //保存拖焊工艺参数
    bool ReadLineWeldFromDb(QString lineWeldName,L_P &lineWeldPara);//读拖焊工艺参数
    bool ReadLineTechNameFromDb(QStringList &lineWeldName); //读拖焊工艺名称
    bool DeleteLineTechFromDb(QString lineTechnicsName);    //删除拖焊工艺参数
    bool SaveClearSolderToDb(QString projectName,CLEAR_SOLDER clearItem);   //保存清锡参数
    bool ReadClearSolderFromDb(QString projectName,CLEAR_SOLDER &clearItem);//读焊接项目清锡参数
    bool DeleteClearSolderFromDb(QString projectName); //删除焊接项目清锡参数
    bool SaveProjLogToDb(QString projectName,int cardCount,int pointCount,int weldTime);//保存产量日志
    bool ReadProjLogFromDb(QString projectName,int *cardCount,int *pointCount,int *weldTime);//读产量日志
    bool DeleteProjLogFromDb(QString projectName); //删除产量日志
    bool SaveGotoCmdToDb(QString projectName,int commNumber,int gotoNumber);//保存跳转指令参数
    bool ReadGotoCmdFromDb(QString projectName,int commNumber,int *gotoNumber);//读跳转指令参数
    bool DeleteGotoCmdFromDb(QString projectName,int commNumber); //删除跳转指令参数
    QSqlDatabase getCurrDb();
    bool SaveMachineToDb(QString id,QString machineModel,int Production,int weldPoint,QString Reserve1="",QString Reserve2="");     //保存机器信息及产量
    bool ReadMachineFromDb(QString &id,QString &machineModel,int *Production,int *weldPoint); //读机器信息及产量
    bool SaveEnvParaToDb(QString paraName,ENV_PARA envPara);   //保存工艺环境参数
    bool ReadEnvParaFromDb(QString paraName,ENV_PARA &envPara); //读工艺环境参数
    bool DeleteEnvParaFromDb(QString paraName); //删除工艺环境参数
    bool SaveCorrectParaToDb(QString projName,double Initangle,double Turning_Radius);   //保存旋转校正参数
    bool ReadCorrectParaFromDb(QString projName,double *Initangle,double *Turning_Radius); //读旋转校正参数
    bool DeleteCorrectParaFromDb(QString projName); //删除旋转校正参数

    bool isOpen;
signals:
    
public slots:

private:
    QSqlDatabase db; //建立数据库的文件描述符
    bool isNewDatabase;
};

#endif // SQLDB_H
