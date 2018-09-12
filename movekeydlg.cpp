#include "movekeydlg.h"
#include "myrwxml.h"

MoveKeyDlg::MoveKeyDlg(QWidget *parent) :
    QDialog(parent)
{
    flayX=0;
    flayY=0;
    beginPointX=0;
    endPointX=0;
    lockPosFlag=0;
    speedValue=2;
}

void MoveKeyDlg::initDialog()
{
    inoutFlag=0;
    MyRWxml xml;
    SYSTEM_PARAMS systemPara;
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
    downValue[a_y2]=10;

    halfX=systemPara.limitPosLen[0]/2;

    for(int i=0;i<8;i++)
        flagLimit[i]=false;
    for(int i=0;i<8;i++)
        flagLimit2[i]=false;
    g_MoveKeyPressed=0;

    curPointAxisPos=QVector<double> (8);
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=0;
    pointsAxisPos.append(curPointAxisPos);
    pointsAxisPos.append(curPointAxisPos);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(inout()));

    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MID_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    //setWindowOpacity(0.9);

    createButtonBox();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(keyBtnBox);
    setLayout(layout);

    connect(lineMoveRdoBtn, SIGNAL(clicked()),this, SLOT(OnLineMoveRdoBtnClicked()));
    connect(pointMoveRdoBtn, SIGNAL(clicked()),this, SLOT(OnPointMoveRdoBtnClicked()));
    connect(stepLengthComBox, SIGNAL(currentIndexChanged(int)),this, SLOT(setCurrStepLen(int)));
    if(!((g_MacType==3) || (g_MacType==4) || (g_MacType==1)))    //非6轴
    {
        OnLockBtnClicked(); //姿态锁定
        lockButton->setEnabled(false);
    }
}

void MoveKeyDlg::closeDlg()
{
    startOut();
}

void MoveKeyDlg::startIn() //飞入
{
    inoutFlag=1;
    timer->start(50);
}

void MoveKeyDlg::startOut() //飞出
{
    inoutFlag=2;
    timer->start(50);
}

void MoveKeyDlg::inout()
{
    if(inoutFlag==1)
    {
        flayX -= 50;
        if(flayX<beginPointX) flayX=beginPointX;
        move(flayX,flayY);
    }
    else if(inoutFlag==2)
    {
        flayX += 50;
        if(flayX>endPointX) flayX=endPointX;
        move(flayX,flayY);
        if((flayX==endPointX))
        {
            inoutFlag=0;
            timer->stop();
             done(QDialog::Accepted);
        }
    }
    if(flayX==beginPointX)
    {
        inoutFlag=0;
        timer->stop();
    }
}

void MoveKeyDlg::createButtonBox()
{
    QFont ft;
    ft.setPointSize(12);
    ft.setItalic(false);

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
    QPushButton *nY2Button=new QPushButton(tr("-Y2"));
    nY2Button->setFont(ft);
    connect(nY2Button, SIGNAL(pressed()), this, SLOT(OnNY2BtnPressed()));
    connect(nY2Button, SIGNAL(released()), this, SLOT(OnNY2BtnReleased()));

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
    QPushButton *pY2Button=new QPushButton(tr("+Y2"));
    pY2Button->setFont(ft);
    connect(pY2Button, SIGNAL(pressed()), this, SLOT(OnPY2BtnPressed()));
    connect(pY2Button, SIGNAL(released()), this, SLOT(OnPY2BtnReleased()));

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

    speedEdit = new QLineEdit;
    speedEdit->setFont(ft);
    speedEdit->setReadOnly(true);
    speedEdit->setText(QString("%1%").arg(speedValue*10));
    speedEdit->setAlignment(Qt::AlignCenter);
    r1CoordEdit=new QLineEdit;
    r1CoordEdit->setFont(ft);
    r1CoordEdit->setAlignment(Qt::AlignCenter);
    r2CoordEdit=new QLineEdit;
    r2CoordEdit->setFont(ft);
    r2CoordEdit->setAlignment(Qt::AlignCenter);
    ft.setPointSize(8);
    QLabel *r1Label=new QLabel(tr("R1坐标"));
    r1Label->setFont(ft);
    QLabel *r2Label=new QLabel(tr("R2坐标"));
    r2Label->setFont(ft);

    lineMoveRdoBtn=new QRadioButton(tr("连续动"));
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
    ft.setPointSize(10);
    stepLengthComBox=new QComboBox;
    stepLengthComBox->setFont(ft);
    stepLengthComBox->addItem("0.1");
    stepLengthComBox->addItem("0.5");
    stepLengthComBox->addItem("1");
    stepLengthComBox->addItem("10");
    stepLengthComBox->addItem("1.27");
    stepLengthComBox->addItem("2.54");
    stepLengthComBox->addItem("3.96");
    stepLengthComBox->addItem("5.08");
    stepLengthComBox->setCurrentIndex(g_setpLen);
    ft.setPointSize(12);
    lockButton=new QPushButton(tr("姿态锁定"));
    lockButton->setFont(ft);
    connect(lockButton, SIGNAL(clicked()), this, SLOT(OnLockBtnClicked()));
    if(!((g_MacType==3) || (g_MacType==4) || (g_MacType==1)))    //非6轴
    {
        OnLockBtnClicked(); //姿态锁定
        lockButton->setDisabled(true);
    }

    QHBoxLayout *r1Layout = new QHBoxLayout;
    r1Layout->addWidget(r1Label);
    r1Layout->addWidget(r1CoordEdit);
    QHBoxLayout *r2Layout = new QHBoxLayout;
    r2Layout->addWidget(r2Label);
    r2Layout->addWidget(r2CoordEdit);

    QGridLayout *btnLayout=new QGridLayout;
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        btnLayout->addWidget(nXButton,0,0);
        btnLayout->addWidget(pXButton,0,1);
        btnLayout->addWidget(nYButton,1,0);
        btnLayout->addWidget(pYButton,1,1);
        btnLayout->addWidget(nY2Button,3,0);
        btnLayout->addWidget(pY2Button,3,1);
        btnLayout->addWidget(nZButton,4,0);
        btnLayout->addWidget(pZButton,4,1);
        btnLayout->addWidget(nRButton,5,0);
        btnLayout->addWidget(pRButton,5,1);
        btnLayout->addLayout(r1Layout,6,0,1,2);
        btnLayout->addLayout(r2Layout,7,0,1,2);

        btnLayout->addWidget(nR1Button,8,0);
        btnLayout->addWidget(pR1Button,8,1);
        btnLayout->addWidget(nR2Button,9,0);
        btnLayout->addWidget(pR2Button,9,1);
        btnLayout->addWidget(speedEdit,10,0,1,2);
        btnLayout->addWidget(nSButton,11,0);
        btnLayout->addWidget(pSButton,11,1);
        btnLayout->addWidget(setpLabel,12,0);
        btnLayout->addWidget(stepLengthComBox,12,1);
        btnLayout->addWidget(lineMoveRdoBtn,13,0);
        btnLayout->addWidget(pointMoveRdoBtn,13,1);
        btnLayout->addWidget(lockButton,14,0,1,2);
    }
    else
    {
        btnLayout->addWidget(nXButton,0,0);
        btnLayout->addWidget(pXButton,0,1);
        btnLayout->addWidget(nYButton,1,0);
        btnLayout->addWidget(pYButton,1,1);
        btnLayout->addWidget(nZButton,2,0);
        btnLayout->addWidget(pZButton,2,1);
        btnLayout->addWidget(nRButton,3,0);
        btnLayout->addWidget(pRButton,3,1);
        btnLayout->addLayout(r1Layout,4,0,1,2);
        btnLayout->addLayout(r2Layout,5,0,1,2);

        btnLayout->addWidget(nR1Button,6,0);
        btnLayout->addWidget(pR1Button,6,1);
        btnLayout->addWidget(nR2Button,7,0);
        btnLayout->addWidget(pR2Button,7,1);
        btnLayout->addWidget(speedEdit,8,0,1,2);
        btnLayout->addWidget(nSButton,9,0);
        btnLayout->addWidget(pSButton,9,1);
        btnLayout->addWidget(setpLabel,10,0);
        btnLayout->addWidget(stepLengthComBox,10,1);
        btnLayout->addWidget(lineMoveRdoBtn,11,0);
        btnLayout->addWidget(pointMoveRdoBtn,11,1);
        btnLayout->addWidget(lockButton,12,0,1,2);
    }

    keyBtnBox=new QGroupBox;
    keyBtnBox->setLayout(btnLayout);
    keyBtnBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(125,45,200, 255)); border-radius:10px; border:none}");
}

void MoveKeyDlg::updateCoordinate(double *coordinate)
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
    etracksWidget->xEdit->setText(QString("%1").arg(coordinate[0],0,'f',2));
    etracksWidget->yEdit->setText(QString("%1").arg(coordinate[1],0,'f',2));
    etracksWidget->y2Edit->setText(QString("%1").arg(coordinate[7],0,'f',2));
    etracksWidget->zEdit->setText(QString("%1").arg(coordinate[2],0,'f',2));
    etracksWidget->rEdit->setText(QString("%1").arg(coordinate[3],0,'f',2));
    r1CoordEdit->setText(QString("%1").arg(coordinate[5],0,'f',2));
    r2CoordEdit->setText(QString("%1").arg(coordinate[6],0,'f',2));

    etracksWidget->pointColor=etracksWidget->color[2];
    etracksWidget->left_Z_Slider->setValue(curPointAxisPos[2]);
    etracksWidget->down_R_Slider->setValue(curPointAxisPos[3]);
    etracksWidget->up_R1_Slider->setValue(curPointAxisPos[5]);
    etracksWidget->right_R2_Slider->setValue(curPointAxisPos[6]);
    etracksWidget->point.setX(curPointAxisPos[0]);
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        if(curPointAxisPos[0]>halfX)    //Y2
            etracksWidget->point.setY(curPointAxisPos[7]);
        else
            etracksWidget->point.setY(curPointAxisPos[1]);
    }
    else
        etracksWidget->point.setY(curPointAxisPos[1]);
    etracksWidget->updateTracks();

    lockButton->setDisabled(false);
}

void MoveKeyDlg::OnNXBtnPressed() //-X按钮按下
{
    if(curPointAxisPos[a_x]<downValue[a_x])
    {
        emit overLimit(a_x,0);
        return;
    }

    flagLimit[a_x]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=0;
    robot->keySelFlag2=0;
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键1的灯

    emit start_Axis_N_Move(a_x);
}

void MoveKeyDlg::OnNYBtnPressed() //-Y按钮按下
{
    if(curPointAxisPos[a_y]<downValue[a_y])
    {
        emit overLimit(a_y,0);
        return;
    }
    flagLimit[a_y]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=0;
    robot->keySelFlag2=0;
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键1的灯
    emit start_Axis_N_Move(a_y);
}

void MoveKeyDlg::OnNY2BtnPressed() //-Y2按钮按下
{
    if(curPointAxisPos[a_y2]<downValue[a_y2])
    {
        emit overLimit(a_y2,0);
        return;
    }
    flagLimit[a_y2]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=0;
    robot->keySelFlag2=1;
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE2]);    //点亮选择键2的灯
    emit start_Axis_N_Move(a_y2);
}

void MoveKeyDlg::OnNZBtnPressed() //-Z按钮按下
{
    if(curPointAxisPos[a_z]<downValue[a_z])
    {
        emit overLimit(a_z,0);
        return;
    }
    flagLimit[a_z]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=0;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键2的灯
    emit start_Axis_N_Move(a_z);
}

void MoveKeyDlg::OnNRBtnPressed() //-R按钮按下
{
    if(curPointAxisPos[a_r]<downValue[a_r])
    {
        emit overLimit(a_r,0);
        return;
    }
    flagLimit[a_r]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=0;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键2的灯
    emit start_Axis_N_Move(a_r);
}

void MoveKeyDlg::OnNR1BtnPressed() //-R1按钮按下
{
    if(curPointAxisPos[a_r1]<downValue[a_r1])
    {
        emit overLimit(a_r1,0);
        return;
    }
    flagLimit[a_r1]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=1;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE2]);    //点亮选择键2的灯
    emit start_Axis_N_Move(a_r1);
}

void MoveKeyDlg::OnNR2BtnPressed() //-R2按钮按下
{
    if(curPointAxisPos[a_r2]<downValue[a_r2])
    {
        emit overLimit(a_r2,0);
        return;
    }
    flagLimit[a_r2]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=1;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE2]);    //点亮选择键2的灯
    emit start_Axis_N_Move(a_r2);
}

void MoveKeyDlg::OnNSBtnPressed() //-S按钮按下
{
}

void MoveKeyDlg::OnNXBtnReleased() //-X按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_x,0);  //停止轴移动
        emit stop_Axis_Move(a_x,0); //停止轴移动
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

void MoveKeyDlg::OnNYBtnReleased() //-Y按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_y,0);  //立即停止轴移动
        emit stop_Axis_Move(a_y,0); //停止轴移动
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

void MoveKeyDlg::OnNY2BtnReleased() //-Y2按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_y2,0);  //立即停止轴移动
        emit stop_Axis_Move(a_y2,0); //停止轴移动
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

void MoveKeyDlg::OnNZBtnReleased() //-Z按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_z,0);  //立即停止轴移动
        emit stop_Axis_Move(a_z,0); //停止轴移动
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

void MoveKeyDlg::OnNRBtnReleased() //-R按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_r,0);  //立即停止轴移动
        emit stop_Axis_Move(a_r,0); //停止轴移动
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

void MoveKeyDlg::OnNR1BtnReleased() //-R1按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_r1,0);  //立即停止轴移动
        emit stop_Axis_Move(a_r1,0); //停止轴移动
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

void MoveKeyDlg::OnNR2BtnReleased() //-R2按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_r2,0);  //立即停止轴移动
        emit stop_Axis_Move(a_r2,0); //停止轴移动
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

void MoveKeyDlg::OnNSBtnReleased() //-S按钮释放
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
    speedEdit->setText(QString("%1%").arg(speedValue*10));
    //speedRightEdit->setText(QString("%1%").arg(speedValue*10));
}
////////////////

void MoveKeyDlg::OnPXBtnPressed() //+X按钮按下
{
    if(curPointAxisPos[a_x]>upValue[a_x])
    {
        emit overLimit(a_x,1);
        return;
    }

    flagLimit2[a_x]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=0;
    robot->keySelFlag2=0;
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键1的灯
    emit start_Axis_P_Move(a_x);
}

void MoveKeyDlg::OnPYBtnPressed() //+Y按钮按下
{
    if(curPointAxisPos[a_y]>upValue[a_y])
    {
        emit overLimit(a_y,1);
        return;
    }
    flagLimit2[a_y]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=0;
    robot->keySelFlag2=0;
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键1的灯
    emit start_Axis_P_Move(a_y);
}

void MoveKeyDlg::OnPY2BtnPressed() //+Y2按钮按下
{
    if(curPointAxisPos[a_y2]>upValue[a_y2])
    {
        emit overLimit(a_y2,1);
        return;
    }
    flagLimit2[a_y2]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=0;
    robot->keySelFlag2=1;
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE2]);    //点亮选择键2的灯
    emit start_Axis_P_Move(a_y2);
}

void MoveKeyDlg::OnPZBtnPressed() //+Z按钮按下
{
    if(curPointAxisPos[a_z]>upValue[a_z])
    {
        emit overLimit(a_z,1);
        return;
    }
    flagLimit2[a_z]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=0;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键2的灯
    emit start_Axis_P_Move(a_z);
}

void MoveKeyDlg::OnPRBtnPressed() //+R按钮按下
{
    if(curPointAxisPos[a_r]>upValue[a_r])
    {
        emit overLimit(a_r,1);
        return;
    }
    flagLimit2[a_r]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=0;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_ClearOutput(g_IOxb.sysOxb[ORE2]);  //熄灭选择键2的灯
    emit start_Axis_P_Move(a_r);
}

void MoveKeyDlg::OnPR1BtnPressed() //+R1按钮按下
{
    if(curPointAxisPos[a_r1]>upValue[a_r1])
    {
        emit overLimit(a_r1,1);
        return;
    }
    flagLimit2[a_r1]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=1;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE2]);    //点亮选择键2的灯
    emit start_Axis_P_Move(a_r1);
}

void MoveKeyDlg::OnPR2BtnPressed() //+R2按钮按下
{
    if(curPointAxisPos[a_r2]>upValue[a_r2])
    {
        emit overLimit(a_r2,1);
        return;
    }
    flagLimit2[a_r2]=false;
    g_MoveKeyPressed=1;
    robot->keySelFlag1=1;
    robot->keySelFlag2=1;
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
    robot->ML_SetOutput(g_IOxb.sysOxb[ORE2]);    //点亮选择键2的灯
    emit start_Axis_P_Move(a_r2);
}

void MoveKeyDlg::OnPSBtnPressed() //+S按钮按下
{
}

void MoveKeyDlg::OnPXBtnReleased() //+X按钮释放
{

    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_x,0);  //立即停止轴移动
        emit stop_Axis_Move(a_x,0);
    }
    //else
        //printf("MoveKeyDlg::OnNXBtnReleased(),mode=point, st=%u\n",st);
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

void MoveKeyDlg::OnPYBtnReleased() //+Y按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_y,0);  //立即停止轴移动
        emit stop_Axis_Move(a_y,0); //停止轴移动
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

void MoveKeyDlg::OnPY2BtnReleased() //+Y2按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_y2,0);  //立即停止轴移动
        emit stop_Axis_Move(a_y2,0); //停止轴移动
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

void MoveKeyDlg::OnPZBtnReleased() //+Z按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_z,0);  //立即停止轴移动
        emit stop_Axis_Move(a_z,0); //停止轴移动
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

void MoveKeyDlg::OnPRBtnReleased() //+R按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_r,0);  //立即停止轴移动
        emit stop_Axis_Move(a_r,0); //停止轴移动
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

void MoveKeyDlg::OnPR1BtnReleased() //+R1按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_r1,0);  //立即停止轴移动
        emit stop_Axis_Move(a_r1,0); //停止轴移动
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

void MoveKeyDlg::OnPR2BtnReleased() //+R2按钮释放
{
    g_MoveKeyPressed=0;
    int st=0;
    if(lineMoveRdoBtn->isChecked())
    {
        //st=robot->ML_Stop(a_r2,0);  //立即停止轴移动
        emit stop_Axis_Move(a_r2,0); //停止轴移动
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

void MoveKeyDlg::OnPSBtnReleased() //+S按钮释放
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
    speedEdit->setText(QString("%1%").arg(speedValue*10));
}

void MoveKeyDlg::OnLockBtnClicked() //姿态锁定
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

void MoveKeyDlg::OnLineMoveRdoBtnClicked()
{
    g_moveMod=LINE_MOVE;
}

void MoveKeyDlg::OnPointMoveRdoBtnClicked()
{
    g_moveMod=POINT_MOVE;
}

void MoveKeyDlg::setCurrStepLen(int index)
{
    g_setpLen=index;
}

void MoveKeyDlg::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
