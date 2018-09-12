#include "clearsolderdlg.h"
#include "global.h"
#include "myrwxml.h"
#include "msqldb.h"

ClearSolderDlg::ClearSolderDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=600;
    dlgHeight=400;
    for(int i=0;i<8;i++)
        clearItem.clearPos[i]=0;
    for(int i=0;i<8;i++)
        currPointPos[i]=0;
}
void ClearSolderDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    setWindowFlags(Qt::FramelessWindowHint);

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QStringList clearListStr;
    clearListStr<<tr("运行速度(mm/s)")<<tr("Z提升到高度(mm)")<<tr("清锡时间(ms)")<<tr("送锡时间(ms)");//<<tr("送锡长度(mm)")<<tr("送锡速度(mm/s)")
    QLabel *clearLabel[4];
    for(int i=0;i<4;i++)
    {
        clearLabel[i]=new QLabel(clearListStr[i]);
        clearLabel[i]->setFont(ft);
        clearGroup[i]=new MyLineEdit;
        clearGroup[i]->setFont(ft);
    }

    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelClicked()));
    QPushButton *updateBtn=new QPushButton(tr("更新"));
    updateBtn->setFont(ft);
    connect(updateBtn, SIGNAL(clicked()), this, SLOT(OnUpdatePosClicked()));

    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);


    //QLabel *coordLabel=new QLabel(tr("坐标系:"));
    //coordLabel->setFont(ft);
    QLabel *xLabel=new QLabel(tr("X:"));
    xLabel->setFont(ft);
    QLabel *yLabel=new QLabel(tr("Y:"));
    yLabel->setFont(ft);
    QLabel *zLabel=new QLabel(tr("Z:"));
    zLabel->setFont(ft);
    QLabel *rLabel=new QLabel(tr("R:"));
    rLabel->setFont(ft);
    QLabel *r1Label=new QLabel(tr("R1:"));
    r1Label->setFont(ft);
    QLabel *r2Label=new QLabel(tr("R2:"));
    r2Label->setFont(ft);
    QLabel *unitLabel=new QLabel(tr("单位:"));
    unitLabel->setFont(ft);


    ft.setPointSize(LABEL_BUTTON_FONT);
    xEdit=new MyLineEdit;
    xEdit->setFont(ft);
    yEdit=new MyLineEdit;
    yEdit->setFont(ft);
    zEdit=new MyLineEdit;
    zEdit->setFont(ft);
    rEdit=new MyLineEdit;
    rEdit->setFont(ft);
    r1Edit=new MyLineEdit;
    r1Edit->setFont(ft);
    r2Edit=new MyLineEdit;
    r2Edit->setFont(ft);
    //ft.setPointSize(20);
    unitEdit=new QLineEdit;
    unitEdit->setFont(ft);
    unitEdit->setAlignment(Qt::AlignCenter);
    unitEdit->setText("mm");

    QHBoxLayout *coordLayout = new QHBoxLayout;
    coordLayout->addWidget(xLabel);
    coordLayout->addWidget(xEdit);
    coordLayout->addWidget(yLabel);
    coordLayout->addWidget(yEdit);
    coordLayout->addWidget(zLabel);
    coordLayout->addWidget(zEdit);
    coordLayout->addWidget(rLabel);
    coordLayout->addWidget(rEdit);
    if((g_MacType==3 || g_MacType==1 || g_MacType==4))
    {
        coordLayout->addWidget(r1Label);
        coordLayout->addWidget(r1Edit);
        coordLayout->addWidget(r2Label);
        coordLayout->addWidget(r2Edit);
    }
    //coordLayout->addWidget(unitLabel);
    //coordLayout->addWidget(unitEdit);
    coordLayout->addWidget(updateBtn);


    QGridLayout *clearLayout = new QGridLayout;
    for(int i=0;i<4;i++)
    {
        clearLayout->addWidget(clearLabel[i],i,0,1,3);
        clearLayout->addWidget(clearGroup[i],i,3,1,3);
    }
    clearLayout->addLayout(coordLayout,4,0,1,6);
    clearLayout->addWidget(okBtn,5,0,1,3);
    clearLayout->addWidget(cancelBtn,5,3,1,3);
    setLayout(clearLayout);

    setWindowTitle(tr("清锡设置"));
    resize(dlgWidth,dlgHeight);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        return;
    }
    if(db.ReadClearSolderFromDb(weldProject->getProjName(),clearItem))
    {
        clearGroup[0]->setText(QString("%1").arg(clearItem.atspeed,0,'f',1));
        clearGroup[1]->setText(QString("%1").arg(clearItem.Zh,0,'f',1));
        clearGroup[2]->setText(QString("%1").arg(clearItem.clearTime));
        clearGroup[3]->setText(QString("%1").arg(clearItem.clear_H));
        xEdit->setText(QString("%1").arg(clearItem.clearPos[0],0,'f',2));
        yEdit->setText(QString("%1").arg(clearItem.clearPos[1],0,'f',2));
        zEdit->setText(QString("%1").arg(clearItem.clearPos[2],0,'f',2));
        rEdit->setText(QString("%1").arg(clearItem.clearPos[3],0,'f',2));
        if((g_MacType==3 || g_MacType==1 || g_MacType==4))
        {
            r1Edit->setText(QString("%1").arg(clearItem.clearPos[5],0,'f',2));
            r2Edit->setText(QString("%1").arg(clearItem.clearPos[6],0,'f',2));
        }
        db.closeDB();
    }
    okBtn->setFocus(Qt::OtherFocusReason);

    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void ClearSolderDlg::updateCoordinate(double *coordinate)
{
    for(int i=0;i<8;i++)
    {
        currPointPos[i]=coordinate[i];
    }
    if(((g_MacType==2) || (g_MacType==4) || (g_MacType==6)) && (currPointPos[0]>systemPara.limitPosLen[0]/2))    //双平台
        currPointPos[1]=coordinate[7];
}

void ClearSolderDlg::OnUpdatePosClicked()
{
    xEdit->setText(QString("%1").arg(currPointPos[0],0,'f',2));
    yEdit->setText(QString("%1").arg(currPointPos[1],0,'f',2));
    zEdit->setText(QString("%1").arg(currPointPos[2],0,'f',2));
    rEdit->setText(QString("%1").arg(currPointPos[3],0,'f',2));
    if((g_MacType==3 || g_MacType==1 || g_MacType==4))
    {
        r1Edit->setText(QString("%1").arg(currPointPos[5],0,'f',2));
        r2Edit->setText(QString("%1").arg(currPointPos[6],0,'f',2));
    }
    clearItem.clearPos[5]=currPointPos[5];
    clearItem.clearPos[6]=currPointPos[6];
}

void ClearSolderDlg::onOkBtnClicked()
{
    clearItem.projectName=weldProject->getProjName();
    clearItem.atspeed=clearGroup[0]->text().trimmed().toDouble();
    clearItem.Zh=clearGroup[1]->text().trimmed().toDouble();
    clearItem.clearTime=clearGroup[2]->text().trimmed().toInt();
    clearItem.clear_H=clearGroup[3]->text().trimmed().toInt();
    clearItem.clearPos[0]=xEdit->text().trimmed().toDouble();
    clearItem.clearPos[1]=yEdit->text().trimmed().toDouble();
    clearItem.clearPos[2]=zEdit->text().trimmed().toDouble();
    clearItem.clearPos[3]=rEdit->text().trimmed().toDouble();
    for(int i=4;i<8;i++)
        clearItem.clearPos[i]=0;
    clearItem.clearPos[5]=currPointPos[5];
    clearItem.clearPos[6]=currPointPos[6];

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
    double xx=clearItem.clearPos[0];
    double yy=clearItem.clearPos[1];
    double zz=clearItem.clearPos[2];
    double zz_ZH=clearItem.clearPos[2]-clearItem.Zh;

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
    else if(zz_ZH<0 || clearItem.Zh<5.0)
    {
        strErrZh=QString(tr(" Z坐标提升值错误 "));
        strErr+=strErrZh;
    }
    if(clearItem.clearPos[3]<systemPara.limitPosLen[4] || clearItem.clearPos[3]>systemPara.limitPosLen[3])
    {
        strErrR=QString(tr(" R坐标值错误 "));
        strErr+=strErrR;
    }
    if(clearItem.atspeed<0 || clearItem.atspeed>ROBOT_MAX_ATSPEED)
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
void ClearSolderDlg::OnCancelClicked()
{
    done(QDialog::Rejected);
}

void ClearSolderDlg::loadMyLineEdit()      //加载自定义文本框
{
    xEdit->context=context;
    QRect rect=xEdit->geometry();
    xEdit->myPos=QPoint(rect.left()-100,rect.top());
    xEdit->parentHeight=geometry().height()+60;

    yEdit->context=context;
    rect=yEdit->geometry();
    yEdit->myPos=QPoint(rect.left()-100,rect.top());
    yEdit->parentHeight=geometry().height()+60;

    zEdit->context=context;
    rect=zEdit->geometry();
    zEdit->myPos=QPoint(rect.left()-100,rect.top());
    zEdit->parentHeight=geometry().height()+60;

    rEdit->context=context;
    rect=rEdit->geometry();
    rEdit->myPos=QPoint(rect.left()-100,rect.top());
    rEdit->parentHeight=geometry().height()+60;

    if((g_MacType==3 || g_MacType==1 || g_MacType==4))
    {
        r1Edit->context=context;
        rect=r1Edit->geometry();
        r1Edit->myPos=QPoint(rect.left()-100,rect.top());
        r1Edit->parentHeight=geometry().height()+60;
        r2Edit->context=context;
        rect=r2Edit->geometry();
        r2Edit->myPos=QPoint(rect.left()-100,rect.top());
        r2Edit->parentHeight=geometry().height()+60;
    }

    for(int i=0;i<4;i++)
    {
        clearGroup[i]->context=context;
        rect=clearGroup[i]->geometry();
        clearGroup[i]->myPos=QPoint(rect.left()-100,rect.top());
        clearGroup[i]->parentHeight=geometry().height()+60;
    }
}
