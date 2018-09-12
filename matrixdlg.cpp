#include "global.h"
#include "matrixdlg.h"
#include "myrwxml.h"

MatrixDlg::MatrixDlg(QWidget *parent) :
    QDialog(parent)
{
    fromStep=0;
    toStep=0;
    rowNum=0;
    colNum=0;
    for(int i=0;i<6;i++)
    {
        onePointPos[i]=0;
        twoPointPos[i]=0;
    }
    dlgWidth=800;
    dlgHeight=400;
    alpha=200;
}

void MatrixDlg::initDialog()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);//border-radius:10px;
    setWindowOpacity(0.5);
    setStyleSheet("QLineEdit { background-color: QColor(255,255,0,10) ; color:white; border:1px groove white; padding:2px 10px;} QPushButton { background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 100), stop:0.5 rgba(236, 236, 236, 100)); border: 1px solid rgb(124, 124, 124); border-radius:10px; color:black;}"
                "QPushButton:pressed{ background:rgba(0,0,255,150); }");

    /*QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);*/
    QFont ft;
    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    ft.setItalic(false);

    connect(this, SIGNAL(loadDlgFinished()),this, SLOT(loadMyLineEdit()));

    createCommandBox();
    createPosBox();
    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    //buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    //buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);
    //buttonLayout->addStretch();

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(commandGroupBox);
    layout->addWidget(pointPosBox);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    setWindowTitle(tr("阵列设置"));
    //resize(800,300);
    //OnUpdatePosBtnClicked();
    //OnUpdatePosBtnClicked2();

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(200,200);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(inout()));
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()),this, SLOT(inout2()));
    fadeTimes = 1000;
    beginPointX=flayX;
    beginPointY=flayY;
    startIn();
}

void MatrixDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,alpha));
}
void MatrixDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}
void MatrixDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

void MatrixDlg::startIn() //飞入
{
    inoutFlag=1;

    timer->start(50);
}

void MatrixDlg::startOut() //飞出
{
    inoutFlag=2;
    timer->start(50);
}

void MatrixDlg::slowlyInOut(int flag) //使对话框透明或不透明 1=透明；2=不透明
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

void MatrixDlg::inout()
{
    if(inoutFlag==1)
    {
        flayX -= 50;
        if(flayX<50)
        {
            flayX=50;
        }
        flayY -= 20;
        if(flayY<120) flayY=120;
        flayWidth=flayWidth+dlgWidth/(fadeTimes/timer->interval());
        if(flayWidth> dlgWidth ) flayWidth=dlgWidth;
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
    if((flayX==50) && (flayY==120) && (flayWidth==dlgWidth) && (flayHeight==dlgHeight))
    {
        inoutFlag=0;
        emit loadDlgFinished();     //触发信号，加载自定义文本框
        timer->stop();
        emit useMoveKeyDlg(1);   //触发信号，打开movetool键盘
    }
}

void MatrixDlg::inout2()
{
    if(inoutFlag==3)
    {
        alpha-=20;
        if(alpha<20)
        {
            alpha=20;
            inoutFlag=0;
            timer2->stop();
        }
        update();
    }
    else if(inoutFlag==4)
    {
        alpha+=20;
        if(alpha>200)
        {
            alpha=200;
            inoutFlag=0;
            timer2->stop();
            emit loadDlgFinished();     //触发信号，加载自定义文本框
        }
        update();
    }
}

void MatrixDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
}

void MatrixDlg::createCommandBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *stepLabel=new QLabel(tr("指定阵列的步骤"));
    stepLabel->setFont(ft);
    QLabel *fromLabel=new QLabel(tr("从"));
    fromLabel->setFont(ft);
    fromLabel->setAlignment(Qt::AlignRight);
    fromEdit=new MyLineEdit;
    fromEdit->setFont(ft);
    fromEdit->setAlignment(Qt::AlignHCenter);
    QLabel *toLabel=new QLabel(tr("步    到"));
    toLabel->setFont(ft);
    toEdit=new MyLineEdit;
    toEdit->setFont(ft);
    toEdit->setAlignment(Qt::AlignHCenter);
    QLabel *buLabel=new QLabel(tr("步"));
    buLabel->setFont(ft);
    buLabel->setAlignment(Qt::AlignLeft);
    QPushButton *selectAllButton=new QPushButton(tr("全选"));
    selectAllButton->setFont(ft);
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(OnSelAllBtnClicked()));

    QHBoxLayout *commLayout=new QHBoxLayout;
    commLayout->addWidget(fromLabel);
    commLayout->addWidget(fromEdit);
    commLayout->addWidget(toLabel);
    commLayout->addWidget(toEdit);
    commLayout->addWidget(buLabel);
    commLayout->addStretch();
    commLayout->addWidget(selectAllButton);

    QVBoxLayout *boxLayout=new QVBoxLayout;
    boxLayout->addWidget(stepLabel);
    boxLayout->addLayout(commLayout);

    commandGroupBox=new QGroupBox;
    commandGroupBox->setLayout(boxLayout);
}

void MatrixDlg::createPosBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *stepLabel=new QLabel(tr("指定阵列的大小"));
    stepLabel->setFont(ft);

    QLabel *rowLabel=new QLabel(tr("行数"));
    rowLabel->setFont(ft);
    rowLabel->setAlignment(Qt::AlignRight);
    rowEdit=new MyLineEdit;
    rowEdit->setFont(ft);
    rowEdit->setAlignment(Qt::AlignHCenter);
    QLabel *colLabel=new QLabel(tr("列数"));
    colLabel->setFont(ft);
    colLabel->setAlignment(Qt::AlignRight);
    colEdit=new MyLineEdit;
    colEdit->setFont(ft);
    colEdit->setAlignment(Qt::AlignHCenter);

    QHBoxLayout *rowcolLayout=new QHBoxLayout;
    rowcolLayout->addWidget(rowLabel);
    rowcolLayout->addWidget(rowEdit);
    rowcolLayout->addWidget(colLabel);
    rowcolLayout->addWidget(colEdit);

    QLabel *lastRowLabel=new QLabel(tr("末行首点坐标"));
    lastRowLabel->setFont(ft);

    QLabel *xLabel=new QLabel(tr("X:"));
    xLabel->setFont(ft);
    xLabel->setAlignment(Qt::AlignRight);
    QLabel *yLabel=new QLabel(tr("Y:"));
    yLabel->setFont(ft);
    yLabel->setAlignment(Qt::AlignRight);
    QLabel *zLabel=new QLabel(tr("Z:"));
    zLabel->setFont(ft);
    zLabel->setAlignment(Qt::AlignRight);
    QLabel *rLabel=new QLabel(tr("R:"));
    rLabel->setFont(ft);
    rLabel->setAlignment(Qt::AlignRight);
    QLabel *r1Label=new QLabel(tr("R1:"));
    r1Label->setFont(ft);
    r1Label->setAlignment(Qt::AlignRight);
    QLabel *r2Label=new QLabel(tr("R2:"));
    r2Label->setFont(ft);
    r2Label->setAlignment(Qt::AlignRight);
    QPushButton *updatePosButton=new QPushButton(tr("更新坐标"));
    updatePosButton->setFont(ft);
    connect(updatePosButton, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtnClicked()));
    for(int i=0;i<6;i++)
        onePointPosEdit.append(new MyLineEdit);
    for(int i=0;i<6;i++)
        onePointPosEdit[i]->setFont(ft);
    for(int i=2;i<6;i++)
        onePointPosEdit[i]->setEnabled(false);

    QHBoxLayout *oneRowLayout=new QHBoxLayout;
    oneRowLayout->addWidget(xLabel);
    oneRowLayout->addWidget(onePointPosEdit[0]);
    oneRowLayout->addWidget(yLabel);
    oneRowLayout->addWidget(onePointPosEdit[1]);
    oneRowLayout->addWidget(zLabel);
    oneRowLayout->addWidget(onePointPosEdit[2]);
    oneRowLayout->addWidget(rLabel);
    oneRowLayout->addWidget(onePointPosEdit[3]);
    oneRowLayout->addWidget(r1Label);
    oneRowLayout->addWidget(onePointPosEdit[4]);
    oneRowLayout->addWidget(r2Label);
    oneRowLayout->addWidget(onePointPosEdit[5]);
    oneRowLayout->addWidget(updatePosButton);

    QLabel *lastColLabel=new QLabel(tr("末列首点坐标"));
    lastColLabel->setFont(ft);

    QLabel *xLabel2=new QLabel(tr("X:"));
    xLabel2->setFont(ft);
    xLabel2->setAlignment(Qt::AlignRight);
    QLabel *yLabel2=new QLabel(tr("Y:"));
    yLabel2->setFont(ft);
    yLabel2->setAlignment(Qt::AlignRight);
    QLabel *zLabel2=new QLabel(tr("Z:"));
    zLabel2->setFont(ft);
    zLabel2->setAlignment(Qt::AlignRight);
    QLabel *rLabel2=new QLabel(tr("R:"));
    rLabel2->setFont(ft);
    rLabel2->setAlignment(Qt::AlignRight);
    QLabel *r1Label2=new QLabel(tr("R1:"));
    r1Label2->setFont(ft);
    r1Label2->setAlignment(Qt::AlignRight);
    QLabel *r2Label2=new QLabel(tr("R2:"));
    r2Label2->setFont(ft);
    r2Label2->setAlignment(Qt::AlignRight);
    QPushButton *updatePosButton2=new QPushButton(tr("更新坐标"));
    updatePosButton2->setFont(ft);
    connect(updatePosButton2, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtnClicked2()));
    for(int i=0;i<6;i++)
        twoPointPosEdit.append(new MyLineEdit);
    for(int i=0;i<6;i++)
        twoPointPosEdit[i]->setFont(ft);
    for(int i=2;i<6;i++)
        twoPointPosEdit[i]->setEnabled(false);

    QHBoxLayout *twoRowLayout=new QHBoxLayout;
    twoRowLayout->addWidget(xLabel2);
    twoRowLayout->addWidget(twoPointPosEdit[0]);
    twoRowLayout->addWidget(yLabel2);
    twoRowLayout->addWidget(twoPointPosEdit[1]);
    twoRowLayout->addWidget(zLabel2);
    twoRowLayout->addWidget(twoPointPosEdit[2]);
    twoRowLayout->addWidget(rLabel2);
    twoRowLayout->addWidget(twoPointPosEdit[3]);
    twoRowLayout->addWidget(r1Label2);
    twoRowLayout->addWidget(twoPointPosEdit[4]);
    twoRowLayout->addWidget(r2Label2);
    twoRowLayout->addWidget(twoPointPosEdit[5]);
    twoRowLayout->addWidget(updatePosButton2);

    QVBoxLayout *boxLayout=new QVBoxLayout;
    boxLayout->addWidget(stepLabel);
    boxLayout->addLayout(rowcolLayout);
    boxLayout->addWidget(lastRowLabel);
    boxLayout->addLayout(oneRowLayout);
    boxLayout->addWidget(lastColLabel);
    boxLayout->addLayout(twoRowLayout);

    pointPosBox=new QGroupBox;
    pointPosBox->setLayout(boxLayout);
}

void MatrixDlg::OnOkBtnClicked()
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

    MyRWxml xml;
    SYSTEM_PARAMS systemPara;
    xml.readSysPara("robot.xml",systemPara);
    double xLimPos,yLimPos,xUpPos=0,xDownPos=0,yUpPos=0,yDownPos=0;
    yLimPos=systemPara.limitPosLen[1];  //Y轴行程
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        xLimPos=systemPara.limitPosLen[0]/2;  //X轴半行程
        if(onePointPos[0]>xLimPos)  //右边工位
        {
            xDownPos=xLimPos;
            xUpPos=systemPara.limitPosLen[0];
            yDownPos=0;
            yUpPos=yLimPos;
        }
        else   //左边工位
        {
            xDownPos=0;
            xUpPos=systemPara.limitPosLen[0];
            yDownPos=0;
            yUpPos=yLimPos;
        }
    }
    else    //单平台
    {
        xLimPos=systemPara.limitPosLen[0];  //X轴行程
        xDownPos=0;
        xUpPos=xLimPos;
        yDownPos=0;
        yUpPos=yLimPos;
    }

    double firstPointPos[4]={0,0,0,0};
    fromStep=fromEdit->text().trimmed().toInt();
    toStep=toEdit->text().trimmed().toInt();
    rowNum=rowEdit->text().trimmed().toInt();
    colNum=colEdit->text().trimmed().toInt();
    for(int i=0;i<6;i++)
    {
        onePointPos[i]=onePointPosEdit[i]->text().trimmed().toDouble();
        twoPointPos[i]=twoPointPosEdit[i]->text().trimmed().toDouble();
        if(i<4)
            firstPointPos[i]=weldProject->curPointPos[i];
    }
 /*   //找出有坐标的第1个点
    for(int i=fromStep;i<=toStep;i++)
    {
        if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("拖焊")))
                 || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
        {
            for(int k=0;k<4;k++)
                firstPointPos[k]=weldProject->pointsAxisPos[i][k];
            break;
        }
    }*/
    //计算x方向斜率
    double xSlope;
    xSlope=(onePointPos[0]-firstPointPos[0])/(onePointPos[1]-firstPointPos[1]); //(x1-x0)/(y1-y0)
    //计算y方向斜率
    double ySlope;
    ySlope=(twoPointPos[1]-firstPointPos[1])/(twoPointPos[0]-firstPointPos[0]); //(y1-y0)/(x1-x0)
/*    //计算列间隔大小
    double xSpacing=(twoPointPos[0]-firstPointPos[0])/(colNum-1);   //(末列首点x-首列首点x)/(列数-1)
    //计算行间隔大小
    double ySpacing=(onePointPos[1]-firstPointPos[1])/(rowNum-1);   //(末行首点y-首行首点y)/(行数-1)

    //计算x方向斜率
    double xSlope;
    xSlope=(twoPointPos[0]-onePointPos[0])/(twoPointPos[1]-onePointPos[1]); //(x1-x0)/(y1-y0)
    //计算y方向斜率
    double ySlope;
    ySlope=(twoPointPos[1]-onePointPos[1])/(twoPointPos[0]-onePointPos[0]); //(y1-y0)/(x1-x0)
*/
    //计算列间隔大小
    double xSpacing=(twoPointPos[0]-onePointPos[0])/(colNum-1);   //(末列首点x-首列首点x)/(列数-1)
    //计算行间隔大小
    double ySpacing=(onePointPos[1]-twoPointPos[1])/(rowNum-1);   //(末行首点y-首行首点y)/(行数-1)
    xOffset=ySpacing*xSlope;
    yOffset=xSpacing*ySlope;

    int matrixCommandNum=toStep-fromStep+1; //所选的操作命令数
    int newCommandNum=weldProject->listCommand.size();   //最后一个操作命令的下面新增的第一条的序号
    int outRange=0;
    for(int i=0;i<rowNum;i++)
    {
        for(int k=0;k<colNum;k++)
        {
            if((i==0) && (k==0)) continue;
            for(int j=fromStep;j<=toStep;j++)
            {
                if(i==0 && k==2 && j==1)
                    toStep=toStep;
                weldProject->listCommand.append(weldProject->listCommand[j]);
                weldProject->listTechnics.append(weldProject->listTechnics[j]);
                weldProject->Zh.append(weldProject->Zh[j]);
                weldProject->optimize.append(weldProject->optimize[j]);
                weldProject->pointsAxisPos.append(weldProject->pointsAxisPos[j]);
                weldProject->pointsAxisPos2.append(weldProject->pointsAxisPos2[j]);
                //int newOperateNum=newCommandNum+(i*colNum+k-1)*matrixCommandNum+j-fromStep; //新增的操作命令的序号
                int newOperateNum=newCommandNum+(i*colNum+k-1)*matrixCommandNum+j-fromStep; //新增的操作命令的序号
                if(weldProject->listCommand[j]==QString(tr("点焊")))
                {
                    double x=weldProject->pointsAxisPos[j][0]+xSpacing*k+xOffset*i;
                    double y=weldProject->pointsAxisPos[j][1]+ySpacing*i+yOffset*k;
                    if((x<xDownPos) || (x>xUpPos) || (y<yDownPos) || (y>yUpPos))
                    {
                        outRange=1;
                        break;
                    }
                    //printf("pointsAxisPos[%d][0]=%.1f  , pointsAxisPos[%d][1]=%.1f\n",newOperateNum,x,newOperateNum,y);
                    weldProject->pointsAxisPos[newOperateNum][0]=x;
                    weldProject->pointsAxisPos[newOperateNum][1]=y;//
                    /*if(i==0 && k==colNum-1 && j==fromStep)
                    {
                    }*/
                }
                else if(weldProject->listCommand[j]==QString(tr("拖焊")) || weldProject->listCommand[j]==QString(tr("多拖1"))
                         || weldProject->listCommand[j]==QString(tr("多拖2")) || weldProject->listCommand[j]==QString(tr("多拖3")))
                {
                    double x1=weldProject->pointsAxisPos[j][0]+xSpacing*k;//+xOffset*i;
                    double y1=weldProject->pointsAxisPos[j][1]+ySpacing*i;//+yOffset*k;
                    double x2=weldProject->pointsAxisPos[j][4]+xSpacing*k;//+xOffset*i;
                    double y2=weldProject->pointsAxisPos[j][5]+ySpacing*i;//+yOffset*k;
                    if((x1<xDownPos) || (x1>xUpPos) || (y1<yDownPos) || (y1>yUpPos) || (x2<xDownPos) || (x2>xUpPos)  || (y2<yDownPos) || (y2>yUpPos))
                    {
                        outRange=1;
                        break;
                    }
                    //printf("pointsAxisPos[%d][0]=%.1f  , pointsAxisPos[%d][1]=%.1f\n",newOperateNum,x,newOperateNum,y);
                    weldProject->pointsAxisPos[newOperateNum][0]=x1;
                    weldProject->pointsAxisPos[newOperateNum][1]=y1;//
                    weldProject->pointsAxisPos[newOperateNum][4]=x2;
                    weldProject->pointsAxisPos[newOperateNum][5]=y2;//
                }
            }
            if(outRange==1)
            {
                msgBox.setText(tr("阵列坐标超出标准范围."));
                msgBox.exec();
                int cnt=weldProject->listCommand.size();
                if(newCommandNum<cnt)
                {
                    for(int i=cnt-1;i<newCommandNum;i--)
                    {
                        weldProject->listCommand.removeAt(i);
                        weldProject->listTechnics.removeAt(i);
                        weldProject->Zh.removeAt(i);
                        weldProject->optimize.removeAt(i);
                        weldProject->pointsAxisPos.removeAt(i);
                    }
                    QString prjName=weldProject->getProjName();
                    weldProject->clear();
                    weldProject->loadProject(prjName);
                    emit weldProject->reloadTableSig();
                }
                return;
            }

        }
    }
    weldProject->saveProject();

    isOK=true;
    startOut();
}

void MatrixDlg::OnCancelBtnClicked()
{
    isOK=false;
    startOut();
}

void MatrixDlg::OnSelAllBtnClicked()
{
    if(weldProject)
    {
        int commNum=weldProject->listCommand.size()-1;
        fromEdit->setText(QString("0"));
        toEdit->setText(QString("%1").arg(commNum));
    }
}

void MatrixDlg::OnUpdatePosBtnClicked()
{
    for(int i=0;i<4;i++)
        onePointPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    onePointPosEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
    onePointPosEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',2));
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        onePointPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
    }
}

void MatrixDlg::OnUpdatePosBtnClicked2()
{
    for(int i=0;i<4;i++)
        twoPointPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    twoPointPosEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
    twoPointPosEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',2));
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        twoPointPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
    }
}
void MatrixDlg::loadMyLineEdit()      //加载自定义文本框
{
    fromEdit->context=context;
    QRect rect=fromEdit->geometry();
    fromEdit->myPos=QPoint(rect.left(),rect.top()+20);
    fromEdit->parentHeight=geometry().height();

    toEdit->context=context;
    rect=toEdit->geometry();
    toEdit->myPos=QPoint(rect.left(),rect.top()+20);
    toEdit->parentHeight=geometry().height();

    rowEdit->context=context;
    rect=rowEdit->geometry();
    rowEdit->myPos=QPoint(rect.left(),rect.top()-30);
    rowEdit->parentHeight=geometry().height();

    colEdit->context=context;
    rect=colEdit->geometry();
    colEdit->myPos=QPoint(rect.left(),rect.top()-30);
    colEdit->parentHeight=geometry().height();

    for(int i=0;i<6;i++)
    {
        onePointPosEdit[i]->context=context;
        QRect rect=onePointPosEdit[i]->geometry();
        onePointPosEdit[i]->myPos=onePointPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        onePointPosEdit[i]->parentHeight=geometry().height();

        twoPointPosEdit[i]->context=context;
        rect=twoPointPosEdit[i]->geometry();
        twoPointPosEdit[i]->myPos=twoPointPosEdit[i]->parentWidget()->mapToParent(QPoint(rect.left(),rect.top()));
        twoPointPosEdit[i]->parentHeight=geometry().height();
    }
}

