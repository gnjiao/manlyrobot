#include "linewelddlg.h"
#include "msqldb.h"
#include "myrwxml.h"
#include "envparadlg.h"

LineWeldDlg::LineWeldDlg(QWidget *parent) :
    QDialog(parent)
{
    Z_h=0;
    pointNum=0;
    alpha=230;
    dlgWidth=730;
    dlgHeight=480;
    for(int i=0;i<8;i++) pointPos[i]=0;
    for(int i=0;i<8;i++) currPointPos[i]=0;
    for(int i=0;i<8;i++) movetoolPos[i]=0;
    flagKB_On=0;
    flagInitFinished=0;
    lineWeldType=0;
    ironPos[0]=0;
    ironPos[1]=0;
}
void LineWeldDlg::initDialog()
{
    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QLineEdit { background-color: QColor(255,255,0,10) ; color:white; border:1px groove white;} \
                  QPushButton { background-color: qconicalgradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(214, 214, 214, 200), stop:0.5 rgba(116, 116, 116, 200), stop:1 rgba(155, 234, 251, 160)); \
                                border: 1px solid rgb(124, 124, 124); border-radius:10px; color:black;}"
                    "QPushButton:pressed{ background:rgba(0,0,255,150); }" \
                    "QPushButton:disabled{ background:qconicalgradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(214, 214, 214, 100), stop:0.5 rgba(36, 36, 36, 100), stop:1 rgba(214, 214, 214, 100)); }");

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    createPointPosBox();
    createLeftLayout();
    createRightLayout();
    createBottomLayout();

    pointPosBox->setMaximumHeight(150);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addLayout(paraLayout);
    rightLayout->addWidget(pointPosBox);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(layout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    this->setFont(ft);
    setWindowTitle(tr("拖焊设置"));
    connect(weldTechnicsComboBox, SIGNAL(currentIndexChanged(const QString & )), this, SLOT(selectLineWeldPara(const QString &)));
    connect(this, SIGNAL(loadDlgFinished()),this, SLOT(loadMyLineEdit()));
    //connect(weldTechnicsEdit, SIGNAL(clicked()),this, SLOT(onClickedTechnicsEdit()));
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(inout()));
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()),this, SLOT(inout2()));
    fadeTimes = 1000;
    beginPointX=flayX;
    beginPointY=flayY;

    if(type==0)     //新建拖焊焊点
    {
        weldTechnicsEdit->setEnabled(false);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(false);
        loadDefaultValBtn->setEnabled(false);
        //copyParaValBtn->setEnabled(false);
        setDefaultValBtn->setEnabled(false);
        //OnUpdatePosBtnClicked();

        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(false);
            technicsParaEdit2[i]->setEnabled(false);
        }
        for(int i=0;i<4;i++)
        {
            startPosEdit[i]->setText(QString("%1").arg(movetoolPos[i],0,'f',2));
            endPosEdit[i]->setText(QString("%1").arg(movetoolPos[i+4],0,'f',2));
        }
        if((g_MacType==3 || g_MacType==1 || g_MacType==4)) //6轴
        {
            startPosEdit[4]->setText(QString("%1").arg(ironPos[0],0,'f',2));
            startPosEdit[5]->setText(QString("%1").arg(ironPos[1],0,'f',2));
            endPosEdit[4]->setText(QString("%1").arg(ironPos[0],0,'f',2));
            endPosEdit[5]->setText(QString("%1").arg(ironPos[1],0,'f',2));
        }
        weldTechnicsEdit->setText(weldTechnicsName);
        Z_hEdit->setText(QString("%1").arg(Z_h,0,'f',1));
        pointNameEdit->setText(QString("%1").arg(pointNum));
        appToSameTechnicsCheckBox->setEnabled(false);
    }
    else if(type==1)    //编辑拖焊焊点
    {
        weldTechnicsEdit->setEnabled(false);
        weldTechnicsComboBox->setEnabled(true);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(false);
        loadDefaultValBtn->setEnabled(true);
        //copyParaValBtn->setEnabled(true);
        setDefaultValBtn->setEnabled(true);
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(false);
            technicsParaEdit2[i]->setEnabled(false);
        }
        for(int i=0;i<4;i++)
        {
            startPosEdit[i]->setText(QString("%1").arg(pointPos[i],0,'f',2));
            endPosEdit[i]->setText(QString("%1").arg(pointPos[i+4],0,'f',2));
        }
        if((g_MacType==3 || g_MacType==1 || g_MacType==4)) //6轴
        {
            startPosEdit[4]->setText(QString("%1").arg(ironPos[0],0,'f',2));
            startPosEdit[5]->setText(QString("%1").arg(ironPos[1],0,'f',2));
            endPosEdit[4]->setText(QString("%1").arg(ironPos[0],0,'f',2));
            endPosEdit[5]->setText(QString("%1").arg(ironPos[1],0,'f',2));
        }
        weldTechnicsEdit->setText(weldTechnicsName);
        weldTechnicsComboBox->setCurrentText(weldTechnicsName);
        Z_hEdit->setText(QString("%1").arg(Z_h,0,'f',1));
        pointNameEdit->setText(QString("%1").arg(pointNum));

        appToSameTechnicsCheckBox->setEnabled(true);
        old_WeldTechnicsName=weldTechnicsName;
        old_Z_h=Z_h;
    }
    else if(type==2)    //新建拖焊工艺参数
    {
        weldTechnicsEdit->setEnabled(true);
        weldTechnicsComboBox->setEnabled(false);
        Z_hEdit->setEnabled(false);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(true);
        loadDefaultValBtn->setEnabled(true);
        //copyParaValBtn->setEnabled(true);
        setDefaultValBtn->setEnabled(true);
        pointPosBox->setEnabled(false);
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(true);
            technicsParaEdit2[i]->setEnabled(true);
        }
        lineWeldRdoBtn->setDisabled(true);
        manyLineWeldRdoBtn1->setDisabled(true);
        manyLineWeldRdoBtn2->setDisabled(true);
        manyLineWeldRdoBtn3->setDisabled(true);
        appToSameTechnicsCheckBox->setEnabled(false);
    }
    else if(type==3)    //编辑拖焊工艺参数
    {
        weldTechnicsEdit->setEnabled(false);
        weldTechnicsComboBox->setEnabled(false);
        Z_hEdit->setEnabled(false);
        pointNameEdit->setEnabled(false);
        runSpeedEdit->setEnabled(true);
        loadDefaultValBtn->setEnabled(true);
        //copyParaValBtn->setEnabled(true);
        setDefaultValBtn->setEnabled(true);
        pointPosBox->setEnabled(false);
        for(int i=0;i<8;i++)
        {
            technicsParaEdit1[i]->setEnabled(true);
            technicsParaEdit2[i]->setEnabled(true);
        }
        /*for(int i=0;i<6;i++)
            currPosEdit[i]->setEnabled(false);*/
        weldTechnicsEdit->setText(weldTechnicsName);
        lineWeldRdoBtn->setDisabled(true);
        manyLineWeldRdoBtn1->setDisabled(true);
        manyLineWeldRdoBtn2->setDisabled(true);
        manyLineWeldRdoBtn3->setDisabled(true);
        appToSameTechnicsCheckBox->setEnabled(false);

        if(weldTechnicsName.left(2)==QString("__"))
        {
            weldTechnicsEdit->setEnabled(false);
            pointNameEdit->setEnabled(false);
            runSpeedEdit->setEnabled(false);
            for(int i=0;i<8;i++)
            {
                technicsParaEdit1[i]->setEnabled(false);
                technicsParaEdit2[i]->setEnabled(false);
            }
        }
    }

    if(lineWeldType==0)
    {
        lineWeldRdoBtn->setChecked(true);
        manyLineWeldRdoBtn1->setChecked(false);
        manyLineWeldRdoBtn2->setChecked(false);
        manyLineWeldRdoBtn3->setChecked(false);
    }
    else if(lineWeldType==1)
    {
        lineWeldRdoBtn->setChecked(false);
        manyLineWeldRdoBtn1->setChecked(true);
        manyLineWeldRdoBtn2->setChecked(false);
        manyLineWeldRdoBtn3->setChecked(false);
    }
    else if(lineWeldType==2)
    {
        lineWeldRdoBtn->setChecked(false);
        manyLineWeldRdoBtn1->setChecked(false);
        manyLineWeldRdoBtn2->setChecked(true);
        manyLineWeldRdoBtn3->setChecked(false);
    }
    else if(lineWeldType==3)
    {
        lineWeldRdoBtn->setChecked(false);
        manyLineWeldRdoBtn1->setChecked(false);
        manyLineWeldRdoBtn2->setChecked(false);
        manyLineWeldRdoBtn3->setChecked(true);
    }

    runSpeedEdit->setText(QString("%1").arg(lineWeldPara.atspeed,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(lineWeldPara.Dlt_X,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(lineWeldPara.Dlt_Y,0,'f',1));
    technicsParaEdit2[0]->setText(QString("%1").arg(lineWeldPara.Dlt_Z,0,'f',1));
    technicsParaEdit2[3]->setText(QString("%1").arg(lineWeldPara.Dlt_speed,0,'f',1));
    technicsParaEdit1[2]->setText(QString("%1").arg(lineWeldPara.FT_Pre_Length,0,'f',1));
    technicsParaEdit1[3]->setText(QString("%1").arg(lineWeldPara.FT_Pre_Speed,0,'f',1));
    technicsParaEdit2[2]->setText(QString("%1").arg(lineWeldPara.TH_speed,0,'f',1));
    technicsParaEdit1[5]->setText(QString("%1").arg(lineWeldPara.S1_Length,0,'f',1));
    technicsParaEdit1[6]->setText(QString("%1").arg(lineWeldPara.S1_T_Length,0,'f',1));
    technicsParaEdit1[7]->setText(QString("%1").arg(lineWeldPara.S1_speed,0,'f',1));
    technicsParaEdit2[5]->setText(QString("%1").arg(lineWeldPara.S2_Length,0,'f',1));
    technicsParaEdit2[6]->setText(QString("%1").arg(lineWeldPara.S2_T_Length,0,'f',1));
    technicsParaEdit2[7]->setText(QString("%1").arg(lineWeldPara.S2_speed,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TX,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TY,0,'f',1));
    technicsParaEdit2[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TZ,0,'f',1));
    technicsParaEdit1[4]->setText(QString("%1").arg(lineWeldPara.T1_Delay_time));
    technicsParaEdit2[4]->setText(QString("%1").arg(lineWeldPara.T2_Delay_time));

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

void LineWeldDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,alpha));
}
void LineWeldDlg::mousePressEvent(QMouseEvent */*event*/)
{
    //this->windowPos = this->pos();
    //this->mousePos = event->globalPos();
    //this->dPos = mousePos - windowPos;
}
void LineWeldDlg::mouseMoveEvent(QMouseEvent */*event*/)
{
    //this->move(event->globalPos() - this->dPos);
}

bool LineWeldDlg::event(QEvent *e)
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

void LineWeldDlg::startIn() //飞入
{
    inoutFlag=1;

    timer->start(50);
}

void LineWeldDlg::startOut() //飞出
{
    inoutFlag=2;
    timer->start(50);
}

void LineWeldDlg::slowlyInOut(int flag) //使对话框透明或不透明 1=透明；2=不透明
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

void LineWeldDlg::inout()
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
        emit loadDlgFinished();     //触发信号，加载自定义文本框
        timer->stop();
        if(type==0) //新建焊点
        {
            emit useMoveKeyDlg(1);   //触发信号，打开movetool键盘
        }
    }
}

void LineWeldDlg::inout2()
{
    if(inoutFlag==3)
    {
        alpha-=20;
        if(alpha<20)
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
        if(alpha>230)
        {
            alpha=230;
            inoutFlag=0;
            timer2->stop();
            emit loadDlgFinished();     //触发信号，加载自定义文本框
        }
        update();
    }
}

void LineWeldDlg::createLeftLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    loadDefaultValBtn=new QPushButton(tr("导入默认值"));
    loadDefaultValBtn->setFont(ft);
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
    pointNameEdit=new MyLineEdit;
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

    ft.setPointSize(12);
    /*
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

    ft.setPointSize(15);
    QPushButton * updatePosBtn=new QPushButton(tr("更新坐标"));
    updatePosBtn->setFont(ft);
    //updatePosBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(updatePosBtn, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtnClicked()));
    */
    lineWeldRdoBtn=new QRadioButton(tr("普通拖焊"));
    lineWeldRdoBtn->setFont(ft);
    lineWeldRdoBtn->setChecked(true);
    manyLineWeldRdoBtn1=new QRadioButton(tr("多段拖焊首段"));
    manyLineWeldRdoBtn1->setFont(ft);
    manyLineWeldRdoBtn1->setChecked(false);
    manyLineWeldRdoBtn2=new QRadioButton(tr("多段拖焊中段"));
    manyLineWeldRdoBtn2->setFont(ft);
    manyLineWeldRdoBtn2->setChecked(false);
    manyLineWeldRdoBtn3=new QRadioButton(tr("多段拖焊末段"));
    manyLineWeldRdoBtn3->setFont(ft);
    manyLineWeldRdoBtn3->setChecked(false);

    QVBoxLayout *lwTypeLayout=new QVBoxLayout;
    lwTypeLayout->addWidget(lineWeldRdoBtn);
    lwTypeLayout->addWidget(manyLineWeldRdoBtn1);
    lwTypeLayout->addWidget(manyLineWeldRdoBtn2);
    lwTypeLayout->addWidget(manyLineWeldRdoBtn3);

    QGroupBox *weldTypeBox=new QGroupBox;
    weldTypeBox->setLayout(lwTypeLayout);

    if(type>1)
    {
        Z_hLabel->setEnabled(false);
        Z_hEdit->setEnabled(false);
        pointNameLabel->setEnabled(false);
        pointNameEdit->setEnabled(false);
    }

    leftLayout=new QVBoxLayout;
    leftLayout->addLayout(btnLayout);
    leftLayout->addLayout(paraLayout);
    leftLayout->addWidget(weldTypeBox);
}

void LineWeldDlg::createRightLayout()
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
        technicsParaEdit2[i]->setFont(ft);
    }

    QLabel *technicsParaLabel1[8]={new QLabel(tr("XY补偿(mm)")),new QLabel(tr("XY退锡补偿(mm)")),new QLabel(tr("预送锡长度(mm)")),new QLabel(tr("预送锡速度(mm/s)")),\
                        new QLabel(tr("预热时间(ms)")),new QLabel(tr("送锡区间1(%)")),new QLabel(tr("退锡速度(mm/s)")),new QLabel(tr("送锡速度1(mm/s)"))};
    QLabel *technicsParaLabel2[8]={new QLabel(tr("Z补偿(mm)")),new QLabel(tr("Z退锡补偿(mm)")),new QLabel(tr("拖焊速度(mm/s)")),new QLabel(tr("补偿速度(mm/s)")),\
                        new QLabel(tr("停滞时间(ms)")),new QLabel(tr("送锡区间2(%)")),new QLabel(tr("退锡长度(mm)")),new QLabel(tr("送锡速度2(mm/s)"))};
    for(int i=0;i<8;i++)
    {
        technicsParaLabel1[i]->setFont(ft);
        technicsParaLabel2[i]->setFont(ft);
    }

    QLabel *runSpeedLabel=new QLabel(tr("运行速度"));
    runSpeedLabel->setFont(ft);
    runSpeedEdit=new MyLineEdit;
    runSpeedEdit->setFont(ft);


    appToSameTechnicsCheckBox=new QCheckBox(tr("将此工艺修改应用到项目中同类工艺项"));
    appToSameTechnicsCheckBox->setFont(ft);
    appToSameTechnicsCheckBox->setChecked(false);

    ft.setPointSize(14);
    envParaBtn=new QPushButton(tr("工艺环境"));
    envParaBtn->setFont(ft);
    connect(envParaBtn, SIGNAL(clicked()), this, SLOT(OnEnvParaBtnClicked()));

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
    paraLayout->addWidget(runSpeedEdit,8,1,1,1);
    paraLayout->addWidget(appToSameTechnicsCheckBox,9,0,1,3);
    paraLayout->addWidget(envParaBtn,10,1,1,2);
    //paraLayout->addWidget(cylinderCheckBox,8,3);
    //paraLayout->addWidget(sLabel,8,0,1,4);//,1,4
    //paraLayout->addLayout(posLayout1,9,0,1,4);
    //paraLayout->addWidget(eLabel,10,0,1,4);//,1,4
    //paraLayout->addLayout(posLayout2,11,0,1,4);
}

void LineWeldDlg::createBottomLayout()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);

    currItemComboBox=new QComboBox;
    currItemComboBox->setFont(ft);
    currItemComboBox->addItem(tr("修改当前项"));
    currItemComboBox->addItem(tr("1"));
    currItemComboBox->addItem(tr("2"));
    currItemComboBox->setCurrentIndex(0);
    //currItemComboBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    //okBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    //cancelBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    bottomLayout=new QHBoxLayout;
    //bottomLayout->addWidget(currItemComboBox);
    bottomLayout->addWidget(okBtn);
    bottomLayout->addWidget(cancelBtn);
    currItemComboBox->hide();
}

void LineWeldDlg::createPointPosBox()
{
    int macType=4;
    if((g_MacType==3 || g_MacType==1 || g_MacType==4))
        macType=6;
    else
        macType=4;
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setItalic(false);
    if(macType==6)
        ft.setPointSize(DLG_LABEL_BUTTON_FONT+2);
    else
        ft.setPointSize(DLG_LABEL_BUTTON_FONT+3);
    QLabel *posLabel1[6]={new QLabel(tr("X")),new QLabel(tr("Y")),new QLabel(tr("Z")),new QLabel(tr("R")),new QLabel(tr("R1")),new QLabel(tr("R2"))};
    QLabel *posLabel2[6]={new QLabel(tr("X")),new QLabel(tr("Y")),new QLabel(tr("Z")),new QLabel(tr("R")),new QLabel(tr("R1")),new QLabel(tr("R2"))};
    for(int i=0;i<6;i++)
    {
        posLabel1[i]->setFont(ft);
        posLabel2[i]->setFont(ft);
    }
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    for(int i=0;i<6;i++)
    {
        startPosEdit.append(new MyLineEdit);
        endPosEdit.append(new MyLineEdit);
    }
    for(int i=0;i<6;i++)
    {
        startPosEdit[i]->setFont(ft);
        startPosEdit[i]->setMaximumWidth(100);
        endPosEdit[i]->setFont(ft);
        endPosEdit[i]->setMaximumWidth(100);
    }
    if(macType==6)
        ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    else
        ft.setPointSize(DLG_LABEL_BUTTON_FONT+3);
    QPushButton * updatePosBtn1=new QPushButton(tr("更新"));
    updatePosBtn1->setFont(ft);
    //updatePosBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(updatePosBtn1, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtn1Clicked()));
    QPushButton * updatePosBtn2=new QPushButton(tr("更新"));
    updatePosBtn2->setFont(ft);
    connect(updatePosBtn2, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtn2Clicked()));

    QLabel *sLabel=new QLabel(tr("起点坐标"));
    sLabel->setFont(ft);
    QLabel *eLabel=new QLabel(tr("终点坐标"));
    eLabel->setFont(ft);

    posLayout1=new QHBoxLayout;
    posLayout1->setMargin(0);
    for(int i=0;i<macType;i++)
    {
        posLayout1->addWidget(posLabel1[i]);
        posLayout1->addWidget(startPosEdit[i]);
    }
    posLayout1->addWidget(updatePosBtn1);
    posLayout2=new QHBoxLayout;
    posLayout2->setMargin(0);
    for(int i=0;i<macType;i++)
    {
        posLayout2->addWidget(posLabel2[i]);
        posLayout2->addWidget(endPosEdit[i]);
    }
    posLayout2->addWidget(updatePosBtn2);

    QVBoxLayout *boxLayout=new QVBoxLayout;
    boxLayout->addWidget(sLabel);
    boxLayout->addLayout(posLayout1);
    boxLayout->addWidget(eLabel);
    boxLayout->addLayout(posLayout2);

    pointPosBox=new QGroupBox;
    pointPosBox->setLayout(boxLayout);
}

void LineWeldDlg::selectLineWeldPara(const QString &lineWeldTechName)
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
    db.ReadLineWeldFromDb(lineWeldTechName,lineWeldPara);
    db.closeDB();

    runSpeedEdit->setText(QString("%1").arg(lineWeldPara.atspeed,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(lineWeldPara.Dlt_X,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(lineWeldPara.Dlt_Y,0,'f',1));
    technicsParaEdit2[0]->setText(QString("%1").arg(lineWeldPara.Dlt_Z,0,'f',1));
    technicsParaEdit2[3]->setText(QString("%1").arg(lineWeldPara.Dlt_speed,0,'f',1));
    technicsParaEdit1[2]->setText(QString("%1").arg(lineWeldPara.FT_Pre_Length,0,'f',1));
    technicsParaEdit1[3]->setText(QString("%1").arg(lineWeldPara.FT_Pre_Speed,0,'f',1));
    technicsParaEdit2[2]->setText(QString("%1").arg(lineWeldPara.TH_speed,0,'f',1));
    technicsParaEdit1[5]->setText(QString("%1").arg(lineWeldPara.S1_Length,0,'f',1));
    technicsParaEdit1[6]->setText(QString("%1").arg(lineWeldPara.S1_T_Length,0,'f',1));
    technicsParaEdit1[7]->setText(QString("%1").arg(lineWeldPara.S1_speed,0,'f',1));
    technicsParaEdit2[5]->setText(QString("%1").arg(lineWeldPara.S2_Length,0,'f',1));
    technicsParaEdit2[6]->setText(QString("%1").arg(lineWeldPara.S2_T_Length,0,'f',1));
    technicsParaEdit2[7]->setText(QString("%1").arg(lineWeldPara.S2_speed,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TX,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TY,0,'f',1));
    technicsParaEdit2[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TZ,0,'f',1));
    technicsParaEdit1[4]->setText(QString("%1").arg(lineWeldPara.T1_Delay_time));
    technicsParaEdit2[4]->setText(QString("%1").arg(lineWeldPara.T2_Delay_time));
}

void LineWeldDlg::OnLoadDefaultBtnClicked()
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
    db.ReadLineWeldFromDb(QString("default"),lineWeldPara);//


    Z_hEdit->setText(QString("%1").arg(lineWeldPara.ZH,0,'f',1));
    runSpeedEdit->setText(QString("%1").arg(lineWeldPara.atspeed,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(lineWeldPara.Dlt_X,0,'f',1));
    technicsParaEdit1[0]->setText(QString("%1").arg(lineWeldPara.Dlt_Y,0,'f',1));
    technicsParaEdit2[0]->setText(QString("%1").arg(lineWeldPara.Dlt_Z,0,'f',1));
    technicsParaEdit2[3]->setText(QString("%1").arg(lineWeldPara.Dlt_speed,0,'f',1));
    technicsParaEdit1[2]->setText(QString("%1").arg(lineWeldPara.FT_Pre_Length,0,'f',1));
    technicsParaEdit1[3]->setText(QString("%1").arg(lineWeldPara.FT_Pre_Speed,0,'f',1));
    technicsParaEdit2[2]->setText(QString("%1").arg(lineWeldPara.TH_speed,0,'f',1));
    technicsParaEdit1[5]->setText(QString("%1").arg(lineWeldPara.S1_Length,0,'f',1));
    technicsParaEdit1[6]->setText(QString("%1").arg(lineWeldPara.S1_T_Length,0,'f',1));
    technicsParaEdit1[7]->setText(QString("%1").arg(lineWeldPara.S1_speed,0,'f',1));
    technicsParaEdit2[5]->setText(QString("%1").arg(lineWeldPara.S2_Length,0,'f',1));
    technicsParaEdit2[6]->setText(QString("%1").arg(lineWeldPara.S2_T_Length,0,'f',1));
    technicsParaEdit2[7]->setText(QString("%1").arg(lineWeldPara.S2_speed,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TX,0,'f',1));
    technicsParaEdit1[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TY,0,'f',1));
    technicsParaEdit2[1]->setText(QString("%1").arg(lineWeldPara.Dlt_TZ,0,'f',1));
    technicsParaEdit1[4]->setText(QString("%1").arg(lineWeldPara.T1_Delay_time));
    technicsParaEdit2[4]->setText(QString("%1").arg(lineWeldPara.T2_Delay_time));

}
/*
void LineWeldDlg::OnCopyParaBtnClicked()
{

}
*/
void LineWeldDlg::OnSetDefaultBtnClicked()
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

    lineWeldPara.ZH=Z_hEdit->text().trimmed().toDouble();
    lineWeldPara.atspeed=runSpeedEdit->text().trimmed().toDouble();
    lineWeldPara.Dlt_X=technicsParaEdit1[0]->text().trimmed().toDouble();
    lineWeldPara.Dlt_Y=technicsParaEdit1[0]->text().trimmed().toDouble();
    lineWeldPara.Dlt_Z=technicsParaEdit2[0]->text().trimmed().toDouble();
    lineWeldPara.Dlt_speed=technicsParaEdit2[3]->text().trimmed().toDouble();
    lineWeldPara.FT_Pre_Length=technicsParaEdit1[2]->text().trimmed().toDouble();
    lineWeldPara.FT_Pre_Speed=technicsParaEdit1[3]->text().trimmed().toDouble();
    lineWeldPara.TH_speed=technicsParaEdit2[2]->text().trimmed().toDouble();
    lineWeldPara.S1_Length=technicsParaEdit1[5]->text().trimmed().toDouble();
    lineWeldPara.S1_T_Length=technicsParaEdit1[6]->text().trimmed().toDouble();
    lineWeldPara.S1_speed=technicsParaEdit1[7]->text().trimmed().toDouble();
    lineWeldPara.S2_Length=technicsParaEdit2[5]->text().trimmed().toDouble();
    lineWeldPara.S2_T_Length=technicsParaEdit2[6]->text().trimmed().toDouble();
    lineWeldPara.S2_speed=technicsParaEdit2[7]->text().trimmed().toDouble();
    lineWeldPara.Dlt_TX=technicsParaEdit1[1]->text().trimmed().toDouble();
    lineWeldPara.Dlt_TY=technicsParaEdit1[1]->text().trimmed().toDouble();
    lineWeldPara.Dlt_TZ=technicsParaEdit2[1]->text().trimmed().toDouble();
    lineWeldPara.T1_Delay_time=technicsParaEdit1[4]->text().trimmed().toDouble();
    lineWeldPara.T2_Delay_time=technicsParaEdit2[4]->text().trimmed().toDouble();
    lineWeldPara.v_Over_Time_count=100000;

    db.SaveLineWeldToDb(QString("default"),lineWeldPara);
}
void LineWeldDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
}

void LineWeldDlg::OnEnvParaBtnClicked()
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
    dlg->technicsType=1;    //拖焊
    dlg->paraName=paraName;
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
    delete dlg;
}

void LineWeldDlg::OnOkBtnClicked()
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
    envPara.LaoTie_ZL=tr("20R");//种类

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

    if(type==2) //
    {
        if(weldTechnicsName.left(2)==QString("__"))
        {
            msgBox.setText(tr("用户自定义工艺参数名称不能以两个下划线开头."));
            msgBox.exec();
            return;
        }
    }

    Z_h=Z_hEdit->text().trimmed().toDouble();
    //runSpeed=runSpeedEdit->text().trimmed().toDouble();lineWeldType
    if(lineWeldRdoBtn->isChecked())
        lineWeldType=0;
    else if(manyLineWeldRdoBtn1->isChecked())
        lineWeldType=1;
    else if(manyLineWeldRdoBtn2->isChecked())
        lineWeldType=2;
    else if(manyLineWeldRdoBtn3->isChecked())
        lineWeldType=3;
    for(int i=0;i<4;i++)
    {
        pointPos[i]=startPosEdit[i]->text().trimmed().toDouble();
        pointPos[i+4]=endPosEdit[i]->text().trimmed().toDouble();
    }
    ironPos[0]=startPosEdit[4]->text().trimmed().toDouble();
    ironPos[1]=startPosEdit[5]->text().trimmed().toDouble();

    lineWeldPara.x0=startPosEdit[0]->text().trimmed().toDouble();
    lineWeldPara.y0=startPosEdit[1]->text().trimmed().toDouble();
    lineWeldPara.z0=startPosEdit[2]->text().trimmed().toDouble();
    lineWeldPara.r0=startPosEdit[3]->text().trimmed().toDouble();
    lineWeldPara.x1=endPosEdit[0]->text().trimmed().toDouble();
    lineWeldPara.y1=endPosEdit[1]->text().trimmed().toDouble();
    lineWeldPara.z1=endPosEdit[2]->text().trimmed().toDouble();
    lineWeldPara.r1=endPosEdit[3]->text().trimmed().toDouble();

    lineWeldPara.ZH=Z_hEdit->text().trimmed().toDouble();
    lineWeldPara.atspeed=runSpeedEdit->text().trimmed().toDouble();
    lineWeldPara.Dlt_X=technicsParaEdit1[0]->text().trimmed().toDouble();
    lineWeldPara.Dlt_Y=technicsParaEdit1[0]->text().trimmed().toDouble();
    lineWeldPara.Dlt_Z=technicsParaEdit2[0]->text().trimmed().toDouble();
    lineWeldPara.Dlt_speed=technicsParaEdit2[3]->text().trimmed().toDouble();
    lineWeldPara.FT_Pre_Length=technicsParaEdit1[2]->text().trimmed().toDouble();
    lineWeldPara.FT_Pre_Speed=technicsParaEdit1[3]->text().trimmed().toDouble();
    lineWeldPara.TH_speed=technicsParaEdit2[2]->text().trimmed().toDouble();
    lineWeldPara.S1_Length=technicsParaEdit1[5]->text().trimmed().toDouble();
    lineWeldPara.S1_T_Length=technicsParaEdit1[6]->text().trimmed().toDouble();
    lineWeldPara.S1_speed=technicsParaEdit1[7]->text().trimmed().toDouble();
    lineWeldPara.S2_Length=100.0-lineWeldPara.S1_Length;//technicsParaEdit2[5]->text().trimmed().toDouble();
    lineWeldPara.S2_T_Length=technicsParaEdit2[6]->text().trimmed().toDouble();
    lineWeldPara.S2_speed=technicsParaEdit2[7]->text().trimmed().toDouble();
    lineWeldPara.Dlt_TX=technicsParaEdit1[1]->text().trimmed().toDouble();
    lineWeldPara.Dlt_TY=technicsParaEdit1[1]->text().trimmed().toDouble();
    lineWeldPara.Dlt_TZ=technicsParaEdit2[1]->text().trimmed().toDouble();
    lineWeldPara.T1_Delay_time=technicsParaEdit1[4]->text().trimmed().toDouble();
    lineWeldPara.T2_Delay_time=technicsParaEdit2[4]->text().trimmed().toDouble();
    lineWeldPara.v_Over_Time_count=100000;
    appToSameTechnicsChecked=appToSameTechnicsCheckBox->isChecked();

    MyRWxml xml;
    SYSTEM_PARAMS systemPara;
    xml.readSysPara("robot.xml",systemPara);
    double xx0=lineWeldPara.x0+lineWeldPara.Dlt_X*cos(PI*lineWeldPara.r0/180);
    double yy0=lineWeldPara.y0+lineWeldPara.Dlt_Y*sin(PI*lineWeldPara.r0/180);
    double zz0=lineWeldPara.z0-lineWeldPara.Dlt_Z;
    double zz_ZH0=lineWeldPara.z0-Z_h;
    double xx1=lineWeldPara.x1+lineWeldPara.Dlt_X*cos(PI*lineWeldPara.r1/180);
    double yy1=lineWeldPara.y1+lineWeldPara.Dlt_Y*sin(PI*lineWeldPara.r1/180);
    double zz1=lineWeldPara.z1-lineWeldPara.Dlt_Z;
    double zz_ZH1=lineWeldPara.z1-Z_h;

    QString strErr,strErrX,strErrY,strErrZ,strErrZh,strErrR,strErrSpeed;
    if(xx0<0 || xx0>systemPara.limitPosLen[0] || xx1<0 || xx1>systemPara.limitPosLen[0])
    {
        strErrX=QString(tr(" X坐标补偿错误 "));
        strErr+=strErrX;
    }
    if(yy0<0 || yy0>systemPara.limitPosLen[1] || yy1<0 || yy1>systemPara.limitPosLen[1])
    {
        strErrY=QString(tr(" Y坐标补偿错误 "));
        strErr+=strErrY;
    }
    if(zz0<0 || zz1<0 )
    {
        strErrZ=QString(tr(" Z坐标补偿错误 "));
        strErr+=strErrZ;
    }
    else if(zz_ZH0<0 || zz_ZH0>systemPara.limitPosLen[2] || zz_ZH1<0 || zz_ZH1>systemPara.limitPosLen[2] || Z_h<5.0)
    {
        strErrZh=QString(tr(" Z坐标提升值错误 "));
        strErr+=strErrZh;
    }
    if(lineWeldPara.r0<systemPara.limitPosLen[4] || lineWeldPara.r0>systemPara.limitPosLen[3] || lineWeldPara.r1<systemPara.limitPosLen[4] || lineWeldPara.r1>systemPara.limitPosLen[3])
    {
        strErrR=QString(tr(" R坐标值错误 "));
        strErr+=strErrR;
    }
    if(lineWeldPara.atspeed<0 || lineWeldPara.atspeed>ROBOT_MAX_ATSPEED)
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

void LineWeldDlg::OnCancelBtnClicked()
{
    isOK=false;
    if(type<2)
        startOut();
    else
        slowlyInOut(1);
}

void LineWeldDlg::OnUpdatePosBtn1Clicked()
{
    for(int i=0;i<4;i++)
        startPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        startPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
    ironPos[0]=currPointPos[5];
    ironPos[1]=currPointPos[6];
    if((g_MacType==3 || g_MacType==1 || g_MacType==4)) //6轴
    {
        startPosEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
        startPosEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',2));
        endPosEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
        endPosEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',2));
    }
}

void LineWeldDlg::OnUpdatePosBtn2Clicked()
{
    for(int i=0;i<4;i++)
        endPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        endPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
}

void LineWeldDlg::loadMyLineEdit()      //加载自定义文本框
{
    Z_hEdit->context=context;
    QRect rect=Z_hEdit->geometry();
    Z_hEdit->myPos=QPoint(rect.left(),rect.top());
    Z_hEdit->parentHeight=geometry().height();
    runSpeedEdit->context=context;
    rect=runSpeedEdit->geometry();
    runSpeedEdit->myPos=QPoint(rect.left(),rect.top());
    runSpeedEdit->parentHeight=geometry().height();

    int macType=4;
    if((g_MacType==3 || g_MacType==1 || g_MacType==4))
        macType=6;
    else
        macType=4;
    for(int i=0;i<macType;i++)
    {
        startPosEdit[i]->context=context;
        QRect rect=startPosEdit[i]->geometry();
        startPosEdit[i]->myPos=startPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        startPosEdit[i]->parentHeight=geometry().height();

        endPosEdit[i]->context=context;
        rect=endPosEdit[i]->geometry();
        endPosEdit[i]->myPos=endPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        endPosEdit[i]->parentHeight=geometry().height();
    }
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
/*
void LineWeldDlg::onClickedTechnicsEdit()   //点击工艺名称对话框
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
*/
