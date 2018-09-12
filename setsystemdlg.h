#ifndef SETSYSTEMDLG_H
#define SETSYSTEMDLG_H

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
#include <QMessageBox>
#include <QStringList>
#include <QFile>
#include <QDir>

#include "myLineEdit.h"

class SetSystemDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SetSystemDlg(QWidget *parent = 0);

    void initDialog();
signals:
    //void loadDlgFinished();         //本对话框加载完毕

public slots:
    void OnBtnGroupClicked(int id);
    void OnAdvanceSetBtnClicked();
    void onRemoteBtnBtnClicked();
    void onOkBtnClicked();
    void OnCancelClicked();
    void updateCurrCoordinate(double *coordinate);  //更新当前坐标
    void loadMyLineEdit();      //加载自定义文本框

private:
    int dlgWidth;
    int dlgHeight;
    MyLineEdit *mHighSpeedEdit[8];       //手动高速文本框
    MyLineEdit *mLowSpeedEdit[8];        //手动低速文本框
    MyLineEdit *aHighSpeedEdit[8];       //自动高速文本框
    MyLineEdit *aLowSpeedEdit[8];        //自动低速文本框
    MyLineEdit *axisLimitPosEdit[9];     //轴行程
    //QPushButton *axisCurrPosBtn[9];
    MyLineEdit *outSolderSpeedEdit;    //出锡速度
    MyLineEdit *backSolderSpeedEdit;   //退锡速度
    QPushButton *advanceSetBtn;
    QButtonGroup *axisCurrPosBtn;
    QStringList CurrPosBtnText;
    QVector<double> curPointAxisPos;

    MyInputPanelContext *context;
};

#endif // SETSYSTEMDLG_H
