#ifndef TECHNICSDLG_H
#define TECHNICSDLG_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QListWidget>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
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

class TechnicsDlg : public QDialog
{
    Q_OBJECT
public:
    explicit TechnicsDlg(QWidget *parent = 0);

    void initDialog();

    int type;   // 1=点焊工艺，2=拖焊工艺
    QString currTechnicsName;   //当前工艺名称
    WeldProject* weldProject;   //焊接项目对象
    QListWidget *listWidget;
    QList<QCheckBox *> listCheckBox;

public slots:
    void OnLoadBtnClicked();
    void OnCancelBtnClicked();
    void OnNewTechnicsBtnClicked();
    void OnEditBtnClicked();
    void OnDeleteBtnClicked();
    void OnSaveAsBtnClicked();
    void OnRenameBtnClicked();
    void OnImportBtnClicked();
    void OnExportChkClicked();
    void OnExportBtnClicked();

protected:
    bool event(QEvent *e);

private:
    QString caption;
    QVBoxLayout *btnLayout;
    QLineEdit* saveAsEdit;
    QPushButton *saveAsButton;
    QPushButton *importButton;
    QPushButton *exportButton;
    QCheckBox *emportCheck;
    QProcess *program;

    QPushButton *loadButton;
    QPushButton *cancelButton;
    QPushButton *newTechnicsButton;
    QPushButton *editButton;
    QPushButton *deleteButton;

    int keybaordOpenedFlag; //输入小键盘已打开标志

    int saveFlag;

    void createButtonLayout();
    int saveNewTechnicPara(QString oldName,QString newName);

    QJsonObject createPointTechnicsJson(S_P item);
    QJsonObject createLineTechnicsJson(L_P item);
    QJsonObject createEnvParaJson(QString technicsName,ENV_PARA envParaItem);
    QJsonObject createPointTechnicsJson(QString technicsName,S_P pointTechnicsItem,ENV_PARA envParaItem);
    QJsonObject createLineTechnicsJson(QString technicsName,L_P lineTechnicsItem,ENV_PARA envParaItem);
};

#endif // TECHNICSDLG_H
