#include "setsystemadvdlg.h"
#include "global.h"
#include "myrwxml.h"
#include "setiodlg.h"
#include "setspeeddlg.h"

SetSystemAdvDlg::SetSystemAdvDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=700;
    dlgHeight=500;
}

void SetSystemAdvDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *maxPulLabel=new QLabel(tr("伺服最大输入脉冲(pps)"));
    maxPulLabel->setFont(ft);
    maxPulseEdit=new MyLineEdit;
    maxPulseEdit->setFont(ft);
    fastStopChk=new QCheckBox(tr("紧急停止启动"));
    fastStopChk->setChecked(true);
    fastStopChk->setFont(ft);
    QHBoxLayout *topLayout=new QHBoxLayout;
    topLayout->addWidget(maxPulLabel);
    topLayout->addWidget(maxPulseEdit);
    topLayout->addWidget(fastStopChk);

    QLabel *ctrllLabel=new QLabel(tr("控制器参数："));
    ctrllLabel->setFont(ft);

    QList <QLabel *> listLabel;
    QStringList listStr,axisStr;
    listStr<<tr("转轴")<<tr("输出脉冲格式")<<tr("VLPPS")<<tr("VHPPS")<<tr("Tacc")<<tr("Tdec")<<tr("Tsacc")<<tr("Tsdec");
    axisStr<<tr("X轴")<<tr("Y轴")<<tr("Z轴")<<tr("R轴")<<tr("R1轴")<<tr("R2轴")<<tr("S轴")<<tr("E轴");
    for(int i=0;i<8;i++)
    {
        listLabel.append(new QLabel(listStr[i]));
        listLabel[i]->setFont(ft);
    }

    QLabel *axisLabel[8];
    for(int i=0;i<8;i++)
    {
        axisLabel[i]=new QLabel;
        axisLabel[i]->setFont(ft);
        axisLabel[i]->setText(axisStr[i]);
        OutPulseFormComBox[i]=new QComboBox;
        OutPulseFormComBox[i]->setFont(ft);
        OutPulseFormComBox[i]->addItem("Pulse/Dir");
        OutPulseFormComBox[i]->addItem("CW/CCW");
        OutPulseFormComBox[i]->addItem("AB-phase");
        OutPulseFormComBox[i]->setCurrentIndex(0);
        VLPPS_Edit[i]=new MyLineEdit;
        VLPPS_Edit[i]->setFont(ft);
        VHPPS_Edit[i]=new MyLineEdit;
        VHPPS_Edit[i]->setFont(ft);
        Tacc_Edit[i]=new MyLineEdit;
        Tacc_Edit[i]->setFont(ft);
        Tdec_Edit[i]=new MyLineEdit;
        Tdec_Edit[i]->setFont(ft);
        Tsacc_Edit[i]=new MyLineEdit;
        Tsacc_Edit[i]->setFont(ft);
        Tsdec_Edit[i]=new MyLineEdit;
        Tsdec_Edit[i]->setFont(ft);
    }

    QGridLayout *ctrlLayout = new QGridLayout;
    ctrlLayout->addWidget(ctrllLabel,0,0,1,8);
    for(int i=0;i<8;i++)
        ctrlLayout->addWidget(listLabel[i],1,i);
    for(int i=0;i<8;i++)
    {
        ctrlLayout->addWidget(axisLabel[i],i+2,0);
        ctrlLayout->addWidget(OutPulseFormComBox[i],i+2,1);
        ctrlLayout->addWidget(VLPPS_Edit[i],i+2,2);
        ctrlLayout->addWidget(VHPPS_Edit[i],i+2,3);
        ctrlLayout->addWidget(Tacc_Edit[i],i+2,4);
        ctrlLayout->addWidget(Tdec_Edit[i],i+2,5);
        ctrlLayout->addWidget(Tsacc_Edit[i],i+2,6);
        ctrlLayout->addWidget(Tsdec_Edit[i],i+2,7);
    }

    QPushButton *ioSetBtn=new QPushButton(tr("IO配置"));
    ioSetBtn->setFont(ft);
    connect(ioSetBtn, SIGNAL(clicked()), this, SLOT(OnIOSetBtnClicked()));

    QPushButton *speedSetBtn=new QPushButton(tr("防冲击配置"));
    speedSetBtn->setFont(ft);
    connect(speedSetBtn, SIGNAL(clicked()), this, SLOT(OnSpeedSetBtnClicked()));

    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelClicked()));

    QLabel *macLabel=new QLabel(tr("电机及机械参数："));
    macLabel->setFont(ft);
    listStr.clear();
    listStr<<tr("转轴")<<tr("PULSE_Per")<<tr("最高限速")<<tr("机械步距");
    QLabel *listLabel2[4];
    for(int i=0;i<4;i++)
    {
        listLabel2[i]=new QLabel(listStr[i]);
        listLabel2[i]->setFont(ft);
    }
    QLabel *axisLabel2[8];
    for(int i=0;i<8;i++)
    {
        axisLabel2[i]=new QLabel;
        axisLabel2[i]->setFont(ft);
        axisLabel2[i]->setText(axisStr[i]);
        PulsePerComBox[i]=new QComboBox;
        PulsePerComBox[i]->setFont(ft);
        PulsePerComBox[i]->addItem("6400");
        PulsePerComBox[i]->addItem("200");
        PulsePerComBox[i]->addItem("1600");
        PulsePerComBox[i]->addItem("3200");
        PulsePerComBox[i]->addItem("12800");
        PulsePerComBox[i]->setCurrentIndex(0);
        maxSpeedEdit[i]=new MyLineEdit;
        maxSpeedEdit[i]->setFont(ft);
        macStepLenEdit[i]=new MyLineEdit;
        macStepLenEdit[i]->setFont(ft);
    }
    PulsePerComBox[0]->setCurrentIndex(0);

    QGridLayout *macLayout = new QGridLayout;
    macLayout->addWidget(macLabel,0,0,1,4);
    for(int i=0;i<4;i++)
        macLayout->addWidget(listLabel2[i],1,i);
    for(int i=0;i<8;i++)
    {
        macLayout->addWidget(axisLabel2[i],i+2,0);
        macLayout->addWidget(PulsePerComBox[i],i+2,1);
        macLayout->addWidget(maxSpeedEdit[i],i+2,2);
        macLayout->addWidget(macStepLenEdit[i],i+2,3);
    }

    QLabel *orgLabel=new QLabel(tr("回原点参数："));
    orgLabel->setFont(ft);
    listStr.clear();
    listStr<<tr("转轴")<<tr("VL_Home")<<tr("VH_hm")<<tr("TAcc");
    QLabel *listLabel3[4];
    for(int i=0;i<4;i++)
    {
        listLabel3[i]=new QLabel(listStr[i]);
        listLabel3[i]->setFont(ft);
    }
    QLabel *axisLabel3[8];
    for(int i=0;i<8;i++)
    {
        axisLabel3[i]=new QLabel;
        axisLabel3[i]->setFont(ft);
        axisLabel3[i]->setText(axisStr[i]);
        VL_Home_Edit[i]=new MyLineEdit;
        VL_Home_Edit[i]->setFont(ft);
        VH_Hm_Edit[i]=new MyLineEdit;
        VH_Hm_Edit[i]->setFont(ft);
        TAcc_Edit[i]=new MyLineEdit;
        TAcc_Edit[i]->setFont(ft);
    }

    QGridLayout *orgLayout = new QGridLayout;
    orgLayout->addWidget(orgLabel,0,0,1,4);
    for(int i=0;i<4;i++)
        orgLayout->addWidget(listLabel3[i],1,i);
    for(int i=0;i<8;i++)
    {
        orgLayout->addWidget(axisLabel3[i],i+2,0);
        orgLayout->addWidget(VL_Home_Edit[i],i+2,1);
        orgLayout->addWidget(VH_Hm_Edit[i],i+2,2);
        orgLayout->addWidget(TAcc_Edit[i],i+2,3);
    }

    QLabel *typeLabel=new QLabel(tr("机器类型"));
    typeLabel->setFont(ft);

    typeComboBox=new QComboBox;
    typeComboBox->addItem(tr("4轴单平台"));
    typeComboBox->addItem(tr("6轴在线式"));
    typeComboBox->addItem(tr("4轴双平台"));
    typeComboBox->addItem(tr("6轴单平台"));
    typeComboBox->addItem(tr("6轴双平台"));
    typeComboBox->addItem(tr("3轴单平台"));
    typeComboBox->addItem(tr("3轴双平台"));
    typeComboBox->setCurrentIndex(0);
    typeComboBox->setFont(ft);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(typeComboBox);
    typeLayout->addWidget(ioSetBtn);
    typeLayout->addWidget(speedSetBtn);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    //btnLayout->addWidget(ioSetBtn);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(topLayout,0,0,1,2);
    mainLayout->addLayout(ctrlLayout,1,0,10,2);
    mainLayout->addLayout(macLayout,11,0,10,1);
    mainLayout->addLayout(orgLayout,11,1,10,1);
    mainLayout->addLayout(typeLayout,22,0,1,3);
    mainLayout->addLayout(btnLayout,23,0,1,2);
/*
    QHBoxLayout *axisLayout = new QHBoxLayout;
    axisLayout->addLayout(axisSpeedLayout);
    axisLayout->addLayout(axisPosLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(axisLayout);
    mainLayout->addLayout(otherLayout);
*/
    setLayout(mainLayout);
    setWindowTitle(tr("系统高级设置"));
    resize(dlgWidth,dlgHeight);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    MyRWxml xml;
    SYSTEM_ADV_PARAMS systemAdvPara;
    xml.readSysAdvPara("robota.xml",systemAdvPara);
    maxPulseEdit->setText(QString("%1").arg(systemAdvPara.maxPusle));
    if(systemAdvPara.stopStart==0)
        fastStopChk->setChecked(false);
    else
        fastStopChk->setChecked(true);
    for(int i=0;i<8;i++)
    {
        if((i<4) || (i==7))
        {
            OutPulseFormComBox[i]->setCurrentIndex(systemAdvPara.Out_pulse_form[i]);
            VLPPS_Edit[i]->setText(QString("%1").arg(systemAdvPara.VLPPS[i]));
            VHPPS_Edit[i]->setText(QString("%1").arg(systemAdvPara.VH[i]));
            Tacc_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tacc[i]));
            Tdec_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tdec[i]));
            Tsacc_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tsacc[i]));
            Tsdec_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tsdec[i]));

            PulsePerComBox[i]->setCurrentIndex(systemAdvPara.PULSE_Per[i]);
            maxSpeedEdit[i]->setText(QString("%1").arg(systemAdvPara.maxSpeed[i]));
            macStepLenEdit[i]->setText(QString("%1").arg(systemAdvPara.macStepLength[i],0,'f',5));

            VL_Home_Edit[i]->setText(QString("%1").arg(systemAdvPara.VL_Home[i]));
            VH_Hm_Edit[i]->setText(QString("%1").arg(systemAdvPara.VH_hm[i]));
            TAcc_Edit[i]->setText(QString("%1").arg(systemAdvPara.TAcc[i]));
        }
        else if(i<6)
        {
            OutPulseFormComBox[i]->setCurrentIndex(systemAdvPara.Out_pulse_form[i+1]);
            VLPPS_Edit[i]->setText(QString("%1").arg(systemAdvPara.VLPPS[i+1]));
            VHPPS_Edit[i]->setText(QString("%1").arg(systemAdvPara.VH[i+1]));
            Tacc_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tacc[i+1]));
            Tdec_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tdec[i+1]));
            Tsacc_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tsacc[i+1]));
            Tsdec_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tsdec[i+1]));

            PulsePerComBox[i]->setCurrentIndex(systemAdvPara.PULSE_Per[i+1]);
            maxSpeedEdit[i]->setText(QString("%1").arg(systemAdvPara.maxSpeed[i+1]));
            macStepLenEdit[i]->setText(QString("%1").arg(systemAdvPara.macStepLength[i+1],0,'f',5));

            VL_Home_Edit[i]->setText(QString("%1").arg(systemAdvPara.VL_Home[i+1]));
            VH_Hm_Edit[i]->setText(QString("%1").arg(systemAdvPara.VH_hm[i+1]));
            TAcc_Edit[i]->setText(QString("%1").arg(systemAdvPara.TAcc[i+1]));
        }
        else if(i==6)
        {
            OutPulseFormComBox[i]->setCurrentIndex(systemAdvPara.Out_pulse_form[4]);
            VLPPS_Edit[i]->setText(QString("%1").arg(systemAdvPara.VLPPS[4]));
            VHPPS_Edit[i]->setText(QString("%1").arg(systemAdvPara.VH[4]));
            Tacc_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tacc[4]));
            Tdec_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tdec[4]));
            Tsacc_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tsacc[4]));
            Tsdec_Edit[i]->setText(QString("%1").arg(systemAdvPara.Tsdec[4]));

            PulsePerComBox[i]->setCurrentIndex(systemAdvPara.PULSE_Per[4]);
            maxSpeedEdit[i]->setText(QString("%1").arg(systemAdvPara.maxSpeed[4]));
            macStepLenEdit[i]->setText(QString("%1").arg(systemAdvPara.macStepLength[4],0,'f',5));

            VL_Home_Edit[i]->setText(QString("%1").arg(systemAdvPara.VL_Home[4]));
            VH_Hm_Edit[i]->setText(QString("%1").arg(systemAdvPara.VH_hm[4]));
            TAcc_Edit[i]->setText(QString("%1").arg(systemAdvPara.TAcc[4]));
        }
    }
    typeComboBox->setCurrentIndex(systemAdvPara.macType);
    okBtn->setFocus(Qt::OtherFocusReason);

    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void SetSystemAdvDlg::OnIOSetBtnClicked()
{
    SetIoDlg *dlg=new SetIoDlg(this);
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
}

void SetSystemAdvDlg::OnSpeedSetBtnClicked()
{
    SetSpeedDlg *dlg=new SetSpeedDlg(this);
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
}

void SetSystemAdvDlg::onOkBtnClicked()
{
    if(QFile::exists("robota.xml"))
    {
        QDir dir;
        dir.remove("robota.xml");
    }
    SYSTEM_ADV_PARAMS systemAdvPara;
    systemAdvPara.maxPusle=maxPulseEdit->text().trimmed().toInt();
    systemAdvPara.stopStart=fastStopChk->isChecked();
    for(int i=0;i<8;i++)
    {
        if((i<4) || (i==7))
        {
            systemAdvPara.Out_pulse_form[i]=OutPulseFormComBox[i]->currentIndex();
            systemAdvPara.VLPPS[i]=VLPPS_Edit[i]->text().trimmed().toInt();
            systemAdvPara.VH[i]=VHPPS_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tacc[i]=Tacc_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tdec[i]=Tdec_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tsacc[i]=Tsacc_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tsdec[i]=Tsdec_Edit[i]->text().trimmed().toInt();

            systemAdvPara.PULSE_Per[i]=PulsePerComBox[i]->currentIndex();
            systemAdvPara.maxSpeed[i]=maxSpeedEdit[i]->text().trimmed().toInt();
            systemAdvPara.macStepLength[i]=macStepLenEdit[i]->text().trimmed().toDouble();

            systemAdvPara.VL_Home[i]=VL_Home_Edit[i]->text().trimmed().toInt();
            systemAdvPara.VH_hm[i]=VH_Hm_Edit[i]->text().trimmed().toInt();
            systemAdvPara.TAcc[i]=TAcc_Edit[i]->text().trimmed().toInt();
        }
        else if(i<6)
        {
            systemAdvPara.Out_pulse_form[i+1]=OutPulseFormComBox[i]->currentIndex();
            systemAdvPara.VLPPS[i+1]=VLPPS_Edit[i]->text().trimmed().toInt();
            systemAdvPara.VH[i+1]=VHPPS_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tacc[i+1]=Tacc_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tdec[i+1]=Tdec_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tsacc[i+1]=Tsacc_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tsdec[i+1]=Tsdec_Edit[i]->text().trimmed().toInt();

            systemAdvPara.PULSE_Per[i+1]=PulsePerComBox[i]->currentIndex();
            systemAdvPara.maxSpeed[i+1]=maxSpeedEdit[i]->text().trimmed().toInt();
            systemAdvPara.macStepLength[i+1]=macStepLenEdit[i]->text().trimmed().toDouble();

            systemAdvPara.VL_Home[i+1]=VL_Home_Edit[i]->text().trimmed().toInt();
            systemAdvPara.VH_hm[i+1]=VH_Hm_Edit[i]->text().trimmed().toInt();
            systemAdvPara.TAcc[i+1]=TAcc_Edit[i]->text().trimmed().toInt();
        }
        else if(i==6)
        {
            systemAdvPara.Out_pulse_form[4]=OutPulseFormComBox[i]->currentIndex();
            systemAdvPara.VLPPS[4]=VLPPS_Edit[i]->text().trimmed().toInt();
            systemAdvPara.VH[4]=VHPPS_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tacc[4]=Tacc_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tdec[4]=Tdec_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tsacc[4]=Tsacc_Edit[i]->text().trimmed().toInt();
            systemAdvPara.Tsdec[4]=Tsdec_Edit[i]->text().trimmed().toInt();

            systemAdvPara.PULSE_Per[4]=PulsePerComBox[i]->currentIndex();
            systemAdvPara.maxSpeed[4]=maxSpeedEdit[i]->text().trimmed().toInt();
            systemAdvPara.macStepLength[4]=macStepLenEdit[i]->text().trimmed().toDouble();

            systemAdvPara.VL_Home[4]=VL_Home_Edit[i]->text().trimmed().toInt();
            systemAdvPara.VH_hm[4]=VH_Hm_Edit[i]->text().trimmed().toInt();
            systemAdvPara.TAcc[4]=TAcc_Edit[i]->text().trimmed().toInt();
        }
    }
    systemAdvPara.macType=typeComboBox->currentIndex();

    MyRWxml xml;
    xml.writeSysAdvPara("robota.xml",systemAdvPara);
    done(QDialog::Accepted);
}

void SetSystemAdvDlg::OnCancelClicked()
{
    done(QDialog::Rejected);
}

void SetSystemAdvDlg::loadMyLineEdit()      //加载自定义文本框
{
    maxPulseEdit->context=context;
    QRect rect=maxPulseEdit->geometry();
    maxPulseEdit->myPos=QPoint(rect.left()-100,rect.top());
    maxPulseEdit->parentHeight=geometry().height();

    for(int i=0;i<8;i++)
    {
        VLPPS_Edit[i]->context=context;
        QRect rect=VLPPS_Edit[i]->geometry();
        VLPPS_Edit[i]->myPos=QPoint(rect.left()-100,rect.top());
        VLPPS_Edit[i]->parentHeight=geometry().height()+60;

        VHPPS_Edit[i]->context=context;
        rect=VHPPS_Edit[i]->geometry();
        VHPPS_Edit[i]->myPos=QPoint(rect.left()-100,rect.top());
        VHPPS_Edit[i]->parentHeight=geometry().height()+60;

        Tacc_Edit[i]->context=context;
        rect=Tacc_Edit[i]->geometry();
        Tacc_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        Tacc_Edit[i]->parentHeight=geometry().height()+60;

        Tdec_Edit[i]->context=context;
        rect=Tdec_Edit[i]->geometry();
        Tdec_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        Tdec_Edit[i]->parentHeight=geometry().height()+60;

        Tsacc_Edit[i]->context=context;
        rect=Tsacc_Edit[i]->geometry();
        Tsacc_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        Tsacc_Edit[i]->parentHeight=geometry().height()+60;

        Tsdec_Edit[i]->context=context;
        rect=Tsdec_Edit[i]->geometry();
        Tsdec_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        Tsdec_Edit[i]->parentHeight=geometry().height()+60;

        maxSpeedEdit[i]->context=context;
        rect=maxSpeedEdit[i]->geometry();
        maxSpeedEdit[i]->myPos=QPoint(rect.left()+50,rect.top());
        maxSpeedEdit[i]->parentHeight=geometry().height()+40;

        macStepLenEdit[i]->context=context;
        rect=macStepLenEdit[i]->geometry();
        macStepLenEdit[i]->myPos=QPoint(rect.left()+50,rect.top());
        macStepLenEdit[i]->parentHeight=geometry().height()+40;

        VL_Home_Edit[i]->context=context;
        rect=VL_Home_Edit[i]->geometry();
        VL_Home_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        VL_Home_Edit[i]->parentHeight=geometry().height()+60;

        VH_Hm_Edit[i]->context=context;
        rect=VH_Hm_Edit[i]->geometry();
        VH_Hm_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        VH_Hm_Edit[i]->parentHeight=geometry().height()+60;

        TAcc_Edit[i]->context=context;
        rect=TAcc_Edit[i]->geometry();
        TAcc_Edit[i]->myPos=QPoint(rect.left()+50,rect.top());
        TAcc_Edit[i]->parentHeight=geometry().height()+60;
    }
}
