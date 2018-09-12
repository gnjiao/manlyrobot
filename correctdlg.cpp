#include "correctdlg.h"
#include "global.h"
#include "myrwxml.h"
#include "msqldb.h"

CorrectDlg::CorrectDlg(QWidget *parent) :
    QDialog(parent)
{
    correctType=0;
    initDialog();
}

void CorrectDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    parallelBtn=new QPushButton(tr("平移校正"));
    parallelBtn->setFont(ft);
    connect(parallelBtn, SIGNAL(clicked()), this, SLOT(OnParallelBtnClicked()));
    rotateBtn=new QPushButton(tr("旋转校正"));
    rotateBtn->setFont(ft);
    connect(rotateBtn, SIGNAL(clicked()), this, SLOT(OnRotateBtnClicked()));
    zOffsetBtn=new QPushButton(tr("Z偏移校正"));
    zOffsetBtn->setFont(ft);
    connect(zOffsetBtn, SIGNAL(clicked()), this, SLOT(OnZOffsetBtnClicked()));

    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(parallelBtn);
    layout->addWidget(rotateBtn);
    //layout->addWidget(zOffsetBtn);
    layout->addWidget(cancelBtn);
    setLayout(layout);
    setWindowTitle(tr("校正设置"));
    resize(200,300);
}

void CorrectDlg::OnParallelBtnClicked()
{
    correctType=1;
    done(QDialog::Accepted);
}

void CorrectDlg::OnRotateBtnClicked()
{
    correctType=2;
    done(QDialog::Accepted);
}

void CorrectDlg::OnZOffsetBtnClicked()
{
    correctType=3;
    done(QDialog::Accepted);
}

void CorrectDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

////////////////////////////////////////////平移校正类//////////////////////////////////////////////
ParalleCorrectDlg::ParalleCorrectDlg(QWidget *parent) :
    QDialog(parent)
{
    mod=0;
    initDialog();
}
void ParalleCorrectDlg::initDialog()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    /*QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);*/
    QFont ft;
    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *stepLabel=new QLabel(tr("指定需要校正的步骤"));
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
    QPushButton *selectAllButton=new QPushButton(tr("全部"));
    selectAllButton->setFont(ft);
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(allSelectBtnClicked()));
    QLabel *promptLabel=new QLabel(tr("提示：在按下“校正”按钮之前,请确认已经完成如下操作："));
    promptLabel->setFont(ft);
    QLabel *promptLabel1=new QLabel(tr("1. 选择参考点,按定位将焊头移动到参考点处."));
    promptLabel1->setFont(ft);
    QLabel *promptLabel2=new QLabel(tr("2. 选择参考点,微调参考点坐标至理想位置."));
    promptLabel2->setFont(ft);

    QLabel *modLabel=new QLabel(tr("平移方式"));
    modLabel->setFont(ft);
    modComboBox=new QComboBox;
    modComboBox->setFont(ft);
    modComboBox->addItem(tr("移动平移"));
    modComboBox->addItem(tr("复制平移"));
    modComboBox->setCurrentIndex(0);
    QHBoxLayout *modLayout=new QHBoxLayout;
    modLayout->addWidget(modLabel);
    modLayout->addWidget(modComboBox);

    QPushButton * okBtn=new QPushButton(tr("校正"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    QHBoxLayout *commLayout=new QHBoxLayout;
    commLayout->addWidget(fromLabel);
    commLayout->addWidget(fromEdit);
    commLayout->addWidget(toLabel);
    commLayout->addWidget(toEdit);
    commLayout->addWidget(buLabel);
    commLayout->addStretch();
    commLayout->addWidget(selectAllButton);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(stepLabel);
    layout->addLayout(commLayout);
    layout->addLayout(modLayout);
    layout->addSpacing(150);
    layout->addWidget(promptLabel);
    layout->addWidget(promptLabel1);
    layout->addWidget(promptLabel2);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("平移校正"));
    resize(450,310);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    fromEdit->context=context;
    fromEdit->myPos=QPoint(23,40);
    fromEdit->parentHeight=geometry().height();
    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void ParalleCorrectDlg::OnOkBtnClicked()
{
    fromStep=fromEdit->text().trimmed().toInt();
    toStep=toEdit->text().trimmed().toInt();
    mod=modComboBox->currentIndex();
    done(QDialog::Accepted);
}

void ParalleCorrectDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void ParalleCorrectDlg::allSelectBtnClicked()
{
    if(weldProject)
    {
        int commNum=weldProject->listCommand.size()-1;
        fromEdit->setText(QString("0"));
        toEdit->setText(QString("%1").arg(commNum));
    }
}

void ParalleCorrectDlg::loadMyLineEdit()      //加载自定义文本框
{
    QRect rect=fromEdit->geometry();
    fromEdit->myPos=QPoint(rect.left()+250,rect.top());
    fromEdit->parentHeight=geometry().height();
    toEdit->context=context;
    rect=toEdit->geometry();
    toEdit->myPos=QPoint(rect.left()+200,rect.top());
    toEdit->parentHeight=geometry().height();
    fromEdit->setFocus(Qt::OtherFocusReason);
}

void ParalleCorrectDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,250));
}
void ParalleCorrectDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}
void ParalleCorrectDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

////////////////////////////////////////////平移+旋转校正类//////////////////////////////////////////////
RotateCorrectDlg::RotateCorrectDlg(QWidget *parent) :
    QDialog(parent)
{
    //for(int i=0;i<8;i++) newFirstPos[i]=0;
    //for(int i=0;i<8;i++) newEndPos[i]=0;
    for(int i=0;i<8;i++)
    {
        oldFirstPointPos[i]=0;
        newFirstPointPos[i]=0;
        oldEndPointPos[i]=0;
        newEndPointPos[i]=0;
        currPointPos[i]=0;
    }
    firstPointNum=-1;
    endPointNum=-1;
    Initangle=0;
    Turning_Radius=0;
}
void RotateCorrectDlg::initDialog()
{
    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    /*QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);*/
    QFont ft;
    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    ft.setItalic(false);

    createR0PointPosBox();
    createPointPosBox();

    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(255,255,255));
    QLabel *stepLabel=new QLabel(tr("2.指定需要校正的焊点的序号范围"));
    stepLabel->setFont(ft);
    stepLabel->setPalette(palette);

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
    QPushButton *selectAllButton=new QPushButton(tr("全部"));
    selectAllButton->setFont(ft);
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(allSelectBtnClicked()));

    QPushButton * firstBtn=new QPushButton(tr("定位到第一个参考点"));
    firstBtn->setFont(ft);
    connect(firstBtn, SIGNAL(clicked()), this, SLOT(OnFirstBtnClicked()));

    QPushButton * endBtn=new QPushButton(tr("定位到第二个参考点"));
    endBtn->setFont(ft);
    connect(endBtn, SIGNAL(clicked()), this, SLOT(OnEndBtnClicked()));

    QPushButton * okBtn=new QPushButton(tr("校正"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    connect(this, SIGNAL(goFixPos(int)), robot, SLOT(goFixPosSlot(int)));    //定位连接

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    QHBoxLayout *commLayout=new QHBoxLayout;
    commLayout->addWidget(fromLabel);
    commLayout->addWidget(fromEdit);
    commLayout->addWidget(toLabel);
    commLayout->addWidget(toEdit);
    commLayout->addWidget(buLabel);
    commLayout->addStretch();
    commLayout->addWidget(selectAllButton);

    QHBoxLayout *btnLayout=new QHBoxLayout;
    btnLayout->addWidget(firstBtn);
    btnLayout->addWidget(endBtn);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addLayout(posR0Layout);
    layout->addStretch();
    layout->addWidget(stepLabel);
    layout->addLayout(commLayout);
    layout->addStretch();
    layout->addLayout(posLayout);
    layout->addLayout(btnLayout);
    layout->addStretch();
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("平移+旋转校正"));
    resize(450,510);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    fromEdit->context=context;
    fromEdit->myPos=QPoint(23,40);
    fromEdit->parentHeight=geometry().height();
    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void RotateCorrectDlg::OnOkBtnClicked()
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

    QString errStr;
    if(initangleEdit->text().trimmed().isEmpty() || radiusEdit->text().trimmed().isEmpty())
    {
        for(int i=0;i<4;i++)
        {
            if(r0PosEdit[i]->text().trimmed().isEmpty())
            {
                errStr=tr("请您更新R=0时的初始化点的坐标！");
                break;
            }
            r0PointPos[i]=r0PosEdit[i]->text().trimmed().toDouble();
            if(r180PosEdit[i]->text().trimmed().isEmpty())
            {
                errStr=tr("请您更新R=180时的初始化点的坐标！");
                break;
            }
            r180PointPos[i]=r180PosEdit[i]->text().trimmed().toDouble();
        }
        if(!errStr.isEmpty())
        {
            msgBox.setText(errStr);
            msgBox.exec();
            return;
        }
    }

    Initangle=initangleEdit->text().trimmed().toDouble();
    Turning_Radius=radiusEdit->text().trimmed().toDouble();
    if(!((Initangle>0) && (Turning_Radius>0)))
    {
        msgBox.setText(tr("请您点击“计算”按钮，计算烙铁的初始角度和旋转半径！"));
        msgBox.exec();
        return;
    }

    fromStep=fromEdit->text().trimmed().toInt();
    toStep=toEdit->text().trimmed().toInt();

    if((fromStep<0) || !(toStep>0))
    {
        msgBox.setText(tr("请您填写需要校正焊点的正确的序号范围！"));
        msgBox.exec();
        return;
    }

    errStr.clear();
    for(int i=0;i<4;i++)
    {
        newFirstPointPos[i]=startPosEdit[i]->text().trimmed().toDouble();
        if(startPosEdit[i]->text().trimmed().isEmpty())
        {
            errStr=tr("请您更新第一个参考点的坐标！");
            break;
        }
        newEndPointPos[i]=endPosEdit[i]->text().trimmed().toDouble();
        if(endPosEdit[i]->text().trimmed().isEmpty())
        {
            errStr=tr("请您更新第二个参考点的坐标！");
            break;
        }
    }
    if(!errStr.isEmpty())
    {
        msgBox.setText(errStr);
        msgBox.exec();
        return;
    }

    if(firstPointNum<0)
    {
        msgBox.setText(tr("请先选择并定位到第一个参考点."));
        msgBox.exec();
        return;
    }

    if(endPointNum<0)
    {
        msgBox.setText(tr("请先选择并定位到第二个参考点."));
        msgBox.exec();
        return;
    }

    for(int i=0;i<8;i++)
    {
        oldFirstPointPos[i]=weldProject->pointsAxisPos[firstPointNum][i];
        oldEndPointPos[i]=weldProject->pointsAxisPos[endPointNum][i];
    }
/*
    robot->ML_F_Get_TRAD_IANG(r0PointPos[0],r0PointPos[1],r0PointPos[2],r0PointPos[3],
            r180PointPos[0],r180PointPos[1],r180PointPos[2],r180PointPos[3],
            &Turning_Radius,&Initangle);
*/


    for (int i = fromStep; i <= toStep; i++)
    {
        if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("空点"))) || (weldProject->listCommand[i]==QString(tr("拖焊")))
                 || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
        {
            F64 x2=0.0,y2=0.0,z2=0.0,r2=0.0;
            F64 x=weldProject->pointsAxisPos[i][0];
            F64 y=weldProject->pointsAxisPos[i][1];
            F64 z=weldProject->pointsAxisPos[i][2];
            F64 r=weldProject->pointsAxisPos[i][3];
            robot->ML_F_Get_x2y2z2r2(oldFirstPointPos[0],oldFirstPointPos[1],oldFirstPointPos[2],oldFirstPointPos[3],
                    oldEndPointPos[0],oldEndPointPos[1],oldEndPointPos[2],oldEndPointPos[3],
                    x,y,z,r,Turning_Radius,Initangle,
                    newFirstPointPos[0],newFirstPointPos[1],newFirstPointPos[2],newFirstPointPos[3],
                    newEndPointPos[0],newEndPointPos[1],newEndPointPos[2],newEndPointPos[3],
                    &x2,&y2,&z2,&r2);
            weldProject->pointsAxisPos[i][0] = x2;
            weldProject->pointsAxisPos[i][1] = y2;
            weldProject->pointsAxisPos[i][2] = z2;
            weldProject->pointsAxisPos[i][3] = r2;

            if ((weldProject->listCommand[i]==QString(tr("拖焊")))
                    || (weldProject->listCommand[i]==QString(tr("多拖1")))
                    || (weldProject->listCommand[i]==QString(tr("多拖2")))
                    || (weldProject->listCommand[i]==QString(tr("多拖3"))))
            {
                F64 x=weldProject->pointsAxisPos[i][4];
                F64 y=weldProject->pointsAxisPos[i][5];
                F64 z=weldProject->pointsAxisPos[i][6];
                F64 r=weldProject->pointsAxisPos[i][7];
                robot->ML_F_Get_x2y2z2r2(oldFirstPointPos[0],oldFirstPointPos[1],oldFirstPointPos[2],oldFirstPointPos[3],
                        oldEndPointPos[0],oldEndPointPos[1],oldEndPointPos[2],oldEndPointPos[3],
                        x,y,z,r,Turning_Radius,Initangle,
                        newFirstPointPos[0],newFirstPointPos[1],newFirstPointPos[2],newFirstPointPos[3],
                        newEndPointPos[0],newEndPointPos[1],newEndPointPos[2],newEndPointPos[3],
                        &x2,&y2,&z2,&r2);
                weldProject->pointsAxisPos[i][4] = x2;
                weldProject->pointsAxisPos[i][5] = y2;
                weldProject->pointsAxisPos[i][6] = z2;
                weldProject->pointsAxisPos[i][7] = r2;
            }
        }
    }
    weldProject->saveProject();    //保存当前项目

    done(QDialog::Accepted);
}

void RotateCorrectDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void RotateCorrectDlg::allSelectBtnClicked()
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

    if(weldProject==0)
    {
        msgBox.setText(tr("系统找不到打开的项目."));
        msgBox.exec();
        return;
    }

    int count=weldProject->listCommand.size();
    for(int i=0;i<count;i++)
    {
        if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("拖焊"))) || (weldProject->listCommand[i]==QString(tr("空点")))
                 || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
        {
            firstPointNum=i;
            break;
        }
    }
    for(int i=0;i<count;i++)
    {
        if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("拖焊"))) || (weldProject->listCommand[i]==QString(tr("空点")))
                 || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
        {
            endPointNum=i;
        }
    }

    if(firstPointNum<0)
    {
        msgBox.setText(tr("系统找不到需要校正的点."));
        msgBox.exec();
        return;
    }
    fromEdit->setText(QString("%1").arg(firstPointNum));
    toEdit->setText(QString("%1").arg(endPointNum));
    fromStep=firstPointNum;
    toStep=endPointNum;
}

void RotateCorrectDlg::loadMyLineEdit()      //加载自定义文本框
{
    QRect rect=fromEdit->geometry();
    fromEdit->myPos=QPoint(rect.left()+250,rect.top());
    fromEdit->parentHeight=geometry().height();
    toEdit->context=context;
    rect=toEdit->geometry();
    toEdit->myPos=QPoint(rect.left()+200,rect.top());
    toEdit->parentHeight=geometry().height();
    for(int i=0;i<4;i++)
    {
        r0PosEdit[i]->context=context;
        QRect rect=r0PosEdit[i]->geometry();
        r0PosEdit[i]->myPos=QPoint(rect.left()+200,rect.top()+50);
        r0PosEdit[i]->parentHeight=geometry().height();

        r180PosEdit[i]->context=context;
        rect=r180PosEdit[i]->geometry();
        r180PosEdit[i]->myPos=QPoint(rect.left()+200,rect.top()+50);
        r180PosEdit[i]->parentHeight=geometry().height();

        startPosEdit[i]->context=context;
        rect=startPosEdit[i]->geometry();
        startPosEdit[i]->myPos=QPoint(rect.left()+200,rect.top()+50);
        startPosEdit[i]->parentHeight=geometry().height();

        endPosEdit[i]->context=context;
        rect=endPosEdit[i]->geometry();
        endPosEdit[i]->myPos=QPoint(rect.left()+200,rect.top()+50);
        endPosEdit[i]->parentHeight=geometry().height();
    }

    fromEdit->setFocus(Qt::OtherFocusReason);
}

void RotateCorrectDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,250));
}
void RotateCorrectDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}
void RotateCorrectDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

void RotateCorrectDlg::createR0PointPosBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT+3);
    ft.setItalic(false);
    QLabel *posLabel1[4]={new QLabel(tr("X:")),new QLabel(tr("Y:")),new QLabel(tr("Z:")),new QLabel(tr("R:"))};
    QLabel *posLabel2[4]={new QLabel(tr("X:")),new QLabel(tr("Y:")),new QLabel(tr("Z:")),new QLabel(tr("R:"))};
    for(int i=0;i<4;i++)
    {
        posLabel1[i]->setFont(ft);
        posLabel2[i]->setFont(ft);
    }
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    for(int i=0;i<4;i++)
    {
        r0PosEdit.append(new MyLineEdit);
        r0PosEdit[i]->setFont(ft);
        r180PosEdit.append(new MyLineEdit);
        r180PosEdit[i]->setFont(ft);
    }
    radiusEdit=new QLineEdit;
    radiusEdit->setReadOnly(true);
    radiusEdit->setFont(ft);
    QLabel *radiusLabel=new QLabel(tr("烙铁旋转半径"));
    radiusLabel->setFont(ft);
    initangleEdit=new QLineEdit;
    initangleEdit->setReadOnly(true);
    initangleEdit->setFont(ft);
    QLabel *initangleLabel=new QLabel(tr("烙铁初始角度"));
    initangleLabel->setFont(ft);

    ft.setPointSize(DLG_LABEL_BUTTON_FONT+2);
    QPushButton * updatePosBtn1=new QPushButton(tr("更新坐标"));
    updatePosBtn1->setFont(ft);
    //updatePosBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(updatePosBtn1, SIGNAL(clicked()), this, SLOT(OnUpdateR0PosBtnClicked()));
    QPushButton * updatePosBtn2=new QPushButton(tr("更新坐标"));
    updatePosBtn2->setFont(ft);
    connect(updatePosBtn2, SIGNAL(clicked()), this, SLOT(OnUpdateR180PosBtnClicked()));
    QPushButton * calBtn=new QPushButton(tr("计算"));
    calBtn->setFont(ft);
    connect(calBtn, SIGNAL(clicked()), this, SLOT(OncalculateBtnClicked()));

    palette.setColor(QPalette::WindowText, QColor(255,255,255));
    QLabel *r0headerLabel=new QLabel(tr("1.选定一点为初始化点，并确定以下两种情况的坐标：\n  (如果参数已经确定可以跳过这一步)"));
    r0headerLabel->setFont(ft);
    r0headerLabel->setPalette(palette);

    QLabel *sLabel=new QLabel(tr("R=0时初始化点坐标"));
    sLabel->setFont(ft);
    QLabel *eLabel=new QLabel(tr("R=180时初始化点坐标"));
    eLabel->setFont(ft);

    QHBoxLayout *ironLayout=new QHBoxLayout;
    ironLayout->addWidget(radiusLabel);
    ironLayout->addWidget(radiusEdit);
    ironLayout->addStretch();
    ironLayout->addWidget(initangleLabel);
    ironLayout->addWidget(initangleEdit);
    ironLayout->addStretch();
    ironLayout->addWidget(calBtn);

    QHBoxLayout *posLayout1=new QHBoxLayout;
    posLayout1->setMargin(0);
    for(int i=0;i<4;i++)
    {
        posLayout1->addWidget(posLabel1[i]);
        posLayout1->addWidget(r0PosEdit[i]);
    }
    posLayout1->addWidget(updatePosBtn1);

    QHBoxLayout *posLayout2=new QHBoxLayout;
    posLayout2->setMargin(0);
    for(int i=0;i<4;i++)
    {
        posLayout2->addWidget(posLabel2[i]);
        posLayout2->addWidget(r180PosEdit[i]);
    }
    posLayout2->addWidget(updatePosBtn2);

    posR0Layout=new QVBoxLayout;
    posR0Layout->addWidget(r0headerLabel);
    posR0Layout->addWidget(sLabel);
    posR0Layout->addLayout(posLayout1);
    posR0Layout->addWidget(eLabel);
    posR0Layout->addLayout(posLayout2);
    posR0Layout->addLayout(ironLayout);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
       //msgBox.setText(tr("打开数据库失败."));
       //msgBox.exec();
       return;
    }
    else
    {
        double Initangle1;               //烙铁初始角度（弧度）
        double Turning_Radius1;          //烙铁旋转半径（mm）
        if(db.ReadCorrectParaFromDb(weldProject->getProjName(),&Initangle1,&Turning_Radius1))
        {
            initangleEdit->setText(QString("%1").arg(Initangle1,0,'f',2));
            radiusEdit->setText(QString("%1").arg(Turning_Radius1,0,'f',2));
        }
        db.closeDB();
    }
}

void RotateCorrectDlg::OncalculateBtnClicked()
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

    initangleEdit->clear();
    radiusEdit->clear();
    QString errStr;
    for(int i=0;i<4;i++)
    {
        if(r0PosEdit[i]->text().trimmed().isEmpty())
        {
            errStr=tr("请您更新R=0时的初始化点的坐标！");
            break;
        }
        r0PointPos[i]=r0PosEdit[i]->text().trimmed().toDouble();
        if(r180PosEdit[i]->text().trimmed().isEmpty())
        {
            errStr=tr("请您更新R=180时的初始化点的坐标！");
            break;
        }
        r180PointPos[i]=r180PosEdit[i]->text().trimmed().toDouble();
    }
    if(!errStr.isEmpty())
    {
        msgBox.setText(errStr);
        msgBox.exec();
        return;
    }

    robot->ML_F_Get_TRAD_IANG(r0PointPos[0],r0PointPos[1],r0PointPos[2],r0PointPos[3],
            r180PointPos[0],r180PointPos[1],r180PointPos[2],r180PointPos[3],
            &Turning_Radius,&Initangle);
    radiusEdit->setText(QString("%1").arg(Turning_Radius,0,'f',2));
    initangleEdit->setText(QString("%1").arg(Initangle,0,'f',2));
    SqlDB db;
    if(!db.openDB("robot.db"))
    {
       msgBox.setText(tr("打开数据库失败."));
       msgBox.exec();
       return;
    }
    else
    {
        db.SaveCorrectParaToDb(weldProject->getProjName(),Initangle,Turning_Radius);
        db.closeDB();
    }
}

void RotateCorrectDlg::createPointPosBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT+3);
    ft.setItalic(false);
    QLabel *posLabel1[4]={new QLabel(tr("X:")),new QLabel(tr("Y:")),new QLabel(tr("Z:")),new QLabel(tr("R:"))};
    QLabel *posLabel2[4]={new QLabel(tr("X:")),new QLabel(tr("Y:")),new QLabel(tr("Z:")),new QLabel(tr("R:"))};
    for(int i=0;i<4;i++)
    {
        posLabel1[i]->setFont(ft);
        posLabel2[i]->setFont(ft);
    }
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    for(int i=0;i<4;i++)
    {
        startPosEdit.append(new MyLineEdit);
        endPosEdit.append(new MyLineEdit);
    }
    for(int i=0;i<4;i++)
    {
        startPosEdit[i]->setFont(ft);
        //startPosEdit[i]->setMaximumWidth(100);
        endPosEdit[i]->setFont(ft);
        //endPosEdit[i]->setMaximumWidth(100);
    }

    palette.setColor(QPalette::WindowText, QColor(255,255,255));
    ft.setPointSize(DLG_LABEL_BUTTON_FONT+2);
    QLabel *pointLabel=new QLabel(tr("3.选定两个校正参考点，定位后调整到正确位置，并更新坐标："));
    pointLabel->setFont(ft);
    pointLabel->setPalette(palette);
    QLabel *pointBLabel=new QLabel(tr("（必须定位到所选择的点焊或拖焊点，拖焊以首点为准。）"));
    pointBLabel->setFont(ft);
    pointBLabel->setPalette(palette);

    QPushButton * updatePosBtn1=new QPushButton(tr("更新坐标"));
    updatePosBtn1->setFont(ft);
    //updatePosBtn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(updatePosBtn1, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtn1Clicked()));
    QPushButton * updatePosBtn2=new QPushButton(tr("更新坐标"));
    updatePosBtn2->setFont(ft);
    connect(updatePosBtn2, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtn2Clicked()));

    QLabel *sLabel=new QLabel(tr("第一个参考点坐标"));
    sLabel->setFont(ft);
    QLabel *eLabel=new QLabel(tr("第二个参考点坐标"));
    eLabel->setFont(ft);

    QHBoxLayout *posLayout1=new QHBoxLayout;
    posLayout1->setMargin(0);
    for(int i=0;i<4;i++)
    {
        posLayout1->addWidget(posLabel1[i]);
        posLayout1->addWidget(startPosEdit[i]);
    }
    posLayout1->addWidget(updatePosBtn1);

    QHBoxLayout *posLayout2=new QHBoxLayout;
    posLayout2->setMargin(0);
    for(int i=0;i<4;i++)
    {
        posLayout2->addWidget(posLabel2[i]);
        posLayout2->addWidget(endPosEdit[i]);
    }
    posLayout2->addWidget(updatePosBtn2);

    posLayout=new QVBoxLayout;
    posLayout->addWidget(pointLabel);
    posLayout->addWidget(pointBLabel);
    posLayout->addWidget(sLabel);
    posLayout->addLayout(posLayout1);
    posLayout->addWidget(eLabel);
    posLayout->addLayout(posLayout2);
}

void RotateCorrectDlg::OnUpdateR0PosBtnClicked()
{
    for(int i=0;i<4;i++)
        r0PosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    if((((g_MacType==2) || (g_MacType==4)) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        r0PosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
}

void RotateCorrectDlg::OnUpdateR180PosBtnClicked()
{
    for(int i=0;i<4;i++)
        r180PosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        r180PosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
}

void RotateCorrectDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
}

void RotateCorrectDlg::OnUpdatePosBtn1Clicked()
{
    for(int i=0;i<4;i++)
        startPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    if((((g_MacType==2) || (g_MacType==4)) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        startPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
}

void RotateCorrectDlg::OnUpdatePosBtn2Clicked()
{
    for(int i=0;i<4;i++)
        endPosEdit[i]->setText(QString("%1").arg(currPointPos[i],0,'f',2));
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        endPosEdit[1]->setText(QString("%1").arg(currPointPos[7],0,'f',2));
}

void RotateCorrectDlg::OnFirstBtnClicked()
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

    fromStep=fromEdit->text().trimmed().toInt();
    toStep=toEdit->text().trimmed().toInt();
    firstPointNum=-1;
    int i=weldProject->currCommNum;
    if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("拖焊")))
             || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
    {
        firstPointNum=i;
    }

    if(firstPointNum<0)
    {
        msgBox.setText(tr("系统找不到您所选的参考点（必须是点焊或拖焊点）."));
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
    double x=weldProject->pointsAxisPos[firstPointNum][0];
    double y=weldProject->pointsAxisPos[firstPointNum][1];
    double z=weldProject->pointsAxisPos[firstPointNum][2];
    double r=weldProject->pointsAxisPos[firstPointNum][3];
    double zh=weldProject->Zh[firstPointNum];
    QString paraName;
    paraName=weldProject->listTechnics[firstPointNum];
    if(weldProject->listCommand[firstPointNum]==QString(tr("点焊")))
    {
        if(db.ReadPointWeldFromDb(paraName,g_PointWeldPara))
        {
            g_PointWeldPara.x=x;
            g_PointWeldPara.y=y;
            g_PointWeldPara.z=z;
            g_PointWeldPara.r=r;
            g_PointWeldPara.ZH=zh;
            emit goFixPos(1);
        }
        for(int i=0;i<8;i++)
            oldFirstPointPos[i]=weldProject->pointsAxisPos[firstPointNum][i];
    }
    else if(weldProject->listCommand[firstPointNum]==QString(tr("拖焊")) || weldProject->listCommand[firstPointNum]==QString(tr("多拖1"))
             || weldProject->listCommand[firstPointNum]==QString(tr("多拖2")) || weldProject->listCommand[firstPointNum]==QString(tr("多拖3")))
    {
        if(db.ReadLineWeldFromDb(paraName,g_LineWeldPara))
        {
            g_LineWeldPara.x0=x;
            g_LineWeldPara.y0=y;
            g_LineWeldPara.z0=z;
            g_LineWeldPara.r0=r;
            g_LineWeldPara.ZH=zh;
            emit goFixPos(2);
        }
        for(int i=0;i<8;i++)
            oldFirstPointPos[i]=weldProject->pointsAxisPos[firstPointNum][i];
    }
    db.closeDB();
}

void RotateCorrectDlg::OnEndBtnClicked()
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

    fromStep=fromEdit->text().trimmed().toInt();
    toStep=toEdit->text().trimmed().toInt();
    endPointNum=-1;
    int i=weldProject->currCommNum;
    if((weldProject->listCommand[i]==QString(tr("点焊"))) || (weldProject->listCommand[i]==QString(tr("拖焊")))
             || (weldProject->listCommand[i]==QString(tr("多拖1"))) || (weldProject->listCommand[i]==QString(tr("多拖2"))) || (weldProject->listCommand[i]==QString(tr("多拖3"))))
    {
        endPointNum=i;
    }

    if(endPointNum<0)
    {
        msgBox.setText(tr("系统找不到您所选的参考点（必须是点焊或拖焊点）."));
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
    double x=weldProject->pointsAxisPos[endPointNum][0];
    double y=weldProject->pointsAxisPos[endPointNum][1];
    double z=weldProject->pointsAxisPos[endPointNum][2];
    double r=weldProject->pointsAxisPos[endPointNum][3];
    double zh=weldProject->Zh[endPointNum];
    QString paraName;
    paraName=weldProject->listTechnics[endPointNum];
    if(weldProject->listCommand[endPointNum]==QString(tr("点焊")))
    {
        if(db.ReadPointWeldFromDb(paraName,g_PointWeldPara))
        {
            g_PointWeldPara.x=x;
            g_PointWeldPara.y=y;
            g_PointWeldPara.z=z;
            g_PointWeldPara.r=r;
            g_PointWeldPara.ZH=zh;
            emit goFixPos(1);
        }
        for(int i=0;i<8;i++)
            oldEndPointPos[i]=weldProject->pointsAxisPos[endPointNum][i];
    }
    else if(weldProject->listCommand[endPointNum]==QString(tr("拖焊")) || weldProject->listCommand[endPointNum]==QString(tr("多拖1"))
            || weldProject->listCommand[endPointNum]==QString(tr("多拖2")) || weldProject->listCommand[endPointNum]==QString(tr("多拖3")))
    {
        if(db.ReadLineWeldFromDb(paraName,g_LineWeldPara))
        {
            g_LineWeldPara.x0=x;
            g_LineWeldPara.y0=y;
            g_LineWeldPara.z0=z;
            g_LineWeldPara.r0=r;
            g_LineWeldPara.ZH=zh;
            emit goFixPos(2);
        }
        for(int i=0;i<8;i++)
            oldEndPointPos[i]=weldProject->pointsAxisPos[endPointNum][i];
    }
    db.closeDB();
}

////////////////////////////////////////////Z偏移校正类//////////////////////////////////////////////
ZOffsetDlg::ZOffsetDlg(QWidget *parent) :
    QDialog(parent)
{
    initDialog();
}
void ZOffsetDlg::initDialog()
{
    mod=1;
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    /*QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);*/
    QFont ft;
    ft.setPointSize(WELD_DLG_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *stepLabel=new QLabel(tr("指定需要校正的步骤"));
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
    QPushButton *selectAllButton=new QPushButton(tr("全部"));
    selectAllButton->setFont(ft);
    connect(selectAllButton, SIGNAL(clicked()), this, SLOT(allSelectBtnClicked()));
/*    QLabel *promptLabel=new QLabel(tr("提示：在按下“校正”按钮之前,请确认已经完成如下操作："));
    promptLabel->setFont(ft);
    QLabel *promptLabel1=new QLabel(tr("1. 选择参考点,按定位将焊头移动到参考点处."));
    promptLabel1->setFont(ft);
    QLabel *promptLabel2=new QLabel(tr("2. 选择参考点,微调参考点坐标至理想位置."));
    promptLabel2->setFont(ft);*/

    zAddBtn=new QRadioButton(tr("Z坐标+"));
    zAddBtn->setFont(ft);
    connect(zAddBtn, SIGNAL(clicked()), this, SLOT(OnZAddBtnClicked()));

    zReduceBtn=new QRadioButton(tr("Z坐标-"));
    zReduceBtn->setFont(ft);
    connect(zReduceBtn, SIGNAL(clicked()), this, SLOT(OnZReduceBtnClicked()));

    zAddBtn->setChecked(true);

    QLabel *zLabel=new QLabel(tr("Z偏移长度(mm)"));
    zLabel->setFont(ft);
    zLabel->setAlignment(Qt::AlignLeft);
    lengthEdit=new MyLineEdit;
    lengthEdit->setFont(ft);
    lengthEdit->setAlignment(Qt::AlignHCenter);

    QPushButton * okBtn=new QPushButton(tr("校正"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QVBoxLayout* zBtnLayout=new QVBoxLayout;
    zBtnLayout->addWidget(zAddBtn);
    zBtnLayout->addWidget(zReduceBtn);
    QVBoxLayout* zEditLayout=new QVBoxLayout;
    zEditLayout->addWidget(zLabel);
    zEditLayout->addWidget(lengthEdit);
    QHBoxLayout *zLayout=new QHBoxLayout;
    zLayout->addStretch();
    zLayout->addLayout(zBtnLayout);
    zLayout->addStretch();
    zLayout->addLayout(zEditLayout);
    zLayout->addStretch();

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    QHBoxLayout *commLayout=new QHBoxLayout;
    commLayout->addWidget(fromLabel);
    commLayout->addWidget(fromEdit);
    commLayout->addWidget(toLabel);
    commLayout->addWidget(toEdit);
    commLayout->addWidget(buLabel);
    commLayout->addStretch();
    commLayout->addWidget(selectAllButton);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(stepLabel);
    layout->addLayout(commLayout);
    layout->addStretch();
    layout->addLayout(zLayout);
    layout->addStretch();
    //layout->addWidget(promptLabel);
    //layout->addWidget(promptLabel1);
    //layout->addWidget(promptLabel2);
    layout->addLayout(buttonLayout);

    setLayout(layout);
    setWindowTitle(tr("Z校正"));
    resize(450,310);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    toEdit->context=context;
    lengthEdit->context=context;
    fromEdit->context=context;
    fromEdit->myPos=QPoint(23,40);
    fromEdit->parentHeight=geometry().height();
    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void ZOffsetDlg::OnZAddBtnClicked() //Z+
{
    mod=1;
}

void ZOffsetDlg::OnZReduceBtnClicked()  //Z-
{
    mod=2;
}

void ZOffsetDlg::OnOkBtnClicked()
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

    QString ss=toEdit->text();
    if(fromEdit->text().isEmpty() || toEdit->text().isEmpty())
    {
        msgBox.setText(tr("请填写校正步骤."));
        msgBox.exec();
        return;
    }
    ss=lengthEdit->text();
    if(lengthEdit->text().isEmpty())
    {
        msgBox.setText(tr("请填写Z偏移长度."));
        msgBox.exec();
        return;
    }

    fromStep=fromEdit->text().trimmed().toInt();
    toStep=toEdit->text().trimmed().toInt();
    zLength=lengthEdit->text().trimmed().toInt();

    done(QDialog::Accepted);
}

void ZOffsetDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void ZOffsetDlg::allSelectBtnClicked()
{
    if(weldProject)
    {
        int commNum=weldProject->listCommand.size()-1;
        fromEdit->setText(QString("0"));
        toEdit->setText(QString("%1").arg(commNum));
    }
}

void ZOffsetDlg::loadMyLineEdit()      //加载自定义文本框
{
    QRect rect=fromEdit->geometry();
    fromEdit->myPos=QPoint(rect.left()+250,rect.top());
    fromEdit->parentHeight=geometry().height();

    rect=toEdit->geometry();
    toEdit->myPos=QPoint(rect.left()+200,rect.top());
    toEdit->parentHeight=geometry().height();
    fromEdit->setFocus(Qt::OtherFocusReason);

    lengthEdit->context=context;
    rect=lengthEdit->geometry();
    lengthEdit->myPos=QPoint(410,300);
    lengthEdit->parentHeight=geometry().height();
}

void ZOffsetDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,250));
}
void ZOffsetDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}
void ZOffsetDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}
