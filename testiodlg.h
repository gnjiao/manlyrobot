#ifndef TESTIODLG_H
#define TESTIODLG_H
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QTimer>
#include <QButtonGroup>

#include "manlyrobot.h"

class TestIoDlg : public QDialog
{
    Q_OBJECT
public:
    explicit TestIoDlg(QWidget *parent = 0);
    ManlyRobot *robot;  //机器人控制器

    void initDialog();
signals:

public slots:
    void OnCheckBtnGroupClicked(int id);
    void OnBtnGroupToggled(int id, bool checked);
    void OnBtnGroupPressed(int id);
    void OnCloseBtnClicked();
    void getIoStatus();         //获得IO的状态,与定时器配合的槽函数

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    int dlgWidth;
    int dlgHeight;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    int alpha;          //窗口透明度
    int fadeTimes;
    QTimer *timer;
    QTimer *timer2;
    QTimer *ioTimer;
    int inoutFlag;      //淡入淡出标志,1=淡入；2=淡出；0=正常
    bool isOK;
    bool buttonChecked[16];

    QLabel *gpiLabel[16];
    QLabel *inputLabel[16];
    QLabel *orgLabel[8];
    QLabel *gpoLabel[16];
    QLabel *estopLabel;
    QButtonGroup *checkButton;
};

#endif // TESTIODLG_H
