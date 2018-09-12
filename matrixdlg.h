#ifndef MATRIXDLG_H
#define MATRIXDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QTimer>

#include "weldproject.h"
#include "myLineEdit.h"

class MatrixDlg : public QDialog
{
    Q_OBJECT
public:
    explicit MatrixDlg(QWidget *parent = 0);

    void initDialog();

    //QLineEdit *onePosEdit[6];   //第1点坐标
    //QLineEdit *twoPosEdit[6];   //第2点坐标
    int fromStep;
    int toStep;
    int rowNum;
    int colNum;
    double currPointPos[8];
    double myPointPos[6];
    double onePointPos[6];
    double twoPointPos[6];
    double xOffset;
    double yOffset;
    WeldProject* weldProject;           //焊接项目对象
    int flayX;
    int flayY;
    int flayWidth;
    int flayHeight;
    int beginPointX;
    int beginPointY;

signals:
    void useMoveKeyDlg(int flag); //此信号触发movetool键盘对话框, 1=开启键盘，2=关闭键盘
    void loadDlgFinished();         //本对话框加载完毕

public slots:
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void OnSelAllBtnClicked();
    void OnUpdatePosBtnClicked();
    void OnUpdatePosBtnClicked2();
    void inout();   //定时器timer槽函数
    void inout2();   //定时器timer2槽函数
    void updateCoordinate(double *coordinate);
    void startIn();     //飞入
    void startOut();    //飞出
    void slowlyInOut(int flag); //使对话框透明或不透明 1=透明；2=不透明
    void loadMyLineEdit();      //加载自定义文本框

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    int dlgWidth;
    int dlgHeight;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    int alpha;          //窗口透明度
    int fadeTimes;
    QTimer *timer;
    QTimer *timer2;
    int inoutFlag;      //淡入淡出标志,1=淡入；2=淡出；0=正常
    bool isOK;

    MyLineEdit *fromEdit;        //从第几步开始
    MyLineEdit *toEdit;          //到第几步结束
    MyLineEdit *rowEdit;         //行数
    MyLineEdit *colEdit;         //列数
    QGroupBox * commandGroupBox;
    QGroupBox * pointPosBox;
    QList <MyLineEdit*> onePointPosEdit;//第1点坐标
    QList <MyLineEdit*> twoPointPosEdit;//第2点坐标

    MyInputPanelContext *context;

    void createCommandBox();
    void createPosBox();
};

#endif // MATRIXDLG_H
