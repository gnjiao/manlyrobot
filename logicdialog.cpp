#include "logicdialog.h"
#include "global.h"
#include "myrwxml.h"

//////////////////////////////////////////////////////////////////////
/// 延时对话框
///
DelayDlg::DelayDlg(QWidget *parent) :
    QDialog(parent)
{
    time=0;
}

void DelayDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *timeLabel=new QLabel(tr("请输入延时时间（秒）"));
    timeLabel->setFont(ft);
    delayTimeEdit=new QLineEdit;
    delayTimeEdit->setFont(ft);

    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(timeLabel);
    layout->addWidget(delayTimeEdit);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    setWindowTitle(tr("延时指令设置"));
    if(time>0)
        delayTimeEdit->setText(QString("%1").arg(time));

    QProcess p(0);
    p.start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
    p.waitForStarted();
    p.waitForFinished();
}

void DelayDlg::OnOkBtnClicked()
{
    if(delayTimeEdit->text().isEmpty())
        done(QDialog::Rejected);
    QString str=delayTimeEdit->text();
    time=delayTimeEdit->text().trimmed().toDouble();
    if(time>0)
        done(QDialog::Accepted);
    else
        done(QDialog::Rejected);
}

void DelayDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

///////////////////////////////////////////////////////////////////////////
/// 跳转对话框
///
GotoDlg::GotoDlg(QWidget *parent) :
    QDialog(parent)
{
    number=-1;
}

void GotoDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(OPERATE_PROJECT_FONT_COLOR));
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *numLabel=new QLabel(tr("请输入要跳转到的指令序号"));
    numLabel->setFont(ft);
    commandNumEdit=new QLineEdit;
    commandNumEdit->setFont(ft);

    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelBtn);

    QVBoxLayout *layout=new QVBoxLayout;
    layout->addWidget(numLabel);
    layout->addWidget(commandNumEdit);
    layout->addLayout(buttonLayout);
    setLayout(layout);
    setWindowTitle(tr("跳转指令设置"));
    if(number>=0)
        commandNumEdit->setText(QString("%1").arg(number));
/*
    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(200,200);
    //loadDlgFinished();     //触发信号，加载自定义文本框
    //atspeedNumEdit->myPos=QPoint(23,50);
    //atspeedNumEdit->parentHeight=geometry().height();
    QRect rect=geometry();
    QPoint topLeft=rect.topLeft();
    topLeft+=QPoint(0,rect.height());
    commandNumEdit->myPos=topLeft;
    commandNumEdit->parentHeight=rect.height();
    commandNumEdit->context=context;
    */

    QProcess p(0);
    p.start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
    p.waitForStarted();
    p.waitForFinished();
}

void GotoDlg::OnOkBtnClicked()
{
    if(commandNumEdit->text().isEmpty())
        done(QDialog::Rejected);
    number=commandNumEdit->text().trimmed().toInt();
    if(number>=0)
        done(QDialog::Accepted);
    else
        done(QDialog::Rejected);
}

void GotoDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

///////////////////////////////////////////////////////////////////////////
/// 条件对话框
///
ConditionDlg::ConditionDlg(QWidget *parent) :
    QDialog(parent)
{
}

void ConditionDlg::initDialog()
{
}

void ConditionDlg::OnOkBtnClicked()
{
    done(QDialog::Accepted);
}

void ConditionDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

///////////////////////////////////////////////////////////////////////////
/// 空点对话框
///
BlankPointDlg::BlankPointDlg(QWidget *parent) :
    QDialog(parent)
{
    atspeed=0;
    zh=0;
    delayTime=0;
    for(int i=0;i<4;i++)
        pointPos[i]=0;
}

void BlankPointDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *posLabel[4]={new QLabel(tr("X:")),new QLabel(tr("Y:")),new QLabel(tr("Z:")),new QLabel(tr("R:"))};
    for(int i=0;i<4;i++)
    {
        posLabel[i]->setFont(ft);
    }
    QLabel *atspeedLabel=new QLabel(tr("移动速度（mm/s）"));
    atspeedLabel->setFont(ft);
    atspeedNumEdit=new MyLineEdit;
    atspeedNumEdit->setFont(ft);
    QLabel *zhLabel=new QLabel(tr("Z提升到（mm）"));
    zhLabel->setFont(ft);
    zhEdit=new MyLineEdit;
    zhEdit->setFont(ft);
    QLabel *timeLabel=new QLabel(tr("延时时间（s）"));
    timeLabel->setFont(ft);
    delayTimeEdit=new MyLineEdit;
    delayTimeEdit->setFont(ft);

    ft.setPointSize(DLG_LABEL_BUTTON_FONT-2);
    for(int i=0;i<4;i++)
    {
        posEdit.append(new MyLineEdit);
        posEdit[i]->setFont(ft);
    }
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    QPushButton * updatePosBtn=new QPushButton(tr("更新"));
    updatePosBtn->setFont(ft);
    connect(updatePosBtn, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtnClicked()));

    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    QHBoxLayout *posLayout=new QHBoxLayout;
    posLayout->setMargin(0);
    for(int i=0;i<4;i++)
    {
        posLayout->addWidget(posLabel[i]);
        posLayout->addWidget(posEdit[i]);
    }
    posLayout->addWidget(updatePosBtn);

    QGridLayout *paraLayout = new QGridLayout;
    paraLayout->addWidget(atspeedLabel,0,0,1,1);
    paraLayout->addWidget(atspeedNumEdit,0,1,1,1);
    paraLayout->addWidget(zhLabel,1,0,1,1);
    paraLayout->addWidget(zhEdit,1,1,1,1);
    paraLayout->addWidget(timeLabel,2,0,1,1);
    paraLayout->addWidget(delayTimeEdit,2,1,1,1);
    paraLayout->addLayout(posLayout,3,0,1,6);
    paraLayout->addLayout(buttonLayout,4,0,1,6);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addLayout(paraLayout);
    //mainLayout->addLayout(posLayout);

    //connect(this, SIGNAL(loadDlgFinished()),this, SLOT(loadMyLineEdit()));
    setLayout(paraLayout);
    setWindowTitle(tr("空点设置"));
    resize(400,400);

    if(atspeed>0)
        atspeedNumEdit->setText(QString("%1").arg(atspeed,0,'f',0));
    if(zh>0)
        zhEdit->setText(QString("%1").arg(zh,0,'f',0));
    if(delayTime>0)
        delayTimeEdit->setText(QString("%1").arg(delayTime));
    for(int i=0;i<4;i++)
    {
        if(pointPos[i]>0)
            posEdit[i]->setText(QString("%1").arg(pointPos[i],0,'f',0));
    }

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);
    //loadDlgFinished();     //触发信号，加载自定义文本框
    atspeedNumEdit->context=context;
    atspeedNumEdit->myPos=QPoint(23,50);
    atspeedNumEdit->parentHeight=geometry().height();
    //timer=new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(loadMyLineEdit()));
    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
    //mySleep(50);
}

void BlankPointDlg::OnOkBtnClicked()
{
    atspeed=atspeedNumEdit->text().trimmed().toDouble();
    zh=zhEdit->text().trimmed().toDouble();
    delayTime=delayTimeEdit->text().trimmed().toInt();
    for(int i=0;i<4;i++)
        pointPos[i]=posEdit[i]->text().trimmed().toDouble();

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
    double xx=pointPos[0];
    double yy=pointPos[1];
    double zz=pointPos[2];
    double zz_ZH=pointPos[2]-zh;

    QString strErr,strErrX,strErrY,strErrZ,strErrZh,strErrR,strErrSpeed;
    if(xx<0 || xx>systemPara.limitPosLen[0])
    {
        strErrX=QString(tr(" X坐标超出正常范围 "));
        strErr+=strErrX;
    }
    if(yy<0 || yy>systemPara.limitPosLen[1])
    {
        strErrY=QString(tr(" Y坐标超出正常范围 "));
        strErr+=strErrY;
    }
    if(zz<0 || zz>systemPara.limitPosLen[2])
    {
        strErrZ=QString(tr(" Z坐标超出正常范围 "));
        strErr+=strErrZ;
    }
    else if(zz_ZH<0 || zh<5.0 || zz_ZH>systemPara.limitPosLen[2])
    {
        strErrZh=QString(tr(" Z坐标提升值错误 "));
        strErr+=strErrZh;
    }
    if(pointPos[3]<systemPara.limitPosLen[4] || pointPos[3]>systemPara.limitPosLen[3])
    {
        strErrR=QString(tr(" R坐标值错误 "));
        strErr+=strErrR;
    }
    if(atspeed<0 || atspeed>ROBOT_MAX_ATSPEED)
    {
        strErrSpeed=QString(tr(" 运行速度值超出正常范围 "));
        strErr+=strErrSpeed;
    }
    if(!strErr.isEmpty())
    {
        msgBox.setText(tr("系统发现")+strErr+tr("请重新修正参数！"));
        msgBox.exec();
        return;
    }

    done(QDialog::Accepted);
}

void BlankPointDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void BlankPointDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        currPointPos[1]=coordinate[7];
}

void BlankPointDlg::OnUpdatePosBtnClicked()
{
    posEdit[0]->setText(QString("%1").arg(currPointPos[0],0,'f',2));
    posEdit[1]->setText(QString("%1").arg(currPointPos[1],0,'f',2));
    posEdit[2]->setText(QString("%1").arg(currPointPos[2],0,'f',2));
    posEdit[3]->setText(QString("%1").arg(currPointPos[3],0,'f',2));
}

void BlankPointDlg::loadMyLineEdit()      //加载自定义文本框
{
    zhEdit->context=context;
    QRect rect=zhEdit->geometry();
    zhEdit->myPos=QPoint(rect.left()-100,rect.top());
    zhEdit->parentHeight=geometry().height();
    atspeedNumEdit->context=context;
    rect=atspeedNumEdit->geometry();
    atspeedNumEdit->myPos=QPoint(rect.left()-100,rect.top());
    atspeedNumEdit->parentHeight=geometry().height();
    delayTimeEdit->context=context;
    rect=delayTimeEdit->geometry();
    delayTimeEdit->myPos=QPoint(rect.left()-100,rect.top());
    delayTimeEdit->parentHeight=geometry().height();
    for(int i=0;i<4;i++)
    {
        posEdit[i]->context=context;
        QRect rect=posEdit[i]->geometry();
        posEdit[i]->myPos=QPoint(rect.left()-180,rect.top());
        posEdit[i]->parentHeight=geometry().height();
    }
    atspeedNumEdit->setFocus(Qt::OtherFocusReason);
}

void BlankPointDlg::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

///////////////////////////////////////////////////////////////////////////
/// 烙铁姿态对话框
///
IronPoseDlg::IronPoseDlg(QWidget *parent) :
    QDialog(parent)
{
    atspeed=0;
    zh=0;
    delayTime=0;
    for(int i=0;i<6;i++)
        pointPos[i]=0;
    exeOnce=0;
}

void IronPoseDlg::initDialog()
{
    setWindowFlags(Qt::FramelessWindowHint);
    //setAttribute(Qt::WA_TranslucentBackground);
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *posLabel[6]={new QLabel(tr("X:")),new QLabel(tr("Y:")),new QLabel(tr("Z:")),new QLabel(tr("R:")),new QLabel(tr("R1:")),new QLabel(tr("R2:"))};
    for(int i=0;i<6;i++)
    {
        posLabel[i]->setFont(ft);
    }
    QLabel *atspeedLabel=new QLabel(tr("移动速度（mm/s）"));
    atspeedLabel->setFont(ft);
    atspeedNumEdit=new MyLineEdit;
    atspeedNumEdit->setFont(ft);
    QLabel *zhLabel=new QLabel(tr("Z提升到（mm）"));
    zhLabel->setFont(ft);
    zhEdit=new MyLineEdit;
    zhEdit->setFont(ft);
    QLabel *timeLabel=new QLabel(tr("延时时间（s）"));
    timeLabel->setFont(ft);
    delayTimeEdit=new MyLineEdit;
    delayTimeEdit->setFont(ft);

    ft.setPointSize(DLG_LABEL_BUTTON_FONT-2);
    for(int i=0;i<6;i++)
    {
        posEdit.append(new MyLineEdit);
        posEdit[i]->setFont(ft);
    }
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    QPushButton * updatePosBtn=new QPushButton(tr("更新"));
    updatePosBtn->setFont(ft);
    connect(updatePosBtn, SIGNAL(clicked()), this, SLOT(OnUpdatePosBtnClicked()));

    onceCheckBox=new QCheckBox(tr("仅执行一次"));

    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    QPushButton * okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnOkBtnClicked()));
    QPushButton * cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelBtnClicked()));

    QHBoxLayout *buttonLayout=new QHBoxLayout;
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    QHBoxLayout *posLayout=new QHBoxLayout;
    posLayout->setMargin(0);
    for(int i=0;i<6;i++)
    {
        posLayout->addWidget(posLabel[i]);
        posLayout->addWidget(posEdit[i]);
    }
    posLayout->addWidget(updatePosBtn);

    QGridLayout *paraLayout = new QGridLayout;
    paraLayout->addWidget(atspeedLabel,0,0,1,1);
    paraLayout->addWidget(atspeedNumEdit,0,1,1,1);
    paraLayout->addWidget(zhLabel,1,0,1,1);
    paraLayout->addWidget(zhEdit,1,1,1,1);
    paraLayout->addWidget(timeLabel,2,0,1,1);
    paraLayout->addWidget(delayTimeEdit,2,1,1,1);
    paraLayout->addLayout(posLayout,3,0,1,6);
    paraLayout->addWidget(onceCheckBox,4,0,1,1);
    paraLayout->addLayout(buttonLayout,5,0,1,6);

    //QVBoxLayout *mainLayout = new QVBoxLayout;
    //mainLayout->addLayout(paraLayout);
    //mainLayout->addLayout(posLayout);

    //connect(this, SIGNAL(loadDlgFinished()),this, SLOT(loadMyLineEdit()));
    setLayout(paraLayout);
    setWindowTitle(tr("烙铁姿态设置"));
    resize(600,400);

    if(atspeed>0)
        atspeedNumEdit->setText(QString("%1").arg(atspeed,0,'f',0));
    if(zh>0)
        zhEdit->setText(QString("%1").arg(zh,0,'f',0));
    if(delayTime>0)
        delayTimeEdit->setText(QString("%1").arg(delayTime));
    for(int i=0;i<6;i++)
    {
         posEdit[i]->setText(QString("%1").arg(pointPos[i],0,'f',2));
    }
    if(exeOnce==1)
        onceCheckBox->setChecked(true);
    else
        onceCheckBox->setChecked(false);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);
    //loadDlgFinished();     //触发信号，加载自定义文本框
    atspeedNumEdit->context=context;
    atspeedNumEdit->myPos=QPoint(23,50);
    atspeedNumEdit->parentHeight=geometry().height();
    //timer=new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(loadMyLineEdit()));
    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
    //mySleep(50);
}

void IronPoseDlg::OnOkBtnClicked()
{
    atspeed=atspeedNumEdit->text().trimmed().toDouble();
    zh=zhEdit->text().trimmed().toDouble();
    delayTime=delayTimeEdit->text().trimmed().toInt();
    for(int i=0;i<6;i++)
        pointPos[i]=posEdit[i]->text().trimmed().toDouble();
    if(onceCheckBox->checkState()==Qt::Checked)
        exeOnce=1;
    else
        exeOnce=0;

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
    double xx=pointPos[0];
    double yy=pointPos[1];
    double zz=pointPos[2];
    double zz_ZH=pointPos[2]-zh;

    QString strErr,strErrX,strErrY,strErrZ,strErrZh,strErrR,strErrSpeed;
    if(xx<0 || xx>systemPara.limitPosLen[0])
    {
        strErrX=QString(tr(" X坐标超出正常范围 "));
        strErr+=strErrX;
    }
    if(yy<0 || yy>systemPara.limitPosLen[1])
    {
        strErrY=QString(tr(" Y坐标超出正常范围 "));
        strErr+=strErrY;
    }
    if(zz<0 || zz>systemPara.limitPosLen[2])
    {
        strErrZ=QString(tr(" Z坐标超出正常范围 "));
        strErr+=strErrZ;
    }
    else if(zz_ZH<0 || zh<5.0 || zz_ZH>systemPara.limitPosLen[2])
    {
        strErrZh=QString(tr(" Z坐标提升值错误 "));
        strErr+=strErrZh;
    }
    if(pointPos[3]<systemPara.limitPosLen[4] || pointPos[3]>systemPara.limitPosLen[3])
    {
        strErrR=QString(tr(" R坐标值错误 "));
        strErr+=strErrR;
    }
    if(pointPos[4]<systemPara.limitPosLen[6] || pointPos[4]>systemPara.limitPosLen[5])
    {
        strErrR=QString(tr(" R1坐标值错误 "));
        strErr+=strErrR;
    }
    if(pointPos[5]<systemPara.limitPosLen[8] || pointPos[5]>systemPara.limitPosLen[7])
    {
        strErrR=QString(tr(" R2坐标值错误 "));
        strErr+=strErrR;
    }
    if(atspeed<0 || atspeed>ROBOT_MAX_ATSPEED)
    {
        strErrSpeed=QString(tr(" 运行速度值超出正常范围 "));
        strErr+=strErrSpeed;
    }
    if(!strErr.isEmpty())
    {
        msgBox.setText(tr("系统发现")+strErr+tr("请重新修正参数！"));
        msgBox.exec();
        return;
    }

    done(QDialog::Accepted);
}

void IronPoseDlg::OnCancelBtnClicked()
{
    done(QDialog::Rejected);
}

void IronPoseDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        currPointPos[1]=coordinate[7];
}

void IronPoseDlg::OnUpdatePosBtnClicked()
{
    posEdit[0]->setText(QString("%1").arg(currPointPos[0],0,'f',2));
    posEdit[1]->setText(QString("%1").arg(currPointPos[1],0,'f',2));
    posEdit[2]->setText(QString("%1").arg(currPointPos[2],0,'f',2));
    posEdit[3]->setText(QString("%1").arg(currPointPos[3],0,'f',2));
    posEdit[4]->setText(QString("%1").arg(currPointPos[5],0,'f',2));
    posEdit[5]->setText(QString("%1").arg(currPointPos[6],0,'f',2));
}

void IronPoseDlg::loadMyLineEdit()      //加载自定义文本框
{
    zhEdit->context=context;
    QRect rect=zhEdit->geometry();
    zhEdit->myPos=QPoint(rect.left()-100,rect.top());
    zhEdit->parentHeight=geometry().height();
    atspeedNumEdit->context=context;
    rect=atspeedNumEdit->geometry();
    atspeedNumEdit->myPos=QPoint(rect.left()-100,rect.top());
    atspeedNumEdit->parentHeight=geometry().height();
    delayTimeEdit->context=context;
    rect=delayTimeEdit->geometry();
    delayTimeEdit->myPos=QPoint(rect.left()-100,rect.top());
    delayTimeEdit->parentHeight=geometry().height();
    for(int i=0;i<6;i++)
    {
        posEdit[i]->context=context;
        QRect rect=posEdit[i]->geometry();
        posEdit[i]->myPos=QPoint(rect.left()-180,rect.top());
        posEdit[i]->parentHeight=geometry().height();
    }
    atspeedNumEdit->setFocus(Qt::OtherFocusReason);
}

void IronPoseDlg::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
