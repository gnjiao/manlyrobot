#ifndef LOGICDIALOG_H
#define LOGICDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMessageBox>

#include "myLineEdit.h"
#include "global.h"

class DelayDlg : public QDialog
{
    Q_OBJECT
public:
    explicit DelayDlg(QWidget *parent = 0);

    void initDialog();

    double time;
    QLineEdit *delayTimeEdit;//要延时的时间，单位：秒

signals:

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();

private:
};

class GotoDlg : public QDialog
{
    Q_OBJECT
public:
    explicit GotoDlg(QWidget *parent = 0);

    void initDialog();

    int number;
    QLineEdit *commandNumEdit;//要跳转到的指令序号

signals:

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();

private:

    MyInputPanelContext *context;
};

class ConditionDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ConditionDlg(QWidget *parent = 0);

    void initDialog();

    QLineEdit *commandNumEdit;//要跳转到的指令序号

signals:

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();

private:
};

class BlankPointDlg : public QDialog
{
    Q_OBJECT
public:
    explicit BlankPointDlg(QWidget *parent = 0);

    void initDialog();

    double atspeed;
    double zh;
    int delayTime;
    double pointPos[4];

signals:
    //void loadDlgFinished();         //本对话框加载完毕

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void OnUpdatePosBtnClicked();
    void updateCoordinate(double *coordinate);
    void loadMyLineEdit();      //加载自定义文本框

private:
    MyLineEdit *atspeedNumEdit;  //移动速度文本框
    MyLineEdit *zhEdit;          //提升高度文本框
    MyLineEdit *delayTimeEdit;   //延时文本框
    QList <MyLineEdit*> posEdit;
    double currPointPos[8];     //当前焊点坐标

    SYSTEM_PARAMS systemPara;

    MyInputPanelContext *context;

    void mySleep(unsigned int msec);
};

//烙铁姿态调整
class IronPoseDlg : public QDialog
{
    Q_OBJECT
public:
    explicit IronPoseDlg(QWidget *parent = 0);

    void initDialog();

    double atspeed;
    double zh;
    int delayTime;
    double pointPos[6];
    int exeOnce;    //1=仅执行一次；0=每次都执行

signals:
    //void loadDlgFinished();         //本对话框加载完毕

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void OnUpdatePosBtnClicked();
    void updateCoordinate(double *coordinate);
    void loadMyLineEdit();      //加载自定义文本框

private:
    MyLineEdit *atspeedNumEdit;  //移动速度文本框
    MyLineEdit *zhEdit;          //提升高度文本框
    MyLineEdit *delayTimeEdit;   //延时文本框
    QList <MyLineEdit*> posEdit;
    QCheckBox* onceCheckBox;
    double currPointPos[8];     //当前焊点坐标

    SYSTEM_PARAMS systemPara;

    MyInputPanelContext *context;

    void mySleep(unsigned int msec);
};

#endif // LOGICDIALOG_H
