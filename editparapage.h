#ifndef EDITPARAPAGE_H
#define EDITPARAPAGE_H

#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QMessageBox>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QModelIndex>
#include <QMenu>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QAbstractItemView>
#include <QProcess>

#include "trackswidget.h"
#include "manlyrobot.h"
#include "weldproject.h"

class EditParaPage : public QWidget
{
    Q_OBJECT
public:
    explicit EditParaPage(QWidget *parent = 0);

    void initWidget();

    bool flagInitFinished;      //机器人控制器初始化结束标志，true=结束； false=未结束
    ManlyRobot *robot;  //机器人控制器
    WeldProject* weldProject;           //焊接项目对象
    TracksWidget *etracksWidget;
    QComboBox *projectComboBox;         //焊接项目名称组合框
    QVector<double> curPointAxisPos;
    QList<QVector<double> > pointsAxisPos;
    QLabel* currentTemp;                //当前温度
    double movetoolMatrixPos[2][8];      //movetool传来的阵列两点坐标
    int clearProjectFlag;   //由项目管理对话框而触发的清理已打开项目的标记
signals:
    void readCoordinate();  //开始读坐标
    void stopCoordinate();  //停止读坐标
    void readIoStatus();    //开始读IO状态
    void stopMoveKey();     //停止movetool键盘对话框
    void slowlyInOutDlg(int flag);  //flag: 1=透明；2=不透明
    void haveCurrCoordinate(double *coordinate);  //有当前坐标
    void goFixPos(int weldType);    //触发定位

public slots:
    void updateCoordinate(double *coordinate);  //更新坐标
    void OnNewProjectBtnClicked();   //新建项目按钮点击槽函数
    void OnTableViewClicked(const QModelIndex & index);
    void RowDoubleClicked(const QModelIndex &);
    void OnMoveBtnClicked();            //屏幕右上角按钮，点击后出现movetool键盘
    void OnUseMoveKey(int flag);        //信号触发出现movetool键盘, 1=开启键盘，2=关闭键盘
    void execSlowlyInOutDlg(int flag);  //改变所调用对话框的透明度
    void receiveIoStatus();             //收到IO状态
    void clearProject();                //清除打开的项目，使系统成无打开项目状态
    void stopWeldOperate(int st);       //停止当前焊接操作
    void reloadTable();                 //刷新项目命令表

protected:
    //virtual void	dragEnterEvent(QDragEnterEvent * event);
        //virtual void	dragLeaveEvent(QDragLeaveEvent * event);
        //virtual void	dragMoveEvent(QDragMoveEvent * event);
    //virtual void	dropEvent(QDropEvent * event);
    //virtual QModelIndexList	selectedIndexes() const;
private:
    QPushButton *moveButton;
    QMenu *logicMenu;
    QPushButton *logicButton;
    QGroupBox *projectBox;
    QGroupBox *paraButtonBox;
    QTableView *tableView;
    QStandardItemModel* dataModel;
    QPushButton* newProjectButton;      //新建项目
    QPushButton *pointWeldButton;
    QPushButton *dragWeldButton;
    QPushButton *arrayButton;
    QCheckBox *autoTempCtrlCheck;       //自动温控检查框
    QLabel *tempLabel;                  //当前温度标签
    int activeMoveDlg;
    int activeWeldDlg;

    SYSTEM_PARAMS systemPara;

    void createProjectBox();
    void createParaButtonBox();
    void createDataWidget();

private slots:
    void openWeldProject(const QString &projectName );
    void OnMoreSelectBtnClicked();

    void OnPointWeldBtnClicked();
    void OnDragWeldBtnClicked();
    void OnPointTechnicsBtnClicked();
    void OnDragTechnicsBtnClicked();

    void OnEditBtnClicked();
    void OnUndoBtnClicked();
    void OnInsertBtnClicked();
    void OnDeleteBtnClicked();
    void OnCopyBtnClicked();
    void OnPasteBtnClicked();

    void OnLogicBtnClicked();
    void OnCoordinateClicked();
    void OnClearBtnClicked();
    void OnIoBtnClicked();
    void OnArrayBtnClicked();
    void OnOptimizeBtnClicked();

    void OnZHeightBtnClicked();
    void OnLocationBtnClicked();
    void OnExecuteBtnClicked();
    void OnTracksBtnClicked();
    void OnCorrectBtnClicked();     //校正


    //按下逻辑按钮后所显示菜单里的槽函数
    void OnDelayActionClicked();    //延时
    void OnGotoActionClicked();     //跳转
    void OnCondiActionClicked();    //条件
    void OnBlankActionClicked();    //空点
    void OnIronPoseActionClicked(); //烙铁姿态

    void OnChangeTempCheckState(int state); //改变自动温控检查框的状态

    void setCurrCommandItem(int index);
};

#endif // EDITPARAPAGE_H
