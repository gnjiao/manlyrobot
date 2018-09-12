#ifndef OPTIMIZEDLG_H
#define OPTIMIZEDLG_H

#include <QDialog>
#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QGridLayout>
#include <QLabel>
#include <QCheckBox>
#include <QListWidget>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QCheckBox>
#include <QFile>
#include <QFileDialog>

#include "global.h"
#include "weldproject.h"

#define POINT_WELD_TECHNICS     1       //点焊工艺
#define LINE_WELD_TECHNICS      2       //拖焊工艺

class ProjManageDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ProjManageDlg(QWidget *parent = 0);
    virtual ~ProjManageDlg();

    void initDialog();

    WeldProject* weldProject;   //焊接项目对象

signals:
    void changeProjFinished();  //修改或删除项目完成

public slots:
    void OnCancelBtnClicked();
    void OnOkBtnClicked();
    void OnDelBtnClicked();
    void OnCopyBtnClicked();
    void OnChangeBtnClicked();
    void OnImportBtnClicked();
    void OnExportChkClicked();
    void OnExportBtnClicked();

protected:
    bool event(QEvent *e);

private:
    int dlgWidth;
    int dlgHeight;
    int keybaordOpenedFlag; //输入小键盘已打开标志
    int changeMod;  //0=修改; 1=复制
    QStringList projectNameList;

    QListWidget *listWidget;
    QList<QCheckBox *> listCheckBox;
    QVBoxLayout *btnLayout;
    QGroupBox *projBox;
    QLineEdit *nameEdit;
    QComboBox *directComboBox;
    QString oldProjectName;
    QProcess *program;
    QLabel *directionLabel;

    QPushButton *delButton;
    QPushButton *copyButton;
    QPushButton *changeButton;
    QPushButton *importButton;
    QPushButton *exportButton;
    QCheckBox *emportCheck;

    void createProjectBox();
    void createButtonLayout();
    void loadProjectContext();

    QJsonObject createCommandJson(PROJ_ITEM item);
    QJsonObject createClearJson(CLEAR_SOLDER clearSolderItem);
    QJsonObject createProjectJson(QString projName,QList <PROJ_ITEM> AllIteme,CLEAR_SOLDER clearSolderItem);
};

#endif // OPTIMIZEDLG_H
