#ifndef SETSYSTEMADVDLG_H
#define SETSYSTEMADVDLG_H

#include <QWidget>
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
#include <QButtonGroup>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QCheckBox>

#include "myLineEdit.h"

class SetSystemAdvDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SetSystemAdvDlg(QWidget *parent = 0);

    void initDialog();
signals:

public slots:
    void OnIOSetBtnClicked();
    void OnSpeedSetBtnClicked();
    void onOkBtnClicked();
    void OnCancelClicked();
    void loadMyLineEdit();      //加载自定义文本框

private:
    int dlgWidth;
    int dlgHeight;
    //控制器参数
    MyLineEdit *maxPulseEdit;            //最大脉冲数文本框
    QCheckBox *fastStopChk;
    QComboBox *OutPulseFormComBox[8];   //输出脉冲格式
    MyLineEdit *VLPPS_Edit[8];       //VLPPS文本框
    MyLineEdit *VHPPS_Edit[8];       //VHPPS文本框
    MyLineEdit *Tacc_Edit[8];        //Tacc文本框
    MyLineEdit *Tdec_Edit[8];        //
    MyLineEdit *Tsacc_Edit[8];       //
    MyLineEdit *Tsdec_Edit[8];       //

    //电机及机械参数
    QComboBox *PulsePerComBox[8];     //
    MyLineEdit *maxSpeedEdit[8];     //
    MyLineEdit *macStepLenEdit[8];   //机械步距

    //回原点参数
    MyLineEdit *VL_Home_Edit[8];        //
    MyLineEdit *VH_Hm_Edit[8];          //
    MyLineEdit *TAcc_Edit[8];           //

    QComboBox *typeComboBox;         //机器类型组合框

    MyInputPanelContext *context;
};

#endif // SETSYSTEMADVDLG_H
