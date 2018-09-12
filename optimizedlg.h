#ifndef OPTIMIZEDLG_H
#define OPTIMIZEDLG_H

#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableView>
#include <QStandardItemModel>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QCheckBox>
#include <QButtonGroup>

#include "global.h"
#include "weldproject.h"

#define POINT_WELD_TECHNICS     1       //点焊工艺
#define LINE_WELD_TECHNICS      2       //拖焊工艺

class OptimizeDlg : public QDialog
{
    Q_OBJECT
public:
    explicit OptimizeDlg(QWidget *parent = 0);

    void initDialog();

    WeldProject* weldProject;   //焊接项目对象
public slots:
    void OnCancelBtnClicked();
    void OnOkBtnClicked();
    void setCheckItem(const QModelIndex &index);

private:
    int checkBoxNum;
    int checkBoxCount;
    int dlgWidth;
    int dlgHeight;
    QHBoxLayout *btnLayout;
    QTableView *tableView;
    QStandardItemModel* dataModel;
    QButtonGroup *listCheckBox;
    //QList<QCheckBox *> listCheckBox;

    void createDataWidget();
    void createButtonLayout();
    void OnLoadOptCmd();
};

#endif // OPTIMIZEDLG_H
