#include "weldproject.h"
#include "msqldb.h"

WeldProject::WeldProject(QObject *parent) :
    QObject(parent)
{
    curPointPos=QVector<double> (8);    //存储当前点的坐标
    for(int i=0;i<8;i++)
        curPointPos[i]=0;
    //pointsAxisPos.append(curPointPos);
    //listCommand.append("");
    //listTechnics.append("");
    currCommNum=0;
    robot=NULL;
    execMode=0;
    currZh=10.0;
    flagInWelding=0;
    currWeldPointCnt=0;
    currWeldTime=0;
    currCardCnt=0;
    oldWeldPointCnt=0;
    atspeed=0;
    stopWeldFlag=0;
    delayTime=0;
    exeChangeIronPose=0;
    mytime=new QTime;
    mytime->start();
    ironPos[0]=0;
    ironPos[1]=0;
    serialPort=new SerialPort(this);
}

WeldProject::~WeldProject()
{
    listCommand.clear();
    listTechnics.clear();
    Zh.clear();
    optimize.clear();
    curPointPos.clear();
    pointsAxisPos.clear();
    pointsAxisPos2.clear();
    if(mytime)
        delete mytime;
    if(serialPort)
    {
        serialPort->close();
        delete serialPort;
    }
}

void WeldProject::initConnect()
{
    connect(this, &WeldProject::startPointWeld, robot, &ManlyRobot::pointWeld);
    connect(this, &WeldProject::startLineWeld, robot, &ManlyRobot::lineWeld);
    connect(robot, &ManlyRobot::operateFinished, this, &WeldProject::execStatus);
    connect(this, &WeldProject::clearSolderSig, robot, &ManlyRobot::clearSolder);
    connect(this, &WeldProject::moveToBlankPoint, robot, &ManlyRobot::MoveToBlankPoint);
    connect(this, &WeldProject::changeIronPose, robot, &ManlyRobot::ChangeIronPose);
    connect(this, SIGNAL(toOrgSig()), robot, SLOT(moveToOrg()));
    connect(this, SIGNAL(resetSig()), robot, SLOT(resetRobot()));
}

QString WeldProject::getProjName()
{
    return name;
}

void WeldProject::setProjName(QString wName)        //设置焊接项目名称
{
    name=wName;
}

QString WeldProject::getDirection()
{
    return direction;
}

void WeldProject::setDirection(QString direct)  //设置焊点锡面方向
{
    direction=direct;
}

bool WeldProject::loadProject(QString projectName)  //加载项目
{
    setProjName(projectName);

    QMessageBox msgBox;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return false;
    }
    serialPort->initSeialPort();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        if(info.portName()==QString("COM4"))
        {
            serialPort->setSerialPortInfo(info);
            break;
        }
    }
    if(!serialPort->openSeialPort())
    {
        msgBox.setText(tr("温控器串口不能打开."));
        msgBox.exec();
    }
    else
    {
        //设置波特率
        serialPort->setBaudRate(QSerialPort::Baud9600);
        //设置数据位数
        serialPort->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        serialPort->setParity(QSerialPort::NoParity);
        //设置停止位
        serialPort->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        serialPort->readParaCmd();
    }
    int blankItemCnt=0;
    int count=db.getProjItemCount(projectName);
    if(count>0)
    {
        envPrarMap.clear();
        double zh=0;
        PROJ_ITEM item;
        for(int i=0;i<count;i++)
        {
            if(db.ReadProjItemFromDb(projectName,i,item))
            {
                if(item.command==QString(""))
                {
                    blankItemCnt++;
                    continue;
                }
                else
                {
                    optimize.append(item.optimize);
                    listCommand.append(item.command);
                    listTechnics.append(item.technics);
                    Zh.append(item.Zh);
                }
            }
            if(item.command==QString(tr("点焊")))
            {
                for(int k=0;k<8;k++)
                    curPointPos[k]=item.pointCoordinate1[k];
                pointsAxisPos.append(curPointPos);
                pointsAxisPos2.append(curPointPos);
                currPointTechnicsName=item.technics;
                zh=item.Zh;
                direction=item.direction;
                ENV_PARA envItem;
                if(db.ReadEnvParaFromDb(item.technics,envItem))
                    envPrarMap.insert(item.technics,envItem);
            }
            else if(item.command==QString(tr("拖焊"))  || item.command==QString(tr("多拖1"))
                     || item.command==QString(tr("多拖2")) || item.command==QString(tr("多拖3")))
            {
                for(int k=0;k<8;k++)
                    curPointPos[k]=item.pointCoordinate1[k];
                pointsAxisPos2.append(curPointPos);
                for(int k=0;k<4;k++)
                    curPointPos[k]=item.pointCoordinate1[k];
                for(int k=0;k<4;k++)
                    curPointPos[k+4]=item.pointCoordinate2[k];
                pointsAxisPos.append(curPointPos);
                currLineTechnicsName=item.technics;
                direction=item.direction;
                ENV_PARA envItem;
                if(db.ReadEnvParaFromDb(item.technics,envItem))
                    envPrarMap.insert(item.technics,envItem);
            }
            else if(item.command==QString(tr("清锡")))
            {
                CLEAR_SOLDER clearItem;
                if(db.ReadClearSolderFromDb(projectName,clearItem))
                for(int k=0;k<8;k++)
                    curPointPos[k]=clearItem.clearPos[k];
                pointsAxisPos.append(curPointPos);
                pointsAxisPos2.append(curPointPos);
            }
            else if(item.command==QString(tr("空点")) || item.command==QString(tr("姿态")))
            {
                for(int k=0;k<8;k++)
                    curPointPos[k]=item.pointCoordinate1[k];
                pointsAxisPos2.append(curPointPos);
                curPointPos[4]=item.pointCoordinate2[4];    //存放atspeed参数
                if(item.command==QString(tr("姿态")))
                    curPointPos[7]=item.pointCoordinate2[7];    //存放仅执行一次的参数
                pointsAxisPos.append(curPointPos);
            }
            else
            {
                for(int k=0;k<8;k++)
                    curPointPos[k]=0;
                pointsAxisPos.append(curPointPos);
                pointsAxisPos2.append(curPointPos);
            }
        }
        for(int i=0;i<8;i++) curPointPos[i]=0;
        currCommNum=0;
        if(zh==0)
            currZh=10.0;
        else
            currZh=zh;
        if(currPointTechnicsName.isEmpty())
        {
            currPointTechnicsName=QString("default");
            db.ReadPointWeldFromDb(QString("default"),currPointWeldPara);//
        }
        else
        {
            db.ReadPointWeldFromDb(currPointTechnicsName,currPointWeldPara);//
        }
        if(currLineTechnicsName.isEmpty())
        {
            currLineTechnicsName=QString("default");
            db.ReadLineWeldFromDb(QString("default"),currLineWeldPara);
        }
        else
        {
            db.ReadLineWeldFromDb(currLineTechnicsName,currLineWeldPara);//
        }

        if(!db.ReadProjLogFromDb(getProjName(),&currCardCnt,&currWeldPointCnt,&currWeldTime))
        {
            //msgBox.setText(tr("从数据库中读取产量日志失败."));
           // msgBox.exec();
            //return;
        }
        unsigned char dirc;
        if(direction==QString("left")) dirc=0;
        else if(direction==QString("right")) dirc=1;
        else if(direction==QString("up")) dirc=2;
        else dirc=3;
        robot->ML_FF_Set_TinFace(dirc);

        if(blankItemCnt>0)  //如果有空命令
        {
            db.DeleteProjectFromDb(projectName);    //先删除原项目数据
            saveProject();
        }

        return true;
    }
    else
        return false;
}

void WeldProject::execReset()
{
    stopWeldFlag=1;  //不可以执行项目
    stopExeProj();  //停止执行项目
    flagInWelding=0;
    if(!robot->isReseting)
    {
        emit resetSig();
    }
}

void WeldProject::execItem()   //执行项目中的当前项
{
    if(stopWeldFlag==1) return;  //停止执行项目
    if(execMode==ROBOT_STOP_EXEC_MODE) return;  //停止执行项目
    QMessageBox msgBox,msgBox1;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    if(execMode!=ROBOT_STOP_EXEC_MODE)
        emit currCmdChange(currCommNum);    //界面上命令列表改变当前项
    weldTime=0;
    mytime->restart();  //开始计时
    if(listCommand[currCommNum] == QString(tr("点焊")))
    {
        if(db.ReadPointWeldFromDb(listTechnics[currCommNum],g_PointWeldPara))
        {
            if(serialPort->isOpen && g_autoTempCtrlState==1)
            {
                ENV_PARA envItem=envPrarMap.value(listTechnics[currCommNum]);
                if(serialPort->getCurrentTemp() != envItem.LaoTie_WD)
                {
                    serialPort->writeParaCmd(1,envItem.LaoTie_WD,envItem.LaoTie_WD,envItem.LaoTie_WD,0);
                    mySleep(200);
                    int temp=serialPort->getCurrentTemp();
                    int cnt=0;
                    while((temp != envItem.LaoTie_WD) && cnt<300)
                    {
                        serialPort->readParaCmd();
                        mySleep(200);
                        temp=serialPort->getCurrentTemp();
                        cnt++;
                    }
                    if(cnt>=300)
                    {
                        msgBox.setText(tr("温度控制器控温超时!"));
                        msgBox.exec();
                    }
                }
            }
            g_PointWeldPara.x=curPointPos[0];
            g_PointWeldPara.y=curPointPos[1];
            g_PointWeldPara.z=curPointPos[2];
            g_PointWeldPara.r=curPointPos[3];
            g_PointWeldPara.rr1=curPointPos[5];
            g_PointWeldPara.rr2=curPointPos[6];
            g_PointWeldPara.ZH=Zh[currCommNum];
            g_PointWeldPara.Optimize=optimize[currCommNum];
            g_PointWeldPara.v_Over_Time_count=30000;
            db.closeDB();
            robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
            emit startPointWeld();
        }
        else
            db.closeDB();
    }
    else if(listCommand[currCommNum] == QString(tr("拖焊")) || listCommand[currCommNum] == QString(tr("多拖1"))
             || listCommand[currCommNum] == QString(tr("多拖2")) ||listCommand[currCommNum] == QString(tr("多拖3")))
    {
        if(listCommand[currCommNum] == QString(tr("拖焊")))
            g_lineWeldType=0;
        else if(listCommand[currCommNum] == QString(tr("多拖1")))
            g_lineWeldType=1;
        else if(listCommand[currCommNum] == QString(tr("多拖2")))
            g_lineWeldType=2;
        else if(listCommand[currCommNum] == QString(tr("多拖3")))
            g_lineWeldType=3;

        if(db.ReadLineWeldFromDb(listTechnics[currCommNum],g_LineWeldPara))
        {
            if(serialPort->isOpen && g_autoTempCtrlState==1)
            {
                ENV_PARA envItem=envPrarMap.value(listTechnics[currCommNum]);
                if(serialPort->getCurrentTemp() != envItem.LaoTie_WD)
                {
                    serialPort->writeParaCmd(1,envItem.LaoTie_WD,envItem.LaoTie_WD,envItem.LaoTie_WD,0);
                    mySleep(200);
                    int temp=serialPort->getCurrentTemp();
                    int cnt=0;
                    while((temp != envItem.LaoTie_WD) && cnt<300)
                    {
                        serialPort->readParaCmd();
                        mySleep(200);
                        temp=serialPort->getCurrentTemp();
                        cnt++;
                    }
                    if(cnt>=300)
                    {
                        msgBox.setText(tr("温度控制器控温超时!"));
                        msgBox.exec();
                    }
                }
            }
            g_LineWeldPara.x0=curPointPos[0];
            g_LineWeldPara.y0=curPointPos[1];
            g_LineWeldPara.z0=curPointPos[2];
            g_LineWeldPara.r0=curPointPos[3];
            g_LineWeldPara.x1=curPointPos[4];
            g_LineWeldPara.y1=curPointPos[5];
            g_LineWeldPara.z1=curPointPos[6];
            g_LineWeldPara.r1=curPointPos[7];
            g_LineWeldPara.ZH=Zh[currCommNum];
            g_LineWeldPara.Optimize=optimize[currCommNum];
            g_LineWeldPara.v_Over_Time_count=600000;

            if((g_MacType==1) || (g_MacType==3) || (g_MacType==4))
            {
                QString projectName=getProjName();
                PROJ_ITEM item;
                if(db.ReadProjItemFromDb(projectName,currCommNum,item))
                {
                    g_LineWeldPara.rr1=item.pointCoordinate1[5];
                    g_LineWeldPara.rr2=item.pointCoordinate1[6];
                }
            }
            db.closeDB();

            robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
            emit startLineWeld();
        }
        else
            db.closeDB();
    }
    else if(listCommand[currCommNum] == QString(tr("清锡")))
    {
        CLEAR_SOLDER clearItem;
        if(db.ReadClearSolderFromDb(getProjName(),clearItem))
        {
            robot->clearItem.projectName=getProjName();
            robot->clearItem.atspeed=clearItem.atspeed;
            robot->clearItem.Zh=clearItem.Zh;
            robot->clearItem.clear_H=clearItem.clear_H;
            robot->clearItem.clearTime=clearItem.clearTime;
            for(int i=0;i<8;i++)
                robot->clearItem.clearPos[i]=clearItem.clearPos[i];

            if((g_MacType==1) || (g_MacType==3) || (g_MacType==4))
            {
                QString projectName=getProjName();
                CLEAR_SOLDER item;
                if(db.ReadClearSolderFromDb(projectName,item))
                {
                    robot->clearItem.clearPos[5]=item.clearPos[5];
                    robot->clearItem.clearPos[6]=item.clearPos[6];
                }
            }
            db.closeDB();

            robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
            emit clearSolderSig();  //触发清锡功能
        }
        else
            db.closeDB();
    }
    else if(listCommand[currCommNum] == QString(tr("跳转")))
    {
        mySleep(500);
        int gotoNumber=listTechnics[currCommNum].toInt();
        setCurrCommand(gotoNumber);
        robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
        emit currCmdChange(currCommNum);    //界面上命令列表改变当前项
        execStatus(ROBOT_GOTO_END);
    }
    else if(listCommand[currCommNum] == QString(tr("延时")))
    {
        double timeNumber=listTechnics[currCommNum].toDouble();
        unsigned int msec=(unsigned int)(timeNumber*1000);
        mySleep(msec);
        execStatus(ROBOT_DELAY_TIME_END);
    }
    else if(listCommand[currCommNum] == QString(tr("空点")))
    {
        g_PointWeldPara.x=pointsAxisPos[currCommNum][0];
        g_PointWeldPara.y=pointsAxisPos[currCommNum][1];
        g_PointWeldPara.z=pointsAxisPos[currCommNum][2];
        g_PointWeldPara.r=pointsAxisPos[currCommNum][3];
        g_PointWeldPara.rr1=pointsAxisPos[currCommNum][5];
        g_PointWeldPara.rr2=pointsAxisPos[currCommNum][6];
        g_PointWeldPara.ZH=Zh[currCommNum];
        g_PointWeldPara.T3_Delay_time=delayTime;
        g_PointWeldPara.atspeed=atspeed;
        db.closeDB();
        robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
        emit moveToBlankPoint();
    }
    else if(listCommand[currCommNum] == QString(tr("姿态")))
    {
        db.closeDB();
        g_PointWeldPara.x=pointsAxisPos[currCommNum][0];
        g_PointWeldPara.y=pointsAxisPos[currCommNum][1];
        g_PointWeldPara.z=pointsAxisPos[currCommNum][2];
        g_PointWeldPara.r=pointsAxisPos[currCommNum][3];
        g_PointWeldPara.ZH=Zh[currCommNum];
        g_PointWeldPara.T3_Delay_time=delayTime;
        g_PointWeldPara.atspeed=atspeed;
        g_PointWeldPara.S2_Length=pointsAxisPos[currCommNum][5];   //r1坐标
        g_PointWeldPara.S2_T_Length=pointsAxisPos[currCommNum][6]; //r2坐标
        if(pointsAxisPos[currCommNum][7]<2)
        {
            exeChangeIronPose=1;
            emit changeIronPose();  //发出调整烙铁姿态信号
        }
        else
        {
            execStatus(ROBOT_CHANGE_POSE_END);
        }
        if(pointsAxisPos[currCommNum][7]==1)
            pointsAxisPos[currCommNum][7]=2;
    }
    else if(listCommand[currCommNum] == QString(tr("复位")))
    {
        db.closeDB();
        if(!robot->isReseting)
            emit toOrgSig();    //机器回原点
    }
    else if(listCommand[currCommNum].isEmpty()) //命令中的空行处理
    {
        if(currCommNum<listCommand.size()-1)
        {
            setCurrCommand(++currCommNum);
            robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
            emit currCmdChange(currCommNum);    //界面上命令列表改变当前项
            execStatus(ROBOT_GOTO_END);
        }
        else
        {
            execStatus(ROBOT_EMPTY_NOTHING_END);
            db.closeDB();
        }
    }
    else
        db.closeDB();
}

void WeldProject::execProject(int index)         //连续执行项目,从index开始
{
    setCurrCommand(index);              //将index设为当前项
    execMode=ROBOT_ALWAYS_EXEC_MODE;    //设置执行模式为连续执行
    robot->ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
    execItem();
}

void WeldProject::saveProject() //保存当前项目
{
    QMessageBox msgBox;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    int count=listCommand.size();
    PROJ_ITEM item;
    int n=0;
    for(int i=0;i<count;i++)
    {
        item.command=listCommand[i];
        item.technics=listTechnics[i];
        item.Zh=Zh[i];
        item.optimize=optimize[i];
        item.direction=direction;
        if(item.command==QString(tr("点焊")))
        {
            item.commNumber=n;
            for(int k=0;k<8;k++)
            {
                item.pointCoordinate1[k]=pointsAxisPos[i][k];
                item.pointCoordinate2[k]=0;
            }
            //将录入的数据写入数据库
            db.SaveProjItemToDb(name,item);
            n=n+1;
        }
        else if(item.command==QString(tr("拖焊")) || item.command==QString(tr("多拖1"))
                || item.command==QString(tr("多拖2")) || item.command==QString(tr("多拖3")))
        {
            item.commNumber=n;
            for(int k=0;k<4;k++)
            {
                item.pointCoordinate1[k]=pointsAxisPos[i][k];
                item.pointCoordinate2[k]=pointsAxisPos[i][k+4];
            }
            //调出原姿态参数保存
            item.pointCoordinate1[5]=pointsAxisPos2[i][5];
            item.pointCoordinate1[6]=pointsAxisPos2[i][6];
            /*PROJ_ITEM lineItem;
            if(db.ReadProjItemFromDb(getProjName(),i,lineItem))
            {
                item.pointCoordinate1[5]=lineItem.pointCoordinate1[5];
                item.pointCoordinate1[6]=lineItem.pointCoordinate1[6];
            }*/
            //将录入的数据写入数据库
            db.SaveProjItemToDb(name,item);
            n=n+1;
        }
        else if(item.command==QString(tr("清锡")))
        {
            item.commNumber=n;
            for(int k=0;k<8;k++)
            {
                item.pointCoordinate1[k]=pointsAxisPos[i][k];
                item.pointCoordinate2[k]=0;
            }
            //将录入的数据写入数据库
            db.SaveProjItemToDb(name,item);
            n=n+1;
        }
        else if(item.command==QString(tr("空点")) || item.command==QString(tr("姿态")))
        {
            item.commNumber=n;
            item.optimize=delayTime;    //空点delayTime借用optimize参数变量
            for(int k=0;k<8;k++)
            {
                item.pointCoordinate1[k]=pointsAxisPos[i][k];
                item.pointCoordinate2[k]=0;
            }
            item.pointCoordinate2[4]=atspeed;   //空点atspeed借用s坐标参数变量
            if(item.command==QString(tr("姿态")))
            {
                if(currCommNum<listCommand.size())
                {
                    if(pointsAxisPos[currCommNum][7]>0)
                    {
                        pointsAxisPos[currCommNum][7]=1;
                        item.technics=tr("仅执行一次");
                    }
                    else
                    {
                        item.technics=tr("可多次执行");
                    }
                    item.pointCoordinate2[7]=pointsAxisPos[currCommNum][7];    //姿态调整次数借用pointCoordinate2[7]参数变量，1=仅可执行一次；0=可多次执行
                }
            }
            //将录入的数据写入数据库
            db.SaveProjItemToDb(name,item);
            n=n+1;
        }
        else if(((item.command==QString(tr("跳转")))) || (item.command==QString(tr("延时"))) || (item.command==QString(tr("复位"))))
        {
            item.commNumber=n;
            for(int i=0;i<8;i++)
            {
                item.pointCoordinate1[i]=0;   //curPointPos[i]
                item.pointCoordinate2[i]=0;
            }
            //将录入的数据写入数据库
            db.SaveProjItemToDb(name,item);
            n=n+1;
        }
        else //if(!item.command.isEmpty())
        {
            item.commNumber=n;
            for(int k=0;k<8;k++)
            {
                item.pointCoordinate1[k]=0;
                item.pointCoordinate2[k]=0;
            }
            //将录入的数据写入数据库
            db.SaveProjItemToDb(name,item);
            n=n+1;
        }
    }
    db.closeDB();
}

void WeldProject::saveCurrItem()    //保存当前命令条目
{
    if(listCommand[currCommNum].isEmpty())
        return;
    QMessageBox msgBox;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    PROJ_ITEM item;
    item.commNumber=currCommNum;
    item.command=listCommand[currCommNum];
    item.technics=listTechnics[currCommNum];
    item.Zh=Zh[currCommNum];
    item.optimize=optimize[currCommNum];
    item.direction=direction;
    if(((item.command==QString(tr("点焊")))) || (item.command==QString(tr("清锡"))))
    {
        for(int i=0;i<8;i++)
        {
            item.pointCoordinate1[i]=pointsAxisPos[currCommNum][i];   //curPointPos[i]
            item.pointCoordinate2[i]=0;
        }
        //将录入的数据写入数据库
        db.SaveProjItemToDb(name,item);
    }
    else if(item.command==QString(tr("拖焊")) || item.command==QString(tr("多拖1"))
            || item.command==QString(tr("多拖2")) || item.command==QString(tr("多拖3")))
    {
        for(int i=0;i<4;i++)
        {
            item.pointCoordinate1[i]=pointsAxisPos[currCommNum][i];
            item.pointCoordinate2[i]=pointsAxisPos[currCommNum][i+4];
        }
        item.pointCoordinate1[5]=ironPos[0];
        item.pointCoordinate1[6]=ironPos[1];
        //将录入的数据写入数据库
        db.SaveProjItemToDb(name,item);
    }
    else if(item.command==QString(tr("空点")) || item.command==QString(tr("姿态")))
    {
        item.optimize=delayTime;    //空点delayTime借用optimize参数变量
        for(int k=0;k<8;k++)
        {
            item.pointCoordinate1[k]=pointsAxisPos[currCommNum][k];
            item.pointCoordinate2[k]=0;
        }
        item.pointCoordinate2[4]=atspeed;   //空点atspeed借用s坐标参数变量
        if(item.command==QString(tr("姿态")))
        {
            if(currCommNum<listCommand.size())
            {
                if(pointsAxisPos[currCommNum][7]>0)
                {
                    pointsAxisPos[currCommNum][7]=1;
                    item.technics=tr("仅执行一次");
                }
                else
                {
                    item.technics=tr("可多次执行");
                }
                item.pointCoordinate2[7]=pointsAxisPos[currCommNum][7];    //姿态调整次数借用pointCoordinate2[7]参数变量，1=仅可执行一次；0=可多次执行
            }
        }
        //将录入的数据写入数据库
        db.SaveProjItemToDb(name,item);
    }
    else if(((item.command==QString(tr("跳转")))) || (item.command==QString(tr("延时"))) || (item.command==QString(tr("复位"))))
    {
        for(int i=0;i<8;i++)
        {
            item.pointCoordinate1[i]=0;   //curPointPos[i]
            item.pointCoordinate2[i]=0;
        }
        //将录入的数据写入数据库
        db.SaveProjItemToDb(name,item);
    }
    else
    {
        for(int i=0;i<8;i++)
        {
            item.pointCoordinate1[i]=0;
            item.pointCoordinate2[i]=0;
        }
    }
    db.closeDB();
}

void WeldProject::insertCommand(int index)      //在项目中index位置插入一条空指令
{
    if(index<0) return;
    if(index<=listCommand.size())
        listCommand.insert(index,QString(""));
    if(index<=listTechnics.size())
        listTechnics.insert(index,QString(""));
    if(index<=Zh.size())
    {
        Zh.insert(index,0);
    }
    if(index<=optimize.size())
    {
        optimize.insert(index,0);
    }
    if(index<=pointsAxisPos.size())
    {
        QVector<double> pointPos;
        for(int i=0;i<8;i++)
            pointPos.append(0);
        pointsAxisPos.insert(index,pointPos);
        pointsAxisPos2.insert(index,pointPos);
    }

    QMessageBox msgBox;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    db.DeleteProjectFromDb(name);    //先删除原项目数据
    saveProject();  //保存整个新项目
    db.closeDB();
}

void WeldProject::deleteCommand(int index)      //在项目对象中删除一条指令
{
    if((index<0) || (index>=listCommand.size())) return;

    listCommand.removeAt(index);
    listTechnics.removeAt(index);
    Zh.removeAt(index);
    optimize.removeAt(index);
    pointsAxisPos.removeAt(index);
    pointsAxisPos2.removeAt(index);

    QMessageBox msgBox;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    db.DeleteProjectFromDb(name);    //先删除原项目数据
    db.closeDB();
    if(listCommand.size()>0)
        saveProject();  //保存整个新项目
}

void WeldProject::appendCommand()               //在项目对象中最后位置添加一条空指令
{
    listCommand.append(QString(""));
    listTechnics.append(QString(""));
    Zh.append(0);
    optimize.append(0);
    QVector<double> pointPos;
    for(int i=0;i<8;i++)
        pointPos.append(0);
    pointsAxisPos.append(pointPos);
    pointsAxisPos2.append(pointPos);
    currCommNum=listCommand.size()-1;   //listCommand已经是新的了，多了一个元素
}

void WeldProject::setCurrCommand(int index)     //设置当前指令
{
    currCommNum=index;
    if(listCommand[currCommNum] == QString(tr("点焊")))
    {
        currZh=Zh[currCommNum];
        for(int i=0;i<8;i++)
            curPointPos[i]=pointsAxisPos[currCommNum][i];
    }
    else if(listCommand[currCommNum] == QString(tr("拖焊")) || listCommand[currCommNum] == QString(tr("多拖1"))
            || listCommand[currCommNum] == QString(tr("多拖2")) ||listCommand[currCommNum] == QString(tr("多拖3")))
    {
        currZh=Zh[currCommNum];
        for(int i=0;i<8;i++)
            curPointPos[i]=pointsAxisPos[currCommNum][i];
    }
    else if(listCommand[currCommNum] == QString(tr("空点")) || listCommand[currCommNum] == QString(tr("姿态")))
    {
        currZh=Zh[currCommNum];
        atspeed=pointsAxisPos[currCommNum][4];  //空点atspeed借用s坐标参数变量
        delayTime=optimize[currCommNum];  //空点delayTime借用optimize参数变量
    }
    else
    {
        currZh=10;
        for(int i=0;i<8;i++)
            curPointPos[i]=0;
    }
    emit currCmdChange(currCommNum);    //界面上命令列表改变当前项
}

void WeldProject::execStatus(int type)  //对象执行焊接操作的结果状态,type:1=点焊, 2=拖焊 ,3=清锡, 4=复位,5=延时,6=跳转,7=空点,8=回原点,9=定位
{
    //robot->ML_FF_Set_DLL_Return(0);    //设置为不强制退出
    if(stopWeldFlag==1)
    {
        robot->ML_ClearOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯灭
        execMode=ROBOT_STOP_EXEC_MODE;  //停止连续执行
        if(type==ROBOT_RESET_ORG_END)   //是复位命令
        {
            if(listCommand.size()==0)
                return;
            flagInWelding=0;
        }
        return;
    }
    if((execMode==ROBOT_SINGLE_EXEC_MODE) || (execMode==ROBOT_STOP_EXEC_MODE))
    {
        flagInWelding=0;
        robot->ML_ClearOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯灭
        return;
    }
    if(flagInWelding==1)    //如果是正式焊接
    {
        int workTime=mytime->restart();
        weldTime=workTime/1000; //焊接操作所用秒数
        currWeldTime+=weldTime; //当前项目焊接用时
        if((type==ROBOT_POINT_WELD_END) || (type==ROBOT_LINE_WELD_END))
            currWeldPointCnt++;
    }
    if(currCommNum==listCommand.size()-1)   //执行完项目最后一条命令，设置可以送锡(主要针对在编辑界面执行轨迹功能)
    {
        if(type==ROBOT_GOTO_END)
        {
            execItem();
            return;
        }
        stopWeldFlag=1;
        execMode=ROBOT_SINGLE_EXEC_MODE;   //设置为单步执行，即为停止连续执行
        robot->ML_FF_Stop_Tin(0);   //执行完项目最后一条命令，设置可以送锡(主要针对在编辑界面执行轨迹功能)
        if(flagInWelding==1)    //如果是正式焊接,项目焊接完成
        {
            robot->ML_ClearOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯灭
            currCardCnt++;
            /////////////////临时//////////////////////
            //currCardCnt=currCardCnt+12;
            //////////////////////////////////////////
            QMessageBox msgBox;
            //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
            msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
            QFont ft;
            ft.setPointSize(MESSAGEBOX_FONT);
            ft.setItalic(false);
            msgBox.setFont(ft);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setWindowTitle(tr(APPLICATION_NAME));
            msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

            SqlDB db;
            if(!db.openDB("robot.db"))
            {
                msgBox.setText(tr("打开数据库失败."));
                msgBox.exec();
                return;
            }
            if(!db.SaveProjLogToDb(getProjName(),currCardCnt,currWeldPointCnt,currWeldTime))
            {
                db.closeDB();
                msgBox.setText(tr("保存产量数据失败."));
                msgBox.exec();
                return;
            }
            QString id=QString("01"),model=QString("MR");
            int production=0,weldPoint=0;
            if(db.ReadMachineFromDb(id,model,&production,&weldPoint))
            {
                production+=1;
                weldPoint += currWeldPointCnt-oldWeldPointCnt;
            }
            if(!db.SaveMachineToDb(id,model,production,weldPoint))
            {
                msgBox.setText(tr("保存机器产量数据失败."));
                msgBox.exec();
            }
            db.closeDB();
            emit weldfinished();
        }
        else if(flagInWelding==2)    //轨迹模拟焊接完成
        {
            robot->ML_ClearOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯灭
            execMode=ROBOT_STOP_EXEC_MODE;
        }
        flagInWelding=0;
        return;
    }
    if(type==ROBOT_RESET_ORG_END)
    {
        stopWeldFlag=1;  //不可以执行项目
        execMode=ROBOT_STOP_EXEC_MODE;
        flagInWelding=0;
        exeChangeIronPose=0;
        robot->ML_ClearOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯灭
        return;
    }
    if(execMode==ROBOT_ALWAYS_EXEC_MODE)
    {
        if(currCommNum<listCommand.size()-1)
        {
            if((type != ROBOT_GOTO_END) && (type != ROBOT_RESET_ORG_END))
                setCurrCommand(++currCommNum);
            execItem();
        }
    }
}

int WeldProject::getExecMode()
{
    return execMode;
}

void WeldProject::setExecMode(int mode)
{
    execMode=mode;
}

void WeldProject::clear()
{
    serialPort->close();
    int n=curPointPos.count();
    for(int i=0;i<n;i++)
        curPointPos[i]=0;
    currCommNum=0;
    direction.clear();
    setProjName(QString(""));
    execMode=ROBOT_STOP_EXEC_MODE;
    stopWeldFlag=0;
    currZh=10.0;
    flagInWelding=0;
    currWeldPointCnt=0;
    currWeldTime=0;
    currCardCnt=0;
    oldWeldPointCnt=0;
    atspeed=0;
    stopWeldFlag=0;
    delayTime=0;
    listCommand.clear();
    listTechnics.clear();
    Zh.clear();
    optimize.clear();
    pointsAxisPos.clear();
}

void WeldProject::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void WeldProject::stopExeProj()
{
    execMode=ROBOT_STOP_EXEC_MODE;   //设置为停止执行
}

void WeldProject::continueExeProj(int index)
{
    setCurrCommand(index);              //将index设为当前项
    execMode=ROBOT_ALWAYS_EXEC_MODE;    //设置执行模式为连续执行
    robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
    execItem();
}
