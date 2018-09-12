#ifndef PARAWIDGET_H
#define PARAWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QList>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QDesktopWidget>
#include <QApplication>
#include <QDoubleValidator>
#include <QProcess>

#include "weldproject.h"
//#include "myLineEdit.h"
class ParaWidget : public QDialog
{
    Q_OBJECT
public:
    explicit ParaWidget(QWidget *parent = 0);
    void initDialog();

    WeldProject* weldProject;   //焊接项目对象
signals:

public slots:
    void OnDelBtnClicked();
    void OnCancelDelBtnClicked();
    void OnSaveBtnClicked();
    void OnCloseBtnClicked();
    void OnSelectAllClicked();
    void OnUpdateTichnicsBtnClicked();
    void OnUpdateCoordinateBtnClicked();
    void OnModelIndexClicked(const QModelIndex &index);

private:
    QList <PROJ_ITEM> AllItem,lastAllItem;//AllItem为当前项目，lastAllItem为上一步的项目
    QTableView *tableView;
    QStandardItemModel* dataModel;
    QGroupBox *chgTechnicsGroup;    //改变工艺组
    QGroupBox *chgCoordinateGroup;  //坐标偏移组
    QCheckBox *selectAllCheckbox;
    QComboBox *pointTechnicsComb;   //点焊工艺参数组合框
    QComboBox *lineTechnicsComb;    //拖焊工艺参数组合框
    QPushButton *saveBtn;           //保存按钮
    QLineEdit *coordinateEdit[6];   //各轴坐标偏移量文本框
    double coordinateOffset[6];     //各轴坐标偏移量
    QProcess *program;

    void createDataWidget();
    void createTechnicsBox();
    void createCoordinateBox();

    inline void refreshTableview();
};

#endif // PARAWIDGET_H
