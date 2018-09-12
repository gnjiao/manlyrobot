#include "envparadlg.h"
#include "global.h"
#include "msqldb.h"

EnvParaDlg::EnvParaDlg(QWidget *parent) :
    QDialog(parent)
{
    type=0;
    technicsType=0;

    dlgWidth=600;
    dlgHeight=400;
}
void EnvParaDlg::initDialog()
{
    //焊盘
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
    if(technicsType==0)
        envPara.LaoTie_ZL=tr("08D");//种类
    else
        envPara.LaoTie_ZL=tr("20R");//种类


    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QGroupBox* hanpanGroup=new QGroupBox(tr("焊盘参数"));
    hanpanGroup->setFont(ft);
    QLabel *Pan_HDLabel=new QLabel(tr("厚度"));
    Pan_HDLabel->setFont(ft);
    hanpan_HD_Box=new QComboBox;
    hanpan_HD_Box->setFont(ft);
    //hanpan_HD_Box->addItem(QString("%1").arg(envPara.HanPan_HD,0,'f',0));
    hanpan_HD_Box->addItem("35");
    hanpan_HD_Box->addItem("70");
    QHBoxLayout* Pan_HDLayout=new QHBoxLayout;
    Pan_HDLayout->addWidget(Pan_HDLabel);
    Pan_HDLayout->addWidget(hanpan_HD_Box);

    QLabel *Pan_BMCLLabel=new QLabel(tr("表面处理"));
    Pan_BMCLLabel->setFont(ft);
    hanpan_BMCL_Box=new QComboBox;
    hanpan_BMCL_Box->setFont(ft);
    hanpan_BMCL_Box->addItem(tr("镀锡"));
    hanpan_BMCL_Box->addItem(tr("镀金"));
    hanpan_BMCL_Box->addItem(tr("镀银"));
    QHBoxLayout* Pan_BMCLLayout=new QHBoxLayout;
    Pan_BMCLLayout->addWidget(Pan_BMCLLabel);
    Pan_BMCLLayout->addWidget(hanpan_BMCL_Box);

    QLabel *Pan_FTXSLabel=new QLabel(tr("覆铜形式"));
    Pan_FTXSLabel->setFont(ft);
    hanpan_FTXS_Box=new QComboBox;
    hanpan_FTXS_Box->setFont(ft);
    hanpan_FTXS_Box->addItem(tr("无"));
    hanpan_FTXS_Box->addItem(tr("十字"));
    hanpan_FTXS_Box->addItem(tr("一字"));
    hanpan_FTXS_Box->addItem(tr("全覆铜"));
    QHBoxLayout* Pan_FTXSLayout=new QHBoxLayout;
    Pan_FTXSLayout->addWidget(Pan_FTXSLabel);
    Pan_FTXSLayout->addWidget(hanpan_FTXS_Box);

    QLabel *Pan_ZJLabel=new QLabel(tr("直径"));
    Pan_ZJLabel->setFont(ft);
    hanpan_ZJ_Edit=new MyLineEdit;
    hanpan_ZJ_Edit->setFont(ft);
    QHBoxLayout* Pan_ZJLayout=new QHBoxLayout;
    Pan_ZJLayout->addWidget(Pan_ZJLabel);
    Pan_ZJLayout->addWidget(hanpan_ZJ_Edit);

    QLabel *Pan_KJLabel=new QLabel(tr("孔径"));
    Pan_KJLabel->setFont(ft);
    hanpan_KJ_Edit=new MyLineEdit;
    hanpan_KJ_Edit->setFont(ft);
    QHBoxLayout* Pan_KJLayout=new QHBoxLayout;
    Pan_KJLayout->addWidget(Pan_KJLabel);
    Pan_KJLayout->addWidget(hanpan_KJ_Edit);

    QVBoxLayout* hanpanLayout=new QVBoxLayout;
    hanpanLayout->addLayout(Pan_HDLayout);
    hanpanLayout->addLayout(Pan_BMCLLayout);
    hanpanLayout->addLayout(Pan_FTXSLayout);
    hanpanLayout->addLayout(Pan_ZJLayout);
    hanpanLayout->addLayout(Pan_KJLayout);
    hanpanGroup->setLayout(hanpanLayout);

    QGroupBox* hanxisiGroup=new QGroupBox(tr("焊锡丝参数"));
    hanxisiGroup->setFont(ft);
    QLabel *hanxisi_ZJLabel=new QLabel(tr("直径"));
    hanxisi_ZJLabel->setFont(ft);
    hanxisi_ZJ_Box=new QComboBox;
    hanxisi_ZJ_Box->setFont(ft);
    hanxisi_ZJ_Box->addItem("0.3");
    hanxisi_ZJ_Box->addItem("0.5");
    hanxisi_ZJ_Box->addItem("0.6");
    hanxisi_ZJ_Box->addItem("0.8");
    hanxisi_ZJ_Box->addItem("1.0");
    hanxisi_ZJ_Box->addItem("1.2");
    QHBoxLayout* hanxisi_ZJLayout=new QHBoxLayout;
    hanxisi_ZJLayout->addWidget(hanxisi_ZJLabel);
    hanxisi_ZJLayout->addWidget(hanxisi_ZJ_Box);

    QLabel *hanxisi_ZLLabel=new QLabel(tr("种类"));
    hanxisi_ZLLabel->setFont(ft);
    hanxisi_ZL_Box=new QComboBox;
    hanxisi_ZL_Box->setFont(ft);
    hanxisi_ZL_Box->addItem(tr("镍锡"));
    hanxisi_ZL_Box->addItem(tr("铜锡"));
    QHBoxLayout* hanxisi_ZLLayout=new QHBoxLayout;
    hanxisi_ZLLayout->addWidget(hanxisi_ZLLabel);
    hanxisi_ZLLayout->addWidget(hanxisi_ZL_Box);

    QLabel *hanxisi_ZHJLabel=new QLabel(tr("助焊剂含量"));
    hanxisi_ZHJLabel->setFont(ft);
    hanxisi_ZHJ_Box=new QComboBox;
    hanxisi_ZHJ_Box->setFont(ft);
    hanxisi_ZHJ_Box->addItem("2");
    hanxisi_ZHJ_Box->addItem("2.5");
    hanxisi_ZHJ_Box->addItem("2.8");
    hanxisi_ZHJ_Box->addItem("3");
    hanxisi_ZHJ_Box->addItem("3.2");
    hanxisi_ZHJ_Box->addItem("3.5");
    QHBoxLayout* hanxisi_ZHJLayout=new QHBoxLayout;
    hanxisi_ZHJLayout->addWidget(hanxisi_ZHJLabel);
    hanxisi_ZHJLayout->addWidget(hanxisi_ZHJ_Box);

    QLabel *hanxisi_FWLabel=new QLabel(tr("方位"));
    hanxisi_FWLabel->setFont(ft);
    hanxisi_FW_Box=new QComboBox;
    hanxisi_FW_Box->setFont(ft);
    hanxisi_FW_Box->addItem(tr("前上"));
    hanxisi_FW_Box->addItem(tr("前中"));
    hanxisi_FW_Box->addItem(tr("前下"));
    hanxisi_FW_Box->addItem(tr("中上"));
    hanxisi_FW_Box->addItem(tr("中中"));
    hanxisi_FW_Box->addItem(tr("中下"));
    hanxisi_FW_Box->addItem(tr("后上"));
    hanxisi_FW_Box->addItem(tr("后中"));
    hanxisi_FW_Box->addItem(tr("后下"));
    QHBoxLayout* hanxisi_FWLayout=new QHBoxLayout;
    hanxisi_FWLayout->addWidget(hanxisi_FWLabel);
    hanxisi_FWLayout->addWidget(hanxisi_FW_Box);

    QVBoxLayout* hanxisiLayout=new QVBoxLayout;
    hanxisiLayout->addLayout(hanxisi_ZJLayout);
    hanxisiLayout->addLayout(hanxisi_ZLLayout);
    hanxisiLayout->addLayout(hanxisi_ZHJLayout);
    hanxisiLayout->addLayout(hanxisi_FWLayout);
    hanxisiGroup->setLayout(hanxisiLayout);

    QGroupBox* laotieGroup=new QGroupBox(tr("烙铁头参数"));
    hanxisiGroup->setFont(ft);
    QLabel *laotie_WDLabel=new QLabel(tr("温度"));
    laotie_WDLabel->setFont(ft);
    laotie_WD_Edit=new MyLineEdit;
    laotie_WD_Edit->setFont(ft);
    QHBoxLayout* laotie_WDLayout=new QHBoxLayout;
    laotie_WDLayout->addWidget(laotie_WDLabel);
    laotie_WDLayout->addWidget(laotie_WD_Edit);

    QLabel *laotie_ZLLabel=new QLabel(tr("种类"));
    laotie_ZLLabel->setFont(ft);
    laotie_ZL_Box=new QComboBox;
    laotie_ZL_Box->setFont(ft);
    if(technicsType==0)
    {
        laotie_ZL_Box->addItem("08D");
        laotie_ZL_Box->addItem("10D");
        laotie_ZL_Box->addItem("13D");
        laotie_ZL_Box->addItem("16D");
        laotie_ZL_Box->addItem("20D");
        laotie_ZL_Box->addItem("24D");
        laotie_ZL_Box->addItem("30D");
        laotie_ZL_Box->addItem("40D");
        laotie_ZL_Box->addItem("50D");
        laotie_ZL_Box->addItem("60D");
        laotie_ZL_Box->addItem("80D");
        laotie_ZL_Box->addItem("08D-2");
        laotie_ZL_Box->addItem("10D-2");
        laotie_ZL_Box->addItem("13D-2");
        laotie_ZL_Box->addItem("16D-2");
        laotie_ZL_Box->addItem("20D-2");
        laotie_ZL_Box->addItem("24D-2");
        laotie_ZL_Box->addItem("30D-2");
        laotie_ZL_Box->addItem("40D-2");
        laotie_ZL_Box->addItem("50D-2");
        laotie_ZL_Box->addItem("60D-2");
        laotie_ZL_Box->addItem("80D-2");
        laotie_ZL_Box->addItem("13DV2");
        laotie_ZL_Box->addItem("16DV2");
        laotie_ZL_Box->addItem("20DV2");
        laotie_ZL_Box->addItem("24DV2");
        laotie_ZL_Box->addItem("30DV2");
        laotie_ZL_Box->addItem("40DV2");
        laotie_ZL_Box->addItem("50DV2");
        laotie_ZL_Box->addItem("60DV2");
        laotie_ZL_Box->addItem("80DV2");
        laotie_ZL_Box->addItem("10DV1");
        laotie_ZL_Box->addItem("13DV1");
        laotie_ZL_Box->addItem("16DV1");
        laotie_ZL_Box->addItem("20DV1");
        laotie_ZL_Box->addItem("24DV1");
        laotie_ZL_Box->addItem("30DV1");
        laotie_ZL_Box->addItem("40DV1");
        laotie_ZL_Box->addItem("50DV1");
        laotie_ZL_Box->addItem("60DV1");
        laotie_ZL_Box->addItem("10DV1-2");
        laotie_ZL_Box->addItem("13DV1-2");
        laotie_ZL_Box->addItem("16DV1-2");
        laotie_ZL_Box->addItem("20DV1-2");
        laotie_ZL_Box->addItem("24DV1-2");
        laotie_ZL_Box->addItem("30DV1-2");
        laotie_ZL_Box->addItem("40DV1-2");
        laotie_ZL_Box->addItem("50DV1-2");
        laotie_ZL_Box->addItem("60DV1-2");
        laotie_ZL_Box->addItem("80DV1-2");
        laotie_ZL_Box->addItem("20BCV1");
        laotie_ZL_Box->addItem("24BCV1");
        laotie_ZL_Box->addItem("30BCV1");
        laotie_ZL_Box->addItem("40BCV1");
        laotie_ZL_Box->addItem("10PC");
        laotie_ZL_Box->addItem("13PC");
        laotie_ZL_Box->addItem("16PC");
        laotie_ZL_Box->addItem("20PC");
        laotie_ZL_Box->addItem("24PC");
        laotie_ZL_Box->addItem("30PC");
        laotie_ZL_Box->addItem("40PC");
        laotie_ZL_Box->addItem("50PC");
        laotie_ZL_Box->addItem("60PC");
        laotie_ZL_Box->addItem("80PC");
        laotie_ZL_Box->addItem("20PCV2");
        laotie_ZL_Box->addItem("24PCV2");
        laotie_ZL_Box->addItem("30PCV2");
        laotie_ZL_Box->addItem("40PCV2");
        laotie_ZL_Box->addItem("50PCV2");
    }
    else
    {
        laotie_ZL_Box->addItem("20R");
        laotie_ZL_Box->addItem("24R");
        laotie_ZL_Box->addItem("30R");
        laotie_ZL_Box->addItem("40R");
        laotie_ZL_Box->addItem("50R");
        laotie_ZL_Box->addItem("16FPR");
        laotie_ZL_Box->addItem("20FPR");
        laotie_ZL_Box->addItem("24FPR");
        laotie_ZL_Box->addItem("30FPR");
        laotie_ZL_Box->addItem("40FPR");
        laotie_ZL_Box->addItem("50FPR");
    }
    QHBoxLayout* laotie_ZLLayout=new QHBoxLayout;
    laotie_ZLLayout->addWidget(laotie_ZLLabel);
    laotie_ZLLayout->addWidget(laotie_ZL_Box);

    QVBoxLayout* laotieLayout=new QVBoxLayout;
    laotieLayout->addLayout(laotie_WDLayout);
    laotieLayout->addLayout(laotie_ZLLayout);
    laotieGroup->setLayout(laotieLayout);

    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelClicked()));
    QHBoxLayout* btnLayout=new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QVBoxLayout* layout=new QVBoxLayout;
    layout->addWidget(hanpanGroup);
    layout->addWidget(hanxisiGroup);
    layout->addWidget(laotieGroup);
    layout->addLayout(btnLayout);
    setLayout(layout);

    setWindowTitle(tr("工艺环境参数"));
    //resize(dlgWidth,dlgHeight);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));

    if(type != 2)
    {
        SqlDB db;
        if(db.openDB("robot.db"))
        {
            db.ReadEnvParaFromDb(paraName,envPara);
            db.closeDB();
        }
    }
    hanpan_HD_Box->setCurrentText(QString("%1").arg(envPara.HanPan_HD,0,'f',0));
    hanpan_BMCL_Box->setCurrentText(envPara.HanPan_BMCL);
    hanpan_FTXS_Box->setCurrentText(envPara.HanPan_FTXS);
    hanpan_ZJ_Edit->setText(QString("%1").arg(envPara.HanPan_ZJ,0,'f',1));
    hanpan_KJ_Edit->setText(QString("%1").arg(envPara.HanPan_KJ,0,'f',1));
    hanxisi_ZJ_Box->setCurrentText(QString("%1").arg(envPara.HanXiSi_ZJ,0,'f',1));
    hanxisi_ZL_Box->setCurrentText(envPara.HanXiSi_ZL);
    hanxisi_ZHJ_Box->setCurrentText(QString("%1").arg(envPara.HanXiSi_ZHJ,0,'f',1));
    hanxisi_FW_Box->setCurrentText(envPara.HanXiSi_FW);
    laotie_WD_Edit->setText(QString("%1").arg(envPara.LaoTie_WD,0,'f',1));
    laotie_ZL_Box->setCurrentText(envPara.LaoTie_ZL);
    okBtn->setFocus(Qt::OtherFocusReason);
    if(type<2)
    {
        hanpan_HD_Box->setEnabled(false);
        hanpan_BMCL_Box->setEnabled(false);
        hanpan_FTXS_Box->setEnabled(false);
        hanpan_ZJ_Edit->setEnabled(false);
        hanpan_KJ_Edit->setEnabled(false);
        hanxisi_ZJ_Box->setEnabled(false);
        hanxisi_ZL_Box->setEnabled(false);
        hanxisi_ZHJ_Box->setEnabled(false);
        hanxisi_FW_Box->setEnabled(false);
        laotie_WD_Edit->setEnabled(false);
        laotie_ZL_Box->setEnabled(false);
    }
    else
    {
        hanpan_HD_Box->setEnabled(true);
        hanpan_BMCL_Box->setEnabled(true);
        hanpan_FTXS_Box->setEnabled(true);
        hanpan_ZJ_Edit->setEnabled(true);
        hanpan_KJ_Edit->setEnabled(true);
        hanxisi_ZJ_Box->setEnabled(true);
        hanxisi_ZL_Box->setEnabled(true);
        hanxisi_ZHJ_Box->setEnabled(true);
        hanxisi_FW_Box->setEnabled(true);
        laotie_WD_Edit->setEnabled(true);
        laotie_ZL_Box->setEnabled(true);
    }
}

void EnvParaDlg::onOkBtnClicked()
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

    if(type>1)
    {
        envPara.HanPan_HD=hanpan_HD_Box->currentText().toDouble();
        envPara.HanPan_BMCL=hanpan_BMCL_Box->currentText();
        envPara.HanPan_FTXS=hanpan_FTXS_Box->currentText();
        envPara.HanPan_ZJ=hanpan_ZJ_Edit->text().toDouble();
        envPara.HanPan_KJ=hanpan_KJ_Edit->text().toDouble();
        envPara.HanXiSi_ZJ=hanxisi_ZJ_Box->currentText().toDouble();
        envPara.HanXiSi_ZL=hanxisi_ZL_Box->currentText();
        envPara.HanXiSi_ZHJ=hanxisi_ZHJ_Box->currentText().toDouble();
        envPara.HanXiSi_FW=hanxisi_FW_Box->currentText();
        envPara.LaoTie_WD=laotie_WD_Edit->text().toDouble();
        envPara.LaoTie_ZL=laotie_ZL_Box->currentText();

        SqlDB db;
        if(!db.openDB("robot.db"))
        {
            msgBox.setText(tr("打开数据库失败."));
            msgBox.exec();
            return;
        }
        else
        {
            if(!db.SaveEnvParaToDb(paraName,envPara))
            {
                msgBox.setText(tr("保存数据失败."));
                msgBox.exec();
                return;
            }
        }
    }

    done(QDialog::Accepted);
}
void EnvParaDlg::OnCancelClicked()
{
    done(QDialog::Rejected);
}

void EnvParaDlg::loadMyLineEdit()      //加载自定义文本框
{
    hanpan_ZJ_Edit->context=context;
    QRect rect=hanpan_ZJ_Edit->geometry();
    hanpan_ZJ_Edit->myPos=QPoint(rect.left()-150,rect.top()+40);
    hanpan_ZJ_Edit->parentHeight=geometry().height()+60;

    hanpan_KJ_Edit->context=context;
    rect=hanpan_KJ_Edit->geometry();
    hanpan_KJ_Edit->myPos=QPoint(rect.left()-150,rect.top()+30);
    hanpan_KJ_Edit->parentHeight=geometry().height()+60;

    laotie_WD_Edit->context=context;
    rect=laotie_WD_Edit->geometry();
    laotie_WD_Edit->myPos=QPoint(rect.left()-150,rect.top()+130);
    laotie_WD_Edit->parentHeight=geometry().height();
}
