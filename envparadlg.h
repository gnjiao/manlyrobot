#ifndef ENVPARADLG_H
#define ENVPARADLG_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>

#include "global.h"
#include "myLineEdit.h"

class EnvParaDlg : public QDialog
{
    Q_OBJECT
public:
    explicit EnvParaDlg(QWidget *parent = 0);

    ENV_PARA envPara;   //工艺环境参数
    QString paraName;   //工艺参数名称
    int technicsType;   //工艺类型：0=点焊； 1=拖焊
    int type;           // 0=新建焊点， 1=编辑已有焊点，2=新建点焊工艺参数，3=编辑点焊工艺参数

    void initDialog();
    //ManlyRobot *robot;  //机器人控制器
    //WeldProject* weldProject;           //焊接项目对象

signals:

public slots:
    void onOkBtnClicked();
    void OnCancelClicked();
    void loadMyLineEdit();      //加载自定义文本框

private:
    int dlgWidth;
    int dlgHeight;
    QComboBox* hanpan_HD_Box;   //焊盘厚度组合框
    QComboBox* hanpan_BMCL_Box; //焊盘表面处理组合框
    QComboBox* hanpan_FTXS_Box; //焊盘覆铜形式组合框
    MyLineEdit* hanpan_ZJ_Edit; //焊盘直径文本框
    MyLineEdit* hanpan_KJ_Edit; //焊盘孔径文本框
    QComboBox* hanxisi_ZJ_Box;  //焊锡丝直径组合框
    QComboBox* hanxisi_ZL_Box;  //焊锡丝种类组合框
    QComboBox* hanxisi_ZHJ_Box; //焊锡丝助焊剂组合框
    QComboBox* hanxisi_FW_Box;  //焊锡丝方位组合框
    MyLineEdit* laotie_WD_Edit; //烙铁温度文本框
    QComboBox* laotie_ZL_Box;   //烙铁种类组合框

    MyInputPanelContext *context;
};

#endif // ENVPARADLG_H
