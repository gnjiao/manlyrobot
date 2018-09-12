#ifndef CLEARSOLDERDLG_H
#define CLEARSOLDERDLG_H

#include <QWidget>
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

#include "manlyrobot.h"
#include "weldproject.h"
#include "myLineEdit.h"

class ClearSolderDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ClearSolderDlg(QWidget *parent = 0);

    void initDialog();
    ManlyRobot *robot;  //机器人控制器
    WeldProject* weldProject;           //焊接项目对象
    CLEAR_SOLDER clearItem;
    double currPointPos[8];     //当前焊点坐标

signals:

public slots:
    void onOkBtnClicked();
    void OnCancelClicked();
    void OnUpdatePosClicked();
    void updateCoordinate(double *coordinate);
    void loadMyLineEdit();      //加载自定义文本框

private:
    int dlgWidth;
    int dlgHeight;
    MyLineEdit * clearGroup[4];
    MyLineEdit *xEdit;       //X文本框
    MyLineEdit *yEdit;       //Y文本框
    MyLineEdit *zEdit;       //Z文本框
    MyLineEdit *rEdit;       //R文本框
    MyLineEdit *r1Edit;      //R1文本框
    MyLineEdit *r2Edit;      //R2文本框
    QLineEdit *unitEdit;    //单位文本框

    SYSTEM_PARAMS systemPara;

    MyInputPanelContext *context;
};

#endif // CLEARSOLDERDLG_H
