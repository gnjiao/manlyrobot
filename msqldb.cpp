//#include <QMessageBox>
#include <QDir>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include "msqldb.h"

SqlDB::SqlDB(QObject *parent) :
    QObject(parent)
{
    isNewDatabase=false;
    isOpen=false;
}

bool SqlDB::openDB(QString dbName)
{
    if(!QDir::current().exists(dbName))
        isNewDatabase=true;
    else
        isNewDatabase=false;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QSQLITE");
    //db = QSqlDatabase::addDatabase("QSQLITE");//添加sqlite类型的数据库
    db.setDatabaseName(dbName); //指定数据库名
    if (!db.open())
    {
        isOpen=false;
        return false;
    }
    QSqlQuery query;
    if(isNewDatabase)
    {
        query.exec(SQL_CREATE_USERS_TABLE);
        query.exec(SQL_CREATE_USERS_INDEX);
        query.exec(SQL_CREATE_PROJFILE_TABLE);
        query.exec(SQL_CREATE_PROJFILE_INDEX);
        query.exec(SQL_CREATE_POINTWELD_TABLE);
        query.exec(SQL_CREATE_POINTWELD_INDEX);
        query.exec(SQL_CREATE_LINEWELD_TABLE);
        query.exec(SQL_CREATE_LINEWELD_INDEX);
        query.exec(SQL_CREATE_DELAY_TABLE);
        query.exec(SQL_CREATE_DELAY_INDEX);
        query.exec(SQL_CREATE_SKIP_TABLE);
        query.exec(SQL_CREATE_SKIP_INDEX);
        query.exec(SQL_CREATE_CONDITION_TABLE);
        query.exec(SQL_CREATE_CONDITION_INDEX);
        query.exec(SQL_CREATE_BLANKPOINT_TABLE);
        query.exec(SQL_CREATE_BLANKPOINT_INDEX);
        query.exec(SQL_CREATE_CLEAR_TABLE);
        query.exec(SQL_CREATE_CLEAR_INDEX);
        query.exec(SQL_CREATE_PROJLOG_TABLE);
        query.exec(SQL_CREATE_PROJLOG_INDEX);
        query.exec(SQL_CREATE_MACHINE);
        query.exec(SQL_CREATE_ENVPARA_TABLE);
        query.exec(SQL_CREATE_ENVPARA_INDEX);
        query.exec(SQL_CREATE_CORRECTPARA_TABLE);
        query.exec(SQL_CREATE_CORRECTPARA_INDEX);
        query.exec("INSERT INTO Users (UserName, UserPassword, Right) VALUES ('admin', 'admin', 3)");
        query.exec("INSERT INTO Users (UserName, UserPassword, Right) VALUES ('Manly', 'manlyly', 4)");
        query.exec("INSERT INTO Machine  VALUES ('01', 'MR', 0,0,'','')");
        query.exec("INSERT INTO EnvPara  VALUES ('default', 35, '镀锡','无',3.5,2.8,  0.3,'镍锡',2,'前上',  380,'08D')");

        S_P pointWeldPara;
        pointWeldPara.ZH=5;
        pointWeldPara.atspeed=20;
        pointWeldPara.Dlt_X=5;
        pointWeldPara.Dlt_Y=5;
        pointWeldPara.Dlt_Z=2;
        pointWeldPara.Dlt_speed=10;
        pointWeldPara.FT_Pre_Length=3;
        pointWeldPara.FT_Pre_Speed=10;
        pointWeldPara.FT_Pre_Modle=0;
        pointWeldPara.Soldering_style=0;
        pointWeldPara.ShakeSolder_Length=10;
        pointWeldPara.ShakeSolder_speed=10;
        pointWeldPara.S1_Length=3;
        pointWeldPara.S1_T_Length=0;
        pointWeldPara.S_speed=10;
        pointWeldPara.S2_Length=3;
        pointWeldPara.S2_T_Length=0;
        pointWeldPara.Dlt_TX=3;
        pointWeldPara.Dlt_TY=3;
        pointWeldPara.Dlt_TZ=2;
        pointWeldPara.T1_Delay_time=100;
        pointWeldPara.T2_Delay_time=20;
        pointWeldPara.T3_Delay_time=0;
        pointWeldPara.v_Over_Time_count=6000000;
        SavePointWeldToDb(QString("default"),pointWeldPara);

        L_P lineWeldPara;
        lineWeldPara.ZH=5;
        lineWeldPara.atspeed=20;
        lineWeldPara.Dlt_X=5;
        lineWeldPara.Dlt_Y=5;
        lineWeldPara.Dlt_Z=2;
        lineWeldPara.Dlt_speed=10;
        lineWeldPara.FT_Pre_Length=3;
        lineWeldPara.FT_Pre_Speed=10;
        lineWeldPara.TH_speed=10;
        lineWeldPara.S1_Length=3;
        lineWeldPara.S1_T_Length=0;
        lineWeldPara.S1_speed=10;
        lineWeldPara.S2_Length=3;
        lineWeldPara.S2_T_Length=0;
        lineWeldPara.S2_speed=10;
        lineWeldPara.Dlt_TX=3;
        lineWeldPara.Dlt_TY=3;
        lineWeldPara.Dlt_TZ=2;
        lineWeldPara.T1_Delay_time=100;
        lineWeldPara.T2_Delay_time=20;
        lineWeldPara.T3_Delay_time=0;
        lineWeldPara.v_Over_Time_count=6000000;
        SaveLineWeldToDb(QString("default"),lineWeldPara);
    }
    else
    {
        QStringList tableList=db.tables();
        if(!tableList.contains(QString("CorrectPara"),Qt::CaseInsensitive))
        {
            query.exec(SQL_CREATE_CORRECTPARA_TABLE);
            query.exec(SQL_CREATE_CORRECTPARA_INDEX);
        }
    }
    isOpen=true;
    return true;
}

void SqlDB::closeDB()
{
    db.close();
}

bool SqlDB::ReadProjNameFromDb(QStringList &listProjName)     //读所有的项目名称
{
    QString strSQL=QString("SELECT distinct ProjectName FROM ProjFile");
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(strSQL);
    int count=model->rowCount();
    for(int i=0;i<count;i++)
        listProjName.append(model->record(i).value(0).toString().trimmed());
    delete model;
    model=0;
    return true;
}

bool SqlDB::SaveProjItemToDb(QString projectName,PROJ_ITEM item)//保存焊接项目单条操作内容
{
    //printf("-------SqlDB::SaveProjItemToDb--------\n");
    QSqlQuery query;
    query.prepare("INSERT INTO ProjFile  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(projectName);        //项目名称
    query.addBindValue(item.commNumber);    //操作序号
    query.addBindValue(item.command);       //操作命令
    query.addBindValue(item.technics);      //工艺名称
    query.addBindValue(item.Zh);            //Z提升高度
    query.addBindValue(item.optimize);      //是否优化：1=优化，0=不优化
    query.addBindValue(item.direction);     //镀锡面方向
    for(int i=0;i<8;i++)
        query.addBindValue(item.pointCoordinate1[i]);
    for(int i=0;i<8;i++)
        query.addBindValue(item.pointCoordinate2[i]);

    if(!query.exec())
    {
        //printf("-------UPDATE ProjFile --------\n");
        query.prepare("UPDATE ProjFile SET Command = ?,Technics = ?,Zh = ? ,Optimize = ?,Direction = ?,X=? ,Y=?, Z=?, R=?,S=? ,R1=?, R2=?, E=?, \
                      eX=? ,eY=?, eZ=?, eR=?,eS=? ,eR1=?, eR2=?, eE=? WHERE ProjectName = ? and OperateNum = ?");
        query.addBindValue(item.command);       //操作命令
        query.addBindValue(item.technics);      //工艺名称
        query.addBindValue(item.Zh);            //Z提升高度
        query.addBindValue(item.optimize);      //是否优化：1=优化，0=不优化
        query.addBindValue(item.direction);     //镀锡面方向
        for(int i=0;i<8;i++)
            query.addBindValue(item.pointCoordinate1[i]);
        for(int i=0;i<8;i++)
            query.addBindValue(item.pointCoordinate2[i]);
        query.addBindValue(projectName);        //项目名称
        query.addBindValue(item.commNumber);    //指令序号
        QString ss=query.lastQuery();
        if(!query.exec())
            return false;
    }
    return true;
}

bool SqlDB::ReadProjItemFromDb(QString projectName,int commNumber,PROJ_ITEM &item)//读焊接项目单条操作内容
{
    QSqlQuery query;
    query.prepare("SELECT * FROM ProjFile where ProjectName = ? and OperateNum = ?");
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(commNumber);     //操作序号
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        item.commNumber=commNumber;
        item.command=query.value(2).toString();
        item.technics=query.value(3).toString();
        item.Zh=query.value(4).toDouble();
        item.optimize=query.value(5).toInt();
        item.direction=query.value(6).toString();
        for(int i=0;i<8;i++)
            item.pointCoordinate1[i]=query.value(i+7).toDouble();
        for(int i=0;i<8;i++)
            item.pointCoordinate2[i]=query.value(i+15).toDouble();
        return true;
    }
    else return false;
}

bool SqlDB::SaveProjAllItemToDb(QString projectName,QList <PROJ_ITEM> AllItem)//保存焊接项目所有命令操作内容
{
    //printf("-------SqlDB::SaveProjItemToDb--------\n");
    QSqlQuery query1;
    query1.prepare("delete from ProjFile where ProjectName = ?");
    query1.addBindValue(projectName);    //项目名称
    if(!query1.exec())
        return false;

    int cnt=AllItem.size();
    for(int i=0;i<cnt;i++)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO ProjFile  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(projectName);        //项目名称
        query.addBindValue(AllItem[i].commNumber);    //操作序号
        query.addBindValue(AllItem[i].command);       //操作命令
        query.addBindValue(AllItem[i].technics);      //工艺名称
        query.addBindValue(AllItem[i].Zh);            //Z提升高度
        query.addBindValue(AllItem[i].optimize);      //是否优化：1=优化，0=不优化
        query.addBindValue(AllItem[i].direction);     //镀锡面方向
        for(int j=0;j<8;j++)
            query.addBindValue(AllItem[i].pointCoordinate1[j]);
        for(int j=0;j<8;j++)
            query.addBindValue(AllItem[i].pointCoordinate2[j]);
        if(!query.exec())
            return false;
    }
    return true;
}

bool SqlDB::ReadProjAllItemFromDb(QString projectName,QList <PROJ_ITEM> &AllItem)//读焊接项目所有命令操作内容
{
    bool rtn=false;
    PROJ_ITEM item;
    QSqlQuery query;
    query.prepare("SELECT * FROM ProjFile where ProjectName = ?");
    query.addBindValue(projectName);    //项目名称
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        while(query.isValid())
        {
            item.commNumber=query.value(1).toInt();
            item.command=query.value(2).toString();
            item.technics=query.value(3).toString();
            item.Zh=query.value(4).toDouble();
            item.optimize=query.value(5).toInt();
            item.direction=query.value(6).toString();
            for(int i=0;i<8;i++)
                item.pointCoordinate1[i]=query.value(i+7).toDouble();
            for(int i=0;i<8;i++)
                item.pointCoordinate2[i]=query.value(i+15).toDouble();
            AllItem.append(item);
            query.next();
            rtn = true;
        }
    }
    return rtn;
}

bool SqlDB::DeleteProjItemFromDb(QString projectName,int commNumber) //删除焊接项目某一条
{
    QSqlQuery query;
    query.prepare("DELETE FROM ProjFile where ProjectName = ? and OperateNum = ?");
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(commNumber);     //操作序号
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::DeleteProjectFromDb(QString projectName) //删除整个焊接项目
{
    QSqlQuery query;
    query.prepare("DELETE FROM ProjFile where ProjectName = ?");
    query.addBindValue(projectName);    //项目名称
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::UpdateProjectFromDb(QString oldProjectName,QString newProjectName,QString direction) //更新整个焊接项目的名称、焊锡面朝向
{
    QSqlQuery query;
    query.prepare("UPDATE ProjFile SET ProjectName = ? , Direction = ? where ProjectName = ?");
    query.addBindValue(newProjectName);    //项目名称
    query.addBindValue(direction);      //焊锡面朝向
    query.addBindValue(oldProjectName);    //项目名称
    QString ss=query.lastQuery();
    if(query.exec())
        return true;
    else
        return false;
}
bool SqlDB::UpdateTechnicsFromDb(QString projectName,QString oldTechnics,QString newTechnics,QString command)//将项目中的oldTechnics工艺参数更新为newTechnics工艺参数
{
    QSqlQuery query;
    query.prepare("UPDATE ProjFile SET Technics = ?   where ProjectName = ? and Technics = ? and Command = ?");
    query.addBindValue(newTechnics);    //新工艺名称
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(oldTechnics);    //原来工艺名称
    query.addBindValue(command);        //操作命令command
    QString ss=query.lastQuery();
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::UpdateZhFromDb(QString projectName,double old_Zh,double new_Zh,QString command) //将项目中的old_Zh提升高度更新为new_Zh提升高度
{
    QSqlQuery query;
    query.prepare("UPDATE ProjFile SET Zh = ?   where ProjectName = ? and Zh = ? and Command = ?");
    query.addBindValue(new_Zh);    //新工艺名称
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(old_Zh);    //原来工艺名称
    query.addBindValue(command);        //操作命令command
    if(query.exec())
        return true;
    else
        return false;
}

int SqlDB::getProjItemCount(QString projectName)  //获得焊接项目里操作命令条数
{
    int count=0;
    QString strSQL=QString("SELECT * FROM ProjFile where ProjectName = '%1' ").arg(projectName);
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(strSQL);
    count=model->rowCount();
    delete model;
    model=0;
    return count;
}

bool SqlDB::SavePointWeldToDb(QString pointWeldName,S_P pointWeldPara) //保存点焊工艺参数
{
    QSqlQuery query;
    query.prepare("INSERT INTO PointWeld \
                   VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(pointWeldName);//
    query.addBindValue(pointWeldPara.ZH);
    query.addBindValue(pointWeldPara.atspeed);
    query.addBindValue(pointWeldPara.Dlt_X);
    query.addBindValue(pointWeldPara.Dlt_Y);
    query.addBindValue(pointWeldPara.Dlt_Z);
    query.addBindValue(pointWeldPara.Dlt_speed);
    query.addBindValue(pointWeldPara.FT_Pre_Length);
    query.addBindValue(pointWeldPara.FT_Pre_Speed);
    query.addBindValue(pointWeldPara.FT_Pre_Modle);
    query.addBindValue(pointWeldPara.Soldering_style);
    query.addBindValue(pointWeldPara.ShakeSolder_Length);
    query.addBindValue(pointWeldPara.ShakeSolder_speed);
    query.addBindValue(pointWeldPara.S1_Length);
    query.addBindValue(pointWeldPara.S1_T_Length);
    query.addBindValue(pointWeldPara.S_speed);
    query.addBindValue(pointWeldPara.S2_Length);
    query.addBindValue(pointWeldPara.S2_T_Length);
    query.addBindValue(pointWeldPara.Dlt_TX);
    query.addBindValue(pointWeldPara.Dlt_TY);
    query.addBindValue(pointWeldPara.Dlt_TZ);
    query.addBindValue(pointWeldPara.T1_Delay_time);
    query.addBindValue(pointWeldPara.T2_Delay_time);
    query.addBindValue(pointWeldPara.T3_Delay_time);
    query.addBindValue(pointWeldPara.v_Over_Time_count);
    QString str=query.lastQuery();
    if(!query.exec())
    {
        query.prepare("UPDATE PointWeld SET ZH=?, atspeed=?,Dlt_X=?,Dlt_Y=?,Dlt_Z=?,Dlt_speed=?,FT_Pre_Length=?,FT_Pre_Speed=?,\
                      FT_Pre_Modle=?,Soldering_style=?,ShakeSolder_Length=?,ShakeSolder_speed=?,S1_Length=?,S1_T_Length=?,S_speed=?,S2_Length=?,\
                      S2_T_Length=?,Dlt_TX=?,Dlt_TY=?,Dlt_TZ=?,T1_Delay_time=?,T2_Delay_time=?,T3_Delay_time=?,v_Over_Time_count=? \
                      WHERE Name = ?");
        query.addBindValue(pointWeldPara.ZH);
        query.addBindValue(pointWeldPara.atspeed);
        query.addBindValue(pointWeldPara.Dlt_X);
        query.addBindValue(pointWeldPara.Dlt_Y);
        query.addBindValue(pointWeldPara.Dlt_Z);
        query.addBindValue(pointWeldPara.Dlt_speed);
        query.addBindValue(pointWeldPara.FT_Pre_Length);
        query.addBindValue(pointWeldPara.FT_Pre_Speed);
        query.addBindValue(pointWeldPara.FT_Pre_Modle);
        query.addBindValue(pointWeldPara.Soldering_style);
        query.addBindValue(pointWeldPara.ShakeSolder_Length);
        query.addBindValue(pointWeldPara.ShakeSolder_speed);
        query.addBindValue(pointWeldPara.S1_Length);
        query.addBindValue(pointWeldPara.S1_T_Length);
        query.addBindValue(pointWeldPara.S_speed);
        query.addBindValue(pointWeldPara.S2_Length);
        query.addBindValue(pointWeldPara.S2_T_Length);
        query.addBindValue(pointWeldPara.Dlt_TX);
        query.addBindValue(pointWeldPara.Dlt_TY);
        query.addBindValue(pointWeldPara.Dlt_TZ);
        query.addBindValue(pointWeldPara.T1_Delay_time);
        query.addBindValue(pointWeldPara.T2_Delay_time);
        query.addBindValue(pointWeldPara.T3_Delay_time);
        query.addBindValue(pointWeldPara.v_Over_Time_count);
        query.addBindValue(pointWeldName);//
        str=query.lastQuery();
        if(!query.exec())
            return false;
    }
    return true;
}

bool SqlDB::ReadPointWeldFromDb(QString pointWeldName,S_P &pointWeldPara) //读点焊工艺参数
{
    QSqlQuery query;
    query.prepare("SELECT * FROM PointWeld where Name = ? ");
    query.addBindValue(pointWeldName);
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        pointWeldPara.ZH=query.value(1).toDouble();
        pointWeldPara.atspeed=query.value(2).toDouble();
        pointWeldPara.Dlt_X=query.value(3).toDouble();
        pointWeldPara.Dlt_Y=query.value(4).toDouble();
        pointWeldPara.Dlt_Z=query.value(5).toDouble();
        pointWeldPara.Dlt_speed=query.value(6).toDouble();
        pointWeldPara.FT_Pre_Length=query.value(7).toDouble();
        pointWeldPara.FT_Pre_Speed=query.value(8).toDouble();
        pointWeldPara.FT_Pre_Modle=query.value(9).toUInt();
        pointWeldPara.Soldering_style=query.value(10).toUInt();
        pointWeldPara.ShakeSolder_Length=query.value(11).toDouble();
        pointWeldPara.ShakeSolder_speed=query.value(12).toDouble();
        pointWeldPara.S1_Length=query.value(13).toDouble();
        pointWeldPara.S1_T_Length=query.value(14).toDouble();
        pointWeldPara.S_speed=query.value(15).toDouble();
        pointWeldPara.S2_Length=query.value(16).toDouble();
        pointWeldPara.S2_T_Length=query.value(17).toDouble();
        pointWeldPara.Dlt_TX=query.value(18).toDouble();
        pointWeldPara.Dlt_TY=query.value(19).toDouble();
        pointWeldPara.Dlt_TZ=query.value(20).toDouble();
        pointWeldPara.T1_Delay_time=query.value(21).toUInt();
        pointWeldPara.T2_Delay_time=query.value(22).toUInt();
        pointWeldPara.T3_Delay_time=query.value(23).toUInt();
        pointWeldPara.v_Over_Time_count=query.value(24).toUInt();

        return true;
    }
    else return false;
}

bool SqlDB::ReadPointTechNameFromDb(QStringList &pointWeldName) //读点焊工艺名称
{
    QString strSQL=QString("SELECT Name FROM PointWeld");
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(strSQL);
    int count=model->rowCount();
    if(count==0)
        return false;
    for(int i=0;i<count;i++)
        pointWeldName.append(model->record(i).value(0).toString().trimmed());
    delete model;
    model=0;
    return true;
}

bool SqlDB::DeletePointTechFromDb(QString pointTechnicsName) //删除点焊工艺参数
{
    QSqlQuery query;
    query.prepare("DELETE FROM PointWeld where Name = ? ");
    query.addBindValue(pointTechnicsName);
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::SaveLineWeldToDb(QString lineWeldName,L_P lineWeldPara)   //保存拖焊工艺参数
{
    QSqlQuery query;
    query.prepare("INSERT INTO LineWeld \
                   VALUES ( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(lineWeldName);//
    query.addBindValue(lineWeldPara.ZH);
    query.addBindValue(lineWeldPara.atspeed);
    query.addBindValue(lineWeldPara.Dlt_X);
    query.addBindValue(lineWeldPara.Dlt_Y);
    query.addBindValue(lineWeldPara.Dlt_Z);
    query.addBindValue(lineWeldPara.Dlt_speed);
    query.addBindValue(lineWeldPara.FT_Pre_Length);
    query.addBindValue(lineWeldPara.FT_Pre_Speed);
    query.addBindValue(lineWeldPara.TH_speed);
    query.addBindValue(lineWeldPara.S1_Length);
    query.addBindValue(lineWeldPara.S1_T_Length);
    query.addBindValue(lineWeldPara.S1_speed);
    query.addBindValue(lineWeldPara.S2_Length);
    query.addBindValue(lineWeldPara.S2_T_Length);
    query.addBindValue(lineWeldPara.S2_speed);
    query.addBindValue(lineWeldPara.Dlt_TX);
    query.addBindValue(lineWeldPara.Dlt_TY);
    query.addBindValue(lineWeldPara.Dlt_TZ);
    query.addBindValue(lineWeldPara.T1_Delay_time);
    query.addBindValue(lineWeldPara.T2_Delay_time);
    query.addBindValue(lineWeldPara.T3_Delay_time);
    query.addBindValue(lineWeldPara.v_Over_Time_count);
    if(!query.exec())
    {
        query.prepare("UPDATE LineWeld SET ZH=?, atspeed=?,Dlt_X=?,Dlt_Y=?,Dlt_Z=?,Dlt_speed=?,FT_Pre_Length=?,FT_Pre_Speed=?,\
                      TH_speed=?,S1_Length=?,S1_T_Length=?,S1_speed=?,S2_Length=?,S2_T_Length=?,S2_speed=?,\
                      Dlt_TX=?,Dlt_TY=?,Dlt_TZ=?,T1_Delay_time=?,T2_Delay_time=?,T3_Delay_time=?,v_Over_Time_count=? \
                      WHERE Name = ?");
        query.addBindValue(lineWeldPara.ZH);
        query.addBindValue(lineWeldPara.atspeed);
        query.addBindValue(lineWeldPara.Dlt_X);
        query.addBindValue(lineWeldPara.Dlt_Y);
        query.addBindValue(lineWeldPara.Dlt_Z);
        query.addBindValue(lineWeldPara.Dlt_speed);
        query.addBindValue(lineWeldPara.FT_Pre_Length);
        query.addBindValue(lineWeldPara.FT_Pre_Speed);
        query.addBindValue(lineWeldPara.TH_speed);
        query.addBindValue(lineWeldPara.S1_Length);
        query.addBindValue(lineWeldPara.S1_T_Length);
        query.addBindValue(lineWeldPara.S1_speed);
        query.addBindValue(lineWeldPara.S2_Length);
        query.addBindValue(lineWeldPara.S2_T_Length);
        query.addBindValue(lineWeldPara.S2_speed);
        query.addBindValue(lineWeldPara.Dlt_TX);
        query.addBindValue(lineWeldPara.Dlt_TY);
        query.addBindValue(lineWeldPara.Dlt_TZ);
        query.addBindValue(lineWeldPara.T1_Delay_time);
        query.addBindValue(lineWeldPara.T2_Delay_time);
        query.addBindValue(lineWeldPara.T3_Delay_time);
        query.addBindValue(lineWeldPara.v_Over_Time_count);
        query.addBindValue(lineWeldName);//
        if(!query.exec())
            return false;
    }
    return true;
}

bool SqlDB::ReadLineWeldFromDb(QString lineWeldName,L_P &lineWeldPara) //读拖焊工艺参数
{
    QSqlQuery query;
    query.prepare("SELECT * FROM LineWeld where Name = ? ");
    query.addBindValue(lineWeldName);
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        lineWeldPara.ZH=query.value(1).toDouble();
        lineWeldPara.atspeed=query.value(2).toDouble();
        lineWeldPara.Dlt_X=query.value(3).toDouble();
        lineWeldPara.Dlt_Y=query.value(4).toDouble();
        lineWeldPara.Dlt_Z=query.value(5).toDouble();
        lineWeldPara.Dlt_speed=query.value(6).toDouble();
        lineWeldPara.FT_Pre_Length=query.value(7).toDouble();
        lineWeldPara.FT_Pre_Speed=query.value(8).toDouble();
        lineWeldPara.TH_speed=query.value(9).toDouble();
        lineWeldPara.S1_Length=query.value(10).toDouble();
        lineWeldPara.S1_T_Length=query.value(11).toDouble();
        lineWeldPara.S1_speed=query.value(12).toDouble();
        lineWeldPara.S2_Length=query.value(13).toDouble();
        lineWeldPara.S2_T_Length=query.value(14).toDouble();
        lineWeldPara.S2_speed=query.value(15).toDouble();
        lineWeldPara.Dlt_TX=query.value(16).toDouble();
        lineWeldPara.Dlt_TY=query.value(17).toDouble();
        lineWeldPara.Dlt_TZ=query.value(18).toDouble();
        lineWeldPara.T1_Delay_time=query.value(19).toUInt();
        lineWeldPara.T2_Delay_time=query.value(20).toUInt();
        lineWeldPara.T3_Delay_time=query.value(21).toUInt();
        lineWeldPara.v_Over_Time_count=query.value(22).toUInt();

        return true;
    }
    else return false;
}

bool SqlDB::ReadLineTechNameFromDb(QStringList &lineWeldName) //读拖焊工艺名称
{
    QString strSQL=QString("SELECT Name FROM LineWeld");
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(strSQL);
    int count=model->rowCount();
    if(count==0)
        return false;
    for(int i=0;i<count;i++)
        lineWeldName.append(model->record(i).value(0).toString().trimmed());
    delete model;
    model=0;
    return true;
}

bool SqlDB::DeleteLineTechFromDb(QString lineTechnicsName) //删除拖焊工艺参数
{
    QSqlQuery query;
    query.prepare("DELETE FROM LineWeld where Name = ? ");
    query.addBindValue(lineTechnicsName);
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::SaveClearSolderToDb(QString projectName,CLEAR_SOLDER clearItem)//保存清锡参数
{
    //printf("-------SqlDB::SaveProjItemToDb--------\n");
    QSqlQuery query;
    query.prepare("INSERT INTO Clear  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(projectName);            //项目名称
    query.addBindValue(clearItem.atspeed);      //下降速度(mm/s)
    query.addBindValue(clearItem.Zh);           //提升高度(mm)
    query.addBindValue(clearItem.clear_H);      //清锡高度(mm)
    query.addBindValue(clearItem.clearTime);    //清锡时间(ms)
    for(int i=0;i<8;i++)                        //清锡坐标
        query.addBindValue(clearItem.clearPos[i]);

    if(!query.exec())
    {
        //printf("-------UPDATE ProjFile --------\n");
        query.prepare("UPDATE Clear SET atspeed = ?,Zh = ? ,CLear_H = ?,Delay_time = ?,X=? ,Y=?, Z=?, R=?,S=? ,R1=?, R2=?, E=? WHERE Name = ?");//
        query.addBindValue(clearItem.atspeed);      //下降速度(mm/s)
        query.addBindValue(clearItem.Zh);           //提升高度(mm)
        query.addBindValue(clearItem.clear_H);      //清锡高度(mm)
        query.addBindValue(clearItem.clearTime);    //清锡时间(ms)
        for(int i=0;i<8;i++)
            query.addBindValue(clearItem.clearPos[i]);
        query.addBindValue(projectName);            //项目名称
        if(!query.exec()) return false;
    }
    return true;
}

bool SqlDB::ReadClearSolderFromDb(QString projectName,CLEAR_SOLDER &clearItem)//读焊接项目清锡参数
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Clear where Name = ?");//
    query.addBindValue(projectName);    //项目名称
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        clearItem.projectName=projectName;
        clearItem.atspeed=query.value(1).toDouble();
        clearItem.Zh=query.value(2).toDouble();
        clearItem.clear_H=query.value(3).toDouble();
        clearItem.clearTime=query.value(4).toInt();
        for(int i=0;i<8;i++)
            clearItem.clearPos[i]=query.value(i+5).toDouble();
        return true;
    }
    else return false;
}

bool SqlDB::SaveAllClearItemToDb(QString projectName,QList <CLEAR_SOLDER> AllItem)//保存焊接项目所有命令操作内容
{
    QSqlQuery query1;
    query1.prepare("delete from Clear where Name = ?");
    query1.addBindValue(projectName);    //项目名称
    if(!query1.exec())
        return false;

    int cnt=AllItem.size();
    for(int i=0;i<cnt;i++)
    {
        QSqlQuery query;
        query.prepare("INSERT INTO Clear  VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(projectName);            //项目名称
        query.addBindValue(AllItem[i].atspeed);      //下降速度(mm/s)
        query.addBindValue(AllItem[i].Zh);           //提升高度(mm)
        query.addBindValue(AllItem[i].clear_H);      //清锡高度(mm)
        query.addBindValue(AllItem[i].clearTime);    //清锡时间(ms)
        for(int k=0;k<8;k++)                        //清锡坐标
            query.addBindValue(AllItem[i].clearPos[k]);
        if(!query.exec())
            return false;
    }
    return true;
}

bool SqlDB::ReadAllClearItemFromDb(QString projectName,QList <CLEAR_SOLDER> &AllItem)//读焊接项目所有命令操作内容
{
    bool rtn=false;
    CLEAR_SOLDER clearItem;
    QSqlQuery query;
    query.prepare("SELECT * FROM Clear where Name = ?");
    query.addBindValue(projectName);    //项目名称
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        while(query.isValid())
        {
            clearItem.projectName=projectName;
            clearItem.atspeed=query.value(1).toDouble();
            clearItem.Zh=query.value(2).toDouble();
            clearItem.clear_H=query.value(3).toDouble();
            clearItem.clearTime=query.value(4).toInt();
            for(int i=0;i<8;i++)
                clearItem.clearPos[i]=query.value(i+5).toDouble();
            AllItem.append(clearItem);
            query.next();
            rtn = true;
        }
    }
    return rtn;
}


bool SqlDB::DeleteClearSolderFromDb(QString projectName) //删除焊接项目清锡参数
{
    QSqlQuery query;
    query.prepare("DELETE FROM Clear where Name = ?"); //
    query.addBindValue(projectName);
    projectName.clear();
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::SaveProjLogToDb(QString projectName,int cardCount,int pointCount,int weldTime)//保存产量日志
{
    //printf("-------SqlDB::SaveProjLogToDb--------\n");
    QSqlQuery query;
    query.prepare("INSERT INTO ProjLog  VALUES (?, ?, ?, ?)");
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(cardCount);      //板子数量
    query.addBindValue(pointCount);     //焊点数量
    query.addBindValue(weldTime);       //焊接时间

    if(!query.exec())
    {
        //printf("-------UPDATE ProjLog --------\n");
        query.prepare("UPDATE ProjLog SET CardCount = ? ,PointCount = ?,WeldTime = ?  WHERE Name = ?");
                query.addBindValue(cardCount);      //板子数量
                query.addBindValue(pointCount);     //焊点数量
                query.addBindValue(weldTime);       //焊接时间
                query.addBindValue(projectName);    //项目名称
        if(!query.exec()) return false;
    }
    return true;
}

bool SqlDB::ReadProjLogFromDb(QString projectName,int *cardCount,int *pointCount,int *weldTime)//读产量日志
{
    *cardCount=0;
    *pointCount=0;
    *weldTime=0;
    QSqlQuery query;
    query.prepare("SELECT * FROM ProjLog where Name = ?");
    query.addBindValue(projectName);    //项目名称
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        *cardCount=query.value(1).toDouble();
        *pointCount=query.value(2).toDouble();
        *weldTime=query.value(3).toDouble();
        return true;
    }
    else return false;
}

bool SqlDB::DeleteProjLogFromDb(QString projectName) //删除产量日志
{
    QSqlQuery query;
    query.prepare("DELETE FROM ProjLog where Name = ? ");
    query.addBindValue(projectName);
    if(query.exec())
        return true;
    else
        return false;
}

bool SqlDB::SaveGotoCmdToDb(QString projectName,int commNumber,int gotoNumber)//保存跳转指令参数
{
    //printf("-------SqlDB::SaveGotoCmdToDb--------\n");
    QSqlQuery query;
    query.prepare("INSERT INTO ProjFile  VALUES (?, ?, '跳转', '', 0, ?, '', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)");
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(commNumber);     //操作序号
    //query.addBindValue(item.command);     //操作命令
    //query.addBindValue(item.technics);      //工艺名称
    //query.addBindValue(item.Zh);            //Z提升高度
    query.addBindValue(gotoNumber);     //要跳转的指令序号
    //query.addBindValue(item.direction);     //镀锡面方向
    //for(int i=0;i<8;i++)
    //    query.addBindValue(item.pointCoordinate1[i]);
    //for(int i=0;i<8;i++)
    //    query.addBindValue(item.pointCoordinate2[i]);

    if(!query.exec())
    {
        //printf("-------UPDATE ProjFile --------\n");
        query.prepare("UPDATE ProjFile SET Optimize = ?  WHERE ProjectName = ? and OperateNum = ?");
        query.addBindValue(gotoNumber);     //要跳转的指令序号
        query.addBindValue(projectName);    //项目名称
        query.addBindValue(commNumber);     //指令序号
        if(!query.exec()) return false;
    }
    return true;
}

bool SqlDB::ReadGotoCmdFromDb(QString projectName,int commNumber,int *gotoNumber)//读跳转指令参数
{
    QSqlQuery query;
    query.prepare("SELECT * FROM ProjFile where ProjectName = ? and OperateNum = ?");
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(commNumber);     //操作序号
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        *gotoNumber=query.value(5).toInt();
        return true;
    }
    else return false;
}

bool SqlDB::DeleteGotoCmdFromDb(QString projectName,int commNumber) //删除跳转指令参数
{
    QSqlQuery query;
    query.prepare("DELETE FROM ProjFile where ProjectName = ? and OperateNum = ?");
    query.addBindValue(projectName);    //项目名称
    query.addBindValue(commNumber);     //操作序号
    if(query.exec())
        return true;
    else
        return false;
}

QSqlDatabase SqlDB::getCurrDb()
{
    if(db.isOpen())
        return db;
    else
        return QSqlDatabase();
}

bool SqlDB::SaveMachineToDb(QString id,QString machineModel,int Production,int weldPoint,QString Reserve1,QString Reserve2)     //保存机器信息及产量
{
    QSqlQuery query;
    //printf("-------UPDATE Machine --------\n");
    query.prepare("UPDATE Machine SET Model = ?,Production = ? , WeldPoint = ?,Reserve1 = ? , Reserve2 = ? where ID = ?");
    query.addBindValue(machineModel);   //机器型号
    query.addBindValue(Production);     //机器产量
    query.addBindValue(weldPoint);      //机器焊点
    query.addBindValue(Reserve1);       //保留项1
    query.addBindValue(Reserve2);       //保留项2
    query.addBindValue(id);             //机器编号
    if(!query.exec()) return false;
    return true;
}

bool SqlDB::ReadMachineFromDb(QString &id,QString &machineModel,int *Production,int *weldPoint) //读机器信息及产量
{
    QSqlQuery query;
    query.prepare("SELECT * FROM Machine");
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        id=query.value(0).toString();
        machineModel=query.value(1).toString();
        *Production=query.value(2).toInt();
        *weldPoint=query.value(3).toInt();
        return true;
    }
    else return false;
}
bool SqlDB::SaveEnvParaToDb(QString paraName,ENV_PARA envPara)   //保存工艺环境参数
{
    //printf("-------SqlDB::SaveEnvParaToDb--------\n");
    QSqlQuery query;
    query.prepare("INSERT INTO EnvPara  VALUES (?, ?, ?, ?,?, ?, ?, ?,?, ?, ?, ?)");
    query.addBindValue(paraName);               //工艺参数名称
    //焊盘
    query.addBindValue(envPara.HanPan_HD);      //厚度
    query.addBindValue(envPara.HanPan_BMCL);    //表面处理
    query.addBindValue(envPara.HanPan_FTXS);    //覆铜形式
    query.addBindValue(envPara.HanPan_ZJ);      //直径
    query.addBindValue(envPara.HanPan_KJ);      //孔径
    //焊锡丝
    query.addBindValue(envPara.HanXiSi_ZJ);     //直径
    query.addBindValue(envPara.HanXiSi_ZL);     //种类
    query.addBindValue(envPara.HanXiSi_ZHJ);    //助焊剂含量
    query.addBindValue(envPara.HanXiSi_FW);     //方位
    //烙铁
    query.addBindValue(envPara.LaoTie_WD);      //温度
    query.addBindValue(envPara.LaoTie_ZL);      //种类

    if(!query.exec())
    {
        //printf("-------UPDATE EnvPara --------\n");
        query.prepare("UPDATE EnvPara SET HanPan_HD = ? ,HanPan_BMCL = ?,HanPan_FTXS = ? ,HanPan_ZJ = ?,HanPan_KJ = ?,  \
                      HanXiSi_ZJ = ? ,HanXiSi_ZL = ?,HanXiSi_ZHJ = ? ,HanXiSi_FW = ?, \
                      LaoTie_WD = ? ,LaoTie_ZL = ? WHERE Name = ?");
        //焊盘
        query.addBindValue(envPara.HanPan_HD);      //厚度
        query.addBindValue(envPara.HanPan_BMCL);    //表面处理
        query.addBindValue(envPara.HanPan_FTXS);    //覆铜形式
        query.addBindValue(envPara.HanPan_ZJ);      //直径
        query.addBindValue(envPara.HanPan_KJ);      //孔径
        //焊锡丝
        query.addBindValue(envPara.HanXiSi_ZJ);     //直径
        query.addBindValue(envPara.HanXiSi_ZL);     //种类
        query.addBindValue(envPara.HanXiSi_ZHJ);    //助焊剂含量
        query.addBindValue(envPara.HanXiSi_FW);     //方位
        //烙铁
        query.addBindValue(envPara.LaoTie_WD);      //温度
        query.addBindValue(envPara.LaoTie_ZL);      //种类
        query.addBindValue(paraName);               //工艺参数名称

        if(!query.exec()) return false;
    }
    return true;
}

bool SqlDB::ReadEnvParaFromDb(QString paraName,ENV_PARA &envPara) //读工艺环境参数
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EnvPara where Name = ?");
    query.addBindValue(paraName);    //项目名称
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        envPara.HanPan_HD=query.value(1).toDouble();
        envPara.HanPan_BMCL=query.value(2).toString();
        envPara.HanPan_FTXS=query.value(3).toString();
        envPara.HanPan_ZJ=query.value(4).toDouble();
        envPara.HanPan_KJ=query.value(5).toDouble();

        envPara.HanXiSi_ZJ=query.value(6).toDouble();
        envPara.HanXiSi_ZL=query.value(7).toString();
        envPara.HanXiSi_ZHJ=query.value(8).toDouble();
        envPara.HanXiSi_FW=query.value(9).toString();

        envPara.LaoTie_WD=query.value(10).toDouble();
        envPara.LaoTie_ZL=query.value(11).toString();
        return true;
    }
    else return false;
}

bool SqlDB::DeleteEnvParaFromDb(QString paraName) //删除工艺环境参数
{
    QSqlQuery query;
    query.prepare("DELETE FROM EnvPara where Name = ?");
    query.addBindValue(paraName);    //工艺环境参数名称
    if(query.exec())
        return true;
    else
        return false;
}
bool SqlDB::SaveCorrectParaToDb(QString projName,double Initangle,double Turning_Radius)   //保存旋转校正参数
{
    //printf("-------SqlDB::SaveCorrectParaToDb--------\n");
    QSqlQuery query;
    query.prepare("INSERT INTO CorrectPara  VALUES (?, ?, ?)");
    query.addBindValue(projName);               //工程名称
    query.addBindValue(Initangle);              //烙铁初始角度
    query.addBindValue(Turning_Radius);         //烙铁旋转半径

    if(!query.exec())
    {
        //printf("-------UPDATE CorrectPara --------\n");
        query.prepare("UPDATE CorrectPara SET Initangle = ? ,Turning_Radius = ? WHERE Name = ?");
        query.addBindValue(Initangle);              //烙铁初始角度
        query.addBindValue(Turning_Radius);         //烙铁旋转半径
        query.addBindValue(projName);               //工程名称

        if(!query.exec()) return false;
    }
    return true;
}

bool SqlDB::ReadCorrectParaFromDb(QString projName,double *Initangle,double *Turning_Radius) //读旋转校正参数
{
    QSqlQuery query;
    query.prepare("SELECT * FROM CorrectPara where Name = ?");
    query.addBindValue(projName);    //项目名称
    if(query.exec())
    {
        query.next();
        if(!query.isValid())
            return false;
        *Initangle=query.value(1).toDouble();
        *Turning_Radius=query.value(2).toDouble();
        return true;
    }
    else return false;
}

bool SqlDB::DeleteCorrectParaFromDb(QString projName) //删除旋转校正参数

{
    QSqlQuery query;
    query.prepare("DELETE FROM CorrectPara where Name = ?");
    query.addBindValue(projName);    //工艺环境参数名称
    if(query.exec())
        return true;
    else
        return false;
}
