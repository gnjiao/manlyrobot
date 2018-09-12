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
#include <QStringList>
#include <QFile>
#include <QDir>

#include "global.h"
#include "myLineEdit.h"

class SetSpeedDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SetSpeedDlg(QWidget *parent = 0);

    void initDialog();
signals:
    //void loadDlgFinished();         //本对话框加载完毕

public slots:
    void onOkBtnClicked();
    void OnCancelClicked();
    void loadMyLineEdit();      //加载自定义文本框

private:
    int dlgWidth;
    int dlgHeight;
    MyLineEdit *maxSpeedEdit[8][21];       //防冲击最大速度文本框,8列20行
    SPEED_PARAMS speedPara;

    MyInputPanelContext *context;
};

#endif // SETSYSTEMDLG_H
