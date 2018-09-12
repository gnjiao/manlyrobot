#ifndef MOVEKEYDLG_H
#define MOVEKEYDLG_H

#include <QWidget>
#include <QDialog>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QTimer>

#include "global.h"
#include "manlyrobot.h"
#include "trackswidget.h"

class MoveKeyDlg : public QDialog
{
    Q_OBJECT
public:
    explicit MoveKeyDlg(QWidget *parent = 0);

    void initDialog();

    ManlyRobot *robot;  //机器人控制器
    TracksWidget *etracksWidget;
    QVector<double> curPointAxisPos;
    QList<QVector<double> > pointsAxisPos;
    int flayX;
    int flayY;
    int beginPointX;
    int endPointX;
    unsigned char speedValue;

signals:
    void readCoordinate();  //开始读坐标
    void stopCoordinate();  //停止读坐标
    //void signal_Axis_P_Move(unsigned char axis,unsigned char moveMod);
    //void signal_Axis_N_Move(unsigned char axis,unsigned char moveMod);
    void stop_Axis_Move(unsigned char axis,unsigned char stopMod);
    void overLimit(unsigned char,unsigned char);    //参数:1个是轴,第2个超上下限
    void slowlyInOutDlg(int flag);  //flag: 1=透明；2=不透明
    void start_Axis_P_Move(unsigned char axis);
    void start_Axis_N_Move(unsigned char axis);

public slots:
    void updateCoordinate(double *coordinate);
    void closeDlg();
    void startIn();
    void startOut();
    void inout();

private:
    QGroupBox *keyBtnBox;
    QRadioButton *lineMoveRdoBtn;
    QRadioButton *pointMoveRdoBtn;
    QComboBox *stepLengthComBox;
    QLineEdit *speedEdit;
    QLineEdit *r1CoordEdit;
    QLineEdit *r2CoordEdit;
    QPushButton *lockButton;
    QPushButton *nR1Button;
    QPushButton *nR2Button;
    QPushButton *pR1Button;
    QPushButton *pR2Button;
    QTimer *timer;

    int lockPosFlag;        //姿态锁定按钮
    double upValue[8];      //上限值
    double downValue[8];    //下限值
    bool flagLimit[8];      //下限标志
    bool flagLimit2[8];     //上限标志
    int inoutFlag;          //淡入淡出标志,1=淡入；2=淡出；0=正常
    double halfX;           //X坐标的中间值

    void createButtonBox();

private slots:
    void OnNXBtnPressed();
    void OnNYBtnPressed();
    void OnNY2BtnPressed();
    void OnNZBtnPressed();
    void OnNRBtnPressed();
    void OnNR1BtnPressed();
    void OnNR2BtnPressed();
    void OnNSBtnPressed();

    void OnNXBtnReleased();
    void OnNYBtnReleased();
    void OnNY2BtnReleased();
    void OnNZBtnReleased();
    void OnNRBtnReleased();
    void OnNR1BtnReleased();
    void OnNR2BtnReleased();
    void OnNSBtnReleased();

    void OnPXBtnPressed();
    void OnPYBtnPressed();
    void OnPY2BtnPressed();
    void OnPZBtnPressed();
    void OnPRBtnPressed();
    void OnPR1BtnPressed();
    void OnPR2BtnPressed();
    void OnPSBtnPressed();

    void OnPXBtnReleased();
    void OnPYBtnReleased();
    void OnPY2BtnReleased();
    void OnPZBtnReleased();
    void OnPRBtnReleased();
    void OnPR1BtnReleased();
    void OnPR2BtnReleased();
    void OnPSBtnReleased();

    void OnLockBtnClicked();
    void OnLineMoveRdoBtnClicked();
    void OnPointMoveRdoBtnClicked();

    void setCurrStepLen(int index);

    void mySleep(unsigned int msec);
};

#endif // MOVEKEYDLG_H
