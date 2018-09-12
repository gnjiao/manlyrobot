#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QStackedWidget>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QCryptographicHash>

#include "msqldb.h"
#include "picturebutton.h"
#include "operatepage.h"
#include "editparapage.h"
#include "movetoolpage.h"
#include "manlyrobot.h"
#include "classNativeEventFilterZ.h"

extern NativeEventFilterZ *nativeEventFilter;
class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void initWidget();

signals:
    void closeRobot();
    void closeApp();

public slots:
    void execReset();   //执行复位

    void pressKey_1_slot();
    void release_key_1_slot();
    void pressKey_2_slot();
    void release_key_2_slot();
    void pressKey_3_slot();
    void release_key_3_slot();
    void pressKey_4_slot();
    void release_key_4_slot();
    void pressKey_5_slot();
    void release_key_5_slot();
    void pressKey_6_slot();
    void release_key_6_slot();
    void pressKey_7_slot();
    void release_key_7_slot();
    void pressKey_8_slot();
    void release_key_8_slot();
    void pressKey_9_slot();
    void release_key_9_slot();
    void release_key_80_slot();
    void release_key_NumLock_slot();
private:
    SqlDB *db;
    /////////////////////////////////////////////////////////////
    bool keySelPressed1;    //选择键1按下
    bool keySelPressed2;    //选择键2按下
    int keySelFlag1;        //手柄按键选择按钮的状态标志
    int keySelFlag2;        //手柄按键选择按钮的状态标志,keySelFlag1,keySelFlag2: (0,0)=x,y轴；(1,0)=z,r轴；(0,1)=y2轴；(1,1)=r1,r2轴；
    /////////////////////////////////////////////////////////////
    QLabel *redayLight;
    QLabel *operateLight;
    QLabel *editLight;
    QLabel *moveToolLight;
    QLabel *softModeLabel;
    QLabel *dateTimeLabel;
    QLabel *currentUserLabel;
    QLabel *currentRightLabel;

    QGroupBox *statusBox;
    QGroupBox *operatorBox;
    QGroupBox *buttonBox;
    QVBoxLayout *conditionLayout;
    QStackedWidget *pagesWidget;
    OperatePage *operatePage;
    EditParaPage *editParaPage;
    //MoveToolPage *moveToolPage;

    PictureButton *operateButton;
    PictureButton *editButton;
    PictureButton *moveToolButton;
    PictureButton *userButton;
    PictureButton *switchButton;
    PictureButton *logoutButton;
    PictureButton *exitButton;
    PictureButton *powerOffButton;

    RobotThread *robotThread;   //机器人控制器线程
    RobotThread2 *robotThread2; //机器人控制器线程2
    RobotThread3 *robotThread3; //机器人控制器线程3
    /////////////////////////////////////////////////////////////////////////////////////
    void gpoSet(unsigned char  Num);          //开关量输出1,Num:通道号
    void gpoClear(unsigned char  Num);        //开关量输出0,Num:通道号
    /////////////////////////////////////////////////////////////////////////////////////
    void createStatusBox();
//    void createOperatorBox();
    void createButtonBox();
    PictureButton *createButton(const QString picturePath, QWidget *receiver,
                              const char *clickedSlot,const char *pressedPath=0,const char *pressedSlot=0);
    void mySleep(unsigned int msec);
    bool isRightUser();
    QString strtomd5(QString str);
private slots:
    void OnOperateBtnClicked();
    void OnEditBtnClicked();
    void OnMoveToolBtnClicked();
    void OnUserBtnClicked();
    void OnSwitchBtnClicked();
    void OnLogoutBtnClicked();
    void OnExitBtnClicked();
    void OnPowerOffBtnClicked();
    void OnCloseBtnClicked();//硬盘序列号绑定读取失败关闭软件
    void time_slot();   //在右上角显示日期时间
    void getRobot();    //当机器人对象完成创建时，可由此槽函数获得机器人对象
    void processError(int errCodeType,int dllErrCode); //处理系统错误
    void redayLightOperate(int st); //对准备就绪灯的状态的操作，0=灭；1=亮
    bool ShutDown();
};

#endif // MAINWIDGET_H
