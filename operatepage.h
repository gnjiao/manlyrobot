#ifndef OPERATEPAGE_H
#define OPERATEPAGE_H

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QGroupBox>
#include <QCheckBox>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>

#include "trackswidget.h"
#include "picturebutton.h"
#include "manlyrobot.h"

class OperatePage : public QWidget
{
    Q_OBJECT
public:
    explicit OperatePage(QWidget *parent = 0);

    void initWidget();
    //void closeCurrentProject();

    ManlyRobot *robot;  //机器人控制器线程
    bool flagInitFinished;      //机器人控制器初始化结束标志，true=结束； false=未结束
    TracksWidget *otracksWidget;
    QVector<double> curPointAxisPos;
    int clearProjectFlag;   //由项目管理对话框而触发的清理已打开项目的标记
    WeldProject* weldProject;           //焊接项目对象
    int flagTempStop;                   //暂时停止标志, 1=停止；0=恢复
    QLabel* currentTemp;                //当前温度

signals:
    void readCoordinate();
    void stopCoordinate();
    void resetSig();    //复位机器人

public slots:
    void OnStartBtnClicked();   //开始按钮点击槽函数
    void OnStopBtnClicked();    //停止按钮点击槽函数
    void OnResetBtnClicked();   //复位按钮点击槽函数
    void updateCoordinate(double *coordinate);
    void OnTableViewClicked(const QModelIndex & index);
    void displayProduceLog();   //显示生产日志
    void loadweldProject();     //装入所有焊接项目到组合框
    void clearProject();        //清除打开的项目，使系统成无打开项目状态
    void receiveIoStatus();     //收到IO状态
    void deleteWeldProject();
    void stopWeldOperate(int st);       //停止当前焊接操作
    void OnProduceLogClicked(); //显示生产日志
    void OnChangeTempCheckState(int state); //改变自动温控检查框的状态


private:
    QGroupBox *projectBox;
    QTableView *tableView;
    QStandardItemModel* dataModel;
    PictureButton *startButton;
    PictureButton *stopButton;
    PictureButton *resetButton;
    QGroupBox *buttonBox;
    QComboBox *projectComboBox;         //焊接项目名称组合框
    QPushButton* startFromCurrentButton;//从当前项开始焊接
    QPushButton* currentButton;         //焊接当前项
    QGroupBox *produceLogBox;           //生产日志组合框
    QCheckBox *autoTempCtrlCheck;       //自动温控检查框
    QLabel *tempLabel;                  //当前温度标签

    QLineEdit *outputEdit;              //产量计数文本框
    QLineEdit *weldedPointCountEdit;    //焊点计数文本框
    QLineEdit *weldedTimeEdit;          //焊接时间文本框
    QLineEdit *myOutputEdit;            //机器产量文本框
    QLineEdit *myWeldedPointEdit;       //机器焊点文本框

    QLabel* stopLable;

    int btnEstopST; //急停按钮状态,0=无效（弹起）； 1=有效（按下）
    int btnStopST;  //停止按钮状态
    int currCommNumber;

    QGridLayout *produceLogLayout;

    SYSTEM_PARAMS systemPara;

    void loadProjectItem();     //装入所有projectComboBox的条目
    void createProjectBox();
    void createProduceLogBox();
    void createButtonBox();
    void createDataWidget();
    PictureButton *createButton(const QString picturePath, QWidget *receiver,
                              const char *clickedSlot,const char *pressedPath=0,const char *pressedSlot=0);

private slots:
    void openWeldProject(const QString &projectName );
    void OnStartFromCurrentBtnClicked();//从当前项开始焊接按钮点击槽函数
    void OnCurrentBtnClicked(); //焊接当前项按钮点击槽函数
    void OnZeroBtnClicked();    //产量清零

    void setCurrCommandItem(int index);
};

#endif // OPERATEPAGE_H
