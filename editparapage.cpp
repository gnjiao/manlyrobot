#include "global.h"
#include "editparapage.h"
#include "createprojdlg.h"
#include "msqldb.h"
#include "pointwelddlg.h"
#include "linewelddlg.h"
#include "technicsdlg.h"
#include "logicdialog.h"
#include "matrixdlg.h"
#include "movekeydlg.h"
#include "testiodlg.h"
#include "clearsolderdlg.h"
#include "optimizedlg.h"
#include "correctdlg.h"
#include "myrwxml.h"
#include "parawidget.h"

EditParaPage::EditParaPage(QWidget *parent) :
    QWidget(parent)
{
    robot=NULL;
    activeMoveDlg=0;
    weldProject=NULL;
    clearProjectFlag=0;
    flagInitFinished=false;
    activeWeldDlg=0;
    for(int i=0;i<8;i++)
    {
        movetoolMatrixPos[0][i]=0;
        movetoolMatrixPos[1][i]=0;
    }
}

void EditParaPage::initWidget()
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
    pointsAxisPos.append(curPointAxisPos);
    pointsAxisPos.append(curPointAxisPos);

    etracksWidget=new TracksWidget;
    etracksWidget->type=2;
    etracksWidget->initWidget();

    createDataWidget();
    createProjectBox();
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        etracksWidget->setMinimumWidth(screenRect.width()*8/12);
        projectBox->setMaximumWidth(screenRect.width()/6);
    }
    else
    {
        etracksWidget->setMinimumWidth(screenRect.width()*6/12);
        projectBox->setMinimumWidth(screenRect.width()/4);
    }

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(etracksWidget);
    layout->addWidget(tableView);
    layout->addWidget(projectBox);
    setLayout(layout);
    connect(projectComboBox, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(openWeldProject(const QString &)));
    connect(tableView, SIGNAL(clicked(const QModelIndex &)), this, SLOT(OnTableViewClicked(const QModelIndex &)));
    //connect(tableView, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(RowDoubleClicked(int,int)));
    //connect(tableView, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(RowDoubleClicked(const QModelIndex &)));

    logicMenu=new QMenu;
    logicMenu->addAction(QString(tr("延时")),this,SLOT(OnDelayActionClicked()));
    logicMenu->addAction(QString(tr("跳转")),this,SLOT(OnGotoActionClicked()));
    logicMenu->addAction(QString(tr("空点")),this,SLOT(OnBlankActionClicked()));
    logicMenu->addAction(QString(tr("复位")),this,SLOT(OnCondiActionClicked()));
    //if(g_MacType==3 || g_MacType==4 || (g_MacType==1))
    //    logicMenu->addAction(QString(tr("姿态")),this,SLOT(OnIronPoseActionClicked()));

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
    if(!listProjName.isEmpty())
    {
        for(int i=0;i<listProjName.size();i++)
            projectComboBox->addItem(listProjName[i]);  //
    }
}

void EditParaPage::OnTableViewClicked(const QModelIndex & index)
{
    if(weldProject)
    {
        if(weldProject->execMode==ROBOT_ALWAYS_EXEC_MODE)
            return;
        weldProject->currCommNum=index.row();
        etracksWidget->updateTracks();
    }
}

void EditParaPage::RowDoubleClicked(const QModelIndex &index)
{
    if(weldProject)
    {
        weldProject->currCommNum=index.row();
    }
}

void EditParaPage::createProjectBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(EDIT_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT+4);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    QLabel *prjLabel=new QLabel(tr("当前项目"));//
    prjLabel->setMaximumHeight(screenRect.height()/40);
    prjLabel->setFont(ft);
    prjLabel->setPalette(palette);

    moveButton=new QPushButton("<<");
    moveButton->setFont(ft);
    connect(moveButton, SIGNAL(clicked()), this, SLOT(OnMoveBtnClicked()));
    QHBoxLayout *moveLayout = new QHBoxLayout;
    moveLayout->addWidget(prjLabel);
    moveLayout->addStretch();
    moveLayout->addWidget(moveButton);

    ft.setPointSize(LABEL_BUTTON_FONT+6);
    projectComboBox=new QComboBox;
    projectComboBox->addItem(tr("请选择项目"));
    projectComboBox->setCurrentIndex(0);
    projectComboBox->setFont(ft);

    ft.setPointSize(LABEL_BUTTON_FONT+4);
    newProjectButton=new QPushButton(tr("新建项目"));
    newProjectButton->setFont(ft);
    connect(newProjectButton, SIGNAL(clicked()), this, SLOT(OnNewProjectBtnClicked()));

    createParaButtonBox();

    ft.setPointSize(LABEL_BUTTON_FONT+2);

    autoTempCtrlCheck=new QCheckBox(tr("开启自动温控"));
    autoTempCtrlCheck->setFont(ft);
    autoTempCtrlCheck->setCheckState(Qt::Unchecked);
    connect(autoTempCtrlCheck, SIGNAL(stateChanged(int)), this, SLOT(OnChangeTempCheckState(int)));
    currentTemp=new QLabel;
    currentTemp->setFont(ft);
    tempLabel=new QLabel(tr("    当前温度  "));
    tempLabel->setFont(ft);
    QHBoxLayout *tempLayout = new QHBoxLayout;
    tempLayout->addWidget(autoTempCtrlCheck);
    tempLayout->addWidget(tempLabel);
    tempLayout->addWidget(currentTemp);

    QVBoxLayout *projectLayout = new QVBoxLayout;
    projectLayout->addLayout(moveLayout);
    projectLayout->addWidget(projectComboBox);
    projectLayout->addSpacing(screenRect.height()/40);
    projectLayout->addWidget(newProjectButton);
    projectLayout->addLayout(tempLayout);
    projectLayout->addSpacing(screenRect.height()/40);
    projectLayout->addWidget(paraButtonBox);

    projectBox=new QGroupBox;
    projectBox->setLayout(projectLayout);
    projectBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(125,45,200, 255)); border-radius:10px; border:none}");
}

void EditParaPage::OnChangeTempCheckState(int state) //改变自动温控检查框的状态
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

void EditParaPage::OnUseMoveKey(int flag)    //信号触发出现movetool键盘, 1=开启键盘，2=关闭键盘
{
    if(flag==1)
    {
        if(activeMoveDlg==0)    //如果键盘为关闭状态才执行开启
            OnMoveBtnClicked();
    }
    else if(flag==2)
    {
        if(activeMoveDlg==1)    //如果键盘为开启状态才执行关闭
            OnMoveBtnClicked();
    }
}

void EditParaPage::OnMoveBtnClicked()    //屏幕右上角按钮，点击后出现movetool键盘
{
    if(activeMoveDlg==0)
    {
        emit slowlyInOutDlg(1);  //使其它对话框透明
        activeMoveDlg=1;
        moveButton->setText(">>");
        MoveKeyDlg* dlg=new MoveKeyDlg(this);
        dlg->robot=robot;
        dlg->speedValue=robot->speedValue;
        dlg->etracksWidget=etracksWidget;
        connect(this, SIGNAL(stopMoveKey()), dlg, SLOT(closeDlg()));
        //connect(dlg, SIGNAL(readCoordinate()), robot, SLOT(startReadCoordinate()));
        //connect(dlg, SIGNAL(stopCoordinate()), robot, SLOT(stopReadCoordinate()));
        //connect(dlg, SIGNAL(signal_Axis_P_Move(unsigned char ,unsigned char)), robot, SLOT(Axis_P_Move(unsigned char ,unsigned char)));
        //connect(dlg, SIGNAL(signal_Axis_N_Move(unsigned char ,unsigned char)), robot, SLOT(Axis_N_Move(unsigned char ,unsigned char)));
        connect(dlg, SIGNAL(start_Axis_P_Move(unsigned char )), robot, SLOT(start_P_Move(unsigned char)));
        connect(dlg, SIGNAL(start_Axis_N_Move(unsigned char )), robot, SLOT(start_N_Move(unsigned char)));
        connect(dlg, SIGNAL(overLimit(unsigned char ,unsigned char)), robot, SLOT(processLimitValue(unsigned char ,unsigned char)));
        connect(dlg, SIGNAL(stop_Axis_Move(unsigned char,unsigned char)), robot, SLOT(stopMove(unsigned char,unsigned char)));
        connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
        connect(dlg, SIGNAL(slowlyInOutDlg(int)), this, SLOT(execSlowlyInOutDlg(int)));
        QPoint pos;
        pos.setX(0);
        pos.setY(0);
        pos=paraButtonBox->mapToGlobal(pos);
        dlg->resize(paraButtonBox->frameSize()+=QSize(0,10));//
        dlg->beginPointX=pos.x();
        dlg->endPointX=dlg->beginPointX+paraButtonBox->width();
        dlg->flayX=dlg->endPointX;
        dlg->flayY=pos.y()-50;
        dlg->initDialog();
        dlg->move(dlg->flayX,dlg->flayY);//
        dlg->startIn();
        dlg->exec();
    }
    else
    {
        emit stopMoveKey();
        activeMoveDlg=0;
        moveButton->setText("<<");
        emit slowlyInOutDlg(2);  //使其它对话框不透明
    }
}

void EditParaPage::execSlowlyInOutDlg(int flag) //改变所调用对话框的透明度
{
    emit slowlyInOutDlg(flag);
}

void EditParaPage::createParaButtonBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    //QDesktopWidget* desktopWidget = QApplication::desktop();
    //QRect screenRect = desktopWidget->screenGeometry();

    QPushButton *moreSelectButton=new QPushButton(tr("多选调整"));
    moreSelectButton->setFont(ft);
    connect(moreSelectButton, SIGNAL(clicked()), this, SLOT(OnMoreSelectBtnClicked()));

    pointWeldButton=new QPushButton(tr("点焊"));
    pointWeldButton->setFont(ft);
    connect(pointWeldButton, SIGNAL(clicked()), this, SLOT(OnPointWeldBtnClicked()));
    dragWeldButton=new QPushButton(tr("拖焊"));
    dragWeldButton->setFont(ft);
    connect(dragWeldButton, SIGNAL(clicked()), this, SLOT(OnDragWeldBtnClicked()));
    QPushButton *pointTechnicsButton=new QPushButton(tr("点焊工艺"));
    pointTechnicsButton->setFont(ft);
    connect(pointTechnicsButton, SIGNAL(clicked()), this, SLOT(OnPointTechnicsBtnClicked()));
    QPushButton *dragTechnicsButton=new QPushButton(tr("拖焊工艺"));
    dragTechnicsButton->setFont(ft);
    connect(dragTechnicsButton, SIGNAL(clicked()), this, SLOT(OnDragTechnicsBtnClicked()));

    QPushButton *editButton=new QPushButton(tr("编辑"));
    editButton->setFont(ft);
    connect(editButton, SIGNAL(clicked()), this, SLOT(OnEditBtnClicked()));
    QPushButton *undoButton=new QPushButton(tr("添加"));
    undoButton->setFont(ft);
    connect(undoButton, SIGNAL(clicked()), this, SLOT(OnUndoBtnClicked()));
    QPushButton *insertButton=new QPushButton(tr("插入"));
    insertButton->setFont(ft);
    connect(insertButton, SIGNAL(clicked()), this, SLOT(OnInsertBtnClicked()));
    QPushButton *deleteButton=new QPushButton(tr("删除"));
    deleteButton->setFont(ft);
    connect(deleteButton, SIGNAL(clicked()), this, SLOT(OnDeleteBtnClicked()));
    QPushButton *copyButton=new QPushButton(tr("复制"));
    copyButton->setFont(ft);
    connect(copyButton, SIGNAL(clicked()), this, SLOT(OnCopyBtnClicked()));
    QPushButton *pasteButton=new QPushButton(tr("粘贴"));
    pasteButton->setFont(ft);
    connect(pasteButton, SIGNAL(clicked()), this, SLOT(OnPasteBtnClicked()));

    logicButton=new QPushButton(tr("逻辑"));
    logicButton->setFont(ft);
    connect(logicButton, SIGNAL(clicked()), this, SLOT(OnLogicBtnClicked()));
    QPushButton *coordinateButton=new QPushButton(tr("坐标系"));
    coordinateButton->setFont(ft);
    connect(coordinateButton, SIGNAL(clicked()), this, SLOT(OnCoordinateClicked()));
    QPushButton *clearButton=new QPushButton(tr("清锡"));
    clearButton->setFont(ft);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(OnClearBtnClicked()));
    QPushButton *ioButton=new QPushButton(tr("I/O"));
    ioButton->setFont(ft);
    connect(ioButton, SIGNAL(clicked()), this, SLOT(OnIoBtnClicked()));
    arrayButton=new QPushButton(tr("阵列"));
    arrayButton->setFont(ft);
    connect(arrayButton, SIGNAL(clicked()), this, SLOT(OnArrayBtnClicked()));
    QPushButton *optimizeButton=new QPushButton(tr("优化"));
    optimizeButton->setFont(ft);
    connect(optimizeButton, SIGNAL(clicked()), this, SLOT(OnOptimizeBtnClicked()));

    QPushButton *zHeightButton=new QPushButton(tr("Z等高"));
    zHeightButton->setFont(ft);
    connect(zHeightButton, SIGNAL(clicked()), this, SLOT(OnZHeightBtnClicked()));
    QPushButton *locationButton=new QPushButton(tr("定位"));
    locationButton->setFont(ft);
    connect(locationButton, SIGNAL(clicked()), this, SLOT(OnLocationBtnClicked()));
    QPushButton *executeButton=new QPushButton(tr("执行此项"));
    executeButton->setFont(ft);
    connect(executeButton, SIGNAL(clicked()), this, SLOT(OnExecuteBtnClicked()));
    QPushButton *tracksButton=new QPushButton(tr("轨迹"));
    tracksButton->setFont(ft);
    connect(tracksButton, SIGNAL(clicked()), this, SLOT(OnTracksBtnClicked()));
    QPushButton *correctButton=new QPushButton(tr("校正"));
    correctButton->setFont(ft);
    connect(correctButton, SIGNAL(clicked()), this, SLOT(OnCorrectBtnClicked()));

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addSpacing(0);
    pLayout->addWidget(pointWeldButton);    //点焊
    pLayout->addSpacing(0);
    pLayout->addWidget(dragWeldButton);     //拖焊
    pLayout->addSpacing(0);

    QHBoxLayout *tLayout = new QHBoxLayout;
    tLayout->addSpacing(0);
    tLayout->addWidget(pointTechnicsButton);    //点焊工艺
    tLayout->addSpacing(0);
    tLayout->addWidget(dragTechnicsButton);     //拖焊工艺
    tLayout->addSpacing(0);

    QHBoxLayout *euLayout = new QHBoxLayout;
    euLayout->addSpacing(0);
    euLayout->addWidget(editButton);    //编辑
    euLayout->addSpacing(0);
    euLayout->addWidget(undoButton);    //撤销
    euLayout->addSpacing(0);

    QHBoxLayout *idLayout = new QHBoxLayout;
    idLayout->addSpacing(0);
    idLayout->addWidget(insertButton);  //插入
    idLayout->addSpacing(0);
    idLayout->addWidget(deleteButton);  //删除
    idLayout->addSpacing(0);
/*
    QHBoxLayout *cpLayout = new QHBoxLayout;
    cpLayout->addSpacing(0);
    cpLayout->addWidget(copyButton);    //拷贝
    cpLayout->addSpacing(0);
    cpLayout->addWidget(pasteButton);   //粘贴
    cpLayout->addSpacing(0);
*/
    QHBoxLayout *lcLayout = new QHBoxLayout;
    lcLayout->addSpacing(0);
    lcLayout->addWidget(logicButton);    //逻辑
    lcLayout->addSpacing(0);
    lcLayout->addWidget(correctButton);   //校正
    lcLayout->addSpacing(0);

    QHBoxLayout *ciLayout = new QHBoxLayout;
    ciLayout->addSpacing(0);
    ciLayout->addWidget(clearButton);    //清锡
    ciLayout->addSpacing(0);
    ciLayout->addWidget(ioButton);   //I/O
    ciLayout->addSpacing(0);

    QHBoxLayout *aoLayout = new QHBoxLayout;
    aoLayout->addSpacing(0);
    aoLayout->addWidget(arrayButton);    //阵列
    aoLayout->addSpacing(0);
    aoLayout->addWidget(optimizeButton);   //优化
    aoLayout->addSpacing(0);

    QHBoxLayout *zlLayout = new QHBoxLayout;
    zlLayout->addSpacing(0);
    zlLayout->addWidget(zHeightButton);    //Z等高
    zlLayout->addSpacing(0);
    zlLayout->addWidget(locationButton);   //定位
    zlLayout->addSpacing(0);

    QHBoxLayout *etLayout = new QHBoxLayout;
    etLayout->addSpacing(0);
    etLayout->addWidget(executeButton);    //执行此项
    etLayout->addSpacing(0);
    etLayout->addWidget(tracksButton);   //轨迹
    etLayout->addSpacing(0);

    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->addWidget(moreSelectButton);
    btnLayout->addSpacing(0);
    btnLayout->addLayout(pLayout);
    btnLayout->addSpacing(0);
    btnLayout->addLayout(tLayout);
    btnLayout->addSpacing(20);  ///////////////////////////////
    btnLayout->addLayout(euLayout);
    btnLayout->addSpacing(0);
    btnLayout->addLayout(idLayout);
    //btnLayout->addSpacing(0);
    //btnLayout->addLayout(cpLayout);
    btnLayout->addSpacing(20);  ///////////////////////////////
    btnLayout->addLayout(lcLayout);
    btnLayout->addSpacing(0);
    btnLayout->addLayout(ciLayout);
    btnLayout->addSpacing(0);
    btnLayout->addLayout(aoLayout);
    btnLayout->addSpacing(20);  ///////////////////////////////
    btnLayout->addLayout(zlLayout);
    btnLayout->addSpacing(0);
    btnLayout->addLayout(etLayout);
    //btnLayout->addStretch();

    paraButtonBox=new QGroupBox;
    paraButtonBox->setLayout(btnLayout);
}

void EditParaPage::createDataWidget()
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
    //tableView->setDragDropMode(QAbstractItemView::DragDrop);
    //tableView->setDragEnabled(true);
    //tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    //tableView->setAcceptDrops(false);
    //setAcceptDrops(true);
    tableView->setStyleSheet("QTableView{background-color:rgb(200,200,200) ; alternate-background-color: lightYellow;} \
                             QTableView::item:!selected{background: lightYellow;} \
                             QTableView::item:alternate:!selected{ background: lightBlue; }");
}

void EditParaPage::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=coordinate[i];
    etracksWidget->xEdit->setText(QString("%1").arg(coordinate[0],0,'f',2));
    etracksWidget->yEdit->setText(QString("%1").arg(coordinate[1],0,'f',2));
    etracksWidget->zEdit->setText(QString("%1").arg(coordinate[2],0,'f',2));
    etracksWidget->rEdit->setText(QString("%1").arg(coordinate[3],0,'f',2));
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        etracksWidget->y2Edit->setText(QString("%1").arg(coordinate[7],0,'f',2));
    }
    if((g_MacType==3) || (g_MacType==4) || (g_MacType==1))    //6轴
    {
        etracksWidget->r1Edit->setText(QString("%1").arg(coordinate[5],0,'f',2));
        etracksWidget->r2Edit->setText(QString("%1").arg(coordinate[6],0,'f',2));
    }

    etracksWidget->pointColor=etracksWidget->color[2];
    etracksWidget->left_Z_Slider->setValue(curPointAxisPos[2]);
    etracksWidget->down_R_Slider->setValue(curPointAxisPos[3]);
    //etracksWidget->up_R1_Slider->setValue(curPointAxisPos[5]);
    //etracksWidget->right_R2_Slider->setValue(curPointAxisPos[6]);   //ZH 提升高度
    //etracksWidget->point.setX(curPointAxisPos[0]);
    //etracksWidget->point.setY(curPointAxisPos[1]);
    etracksWidget->point.setX(curPointAxisPos[0]);
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        if(curPointAxisPos[0]>systemPara.limitPosLen[0]/2)    //Y2
            etracksWidget->point.setY(curPointAxisPos[7]);
        else
            etracksWidget->point.setY(curPointAxisPos[1]);
    }
    else
        etracksWidget->point.setY(curPointAxisPos[1]);
    etracksWidget->updateTracks();
    emit haveCurrCoordinate(coordinate);

    //lockButton->setDisabled(false);
}

void EditParaPage::receiveIoStatus()             //收到IO状态
{
    if(g_currOperatePage==1)    //当前不是编辑界面，而是自动操作界面
        return;
    if(g_ioStatus.eStop==0)    //急停信号输入有效
    {
        if(weldProject)
        {
            weldProject->stopWeldFlag=1;  //不可以执行项目
            weldProject->execMode=ROBOT_STOP_EXEC_MODE;  //不可以执行项目
            weldProject->flagInWelding=0;
            weldProject->stopExeProj();  //停止执行项目
        }
        return;
    }
/*    if(g_sysInput[key_RESET]==1)    //复位
    {
        if(weldProject)
        {
            weldProject->stopWeldFlag=1;  //不可以执行项目
            weldProject->flagInWelding=0;
            weldProject->stopExeProj();  //停止执行项目
        }
    }*/
}

void EditParaPage::OnNewProjectBtnClicked() //新建项目
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    CreateProjDlg *dlg=new CreateProjDlg(this);
    dlg->move((screenRect.width()-dlg->size().width())/2,30);
    if(dlg->exec()==QDialog::Accepted)      //Rejected
    {
        if(weldProject)
        {
            delete weldProject;
            weldProject=NULL;
        }
        if(dataModel->rowCount()>0)
            dataModel->removeRows(0,dataModel->rowCount(),QModelIndex());
        weldProject=new WeldProject;
        weldProject->robot=robot;
        weldProject->initConnect();
        connect(weldProject, SIGNAL(currCmdChange(int)), this, SLOT(setCurrCommandItem(int)));
        connect(weldProject, SIGNAL(reloadTableSig()), this, SLOT(reloadTable()));
        //weldProject->stopWeldFlag=0;  //允许执行项目
        etracksWidget->weldProject=weldProject;
        projectComboBox->addItem(dlg->projectName);
        projectComboBox->setCurrentText(dlg->projectName);
        weldProject->setProjName(dlg->projectName);
        weldProject->setDirection(dlg->direction);
        weldProject->currCommNum=0;
        weldProject->listCommand.append("");
        weldProject->listTechnics.append("");
        weldProject->Zh.append(0);
        weldProject->optimize.append(0);
        weldProject->pointsAxisPos.append(weldProject->curPointPos);    //存储当前点的坐标
        weldProject->pointsAxisPos2.append(weldProject->curPointPos);    //存储当前点的坐标
        weldProject->currPointTechnicsName=QString("default");
        weldProject->currLineTechnicsName=QString("default");
        dataModel->setItem(0, 0, new QStandardItem(QString("0")));
        tableView->selectRow(0);
        QMessageBox msgBox;
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
        db.ReadPointWeldFromDb(QString("default"),weldProject->currPointWeldPara);
        db.ReadLineWeldFromDb(QString("default"),weldProject->currLineWeldPara);
        tableView->setFocus(Qt::OtherFocusReason);
        unsigned char dirc;
        if(dlg->direction==QString("left")) dirc=0;
        else if(dlg->direction==QString("right")) dirc=1;
        else if(dlg->direction==QString("up")) dirc=2;
        else dirc=3;
        robot->ML_FF_Set_TinFace(dirc);
        db.closeDB();
        etracksWidget->updateTracks();

        if(g_autoTempCtrlState==1)
            weldProject->serialPort->readParaCmd();
    }
    delete dlg;
}

void EditParaPage::openWeldProject(const QString &projectName ) //打开一个项目
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

    if(clearProjectFlag) return;    //如果是由于项目管理对话框而触发，则退出忽略
    if(projectComboBox->currentIndex()==0)
    {
        return;
    }
    if(weldProject)   //当前已经打开一个项目
    {
        //if(weldProject->getProjName() != projectName)       //现在正在准备打开的项目与已经打开的项目不是同一个项目
        //{
            delete weldProject;
            weldProject=NULL;
            dataModel->removeRows(0,dataModel->rowCount(),QModelIndex());
        //}
        //else return;
    }
    if(projectComboBox->currentIndex()==0)
    {
        return;
    }
    if((weldProject==NULL) && (projectComboBox->currentIndex()>0))  //已经选择了一个已经存在的项目
    {
        weldProject=new WeldProject;
        weldProject->robot=robot;
        weldProject->initConnect();
        connect(weldProject, SIGNAL(currCmdChange(int)), this, SLOT(setCurrCommandItem(int)));
        connect(weldProject, SIGNAL(reloadTableSig()), this, SLOT(reloadTable()));
        if(!weldProject->loadProject(projectName))      //从数据库加载项目
            return;
        weldProject->setCurrCommand(0);             //设置当前指令为第0项
        etracksWidget->weldProject=weldProject;     //传递项目对象
        int count=weldProject->listCommand.size();
        QString strTechnics;
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
        etracksWidget->updateTracks();
        //weldProject->stopWeldFlag=0;  //允许执行项目
    }
    if(weldProject)
    {
        tableView->setFocus(Qt::OtherFocusReason);
        unsigned char dirc;
        if(weldProject->getDirection()==QString("left")) dirc=0;
        else if(weldProject->getDirection()==QString("right")) dirc=1;
        else if(weldProject->getDirection()==QString("up")) dirc=2;
        else dirc=3;
        robot->ML_FF_Set_TinFace(dirc);

        if(g_autoTempCtrlState==1)
            weldProject->serialPort->readParaCmd();
    }
}

void EditParaPage::reloadTable()                 //刷新项目命令表
{
    dataModel->removeRows(0,dataModel->rowCount(),QModelIndex());
    int count=weldProject->listCommand.size();
    QString strTechnics;
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
    weldProject->currCommNum=0;
    tableView->selectRow(weldProject->currCommNum);
    weldProject->setCurrCommand(0);             //设置当前指令为第0项
    etracksWidget->updateTracks();
}

void EditParaPage::OnMoreSelectBtnClicked()  //多选调整槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }

    ParaWidget * dlg=new ParaWidget(this);
    dlg->weldProject=weldProject;
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
    tableView->selectRow(weldProject->currCommNum);
    tableView->setFocus(Qt::OtherFocusReason);
    delete dlg;
}

void EditParaPage::OnPointWeldBtnClicked()  //点焊槽函数
{
    if(activeWeldDlg==1) return;
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    else
    {
        if(weldProject->listCommand.size()==0)
            return;
        if(!weldProject->listCommand[weldProject->currCommNum].isEmpty())
        {
            tableView->setFocus(Qt::OtherFocusReason);
            return;
        }
    }

    activeWeldDlg=1;
    PointWeldDlg *dlg=new PointWeldDlg(this);
    dlg->setGeometry(0,0,0,0);
    connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
    connect(dlg, SIGNAL(useMoveKeyDlg(int)), this, SLOT(OnUseMoveKey(int)));
    connect(this, SIGNAL(slowlyInOutDlg(int)), dlg, SLOT(slowlyInOut(int)));

    for(int i=0;i<8;i++)
        dlg->pointPos[i]=weldProject->pointsAxisPos[weldProject->currCommNum][i];
    for(int i=0;i<8;i++)
        dlg->currPointPos[i]=curPointAxisPos[i];
    dlg->weldTechnicsName=weldProject->currPointTechnicsName;
    dlg->Z_h=weldProject->currZh;
    dlg->pointNum=weldProject->currCommNum;
    dlg->type=0;
    dlg->pointWeldPara.atspeed=weldProject->currPointWeldPara.atspeed;
    dlg->pointWeldPara.Dlt_X=weldProject->currPointWeldPara.Dlt_X;
    dlg->pointWeldPara.Dlt_Y=weldProject->currPointWeldPara.Dlt_Y;
    dlg->pointWeldPara.Dlt_Z=weldProject->currPointWeldPara.Dlt_Z;
    dlg->pointWeldPara.Dlt_speed=weldProject->currPointWeldPara.Dlt_speed;
    dlg->pointWeldPara.FT_Pre_Length=weldProject->currPointWeldPara.FT_Pre_Length;
    dlg->pointWeldPara.FT_Pre_Speed=weldProject->currPointWeldPara.FT_Pre_Speed;
    dlg->pointWeldPara.FT_Pre_Modle=weldProject->currPointWeldPara.FT_Pre_Modle;
    dlg->pointWeldPara.Soldering_style=weldProject->currPointWeldPara.Soldering_style;
    dlg->pointWeldPara.ShakeSolder_Length=weldProject->currPointWeldPara.ShakeSolder_Length;
    dlg->pointWeldPara.ShakeSolder_speed=weldProject->currPointWeldPara.ShakeSolder_speed;
    dlg->pointWeldPara.S1_Length=weldProject->currPointWeldPara.S1_Length;
    dlg->pointWeldPara.S1_T_Length=weldProject->currPointWeldPara.S1_T_Length;
    dlg->pointWeldPara.S_speed=weldProject->currPointWeldPara.S_speed;
    dlg->pointWeldPara.S2_Length=weldProject->currPointWeldPara.S2_Length;
    dlg->pointWeldPara.S2_T_Length=weldProject->currPointWeldPara.S2_T_Length;
    dlg->pointWeldPara.Dlt_TX=weldProject->currPointWeldPara.Dlt_TX;
    dlg->pointWeldPara.Dlt_TY=weldProject->currPointWeldPara.Dlt_TY;
    dlg->pointWeldPara.Dlt_TZ=weldProject->currPointWeldPara.Dlt_TZ;
    dlg->pointWeldPara.T1_Delay_time=weldProject->currPointWeldPara.T1_Delay_time;
    dlg->pointWeldPara.T2_Delay_time=weldProject->currPointWeldPara.T2_Delay_time;
    dlg->pointWeldPara.T3_Delay_time=weldProject->currPointWeldPara.T3_Delay_time;
    dlg->pointWeldPara.v_Over_Time_count=weldProject->currPointWeldPara.v_Over_Time_count;
    QPoint pos;
    pos.setX(0);
    pos.setY(0);
    pos=pointWeldButton->mapToGlobal(pos);
    dlg->flayX=pos.x();
    dlg->flayY=pos.y();
    dlg->flayWidth=100;
    dlg->flayHeight=50;
    dlg->initDialog();

    if(dlg->exec()==QDialog::Accepted)
    {
        weldProject->currCommNum=dlg->pointNum;
        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令
        if(activeMoveDlg==1)
            OnMoveBtnClicked();

        //weldProject->currCommNum=index.row();
        weldProject->listCommand[weldProject->currCommNum]=QString(tr("点焊"));
        weldProject->listTechnics[weldProject->currCommNum]=dlg->weldTechnicsName;
        weldProject->Zh[weldProject->currCommNum]=dlg->Z_h;
        for(int i=0;i<8;i++)
        {
            weldProject->curPointPos[i]=dlg->currPointPos[i];
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];
        }

        weldProject->saveCurrItem();    //保存当前命令条目

        QModelIndex index = tableView->currentIndex();
        QVariant data=QVariant(QString(tr("点焊")));
        dataModel->setData(dataModel->index(index.row(),1),data);
        data=QVariant(dlg->weldTechnicsName);
        dataModel->setData(dataModel->index(index.row(),2),data);

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
        etracksWidget->updateTracks();
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
    activeWeldDlg=0;
}

void EditParaPage::OnDragWeldBtnClicked()  //拖焊槽函数
{
    if(activeWeldDlg==1) return;
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    else
    {
        if(weldProject->listCommand.size()==0)
            return;
        if(!weldProject->listCommand[weldProject->currCommNum].isEmpty())
        {
            tableView->setFocus(Qt::OtherFocusReason);
            return;
        }
    }

    activeWeldDlg=1;
    LineWeldDlg *dlg=new LineWeldDlg(this);
    dlg->setGeometry(0,0,0,0);
    connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
    connect(dlg, SIGNAL(useMoveKeyDlg(int)), this, SLOT(OnUseMoveKey(int)));
    connect(this, SIGNAL(slowlyInOutDlg(int)), dlg, SLOT(slowlyInOut(int)));
    dlg->pointNum=weldProject->currCommNum;
    dlg->weldTechnicsName=weldProject->currLineTechnicsName;
    dlg->Z_h=weldProject->currZh;
    dlg->type=0;
    for(int i=0;i<4;i++)
    {
        dlg->movetoolPos[i]=pointsAxisPos[0][i];        //对拖焊来说，此当前坐标为:0~3为起点坐标
        dlg->movetoolPos[i+4]=pointsAxisPos[1][i];      //对拖焊来说，此当前坐标为:4~7为终点坐标
    }
    dlg->lineWeldPara.atspeed=weldProject->currLineWeldPara.atspeed;
    dlg->lineWeldPara.Dlt_X=weldProject->currLineWeldPara.Dlt_X;
    dlg->lineWeldPara.Dlt_Y=weldProject->currLineWeldPara.Dlt_Y;
    dlg->lineWeldPara.Dlt_Z=weldProject->currLineWeldPara.Dlt_Z;
    dlg->lineWeldPara.Dlt_speed=weldProject->currLineWeldPara.Dlt_speed;
    dlg->lineWeldPara.FT_Pre_Length=weldProject->currLineWeldPara.FT_Pre_Length;
    dlg->lineWeldPara.FT_Pre_Speed=weldProject->currLineWeldPara.FT_Pre_Speed;
    dlg->lineWeldPara.TH_speed=weldProject->currLineWeldPara.TH_speed;
    dlg->lineWeldPara.S1_Length=weldProject->currLineWeldPara.S1_Length;
    dlg->lineWeldPara.S1_T_Length=weldProject->currLineWeldPara.S1_T_Length;
    dlg->lineWeldPara.S1_speed=weldProject->currLineWeldPara.S1_speed;
    dlg->lineWeldPara.S2_Length=weldProject->currLineWeldPara.S2_Length;
    dlg->lineWeldPara.S2_T_Length=weldProject->currLineWeldPara.S2_T_Length;
    dlg->lineWeldPara.S2_speed=weldProject->currLineWeldPara.S2_speed;
    dlg->lineWeldPara.Dlt_TX=weldProject->currLineWeldPara.Dlt_TX;
    dlg->lineWeldPara.Dlt_TY=weldProject->currLineWeldPara.Dlt_TY;
    dlg->lineWeldPara.Dlt_TZ=weldProject->currLineWeldPara.Dlt_TZ;
    dlg->lineWeldPara.T1_Delay_time=weldProject->currLineWeldPara.T1_Delay_time;
    dlg->lineWeldPara.T2_Delay_time=weldProject->currLineWeldPara.T2_Delay_time;
    dlg->lineWeldPara.T3_Delay_time=weldProject->currLineWeldPara.T3_Delay_time;
    dlg->lineWeldPara.v_Over_Time_count=weldProject->currLineWeldPara.v_Over_Time_count;

    dlg->ironPos[0]=weldProject->pointsAxisPos2[weldProject->currCommNum][5];
    dlg->ironPos[1]=weldProject->pointsAxisPos2[weldProject->currCommNum][6];
    /*
    SqlDB db;
    if(db.openDB("robot.db"))
    {
        PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
        {
            dlg->ironPos[0]=item.pointCoordinate1[5];
            dlg->ironPos[1]=item.pointCoordinate1[6];
        }
        db.closeDB();
    }*/

    QPoint pos;
    pos.setX(0);
    pos.setY(0);
    pos=dragWeldButton->mapToGlobal(pos);
    dlg->flayX=pos.x();
    dlg->flayY=pos.y();
    dlg->flayWidth=100;
    dlg->flayHeight=50;
    dlg->initDialog();


    if(dlg->exec()==QDialog::Accepted)
    {
        weldProject->currCommNum=dlg->pointNum;
        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        int lineWeldType=dlg->lineWeldType;
        //weldProject->currCommNum=index.row();
        if(lineWeldType==0)
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("拖焊"));
        else if(lineWeldType==1)
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("多拖1"));
        else if(lineWeldType==2)
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("多拖2"));
        else if(lineWeldType==3)
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("多拖3"));
        weldProject->listTechnics[weldProject->currCommNum]=dlg->weldTechnicsName;
        weldProject->Zh[weldProject->currCommNum]=dlg->Z_h;
        for(int i=0;i<8;i++)
        {
            weldProject->curPointPos[i]=dlg->currPointPos[i];
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];//拖焊起点终点坐标xyzr
        }
        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        //姿态参数
        weldProject->ironPos[0]=dlg->ironPos[0];
        weldProject->ironPos[1]=dlg->ironPos[1];
        weldProject->pointsAxisPos2[weldProject->currCommNum][5]=dlg->ironPos[0];
        weldProject->pointsAxisPos2[weldProject->currCommNum][6]=dlg->ironPos[1];

        weldProject->saveCurrItem();    //保存当前命令条目

        QModelIndex index = tableView->currentIndex();
        QVariant data=QVariant(QString(tr("拖焊")));
        if(lineWeldType==0)
            data=QString(tr("拖焊"));
        else if(lineWeldType==1)
            data=QString(tr("多拖1"));
        else if(lineWeldType==2)
            data=QString(tr("多拖2"));
        else if(lineWeldType==3)
            data=QString(tr("多拖3"));
        dataModel->setData(dataModel->index(index.row(),1),data);
        data=QVariant(dlg->weldTechnicsName);
        dataModel->setData(dataModel->index(index.row(),2),data);

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
        etracksWidget->updateTracks();
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
    activeWeldDlg=0;
}

void EditParaPage::OnPointTechnicsBtnClicked()  //点焊工艺槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    TechnicsDlg *dlg=new TechnicsDlg(this);
    dlg->type=POINT_WELD_TECHNICS;      //点焊工艺
    dlg->weldProject=weldProject;
    dlg->initDialog();
    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        delete dlg;
        return;
    }
    QStringList technicsName;
    if(db.ReadPointTechNameFromDb(technicsName))
    {
        for(int i=0;i<technicsName.size();i++)
            dlg->listWidget->addItem(technicsName[i]);
    }
    db.closeDB();
    if(dlg->exec()==QDialog::Accepted)
    {
        weldProject->currPointTechnicsName=dlg->currTechnicsName;
    }
    delete dlg;
}

void EditParaPage::OnDragTechnicsBtnClicked()  //拖焊工艺槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    TechnicsDlg *dlg=new TechnicsDlg(this);
    dlg->type=LINE_WELD_TECHNICS;       //拖焊工艺
    dlg->weldProject=weldProject;
    dlg->initDialog();
    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        delete dlg;
        return;
    }
    QStringList technicsName;
    if(db.ReadLineTechNameFromDb(technicsName))
    {
        for(int i=0;i<technicsName.size();i++)
            dlg->listWidget->addItem(technicsName[i]);
    }
    if(dlg->exec()==QDialog::Accepted)
    {
        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            delete dlg;
            return;
        }
        if(!db.ReadLineWeldFromDb(dlg->currTechnicsName,weldProject->currLineWeldPara))    //加载所选点焊工艺参数
        {
            msgBox.setText(tr("从数据库加载工艺参数失败！"));
            msgBox.exec();
            delete dlg;
            return;
        }
    }
    delete dlg;
}

void EditParaPage::OnEditBtnClicked()  //编辑槽函数
{
    if(activeWeldDlg==1) return;
    if(weldProject)
    {
        if(weldProject->listCommand.size()==0)
            return;
        if(weldProject->listCommand[weldProject->currCommNum].isEmpty())
        {
            tableView->setFocus(Qt::OtherFocusReason);
            return;
        }
    }
    else return;

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

    if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("点焊")))
    {
        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }

        S_P pointWeldPara;
        PROJ_ITEM item;
        QString technicsName;
        if(db.ReadPointWeldFromDb(weldProject->listTechnics[weldProject->currCommNum],pointWeldPara))
            technicsName=weldProject->listTechnics[weldProject->currCommNum];  //工艺名称

        activeWeldDlg=1;
        PointWeldDlg *dlg=new PointWeldDlg(this);
        dlg->setGeometry(0,0,0,0);
        connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
        connect(dlg, SIGNAL(useMoveKeyDlg(int)), this, SLOT(OnUseMoveKey(int)));
        connect(this, SIGNAL(slowlyInOutDlg(int)), dlg, SLOT(slowlyInOut(int)));
        for(int i=0;i<8;i++)
            dlg->pointPos[i]=weldProject->pointsAxisPos[weldProject->currCommNum][i];
        for(int i=0;i<8;i++)
            dlg->currPointPos[i]=curPointAxisPos[i];
        dlg->type=1;    //编辑已有焊点
        dlg->weldTechnicsName=technicsName;
        dlg->pointNum=weldProject->currCommNum;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
            dlg->Z_h=item.Zh;
        dlg->pointWeldPara.atspeed=pointWeldPara.atspeed;
        dlg->pointWeldPara.Dlt_X=pointWeldPara.Dlt_X;
        dlg->pointWeldPara.Dlt_Y=pointWeldPara.Dlt_Y;
        dlg->pointWeldPara.Dlt_Z=pointWeldPara.Dlt_Z;
        dlg->pointWeldPara.Dlt_speed=pointWeldPara.Dlt_speed;
        dlg->pointWeldPara.FT_Pre_Length=pointWeldPara.FT_Pre_Length;
        dlg->pointWeldPara.FT_Pre_Speed=pointWeldPara.FT_Pre_Speed;
        dlg->pointWeldPara.FT_Pre_Modle=pointWeldPara.FT_Pre_Modle;
        dlg->pointWeldPara.Soldering_style=pointWeldPara.Soldering_style;
        dlg->pointWeldPara.ShakeSolder_Length=pointWeldPara.ShakeSolder_Length;
        dlg->pointWeldPara.ShakeSolder_speed=pointWeldPara.ShakeSolder_speed;
        dlg->pointWeldPara.S1_Length=pointWeldPara.S1_Length;
        dlg->pointWeldPara.S1_T_Length=pointWeldPara.S1_T_Length;
        dlg->pointWeldPara.S_speed=pointWeldPara.S_speed;
        dlg->pointWeldPara.S2_Length=pointWeldPara.S2_Length;
        dlg->pointWeldPara.S2_T_Length=pointWeldPara.S2_T_Length;
        dlg->pointWeldPara.Dlt_TX=pointWeldPara.Dlt_TX;
        dlg->pointWeldPara.Dlt_TY=pointWeldPara.Dlt_TY;
        dlg->pointWeldPara.Dlt_TZ=pointWeldPara.Dlt_TZ;
        dlg->pointWeldPara.T1_Delay_time=pointWeldPara.T1_Delay_time;
        dlg->pointWeldPara.T2_Delay_time=pointWeldPara.T2_Delay_time;
        dlg->pointWeldPara.T3_Delay_time=pointWeldPara.T3_Delay_time;
        dlg->pointWeldPara.v_Over_Time_count=pointWeldPara.v_Over_Time_count;
        QPoint pos;
        pos.setX(0);
        pos.setY(0);
        pos=pointWeldButton->mapToGlobal(pos);
        dlg->flayX=pos.x();
        dlg->flayY=pos.y();
        dlg->flayWidth=100;
        dlg->flayHeight=50;
        dlg->initDialog();
        QStringList pointWeldName;
        if(db.ReadPointTechNameFromDb(pointWeldName))
        {
            int currIndex=-1;
            for(int i=0;i<pointWeldName.size();i++)
            {
                dlg->weldTechnicsComboBox->addItem(pointWeldName[i]);
                if(technicsName==pointWeldName[i])
                    currIndex=i;
            }
            dlg->weldTechnicsComboBox->setCurrentIndex(currIndex);
        }
        db.closeDB();
        if(dlg->exec()==QDialog::Accepted)
        {
            weldProject->currCommNum=dlg->pointNum;
            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            if(activeMoveDlg==1)
                OnMoveBtnClicked();

            //weldProject->currCommNum=index.row();
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("点焊"));
            weldProject->listTechnics[weldProject->currCommNum]=dlg->weldTechnicsName;
            weldProject->Zh[weldProject->currCommNum]=dlg->Z_h;
            for(int i=0;i<8;i++)
            {
                weldProject->curPointPos[i]=dlg->currPointPos[i];
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];
            }

            weldProject->saveCurrItem();    //保存当前命令条目updateTechnicsFromDb

            QModelIndex index = tableView->currentIndex();
            QVariant data=QVariant(QString(tr("点焊")));
            dataModel->setData(dataModel->index(index.row(),1),data);
            data=QVariant(dlg->weldTechnicsName);
            dataModel->setData(dataModel->index(index.row(),2),data);

            if(dlg->appToSameTechnicsChecked)
            {
                SqlDB db;
                if(!db.openDB("robot.db"))
                {
                    msgBox.setText(tr("打开数据库失败."));
                    msgBox.exec();
                    return;
                }
                if(!db.UpdateTechnicsFromDb(weldProject->getProjName(),dlg->old_WeldTechnicsName,dlg->weldTechnicsName,tr("点焊")))
                {
                    msgBox.setText(tr("更新同类工艺参数失败."));
                    msgBox.exec();
                    db.closeDB();
                    return;
                }
                if(!db.UpdateZhFromDb(weldProject->getProjName(),dlg->old_Z_h,dlg->Z_h,tr("点焊")))
                {
                    msgBox.setText(tr("更新同类提升高度失败."));
                    msgBox.exec();
                    db.closeDB();
                    return;
                }
                int cnt=weldProject->listCommand.size();
                for(int i=0;i<cnt;i++)
                {
                    PROJ_ITEM item;
                    if(db.ReadProjItemFromDb(weldProject->getProjName(),i,item))
                    {
                        QString strTechnics;
                        if(weldProject->optimize[i]==1)
                            strTechnics=item.technics.append(tr(" 优化"));
                        else
                            strTechnics=item.technics;
                        data=QVariant(strTechnics);
                        dataModel->setData(dataModel->index(i,2),data);
                    }
                    weldProject->listTechnics[i]=item.technics;
                }
                db.closeDB();
            }
            /*
            if(weldProject->currCommNum==dataModel->rowCount()-1)
            {
                dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
                weldProject->currCommNum+=1;
                weldProject->appendCommand();
            }
            */
            tableView->selectRow(weldProject->currCommNum);//
            etracksWidget->updateTracks();
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
        activeWeldDlg=0;
    }
    else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("拖焊")) || weldProject->listCommand[weldProject->currCommNum] == QString(tr("多拖1"))
            || weldProject->listCommand[weldProject->currCommNum] == QString(tr("多拖2")) ||weldProject->listCommand[weldProject->currCommNum] == QString(tr("多拖3")))
    {
        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }
        L_P lineWeldPara;
        QString technicsName;
        if(db.ReadLineWeldFromDb(weldProject->listTechnics[weldProject->currCommNum],lineWeldPara))
            technicsName=weldProject->listTechnics[weldProject->currCommNum];  //工艺名称

        activeWeldDlg=1;
        LineWeldDlg *dlg=new LineWeldDlg(this);
        //烙铁姿态
        dlg->ironPos[0]=weldProject->pointsAxisPos2[weldProject->currCommNum][5];
        dlg->ironPos[1]=weldProject->pointsAxisPos2[weldProject->currCommNum][6];
        /*PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
        {
            dlg->ironPos[0]=item.pointCoordinate1[5];
            dlg->ironPos[1]=item.pointCoordinate1[6];
        }*/

        if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("拖焊")))
            dlg->lineWeldType=0;
        else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("多拖1")))
            dlg->lineWeldType=1;
        else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("多拖2")))
            dlg->lineWeldType=2;
        else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("多拖3")))
            dlg->lineWeldType=3;
        dlg->setGeometry(0,0,0,0);
        connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
        connect(dlg, SIGNAL(useMoveKeyDlg(int)), this, SLOT(OnUseMoveKey(int)));
        connect(this, SIGNAL(slowlyInOutDlg(int)), dlg, SLOT(slowlyInOut(int)));

        for(int i=0;i<8;i++)
            dlg->pointPos[i]=weldProject->pointsAxisPos[weldProject->currCommNum][i];   //所编辑条目的起点终点坐标
        for(int i=0;i<8;i++)
            dlg->currPointPos[i]=curPointAxisPos[i];    //当前烙铁位置
        dlg->type=1;    //编辑已有焊点
        dlg->weldTechnicsName=technicsName;
        dlg->pointNum=weldProject->currCommNum;
        dlg->Z_h=weldProject->Zh[weldProject->currCommNum];
        dlg->lineWeldPara.atspeed=lineWeldPara.atspeed;
        dlg->lineWeldPara.Dlt_X=lineWeldPara.Dlt_X;
        dlg->lineWeldPara.Dlt_Y=lineWeldPara.Dlt_Y;
        dlg->lineWeldPara.Dlt_Z=lineWeldPara.Dlt_Z;
        dlg->lineWeldPara.Dlt_speed=lineWeldPara.Dlt_speed;
        dlg->lineWeldPara.FT_Pre_Length=lineWeldPara.FT_Pre_Length;
        dlg->lineWeldPara.FT_Pre_Speed=lineWeldPara.FT_Pre_Speed;
        dlg->lineWeldPara.TH_speed=lineWeldPara.TH_speed;
        dlg->lineWeldPara.S1_Length=lineWeldPara.S1_Length;
        dlg->lineWeldPara.S1_T_Length=lineWeldPara.S1_T_Length;
        dlg->lineWeldPara.S1_speed=lineWeldPara.S1_speed;
        dlg->lineWeldPara.S2_Length=lineWeldPara.S2_Length;
        dlg->lineWeldPara.S2_T_Length=lineWeldPara.S2_T_Length;
        dlg->lineWeldPara.S2_speed=lineWeldPara.S2_speed;
        dlg->lineWeldPara.Dlt_TX=lineWeldPara.Dlt_TX;
        dlg->lineWeldPara.Dlt_TY=lineWeldPara.Dlt_TY;
        dlg->lineWeldPara.Dlt_TZ=lineWeldPara.Dlt_TZ;
        dlg->lineWeldPara.T1_Delay_time=lineWeldPara.T1_Delay_time;
        dlg->lineWeldPara.T2_Delay_time=lineWeldPara.T2_Delay_time;
        dlg->lineWeldPara.T3_Delay_time=lineWeldPara.T3_Delay_time;
        dlg->lineWeldPara.v_Over_Time_count=lineWeldPara.v_Over_Time_count;

        QPoint pos;
        pos.setX(0);
        pos.setY(0);
        pos=dragWeldButton->mapToGlobal(pos);
        dlg->flayX=pos.x();
        dlg->flayY=pos.y();
        dlg->flayWidth=100;
        dlg->flayHeight=50;
        dlg->initDialog();

        QStringList lineWeldName;
        if(db.ReadLineTechNameFromDb(lineWeldName))
        {
            int currIndex=-1;
            for(int i=0;i<lineWeldName.size();i++)
            {
                dlg->weldTechnicsComboBox->addItem(lineWeldName[i]);
                if(technicsName==lineWeldName[i])
                    currIndex=i;
            }
            dlg->weldTechnicsComboBox->setCurrentIndex(currIndex);
        }
        db.closeDB();
        if(dlg->exec()==QDialog::Accepted)
        {
            weldProject->currCommNum=dlg->pointNum;
            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            int lineWeldType=dlg->lineWeldType;
            //weldProject->currCommNum=index.row();
            if(lineWeldType==0)
                weldProject->listCommand[weldProject->currCommNum]=QString(tr("拖焊"));
            else if(lineWeldType==1)
                weldProject->listCommand[weldProject->currCommNum]=QString(tr("多拖1"));
            else if(lineWeldType==2)
                weldProject->listCommand[weldProject->currCommNum]=QString(tr("多拖2"));
            else if(lineWeldType==3)
                weldProject->listCommand[weldProject->currCommNum]=QString(tr("多拖3"));

            weldProject->listTechnics[weldProject->currCommNum]=dlg->weldTechnicsName;
            weldProject->Zh[weldProject->currCommNum]=dlg->Z_h;
            for(int i=0;i<8;i++)
            {
                weldProject->curPointPos[i]=dlg->currPointPos[i];
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];//拖焊起点终点坐标xyzr
            }

            //姿态参数
            weldProject->ironPos[0]=dlg->ironPos[0];
            weldProject->ironPos[1]=dlg->ironPos[1];
            weldProject->pointsAxisPos2[weldProject->currCommNum][5]=dlg->ironPos[0];
            weldProject->pointsAxisPos2[weldProject->currCommNum][6]=dlg->ironPos[1];

            weldProject->saveCurrItem();    //保存当前命令条目

            QModelIndex index = tableView->currentIndex();
            QVariant data=QVariant(QString(tr("拖焊")));
            if(lineWeldType==0)
                data=QString(tr("拖焊"));
            else if(lineWeldType==1)
                data=QString(tr("多拖1"));
            else if(lineWeldType==2)
                data=QString(tr("多拖2"));
            else if(lineWeldType==3)
                data=QString(tr("多拖3"));
            dataModel->setData(dataModel->index(index.row(),1),data);
            data=QVariant(dlg->weldTechnicsName);
            dataModel->setData(dataModel->index(index.row(),2),data);

            if(dlg->appToSameTechnicsChecked)
            {
                SqlDB db;
                if(!db.openDB("robot.db"))
                {
                    msgBox.setText(tr("打开数据库失败."));
                    msgBox.exec();
                    return;
                }
                QString command=weldProject->listCommand[weldProject->currCommNum];
                if(!db.UpdateTechnicsFromDb(weldProject->getProjName(),dlg->old_WeldTechnicsName,dlg->weldTechnicsName,command))
                {
                    msgBox.setText(tr("更新同类工艺参数失败."));
                    msgBox.exec();
                    db.closeDB();
                    return;
                }
                if(!db.UpdateZhFromDb(weldProject->getProjName(),dlg->old_Z_h,dlg->Z_h,command))
                {
                    msgBox.setText(tr("更新同类提升高度失败."));
                    msgBox.exec();
                    db.closeDB();
                    return;
                }
                int cnt=weldProject->listCommand.size();
                for(int i=0;i<cnt;i++)
                {
                    PROJ_ITEM item;
                    if(db.ReadProjItemFromDb(weldProject->getProjName(),i,item))
                    {
                        QString strTechnics;
                        if(weldProject->optimize[i]==1)
                            strTechnics=item.technics.append(tr(" 优化"));
                        else
                            strTechnics=item.technics;
                        data=QVariant(strTechnics);
                        dataModel->setData(dataModel->index(i,2),data);
                    }
                    weldProject->listTechnics[i]=item.technics;
                }
                db.closeDB();
            }
            /*
            if(weldProject->currCommNum==dataModel->rowCount()-1)
            {
                dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
                weldProject->currCommNum+=1;
                weldProject->appendCommand();
            }
            */
            tableView->selectRow(weldProject->currCommNum);
            etracksWidget->updateTracks();
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
        activeWeldDlg=0;
    }
    else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("清锡")))
    {
        ClearSolderDlg *dlg=new ClearSolderDlg(this);
        dlg->robot=robot;
        dlg->weldProject=weldProject;
        connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            SqlDB db;
            if(!db.openDB("robot.db"))
            {
                msgBox.setText(tr("打开数据库失败."));
                msgBox.exec();
                return;
            }
            if(!db.SaveClearSolderToDb(dlg->clearItem.projectName,dlg->clearItem))
            {
                msgBox.setText(tr("保存清锡参数到数据库失败."));
                msgBox.exec();
                return;
            }
            db.closeDB();

            QModelIndex index = tableView->currentIndex();
            QVariant data=QVariant(QString(tr("清锡")));
            dataModel->setData(dataModel->index(index.row(),1),data);
            //data=QVariant(dlg->weldTechnicsName);
            //dataModel->setData(dataModel->index(index.row(),2),data);

            weldProject->currCommNum=index.row();
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("清锡"));
            weldProject->listTechnics[weldProject->currCommNum]=weldProject->getProjName();
            weldProject->Zh[weldProject->currCommNum]=dlg->clearItem.Zh;
            for(int i=0;i<8;i++)
            {
                if(i<4)
                    weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->clearItem.clearPos[i];
                else
                    weldProject->pointsAxisPos[weldProject->currCommNum][i]=0;
            }
            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            weldProject->saveCurrItem();    //保存当前命令条目
            /*
            if(weldProject->currCommNum==dataModel->rowCount()-1)
            {
                dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
                weldProject->currCommNum+=1;
                weldProject->appendCommand();
            }
            */
            tableView->selectRow(weldProject->currCommNum);
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
    }
    else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("跳转")))
    {
        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }

        QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect screenRect = desktopWidget->screenGeometry();
        GotoDlg *dlg=new GotoDlg(this);
        PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
            dlg->number=item.technics.toInt();  //要跳转的指令号
        db.closeDB();
        dlg->initDialog();
        dlg->move((screenRect.width()-dlg->size().width())/2,30);
        if(dlg->exec()==QDialog::Accepted)
        {
            QModelIndex index = tableView->currentIndex();
            QVariant data=QVariant(QString(tr("跳转")));
            dataModel->setData(dataModel->index(index.row(),1),data);
            data=QVariant(dlg->number);
            dataModel->setData(dataModel->index(index.row(),2),data);

            weldProject->currCommNum=index.row();
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("跳转"));
            weldProject->listTechnics[weldProject->currCommNum]=QString("%1").arg(dlg->number);
            weldProject->Zh[weldProject->currCommNum]=0;
            for(int i=0;i<8;i++)
            {
                weldProject->curPointPos[i]=0;
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=0;
            }
            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            weldProject->saveCurrItem();    //保存当前命令条目
            /*
            if(weldProject->currCommNum==dataModel->rowCount()-1)
            {
                dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
                weldProject->currCommNum+=1;
                weldProject->appendCommand();
            }
            */
            tableView->selectRow(weldProject->currCommNum);
            etracksWidget->updateTracks();
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
    }
    else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("延时")))
    {

        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }

        QDesktopWidget* desktopWidget = QApplication::desktop();
        QRect screenRect = desktopWidget->screenGeometry();
        DelayDlg *dlg=new DelayDlg(this);
        PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
            dlg->time=item.technics.toDouble();  //要延时的时长,单位(秒)
        db.closeDB();
        dlg->initDialog();
        dlg->move((screenRect.width()-dlg->size().width())/2,30);
        if(dlg->exec()==QDialog::Accepted)
        {
            QModelIndex index = tableView->currentIndex();
            QVariant data=QVariant(QString(tr("延时")));
            dataModel->setData(dataModel->index(index.row(),1),data);
            data=QVariant(tr("%1 (秒)").arg(dlg->time));
            dataModel->setData(dataModel->index(index.row(),2),data);

            weldProject->currCommNum=index.row();
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("延时"));
            weldProject->listTechnics[weldProject->currCommNum]=QString("%1").arg(dlg->time);
            weldProject->Zh[weldProject->currCommNum]=0;
            for(int i=0;i<8;i++)
            {
                weldProject->curPointPos[i]=0;
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=0;
            }
            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            weldProject->saveCurrItem();    //保存当前命令条目
            /*
            if(weldProject->currCommNum==dataModel->rowCount()-1)
            {
                dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
                weldProject->currCommNum+=1;
                weldProject->appendCommand();
            }
            */
            tableView->selectRow(weldProject->currCommNum);
            etracksWidget->updateTracks();
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
    }
    else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("空点")))
    {

        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }

        BlankPointDlg *dlg=new BlankPointDlg(this);
        connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
        PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
        {
            dlg->zh=item.Zh;
            dlg->delayTime=item.optimize;
            dlg->atspeed=item.pointCoordinate2[4];
            for(int k=0;k<4;k++)
            {
                dlg->pointPos[k]=item.pointCoordinate1[k];
            }
        }
        db.closeDB();
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            QModelIndex index = tableView->currentIndex();
            QVariant data=QVariant(QString(tr("空点")));
            dataModel->setData(dataModel->index(index.row(),1),data);

            weldProject->currCommNum=index.row();
            weldProject->listCommand[weldProject->currCommNum]=QString(tr("空点"));
            weldProject->Zh[weldProject->currCommNum]=dlg->zh;
            for(int i=0;i<4;i++)
            {
                weldProject->curPointPos[i]=dlg->pointPos[i];
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];
            }
            weldProject->optimize[weldProject->currCommNum]=dlg->delayTime;
            weldProject->pointsAxisPos[weldProject->currCommNum][4]=dlg->atspeed;

            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            weldProject->saveCurrItem();    //保存当前命令条目
            /*
            if(weldProject->currCommNum==dataModel->rowCount()-1)
            {
                dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
                weldProject->currCommNum+=1;
                weldProject->appendCommand();
            }
            */
            tableView->selectRow(weldProject->currCommNum);
            etracksWidget->updateTracks();
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
    }
    else if(weldProject->listCommand[weldProject->currCommNum] == QString(tr("姿态")))
    {
    /*    IronPoseDlg *dlg=new IronPoseDlg(this);
        connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
        PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
        {
            dlg->zh=item.Zh;
            dlg->delayTime=item.optimize;
            dlg->atspeed=item.pointCoordinate2[4];
            dlg->exeOnce=item.pointCoordinate2[7];
            for(int k=0;k<4;k++)
            {
                dlg->pointPos[k]=item.pointCoordinate1[k];
            }
            dlg->pointPos[4]=item.pointCoordinate1[5];
            dlg->pointPos[5]=item.pointCoordinate1[6];
            dlg->exeOnce=item.pointCoordinate2[7];
        }
        dlg->initDialog();
        if(dlg->exec()==QDialog::Accepted)
        {
            QModelIndex index = tableView->currentIndex();
            weldProject->currCommNum=index.row();
            QVariant data=QVariant(QString(tr("姿态")));
            dataModel->setData(dataModel->index(index.row(),1),data);
            if(dlg->exeOnce==1)
            {
                data=QVariant(tr("仅执行一次"));
                dataModel->setData(dataModel->index(index.row(),2),data);
                weldProject->listTechnics[weldProject->currCommNum]=tr("仅执行一次");
            }
            else
            {
                data=QVariant(tr("可多次执行"));
                dataModel->setData(dataModel->index(index.row(),2),data);
                weldProject->listTechnics[weldProject->currCommNum]=tr("可多次执行");
            }

            weldProject->listCommand[weldProject->currCommNum]=QString(tr("姿态"));
            //weldProject->listTechnics[weldProject->currCommNum]=QString("%1").arg(dlg->number);
            weldProject->Zh[weldProject->currCommNum]=dlg->zh;
            for(int i=0;i<4;i++)
            {
                weldProject->curPointPos[i]=dlg->pointPos[i];
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];
            }
            weldProject->curPointPos[5]=dlg->pointPos[4];
            weldProject->pointsAxisPos[weldProject->currCommNum][5]=dlg->pointPos[4];
            weldProject->curPointPos[6]=dlg->pointPos[5];
            weldProject->pointsAxisPos[weldProject->currCommNum][6]=dlg->pointPos[5];

            weldProject->optimize[weldProject->currCommNum]=dlg->delayTime;
            weldProject->pointsAxisPos[weldProject->currCommNum][4]=dlg->atspeed;
            weldProject->listTechnics[weldProject->currCommNum].clear();
            weldProject->pointsAxisPos[weldProject->currCommNum][7]=dlg->exeOnce;

            weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

            weldProject->saveCurrItem();    //保存当前命令条目
            tableView->selectRow(weldProject->currCommNum);
            etracksWidget->updateTracks();
        }
        delete dlg;
        tableView->setFocus(Qt::OtherFocusReason);
        */
    }
}

void EditParaPage::OnUndoBtnClicked()  //添加槽函数
{
    if(!weldProject) return;
    int cnt=weldProject->listCommand.size();
    dataModel->appendRow(new QStandardItem(QString("%1").arg(cnt)));
    weldProject->currCommNum=cnt;
    weldProject->appendCommand();
    tableView->selectRow(weldProject->currCommNum);
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnInsertBtnClicked()  //插入槽函数
{
    if(weldProject)
    {
        if(weldProject->listCommand.size()==0)
        {
            weldProject->currCommNum=0;
            weldProject->listCommand.append("");
            weldProject->listTechnics.append("");
            weldProject->Zh.append(0);
            weldProject->optimize.append(0);
            weldProject->pointsAxisPos.append(weldProject->curPointPos);    //存储当前点的坐标
        }
    }
    else return;
    int flag=0;
    int rowCount=dataModel->rowCount();
    if(rowCount==0)
    {
        flag=1;
        dataModel->setItem(0, 0, new QStandardItem("0"));
        dataModel->setItem(0, 1, new QStandardItem(""));
        dataModel->setItem(0, 2, new QStandardItem(""));
        tableView->selectRow(0);
    }
    QModelIndex index = tableView->currentIndex();
    int row=index.row();
    if(row<0) return;   //未选中任何一行，退出
    QVariant data=QVariant(row);
    if(flag==0)
        dataModel->insertRow(row,QModelIndex());
    for(int i=row;i<=rowCount;i++)
    {
        data=QVariant(i);
        dataModel->setData(dataModel->index(i,0),data);
    }
    weldProject->insertCommand(row);
    weldProject->setCurrCommand(row);
    tableView->selectRow(row);
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnDeleteBtnClicked()  //删除槽函数
{
    if(!weldProject)
    {
        return;
    }
    if(weldProject->currCommNum<0)
        return;
    if(dataModel->rowCount()==0)
        return;

    QMessageBox msgBox1;
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
    msgBox1.setText(tr("您是否真的要删除所选择的命令?"));
    if(msgBox1.exec()==QMessageBox::RejectRole)
    {
        return;
    }

    QModelIndex index = tableView->currentIndex();
    int row=index.row();
    weldProject->deleteCommand(row);
    dataModel->removeRow(row,QModelIndex());

    int rowCount=dataModel->rowCount();
    for(int i=row;i<rowCount;i++)
    {
        QVariant data=QVariant(i);
        dataModel->setData(dataModel->index(i,0),data);
    }

    if(row>0) row--;

    if(rowCount>0)
    {
        weldProject->setCurrCommand(row);
        etracksWidget->updateTracks();
        tableView->selectRow(row);
    }
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnCopyBtnClicked()  //复制槽函数
{
}

void EditParaPage::OnPasteBtnClicked()  //粘贴槽函数
{
}

void EditParaPage::OnLogicBtnClicked()  //逻辑槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }

    QPoint pos;
    pos.setX(0);
    pos.setY(-logicMenu->sizeHint().height());
    logicMenu->exec(logicButton->mapToGlobal(pos));
  }

void EditParaPage::OnDelayActionClicked()    //延时
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    DelayDlg *dlg=new DelayDlg(this);
    dlg->initDialog();
    dlg->move((screenRect.width()-dlg->size().width())/2,30);
    if(dlg->exec()==QDialog::Accepted)
    {
        QModelIndex index = tableView->currentIndex();
        QVariant data=QVariant(QString(tr("延时")));
        dataModel->setData(dataModel->index(index.row(),1),data);
        data=QVariant(tr("%1 (秒)").arg(dlg->time));
        dataModel->setData(dataModel->index(index.row(),2),data);

        weldProject->currCommNum=index.row();
        weldProject->listCommand[weldProject->currCommNum]=QString(tr("延时"));
        weldProject->listTechnics[weldProject->currCommNum]=QString("%1").arg(dlg->time);
        weldProject->Zh[weldProject->currCommNum]=0;
        for(int i=0;i<8;i++)
        {
            weldProject->curPointPos[i]=0;
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=0;
        }
        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        weldProject->saveCurrItem();    //保存当前命令条目

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
        etracksWidget->updateTracks();
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnGotoActionClicked()     //跳转
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    GotoDlg *dlg=new GotoDlg(this);
    dlg->initDialog();
    dlg->move((screenRect.width()-dlg->size().width())/2,30);
    if(dlg->exec()==QDialog::Accepted)
    {
        QModelIndex index = tableView->currentIndex();
        QVariant data=QVariant(QString(tr("跳转")));
        dataModel->setData(dataModel->index(index.row(),1),data);
        data=QVariant(dlg->number);
        dataModel->setData(dataModel->index(index.row(),2),data);

        weldProject->currCommNum=index.row();
        weldProject->listCommand[weldProject->currCommNum]=QString(tr("跳转"));
        weldProject->listTechnics[weldProject->currCommNum]=QString("%1").arg(dlg->number);
        weldProject->Zh[weldProject->currCommNum]=0;
        for(int i=0;i<8;i++)
        {
            weldProject->curPointPos[i]=0;
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=0;
        }
        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        weldProject->saveCurrItem();    //保存当前命令条目

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
        etracksWidget->updateTracks();
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnCondiActionClicked()    //条件
{
    QModelIndex index = tableView->currentIndex();
    QVariant data=QVariant(QString(tr("复位")));
    dataModel->setData(dataModel->index(index.row(),1),data);
    data=QVariant(QString(""));
    dataModel->setData(dataModel->index(index.row(),2),data);

    weldProject->currCommNum=index.row();
    weldProject->listCommand[weldProject->currCommNum]=QString(tr("复位"));
    weldProject->listTechnics[weldProject->currCommNum]=QString("");
    weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

    weldProject->saveCurrItem();    //保存当前命令条目

    if(weldProject->currCommNum==dataModel->rowCount()-1)
    {
        dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
        weldProject->currCommNum+=1;
        weldProject->appendCommand();
    }
    tableView->selectRow(weldProject->currCommNum);
    etracksWidget->updateTracks();
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnBlankActionClicked()    //空点
{
    BlankPointDlg *dlg=new BlankPointDlg;
    connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
        QModelIndex index = tableView->currentIndex();
        QVariant data=QVariant(QString(tr("空点")));
        dataModel->setData(dataModel->index(index.row(),1),data);
        //data=QVariant(dlg->number);
        //dataModel->setData(dataModel->index(index.row(),2),data);

        weldProject->currCommNum=index.row();
        weldProject->listCommand[weldProject->currCommNum]=QString(tr("空点"));
        //weldProject->listTechnics[weldProject->currCommNum]=QString("%1").arg(dlg->number);
        weldProject->Zh[weldProject->currCommNum]=dlg->zh;
        for(int i=0;i<4;i++)
        {
            weldProject->curPointPos[i]=dlg->pointPos[i];
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];
        }
        weldProject->optimize[weldProject->currCommNum]=dlg->delayTime;
        weldProject->pointsAxisPos[weldProject->currCommNum][4]=dlg->atspeed;
        weldProject->listTechnics[weldProject->currCommNum].clear();

        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        weldProject->saveCurrItem();    //保存当前命令条目

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
        etracksWidget->updateTracks();
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnIronPoseActionClicked()    //烙铁姿态
{
    IronPoseDlg *dlg=new IronPoseDlg;
    connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
        QModelIndex index = tableView->currentIndex();
        weldProject->currCommNum=index.row();
        QVariant data=QVariant(QString(tr("姿态")));
        dataModel->setData(dataModel->index(index.row(),1),data);
        if(dlg->exeOnce==1)
        {
            data=QVariant(tr("仅执行一次"));
            dataModel->setData(dataModel->index(index.row(),2),data);
            weldProject->listTechnics[weldProject->currCommNum]=tr("仅执行一次");
        }
        else
        {
            data=QVariant(tr("可多次执行"));
            dataModel->setData(dataModel->index(index.row(),2),data);
            weldProject->listTechnics[weldProject->currCommNum]=tr("可多次执行");
        }

        weldProject->listCommand[weldProject->currCommNum]=QString(tr("姿态"));
        weldProject->Zh[weldProject->currCommNum]=dlg->zh;
        for(int i=0;i<4;i++)
        {
            weldProject->curPointPos[i]=dlg->pointPos[i];
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->pointPos[i];
        }
        weldProject->curPointPos[5]=dlg->pointPos[4];
        weldProject->pointsAxisPos[weldProject->currCommNum][5]=dlg->pointPos[4];
        weldProject->curPointPos[6]=dlg->pointPos[5];
        weldProject->pointsAxisPos[weldProject->currCommNum][6]=dlg->pointPos[5];

        weldProject->optimize[weldProject->currCommNum]=dlg->delayTime;
        weldProject->pointsAxisPos[weldProject->currCommNum][4]=dlg->atspeed;
        weldProject->listTechnics[weldProject->currCommNum].clear();
        weldProject->pointsAxisPos[weldProject->currCommNum][7]=dlg->exeOnce;   //仅执行一次

        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        weldProject->saveCurrItem();    //保存当前命令条目

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
        etracksWidget->updateTracks();
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnCoordinateClicked()  //坐标系槽函数
{
}

void EditParaPage::OnClearBtnClicked()  //清锡槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }
    ClearSolderDlg *dlg=new ClearSolderDlg(this);
    dlg->robot=robot;
    dlg->weldProject=weldProject;
    connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
         SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }
        if(!db.SaveClearSolderToDb(dlg->clearItem.projectName,dlg->clearItem))
        {
            msgBox.setText(tr("保存清锡参数到数据库失败."));
            msgBox.exec();
            return;
        }
        db.closeDB();

        QModelIndex index = tableView->currentIndex();
        QVariant data=QVariant(QString(tr("清锡")));
        dataModel->setData(dataModel->index(index.row(),1),data);
        data=QVariant(weldProject->getProjName());
        dataModel->setData(dataModel->index(index.row(),2),data);

        weldProject->currCommNum=index.row();
        weldProject->listCommand[weldProject->currCommNum]=QString(tr("清锡"));
        weldProject->listTechnics[weldProject->currCommNum]=weldProject->getProjName();
        weldProject->Zh[weldProject->currCommNum]=dlg->clearItem.Zh;
        for(int i=0;i<8;i++)
        {
            weldProject->pointsAxisPos[weldProject->currCommNum][i]=dlg->clearItem.clearPos[i];
            /*if(i<4)
            else
                weldProject->pointsAxisPos[weldProject->currCommNum][i]=0;*/
        }
        weldProject->setCurrCommand(weldProject->currCommNum);             //设置当前指令

        weldProject->saveCurrItem();    //保存当前命令条目

        if(weldProject->currCommNum==dataModel->rowCount()-1)
        {
            dataModel->appendRow(new QStandardItem(QString("%1").arg(weldProject->currCommNum+1)));
            weldProject->currCommNum+=1;
            weldProject->appendCommand();
        }
        tableView->selectRow(weldProject->currCommNum);
    }
    delete dlg;
    tableView->setFocus(Qt::OtherFocusReason);
}

void EditParaPage::OnIoBtnClicked()  //I/O槽函数
{
    TestIoDlg *dlg=new TestIoDlg(this);
    dlg->robot=robot;
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
}

void EditParaPage::OnArrayBtnClicked()  //阵列槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }

    MatrixDlg *dlg=new MatrixDlg(this);
    dlg->weldProject=weldProject;
    dlg->setGeometry(0,0,0,0);
    connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg, SLOT(updateCoordinate(double *)));
    connect(dlg, SIGNAL(useMoveKeyDlg(int)), this, SLOT(OnUseMoveKey(int)));
    connect(this, SIGNAL(slowlyInOutDlg(int)), dlg, SLOT(slowlyInOut(int)));

    for(int i=0;i<4;i++)
    {
        dlg->onePointPos[i]=pointsAxisPos[0][i];
        dlg->twoPointPos[i]=pointsAxisPos[1][i];
    }
    dlg->onePointPos[4]=pointsAxisPos[0][5];
    dlg->onePointPos[5]=pointsAxisPos[0][6];
    dlg->twoPointPos[4]=pointsAxisPos[1][5];
    dlg->twoPointPos[5]=pointsAxisPos[1][6];
    QPoint pos;
    pos.setX(0);
    pos.setY(0);
    pos=arrayButton->mapToGlobal(pos);
    dlg->flayX=pos.x();
    dlg->flayY=pos.y();
    dlg->flayWidth=100;
    dlg->flayHeight=50;
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
        int count=weldProject->listCommand.size();
        QString strTechnics;
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
        etracksWidget->updateTracks();
    }
}

void EditParaPage::OnOptimizeBtnClicked()  //优化槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }

    OptimizeDlg * dlg=new OptimizeDlg(this);
    dlg->weldProject=weldProject;
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
        int count=weldProject->listCommand.size();
        QString strTechnics;
        for(int i=0;i<count;i++)
        {
            if(weldProject->optimize[i]==1)
                strTechnics=weldProject->listTechnics[i]+tr(" 优化");
            else
                strTechnics=weldProject->listTechnics[i];
            QVariant data=QVariant(strTechnics);
            dataModel->setData(dataModel->index(i,2),data);
        }
    }
    tableView->selectRow(weldProject->currCommNum);
    tableView->setFocus(Qt::OtherFocusReason);
    delete dlg;
}

void EditParaPage::OnZHeightBtnClicked()  //Z等高槽函数
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }

    msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox1.setFont(ft);
    msgBox1.setIcon(QMessageBox::Question);
    msgBox1.setWindowTitle(tr(APPLICATION_NAME));
    msgBox1.addButton(tr("是"), QMessageBox::YesRole);
    msgBox1.addButton(tr("否"), QMessageBox::NoRole);
    msgBox1.setText(tr("您是否真的要进行Z等高操作?\n如果是,请在Z等高操作之前选择一个焊点."));
    if(msgBox1.exec()==QMessageBox::RejectRole)
    {
         return;
    }
    if(weldProject->flagInWelding>0)
    {
        msgBox.setText(tr("正在进行轨迹模拟焊接，无法进行此操作."));
        msgBox.exec();
        return;
    }

    if(!((weldProject->listCommand[weldProject->currCommNum]==QString(tr("点焊"))) || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("拖焊")))
          || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("多拖1"))) || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("多拖2"))) || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("多拖3")))))
    {
        msgBox.setText(tr("请先选择一个实际的焊点."));
        msgBox.exec();
        return;
    }
    double cZ=weldProject->pointsAxisPos[weldProject->currCommNum][2];
    for(int i=0;i<weldProject->listCommand.size();i++)
    {
        if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("空点"))))
            weldProject->pointsAxisPos[i][2]=cZ;
        if((weldProject->listCommand[i]==QString(tr("拖焊")))
                 || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
        {
            weldProject->pointsAxisPos[i][2]=cZ;
            weldProject->pointsAxisPos[i][6]=cZ;
        }
    }
    weldProject->saveProject();
}

void EditParaPage::OnLocationBtnClicked()  //定位槽函数
{
    QMessageBox msgBox,msgBox1,msgBox2;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    if(g_ioStatus.eStop==0)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }

    if(robot->isReseting)
    {
        msgBox.setText(tr("机器正在复位,无法操作！"));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }

    if(!((weldProject->listCommand[weldProject->currCommNum]==QString(tr("点焊"))) || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("拖焊")))
          || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("多拖1"))) || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("多拖2"))) || (weldProject->listCommand[weldProject->currCommNum]==QString(tr("多拖3")))))
    {
        msgBox.setText(tr("请先选择一个实际的焊点."));
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
    double x=weldProject->pointsAxisPos[weldProject->currCommNum][0];
    double y=weldProject->pointsAxisPos[weldProject->currCommNum][1];
    double z=weldProject->pointsAxisPos[weldProject->currCommNum][2];
    double r=weldProject->pointsAxisPos[weldProject->currCommNum][3];
    double zh=weldProject->Zh[weldProject->currCommNum];
    QString paraName;
    paraName=weldProject->listTechnics[weldProject->currCommNum];

    msgBox1.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox1.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox1.setFont(ft);
    msgBox1.setIcon(QMessageBox::Question);
    msgBox1.setWindowTitle(tr(APPLICATION_NAME));

    weldProject->flagInWelding=0;
    if(weldProject->listCommand[weldProject->currCommNum]==QString(tr("点焊")))
    {
        if(db.ReadPointWeldFromDb(paraName,g_PointWeldPara))
        {
            PROJ_ITEM item;
            if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
            {
                g_PointWeldPara.rr1=item.pointCoordinate1[5];
                g_PointWeldPara.rr2=item.pointCoordinate1[6];
            }
            g_PointWeldPara.x=x;
            g_PointWeldPara.y=y;
            g_PointWeldPara.z=z;
            g_PointWeldPara.r=r;
            g_PointWeldPara.ZH=zh;
            weldProject->execMode=ROBOT_SINGLE_EXEC_MODE;
            emit goFixPos(1);
        }
    }
    else
    {
        PROJ_ITEM item;
        if(db.ReadProjItemFromDb(weldProject->getProjName(),weldProject->currCommNum,item))
        {
            g_LineWeldPara.rr1=item.pointCoordinate1[5];
            g_LineWeldPara.rr2=item.pointCoordinate1[6];
        }

        msgBox1.addButton(tr("起点"), QMessageBox::YesRole);
        msgBox1.addButton(tr("终点"), QMessageBox::NoRole);
        msgBox1.setText(tr("这是拖焊命令，您要定位到起点还是终点?"));
        if(msgBox1.exec()==QMessageBox::RejectRole)
        {
            if(db.ReadLineWeldFromDb(paraName,g_LineWeldPara))
            {
                x=weldProject->pointsAxisPos[weldProject->currCommNum][4];
                y=weldProject->pointsAxisPos[weldProject->currCommNum][5];
                z=weldProject->pointsAxisPos[weldProject->currCommNum][6];
                r=weldProject->pointsAxisPos[weldProject->currCommNum][7];
                g_LineWeldPara.x0=x;
                g_LineWeldPara.y0=y;
                g_LineWeldPara.z0=z;
                g_LineWeldPara.r0=r;
                g_LineWeldPara.ZH=zh;
                weldProject->execMode=ROBOT_SINGLE_EXEC_MODE;
                emit goFixPos(2);
            }
        }
        else
        {
            if(db.ReadLineWeldFromDb(paraName,g_LineWeldPara))
            {
                g_LineWeldPara.x0=x;
                g_LineWeldPara.y0=y;
                g_LineWeldPara.z0=z;
                g_LineWeldPara.r0=r;
                g_LineWeldPara.ZH=zh;
                weldProject->execMode=ROBOT_SINGLE_EXEC_MODE;
                emit goFixPos(2);
            }
        }
    }
    db.closeDB();
}

void EditParaPage::OnExecuteBtnClicked()  //执行此项槽函数
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
/*
    if(g_ioStatus.eStop==0)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }

    if(robot->isReseting)
    {
        msgBox.setText(tr("机器正在复位,无法操作！"));
        msgBox.exec();
        return;
    }
*/
    QModelIndex index = tableView->currentIndex();
    weldProject->setExecMode(ROBOT_SINGLE_EXEC_MODE);   //单条执行

    weldProject->stopWeldFlag=0;  //允许执行项目
    weldProject->setCurrCommand(index.row());
    robot->ML_FF_Set_Do_Step_style(0);  //设置为第1项,0(起始项),1（中间项）,2（最后一项）
    robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
    robot->ML_FF_Stop_Tin(0);   //送锡
    weldProject->execItem();
    weldProject->flagInWelding=0;
}

void EditParaPage::OnTracksBtnClicked()  //轨迹槽函数
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

    if(g_ioStatus.eStop==0)
    {
        msgBox.setText(tr("急停按钮已经按下,无法操作！"));
        msgBox.exec();
        return;
    }
    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->flagInWelding==2)
        return;
    if(robot)
    {
        if(robot->isReseting)
        {
            msgBox.setText(tr("机器正在复位,无法操作！"));
            msgBox.exec();
            return;
        }

        QModelIndex index = tableView->currentIndex();
        weldProject->flagInWelding=2;
        weldProject->stopWeldFlag=0;  //允许执行项目
        robot->ML_FF_Stop_Tin(1);   //停止送锡
        robot->ML_SetOutput(g_IOxb.sysOxb[LED_START_O]);    //启动按钮灯亮
        QThread::msleep(5);    //延时5ms
        weldProject->execProject(index.row());   //连续执行
    }
}

void EditParaPage::OnCorrectBtnClicked()     //校正
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

    if(!weldProject)
    {
        msgBox.setText(tr("请先新建或打开一个项目."));
        msgBox.exec();
        return;
    }
    if(weldProject->listCommand.size()==0)
    {
        msgBox.setText(tr("请先新建或打开一个正确的项目."));
        msgBox.exec();
        return;
    }
    CorrectDlg *dlg=new CorrectDlg(this);
    if(dlg->exec()==QDialog::Accepted)
    {
        if(dlg->correctType==1)
        {
            ParalleCorrectDlg *dlg1=new ParalleCorrectDlg(this);
            dlg1->weldProject=weldProject;
            if(dlg1->exec()==QDialog::Accepted)
            {
                int currCmdNum=weldProject->currCommNum;
                int fromStep=dlg1->fromStep;   //从第几步
                int toStep=dlg1->toStep;     //到第几步
                double dltX=curPointAxisPos[0]-weldProject->pointsAxisPos[currCmdNum][0];
                double dltY;
                if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
                {
                    for(int i=fromStep;i<=toStep;i++)
                    {
                        weldProject->pointsAxisPos[currCmdNum][7]=weldProject->pointsAxisPos[currCmdNum][1];
                    }
                    if(curPointAxisPos[0]>systemPara.limitPosLen[0]/2)    //Y2
                        dltY=curPointAxisPos[7]-weldProject->pointsAxisPos[currCmdNum][7];
                    else
                        dltY=curPointAxisPos[1]-weldProject->pointsAxisPos[currCmdNum][1];
                }
                else
                    dltY=curPointAxisPos[1]-weldProject->pointsAxisPos[currCmdNum][1];
                if(dlg1->mod==0)    //移动平移
                {
                    for(int i=fromStep;i<=toStep;i++)
                    {
                         if((weldProject->listCommand[i]==tr("点焊")) || (weldProject->listCommand[i]==tr("空点")))
                        {
                            weldProject->pointsAxisPos[i][0]+=dltX;
                            weldProject->pointsAxisPos[i][1]+=dltY;
                            weldProject->pointsAxisPos[i][7]+=dltY;
                        }
                        else if((weldProject->listCommand[i]==tr("拖焊")) || (weldProject->listCommand[i]==tr("多拖1"))
                                 || (weldProject->listCommand[i]==tr("多拖2")) || (weldProject->listCommand[i]==tr("多拖3")))
                        {
                            weldProject->pointsAxisPos[i][0]+=dltX;
                            weldProject->pointsAxisPos[i][1]+=dltY;
                            weldProject->pointsAxisPos[i][4]+=dltX;
                            weldProject->pointsAxisPos[i][5]+=dltY;
                        }
                    }
                }
                else    //复制平移
                {
                    int cnt=weldProject->listCommand.size();
                    for(int i=0;i<toStep-fromStep+1;i++)
                    {
                        weldProject->appendCommand();
                        weldProject->listCommand[cnt+i]=weldProject->listCommand[fromStep+i];
                        weldProject->listTechnics[cnt+i]=weldProject->listTechnics[fromStep+i];
                        weldProject->Zh[cnt+i]=weldProject->Zh[fromStep+i];
                        weldProject->optimize[cnt+i]=weldProject->optimize[fromStep+i];
                        for(int j=0;j<8;j++)
                            weldProject->pointsAxisPos[cnt+i][j]=weldProject->pointsAxisPos[fromStep+i][j];
                        if((weldProject->listCommand[cnt+i]==tr("点焊")) || (weldProject->listCommand[cnt+i]==tr("空点")))
                        {
                            weldProject->pointsAxisPos[cnt+i][0]+=dltX;
                            weldProject->pointsAxisPos[cnt+i][1]+=dltY;
                            weldProject->pointsAxisPos[cnt+i][7]+=dltY;
                        }
                        else if((weldProject->listCommand[cnt+i]==tr("拖焊")) || (weldProject->listCommand[cnt+i]==tr("多拖1"))
                                 || (weldProject->listCommand[cnt+i]==tr("多拖2")) || (weldProject->listCommand[cnt+i]==tr("多拖3")))
                        {
                            weldProject->pointsAxisPos[cnt+i][0]+=dltX;
                            weldProject->pointsAxisPos[cnt+i][1]+=dltY;
                            weldProject->pointsAxisPos[cnt+i][4]+=dltX;
                            weldProject->pointsAxisPos[cnt+i][5]+=dltY;
                        }
                    }
                    QString strTechnics;
                    for(int i=cnt;i<cnt+toStep-fromStep+1;i++)
                    {
                        dataModel->appendRow(new QStandardItem(QString("%1").arg(i)));
                        if(weldProject->optimize[i]==1)
                            strTechnics=weldProject->listTechnics[i]+tr(" 优化");
                        else
                            strTechnics=weldProject->listTechnics[i];
                        dataModel->setItem(i, 0, new QStandardItem(QString("%1").arg(i)));
                        dataModel->setItem(i, 1, new QStandardItem(weldProject->listCommand[i]));
                        dataModel->setItem(i, 2, new QStandardItem(strTechnics));
                    }
                }
                weldProject->saveProject();    //保存当前项目

                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("校正完毕."));
                msgBox.exec();

                etracksWidget->updateTracks();
            }
            delete dlg1;
        }
        else if(dlg->correctType==2)
        {
            RotateCorrectDlg *dlg1=new RotateCorrectDlg(this);
            dlg1->weldProject=weldProject;
            dlg1->robot=robot;
            dlg1->initDialog();
            connect(robot, SIGNAL(haveAxisCoordinate(double *)), dlg1, SLOT(updateCoordinate(double *)));
            if(dlg1->exec()==QDialog::Accepted)
            {

                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("校正完毕."));
                msgBox.exec();

                etracksWidget->updateTracks();
            }
            delete dlg1;
        }
        else if(dlg->correctType==3)
        {
            ZOffsetDlg *dlg2=new ZOffsetDlg(this);
            dlg2->weldProject=weldProject;
            dlg2->initDialog();
            if(dlg2->exec()==QDialog::Accepted)
            {
                double zLength=dlg2->zLength;
                int fromStep=dlg2->fromStep;   //从第几步
                int toStep=dlg2->toStep;     //到第几步
                int iK=1;
                if(dlg2->mod==1)
                    iK=1;
                else if(dlg2->mod==2)
                    iK=-1;

                for(int i=fromStep;i<=toStep;i++)
                {
                    if((weldProject->listCommand[i]==tr("点焊")))
                    {
                        weldProject->pointsAxisPos[i][2] += zLength*iK;
                    }
                    else if((weldProject->listCommand[i]==tr("拖焊")) || (weldProject->listCommand[i]==tr("多拖1"))
                            || (weldProject->listCommand[i]==tr("多拖2")) ||  (weldProject->listCommand[i]==tr("多拖3")))
                    {
                        weldProject->pointsAxisPos[i][2] += zLength*iK;
                        weldProject->pointsAxisPos[i][6] += zLength*iK;
                    }
                }
                weldProject->saveProject();

                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("校正完毕."));
                msgBox.exec();

                etracksWidget->updateTracks();
            }
            delete dlg2;
        }
    }
    tableView->selectRow(weldProject->currCommNum);
    tableView->setFocus(Qt::OtherFocusReason);
    delete dlg;
}

void EditParaPage::setCurrCommandItem(int index)    //在指令列表里设置当前命令
{
    if(weldProject)
    {
        if(index<0 || index>=weldProject->listCommand.size())
            index=0;
        tableView->selectRow(index);
        tableView->setFocus(Qt::OtherFocusReason);
        weldProject->currCommNum=index;
        etracksWidget->updateTracks();
    }
}

void EditParaPage::clearProject()                //清除打开的项目，使系统成无打开项目状态
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
    etracksWidget->updateTracks();
    //delete weldProject;
    //weldProject=NULL;
}
void EditParaPage::stopWeldOperate(int st)
{
    if(g_currOperatePage != 2)  //当前页不是编辑界面
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
        //if(weldProject)
        //    weldProject->setCurrCommand(0);
    }
}
