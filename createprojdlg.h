#ifndef CREATEPROJDLG_H
#define CREATEPROJDLG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPoint>
#include <QTimer>
#include <QMessageBox>
#include <QProcess>

class CreateProjDlg : public QDialog
{
    Q_OBJECT
public:
    explicit CreateProjDlg(QWidget *parent = 0);
    void initDialog();
    void startIn();
    void startOut();


    QString projectName;    //项目名称
    QString direction;      //焊点锡面方向
signals:

public slots:
    void OnClickedOKButton();
    void OnClickedCancelButton();
    void OnClickedUpButton();
    void OnClickedDownButton();
    void OnClickedLeftButton();
    void OnClickedRightButton();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPushButton *upButton;
    QPushButton *downButton;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QLineEdit *nameEdit;
    QLineEdit *directEdit;
    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
    QColor startColor;
    int currentAlpha;
    int fadeTimes;
    QTimer *timer;
    int inoutFlag;      //淡入淡出标志,1=淡入；2=淡出；0=正常
    bool isOK;
};

#endif // CREATEPROJDLG_H
