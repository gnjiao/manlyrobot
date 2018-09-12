#ifndef TRACKSWIDGET_H
#define TRACKSWIDGET_H

#include <QWidget>
#include <QDesktopWidget>
#include <QApplication>
#include <QLabel>
#include <QGroupBox>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSlider>
#include <QPainter>
#include <QPoint>
#include <QPen>
#include <QColor>

#include "weldproject.h"

class TracksWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TracksWidget(QWidget *parent = 0);

    void initWidget();

    int type;
    QLabel *widgetCapion;
    QLineEdit *coordEdit;   //坐标系文本框
    QLineEdit *xEdit;       //X文本框
    QLineEdit *yEdit;       //Y文本框
    QLineEdit *y2Edit;      //Y2文本框
    QLineEdit *zEdit;       //Z文本框
    QLineEdit *rEdit;       //R文本框
    QLineEdit *r1Edit;      //R1文本框
    QLineEdit *r2Edit;      //R2文本框
    QLineEdit *unitEdit;    //单位文本框
    QPointF point;          //目标点
    QColor lineColor;       //画线的颜色
    QColor pointColor;      //画点的颜色
    QColor color[3];        //3种颜色
    QList<QVector<double> > pointsAxisPos;

    QSlider *left_Z_Slider; //Z轴滑块条(左边)
    QSlider *down_R_Slider; //R轴滑块条(下边)
    QSlider *up_R1_Slider;  //R1轴滑块条(上边)
    QSlider *right_R2_Slider;//R2轴滑块条(右边)

    WeldProject *weldProject;
signals:

public slots:
    void updateTracks();    //更新所画轨迹

protected:
    void paintEvent(QPaintEvent *event);

private:
    QGroupBox *coordinateBox;
    //绘画区尺寸
    int left;
    int top;
    int width;
    int height;
    SYSTEM_PARAMS systemPara;
    //double axisLimitPos[9];

    void createCoordinateBox();
    void createSlider();
    void drawCurrPoint(QPainter &painter,double x,double y); //画焊点
    void drawPoint(QPainter &painter,double x,double y,bool currPoint,int num); //画焊点,currPoint,是否当前点；num,操作命令序号
    void drawLineWeld(QPainter &painter,QPointF point1,QPointF point2,bool currPoint,int num); //画拖焊线
    void drawPoints(QPainter &painter); //画多个焊点
    void drawBlankPoint(QPainter &painter,double x,double y,bool currPoint,int num); //画焊点,currPoint,是否当前点；num,操作命令序号
    void drawIronPosePoint(QPainter &painter,double x,double y,bool currPoint,int num); //画烙铁姿态,currPoint:是否当前点；num:该点的序号
};

#endif // TRACKSWIDGET_H
