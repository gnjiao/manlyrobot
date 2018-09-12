#ifndef SETIODLG_H
#define SETIODLG_H
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QComboBox>
#include <QDir>

#include "global.h"

class SetIoDlg : public QDialog
{
    Q_OBJECT
public:
    explicit SetIoDlg(QWidget *parent = 0);

    void initDialog();
signals:

public slots:
    void onOkBtnClicked();
    void OnCancelClicked();
    void onInSelectItem(int index);
    void onOutSelectItem(int index);
    void onHighlightedItem(int index);  //高亮显示某项的槽

protected:
    void paintEvent(QPaintEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    //void mouseMoveEvent(QMouseEvent *event);

private:
    int dlgWidth;
    int dlgHeight;
    bool inItemSelect[33];
    bool outItemSelect[17];
    int boxIndex;
    IO_SET  currIOxb;
    int currIndex;
/*    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    int alpha;          //窗口透明度
    int fadeTimes;
    QTimer *timer;
    QTimer *timer2;
    int inoutFlag;      //淡入淡出标志,1=淡入；2=淡出；0=正常
    bool isOK;*/

    QComboBox *sysInputComBox1[8];        //
    QComboBox *sysInputComBox2[8];        //
    QComboBox *sysInputComBox3[8];        //
    QComboBox *sysOutputComBox[8];        //

    QComboBox *cusInputComBox1[8];        //
    QComboBox *cusInputComBox2[8];        //
    QComboBox *cusInputComBox3[8];        //
    QComboBox *cusOutputComBox[8];        //

};

#endif // SETIODLG_H
