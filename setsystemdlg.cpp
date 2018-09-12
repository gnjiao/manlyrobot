#include "setsystemdlg.h"
#include "setsystemadvdlg.h"
#include "global.h"
#include "myrwxml.h"

SetSystemDlg::SetSystemDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=700;
    dlgHeight=300;
}

void SetSystemDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    curPointAxisPos=QVector<double> (8);    //存储当前点的坐标
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=0;

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);
    QLabel *axisLabel[8];
    for(int i=0;i<8;i++)
    {
        axisLabel[i]=new QLabel;
        axisLabel[i]->setFont(ft);
        mHighSpeedEdit[i]=new MyLineEdit;
        mHighSpeedEdit[i]->setFont(ft);
        mLowSpeedEdit[i]=new MyLineEdit;
        mLowSpeedEdit[i]->setFont(ft);
        aHighSpeedEdit[i]=new MyLineEdit;
        aHighSpeedEdit[i]->setFont(ft);
        aLowSpeedEdit[i]=new MyLineEdit;
        aLowSpeedEdit[i]->setFont(ft);
    }
    axisLabel[0]->setText(tr("X轴"));
    axisLabel[1]->setText(tr("Y轴"));
    axisLabel[2]->setText(tr("Z轴"));
    axisLabel[3]->setText(tr("R轴"));
    axisLabel[4]->setText(tr("R1轴"));
    axisLabel[5]->setText(tr("R2轴"));
    axisLabel[6]->setText(tr("S轴"));
    axisLabel[7]->setText(tr("E轴"));

    QLabel *axisPosLabel[9];
    QPushButton *currPosBtn[9];

    axisCurrPosBtn=new QButtonGroup;
    connect(axisCurrPosBtn, SIGNAL(buttonClicked(int)), this, SLOT(OnBtnGroupClicked(int)));
    CurrPosBtnText.append(tr("X轴当前位置"));
    CurrPosBtnText.append(tr("Y轴当前位置"));
    CurrPosBtnText.append(tr("Z轴当前位置"));
    CurrPosBtnText.append(tr("R轴当前位置"));
    CurrPosBtnText.append(tr("R轴当前位置"));
    CurrPosBtnText.append(tr("R1轴当前位置"));
    CurrPosBtnText.append(tr("R1轴当前位置"));
    CurrPosBtnText.append(tr("R2轴当前位置"));
    CurrPosBtnText.append(tr("R2轴当前位置"));
    for(int i=0;i<9;i++)
    {
        axisPosLabel[i]=new QLabel;
        axisPosLabel[i]->setFont(ft);
        axisLimitPosEdit[i]=new MyLineEdit;
        axisLimitPosEdit[i]->setFont(ft);
        axisLimitPosEdit[i]->setMinimumWidth(dlgWidth/7);
        currPosBtn[i]=new QPushButton;
        currPosBtn[i]->setFont(ft);
        currPosBtn[i]->setText(CurrPosBtnText[i]);
        axisCurrPosBtn->addButton(currPosBtn[i],i);
    }
    axisPosLabel[0]->setText(tr("X轴行程"));
    axisPosLabel[1]->setText(tr("Y轴行程"));
    axisPosLabel[2]->setText(tr("Z轴行程"));
    axisPosLabel[3]->setText(tr("R轴正行程"));
    axisPosLabel[4]->setText(tr("R轴负行程"));
    axisPosLabel[5]->setText(tr("R1轴正行程"));
    axisPosLabel[6]->setText(tr("R1轴负行程"));
    axisPosLabel[7]->setText(tr("R2轴正行程"));
    axisPosLabel[8]->setText(tr("R2轴负行程"));

    QLabel *outLabel=new QLabel(tr("手动出锡速度(mm/s)"));
    outLabel->setFont(ft);
    outLabel->setAlignment(Qt::AlignRight);
    QLabel *backLabel=new QLabel(tr("手动退锡速度(mm/s)"));
    backLabel->setFont(ft);
    backLabel->setAlignment(Qt::AlignRight);
    outSolderSpeedEdit=new MyLineEdit;
    outSolderSpeedEdit->setFont(ft);
    outSolderSpeedEdit->setMaximumWidth(dlgWidth/3);
    backSolderSpeedEdit=new MyLineEdit;
    backSolderSpeedEdit->setFont(ft);
    backSolderSpeedEdit->setMaximumWidth(dlgWidth/3);

    QPushButton *advanceSetBtn=new QPushButton(tr("高级设置"));
    advanceSetBtn->setFont(ft);
    connect(advanceSetBtn, SIGNAL(clicked()), this, SLOT(OnAdvanceSetBtnClicked()));

    QPushButton *remoteBtn=new QPushButton(tr("远程操作"));
    remoteBtn->setFont(ft);
    connect(remoteBtn, SIGNAL(clicked()), this, SLOT(onRemoteBtnBtnClicked()));
    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelClicked()));

    QLabel *unitLabel1=new QLabel(tr("单位：脉冲/秒"));
    unitLabel1->setFont(ft);
    unitLabel1->setMaximumHeight(dlgHeight/15);

    QGridLayout *axisSpeedLayout = new QGridLayout;
    axisSpeedLayout->addWidget(unitLabel1,0,0,1,5);
    QLabel *lb1=new QLabel(tr("转轴"));
    lb1->setFont(ft);
    axisSpeedLayout->addWidget(lb1,1,0);
    QLabel *lb2=new QLabel(tr("手动高速"));
    axisSpeedLayout->addWidget(lb2,1,1);
    QLabel *lb3=new QLabel(tr("手动低速"));
    axisSpeedLayout->addWidget(lb3,1,2);
    QLabel *lb4=new QLabel(tr("自动高速"));
    axisSpeedLayout->addWidget(lb4,1,3);
    QLabel *lb5=new QLabel(tr("自动低速"));
    axisSpeedLayout->addWidget(lb5,1,4);

    lb1->setMaximumHeight(dlgHeight/15);
    lb2->setMaximumHeight(dlgHeight/15);
    lb3->setMaximumHeight(dlgHeight/15);
    lb4->setMaximumHeight(dlgHeight/15);
    lb5->setMaximumHeight(dlgHeight/15);
    for(int i=0;i<8;i++)
    {
        axisSpeedLayout->addWidget(axisLabel[i],i+2,0);
        axisSpeedLayout->addWidget(mHighSpeedEdit[i],i+2,1);
        axisSpeedLayout->addWidget(mLowSpeedEdit[i],i+2,2);
        axisSpeedLayout->addWidget(aHighSpeedEdit[i],i+2,3);
        axisSpeedLayout->addWidget(aLowSpeedEdit[i],i+2,4);
    }

    QLabel *unitLabel2=new QLabel(tr("单位：mm"));
    unitLabel2->setFont(ft);
    unitLabel2->setMaximumHeight(axisLimitPosEdit[0]->size().height());
    QGridLayout *axisPosLayout = new QGridLayout;
    axisPosLayout->addWidget(unitLabel2,0,0,1,3);
    for(int i=0;i<9;i++)
    {
        axisPosLayout->addWidget(axisPosLabel[i],i+1,0);
        axisPosLayout->addWidget(axisLimitPosEdit[i],i+1,1);
        axisPosLayout->addWidget(axisCurrPosBtn->button(i),i+1,2);
    }

    QGridLayout *otherLayout = new QGridLayout;
    otherLayout->addWidget(outLabel,0,0);
    otherLayout->addWidget(outSolderSpeedEdit,0,1);
    otherLayout->addWidget(advanceSetBtn,0,2,2,1);
    otherLayout->addWidget(backLabel,1,0);
    otherLayout->addWidget(backSolderSpeedEdit,1,1);
    otherLayout->addWidget(remoteBtn,2,0);
    otherLayout->addWidget(okBtn,2,1);
    otherLayout->addWidget(cancelBtn,2,2);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(axisSpeedLayout,0,0,4,1);
    mainLayout->addLayout(axisPosLayout,0,1,4,1);
    mainLayout->addLayout(otherLayout,4,0,1,2);
/*
    QHBoxLayout *axisLayout = new QHBoxLayout;
    axisLayout->addLayout(axisSpeedLayout);
    axisLayout->addLayout(axisPosLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(axisLayout);
    mainLayout->addLayout(otherLayout);
*/
    setLayout(mainLayout);
    setWindowTitle(tr("系统设置"));
    resize(dlgWidth,dlgHeight);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    MyRWxml xml;
    SYSTEM_PARAMS systemPara;
    xml.readSysPara("robot.xml",systemPara);
    for(int i=0;i<8;i++)
    {
        if((i<4) || (i==7))
        {
            mHighSpeedEdit[i]->setText(QString("%1").arg(systemPara.highSpeed[i]));
            mLowSpeedEdit[i]->setText(QString("%1").arg(systemPara.lowSpeed[i]));
            aHighSpeedEdit[i]->setText(QString("%1").arg(systemPara.aHighSpeed[i]));
            aLowSpeedEdit[i]->setText(QString("%1").arg(systemPara.aLowSpeed[i]));
        }
        else if(i<6)
        {
            mHighSpeedEdit[i]->setText(QString("%1").arg(systemPara.highSpeed[i+1]));
            mLowSpeedEdit[i]->setText(QString("%1").arg(systemPara.lowSpeed[i+1]));
            aHighSpeedEdit[i]->setText(QString("%1").arg(systemPara.aHighSpeed[i+1]));
            aLowSpeedEdit[i]->setText(QString("%1").arg(systemPara.aLowSpeed[i+1]));
        }
        else if(i==6)
        {
            mHighSpeedEdit[i]->setText(QString("%1").arg(systemPara.highSpeed[4]));
            mLowSpeedEdit[i]->setText(QString("%1").arg(systemPara.lowSpeed[4]));
            aHighSpeedEdit[i]->setText(QString("%1").arg(systemPara.aHighSpeed[4]));
            aLowSpeedEdit[i]->setText(QString("%1").arg(systemPara.aLowSpeed[4]));
        }
    }
    for(int i=0;i<9;i++)
    {
        axisLimitPosEdit[i]->setText(QString("%1").arg(systemPara.limitPosLen[i],0,'f',1));
    }
    outSolderSpeedEdit->setText(QString("%1").arg(systemPara.outSolderSpeed,0,'f',1));
    backSolderSpeedEdit->setText(QString("%1").arg(systemPara.backSolderSpeed,0,'f',1));
    okBtn->setFocus(Qt::OtherFocusReason);

    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void SetSystemDlg::updateCurrCoordinate(double *coordinate)  //更新当前坐标
{
    for(int i=0;i<8;i++)
        curPointAxisPos[i]=coordinate[i];
}

void SetSystemDlg::OnBtnGroupClicked(int id)
{
    if(id<3)
        axisLimitPosEdit[id]->setText(QString("%1").arg(curPointAxisPos[id],0,'f',2));
    else if((id>2) && (id<5))
        axisLimitPosEdit[id]->setText(QString("%1").arg(curPointAxisPos[3],0,'f',2));
    else if((id>4) && (id<7))
        axisLimitPosEdit[id]->setText(QString("%1").arg(curPointAxisPos[5],0,'f',2));
    else if((id>6) && (id<9))
        axisLimitPosEdit[id]->setText(QString("%1").arg(curPointAxisPos[6],0,'f',2));
}
void SetSystemDlg::OnAdvanceSetBtnClicked()
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

    if(g_userRight<USER_RIGHT_VENDER) //厂家级以下，权限不足
    {
        msgBox.setText(tr("需要厂家级权限."));
        msgBox.exec();
        return;
    }
    SetSystemAdvDlg *dlg=new SetSystemAdvDlg(this);
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
}

void SetSystemDlg::onOkBtnClicked()
{
    if(QFile::exists("robot.xml"))
    {
        QDir dir;
        dir.remove("robot.xml");
    }
    SYSTEM_PARAMS systemPara;
    for(int i=0;i<8;i++)
    {
        if((i<4) || (i==7))
        {
            systemPara.highSpeed[i]=mHighSpeedEdit[i]->text().trimmed().toInt();
            systemPara.lowSpeed[i]=mLowSpeedEdit[i]->text().trimmed().toInt();
            systemPara.aHighSpeed[i]=aHighSpeedEdit[i]->text().trimmed().toInt();
            systemPara.aLowSpeed[i]=aLowSpeedEdit[i]->text().trimmed().toInt();
        }
        else if(i<6)
        {
            systemPara.highSpeed[i+1]=mHighSpeedEdit[i]->text().trimmed().toInt();
            systemPara.lowSpeed[i+1]=mLowSpeedEdit[i]->text().trimmed().toInt();
            systemPara.aHighSpeed[i+1]=aHighSpeedEdit[i]->text().trimmed().toInt();
            systemPara.aLowSpeed[i+1]=aLowSpeedEdit[i]->text().trimmed().toInt();
        }
        else if(i==6)
        {
            systemPara.highSpeed[4]=mHighSpeedEdit[i]->text().trimmed().toInt();
            systemPara.lowSpeed[4]=mLowSpeedEdit[i]->text().trimmed().toInt();
            systemPara.aHighSpeed[4]=aHighSpeedEdit[i]->text().trimmed().toInt();
            systemPara.aLowSpeed[4]=aLowSpeedEdit[i]->text().trimmed().toInt();
        }
    }
    for(int i=0;i<9;i++)
        systemPara.limitPosLen[i]=axisLimitPosEdit[i]->text().trimmed().toDouble();
    systemPara.outSolderSpeed=outSolderSpeedEdit->text().trimmed().toDouble();
    systemPara.backSolderSpeed=backSolderSpeedEdit->text().trimmed().toDouble();

    MyRWxml xml;
    xml.writeSysPara("robot.xml",systemPara);
    done(QDialog::Accepted);
}

void SetSystemDlg::OnCancelClicked()
{
    done(QDialog::Rejected);
}

void SetSystemDlg::onRemoteBtnBtnClicked()
{
    QProcess *program=new QProcess(this);//Progra~1
    program->start("C:/Progra~2/Tencent/QQ/QQProtect/Bin/QQProtect.exe");
    program->waitForStarted();
    program->waitForFinished();
    delete program;
}

void SetSystemDlg::loadMyLineEdit()      //加载自定义文本框
{
    outSolderSpeedEdit->context=context;
    QRect rect=outSolderSpeedEdit->geometry();
    outSolderSpeedEdit->myPos=QPoint(rect.left()-100,rect.top());
    outSolderSpeedEdit->parentHeight=geometry().height();

    backSolderSpeedEdit->context=context;
    rect=backSolderSpeedEdit->geometry();
    backSolderSpeedEdit->myPos=QPoint(rect.left()-100,rect.top());
    backSolderSpeedEdit->parentHeight=geometry().height();

    for(int i=0;i<8;i++)
    {
        mHighSpeedEdit[i]->context=context;
        QRect rect=mHighSpeedEdit[i]->geometry();
        mHighSpeedEdit[i]->myPos=QPoint(rect.left()-100,rect.top());
        mHighSpeedEdit[i]->parentHeight=geometry().height()+60;

        mLowSpeedEdit[i]->context=context;
        rect=mLowSpeedEdit[i]->geometry();
        mLowSpeedEdit[i]->myPos=QPoint(rect.left()-100,rect.top());
        mLowSpeedEdit[i]->parentHeight=geometry().height()+60;

        aHighSpeedEdit[i]->context=context;
        rect=aHighSpeedEdit[i]->geometry();
        aHighSpeedEdit[i]->myPos=QPoint(rect.left()+50,rect.top());
        aHighSpeedEdit[i]->parentHeight=geometry().height()+60;

        aLowSpeedEdit[i]->context=context;
        rect=aLowSpeedEdit[i]->geometry();
        aLowSpeedEdit[i]->myPos=QPoint(rect.left()+50,rect.top());
        aLowSpeedEdit[i]->parentHeight=geometry().height()+60;
    }
    for(int i=0;i<9;i++)
    {
        axisLimitPosEdit[i]->context=context;
        QRect rect=axisLimitPosEdit[i]->geometry();
        axisLimitPosEdit[i]->myPos=QPoint(rect.left()-30,rect.top());
        axisLimitPosEdit[i]->parentHeight=geometry().height()+60;
    }
    //mHighSpeedEdit[0]->setFocus(Qt::OtherFocusReason);
}
