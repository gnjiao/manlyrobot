#include "global.h"
#include "mainwidget.h"
#include "logindialog.h"
#include "msqldb.h"
#include "userwidget.h"
#include "myrwxml.h"
#include "setsystemdlg.h"
#include "projmanageDlg.h"
#include "windows.h"
#include "diskid32.h"
#include "movekeydlg.h"

#define SERIAL_NUM_KEY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\windows\\CurrentVersion\\BITS"
#define MODEL_NUM_KEY "HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\windows\\CurrentVersion\\Explorer"

int g_userRight;
QString g_userName;
S_P g_PointWeldPara;
L_P g_LineWeldPara;
IO_STATUS g_ioStatus;
IO_SET  g_IOxb;   //开关量配置的下标
unsigned char  g_sysInput[24];  //给系统配置的开关量输入
unsigned char  g_cusInput[24];  //给客户配置的开关量输入
unsigned char  g_sysGpo[8];     //给系统配置的开关量输出反馈
unsigned char  g_cusGpo[8];     //给客户配置的开关量输出反馈
unsigned char  g_MoveKeyPressed;   //movetool键盘按下，1=按下，0=未按下
unsigned char  g_MacType;   //机器类型：0=台式4轴单平台； 1=6轴在线式(Y,R反转)； 2=台式4轴双平台（Y1,Y2）;3=台式6轴单平台；4=台式6轴双平台
int g_moveMod;  //移动方式，0=点动，1=连续移动
int g_setpLen;  //点动步长,0=0.01mm,1=0.1mm,2=1mm,3=10mm
int g_lineWeldType; //拖焊类型：0=普通拖焊；1=多段拖焊首段；2=多段拖焊中段；3=多段拖焊末段；
int g_currOperatePage;     //当前操作页，1=自动操作页，2=编辑页
int g_autoTempCtrlState;     //自动温控检查框的状态,0=手动温控 ; 1=自动温控
/*
QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(CKeyHook,"KeyHook");  //这个宏用声明接口
QT_END_NAMESPACE
*/
MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    g_MacType=0;
    g_lineWeldType=0;
    g_autoTempCtrlState=1;
    g_NumLockSt=0;

    keySelFlag1=0;
    keySelFlag2=0;
    keySelPressed1=false;
    keySelPressed2=false;
}

MainWidget::~MainWidget()
{

}
void MainWidget::initWidget()
{
    printf("***********************Application start run **********************\n");
    g_moveMod=1;
    g_setpLen=0;
    robotThread=NULL;
    robotThread2=NULL;
    robotThread3=NULL;
    ////////////////////////////////////////////////////////
    if(!isRightUser())
    {
        QFont ft;
        ft.setPointSize(MESSAGEBOX_FONT+10);
        ft.setItalic(false);
        QLabel *label=new QLabel(tr("您可能是盗版用户或者软件尚未注册，请进行正版注册后再使用！"));
        label->setFont(ft);
        QPushButton *closeBtn=new QPushButton(tr("退出"),this);
        closeBtn->setFont(ft);
        connect(closeBtn, SIGNAL(clicked()), this, SLOT(OnCloseBtnClicked()));
        QVBoxLayout *closeLayout = new QVBoxLayout;
        closeLayout->addWidget(label);
        closeLayout->addSpacing(50);
        closeLayout->addWidget(closeBtn);
        setLayout(closeLayout);
        return;
    }
    ///////////////////////////移动键盘/////////////////////////////
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_1()), this, SLOT(pressKey_1_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_1()), this, SLOT(release_key_1_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_2()), this, SLOT(pressKey_2_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_2()), this, SLOT(release_key_2_slot()));

    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_3()), this, SLOT(pressKey_3_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_3()), this, SLOT(release_key_3_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_4()), this, SLOT(pressKey_4_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_4()), this, SLOT(release_key_4_slot()));

    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_5()), this, SLOT(pressKey_5_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_5()), this, SLOT(release_key_5_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_6()), this, SLOT(pressKey_6_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_6()), this, SLOT(release_key_6_slot()));

    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_7()), this, SLOT(pressKey_7_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_7()), this, SLOT(release_key_7_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_8()), this, SLOT(pressKey_8_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_8()), this, SLOT(release_key_8_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(press_key_9()), this, SLOT(pressKey_9_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_9()), this, SLOT(release_key_9_slot()));
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_80()), this, SLOT(release_key_80_slot()));//"+"
    connect(nativeEventFilter->getKeyboardSignal(),SIGNAL(release_key_NumLock()), this, SLOT(release_key_NumLock_slot()));
    ////////////////////////////////////////////////////////
    g_currOperatePage=1;
    for(int i=0;i<24;i++)
    {
        g_sysInput[i]=0;
        g_cusInput[i]=0;
    }
    for(int i=0;i<8;i++)
    {
        g_sysGpo[i]=0;
        g_cusGpo[i]=0;
    }
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    //this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    resize(screenRect.width(),screenRect.height());
    //resize(1024,768);
    setWindowTitle(tr("迈力焊锡机器人"));
    g_userRight=USER_RIGHT_OPERATOR;

    MyRWxml mXml;
    if(!QFile::exists("robot.xml"))
        mXml.createXmlFile("robot.xml");
    if(!QFile::exists("robota.xml"))
        mXml.createAXmlFile("robota.xml");
    if(!QFile::exists("robotio.xml"))
        mXml.createIoXmlFile("robotio.xml");
    if(!QFile::exists("robotspeed.xml"))
        mXml.createSpeedXmlFile("robotspeed.xml");

    mXml.readIoPara("robotio.xml",g_IOxb);

    SYSTEM_ADV_PARAMS systemAdvPara;
    mXml.readSysAdvPara("robota.xml",systemAdvPara);
    if((systemAdvPara.macType>-1) && (systemAdvPara.macType<20))
        g_MacType=systemAdvPara.macType;
    else
        g_MacType=0;

    createStatusBox();
    createButtonBox();

    pagesWidget = new QStackedWidget;
    operatePage = new OperatePage;
    operatePage->initWidget();
    editParaPage = new EditParaPage;
    editParaPage->initWidget();
    //moveToolPage = new MoveToolPage;
    //moveToolPage->initWidget();
    pagesWidget->setMinimumHeight(screenRect.height()*5/8);
    pagesWidget->addWidget(operatePage);
    pagesWidget->addWidget(editParaPage);
    //pagesWidget->addWidget(moveToolPage);
    pagesWidget->setCurrentIndex(0);
    pagesWidget->show();
    QPixmap img1(":/light/res/light1.png");
    QPixmap img2(":/light/res/light2.png");
    operateLight->setPixmap(img2);
    editLight->setPixmap(img1);
    moveToolLight->setPixmap(img1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(statusBox);
    layout->addWidget(pagesWidget);
    layout->addWidget(buttonBox);
    layout->setContentsMargins(0,0,0,0);
    //layout->addSpacing(2);
    setLayout(layout);
    setWindowState(Qt::WindowFullScreen);
    //resize(1024,768);
    setContentsMargins(0,0,0,0);

    QTimer *timer1=new QTimer(this);//用于时间显示
    timer1->start(1000);
    connect(timer1, SIGNAL(timeout()), this, SLOT(time_slot()));

    //加载机器人控制器，即类ManlyRobot，初始化机器人
    robotThread=new RobotThread(this);
    connect(robotThread, &RobotThread::robotCreated, this, &MainWidget::getRobot);//SIGNAL(robotCreated()()) SLOT(getRobot())
    connect(robotThread, SIGNAL(finished()), robotThread, SLOT(terminate()));
    robotThread->start();

    SqlDB db;
    if(db.openDB("robot.db"))
        db.closeDB();
}

bool MainWidget::isRightUser()
{
    diskInfo info=getDiskInfo();
    QString thisSerialNumber=QString(info.serialNumber);//读本机的序列号
    QString thisModelNumber=QString(info.modelNumber);//读本机的model号
    thisSerialNumber=strtomd5(thisSerialNumber);
    thisModelNumber=strtomd5(thisModelNumber);

    QSettings settings(SERIAL_NUM_KEY,QSettings::NativeFormat);
    QString rightSerialNumber = settings.value("msflag", "error").toString();//读正确的序列号
    QSettings settings2(MODEL_NUM_KEY,QSettings::NativeFormat);
    QString rightModelNumber = settings2.value("explorerFlag", "error").toString();//读正确的model号
    bool result=false;
    if((thisSerialNumber==rightSerialNumber) && (thisModelNumber==rightModelNumber))
        result=true;
    else
        result=false;
    return result;
}

QString MainWidget::strtomd5(QString str)
{
    QByteArray byte_array=str.toLatin1();
    QByteArray hash_byte_array = QCryptographicHash::hash(byte_array, QCryptographicHash::Md5);
    return hash_byte_array.toHex();
}

void MainWidget::time_slot()
{
    dateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    if(g_autoTempCtrlState==1)
    {
        if(operatePage->weldProject)
        {
            if(operatePage->weldProject->serialPort)
            {
                operatePage->currentTemp->setText(QString("%1").arg(operatePage->weldProject->serialPort->getCurrentTemp()));
            }
        }
        if(editParaPage->weldProject)
        {
            if(editParaPage->weldProject->serialPort)
            {
                editParaPage->currentTemp->setText(QString("%1").arg(editParaPage->weldProject->serialPort->getCurrentTemp()));
            }
        }
    }
}

void MainWidget::createStatusBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(MAIN_STATUS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    ft.setBold(true);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    QFile file("robotrun");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);    // read the data serialized from the file
    in >> g_userName >> g_userRight;           // extract "the answer is" and 42
    file.close();
    g_userRight=USER_RIGHT_OPERATOR;

    statusBox=new QGroupBox;
    statusBox->setMaximumHeight(screenRect.height()/10);
    redayLight=new QLabel;  //指示灯
    QPixmap img1(":/light/res/light1.png");
    QPixmap img2(":/light/res/light2.png");
    redayLight->setPixmap(img1);
    QLabel *redayLabel=new QLabel(tr("准备就绪"));
    redayLight->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    redayLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    redayLabel->setFont(ft);
    operateLight=new QLabel;  //指示灯
    operateLight->setPixmap(img1);
    QLabel *operateLabel=new QLabel(tr("自动操作"));
    operateLight->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    operateLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    operateLabel->setFont(ft);
    editLight=new QLabel;  //指示灯
    //QPixmap img(":/light/res/light1.png");
    editLight->setPixmap(img1);
    QLabel *editLabel=new QLabel(tr("编辑参数"));
    editLabel->setFont(ft);
    editLight->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    editLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    moveToolLight=new QLabel;  //指示灯
    //QPixmap img(":/light/res/light1.png");
    moveToolLight->setPixmap(img1);
    QLabel *moveToolLabel=new QLabel(tr("项目管理"));
    moveToolLabel->setFont(ft);
    moveToolLight->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    moveToolLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QLabel *curUserLabel=new QLabel(tr("当前用户:"));
    curUserLabel->setFont(ft);
    //curUserLabel->setMinimumWidth(screenRect.width()/10);
    curUserLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    currentUserLabel=new QLabel;  //当前用户名
    currentUserLabel->setFont(ft);
    currentUserLabel->setPalette(palette);
    currentUserLabel->setText(g_userName);
    //currentUserLabel->setMinimumWidth(screenRect.width()/6);
    currentUserLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    QLabel *curRightLabel=new QLabel(tr("用户级别:"));
    curRightLabel->setFont(ft);
    //curRightLabel->setMinimumWidth(screenRect.width()/10);
    curRightLabel->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    QString right;
    if(g_userRight==USER_RIGHT_VENDER)
        right=QString(tr("厂家级"));
    else if(g_userRight==USER_RIGHT_ADMIN)
        right=QString(tr("管理员"));
    else if(g_userRight==USER_RIGHT_DESIGNER)
        right=QString(tr("设计师"));
    else
        right=QString(tr("操作员"));
    currentRightLabel=new QLabel;  //当前用户权限
    currentRightLabel->setFont(ft);
    currentRightLabel->setPalette(palette);
    currentRightLabel->setText(right);
    //currentRightLabel->setMinimumWidth(screenRect.width()/6);
    currentRightLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

    softModeLabel=new QLabel;  //操作模式
    softModeLabel->setFont(ft);
    softModeLabel->setText(tr("按项目自动焊接操作"));
    //softModeLabel->setPalette(palette);
    softModeLabel->setMinimumWidth(screenRect.width()/8);
    softModeLabel->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);
    dateTimeLabel=new QLabel;  //日期时间
    dateTimeLabel->setFont(ft);
    dateTimeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    dateTimeLabel->setMinimumWidth(screenRect.width()/8);
    dateTimeLabel->setAlignment(Qt::AlignCenter|Qt::AlignVCenter);

    QHBoxLayout *curUserLayout = new QHBoxLayout;  //当前用户名
    curUserLayout->addWidget(curUserLabel);
    curUserLayout->addWidget(currentUserLabel);

    QHBoxLayout *curRightLayout = new QHBoxLayout;  //当前用户权限
    curRightLayout->addWidget(curRightLabel);
    curRightLayout->addWidget(currentRightLabel);

    QHBoxLayout *redayLayout = new QHBoxLayout;
    redayLayout->addWidget(redayLight);
    redayLayout->addWidget(redayLabel);
    QHBoxLayout *operateLayout = new QHBoxLayout;
    operateLayout->addWidget(operateLight);
    operateLayout->addWidget(operateLabel);
    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(editLight);
    editLayout->addWidget(editLabel);
    QHBoxLayout *moveToolLayout = new QHBoxLayout;
    moveToolLayout->addWidget(moveToolLight);
    moveToolLayout->addWidget(moveToolLabel);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(redayLayout);
    layout->addLayout(operateLayout);
    layout->addLayout(editLayout);
    layout->addLayout(moveToolLayout);
    layout->addSpacing(screenRect.width()/40);
    layout->addLayout(curUserLayout);
    layout->addSpacing(screenRect.width()/100);
    layout->addLayout(curRightLayout);
    layout->addSpacing(screenRect.width()/40);
    layout->addWidget(softModeLabel);
    layout->addWidget(dateTimeLabel);
    layout->addSpacing(10);
    layout->setContentsMargins(0,0,0,0);
    statusBox->setLayout(layout);
    statusBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(150, 150, 150, 255), stop:1 rgba(200, 200, 200, 255)); border:none}");
    statusBox->setContentsMargins(0,0,0,0);
    statusBox->setFlat(true);
    //statusBox ->setStyleSheet("background-image:url(./backgrandbar.png)");
}
/*
void MainWidget::createOperatorBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(20,95,120));
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    operatorBox=new QGroupBox;
    operatorBox->setMinimumHeight(screenRect.height()*2/3);
}
*/
void MainWidget::createButtonBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(MAIN_BUTTONS_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    ft.setBold(true);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    buttonBox=new QGroupBox;
    buttonBox->setMaximumHeight(screenRect.height()/8);
    QLabel *logoLabel;
    logoLabel=new QLabel ;
    QPixmap img(":/logo/res/logo.png");
    logoLabel->setPixmap(img);
    logoLabel->setScaledContents(true);
    logoLabel->setMaximumHeight(screenRect.height()/15);
    logoLabel->setMinimumWidth(screenRect.width()/4);

    operateButton = createButton(":/button/res/operatebtn1.png", this, SLOT(OnOperateBtnClicked()),":/button/res/operatebtn2.png");
    editButton = createButton(":/button/res/editbtn1.png", this, SLOT(OnEditBtnClicked()),":/button/res/editbtn2.png");
    moveToolButton = createButton(":/button/res/movetoolbtn1.png", this, SLOT(OnMoveToolBtnClicked()),":/button/res/movetoolbtn2.png");
    userButton = createButton(":/button/res/userbtn1.png", this, SLOT(OnUserBtnClicked()),":/button/res/userbtn2.png");
    switchButton = createButton(":/button/res/switchbtn1.png", this, SLOT(OnSwitchBtnClicked()),":/button/res/switchbtn2.png");
    logoutButton = createButton(":/button/res/logout1.png", this, SLOT(OnLogoutBtnClicked()),":/button/res/logout2.png");
    exitButton = createButton(":/button/res/exitbtn1.png", this, SLOT(OnExitBtnClicked()),":/button/res/exitbtn2.png");//close()
    powerOffButton = createButton(":/button/res/poweroffbtn1.png", this, SLOT(OnPowerOffBtnClicked()),":/button/res/poweroffbtn2.png");

    int width=70;
    operateButton->setMaximumWidth(width);
    editButton->setMaximumWidth(width);
    moveToolButton->setMaximumWidth(width);
    userButton->setMaximumWidth(width);
    switchButton->setMaximumWidth(width);
    logoutButton->setMaximumWidth(width);
    exitButton->setMaximumWidth(width);
    powerOffButton->setMaximumWidth(width);

    QLabel* operateLable=new QLabel(tr("自动操作"));
    operateLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    operateLable->setFont(ft);
    operateLable->setPalette(palette);
    QVBoxLayout *opLayout = new QVBoxLayout;
    opLayout->addWidget(operateButton);
    opLayout->addWidget(operateLable);

    QLabel* editLable=new QLabel(tr("编辑参数"));
    editLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    editLable->setFont(ft);
    editLable->setPalette(palette);
    QVBoxLayout *editLayout = new QVBoxLayout;
    editLayout->addWidget(editButton);
    editLayout->addWidget(editLable);

    QLabel* moveToolLable=new QLabel(tr("项目管理"));
    moveToolLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    moveToolLable->setFont(ft);
    moveToolLable->setPalette(palette);
    QVBoxLayout *moveToolLayout = new QVBoxLayout;
    moveToolLayout->addWidget(moveToolButton);
    moveToolLayout->addWidget(moveToolLable);

    QLabel* userLable=new QLabel(tr("用户管理"));
    userLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    userLable->setFont(ft);
    userLable->setPalette(palette);
    QVBoxLayout *userLayout = new QVBoxLayout;
    userLayout->addWidget(userButton);
    userLayout->addWidget(userLable);

    QLabel* switchLable=new QLabel(tr("切换用户"));
    switchLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    switchLable->setFont(ft);
    switchLable->setPalette(palette);
    QVBoxLayout *switchLayout = new QVBoxLayout;
    switchLayout->addWidget(switchButton);
    switchLayout->addWidget(switchLable);

    QLabel* logoutLable=new QLabel(tr("注销"));
    logoutLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    logoutLable->setFont(ft);
    logoutLable->setPalette(palette);
    QVBoxLayout *logoutLayout = new QVBoxLayout;
    logoutLayout->addWidget(logoutButton);
    logoutLayout->addWidget(logoutLable);

    QLabel* exitLable=new QLabel(tr("系统设置"));
    exitLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    exitLable->setFont(ft);
    exitLable->setPalette(palette);
    QVBoxLayout *exitLayout = new QVBoxLayout;
    exitLayout->addWidget(exitButton);
    exitLayout->addWidget(exitLable);

    QLabel* powerOffLable=new QLabel(tr("关机"));
    powerOffLable->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    powerOffLable->setFont(ft);
    powerOffLable->setPalette(palette);
    QVBoxLayout *powerOffLayout = new QVBoxLayout;
    powerOffLayout->addWidget(powerOffButton);
    powerOffLayout->addWidget(powerOffLable);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(logoLabel);
    layout->addSpacing(20);
    layout->addLayout(opLayout);
    layout->addLayout(editLayout);
    layout->addLayout(moveToolLayout);
    layout->addSpacing(20);
    layout->addLayout(userLayout);
    layout->addLayout(switchLayout);
    layout->addLayout(logoutLayout);
    layout->addSpacing(20);
    layout->addLayout(exitLayout);
    layout->addLayout(powerOffLayout);
    buttonBox->setLayout(layout);
    buttonBox->setStyleSheet("QGroupBox {background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 rgba(125, 145, 200, 255), stop:0.5 rgba(200, 200, 200, 255), stop:1 rgba(125, 145, 200, 255)); border:none}");
}

PictureButton *MainWidget::createButton(const QString picturePath, QWidget *receiver,
                                        const char *clickedSlot,const char *pressedPath/* =0 */,const char *pressedSlot/* =0 */)
{
    PictureButton* button=new PictureButton(picturePath,pressedPath);
    button->connect(button, SIGNAL(clicked()), receiver, clickedSlot);
    if(pressedSlot)
        button->connect(button, SIGNAL(pressed()), receiver, pressedSlot);
    return button;
}

void MainWidget::getRobot() //机器人控制器加载完毕
{
    if(robotThread->robot==NULL) return;
    connect(robotThread->robot, SIGNAL(activeError(int,int)), this, SLOT(processError(int,int)));
    int st=robotThread->robot->getLoadDllStatus();
    if(st==0)
    {
        operatePage->flagInitFinished=true;
        editParaPage->flagInitFinished=true;
        robotThread2=new RobotThread2(this);
        robotThread2->robot=robotThread->robot;
        robotThread2->start();
        robotThread3=new RobotThread3(this);
        robotThread3->robot=robotThread->robot;
        robotThread3->start();
        operatePage->robot=robotThread->robot;
        editParaPage->robot=robotThread->robot;
        connect(robotThread->robot, SIGNAL(haveAxisCoordinate(double *)), this->operatePage, SLOT(updateCoordinate(double *)));
        connect(robotThread->robot, SIGNAL(macTypeSig()), operatePage->otracksWidget, SLOT(updateTracks()));

        connect(robotThread->robot, SIGNAL(haveAxisCoordinate(double *)), this->editParaPage, SLOT(updateCoordinate(double *)));
        connect(robotThread->robot, SIGNAL(haveIoStatus()), this->operatePage, SLOT(receiveIoStatus()));
        connect(robotThread->robot, SIGNAL(haveIoStatus()), this->editParaPage, SLOT(receiveIoStatus()));

        connect(this->editParaPage, SIGNAL(goFixPos(int)), robotThread->robot, SLOT(goFixPosSlot(int)));

        connect(this, SIGNAL(closeRobot()), robotThread->robot, SLOT(exitForm()));

        redayLightOperate(2);   //开机复位结束，准备就绪灯亮
        connect(robotThread->robot, SIGNAL(resetOperate(int)), this, SLOT(redayLightOperate(int)));

        //项目正在焊接当中进行复位，要先停止执行项目操作
        connect(robotThread->robot, SIGNAL(resetRobotSig()), this, SLOT(execReset()));

        //启动一个空项目,以便使用复位的信号槽
        operatePage->weldProject=new WeldProject;
        operatePage->weldProject->robot=robotThread->robot;
        operatePage->weldProject->initConnect();
        editParaPage->weldProject=new WeldProject;
        editParaPage->weldProject->robot=robotThread->robot;
        editParaPage->weldProject->initConnect();
    }
}

void MainWidget::execReset()
{
    if(g_currOperatePage==1)
    {
        if(operatePage->weldProject)
            operatePage->weldProject->execReset();
    }
    else if(g_currOperatePage==2)
    {
        if(editParaPage->weldProject)
        {
            editParaPage->weldProject->execReset();
        }
    }
}

void MainWidget::OnOperateBtnClicked()
{
    if(robotThread->robot)
    {
        if(robotThread->robot->isReseting)
        {
            return;
        }
    }
    if(editParaPage->weldProject)
        editParaPage->weldProject->stopExeProj();
    //printf("MainWidget::OnOperateBtnClicked(), statusLoadDll=%d \n",robotThread->robot->getLoadDllStatus());
    g_currOperatePage=1;
    pagesWidget->setCurrentIndex(0);
    pagesWidget->show();
    softModeLabel->setText(tr("按项目自动焊接操作"));
    QPixmap img1(":/light/res/light1.png");
    QPixmap img2(":/light/res/light2.png");
    operateLight->setPixmap(img2);
    editLight->setPixmap(img1);
    moveToolLight->setPixmap(img1);

    if(robotThread->robot)
        robotThread->robot->ML_FF_Stop_Tin(0);   //送锡
    operatePage->otracksWidget->type=1;
    operatePage->clearProjectFlag=0;
    operatePage->loadweldProject();

    editParaPage->clearProject();
    if(editParaPage->weldProject)
        editParaPage->weldProject->clear();
}

void MainWidget::OnEditBtnClicked()
{
    if(g_userRight<USER_RIGHT_DESIGNER) return; //权限不足
    if(robotThread->robot)
    {
        if(robotThread->robot->isReseting)
        {
            return;
        }
    }
    g_currOperatePage=2;
    pagesWidget->setCurrentIndex(1);
    pagesWidget->show();
    softModeLabel->setText(tr("创建或修改项目"));
    QPixmap img1(":/light/res/light1.png");
    QPixmap img2(":/light/res/light2.png");
    operateLight->setPixmap(img1);
    editLight->setPixmap(img2);
    moveToolLight->setPixmap(img1);

    editParaPage->clearProjectFlag=0;
    editParaPage->etracksWidget->type=2;

    operatePage->clearProject();
    if(operatePage->weldProject)
        operatePage->weldProject->clear();
}

void MainWidget::OnMoveToolBtnClicked()     //项目管理
{
    if(g_userRight<USER_RIGHT_DESIGNER) return; //设计师以下，权限不足

    //printf("MainWidget::OnMoveToolBtnClicked(), statusLoadDll=%d \n",robotThread->robot->getLoadDllStatus());
    //pagesWidget->setCurrentIndex(2);
    //pagesWidget->show();
    softModeLabel->setText(tr("项目管理"));

    QPixmap img1(":/light/res/light1.png");
    QPixmap img2(":/light/res/light2.png");
    operateLight->setPixmap(img1);
    editLight->setPixmap(img1);
    moveToolLight->setPixmap(img2);

    ProjManageDlg *dlg=new ProjManageDlg(this);
    dlg->initDialog();
    connect(dlg, SIGNAL(changeProjFinished()), this->operatePage, SLOT(clearProject()));
    connect(dlg, SIGNAL(changeProjFinished()), this->editParaPage, SLOT(clearProject()));
    if(dlg->exec()==QDialog::Accepted)
    {
        //operatePage->otracksWidget->updateTracks();
        //editParaPage->etracksWidget->updateTracks();
    }
    delete dlg;
    operatePage->clearProjectFlag=0;
    editParaPage->clearProjectFlag=0;
}

void MainWidget::OnUserBtnClicked()     //用户管理槽函数
{
    if(g_userRight<USER_RIGHT_ADMIN) return; //管理员以下，权限不足
    UserWidget *user=new UserWidget;
    user->exec();
}

void MainWidget::OnSwitchBtnClicked()   //切换用户槽函数
{
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    LoginDialog loginDlg;
    loginDlg.move((screenRect.width()-loginDlg.size().width())/2+100,70);
    if(loginDlg.exec()==QDialog::Accepted)
    {
        currentUserLabel->setText(g_userName);
        QString right;
        if(g_userRight==USER_RIGHT_VENDER)
            right=QString(tr("厂家级"));
        else if(g_userRight==USER_RIGHT_ADMIN)
            right=QString(tr("管理员"));
        else if(g_userRight==USER_RIGHT_DESIGNER)
            right=QString(tr("设计师"));
        else
        {
            right=QString(tr("操作员"));
            OnOperateBtnClicked();
        }
        currentRightLabel->setText(right);
    }
}

void MainWidget::OnLogoutBtnClicked()     //注销用户槽函数
{
    if(g_userRight<USER_RIGHT_DESIGNER) return; //设计师以下，权限不足
    g_userRight=USER_RIGHT_OPERATOR;
    OnOperateBtnClicked();
    currentRightLabel->setText(tr("操作员"));
}

void MainWidget::OnExitBtnClicked()     //系统设置槽函数
{
    if(g_userRight<USER_RIGHT_ADMIN) return; //管理员以下，权限不足
    SetSystemDlg *dlg=new SetSystemDlg(this);
    //connect(editParaPage, SIGNAL(haveCurrCoordinate(double *)), dlg, SLOT(updateCurrCoordinate(double *)));
    dlg->initDialog();
    if(dlg->exec()==QDialog::Accepted)
    {
    }
}

void MainWidget::OnCloseBtnClicked()     //硬盘序列号绑定读取失败关闭软件
{
    close();
}

void MainWidget::OnPowerOffBtnClicked() //关机槽函数
{
    if(robotThread)
    {
        robotThread->robot->ML_FF_Set_DLL_Return(1);    //立即退出任何焊接动作
        mySleep(10);
        emit closeRobot();
    }
    mySleep(50);
    //robotThread->robot->ML_FF_Set_DLL_Return(1);    //立即退出任何焊接动作
    if(robotThread2)
    {
        robotThread2->quit();
        robotThread2->wait();
        //delete robotThread2;
    }
    if(robotThread3)
    {
        robotThread3->quit();
        robotThread3->wait();
        //delete robotThread3;
    }
    mySleep(10);
    if(robotThread)
    {
        robotThread->quit();
        robotThread->wait();
        //delete robotThread;
    }
    mySleep(50);
    close();
    if(g_userRight<USER_RIGHT_VENDER)  //厂家权限以下，关机
        ShutDown();
}
void MainWidget::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWidget::processError(int errCodeType,int dllErrCode) //处理系统错误
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

    QString strErr,strErrX,strErrY,strErrZ,strErrZh,strErrR,strErrSpeed;
    if((errCodeType & 0x01)==0x01)
    {
        strErrX=QString(tr(" X坐标错误 "));
        strErr+=strErrX;
    }
    if((errCodeType & 0x02)==0x02)
    {
        strErrY=QString(tr(" Y坐标错误 "));
        strErr+=strErrY;
    }
    if((errCodeType & 0x04)==0x04)
    {
        strErrZ=QString(tr(" Z坐标错误 "));
        strErr+=strErrZ;
    }
    if((errCodeType & 0x08)==0x08)
    {
        strErrZh=QString(tr(" Z坐标提升值错误 "));
        strErr+=strErrZh;
    }
    if((errCodeType & 0x10)==0x10)
    {
        strErrR=QString(tr(" R坐标值错误 "));
        strErr+=strErrR;
    }
    if((errCodeType & 0x20)==0x20)
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
    if((errCodeType & 0x100)==0x100)
    {
        strErr=tr("系统发现机器在执行点焊操作时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x200)==0x200)
    {
        strErr=tr("系统发现机器在执行拖焊操作时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x400)==0x400)
    {
        strErr=tr("系统发现机器在执行空点操作时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x800)==0x800)
    {
        strErr=tr("系统发现机器在执行清锡操作时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x1000)==0x1000)
    {
        strErr=tr("系统发现机器在执行复位操作时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x2000)==0x2000)
    {
        strErr=tr("系统发现机器在软件退出关闭板卡时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x4000)==0x4000)
    {
        strErr=tr("系统发现机器在R轴回零时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
    if((errCodeType & 0x8000)==0x8000)
    {
        strErr=tr("系统发现加载动态库时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        close();
        return;
    }
    if((errCodeType & 0x10000)==0x10000)
    {
        strErr=tr("系统发现机器在执行定位操作时出错，错误代码是%1").arg(dllErrCode);
        msgBox.setText(strErr);
        msgBox.exec();
        return;
    }
}
void MainWidget::redayLightOperate(int st) //对准备就绪灯的状态的操作，1=灭；2=亮
{
    if((st==1) || (st==3))
    {
        QPixmap img1(":/light/res/light1.png");
        redayLight->setPixmap(img1);
    }
    else if(st==2)
    {
        QPixmap img2(":/light/res/light2.png");
        redayLight->setPixmap(img2);
        if(g_currOperatePage==1)
        {
            operatePage->flagTempStop=0;
            if(operatePage->weldProject)
            {
                if(operatePage->weldProject->listCommand.size()>0)
                    operatePage->weldProject->setCurrCommand(0);
            }
        }
        else if(g_currOperatePage==2)
        {
            if(editParaPage->weldProject)
            {
                if(editParaPage->weldProject->listCommand.size()>0)
                    editParaPage->weldProject->setCurrCommand(0);
            }
        }
    }
    else if(st==4)
    {
        QPixmap img2(":/light/res/light2.png");
        redayLight->setPixmap(img2);
    }
}
bool MainWidget::ShutDown()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    //获取进程标志
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
         return false;

    //获取关机特权的LUID
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,    &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    //获取这个进程的关机特权
    AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    if (GetLastError() != ERROR_SUCCESS) return false;

    // 强制关闭计算机
    if ( !ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
          return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWidget::gpoSet(unsigned char  Num)          //开关量输出1,Num:通道号
{
    if(robotThread)
    {
        if(robotThread->robot)
        {
            robotThread->robot->ML_SetOutput(Num);
        }
    }
}

void MainWidget::gpoClear(unsigned char  Num)        //开关量输出0,Num:通道号
{
    if(robotThread)
    {
        if(robotThread->robot)
        {
            robotThread->robot->ML_ClearOutput(Num);
        }
    }
}

void MainWidget::pressKey_1_slot()
{
    qDebug() << "Enter pressKey_1_slot(). user press key_1.";
    keySelPressed1=true;
}

void MainWidget::release_key_1_slot()
{
    qDebug() << "Enter release_key_1_slot(). user release key_1.";
    if(keySelPressed1)
    {
        if(keySelFlag1==0)
        {
            keySelFlag1=1;
            //gpoSet(g_IOxb.sysOxb[ORE1]);    //点亮选择键1的灯
        }
        else
        {
            keySelFlag1=0;
            //gpoClear(g_IOxb.sysOxb[ORE1]);  //熄灭选择键1的灯
        }
        keySelPressed1=false;
    }
}

void MainWidget::pressKey_2_slot()
{
    qDebug() << " user press key_2.";
    if((keySelFlag1==0) && (keySelFlag2==0))    //选择x,y轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_x,0);
                robotThread->robot->start_N_Move(a_y);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==0))    //选择r,z轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_r,0);
                robotThread->robot->start_N_Move(a_z);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==1))    //选择r1,r2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_r1,0);
                robotThread->robot->start_N_Move(a_r2);
            }
        }
    }
    else if((keySelFlag1==0) && (keySelFlag2==1))    //选择x,y2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_x,0);
                robotThread->robot->start_N_Move(a_y2);
            }
        }
    }
}

void MainWidget::release_key_2_slot()
{
    qDebug() << " user release key_2.";
    if(robotThread)
    {
        if(robotThread->robot)
        {
            robotThread->robot->stopMove(a_y,0);
            robotThread->robot->stopMove(a_z,0);
            robotThread->robot->stopMove(a_r2,0);
            robotThread->robot->stopMove(a_y2,0);
        }
    }
}

void MainWidget::pressKey_3_slot()
{
    qDebug() << " user press key_3.";
    keySelPressed2=true;
}

void MainWidget::release_key_3_slot()
{
    qDebug() << " user release key_3.";
    if(keySelPressed2)
    {
        if(keySelFlag2==0)
        {
            keySelFlag2=1;
            //gpoSet(g_IOxb.sysOxb[ORE2]);    //点亮选择键1的灯
        }
        else
        {
            keySelFlag2=0;
            //gpoClear(g_IOxb.sysOxb[ORE2]);  //熄灭选择键1的灯
        }
        keySelPressed2=false;
    }
}

void MainWidget::pressKey_4_slot()
{
    qDebug() << " user press key_4.";
    if((keySelFlag1==0) && (keySelFlag2==0))    //选择x,y轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_y,0);
                robotThread->robot->start_N_Move(a_x);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==0))    //选择r,z轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_z,0);
                robotThread->robot->start_N_Move(a_r);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==1))    //选择r1,r2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_r2,0);
                robotThread->robot->start_N_Move(a_r1);
            }
        }
    }
    else if((keySelFlag1==0) && (keySelFlag2==1))    //选择x,y2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_y2,0);
                robotThread->robot->start_N_Move(a_x);
            }
        }
    }
}

void MainWidget::release_key_4_slot()
{
    qDebug() << " user release key_4.";
    if(robotThread)
    {
        if(robotThread->robot)
        {
            robotThread->robot->stopMove(a_x,0);
            robotThread->robot->stopMove(a_r,0);
            robotThread->robot->stopMove(a_r1,0);
        }
    }
}

void MainWidget::pressKey_5_slot()
{
    qDebug() << " user press key_5.";
}

void MainWidget::release_key_5_slot()
{
    qDebug() << " user release key_5.";
}

void MainWidget::pressKey_6_slot()
{
    qDebug() << " user press key_6.";
    if((keySelFlag1==0) && (keySelFlag2==0))    //选择x,y轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_y,0);
                robotThread->robot->start_P_Move(a_x);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==0))    //选择r,z轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_z,0);
                robotThread->robot->start_P_Move(a_r);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==1))    //选择r1,r2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_r2,0);
                robotThread->robot->start_P_Move(a_r1);
            }
        }
    }
    else if((keySelFlag1==0) && (keySelFlag2==1))    //选择x,y2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_y2,0);
                robotThread->robot->start_P_Move(a_x);
            }
        }
    }
}

void MainWidget::release_key_6_slot()
{
    qDebug() << " user release key_6.";
    if(robotThread)
    {
        if(robotThread->robot)
        {
            robotThread->robot->stopMove(a_x,0);
            robotThread->robot->stopMove(a_r,0);
            robotThread->robot->stopMove(a_r1,0);
        }
    }
}

void MainWidget::pressKey_7_slot()
{
    qDebug() << " user press key_7.";
}

void MainWidget::release_key_7_slot()
{
    qDebug() << " user release key_7.";
}

void MainWidget::pressKey_8_slot()
{
    qDebug() << " user press key_8.";
    if((keySelFlag1==0) && (keySelFlag2==0))    //选择x,y轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_x,0);
                robotThread->robot->start_P_Move(a_y);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==0))    //选择r,z轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_r,0);
                robotThread->robot->start_P_Move(a_z);
            }
        }
    }
    else if((keySelFlag1==1) && (keySelFlag2==1))    //选择r1,r2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_r1,0);
                robotThread->robot->start_P_Move(a_r2);
            }
        }
    }
    else if((keySelFlag1==0) && (keySelFlag2==1))    //选择x,y2轴移动
    {
        if(robotThread)
        {
            if(robotThread->robot)
            {
                robotThread->robot->stopMove(a_x,0);
                robotThread->robot->start_P_Move(a_y2);
            }
        }
    }
}

void MainWidget::release_key_8_slot()
{
    qDebug() << " user release key_8.";
    if(robotThread)
    {
        if(robotThread->robot)
        {
            robotThread->robot->stopMove(a_y,0);
            robotThread->robot->stopMove(a_z,0);
            robotThread->robot->stopMove(a_r2,0);
            robotThread->robot->stopMove(a_y2,0);
        }
    }
}

void MainWidget::pressKey_9_slot()
{
    qDebug() << " user press key_9.";
}

void MainWidget::release_key_9_slot()
{
    qDebug() << " user release key_9.";
}

void MainWidget::release_key_80_slot()
{
    qDebug() << " user release key_+.";
    MoveKeyDlg* dlg=new MoveKeyDlg(this);
    dlg->robot=robotThread->robot;
    dlg->speedValue=robotThread->robot->speedValue;
    dlg->resize(600,480);
    dlg->exec();

}

void MainWidget::release_key_NumLock_slot()
{
    if(g_NumLockSt)
        qDebug() << " The Num Lock is on.";
    else
        qDebug() << " The Num Lock is off.";
}
////////////////////////////////////////////////////////////////////////////////////////////////////
