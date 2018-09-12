#ifndef LINEWELDDLG_H
#define LINEWELDDLG_H

#include <QDialog>
#include <QLineEdit>
#include <QList>
#include <QPushButton>
#include <QRadioButton>
#include <QString>
#include <QLabel>
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

#include "global.h"
#include "myLineEdit.h"

class LineWeldDlg : public QDialog
{
    Q_OBJECT
public:
    explicit LineWeldDlg(QWidget *parent = 0);

    void initDialog();

    int type;                   // 0=新建焊点， 1=编辑已有焊点，2=新建点焊工艺参数，3=编辑点焊工艺参数
    int pointNum;               //焊点序号
    QString weldTechnicsName;   //焊接工艺名称
    double Z_h;                 //Z提升高度
    double pointPos[8];         //拖焊焊点坐标
    double currPointPos[8];     //当前焊点坐标
    double movetoolPos[8];      //movetool传来的焊点坐标
    double ironPos[2];          //烙铁姿态
    bool cylinderEnable;        //气缸使能
    int flayX;
    int flayY;
    int flayWidth;
    int flayHeight;
    int beginPointX;
    int beginPointY;
    int lineWeldType;           //拖焊类型： 0=普通拖焊；1=多段拖焊首段；2=多段拖焊中段；3=多段拖焊末段

    bool appToSameTechnicsChecked;  //将工艺应用于同类工艺选择框结果
    L_P lineWeldPara;           //拖焊参数

    QLineEdit *weldTechnicsEdit;//焊接工艺名称文本框(type=0时)
    QComboBox *weldTechnicsComboBox;//焊接工艺名称组合框(type=1时)
    MyLineEdit *pointNameEdit;   //焊点名称文本框
    MyLineEdit *Z_hEdit;         //Z提升高度文本框

    QString old_WeldTechnicsName;   //在编辑焊点参数时原来的焊接工艺名称
    double old_Z_h;                 //在编辑焊点参数时原来的Z提升高度

signals:
    void useMoveKeyDlg(int flag);   //此信号触发movetool键盘对话框, 1=开启键盘，2=关闭键盘
    void loadDlgFinished();         //本对话框加载完毕

public slots:
    void OnLoadDefaultBtnClicked();
    //void OnCopyParaBtnClicked();
    void OnSetDefaultBtnClicked();
    //void OnUpdatePosBtnClicked();
    void OnOkBtnClicked();
    void OnCancelBtnClicked();
    void OnEnvParaBtnClicked();
    void updateCoordinate(double *coordinate);
    void OnUpdatePosBtn1Clicked();
    void OnUpdatePosBtn2Clicked();
    void selectLineWeldPara(const QString &lineWeldTechName);
    void inout();   //定时器timer槽函数
    void inout2();   //定时器timer2槽函数
    void startIn();     //飞入
    void startOut();    //飞出
    void slowlyInOut(int flag); //使对话框透明或不透明 1=透明；2=不透明
    void loadMyLineEdit();      //加载自定义文本框
    //void onClickedTechnicsEdit();   //点击工艺名称对话框

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    bool event(QEvent *e);

private:
    void createLeftLayout();
    void createRightLayout();
    void createBottomLayout();
    void createPointPosBox();

    int dlgWidth;
    int dlgHeight;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    int alpha;          //窗口透明度
    int fadeTimes;
    int inoutFlag;      //淡入淡出标志,1=淡入；2=淡出；0=正常
    QTimer *timer;
    QTimer *timer2;
    bool isOK;
    SYSTEM_PARAMS systemPara;   //系统设置参数
    int flagKB_On;              //触摸键盘已调出来
    int flagInitFinished;       //对话框初始化完毕=1

    MyLineEdit *runSpeedEdit;    //运行速度文本框
    QCheckBox *cylinderCheckBox;//气缸使能检查框
    QComboBox *currItemComboBox;//当前项目组合框
    QGroupBox *pointPosBox;     //起始点和结束点的box
    QPushButton * loadDefaultValBtn;
    QPushButton * copyParaValBtn;
    QPushButton * setDefaultValBtn;
    QRadioButton *lineWeldRdoBtn;
    QRadioButton *manyLineWeldRdoBtn1;
    QRadioButton *manyLineWeldRdoBtn2;
    QRadioButton *manyLineWeldRdoBtn3;
    QCheckBox* appToSameTechnicsCheckBox;   //将工艺应用于同类工艺选择框
    QPushButton * envParaBtn;

    QVBoxLayout *leftLayout;
    QGridLayout *paraLayout;
    QHBoxLayout *bottomLayout;
    QHBoxLayout *posLayout1;
    QHBoxLayout *posLayout2;

    //QList <MyLineEdit*> currPosEdit;
    QList <MyLineEdit*> technicsParaEdit1;
    QList <MyLineEdit*> technicsParaEdit2;
    QList <MyLineEdit*> startPosEdit;
    QList <MyLineEdit*> endPosEdit;

    MyInputPanelContext *context;
};

#endif // LINEWELDDLG_H
