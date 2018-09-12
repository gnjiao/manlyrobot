#ifndef POINTWELDDLG_H
#define POINTWELDDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QTimer>
#include <QProcess>

#include "global.h"
#include "myinputpanel.h"
#include "myLineEdit.h"

class PointWeldDlg : public QDialog
{
    Q_OBJECT
public:
    explicit PointWeldDlg(QWidget *parent = 0);

    void initDialog();

    int type;                   // 0=新建焊点， 1=编辑已有焊点，2=新建点焊工艺参数，3=编辑点焊工艺参数
    int pointNum;               //焊点序号
    QString weldTechnicsName;   //焊接工艺名称
    double Z_h;                 //Z提升高度
    //double runSpeed;            //运行速度
    double pointPos[8];         //指令中焊点坐标
    double currPointPos[8];     //当前焊点坐标
    bool cylinderEnable;        //气缸使能
    S_P pointWeldPara;          //点焊参数
    int flayX;
    int flayY;
    int flayWidth;
    int flayHeight;
    int beginPointX;
    int beginPointY;

    QLineEdit *weldTechnicsEdit;//焊接工艺名称文本框(type=0时)
    QComboBox *weldTechnicsComboBox;//焊接工艺名称组合框(type=1时)
    QLineEdit *pointNameEdit;   //焊点名称文本框
    MyLineEdit *Z_hEdit;         //Z提升高度文本框
    bool appToSameTechnicsChecked;  //将工艺应用于同类工艺选择框结果

    QString old_WeldTechnicsName;   //在编辑焊点参数时原来的焊接工艺名称
    double old_Z_h;                 //在编辑焊点参数时原来的Z提升高度

signals:
    void useMoveKeyDlg(int flag); //此信号触发movetool键盘对话框, 1=开启键盘，2=关闭键盘
    void loadDlgFinished();         //本对话框加载完毕

public slots:
    void OnLoadDefaultBtnClicked();
    //void OnCopyParaBtnClicked();
    void OnSetDefaultBtnClicked();
    void OnUpdatePosBtnClicked();
    void OnEnvParaBtnClicked();
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void selectPointWeldPara(const QString &pointWeldTechName);
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
    bool event(QEvent *e);

private:
    void createLeftLayout();
    void createRightLayout();
    void createBottomLayout();

    MyInputPanelContext *context;
    int flagKB_On;      //触摸键盘已调出来
    int flagZ_h_Focus;  //提升高度文本框有焦点标志
    int flagRunSpeed_Focus;         //提升高度文本框有焦点标志
    int flagCurrPosEdit_Focus[6];
    int flagParaEdit1_Focus[8];
    int flagParaEdit2_Focus[8];
    int flagInitFinished;
    SYSTEM_PARAMS systemPara;
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
    MyLineEdit *runSpeedEdit;    //运行速度文本框
    QComboBox *preModeComboBox; //预送锡方式组合框
    QComboBox *weldModeComboBox;//抖焊方式组合框
    QCheckBox *cylinderCheckBox;//气缸使能检查框
    //QComboBox *currItemComboBox;//当前项目组合框
    QPushButton * loadDefaultValBtn;
    QPushButton * copyParaValBtn;
    QPushButton * setDefaultValBtn;
    QPushButton * updatePosBtn;
    QCheckBox* appToSameTechnicsCheckBox;
    QPushButton * envParaBtn;

    QVBoxLayout *leftLayout;
    QGridLayout *paraLayout;
    QHBoxLayout *bottomLayout;

    QList <MyLineEdit*> currPosEdit;
    QList <MyLineEdit*> technicsParaEdit1;
    QList <MyLineEdit*> technicsParaEdit2;
};

#endif // POINTWELDDLG_H
