#ifndef CORRECTDLG_H
#define CORRECTDLG_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QRadioButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <math.h>

#include "global.h"
#include "myLineEdit.h"
#include "weldproject.h"

class CorrectDlg : public QDialog   //校正对话框
{
    Q_OBJECT
public:
    explicit CorrectDlg(QWidget *parent = 0);

    int correctType;    //校正类型： 1=平移校正; 2=平移+旋转校正; 3=Z偏移

    void initDialog();

signals:

public slots:
    void OnParallelBtnClicked();
    void OnRotateBtnClicked();
    void OnZOffsetBtnClicked();
    void OnCancelBtnClicked();

private:
    QPushButton * parallelBtn;  //平移校正
    QPushButton * rotateBtn;    //平移+旋转校正
    QPushButton * zOffsetBtn;   //Z偏移
};

class ParalleCorrectDlg : public QDialog    //平移校正对话框
{
    Q_OBJECT
public:
    explicit ParalleCorrectDlg(QWidget *parent = 0);
    int fromStep;   //从第几步
    int toStep;     //到第几步
    WeldProject* weldProject;           //焊接项目对象
    int mod;    //平移方式:0=移动；1=复制

    void initDialog();

signals:

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void allSelectBtnClicked();
    void loadMyLineEdit();      //加载自定义文本框

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    //QPushButton * allSelectBtn;  //全部按钮
    MyInputPanelContext *context;
    MyLineEdit *fromEdit;   //从步骤文本框
    MyLineEdit *toEdit;     //到步骤文本框
    QComboBox* modComboBox; //平移方式组合框

    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

class RotateCorrectDlg : public QDialog //平移+旋转校正对话框
{
    Q_OBJECT
public:
    explicit RotateCorrectDlg(QWidget *parent = 0);
    int fromStep;   //从第几步
    int toStep;     //到第几步
    WeldProject* weldProject;           //焊接项目对象
    ManlyRobot *robot;  //机器人控制器
    //double newFirstPos[8];      //第一个点新坐标
    //double newEndPos[8];        //最后一个点新坐标

    void initDialog();

signals:
    void goFixPos(int weldType);    //触发定位

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void allSelectBtnClicked();
    void OnFirstBtnClicked();
    void OnEndBtnClicked();
    void updateCoordinate(double *coordinate);
    void OnUpdatePosBtn1Clicked();
    void OnUpdatePosBtn2Clicked();
    void OnUpdateR0PosBtnClicked();
    void OnUpdateR180PosBtnClicked();
    void OncalculateBtnClicked();
    void loadMyLineEdit();      //加载自定义文本框

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    void createR0PointPosBox();
    void createPointPosBox();

    //QPushButton * allSelectBtn;  //全部按钮
    MyInputPanelContext *context;
    MyLineEdit *fromEdit;   //从步骤文本框
    MyLineEdit *toEdit;     //到步骤文本框
    QList <MyLineEdit*> r0PosEdit;
    QList <MyLineEdit*> r180PosEdit;
    QList <MyLineEdit*> startPosEdit;
    QList <MyLineEdit*> endPosEdit;
    QVBoxLayout *posR0Layout;       //初始化点的布局
    QVBoxLayout *posLayout;         //参考点的布局
    double currPointPos[8];         //当前焊头坐标
    SYSTEM_PARAMS systemPara;       //系统设置参数
    QLineEdit *radiusEdit;           //烙铁旋转半径
    QLineEdit *initangleEdit;        //烙铁初始角度

    double r0PointPos[8];           //某点R=0的坐标
    double r180PointPos[8];         //某点R=180的坐标
    double oldFirstPointPos[8];     //第一点旧坐标
    double newFirstPointPos[8];     //第一点新坐标
    double oldEndPointPos[8];       //第二点旧坐标
    double newEndPointPos[8];       //第二点新坐标
    int firstPointNum;              //第一个焊点序号
    int endPointNum;                //第二个焊点序号
    double Initangle;               //烙铁初始角度（弧度）
    double Turning_Radius;          //烙铁旋转半径（mm）

    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

class ZOffsetDlg : public QDialog   //Z偏移
{
    Q_OBJECT
public:
    explicit ZOffsetDlg(QWidget *parent = 0);
    int fromStep;   //从第几步
    int toStep;     //到第几步
    WeldProject* weldProject;           //焊接项目对象
    int mod;    //偏移方式：1=+ ； 2=-；
    double zLength;

    void initDialog();

signals:

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void allSelectBtnClicked();
    void loadMyLineEdit();      //加载自定义文本框
    void OnZAddBtnClicked();
    void OnZReduceBtnClicked();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    //QPushButton * allSelectBtn;  //全部按钮
    MyInputPanelContext *context;
    MyLineEdit *fromEdit;   //从步骤文本框
    MyLineEdit *toEdit;     //到步骤文本框
    QRadioButton* zAddBtn;  //Z坐标加按钮
    QRadioButton* zReduceBtn;  //Z坐标减按钮
    MyLineEdit* lengthEdit; //Z偏移的长度编辑框

    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

#endif // CORRECTDLG_H
