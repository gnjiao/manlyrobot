#include "movetoolpage.h"
#include "myrwxml.h"

MoveToolPage::MoveToolPage(QWidget *parent) :
    QWidget(parent)
{
    robot=NULL;
    speedValue=0;
    pointIndex=0;
    lockPosFlag=0;
    //stopOverFlag=true;
    for(int i=0;i<8;i++)
    {
        movetoolMatrixPos[0][i]=0;
        movetoolMatrixPos[1][i]=0;
    }
}

void MoveToolPage::initWidget()
{
    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    upValue[a_x]=systemPara.limitPosLen[0]-10 ; //290
    upValue[a_y]=systemPara.limitPosLen[1]-10; //290;
    upValue[a_z]=systemPara.limitPosLen[2]-10; //80;
    upValue[a_r]=systemPara.limitPosLen[3]-10; //170;
    upValue[a_s]=290;
    upValue[a_r1]=systemPara.limitPosLen[5]-5; //55
    upValue[a_r2]=systemPara.limitPosLen[7]-5; //55;
    upValue[a_y2]=290;
    downValue[a_x]=10;
    downValue[a_y]=10;
    downValue[a_z]=10;
    downValue[a_r]=systemPara.limitPosLen[4]+10; //-170;
    downValue[a_s]=0;
    downValue[a_r1]=systemPara.limitPosLen[6]+5; //-55;
    downValue[a_r2]=systemPara.limitPosLen[8]+5; //-55;
    downValue[a_y2]=systemPara.limitPosLen[1]-10;

    for(int i=0;i<8;i++)
        flagLimit[i]=false;
    for(int i=0;i<8;i++)
        flagLimit2[i]=false;
    speedValue=2;
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MID_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    curPointAxisPos=QVector<double> (8);
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=0;
    pointsAxisPos.append(curPointAxisPos);
    pointsAxisPos.append(curPointAxisPos);

    mtracksWidget=new TracksWidget;
    mtracksWidget->type=3;
    mtracksWidget->initWidget();
    mtracksWidget->setMinimumWidth(screenRect.width()*8/12);
    createLeftButtonBox();
    createRightButtonBox();
    createTwoPointPosBox();
    //buttonBox->setMaximumWidth(screenRect.width()/3);
/*
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addWidget(buttonBox);
    rightLayout->addWidget(coordListView);
*/

    lineMoveRdoBtn=new QRadioButton(tr("连续移动"));
    lineMoveRdoBtn->setFont(ft);
    pointMoveRdoBtn=new QRadioButton(tr("点动"));
    pointMoveRdoBtn->setFont(ft);
    if(g_moveMod==POINT_MOVE)   //如果是点动
    {
        pointMoveRdoBtn->setChecked(true);
        lineMoveRdoBtn->setChecked(false);
    }
    else
    {
        pointMoveRdoBtn->setChecked(false);
        lineMoveRdoBtn->setChecked(true);
    }
    QLabel *setpLabel=new QLabel(tr("步长(mm)"));
    setpLabel->setFont(ft);
    stepLengthComBox=new QComboBox;
    stepLengthComBox->setFont(ft);
    stepLengthComBox->addItem("0.01");
    stepLengthComBox->addItem("0.1");
    stepLengthComBox->addItem("1");
    stepLengthComBox->addItem("10");
    stepLengthComBox->setCurrentIndex(g_setpLen);
    lockButton=new QPushButton(tr("姿态锁定"));
    lockButton->setFont(ft);
    connect(lockButton, SIGNAL(clicked()), this, SLOT(OnLockBtnClicked()));
    lsButton=new QPushButton(tr("拖焊起点"));
    lsButton->setFont(ft);
    connect(lsButton, SIGNAL(clicked()), this, SLOT(OnLsBtnClicked()));
    leButton=new QPushButton(tr("拖焊终点"));
    leButton->setFont(ft);
    connect(leButton, SIGNAL(clicked()), this, SLOT(OnLeBtnClicked()));
    QPushButton *oneButton=new QPushButton(tr("阵列第一点"));
    oneButton->setFont(ft);
    connect(oneButton, SIGNAL(clicked()), this, SLOT(OnOneBtnClicked()));
    QPushButton *twoButton=new QPushButton(tr("阵列第二点"));
    twoButton->setFont(ft);
    connect(twoButton, SIGNAL(clicked()), this, SLOT(OnTwoBtnClicked()));
    r1CoordEdit=new QLineEdit;
    r1CoordEdit->setFont(ft);
    r1CoordEdit->setAlignment(Qt::AlignCenter);
    r2CoordEdit=new QLineEdit;
    r2CoordEdit->setFont(ft);
    r2CoordEdit->setAlignment(Qt::AlignCenter);
    QLabel *r1Label=new QLabel(tr("R1坐标"));
    r1Label->setFont(ft);
    QLabel *r2Label=new QLabel(tr("R2坐标"));
    r2Label->setFont(ft);

    QHBoxLayout *r1Layout = new QHBoxLayout;
    r1Layout->addWidget(r1Label);
    r1Layout->addWidget(r1CoordEdit);
    QHBoxLayout *r2Layout = new QHBoxLayout;
    r2Layout->addWidget(r2Label);
    r2Layout->addWidget(r2CoordEdit);

    QGroupBox *radioBox=new QGroupBox;
    QVBoxLayout *radioLayout = new QVBoxLayout;
    radioLayout->addWidget(lineMoveRdoBtn);
    radioLayout->addWidget(pointMoveRdoBtn);
    radioBox->setLayout(radioLayout);
    QHBoxLayout *stepLayout = new QHBoxLayout;
    stepLayout->addWidget(setpLabel);
    stepLayout->addWidget(stepLengthComBox);
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->addStretch();
    rightLayout->addWidget(twoPointPosBox);
    rightLayout->addLayout(r1Layout);
    rightLayout->addLayout(r2Layout);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(radioBox);
    rightLayout->addSpacing(5);
    rightLayout->addLayout(stepLayout);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(lockButton);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(lsButton);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(leButton);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(oneButton);
    rightLayout->addSpacing(5);
    rightLayout->addWidget(twoButton);
    rightLayout->addSpacing(5);

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addStretch();
    midLayout->addWidget(mtracksWidget);
    midLayout->addSpacing(20);
    midLayout->addLayout(rightLayout);
    midLayout->addStretch();
    midLayout->addWidget(leftBtnBox);
    midLayout->addWidget(rightBtnBox);
    midLayout->addStretch();
/*
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addLayout(midLayout);
    layout->addStretch();
    layout->addLayout(rightLayout);
    layout->addStretch();
*/
    setLayout(midLayout);

    connect(lineMoveRdoBtn, SIGNAL(clicked()),this, SLOT(OnLineMoveRdoBtnClicked()));
    connect(pointMoveRdoBtn, SIGNAL(clicked()),this, SLOT(OnPointMoveRdoBtnClicked()));
    connect(stepLengthComBox, SIGNAL(currentIndexChanged(int)),this, SLOT(setCurrStepLen(int)));
}

void MoveToolPage::createLeftButtonBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_BUTTONS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    QPushButton *nXButton=new QPushButton(tr("-X"));
    nXButton->setFont(ft);
    connect(nXButton, SIGNAL(pressed()), this, SLOT(OnNXBtnPressed()));
    connect(nXButton, SIGNAL(released()), this, SLOT(OnNXBtnReleased()));
    QPushButton *nYButton=new QPushButton(tr("-Y"));
    nYButton->setFont(ft);
    connect(nYButton, SIGNAL(pressed()), this, SLOT(OnNYBtnPressed()));
    connect(nYButton, SIGNAL(released()), this, SLOT(OnNYBtnReleased()));
    QPushButton *nZButton=new QPushButton(tr("-Z"));
    nZButton->setFont(ft);
    connect(nZButton, SIGNAL(pressed()), this, SLOT(OnNZBtnPressed()));
    connect(nZButton, SIGNAL(released()), this, SLOT(OnNZBtnReleased()));
    QPushButton *nRButton=new QPushButton(tr("-R"));
    nRButton->setFont(ft);
    connect(nRButton, SIGNAL(pressed()), this, SLOT(OnNRBtnPressed()));
    connect(nRButton, SIGNAL(released()), this, SLOT(OnNRBtnReleased()));

    nR1Button=new QPushButton(tr("-R1"));
    nR1Button->setFont(ft);
    connect(nR1Button, SIGNAL(pressed()), this, SLOT(OnNR1BtnPressed()));
    connect(nR1Button, SIGNAL(released()), this, SLOT(OnNR1BtnReleased()));
    nR2Button=new QPushButton(tr("-R2"));
    nR2Button->setFont(ft);
    connect(nR2Button, SIGNAL(pressed()), this, SLOT(OnNR2BtnPressed()));
    connect(nR2Button, SIGNAL(released()), this, SLOT(OnNR2BtnReleased()));

    ft.setPointSize(12);
    QPushButton *nSButton=new QPushButton(tr("减速"));
    nSButton->setFont(ft);
    connect(nSButton, SIGNAL(pressed()), this, SLOT(OnNSBtnPressed()));
    connect(nSButton, SIGNAL(released()), this, SLOT(OnNSBtnReleased()));

    speedLeftEdit = new QLineEdit;
    speedLeftEdit->setFont(ft);
    speedLeftEdit->setReadOnly(true);
    speedLeftEdit->setText(QString("%1%").arg(speedValue*10));
    speedLeftEdit->setAlignment(Qt::AlignCenter);

    leftBtnBox=new QGroupBox;
    leftBtnBox->setMaximumWidth(screenRect.width()/10);
    //buttonBox->setMinimumHeight(screenRect.height()/2);

    QVBoxLayout *speedLayout = new QVBoxLayout;
    speedLayout->addWidget(speedLeftEdit);
    speedLayout->addWidget(nSButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(nXButton);
    layout->addWidget(nYButton);
    layout->addWidget(nZButton);
    layout->addWidget(nRButton);
    layout->addWidget(nR1Button);
    layout->addWidget(nR2Button);
    layout->addLayout(speedLayout);
    leftBtnBox->setLayout(layout);
    leftBtnBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(125,45,200, 255)); border-radius:10px; border:none}");
}

void MoveToolPage::createRightButtonBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_BUTTONS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();


    QPushButton *pXButton=new QPushButton(tr("+X"));
    pXButton->setFont(ft);
    connect(pXButton, SIGNAL(pressed()), this, SLOT(OnPXBtnPressed()));
    connect(pXButton, SIGNAL(released()), this, SLOT(OnPXBtnReleased()));
    QPushButton *pYButton=new QPushButton(tr("+Y"));
    pYButton->setFont(ft);
    connect(pYButton, SIGNAL(pressed()), this, SLOT(OnPYBtnPressed()));
    connect(pYButton, SIGNAL(released()), this, SLOT(OnPYBtnReleased()));
    QPushButton *pZButton=new QPushButton(tr("+Z"));
    pZButton->setFont(ft);
    connect(pZButton, SIGNAL(pressed()), this, SLOT(OnPZBtnPressed()));
    connect(pZButton, SIGNAL(released()), this, SLOT(OnPZBtnReleased()));
    QPushButton *pRButton=new QPushButton(tr("+R"));
    pRButton->setFont(ft);
    connect(pRButton, SIGNAL(pressed()), this, SLOT(OnPRBtnPressed()));
    connect(pRButton, SIGNAL(released()), this, SLOT(OnPRBtnReleased()));

    pR1Button=new QPushButton(tr("+R1"));
    pR1Button->setFont(ft);
    connect(pR1Button, SIGNAL(pressed()), this, SLOT(OnPR1BtnPressed()));
    connect(pR1Button, SIGNAL(released()), this, SLOT(OnPR1BtnReleased()));
    pR2Button=new QPushButton(tr("+R2"));
    pR2Button->setFont(ft);
    connect(pR2Button, SIGNAL(pressed()), this, SLOT(OnPR2BtnPressed()));
    connect(pR2Button, SIGNAL(released()), this, SLOT(OnPR2BtnReleased()));
    ft.setPointSize(12);
    QPushButton *pSButton=new QPushButton(tr("加速"));
    pSButton->setFont(ft);
    connect(pSButton, SIGNAL(pressed()), this, SLOT(OnPSBtnPressed()));
    connect(pSButton, SIGNAL(released()), this, SLOT(OnPSBtnReleased()));

    speedRightEdit = new QLineEdit;
    speedRightEdit->setFont(ft);
    speedRightEdit->setReadOnly(true);
    speedRightEdit->setText(QString("%1%").arg(speedValue*10));
    speedRightEdit->setAlignment(Qt::AlignCenter);

    rightBtnBox=new QGroupBox;
    rightBtnBox->setMaximumWidth(screenRect.width()/10);
    //buttonBox->setMinimumHeight(screenRect.height()/2);

    QVBoxLayout *speedLayout = new QVBoxLayout;
    speedLayout->addWidget(speedRightEdit);
    speedLayout->addWidget(pSButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(pXButton);
    layout->addWidget(pYButton);
    layout->addWidget(pZButton);
    layout->addWidget(pRButton);
    layout->addWidget(pR1Button);
    layout->addWidget(pR2Button);
    layout->addLayout(speedLayout);
    rightBtnBox->setLayout(layout);
    rightBtnBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(125,45,200, 255)); border-radius:10px; border:none}");
}

void MoveToolPage::createTwoPointPosBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_BUTTONS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *oneLabel=new QLabel(tr("起点坐标"));
    oneLabel->setFont(ft);
    QLabel *xLabel=new QLabel(tr("X:"));
    xLabel->setFont(ft);
    QLabel *yLabel=new QLabel(tr("Y:"));
    yLabel->setFont(ft);
    QLabel *zLabel=new QLabel(tr("Z:"));
    zLabel->setFont(ft);
    QLabel *rLabel=new QLabel(tr("R:"));
    rLabel->setFont(ft);
    xEdit=new QLineEdit;
    xEdit->setFont(ft);
    yEdit=new QLineEdit;
    yEdit->setFont(ft);
    zEdit=new QLineEdit;
    zEdit->setFont(ft);
    rEdit=new QLineEdit;
    rEdit->setFont(ft);

    QLabel *twoLabel=new QLabel(tr("终点坐标"));
    twoLabel->setFont(ft);
    QLabel *xLabel2=new QLabel(tr("X:"));
    xLabel2->setFont(ft);
    QLabel *yLabel2=new QLabel(tr("Y:"));
    yLabel2->setFont(ft);
    QLabel *zLabel2=new QLabel(tr("Z:"));
    zLabel2->setFont(ft);
    QLabel *rLabel2=new QLabel(tr("R:"));
    rLabel2->setFont(ft);
    xEdit2=new QLineEdit;
    xEdit2->setFont(ft);
    yEdit2=new QLineEdit;
    yEdit2->setFont(ft);
    zEdit2=new QLineEdit;
    zEdit2->setFont(ft);
    rEdit2=new QLineEdit;
    rEdit2->setFont(ft);

    QHBoxLayout *coordLayout1 = new QHBoxLayout;
    coordLayout1->addWidget(xLabel);
    coordLayout1->addWidget(xEdit);
    coordLayout1->addWidget(yLabel);
    coordLayout1->addWidget(yEdit);
    QHBoxLayout *coordLayout2 = new QHBoxLayout;
    coordLayout2->addWidget(zLabel);
    coordLayout2->addWidget(zEdit);
    coordLayout2->addWidget(rLabel);
    coordLayout2->addWidget(rEdit);
    QHBoxLayout *coordLayout3 = new QHBoxLayout;
    coordLayout3->addWidget(xLabel2);
    coordLayout3->addWidget(xEdit2);
    coordLayout3->addWidget(yLabel2);
    coordLayout3->addWidget(yEdit2);
    QHBoxLayout *coordLayout4 = new QHBoxLayout;
    coordLayout4->addWidget(zLabel2);
    coordLayout4->addWidget(zEdit2);
    coordLayout4->addWidget(rLabel2);
    coordLayout4->addWidget(rEdit2);

    QVBoxLayout *coordLayout = new QVBoxLayout;
    coordLayout->addWidget(oneLabel);
    coordLayout->addLayout(coordLayout1);
    coordLayout->addLayout(coordLayout2);
    coordLayout->addWidget(twoLabel);
    coordLayout->addLayout(coordLayout3);
    coordLayout->addLayout(coordLayout4);

    twoPointPosBox=new QGroupBox;
    twoPointPosBox->setLayout(coordLayout);
}

/*
void MoveToolPage::createCoordinate()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_BUTTONS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    coordListView=new QListView;
    coordListView->setMaximumWidth(screenRect.width()/3);
}
*/
void MoveToolPage::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        curPointAxisPos[i]=coordinate[i];
        if(!flagLimit[i])
        {
            if(curPointAxisPos[i]<downValue[i])
            {
                robot->ML_Stop(i, 1);
                robot->ML_SetJogMode(POINT_MOVE);
                robot->ML_SetJogMove ( i, NEGATIVE, DISABLE );
                flagLimit[i]=true;
            }
        }
        if(!flagLimit2[i])
        {
            if(curPointAxisPos[i]>upValue[i])
            {
                robot->ML_Stop(i, 1);
                robot->ML_SetJogMode(POINT_MOVE);
                robot->ML_SetJogMove ( i, POSITIVE, DISABLE );
                flagLimit2[i]=true;
            }
        }
    }
    this->mtracksWidget->xEdit->setText(QString("%1").arg(coordinate[0],0,'f',2));
    this->mtracksWidget->yEdit->setText(QString("%1").arg(coordinate[1],0,'f',2));
    this->mtracksWidget->zEdit->setText(QString("%1").arg(coordinate[2],0,'f',2));
    this->mtracksWidget->rEdit->setText(QString("%1").arg(coordinate[3],0,'f',2));
    r1CoordEdit->setText(QString("%1").arg(coordinate[5],0,'f',2));
    r2CoordEdit->setText(QString("%1").arg(coordinate[6],0,'f',2));
    if(g_MacType==2)    //双平台
    {
        mtracksWidget->y2Edit->setText(QString("%1").arg(coordinate[7],0,'f',2));
    }

    mtracksWidget->pointColor=mtracksWidget->color[2];
    mtracksWidget->left_Z_Slider->setValue(curPointAxisPos[2]);
    mtracksWidget->down_R_Slider->setValue(curPointAxisPos[3]);
    mtracksWidget->up_R1_Slider->setValue(curPointAxisPos[5]);
    mtracksWidget->right_R2_Slider->setValue(curPointAxisPos[6]);
    mtracksWidget->point.setX(curPointAxisPos[0]);
    if(curPointAxisPos[0]>systemPara.limitPosLen[0]/2)    //Y2
        mtracksWidget->point.setY(curPointAxisPos[7]);
    else
        mtracksWidget->point.setY(curPointAxisPos[1]);
    mtracksWidget->updateTracks();

    lockButton->setDisabled(false);
}

void MoveToolPage::OnNXBtnPressed() //-X按钮按下
{
    emit readCoordinate();  //开始读坐标
    int st=0;
    if(curPointAxisPos[a_x]<downValue[a_x])
    {
        emit overLimit(a_x,0);
        return;
    }

    flagLimit[a_x]=false;

    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_N_Move(a_x,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_x,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_N_Move(a_x,POINT_MOVE);
    }
}

void MoveToolPage::OnNYBtnPressed() //-Y按钮按下
{
    //emit stopCoordinate();  //停止读坐标
    emit readCoordinate();  //开始读坐标
    int st=0;
    if(curPointAxisPos[a_y]<downValue[a_y])
    {
        emit overLimit(a_y,0);
        return;
    }
    flagLimit[a_y]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        //mySleep(100);
        emit signal_Axis_N_Move(a_y,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_y,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_N_Move(a_y,POINT_MOVE);
    }
}

void MoveToolPage::OnNZBtnPressed() //-Z按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_z]<downValue[a_z])
    {
        emit overLimit(a_z,0);
        return;
    }
    flagLimit[a_z]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_N_Move(a_z,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_z,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_N_Move(a_z,POINT_MOVE);
    }
}

void MoveToolPage::OnNRBtnPressed() //-R按钮按下
{
    emit readCoordinate();  //开始读坐标
    int st=0;
    if(curPointAxisPos[a_r]<downValue[a_r])
    {
        emit overLimit(a_r,0);
        return;
    }
    flagLimit[a_r]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_N_Move(a_r,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_r,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_N_Move(a_r,POINT_MOVE);
    }
}

void MoveToolPage::OnNR1BtnPressed() //-R1按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_r1]<downValue[a_r1])
    {
        emit overLimit(a_r1,0);
        return;
    }
    flagLimit[a_r1]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_N_Move(a_r1,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_r1,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_N_Move(a_r1,POINT_MOVE);
    }
}

void MoveToolPage::OnNR2BtnPressed() //-R2按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_r2]<downValue[a_r2])
    {
        emit overLimit(a_r2,0);
        return;
    }
    flagLimit[a_r2]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_N_Move(a_r2,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_r2,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_N_Move(a_r2,POINT_MOVE);
    }
}

void MoveToolPage::OnNSBtnPressed() //-S按钮按下
{
}

void MoveToolPage::OnNXBtnReleased() //-X按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_x,1);  //立即停止轴移动
        //emit stop_Axis_Move(a_x,1);
        //stopOverFlag=true;
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("X轴负向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }

}

void MoveToolPage::OnNYBtnReleased() //-Y按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_y,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("Y轴负向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
}

void MoveToolPage::OnNZBtnReleased() //-Z按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_z,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("Z轴负向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
}

void MoveToolPage::OnNRBtnReleased() //-R按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_r,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("R轴负向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
}

void MoveToolPage::OnNR1BtnReleased() //-R1按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_r1,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("R1轴负向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
}

void MoveToolPage::OnNR2BtnReleased() //-R2按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_r2,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("R2轴负向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
    }
}

void MoveToolPage::OnNSBtnReleased() //-S按钮释放
{
    if(speedValue>0)
        speedValue--;
    robot->speedValue=speedValue;
    for(int i=0;i<8;i++)
    {
        int st=robot->ML_SetExtSpeed(i,speedValue);
        if(st>0)
        {
            QString str=QString("设置速度时出错，出错代码为%1,轴号为%1.").arg(st).arg(i);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            break;
        }
    }
    speedLeftEdit->setText(QString("%1%").arg(speedValue*10));
    speedRightEdit->setText(QString("%1%").arg(speedValue*10));
}
////////////////

void MoveToolPage::OnPXBtnPressed() //+X按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_x]>upValue[a_x])
    {
        emit overLimit(a_x,1);
        return;
    }

    flagLimit2[a_x]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(a_x,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_x,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(a_x,POINT_MOVE);
    }
}

void MoveToolPage::OnPYBtnPressed() //+Y按钮按下
{
    int st=0;
    //emit stopCoordinate();  //停止读坐标
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_y]>upValue[a_y])
    {
        emit overLimit(a_y,1);
        return;
    }
    flagLimit2[a_y]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(a_y,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_y,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(a_y,POINT_MOVE);
    }
}

void MoveToolPage::OnPZBtnPressed() //+Z按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_z]>upValue[a_z])
    {
        emit overLimit(a_z,1);
        return;
    }
    flagLimit2[a_z]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(a_z,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_z,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(a_z,POINT_MOVE);
    }
}

void MoveToolPage::OnPRBtnPressed() //+R按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_r]>upValue[a_r])
    {
        emit overLimit(a_r,1);
        return;
    }
    flagLimit2[a_r]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(a_r,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_r,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(a_r,POINT_MOVE);
    }
}

void MoveToolPage::OnPR1BtnPressed() //+R1按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_r1]>upValue[a_r1])
    {
        emit overLimit(a_r1,1);
        return;
    }
    flagLimit2[a_r1]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(a_r1,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_r1,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(a_r1,POINT_MOVE);
    }
}

void MoveToolPage::OnPR2BtnPressed() //+R2按钮按下
{
    int st=0;
    emit readCoordinate();  //开始读坐标
    if(curPointAxisPos[a_r2]>upValue[a_r2])
    {
        emit overLimit(a_r2,1);
        return;
    }
    flagLimit2[a_r2]=false;
    if(lineMoveRdoBtn->isChecked())
    {
        emit signal_Axis_P_Move(a_r2,LINE_MOVE);
    }
    else
    {
        st=robot->ML_Set_Axis_INCJogValue(a_r2,stepLengthComBox->currentIndex()+1);   //设置步距
        if(st>0)
        {
            QString str=QString("设置步距时出错，出错代码为%1.").arg(st);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            return;
        }
        emit signal_Axis_P_Move(a_r2,POINT_MOVE);
    }
}

void MoveToolPage::OnPSBtnPressed() //+S按钮按下
{
}

void MoveToolPage::OnPXBtnReleased() //+X按钮释放
{

    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_x,1);  //立即停止轴移动
        //emit stop_Axis_Move(a_x,1);
    }
    //else
        //printf("MoveToolPage::OnNXBtnReleased(),mode=point, st=%u\n",st);
    //emit stopCoordinate();  //停止读坐标
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("X轴正向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }

}

void MoveToolPage::OnPYBtnReleased() //+Y按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_y,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("Y轴正向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
}

void MoveToolPage::OnPZBtnReleased() //+Z按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_z,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("Z轴正向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
}

void MoveToolPage::OnPRBtnReleased() //+R按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_r,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("R轴正向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
}

void MoveToolPage::OnPR1BtnReleased() //+R1按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_r1,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("R1轴正向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
}

void MoveToolPage::OnPR2BtnReleased() //+R2按钮释放
{
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        st=robot->ML_Stop(a_r2,1);  //立即停止轴移动
    }
    if(st>0)
    {
        QString str=QString("停止轴移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
    st=robot->getMoveStatus();
    if(st>0)
    {
        QString str=QString("R2轴正向移动时出错，出错代码为%1.").arg(st);
        QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
        return;
    }
}

void MoveToolPage::OnPSBtnReleased() //+S按钮释放
{
    if(speedValue<10)
        speedValue++;
    robot->speedValue=speedValue;
    for(int i=0;i<8;i++)
    {
        int st=robot->ML_SetExtSpeed(i,speedValue);
        if(st>0)
        {
            QString str=QString("设置速度时出错，出错代码为%1,轴号为%1.").arg(st).arg(i);
            QMessageBox::critical(NULL,APPLICATION_NAME,str,QMessageBox::Ok);
            break;
        }
    }
    speedLeftEdit->setText(QString("%1%").arg(speedValue*10));
    speedRightEdit->setText(QString("%1%").arg(speedValue*10));
}

void MoveToolPage::OnLockBtnClicked() //姿态锁定
{/*
    pointsAxisPos.append(curPointAxisPos);
    mtracksWidget->pointColor=mtracksWidget->color[0];
    mtracksWidget->updateTracks();
*/
    if(lockPosFlag==0)
    {
        lockPosFlag=1;
        lockButton->setText(tr("姿态解锁"));
        nR1Button->setDisabled(true);
        nR2Button->setDisabled(true);
        pR1Button->setDisabled(true);
        pR2Button->setDisabled(true);
    }
    else
    {
        lockPosFlag=0;
        lockButton->setText(tr("姿态锁定"));
        nR1Button->setDisabled(false);
        nR2Button->setDisabled(false);
        pR1Button->setDisabled(false);
        pR2Button->setDisabled(false);
    }
}

void MoveToolPage::OnLsBtnClicked() //拖焊起点
{
    for(int i=0;i<8;i++)
    {
        mtracksWidget->pointsAxisPos[0][i]=curPointAxisPos[i];
    }
    pointsAxisPos.replace(0,curPointAxisPos);
    mtracksWidget->updateTracks();
    xEdit->setText(QString("%1").arg(curPointAxisPos[0],0,'f',2));
    yEdit->setText(QString("%1").arg(curPointAxisPos[1],0,'f',2));
    zEdit->setText(QString("%1").arg(curPointAxisPos[2],0,'f',2));
    rEdit->setText(QString("%1").arg(curPointAxisPos[3],0,'f',2));
}

void MoveToolPage::OnLeBtnClicked() //拖焊终点
{
    for(int i=0;i<8;i++)
    {
        mtracksWidget->pointsAxisPos[1][i]=curPointAxisPos[i];
    }
    pointsAxisPos.replace(1,curPointAxisPos);
    mtracksWidget->updateTracks();
    xEdit2->setText(QString("%1").arg(curPointAxisPos[0],0,'f',2));
    yEdit2->setText(QString("%1").arg(curPointAxisPos[1],0,'f',2));
    zEdit2->setText(QString("%1").arg(curPointAxisPos[2],0,'f',2));
    rEdit2->setText(QString("%1").arg(curPointAxisPos[3],0,'f',2));
}

void MoveToolPage::OnOneBtnClicked() //阵列第一点
{
    for(int i=0;i<8;i++)
    {
        movetoolMatrixPos[0][i]=curPointAxisPos[i];
        mtracksWidget->pointsAxisPos[0][i]=curPointAxisPos[i];
    }
    pointsAxisPos.replace(0,curPointAxisPos);
    mtracksWidget->updateTracks();
    xEdit->setText(QString("%1").arg(curPointAxisPos[0],0,'f',2));
    yEdit->setText(QString("%1").arg(curPointAxisPos[1],0,'f',2));
    zEdit->setText(QString("%1").arg(curPointAxisPos[2],0,'f',2));
    rEdit->setText(QString("%1").arg(curPointAxisPos[3],0,'f',2));
}

void MoveToolPage::OnTwoBtnClicked() //阵列第二点
{
    for(int i=0;i<8;i++)
    {
        movetoolMatrixPos[1][i]=curPointAxisPos[i];
        mtracksWidget->pointsAxisPos[1][i]=curPointAxisPos[i];
    }
    pointsAxisPos.replace(1,curPointAxisPos);
    mtracksWidget->updateTracks();
    xEdit2->setText(QString("%1").arg(curPointAxisPos[0],0,'f',2));
    yEdit2->setText(QString("%1").arg(curPointAxisPos[1],0,'f',2));
    zEdit2->setText(QString("%1").arg(curPointAxisPos[2],0,'f',2));
    rEdit2->setText(QString("%1").arg(curPointAxisPos[3],0,'f',2));
}

void MoveToolPage::OnLineMoveRdoBtnClicked()
{
    g_moveMod=0;
}

void MoveToolPage::OnPointMoveRdoBtnClicked()
{
    g_moveMod=1;
}

void MoveToolPage::setCurrStepLen(int index)
{
    g_setpLen=index;
}

void MoveToolPage::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
