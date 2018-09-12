#ifndef MOVETOOLPAGE_H
#define MOVETOOLPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QListView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QComboBox>
#include <QVector>
#include <QList>

#include "global.h"
#include "manlyrobot.h"
#include "trackswidget.h"
#include "picturebutton.h"

class MoveToolPage : public QWidget
{
    Q_OBJECT
public:
    explicit MoveToolPage(QWidget *parent = 0);

    void initWidget();

    ManlyRobot *robot;  //机器人控制器
    TracksWidget *mtracksWidget;
    QVector<double> curPointAxisPos;
    QList<QVector<double> > pointsAxisPos;
    double movetoolMatrixPos[2][8];      //阵列两点坐标
signals:
    void readCoordinate();  //开始读坐标
    void stopCoordinate();  //停止读坐标
    void signal_Axis_P_Move(unsigned char axis,unsigned char moveMod);
    void signal_Axis_N_Move(unsigned char axis,unsigned char moveMod);
    void stop_Axis_Move(unsigned char axis,unsigned char stopMod);
    void overLimit(unsigned char,unsigned char);    //参数:1个是轴,第2个超上下限

public slots:
    void updateCoordinate(double *coordinate);

private:
    QGroupBox *leftBtnBox;
    QRadioButton *lineMoveRdoBtn;
    QRadioButton *pointMoveRdoBtn;
    QComboBox *stepLengthComBox;
    QLineEdit *speedLeftEdit;
    QGroupBox *rightBtnBox;
    QGroupBox *twoPointPosBox;
     QLineEdit *speedRightEdit;
    QLineEdit *r1CoordEdit;
    QLineEdit *r2CoordEdit;
    QPushButton *lockButton;
    QPushButton *nR1Button;
    QPushButton *nR2Button;
    QPushButton *pR1Button;
    QPushButton *pR2Button;
    QPushButton *lsButton;      //拖焊起点按钮
    QPushButton *leButton;      //拖焊终点按钮
    QLineEdit *xEdit;       //X文本框
    QLineEdit *yEdit;       //Y文本框
    QLineEdit *zEdit;       //Z文本框
    QLineEdit *rEdit;       //R文本框
    QLineEdit *xEdit2;       //X文本框
    QLineEdit *yEdit2;       //Y文本框
    QLineEdit *zEdit2;       //Z文本框
    QLineEdit *rEdit2;       //R文本框

    SYSTEM_PARAMS systemPara;

    int pointIndex;
    int lockPosFlag;        //姿态锁定按钮
    unsigned char speedValue;
    double upValue[8];      //上限值
    double downValue[8];    //下限值
    bool flagLimit[8];      //下限标志
    bool flagLimit2[8];     //上限标志
    //bool stopOverFlag;

    void createLeftButtonBox();
    void createRightButtonBox();
    void createTwoPointPosBox();
    PictureButton *createButton(const QString picturePath, QWidget *receiver,
                              const char *clickedSlot,const char *pressedPath=0,const char *pressedSlot=0);

private slots:
    void OnNXBtnPressed();
    void OnNYBtnPressed();
    void OnNZBtnPressed();
    void OnNRBtnPressed();
    void OnNR1BtnPressed();
    void OnNR2BtnPressed();
    void OnNSBtnPressed();
    void OnNXBtnReleased();
    void OnNYBtnReleased();
    void OnNZBtnReleased();
    void OnNRBtnReleased();
    void OnNR1BtnReleased();
    void OnNR2BtnReleased();
    void OnNSBtnReleased();

    void OnPXBtnPressed();
    void OnPYBtnPressed();
    void OnPZBtnPressed();
    void OnPRBtnPressed();
    void OnPR1BtnPressed();
    void OnPR2BtnPressed();
    void OnPSBtnPressed();
    void OnPXBtnReleased();
    void OnPYBtnReleased();
    void OnPZBtnReleased();
    void OnPRBtnReleased();
    void OnPR1BtnReleased();
    void OnPR2BtnReleased();
    void OnPSBtnReleased();

    void OnLockBtnClicked();
    void OnLsBtnClicked();
    void OnLeBtnClicked();
    void OnOneBtnClicked();
    void OnTwoBtnClicked();
    void OnLineMoveRdoBtnClicked();
    void OnPointMoveRdoBtnClicked();

    void setCurrStepLen(int index);

    void mySleep(unsigned int msec);
};

#endif // MOVETOOLPAGE_H
