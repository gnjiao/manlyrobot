#include "global.h"
#include "operatepage.h"
#include "manlyrobot.h"
#include "qlibrary.h"
#include "msqldb.h"
#include "myrwxml.h"

OperatePage::OperatePage(QWidget *parent) :
    QWidget(parent)
{
    robot=NULL;
    weldProject=NULL;
    flagInitFinished=false;
    flagTempStop=0;
    clearProjectFlag=0;
    btnEstopST=0;
    btnStopST=0;
    currCommNumber=0;
}

void OperatePage::initWidget()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MID_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    curPointAxisPos=QVector<double> (8);    //存储当前点的坐标
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=0;

    otracksWidget=new TracksWidget;
    otracksWidget->type=1;
    otracksWidget->initWidget();
    createProjectBox();
    createDataWidget();
    createButtonBox();
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        otracksWidget->setMinimumWidth(screenRect.width()*8/12);
        projectBox->setMaximumWidth(screenRect.width()/6);
        buttonBox->setMaximumWidth(screenRect.width()/6);
    }
    else
    {
        otracksWidget->setMinimumWidth(screenRect.width()*6/12);
        projectBox->setMinimumWidth(screenRect.width()/4);
        buttonBox->setMaximumWidth(screenRect.width()/4);
    }
    //otracksWidget->setMinimumWidth(screenRect.width()*8/12);
    //projectBox->setMinimumWidth(screenRect.width()/5);
    buttonBox->setMaximumHeight(screenRect.height()/10);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(buttonBox);
    rightLayout->addWidget(projectBox);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(otracksWidget);
    layout->addWidget(tableView);
    layout->addLayout(rightLayout);
    setLayout(layout);

    connect(projectComboBox, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(openWeldProject(const QString &)));
    connect(tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(OnTableViewClicked(const QModelIndex &)));

    loadProjectItem();
}

PictureButton *OperatePage::createButton(const QString picturePath, QWidget *receiver,
                                        const char *clickedSlot,const char *pressedPath/* =0 */,const char *pressedSlot/* =0 */)
{
    PictureButton* button=new PictureButton(picturePath,pressedPath);
    button->connect(button, SIGNAL(clicked()), receiver, clickedSlot);
    if(pressedSlot)
        button->connect(button, SIGNAL(pressed()), receiver, pressedSlot);
    return button;
}

void OperatePage::createProjectBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    QLabel *prjLabel=new QLabel(tr("当前项目"));
    prjLabel->setMaximumHeight(screenRect.height()/20);
    prjLabel->setFont(ft);

    ft.setPointSize(LABEL_BUTTON_FONT+6);
    projectComboBox=new QComboBox;
    projectComboBox->addItem(tr("请打开项目"));
    projectComboBox->setCurrentIndex(0);
    projectComboBox->setFont(ft);

    ft.setPointSize(LABEL_BUTTON_FONT+2);
    autoTempCtrlCheck=new QCheckBox(tr("开启自动温控"),this);
    autoTempCtrlCheck->setFont(ft);
    autoTempCtrlCheck->setCheckState(Qt::Unchecked);
    connect(autoTempCtrlCheck, SIGNAL(stateChanged(int)), this, SLOT(OnChangeTempCheckState(int)));
    currentTemp=new QLabel;
    tempLabel=new QLabel(tr("    当前温度  "));
    tempLabel->setFont(ft);
    QHBoxLayout *tempLayout = new QHBoxLayout;
    tempLayout->addWidget(autoTempCtrlCheck);
    tempLayout->addWidget(tempLabel);
    tempLayout->addWidget(currentTemp);

    ft.setPointSize(LABEL_BUTTON_FONT+4);
    startFromCurrentButton=new QPushButton(tr("从所选项开始"));
    startFromCurrentButton->setFont(ft);
    //startFromCurrentButton->setFlat(true);
    connect(startFromCurrentButton, SIGNAL(clicked()), this, SLOT(OnStartFromCurrentBtnClicked()));
    currentButton=new QPushButton(tr("焊接当前项"));
    currentButton->setFont(ft);
    connect(currentButton, SIGNAL(clicked()), this, SLOT(OnCurrentBtnClicked()));

    QPushButton *produceLogLabel=new QPushButton(tr("焊接情况统计"));
    produceLogLabel->setFont(ft);
    produceLogLabel->setMaximumHeight(screenRect.height()/20);
    connect(produceLogLabel, SIGNAL(clicked()), this, SLOT(OnProduceLogClicked()));
    //QLabel *produceLogLabel=new QLabel(tr("焊接情况统计"));
    //produceLogLabel->setFont(ft);
    //produceLogLabel->setMaximumHeight(screenRect.height()/20);
    createProduceLogBox();

    QVBoxLayout *projectLayout = new QVBoxLayout;
    projectLayout->addWidget(prjLabel);
    projectLayout->addWidget(projectComboBox);
    projectLayout->addLayout(tempLayout);
    projectLayout->addSpacing(screenRect.height()/30);
    projectLayout->addWidget(startFromCurrentButton);
    projectLayout->addSpacing(screenRect.height()/40);
    projectLayout->addWidget(currentButton);
    projectLayout->addSpacing(screenRect.height()/20);
    projectLayout->addWidget(produceLogLabel);
    projectLayout->addWidget(produceLogBox);//produceLogLayout
    produceLogBox->setVisible(false);

    projectBox=new QGroupBox;
    projectBox->setLayout(projectLayout);
    projectBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(125,45,200, 255)); border-radius:10px; border:none}");
    projectBox->setContentsMargins(0,0,0,0);
    projectBox->setFlat(true);
}

void OperatePage::createButtonBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_BUTTONS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    //QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect screenRect = desktopWidget->screenGeometry();

    buttonBox=new QGroupBox;
    startButton = createButton(":/button/res/start1.png", this, SLOT(OnStartBtnClicked()),":/button/res/start2.png");
    stopButton = createButton(":/button/res/stop1.png", this, SLOT(OnStopBtnClicked()),":/button/res/stop2.png");
    resetButton = createButton(":/button/res/reset1.png", this, SLOT(OnResetBtnClicked()),":/button/res/reset2.png");

    QLabel* startLable=new QLabel(tr("开始"));
    startLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    startLable->setFont(ft);
    startLable->setPalette(palette);
    QVBoxLayout *startLayout = new QVBoxLayout;
    startLayout->addWidget(startButton);
    startLayout->addWidget(startLable);

    stopLable=new QLabel(tr("暂停"));
    stopLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    stopLable->setFont(ft);
    stopLable->setPalette(palette);
    QVBoxLayout *stopLayout = new QVBoxLayout;
    stopLayout->addWidget(stopButton);
    stopLayout->addWidget(stopLable);

    QLabel* resetLable=new QLabel(tr("复位"));
    resetLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    resetLable->setFont(ft);
    resetLable->setPalette(palette);
    QVBoxLayout *resetLayout = new QVBoxLayout;
    resetLayout->addWidget(resetButton);
    resetLayout->addWidget(resetLable);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(startLayout);
    layout->addStretch();
    layout->addLayout(stopLayout);
    layout->addStretch();
    layout->addLayout(resetLayout);
    buttonBox->setLayout(layout);
    buttonBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(200, 200, 200, 255)); border-radius:10px; border:none}");
    buttonBox->setContentsMargins(0,0,0,0);
    buttonBox->setFlat(true);
}

void OperatePage::createProduceLogBox()
{
    produceLogBox=new QGroupBox;

    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+2);
    ft.setItalic(false);
    //QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect screenRect = desktopWidget->screenGeometry();

    QPushButton *zeroButton=new QPushButton(tr("当前参数清零"));
    zeroButton->setFont(ft);
    connect(zeroButton, SIGNAL(clicked()), this, SLOT(OnZeroBtnClicked()));

    QLabel *outputLabel=new QLabel(tr("当前焊板产量(块)"));
    outputLabel->setFont(ft);
    QLabel *weldedPointCountLabel=new QLabel(tr("当前焊点计数(个)"));
    weldedPointCountLabel->setFont(ft);
    QLabel *weldedTimeLabel=new QLabel(tr("当前焊接时间"));
    weldedTimeLabel->setFont(ft);
    QLabel *myOutputLabel=new QLabel(tr("焊板总数(块)"));
    myOutputLabel->setFont(ft);
    QLabel *myWeldedPointLabel=new QLabel(tr("焊点总数(个)"));
    myWeldedPointLabel->setFont(ft);

    outputEdit=new QLineEdit;
    outputEdit->setFont(ft);
    outputEdit->setFrame(false);
    weldedPointCountEdit=new QLineEdit;
    weldedPointCountEdit->setFont(ft);
    weldedTimeEdit=new QLineEdit;
    weldedTimeEdit->setFont(ft);
    myOutputEdit=new QLineEdit;
    myOutputEdit->setFont(ft);
    myWeldedPointEdit=new QLineEdit;
    myWeldedPointEdit->setFont(ft);

    produceLogLayout = new QGridLayout;
    produceLogLayout->addWidget(outputLabel,0,0);
    produceLogLayout->addWidget(outputEdit,0,1);
    produceLogLayout->addWidget(weldedPointCountLabel,1,0);
    produceLogLayout->addWidget(weldedPointCountEdit,1,1);
    produceLogLayout->addWidget(weldedTimeLabel,2,0);
    produceLogLayout->addWidget(weldedTimeEdit,2,1);
    produceLogLayout->addWidget(myOutputLabel,3,0);
    produceLogLayout->addWidget(myOutputEdit,3,1);
    produceLogLayout->addWidget(myWeldedPointLabel,4,0);
    produceLogLayout->addWidget(myWeldedPointEdit,4,1);
    produceLogLayout->addWidget(zeroButton,5,1);
/*
    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->addSpacing(screenRect.height()/80);
    labelLayout->addWidget(outputLabel);
    labelLayout->addWidget(weldedPointCountLabel);
    labelLayout->addWidget(weldedTimeLabel);
    labelLayout->addWidget(myOutputLabel);
    labelLayout->addWidget(myWeldedPointLabel);
    labelLayout->addSpacing(screenRect.height()/80);

    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(outputEdit);
    editLayout->addWidget(weldedPointCountEdit);
    editLayout->addWidget(weldedTimeEdit);
    editLayout->addWidget(myOutputEdit);
    editLayout->addWidget(myWeldedPointEdit);

    produceLogLayout = new QHBoxLayout;
    produceLogLayout->addLayout(labelLayout);
    produceLogLayout->addLayout(editLayout);
*/
    produceLogBox->setLayout(produceLogLayout);
}

void OperatePage::OnChangeTempCheckState(int state) //改变自动温控检查框的状态
{
    if(state==Qt::Unchecked)
    {
        g_autoTempCtrlState=0;
        //tempLabel->hide();
        //currentTemp->hide();
    }
    else
    {
        g_autoTempCtrlState=1;
        //tempLabel->show();
        //currentTemp->show();
    }
}

void OperatePage::OnProduceLogClicked() //生产日志按钮
{
    if(produceLogBox->isVisible())
        produceLogBox->setVisible(false);
    else
        produceLogBox->setVisible(true);
}
void OperatePage::createDataWidget()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(20,120,120));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    dataModel = new QStandardItemModel(0,3);
    dataModel->setHeaderData(0, Qt::Horizontal, QObject::tr("序号"));
    dataModel->setHeaderData(1, Qt::Horizontal, QObject::tr(" 指令 "));
    dataModel->setHeaderData(2, Qt::Horizontal, QObject::tr("工艺参数"));
    //////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////
    tableView=new QTableView(this);
    tableView->setSortingEnabled(false);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->verticalHeader()->hide();
    tableView->horizontalHeader()->setFont(ft);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);//需要时加滚动条ScrollBarAsNeeded
    //palette.setColor(QPalette::Background, QColor(0,0,0));
    tableView->setPalette(palette);
    tableView->setFont(ft);
    tableView->setModel(dataModel);
    tableView->resizeColumnsToContents();
    tableView->setFrameShape(QFrame::Panel);
    tableView->setShowGrid(true);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setAlternatingRowColors(true);
    tableView->setStyleSheet("QTableView{background-color:rgb(200,200,200) ; alternate-background-color: lightYellow;} \
                             QTableView::item:!selected{background: lightYellow;} \
                             QTableView::item:alternate:!selected{ background: lightBlue; }");
}

void OperatePage::updateCoordinate(F64 *coordinate)
{
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=coordinate[i];

    this->otracksWidget->xEdit->setText(QString("%1").arg(coordinate[0],0,'f',2));
    this->otracksWidget->yEdit->setText(QString("%1").arg(coordinate[1],0,'f',2));
    this->otracksWidget->zEdit->setText(QString("%1").arg(coordinate[2],0,'f',2));
    this->otracksWidget->rEdit->setText(QString("%1").arg(coordinate[3],0,'f',2));
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        otracksWidget->y2Edit->setText(QString("%1").arg(coordinate[7],0,'f',2));
    }
    if((g_MacType==3) || (g_MacType==4) || (g_MacType==1))    //6轴
    {
        otracksWidget->r1Edit->setText(QString("%1").arg(coordinate[5],0,'f',2));
        otracksWidget->r2Edit->setText(QString("%1").arg(coordinate[6],0,'f',2));
    }

    if(weldProject)
    {
        myOutputEdit->setText(QString("%1").arg(flagTempStop)); // 机器产量
        myWeldedPointEdit->setText(QString("%1").arg(weldProject->stopWeldFlag)); // 机器焊点
    }

    otracksWidget->pointColor=otracksWidget->color[2];
    otracksWidget->left_Z_Slider->setValue(curPointAxisPos[2]);
    otracksWidget->down_R_Slider->setValue(curPointAxisPos[3]);

    otracksWidget->point.setX(curPointAxisPos[0]);
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        if(curPointAxisPos[0]>systemPara.limitPosLen[0]/2)    //Y2
            otracksWidget->point.setY(curPointAxisPos[7]);
        else
            otracksWidget->point.setY(curPointAxisPos[1]);
    }
    else
        otracksWidget->point.setY(curPointAxisPos[1]);
    otracksWidget->updateTracks();
}

void OperatePage::OnTableViewClicked(const QModelIndex & index)
{
    if(weldProject)
    {
        if((weldProject->execMode==ROBOT_ALWAYS_EXEC_MODE) && (flagTempStop==0))    //连续执行项目时，禁止按界面项目改变当前命令
            return;
        weldProject->currCommNum=index.row();
        otracksWidget->updateTracks();
    }
}

void OperatePage::openWeldProject(const QString &projectName ) //打开一个项目
{
    if(clearProjectFlag) return;    //如果是由于项目管理对话框而触发，则退出忽略
    if(projectName.isEmpty()) return;
    if(projectName==tr("请选择项目")) return;
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        projectComboBox->setCurrentIndex(0);
        return;
    }

    if(weldProject &&(projectComboBox->currentIndex()>0))   //当前已经打开一个项目
    {
        //if(weldProject->getProjName() != projectName)       //现在正在准备打开的项目与已经打开的项目不是同一个项目
        //{
            delete weldProject;
            weldProject=NULL;
            dataModel->removeRows(0,dataModel->rowCount(),QModelIndex());
        //}
        //else return;
    }
    if((projectComboBox->currentIndex()>0))  //已经选择了一个已经存在的项目(weldProject==NULL) &&
    {
        if(weldProject==NULL)
        {
            weldProject=new WeldProject;
            weldProject->robot=robot;
            weldProject->initConnect();
            connect(weldProject, SIGNAL(currCmdChange(int)), this, SLOT(setCurrCommandItem(int)));
            connect(weldProject, SIGNAL(weldfinished()), this, SLOT(displayProduceLog()));
        }
        weldProject->loadProject(projectName);      //从数据库加载项目
        weldProject->setCurrCommand(0);             //设置当前指令为第0项
        otracksWidget->weldProject=weldProject;     //传递项目对象
        QString strTechnics;
        int count=weldProject->listCommand.size();
        for(int i=0;i<count;i++)
        {
            if(weldProject->optimize[i]==1)
                strTechnics=weldProject->listTechnics[i]+tr(" 优化");
            else
                strTechnics=weldProject->listTechnics[i];
            dataModel->setItem(i, 0, new QStandardItem(QString("%1").arg(i)));
            dataModel->setItem(i, 1, new QStandardItem(weldProject->listCommand[i]));
            dataModel->setItem(i, 2, new QStandardItem(strTechnics));
        }
        tableView->selectRow(0);
        otracksWidget->updateTracks();
        currCommNumber=0;
    }
    tableView->setFocus(Qt::OtherFocusReason);
    unsigned char dirc;
    if(weldProject->getDirection()==QString("left")) dirc=0;
    else if(weldProject->getDirection()==QString("right")) dirc=1;
    else if(weldProject->getDirection()==QString("up")) dirc=2;
    else dirc=3;
    robot->ML_FF_Set_TinFace(dirc);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    int hour=0,mint=0,secd=0;
    mint=weldProject->currWeldTime/60;
    secd=weldProject->currWeldTime%60;
    hour=mint/60;
    if(mint>60) mint=mint%60;
    QString strTime;
    if(hour>0)
        strTime=tr("%1小时%2分%3秒").arg(hour).arg(mint).arg(secd);
    else if(mint>0)
        strTime=tr("%1分%2秒").arg(mint).arg(secd);
    else
        strTime=tr("%1秒").arg(secd);

    outputEdit->setText(QString("%1").arg(weldProject->currCardCnt));
    weldedPointCountEdit->setText(QString("%1").arg(weldProject->currWeldPointCnt));
    weldedTimeEdit->setText(strTime);

    QString id=QString("01"),model=QString("MR");
    int production=0,weldPoint=0;
    if(db.ReadMachineFromDb(id,model,&production,&weldPoint))
    {
        myOutputEdit->setText(QString("%1").arg(production)); //机器产量
        myWeldedPointEdit->setText(QString("%1").arg(weldPoint)); // 机器焊点
    }
    db.closeDB();
}

void OperatePage::OnStartBtnClicked()//开始按钮点击槽函数
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    if(btnEstopST==1)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        return;
    }
    if(robot->isReseting)
    {
        msgBox.setText(tr("机器人正在复位,请稍后操作."));
        msgBox.exec();
        return;
    }


    if(!weldProject)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()<=0)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if((weldProject->execMode==ROBOT_ALWAYS_EXEC_MODE) && (flagTempStop==0))
        return;

    flagTempStop=0;
    weldProject->stopWeldFlag=0;  //允许执行项目
    stopLable->setText(tr("暂停"));
    weldProject->oldWeldPointCnt=weldProject->currWeldPointCnt;

    robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮亮

    weldProject->flagInWelding=1;   //开始进入焊接
    weldProject->execProject(0);    //从首项开始连续执行
    tableView->setFocus(Qt::OtherFocusReason);
}

void OperatePage::OnStopBtnClicked()//停止按钮点击槽函数
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    if(btnEstopST==1)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(robot->isReseting)
    {
        msgBox.setText(tr("机器人正在复位,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(!weldProject)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()<=0)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }

    if(flagTempStop==0)
    {
        if(weldProject->currCommNum==weldProject->listCommand.size()-1)
            return;

        //if(currCommNumber==weldProject->currCommNum) || (weldProject->execMode==ROBOT_STOP_EXEC_MODE)
            //return;
        if((weldProject->currCommNum==0) && (weldProject->execMode!=ROBOT_ALWAYS_EXEC_MODE)) //加载完项目，直接按“启动/停止”按钮,从0开始执行项目
        {
            OnStartBtnClicked();//开始按钮点击槽函数
            return;
        }

        weldProject->stopWeldFlag=1;  //停止执行项目
        weldProject->stopExeProj();   //设置为停止执行
        flagTempStop=1;//停止状态
        stopLable->setText(tr("继续"));
    }
    else
    {
        int index=weldProject->currCommNum+1;
        //if(currCommNumber==index)
        //    return;
        if(index<weldProject->listCommand.size())
        {
            currCommNumber=index;
            //weldProject->currCommNum=index;
            flagTempStop=0;//正在执行状态
            stopLable->setText(tr("暂停"));
            weldProject->stopWeldFlag=0;  //继续执行项目
            robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
            weldProject->continueExeProj(index); //继续执行
        }
    }
    tableView->setFocus(Qt::OtherFocusReason);
}
void OperatePage::OnResetBtnClicked()//复位按钮点击槽函数
{
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    if(btnEstopST==1)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(robot->isReseting)
    {
        msgBox.setText(tr("机器人正在复位,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(weldProject)
    {
        weldProject->execReset();    //复位机器人
    }
    else
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    tableView->setFocus(Qt::OtherFocusReason);
}
void OperatePage::OnStartFromCurrentBtnClicked()//从当前项开始焊接按钮点击槽函数
{
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    if(btnEstopST==1)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(robot->isReseting)
    {
        msgBox.setText(tr("机器人正在复位,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(!weldProject)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()<=0)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if((weldProject->execMode==ROBOT_ALWAYS_EXEC_MODE) && (flagTempStop==0))
        return;

    weldProject->stopWeldFlag=0;  //允许执行项目
    stopLable->setText(tr("暂停"));
    weldProject->oldWeldPointCnt=weldProject->currWeldPointCnt;

    QModelIndex index = tableView->currentIndex();
/*
    if(weldProject->exeChangeIronPose==0)   //未执行过调整姿态命令
    {
        if(weldProject->listCommand[0] != tr("姿态"))
        {
            msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
            msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
            QFont ft;
            ft.setPointSize(MESSAGEBOX_FONT);
            ft.setItalic(false);
            msgBox1.setFont(ft);
            msgBox1.setIcon(QMessageBox::Question);
            msgBox1.setWindowTitle(tr(APPLICATION_NAME));
            msgBox1.addButton(tr("是"), QMessageBox::YesRole);
            msgBox1.addButton(tr("否"), QMessageBox::NoRole);
            msgBox1.setText(tr("您还没有执行调整烙铁姿态命令，是否要执行一次姿态命令?\n如果选“是”，请先执行姿态命令。\n如果选“否”，则立即开始焊接。"));
            if(msgBox1.exec()==QMessageBox::RejectRole)
            {
                weldProject->execProject(index.row());   //连续执行
            }
        }
    }
    else
    {
        weldProject->execProject(index.row());   //连续执行
    }
    */
    robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
    weldProject->execProject(index.row());   //连续执行
    tableView->setFocus(Qt::OtherFocusReason);
}
void OperatePage::OnCurrentBtnClicked()//焊接当前项按钮点击槽函数
{
    QMessageBox msgBox,msgBox1;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    if(btnEstopST==1)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(robot->isReseting)
    {
        msgBox.setText(tr("机器人正在复位,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(!weldProject)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()<=0)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }
    if((weldProject->execMode==ROBOT_ALWAYS_EXEC_MODE) && (flagTempStop==0))
        return;

    weldProject->stopWeldFlag=0;  //允许执行项目
    stopLable->setText(tr("暂停"));
    weldProject->flagInWelding=1;   //开始进入焊接

    QModelIndex index = tableView->currentIndex();
    weldProject->execMode=ROBOT_SINGLE_EXEC_MODE;   //设置为单步执行
    robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮

    weldProject->setCurrCommand(index.row());   //
    robot->ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
    weldProject->execItem();
    tableView->setFocus(Qt::OtherFocusReason);
}

void OperatePage::setCurrCommandItem(int index)    //在指令列表里设置当前命令
{
    if(weldProject)
    {
        if(index<0 || index>=weldProject->listCommand.size())
            index=0;
        tableView->selectRow(index);
        tableView->setFocus(Qt::OtherFocusReason);
        weldProject->currCommNum=index;
        otracksWidget->updateTracks();
    }
}

void OperatePage::receiveIoStatus()             //收到IO状态
{
    if(g_currOperatePage==2)    //当前不是自动操作界面，而是编辑界面
        return;
    if(robot->isReseting)
        return;
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    if((g_sysInput[key_STOP]==1) && (btnStopST==0))    //停止信号输入有效
    {
        btnStopST++;     //stEStop=1,停止按钮按下
    }
    else if((g_sysInput[key_STOP]==0) && (btnStopST==1))
    {
        btnStopST++;     //stEStop=2,停止按钮抬起释放，信号=1
    }
    else if((g_sysInput[key_STOP]==0) && (btnStopST==2))
    {
        btnStopST=0;

        if(!weldProject)
        {
            msgBox.setText(tr("请先打开一个项目."));
            msgBox.exec();
            return;
        }
        if(weldProject->listCommand.size()<=0)
        {
            msgBox.setText(tr("请先打开一个项目."));
            msgBox.exec();
            return;
        }

        if((weldProject->currCommNum==weldProject->listCommand.size()-1) || ((weldProject->currCommNum==0)&&(flagTempStop==0)&&(weldProject->flagInWelding==0)))
        {
            weldProject->execMode=ROBOT_STOP_EXEC_MODE;
            flagTempStop=0;
            OnStartBtnClicked();   //开始按钮点击槽函数
        }
        else
        {
            OnStopBtnClicked();//停止按钮点击槽函数
        }
    }
    if(g_ioStatus.eStop==0)    //急停信号输入有效
    {
        if(btnEstopST==0)
        {
            btnEstopST=1;
            if(weldProject)
            {
                if(weldProject->listCommand.size()>0)
                {
                    flagTempStop=0;
                    weldProject->stopWeldFlag=1;  //停止执行项目
                    weldProject->stopExeProj();  //停止执行项目
                }
            }
        }
        return;
    }
    else
    {
        btnEstopST=0;
    }
/*    if(g_sysInput[key_RESET]==1)    //复位
    {
        if(weldProject)
        {
            if(weldProject->listCommand.size()>0)
            {
                weldProject->stopWeldFlag=1;  //不可以执行项目
                weldProject->stopExeProj();  //停止执行项目
                weldProject->flagInWelding=0;
                flagTempStop=0;//正在执行状态
                stopLable->setText(tr("暂停"));
                //weldProject->setCurrCommand(0);
            }
        }
    }*/
}

void OperatePage::displayProduceLog()   //显示生产日志
{
    int hour=0,mint=0,secd=0;
    mint=weldProject->currWeldTime/60;
    secd=weldProject->currWeldTime%60;
    hour=mint/60;
    if(mint>60) mint=mint%60;
    QString strTime;
    if(hour>0)
        strTime=tr("%1小时%2分%3秒").arg(hour).arg(mint).arg(secd);
    else if(mint>0)
        strTime=tr("%1分%2秒").arg(mint).arg(secd);
    else
        strTime=tr("%1秒").arg(secd);

    outputEdit->setText(QString("%1").arg(weldProject->currCardCnt));
    weldedPointCountEdit->setText(QString("%1").arg(weldProject->currWeldPointCnt));
    weldedTimeEdit->setText(strTime);

    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    QString id=QString("01"),model=QString("MR");
    int production=0,weldPoint=0;
    if(db.ReadMachineFromDb(id,model,&production,&weldPoint))
    {
        myOutputEdit->setText(QString("%1").arg(production)); //机器产量
        myWeldedPointEdit->setText(QString("%1").arg(weldPoint)); // 机器焊点
    }
    db.closeDB();
}

void OperatePage::loadProjectItem()     //装入所有projectComboBox的条目
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
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
    QStringList listProjName;
    listProjName.clear();
    db.ReadProjNameFromDb(listProjName);
    db.closeDB();
    if(!listProjName.isEmpty())
    {
        for(int i=0;i<listProjName.size();i++)
            projectComboBox->addItem(listProjName[i]);  //editParaPage->
    }
}

void OperatePage::loadweldProject()     //装入所有焊接项目到组合框
{
    int index=projectComboBox->currentIndex();
    projectComboBox->clear();
    projectComboBox->addItem(tr("请选择项目"));
    loadProjectItem();
    projectComboBox->setCurrentIndex(index);
}

void OperatePage::clearProject()                //清除打开的项目，使系统成无打开项目状态
{
    clearProjectFlag=1;
    int count=projectComboBox->count();
    for(int i=1;i<count;i++)
    {
        projectComboBox->removeItem(1);
    }
    projectComboBox->setCurrentIndex(0);

    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
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
    QStringList listProjName;
    db.ReadProjNameFromDb(listProjName);
    db.closeDB();
    if(!listProjName.isEmpty())
    {
        for(int i=0;i<listProjName.size();i++)
            projectComboBox->addItem(listProjName[i]);  //
    }
    projectComboBox->setCurrentIndex(0);
    if(weldProject)       //现在有已经打开的项目
    {
        weldProject->clear();
        dataModel->removeRows(0,dataModel->rowCount(),QModelIndex());
    }
    otracksWidget->updateTracks();
    //delete weldProject;
    //weldProject=NULL;
}

void OperatePage::deleteWeldProject()
{
    delete weldProject;
    weldProject=NULL;
}
void OperatePage::stopWeldOperate(int st)
{
    if(g_currOperatePage != 1)  //当前页不是自动操作界面
        return;
    if(st==1)
    {
        if(weldProject)
        {
            weldProject->stopWeldFlag=1;  //停止执行项目
            weldProject->execMode=ROBOT_STOP_EXEC_MODE;
        }
    }
    else if(st==2)
    {
        //setCurrCommandItem(0);
        flagTempStop=0;
        stopLable->setText(tr("暂停"));
    }
}
void OperatePage::OnZeroBtnClicked()    //产量清零
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
    if(!flagInitFinished)
    {
        msgBox.setText(tr("机器人初始化未结束,请稍后操作."));
        msgBox.exec();
        return;
    }

    if(!weldProject)
    {
        msgBox.setText(tr("请先打开一个项目."));
        msgBox.exec();
        return;
    }

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    if(!db.DeleteProjLogFromDb(weldProject->getProjName()))
    {
        db.closeDB();
        msgBox.setText(tr("删除项目产量数据失败."));
        msgBox.exec();
        return;
    }
    if(!db.ReadProjLogFromDb(weldProject->getProjName(),&weldProject->currCardCnt,&weldProject->currWeldPointCnt,&weldProject->currWeldTime))
    {
        //msgBox.setText(tr("从数据库中读取产量日志失败."));
       // msgBox.exec();
        //return;
    }
    displayProduceLog();   //显示生产日志
}
