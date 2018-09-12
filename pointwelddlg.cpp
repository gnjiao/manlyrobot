#include "pointwelddlg.h"
#include "msqldb.h"
#include "weldproject.h"
#include "myrwxml.h"
#include "envparadlg.h"

//#define PI          3.14159       //圆周率


PointWeldDlg::PointWeldDlg(QWidget *parent) :
    QDialog(parent)
{
    Z_h=0;
    old_Z_h=0;
    type=0;
    pointNum=0;
    dlgWidth=700;
    dlgHeight=480;
    alpha=230;
    context=0;
    flagInitFinished=0;
    flagRunSpeed_Focus=0;
    flagZ_h_Focus=0;
    flagKB_On=0;
    appToSameTechnicsChecked=false;
    for(int i=0;i<8;i++) pointPos[i]=0;
    for(int i=0;i<8;i++) currPointPos[i]=0;
    for(int i=0;i<6;i++) flagCurrPosEdit_Focus[i]=0;
    for(int i=0;i<8;i++)
    {
        flagParaEdit1_Focus[i]=0;
        flagParaEdit2_Focus[i]=0;
    }

    //context=new MyInputPanelContext(this);
    //context->inputPanel->hide();
}
void PointWeldDlg::initDialog()
{
    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    //setWindowOpacity(0.5);     /********************border-radius:10px; padding:2px 10px;*///////////
    setStyleSheet("QLineEdit { background-color: QColor(255,255,0,10) ; color:white; border:1px groove white; } \
                  QPushButton { background-color: qconicalgradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(214, 214, 214, 200), stop:0.5 rgba(116, 116, 116, 200), stop:1 rgba(155, 234, 251, 160)); \
                                border: 1px solid rgb(124, 124, 124); border-radius:10px; color:black;}"
                    "QPushButton:pressed{ background:rgba(0,0,255,150); }" \
                    "QPushButton:disabled{ background:qconicalgradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(214, 214, 214, 100), stop:0.5 rgba(36, 36, 36, 100), stop:1 rgba(214, 214, 214, 100)); }");
    /*QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    palette.setColor(QPalette::WindowText, QColor(0,0,0));*/
    QFont ft;
    ft.setPointSize(12);
    ft.setItalic(false);

    createLeftLayout();
    createRightLayout();
    createBottomLayout();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(paraLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    this->setFont(ft);
    setWindowTitle(tr("点焊设置"));
    //resize(750,400);
    //move(50,120);
    connect(weldTechnicsComboBox, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(selectPointWeldPara(const QString &)));
    connect(this, SIGNAL(loadDlgFinished()),this, SLOT(loadMyLineEdit()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(inout()));
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()),this, SLOT(inout2()));
    fadeTimes = 1000;
    beginPointX=flayX;
    beginPointY=flayY;

    if(type==0) //新建焊点
    {
        weldTechnicsEdit->setEnabled(false);
        weldTechnicsComboBox->setEnabled(false);
        Z_hEdit->setEnabled(true);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(false);
        preModeComboBox->setEnabled(false);
        weldModeComboBox->setEnabled(false);
        loadDefaultValBtn->setEnabled(false);
        //copyParaValBtn->setEnabled(false);
        setDefaultValBtn->setEnabled(false);
        updatePosBtn->setEnabled(true);
        OnUpdatePosBtnClicked();
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(false);
            technicsParaEdit2[i]->setEnabled(false);
        }
        for(int i=0;i<4;i++)
            currPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
        currPosEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
        currPosEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',1));
        if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
        {
            if(currPointPos[0]>systemPara.limitPosLen[0]/2)    //Y2
                currPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
            else
                currPosEdit[1]->setText(QString("%1").arg(currPointPos[1],0,'f',2));
        }
        weldTechnicsEdit->setText(weldTechnicsName);
        Z_hEdit->setText(QString("%1").arg(Z_h,0,'f',1));
        pointNameEdit->setText(QString("%1").arg(pointNum));
        appToSameTechnicsCheckBox->setEnabled(false);
    }
    else if(type==1)    //编辑已有焊点
    {
        weldTechnicsEdit->setEnabled(false);
        weldTechnicsComboBox->setEnabled(true);
        Z_hEdit->setEnabled(true);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(false);
        preModeComboBox->setEnabled(false);
        weldModeComboBox->setEnabled(false);
        loadDefaultValBtn->setEnabled(true);
        //copyParaValBtn->setEnabled(true);
        setDefaultValBtn->setEnabled(true);
        updatePosBtn->setEnabled(true);
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(false);
            technicsParaEdit2[i]->setEnabled(false);
        }
        for(int i=0;i<4;i++)
            currPosEdit[i]->setText(QString("%1").arg(pointPos[i],0,'f',2));
        currPosEdit[4]->setText(QString("%1").arg(pointPos[5],0,'f',2));
        currPosEdit[5]->setText(QString("%1").arg(pointPos[6],0,'f',2));
        weldTechnicsEdit->setText(weldTechnicsName);
        Z_hEdit->setText(QString("%1").arg(Z_h,0,'f',1));
        pointNameEdit->setText(QString("%1").arg(pointNum));
        appToSameTechnicsCheckBox->setEnabled(true);
        old_WeldTechnicsName=weldTechnicsName;
        old_Z_h=Z_h;
    }
    else if(type==2)    //新建点焊工艺参数
    {
        weldTechnicsEdit->setEnabled(true);
        weldTechnicsComboBox->setEnabled(false);
        Z_hEdit->setEnabled(false);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(true);
        preModeComboBox->setEnabled(true);
        weldModeComboBox->setEnabled(true);
        loadDefaultValBtn->setEnabled(true);
        //copyParaValBtn->setEnabled(true);
        setDefaultValBtn->setEnabled(true);
        updatePosBtn->setEnabled(false);
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(true);
            technicsParaEdit2[i]->setEnabled(true);
        }
        for(int i=0;i<6;i++)
            currPosEdit[i]->setEnabled(false);
        appToSameTechnicsCheckBox->setEnabled(false);
    }
    else if(type==3)    //编辑点焊工艺参数
    {
        weldTechnicsEdit->setEnabled(false);
        weldTechnicsComboBox->setEnabled(false);
        Z_hEdit->setEnabled(false);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(true);
        preModeComboBox->setEnabled(true);
        weldModeComboBox->setEnabled(true);
        loadDefaultValBtn->setEnabled(true);
        //copyParaValBtn->setEnabled(true);
        setDefaultValBtn->setEnabled(true);
        updatePosBtn->setEnabled(false);
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(true);
            technicsParaEdit2[i]->setEnabled(true);
        }
        for(int i=0;i<6;i++)
            currPosEdit[i]->setEnabled(false);
        weldTechnicsEdit->setText(weldTechnicsName);
        appToSameTechnicsCheckBox->setEnabled(false);

        if(weldTechnicsName.left(2)==QString("__"))
        {
            weldTechnicsEdit->setEnabled(false);
            weldTechnicsComboBox->setEnabled(false);
            Z_hEdit->setEnabled(true);
            pointNameEdit->setEnabled(false);
            runSpeedEdit->setEnabled(false);
            preModeComboBox->setEnabled(false);
            weldModeComboBox->setEnabled(false);
            for(int i=0;i<8;i++)
            {
                technicsParaEdit1[i]->setEnabled(false);
                technicsParaEdit2[i]->setEnabled(false);
            }
        }
    }

    runSpeedEdit->setText(QString("%1").arg(pointWeldPara.atspeed,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(pointWeldPara.Dlt_X,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(pointWeldPara.Dlt_Y,0,'f',1));
    technicsParaEdit2[0]->setText(QString("%1").arg(pointWeldPara.Dlt_Z,0,'f',1));
    technicsParaEdit2[3]->setText(QString("%1").arg(pointWeldPara.Dlt_speed,0,'f',1));
    technicsParaEdit1[2]->setText(QString("%1").arg(pointWeldPara.FT_Pre_Length,0,'f',1));
    technicsParaEdit1[3]->setText(QString("%1").arg(pointWeldPara.FT_Pre_Speed,0,'f',1));
    preModeComboBox->setCurrentIndex(pointWeldPara.FT_Pre_Modle);
    weldModeComboBox->setCurrentIndex(pointWeldPara.Soldering_style);
    technicsParaEdit2[7]->setText(QString("%1").arg(pointWeldPara.ShakeSolder_Length,0,'f',1));
    technicsParaEdit1[7]->setText(QString("%1").arg(pointWeldPara.ShakeSolder_speed,0,'f',1));
    technicsParaEdit1[5]->setText(QString("%1").arg(pointWeldPara.S1_Length,0,'f',1));
    technicsParaEdit1[6]->setText(QString("%1").arg(pointWeldPara.S1_T_Length,0,'f',1));
    technicsParaEdit2[2]->setText(QString("%1").arg(pointWeldPara.S_speed,0,'f',1));
    technicsParaEdit2[5]->setText(QString("%1").arg(pointWeldPara.S2_Length,0,'f',1));
    technicsParaEdit2[6]->setText(QString("%1").arg(pointWeldPara.S2_T_Length,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TX,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TY,0,'f',1));
    technicsParaEdit2[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TZ,0,'f',1));
    technicsParaEdit1[4]->setText(QString("%1").arg(pointWeldPara.T1_Delay_time));
    technicsParaEdit2[4]->setText(QString("%1").arg(pointWeldPara.T2_Delay_time));
    //pointWeldPara.v_Over_Time_count=100000;
    if(type<2)
        startIn();
    else
    {
        resize(dlgWidth,dlgHeight);
        alpha=20;
        slowlyInOut(2);
    }

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(200,200);

    flagInitFinished=1;
}

bool PointWeldDlg::event(QEvent *e)
{
    if(!flagInitFinished) return QWidget::event(e);

    if(weldTechnicsEdit->hasFocus())
    {
        if(flagKB_On==0)
        {
            flagKB_On=1;
            QProcess p(0);
            p.start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
            p.waitForStarted();
            p.waitForFinished();
        }
    }
    return QWidget::event(e);
}

void PointWeldDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,alpha));
}
void PointWeldDlg::mousePressEvent(QMouseEvent */*event*/)
{
    //this->windowPos = this->pos();
    //this->mousePos = event->globalPos();
    //this->dPos = mousePos - windowPos;
}
void PointWeldDlg::mouseMoveEvent(QMouseEvent */*event*/)
{
    //this->move(event->globalPos() - this->dPos);
}

void PointWeldDlg::startIn() //飞入
{
    inoutFlag=1;

    timer->start(50);
}

void PointWeldDlg::startOut() //飞出
{
    inoutFlag=2;
    timer->start(50);
}

void PointWeldDlg::slowlyInOut(int flag) //使对话框透明或不透明 1=透明；2=不透明
{
    if(flag==1)
    {
        inoutFlag=3;
        timer2->start(50);
    }
    else if(flag==2)
    {
        inoutFlag=4;
        timer2->start(50);
    }
}

void PointWeldDlg::inout()
{
    if(inoutFlag==1)
    {
        flayX -= 50;
        if(flayX<20)
        {
            flayX=20;
        }
        flayY -= 20;
        if(flayY<80) flayY=80;
        flayWidth=flayWidth+dlgWidth/(fadeTimes/timer->interval());
        if(flayWidth>dlgWidth) flayWidth=dlgWidth;
        flayHeight=flayHeight+dlgHeight/(fadeTimes/timer->interval());
        if(flayHeight>dlgHeight) flayHeight=dlgHeight;
        resize(flayWidth,flayHeight);
        move(flayX,flayY);
    }
    else if(inoutFlag==2)
    {
        flayX += 50;
        if(flayX>beginPointX) flayX=beginPointX;
        flayY += 20;
        if(flayY>beginPointY) flayY=beginPointY;
        flayWidth=flayWidth-dlgWidth/(fadeTimes/timer->interval());
        if(flayWidth<100) flayWidth=100;
        flayHeight=flayHeight-dlgHeight/(fadeTimes/timer->interval());
        if(flayHeight<50) flayHeight=50;
        resize(flayWidth,flayHeight);
        move(flayX,flayY);
        if((flayX==beginPointX) && (flayY==beginPointY) && (flayWidth==100) && (flayHeight==50))
        {
            emit useMoveKeyDlg(2); //此信号触发关闭movetool键盘对话框
            if(isOK)
                done(QDialog::Accepted);
            else
                done(QDialog::Rejected);
        }
    }
    if((flayX==20) && (flayY==80) && (flayWidth==dlgWidth) && (flayHeight==dlgHeight))
    {
        inoutFlag=0;
        timer->stop();
        emit loadDlgFinished();     //触发信号，加载自定义文本框
        if(type==0) //新建焊点
        {
            emit useMoveKeyDlg(1);   //触发信号，打开movetool键盘
        }
    }
}

void PointWeldDlg::inout2()
{
    if(inoutFlag==3)
    {
        alpha-=20;
        if(alpha<=20)
        {
            alpha=20;
            inoutFlag=0;
            timer2->stop();
            if(type>1)
            {
                if(isOK)
                    done(QDialog::Accepted);
                else
                    done(QDialog::Rejected);
            }
        }
        update();
    }
    else if(inoutFlag==4)
    {
        alpha+=20;
        if(alpha>=230)
        {
            alpha=230;
            inoutFlag=0;
            timer2->stop();
            emit loadDlgFinished();     //触发信号，加载自定义文本框
        }
        update();
    }
}

void PointWeldDlg::createLeftLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    loadDefaultValBtn=new QPushButton(tr("导入默认值"));
    loadDefaultValBtn->setFont(ft);
    loadDefaultValBtn->setMinimumWidth(200);
    //loadDefaultValBtn->setSizePolicy(QSizePolicy::Preferred , QSizePolicy::Fixed);
    connect(loadDefaultValBtn, SIGNAL(clicked()), this, SLOT(OnLoadDefaultBtnClicked()));
    //copyParaValBtn=new QPushButton(tr("复制参数"));
    //copyParaValBtn->setFont(ft);
    //copyParaValBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //connect(copyParaValBtn, SIGNAL(clicked()), this, SLOT(OnCopyParaBtnClicked()));
    setDefaultValBtn=new QPushButton(tr("设为默认值"));
    setDefaultValBtn->setFont(ft);
    //setDefaultValBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(setDefaultValBtn, SIGNAL(clicked()), this, SLOT(OnSetDefaultBtnClicked()));

    ft.setPointSize(12);
    for(int i=0;i<6;i++)
        currPosEdit.append(new MyLineEdit);
    for(int i=0;i<6;i++)
        currPosEdit[i]->setFont(ft);

    QLabel *weldTechnicsLabel=new QLabel(tr("工艺名称"));
    weldTechnicsLabel->setFont(ft);
    QLabel *Z_hLabel=new QLabel(tr("Z提升到"));
    Z_hLabel->setFont(ft);
    QLabel *pointNameLabel=new QLabel(tr("焊点序号"));
    pointNameLabel->setFont(ft);
    QLabel *currentPosLabel=new QLabel(tr("当前位置坐标"));
    currentPosLabel->setFont(ft);

    weldTechnicsComboBox=new QComboBox;
    weldTechnicsComboBox->setFont(ft);
    weldTechnicsEdit=new QLineEdit;
    weldTechnicsEdit->setFont(ft);
    Z_hEdit=new MyLineEdit;
    Z_hEdit->setFont(ft);
    pointNameEdit=new QLineEdit;
    pointNameEdit->setFont(ft);

    QVBoxLayout *btnLayout = new QVBoxLayout;
    btnLayout->addWidget(loadDefaultValBtn);
    //btnLayout->addWidget(copyParaValBtn);
    btnLayout->addWidget(setDefaultValBtn);

    QGridLayout *paraLayout = new QGridLayout;
    paraLayout->addWidget(weldTechnicsLabel, 0, 0);
    if(type==1)
        paraLayout->addWidget(weldTechnicsComboBox, 0, 1);
    else
        paraLayout->addWidget(weldTechnicsEdit, 0, 1);
    paraLayout->addWidget(Z_hLabel, 1, 0);
    paraLayout->addWidget(Z_hEdit, 1, 1);
    paraLayout->addWidget(pointNameLabel, 2, 0);
    paraLayout->addWidget(pointNameEdit, 2, 1);

    QLabel *posLabel[6]={new QLabel(tr("X轴(mm)")),new QLabel(tr("Y轴(mm)")),new QLabel(tr("Z轴(mm)")),\
                        new QLabel(tr("R轴(度)")),new QLabel(tr("R1轴(度)")),new QLabel(tr("R2轴(度)"))};
    for(int i=0;i<6;i++)
        posLabel[i]->setFont(ft);
    QGridLayout *posLayout = new QGridLayout;
    posLayout->addWidget(posLabel[0], 0, 0);
    posLayout->addWidget(currPosEdit[0], 0, 1);
    posLayout->addWidget(posLabel[1], 1, 0);
    posLayout->addWidget(currPosEdit[1], 1, 1);
    posLayout->addWidget(posLabel[2], 2, 0);
    posLayout->addWidget(currPosEdit[2], 2, 1);
    posLayout->addWidget(posLabel[3], 3, 0);
    posLayout->addWidget(currPosEdit[3], 3, 1);
    posLayout->addWidget(posLabel[4], 4, 0);
    posLayout->addWidget(currPosEdit[4], 4, 1);
    posLayout->addWidget(posLabel[5], 5, 0);
    posLayout->addWidget(currPosEdit[5], 5, 1);

    QVBoxLayout *currPosLayout=new QVBoxLayout;
    currPosLayout->addWidget(currentPosLabel);
    currPosLayout->addLayout(posLayout);

    QGroupBox *pointWeldBox=new QGroupBox;
    pointWeldBox->setLayout(currPosLayout);
    if(type>1)
    {
        Z_hLabel->setEnabled(false);
        Z_hEdit->setEnabled(false);
        pointNameLabel->setEnabled(false);
        pointNameEdit->setEnabled(false);
        pointWeldBox->setEnabled(false);
    }

    ft.setPointSize(15);
    updatePosBtn=new QPushButton(tr("更新坐标"));
    updatePosBtn->setFont(ft);
    //updatePosBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(updatePosBtn, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtnClicked()));
    updatePosBtn->setEnabled(false);

    leftLayout=new QVBoxLayout;
    leftLayout->addLayout(btnLayout);
    leftLayout->addLayout(paraLayout);
    leftLayout->addWidget(pointWeldBox);
    leftLayout->addWidget(updatePosBtn);
}

void PointWeldDlg::createRightLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    for(int i=0;i<8;i++)
    {
        technicsParaEdit1.append(new MyLineEdit);
        technicsParaEdit2.append(new MyLineEdit);
    }
    for(int i=0;i<8;i++)
    {
        technicsParaEdit1[i]->setFont(ft);
        technicsParaEdit1[i]->setMaximumWidth(dlgWidth/8);
        technicsParaEdit2[i]->setFont(ft);
        technicsParaEdit2[i]->setMaximumWidth(dlgWidth/8);
    }

    QLabel *technicsParaLabel1[8]={new QLabel(tr("XY补偿(mm)")),new QLabel(tr("XY退锡补偿(mm)")),new QLabel(tr("预送锡长度(mm)")),new QLabel(tr("预送锡速度(mm/s)")),\
                        new QLabel(tr("预热时间(ms)")),new QLabel(tr("送锡长度1(mm)")),new QLabel(tr("退锡长度1(mm)")),new QLabel(tr("抖焊速度(mm/s)"))};
    QLabel *technicsParaLabel2[8]={new QLabel(tr("Z补偿(mm)")),new QLabel(tr("Z退锡补偿(mm)")),new QLabel(tr("送锡速度(mm/s)")),new QLabel(tr("补偿速度(mm/s)")),\
                        new QLabel(tr("停滞时间(ms)")),new QLabel(tr("送锡长度2(mm)")),new QLabel(tr("退锡长度2(mm)")),new QLabel(tr("抖焊距离(mm)"))};
    for(int i=0;i<8;i++)
    {
        technicsParaLabel1[i]->setFont(ft);
        technicsParaLabel1[i]->setMinimumWidth(dlgWidth/6);
        technicsParaLabel2[i]->setFont(ft);
        technicsParaLabel2[i]->setMinimumWidth(dlgWidth/6);
    }

    QLabel *runSpeedLabel=new QLabel(tr("运行速度(mm/s)"));
    runSpeedLabel->setFont(ft);
    QLabel *preModeLabel=new QLabel(tr("预送锡方式"));
    preModeLabel->setFont(ft);


    runSpeedEdit=new MyLineEdit;
    runSpeedEdit->setFont(ft);
    runSpeedEdit->setMaximumWidth(dlgWidth/8);

    QLabel *weldModeLabel=new QLabel(tr("抖焊方式"));
    weldModeLabel->setFont(ft);
    //ft.setPointSize(WELD_DLG_BUTTON_FONT);
    preModeComboBox=new QComboBox;
    preModeComboBox->setFont(ft);
    preModeComboBox->addItem(tr("移动开始送锡"));
    preModeComboBox->addItem(tr("补偿前完成送锡"));
    preModeComboBox->addItem(tr("补偿时开始送锡"));
    preModeComboBox->setCurrentText(0);
    //preModeComboBox->setMaximumWidth(dlgWidth/8);
    //preModeComboBox->setStyleSheet("QComboBox QAbstractItemView::item{height:50px; }");
    preModeComboBox->setStyleSheet("QComboBox { min-height: 20px; min-width: 120px; }");
    //preModeComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
    preModeComboBox->setMinimumContentsLength(18);
    //preModeComboBox->setMinimumHeight(50);

    weldModeComboBox=new QComboBox;
    weldModeComboBox->setFont(ft);
    weldModeComboBox->addItem(tr("不执行抖焊"));
    weldModeComboBox->addItem(tr("正常水平抖焊，终点结束"));
    weldModeComboBox->addItem(tr("正常垂直抖焊，终点结束"));
    weldModeComboBox->addItem(tr("定点水平抖焊，终点结束"));
    weldModeComboBox->addItem(tr("定点垂直抖焊，终点结束"));
    weldModeComboBox->addItem(tr("半定点水平抖焊，终点结束"));
    weldModeComboBox->addItem(tr("半定点垂直抖焊，终点结束"));
    weldModeComboBox->addItem(tr("正常水平抖焊，起点结束"));
    weldModeComboBox->addItem(tr("正常垂直抖焊，起点结束"));
    weldModeComboBox->addItem(tr("定点水平抖焊，起点结束"));
    weldModeComboBox->addItem(tr("定点垂直抖焊，起点结束"));
    weldModeComboBox->addItem(tr("半定点水平抖焊，起点结束"));
    weldModeComboBox->addItem(tr("半定点垂直抖焊，起点结束"));
    weldModeComboBox->setCurrentText(0);
    //weldModeComboBox->setMaximumWidth(dlgWidth/8);
    preModeComboBox->setMinimumContentsLength(10);

    appToSameTechnicsCheckBox=new QCheckBox(tr("将此工艺修改应用到项目中同类工艺项"));
    appToSameTechnicsCheckBox->setFont(ft);
    appToSameTechnicsCheckBox->setChecked(false);

    ft.setPointSize(15);
    envParaBtn=new QPushButton(tr("工艺环境"));
    envParaBtn->setFont(ft);
    connect(envParaBtn, SIGNAL(clicked()), this, SLOT(OnEnvParaBtnClicked()));

    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    cylinderCheckBox=new QCheckBox(tr("气缸使能"));
    cylinderCheckBox->setFont(ft);
    cylinderCheckBox->setChecked(false);

    paraLayout = new QGridLayout;
    for(int i=0;i<8;i++)
    {
        paraLayout->addWidget(technicsParaLabel1[i],i,0);
        paraLayout->addWidget(technicsParaEdit1[i],i,1);
        paraLayout->addWidget(technicsParaLabel2[i],i,2);
        paraLayout->addWidget(technicsParaEdit2[i],i,3);
    }
    paraLayout->addWidget(runSpeedLabel,8,0);
    paraLayout->addWidget(runSpeedEdit,8,1);
    paraLayout->addWidget(preModeLabel,8,2);
    paraLayout->addWidget(preModeComboBox,8,3);
    paraLayout->addWidget(weldModeLabel,9,0);
    paraLayout->addWidget(weldModeComboBox,9,1,1,2);
    paraLayout->addWidget(appToSameTechnicsCheckBox,10,0,1,3);
    paraLayout->addWidget(envParaBtn,11,1,1,2);
    //paraLayout->addWidget(cylinderCheckBox,9,3);
}

void PointWeldDlg::createBottomLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);//WELD_DLG_BUTTON_FONT
    ft.setItalic(false);
/*
    currItemComboBox=new QComboBox;
    currItemComboBox->setFont(ft);
    currItemComboBox->addItem(tr("修改当前项"));
    currItemComboBox->addItem(tr("1"));
    currItemComboBox->addItem(tr("2"));
    currItemComboBox->setCurrentIndex(0);
    //currItemComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
*/
    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    //okBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    //cancelBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    bottomLayout=new QHBoxLayout;
    //bottomLayout->addStretch();
    bottomLayout->addWidget(okBtn);
    //bottomLayout->addStretch();
    bottomLayout->addWidget(cancelBtn);
    //bottomLayout->addStretch();
}

void PointWeldDlg::selectPointWeldPara(const QString &pointWeldTechName)
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
        db.closeDB();
        return;
    }
    db.ReadPointWeldFromDb(pointWeldTechName,pointWeldPara);
    db.closeDB();

    runSpeedEdit->setText(QString("%1").arg(pointWeldPara.atspeed,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(pointWeldPara.Dlt_X,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(pointWeldPara.Dlt_Y,0,'f',1));
    technicsParaEdit2[0]->setText(QString("%1").arg(pointWeldPara.Dlt_Z,0,'f',1));
    technicsParaEdit2[3]->setText(QString("%1").arg(pointWeldPara.Dlt_speed,0,'f',1));
    technicsParaEdit1[2]->setText(QString("%1").arg(pointWeldPara.FT_Pre_Length,0,'f',1));
    technicsParaEdit1[3]->setText(QString("%1").arg(pointWeldPara.FT_Pre_Speed,0,'f',1));
    preModeComboBox->setCurrentIndex(pointWeldPara.FT_Pre_Modle);
    weldModeComboBox->setCurrentIndex(pointWeldPara.Soldering_style);
    technicsParaEdit2[7]->setText(QString("%1").arg(pointWeldPara.ShakeSolder_Length,0,'f',1));
    technicsParaEdit1[7]->setText(QString("%1").arg(pointWeldPara.ShakeSolder_speed,0,'f',1));
    technicsParaEdit1[5]->setText(QString("%1").arg(pointWeldPara.S1_Length,0,'f',1));
    technicsParaEdit1[6]->setText(QString("%1").arg(pointWeldPara.S1_T_Length,0,'f',1));
    technicsParaEdit2[2]->setText(QString("%1").arg(pointWeldPara.S_speed,0,'f',1));
    technicsParaEdit2[5]->setText(QString("%1").arg(pointWeldPara.S2_Length,0,'f',1));
    technicsParaEdit2[6]->setText(QString("%1").arg(pointWeldPara.S2_T_Length,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TX,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TY,0,'f',1));
    technicsParaEdit2[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TZ,0,'f',1));
    technicsParaEdit1[4]->setText(QString("%1").arg(pointWeldPara.T1_Delay_time));
    technicsParaEdit2[4]->setText(QString("%1").arg(pointWeldPara.T2_Delay_time));
}

void PointWeldDlg::OnLoadDefaultBtnClicked()
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
    db.ReadPointWeldFromDb(QString("default"),pointWeldPara);//

    Z_hEdit->setText(QString("%1").arg(pointWeldPara.ZH,0,'f',1));
    runSpeedEdit->setText(QString("%1").arg(pointWeldPara.atspeed,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(pointWeldPara.Dlt_X,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(pointWeldPara.Dlt_Y,0,'f',1));
    technicsParaEdit2[0]->setText(QString("%1").arg(pointWeldPara.Dlt_Z,0,'f',1));
    technicsParaEdit2[3]->setText(QString("%1").arg(pointWeldPara.Dlt_speed,0,'f',1));
    technicsParaEdit1[2]->setText(QString("%1").arg(pointWeldPara.FT_Pre_Length,0,'f',1));
    technicsParaEdit1[3]->setText(QString("%1").arg(pointWeldPara.FT_Pre_Speed,0,'f',1));
    preModeComboBox->setCurrentIndex(pointWeldPara.FT_Pre_Modle);
    weldModeComboBox->setCurrentIndex(pointWeldPara.Soldering_style);
    technicsParaEdit2[7]->setText(QString("%1").arg(pointWeldPara.ShakeSolder_Length,0,'f',1));
    technicsParaEdit1[7]->setText(QString("%1").arg(pointWeldPara.ShakeSolder_speed,0,'f',1));
    technicsParaEdit1[5]->setText(QString("%1").arg(pointWeldPara.S1_Length,0,'f',1));
    technicsParaEdit1[6]->setText(QString("%1").arg(pointWeldPara.S1_T_Length,0,'f',1));
    technicsParaEdit2[2]->setText(QString("%1").arg(pointWeldPara.S_speed,0,'f',1));
    technicsParaEdit2[5]->setText(QString("%1").arg(pointWeldPara.S2_Length,0,'f',1));
    technicsParaEdit2[6]->setText(QString("%1").arg(pointWeldPara.S2_T_Length,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TX,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TY,0,'f',1));
    technicsParaEdit2[1]->setText(QString("%1").arg(pointWeldPara.Dlt_TZ,0,'f',1));
    technicsParaEdit1[4]->setText(QString("%1").arg(pointWeldPara.T1_Delay_time));
    technicsParaEdit2[4]->setText(QString("%1").arg(pointWeldPara.T2_Delay_time));
}
/*
void PointWeldDlg::OnCopyParaBtnClicked()
{

}
*/
void PointWeldDlg::OnSetDefaultBtnClicked()
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

    pointWeldPara.ZH=Z_hEdit->text().trimmed().toDouble();
    pointWeldPara.atspeed=runSpeedEdit->text().trimmed().toDouble();
    pointWeldPara.Dlt_X=technicsParaEdit1[0]->text().trimmed().toDouble();
    pointWeldPara.Dlt_Y=technicsParaEdit1[0]->text().trimmed().toDouble();
    pointWeldPara.Dlt_Z=technicsParaEdit2[0]->text().trimmed().toDouble();
    pointWeldPara.Dlt_speed=technicsParaEdit2[3]->text().trimmed().toDouble();
    pointWeldPara.FT_Pre_Length=technicsParaEdit1[2]->text().trimmed().toDouble();
    pointWeldPara.FT_Pre_Speed=technicsParaEdit1[3]->text().trimmed().toDouble();
    pointWeldPara.FT_Pre_Modle=preModeComboBox->currentIndex();
    pointWeldPara.Soldering_style=weldModeComboBox->currentIndex();
    pointWeldPara.ShakeSolder_Length=technicsParaEdit2[7]->text().trimmed().toDouble();
    pointWeldPara.ShakeSolder_speed=technicsParaEdit1[7]->text().trimmed().toDouble();
    pointWeldPara.S1_Length=technicsParaEdit1[5]->text().trimmed().toDouble();
    pointWeldPara.S1_T_Length=technicsParaEdit1[6]->text().trimmed().toDouble();
    pointWeldPara.S_speed=technicsParaEdit2[2]->text().trimmed().toDouble();
    pointWeldPara.S2_Length=technicsParaEdit2[5]->text().trimmed().toDouble();
    pointWeldPara.S2_T_Length=technicsParaEdit2[6]->text().trimmed().toDouble();
    pointWeldPara.Dlt_TX=technicsParaEdit1[1]->text().trimmed().toDouble();
    pointWeldPara.Dlt_TY=technicsParaEdit1[1]->text().trimmed().toDouble();
    pointWeldPara.Dlt_TZ=technicsParaEdit2[1]->text().trimmed().toDouble();
    pointWeldPara.T1_Delay_time=technicsParaEdit1[4]->text().trimmed().toDouble();
    pointWeldPara.T2_Delay_time=technicsParaEdit2[4]->text().trimmed().toDouble();
    pointWeldPara.v_Over_Time_count=100000;
    db.SavePointWeldToDb(QString("default"),pointWeldPara);
}
void PointWeldDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
}

void PointWeldDlg::OnUpdatePosBtnClicked()
{
    for(int i=0;i<4;i++)
        currPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    currPosEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
    currPosEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',2));
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        currPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
}

void PointWeldDlg::OnEnvParaBtnClicked()
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

    QString paraName;
    if(type==1)
        paraName=weldTechnicsComboBox->currentText();
    else
        paraName=weldTechnicsEdit->text();
    if(type==2) //新建点焊工艺参数
    {
        if(paraName.left(2)==QString("__"))
        {
            msgBox.setText(tr("用户自定义工艺参数名称不能以两个下划线开头."));
            msgBox.exec();
            return;
        }
    }

    EnvParaDlg *dlg=new EnvParaDlg(this);
    dlg->type=type;
    dlg->technicsType=0;    //点焊
    dlg->paraName=paraName;
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
    delete dlg;
}

void PointWeldDlg::OnOkBtnClicked()
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

    if(type==1)
        weldTechnicsName=weldTechnicsComboBox->currentText();
    else
        weldTechnicsName=weldTechnicsEdit->text().trimmed();

    if(weldTechnicsName.isEmpty())
    {
        msgBox.setText(tr("请录入工艺参数名称."));
        msgBox.exec();
        return;
    }

    /////////////////////保存工艺环境参数////////////////////////////////////
    //焊盘
    ENV_PARA envPara;   //工艺环境参数
    envPara.HanPan_HD=35;           //厚度
    envPara.HanPan_BMCL=tr("镀锡");  //表面处理
    envPara.HanPan_FTXS=tr("无");   //覆铜形式
    envPara.HanPan_ZJ=3.5;          //直径
    envPara.HanPan_KJ=2.8;          //孔径
    //焊锡丝
    envPara.HanXiSi_ZJ=0.3;         //直径
    envPara.HanXiSi_ZL=tr("镍锡");   //种类
    envPara.HanXiSi_ZHJ=2;          //助焊剂含量
    envPara.HanXiSi_FW=tr("前上");   //方位
    //烙铁
    envPara.LaoTie_WD=380;          //温度
    envPara.LaoTie_ZL=tr("08D");//种类

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    else
    {
        if(!db.ReadEnvParaFromDb(weldTechnicsName,envPara)) //如果数据库原来没有该工艺名称的环境参数
        {
            if(!db.SaveEnvParaToDb(weldTechnicsName,envPara))   //保存一组默认的环境参数
            {
                msgBox.setText(tr("保存工艺环境参数失败."));
                msgBox.exec();
            }
        }
        db.closeDB();
    }
    //////////////////////////////////////////////////////////////////////

    if(type==2) //新建点焊工艺参数
    {
        if(weldTechnicsName.left(2)==QString("__"))
        {
            msgBox.setText(tr("用户自定义工艺参数名称不能以两个下划线开头."));
            msgBox.exec();
            return;
        }
    }

    Z_h=Z_hEdit->text().trimmed().toDouble();
    pointPos[0]=currPosEdit[0]->text().trimmed().toDouble();
    pointPos[1]=currPosEdit[1]->text().trimmed().toDouble();
    pointPos[2]=currPosEdit[2]->text().trimmed().toDouble();
    pointPos[3]=currPosEdit[3]->text().trimmed().toDouble();
    pointPos[5]=currPosEdit[4]->text().trimmed().toDouble();
    pointPos[6]=currPosEdit[5]->text().trimmed().toDouble();
    cylinderEnable=cylinderCheckBox->isChecked();
    pointWeldPara.x=pointPos[0];
    pointWeldPara.y=pointPos[1];
    pointWeldPara.z=pointPos[2];
    pointWeldPara.r=pointPos[3];
    pointWeldPara.ZH=Z_h;
    pointWeldPara.atspeed=runSpeedEdit->text().trimmed().toDouble();
    pointWeldPara.Dlt_X=technicsParaEdit1[0]->text().trimmed().toDouble();
    pointWeldPara.Dlt_Y=technicsParaEdit1[0]->text().trimmed().toDouble();
    pointWeldPara.Dlt_Z=technicsParaEdit2[0]->text().trimmed().toDouble();
    pointWeldPara.Dlt_speed=technicsParaEdit2[3]->text().trimmed().toDouble();
    pointWeldPara.FT_Pre_Length=technicsParaEdit1[2]->text().trimmed().toDouble();
    pointWeldPara.FT_Pre_Speed=technicsParaEdit1[3]->text().trimmed().toDouble();
    pointWeldPara.FT_Pre_Modle=preModeComboBox->currentIndex();
    pointWeldPara.Soldering_style=weldModeComboBox->currentIndex();
    pointWeldPara.ShakeSolder_Length=technicsParaEdit2[7]->text().trimmed().toDouble();
    pointWeldPara.ShakeSolder_speed=technicsParaEdit1[7]->text().trimmed().toDouble();
    pointWeldPara.S1_Length=technicsParaEdit1[5]->text().trimmed().toDouble();
    pointWeldPara.S1_T_Length=technicsParaEdit1[6]->text().trimmed().toDouble();
    pointWeldPara.S_speed=technicsParaEdit2[2]->text().trimmed().toDouble();
    pointWeldPara.S2_Length=technicsParaEdit2[5]->text().trimmed().toDouble();
    pointWeldPara.S2_T_Length=technicsParaEdit2[6]->text().trimmed().toDouble();
    pointWeldPara.Dlt_TX=technicsParaEdit1[1]->text().trimmed().toDouble();
    pointWeldPara.Dlt_TY=technicsParaEdit1[1]->text().trimmed().toDouble();
    pointWeldPara.Dlt_TZ=technicsParaEdit2[1]->text().trimmed().toDouble();
    pointWeldPara.T1_Delay_time=technicsParaEdit1[4]->text().trimmed().toDouble();
    pointWeldPara.T2_Delay_time=technicsParaEdit2[4]->text().trimmed().toDouble();
    pointWeldPara.v_Over_Time_count=600000;
    appToSameTechnicsChecked=appToSameTechnicsCheckBox->isChecked();

    MyRWxml xml;
    SYSTEM_PARAMS systemPara;
    xml.readSysPara("robot.xml",systemPara);
    double xx=pointWeldPara.x+pointWeldPara.Dlt_X*cos(PI*pointWeldPara.r/180);
    double yy=pointWeldPara.y+pointWeldPara.Dlt_Y*sin(PI*pointWeldPara.r/180);
    double zz=pointWeldPara.z-pointWeldPara.Dlt_Z;
    double zz_ZH=pointWeldPara.z-Z_h;

    QString strErr,strErrX,strErrY,strErrZ,strErrZh,strErrR,strErrSpeed;
    if(xx<0 || xx>systemPara.limitPosLen[0])
    {
        strErrX=QString(tr(" X坐标补偿错误 "));
        strErr+=strErrX;
    }
    if(yy<0 || yy>systemPara.limitPosLen[1])
    {
        strErrY=QString(tr(" Y坐标补偿错误 "));
        strErr+=strErrY;
    }
    if(zz<0 || zz>systemPara.limitPosLen[2])
    {
        strErrZ=QString(tr(" Z坐标补偿错误 "));
        strErr+=strErrZ;
    }
    else if(zz_ZH<0 || Z_h<5.0 || zz_ZH>systemPara.limitPosLen[2])
    {
        strErrZh=QString(tr(" Z坐标提升值错误 "));
        strErr+=strErrZh;
    }
    if(pointWeldPara.r<systemPara.limitPosLen[4] || pointWeldPara.r>systemPara.limitPosLen[3])
    {
        strErrR=QString(tr(" R坐标值错误 "));
        strErr+=strErrR;
    }
    if(pointWeldPara.atspeed<0 || pointWeldPara.atspeed>ROBOT_MAX_ATSPEED)
    {
        strErrSpeed=QString(tr(" 运行速度值超出正常范围 "));
        strErr+=strErrSpeed;
    }
    if(type<2)
    {
        if(!strErr.isEmpty())
        {
            msgBox.setText(tr("系统发现")+strErr+tr("请重新修正参数！"));
            msgBox.exec();
            return;
        }
    }

    isOK=true;
    if(type<2)
        startOut();
    else
        slowlyInOut(1);
}

void PointWeldDlg::OnCancelBtnClicked()
{
    isOK=false;
    if(type<2)
        startOut();
    else
        slowlyInOut(1);
}


void PointWeldDlg::loadMyLineEdit()      //加载自定义文本框
{
    Z_hEdit->context=context;
    QRect rect=Z_hEdit->geometry();
    Z_hEdit->myPos=QPoint(rect.left(),rect.top());
    Z_hEdit->parentHeight=geometry().height();
    runSpeedEdit->context=context;
    rect=runSpeedEdit->geometry();
    runSpeedEdit->myPos=QPoint(rect.left(),rect.top());
    runSpeedEdit->parentHeight=geometry().height();
    for(int i=0;i<6;i++)
    {
        currPosEdit[i]->context=context;
        QRect rect=currPosEdit[i]->geometry();
        currPosEdit[i]->myPos=currPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        currPosEdit[i]->parentHeight=geometry().height();
    }
/*    for(int i=0;i<4;i++)
    {
        startPosEdit[i]->context=context;
        QRect rect=startPosEdit[i]->geometry();
        startPosEdit[i]->myPos=startPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        startPosEdit[i]->parentHeight=geometry().height();

        endPosEdit[i]->context=context;
        rect=endPosEdit[i]->geometry();
        endPosEdit[i]->myPos=endPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        endPosEdit[i]->parentHeight=geometry().height();
    }*/
    for(int i=0;i<8;i++)
    {
        technicsParaEdit1[i]->context=context;
        QRect rect=technicsParaEdit1[i]->geometry();
        technicsParaEdit1[i]->myPos=QPoint(rect.left(),rect.top());
        technicsParaEdit1[i]->parentHeight=geometry().height();
        technicsParaEdit2[i]->context=context;
        rect=technicsParaEdit2[i]->geometry();
        technicsParaEdit2[i]->myPos=QPoint(rect.left(),rect.top());
        technicsParaEdit2[i]->parentHeight=geometry().height();
    }
}
