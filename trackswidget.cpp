#include "global.h"
#include "trackswidget.h"
#include "myrwxml.h"

TracksWidget::TracksWidget(QWidget *parent) :
    QWidget(parent)
{
    weldProject=NULL;
    type=0;
    QVector<double> tmpPoint;
    for(int i=0;i<8;i++)
        tmpPoint.append(0);
    pointsAxisPos.append(tmpPoint);
    pointsAxisPos.append(tmpPoint);
    //g_MacType=2;
}

void TracksWidget::initWidget()
{
    for(int i=0;i<9;i++)
        systemPara.limitPosLen[i]=0;
    MyRWxml xml;
    xml.readSysPara("robot.xml",systemPara);

    QPalette palette = QPalette();  //  得到窗口部件的调色板
    //palette.setColor(QPalette::Background, QColor(125,145,200));
    //this->setAutoFillBackground(true);
    //this->setPalette(palette);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    this->setMinimumWidth(screenRect.width()/2);
    this->setMaximumWidth(screenRect.width()/2);
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);
    ft.setBold(true);
    widgetCapion=new QLabel(tr("焊点轨迹"),this);
    widgetCapion->setFont(ft);
    widgetCapion->setMaximumHeight(screenRect.height()/20);
    if(type==1)
        widgetCapion->setText(tr("自动操作焊点轨迹"));
    else if(type==2)
        widgetCapion->setText(tr("编辑项目焊点轨迹"));
    else if(type==3)
        widgetCapion->setText(tr("手工移动焊点轨迹"));
/*
    QVector<double>  pointPos=QVector<double> (8);    //存储当前点的坐标
    for(int i=0;i<8;i++)
        pointPos[i]=0;
    pointsAxisPos.append(pointPos);
    pointsAxisPos.append(pointPos);
*/
    createCoordinateBox();
    createSlider();

    color[0].setRgb(255,0,0);
    color[1].setRgb(0,255,0);
    color[2].setRgb(0,0,255);
    lineColor=QColor(255,255,255);
    pointColor=color[0];

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addWidget(left_Z_Slider);
    midLayout->addStretch();
    midLayout->addWidget(right_R2_Slider);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(widgetCapion);
    layout->addWidget(up_R1_Slider);
    //layout->addStretch();
    layout->addLayout(midLayout);
    layout->addWidget(down_R_Slider);
    layout->addWidget(coordinateBox);
    setLayout(layout);
}

void TracksWidget::createCoordinateBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(TRACKS_COORDINATE_FONT_COLOR));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT-2);
    ft.setItalic(false);
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();

    QLabel *coordLabel=new QLabel(tr("坐标系:"));
    coordLabel->setFont(ft);
    QLabel *xLabel=new QLabel(tr("X:"));
    xLabel->setFont(ft);
    QLabel *yLabel=new QLabel(tr("Y:"));
    yLabel->setFont(ft);
    QLabel *y2Label=new QLabel(tr("Y2:"));
    y2Label->setFont(ft);
    QLabel *zLabel=new QLabel(tr("Z:"));
    zLabel->setFont(ft);
    QLabel *rLabel=new QLabel(tr("R:"));
    rLabel->setFont(ft);
    QLabel *r1Label=new QLabel(tr("R1:"));
    r1Label->setFont(ft);
    QLabel *r2Label=new QLabel(tr("R2:"));
    r2Label->setFont(ft);
    QLabel *unitLabel=new QLabel(tr("单位:"));
    unitLabel->setFont(ft);
    ft.setPointSize(LABEL_BUTTON_FONT);
    coordEdit=new QLineEdit;
    coordEdit->setFont(ft);
    coordEdit->setMaximumWidth(50);
    xEdit=new QLineEdit;
    xEdit->setFont(ft);
    yEdit=new QLineEdit;
    yEdit->setFont(ft);
    y2Edit=new QLineEdit;
    y2Edit->setFont(ft);
    zEdit=new QLineEdit;
    zEdit->setFont(ft);
    rEdit=new QLineEdit;
    rEdit->setFont(ft);
    r1Edit=new QLineEdit;
    r1Edit->setFont(ft);
    r2Edit=new QLineEdit;
    r2Edit->setFont(ft);
    //ft.setPointSize(20);
    unitEdit=new QLineEdit;
    unitEdit->setFont(ft);
    unitEdit->setAlignment(Qt::AlignCenter);
    unitEdit->setText("mm");
    unitEdit->setMaximumWidth(50);

    QHBoxLayout *coordLayout = new QHBoxLayout;
    //coordLayout->addWidget(coordLabel);
    //coordLayout->addWidget(coordEdit);
    coordLayout->addWidget(xLabel);
    coordLayout->addWidget(xEdit);
    coordLayout->addWidget(yLabel);
    coordLayout->addWidget(yEdit);
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        coordLayout->addWidget(y2Label);
        coordLayout->addWidget(y2Edit);
    }
    coordLayout->addWidget(zLabel);
    coordLayout->addWidget(zEdit);
    coordLayout->addWidget(rLabel);
    coordLayout->addWidget(rEdit);
    if((g_MacType==3) || (g_MacType==4) || (g_MacType==1))    //6轴
    {
        coordLayout->addWidget(r1Label);
        coordLayout->addWidget(r1Edit);
        coordLayout->addWidget(r2Label);
        coordLayout->addWidget(r2Edit);
    }
    //coordLayout->addWidget(unitLabel);
    //coordLayout->addWidget(unitEdit);

    coordinateBox=new QGroupBox;
    coordinateBox->setLayout(coordLayout);
    coordinateBox->setMaximumHeight(screenRect.height()/20);
}

void TracksWidget::createSlider()
{
    left_Z_Slider=new QSlider;
    left_Z_Slider->setTickPosition(QSlider::TicksRight);
    left_Z_Slider->setTickInterval(1);  //刻度线的间隔
    left_Z_Slider->setMinimum(0);
    left_Z_Slider->setMaximum((int)systemPara.limitPosLen[2]);
    left_Z_Slider->setInvertedAppearance(true);
    //left_Z_Slider->setInvertedControls(true);

    down_R_Slider=new QSlider(Qt::Horizontal);
    down_R_Slider->setTickPosition(QSlider::TicksAbove);
    down_R_Slider->setTickInterval(1);  //刻度线的间隔
    down_R_Slider->setMaximum((int)systemPara.limitPosLen[3]);
    down_R_Slider->setMinimum((int)systemPara.limitPosLen[4]);

    up_R1_Slider=new QSlider(Qt::Horizontal);
    up_R1_Slider->setTickPosition(QSlider::TicksBelow);
    up_R1_Slider->setTickInterval(1);  //刻度线的间隔
    up_R1_Slider->setMaximum((int)systemPara.limitPosLen[5]);
    up_R1_Slider->setMinimum((int)systemPara.limitPosLen[6]);

    right_R2_Slider=new QSlider;
    right_R2_Slider->setTickPosition(QSlider::TicksLeft);
    right_R2_Slider->setTickInterval(1);  //刻度线的间隔
    right_R2_Slider->setMaximum((int)systemPara.limitPosLen[7]);
    right_R2_Slider->setMinimum((int)systemPara.limitPosLen[8]);
}

void TracksWidget::drawCurrPoint(QPainter &painter,double x,double y) //画焊点
{
    painter.setPen( QPen(color[1], 1) );
    painter.setBrush( color[1] );
    //将控制器返回的坐标转换为屏幕画点的坐标
    double xx=(width/systemPara.limitPosLen[0])*x+left;
    double yy=(height/systemPara.limitPosLen[1])*y+top;
    //画点
    QPointF point;
    point.setX(xx);
    point.setY(yy);
    painter.drawEllipse(point,3,3);
}

void TracksWidget::drawPoint(QPainter &painter,double x,double y,bool currPoint,int num) //画焊点,currPoint:是否当前点；num:该点的序号
{
    if(currPoint)
    {
        painter.setPen( QPen(color[0], 1) );
        painter.setBrush( color[0] );
    }
    else
    {
        painter.setPen( QPen(color[2], 1) );
        painter.setBrush( color[2] );
    }
    //将控制器返回的坐标转换为屏幕画点的坐标
    double xx=(width/systemPara.limitPosLen[0])*x+left;
    double yy=(height/systemPara.limitPosLen[1])*y+top;
    //画点
    QPointF point;
    point.setX(xx);
    point.setY(yy);
    painter.drawEllipse(point,3,3);

    QFont ft;
    ft.setPointSize(8);
    ft.setItalic(false);
    painter.setFont(ft);
    painter.setPen( QPen(color[1], 1) );
    QString pointNum=QString("%1").arg(num);
    painter.drawText(xx-5,yy-3,pointNum);
}

void TracksWidget::drawBlankPoint(QPainter &painter,double x,double y,bool currPoint,int num) //画空点,currPoint:是否当前点；num:该点的序号
{
    if(currPoint)
    {
        painter.setPen( QPen(color[0], 1) );
        painter.setBrush( QBrush(Qt::NoBrush) );
    }
    else
    {
        painter.setPen( QPen(color[2], 1) );
        painter.setBrush( QBrush(Qt::NoBrush) );
    }
    //将控制器返回的坐标转换为屏幕画点的坐标
    double xx=(width/systemPara.limitPosLen[0])*x+left;
    double yy=(height/systemPara.limitPosLen[1])*y+top;
    //画点
    QPointF point;
    point.setX(xx);
    point.setY(yy);
    painter.drawEllipse(point,3,3);

    QFont ft;
    ft.setPointSize(8);
    ft.setItalic(false);
    painter.setFont(ft);
    painter.setPen( QPen(color[1], 1) );
    QString pointNum=QString("%1").arg(num);
    painter.drawText(xx-5,yy-3,pointNum);
}

void TracksWidget::drawIronPosePoint(QPainter &painter,double x,double y,bool currPoint,int num) //画烙铁姿态,currPoint:是否当前点；num:该点的序号
{
    if(currPoint)
    {
        painter.setPen( QPen(color[0], 1) );
        painter.setBrush( QBrush(Qt::NoBrush) );
    }
    else
    {
        painter.setPen( QPen(color[2], 1) );
        painter.setBrush( QBrush(Qt::NoBrush) );
    }
    //将控制器返回的坐标转换为屏幕画点的坐标
    double xx=(width/systemPara.limitPosLen[0])*x+left;
    double yy=(height/systemPara.limitPosLen[1])*y+top;
    //画点
    QPointF point1,point2;
    point1.setX(xx);
    point1.setY(yy);
    point2.setX(xx);
    point2.setY(yy-3);
    painter.drawLine(point1,point2);
    QRectF rectangle(xx-1.5,yy-3-7,3,7);
    painter.drawRect(rectangle);

    QFont ft;
    ft.setPointSize(8);
    ft.setItalic(false);
    painter.setFont(ft);
    painter.setPen( QPen(color[1], 1) );
    QString pointNum=QString("%1").arg(num);
    painter.drawText(xx-10,yy-3,pointNum);
}

void TracksWidget::drawLineWeld(QPainter &painter,QPointF point1,QPointF point2,bool currPoint,int num) //画拖焊线
{
        painter.setPen( QPen(lineColor, 4) );
        painter.setBrush( lineColor );
        //将控制器返回的坐标转换为屏幕画点的坐标
        /*point1 *= (width/300.0);
        point1 += QPointF(left,left);
        point2 *= (width/300.0);
        point2 += QPointF(left,left);*/
        double x1=(width/systemPara.limitPosLen[0])*point1.x()+left;
        double y1=(height/systemPara.limitPosLen[1])*point1.y()+top;
        double x2=(width/systemPara.limitPosLen[0])*point2.x()+left;
        double y2=(height/systemPara.limitPosLen[1])*point2.y()+top;
        point1.setX(x1);
        point1.setY(y1);
        point2.setX(x2);
        point2.setY(y2);
        //画线
        painter.drawLine(point1,point2);
        //画点
        if(currPoint)
        {
            painter.setPen( QPen(color[0], 1) );
            painter.setBrush( color[0] );
        }
        else
        {
            painter.setPen( QPen(color[2], 1) );
            painter.setBrush( color[2] );
        }
        painter.drawEllipse(point1,3,3);
        painter.drawEllipse(point2,3,3);
        // 写焊点命令序号
        painter.setPen( QPen(color[1], 1) );
        QString pointNum=QString("%1").arg(num);
        painter.drawText(point1.x()-5,point1.y()-3,pointNum);
}

void TracksWidget::drawPoints(QPainter &painter) //画多个焊点
{
    painter.setPen( QPen(color[1], 1) );
    painter.setBrush( color[1] );
    for(int i=0;i<pointsAxisPos.size();i++)
    {
        //将控制器返回的坐标转换为屏幕画点的坐标
        double xx=(width/systemPara.limitPosLen[0])*pointsAxisPos[i][0]+left;
        double yy=(height/systemPara.limitPosLen[1])*pointsAxisPos[i][1]+top;
        //画点
        QPointF point;
        point.setX(xx);
        point.setY(yy);
        painter.drawEllipse(point,3,3);
    }
}

void TracksWidget::paintEvent(QPaintEvent */*event*/)
{
    left=left_Z_Slider->size().width()+10+5;
    top=widgetCapion->size().height()+up_R1_Slider->size().height()+10+10+5;
    width=this->size().width()-left*2-5;
    height=this->size().height()-(top+down_R_Slider->size().height()+coordinateBox->size().height()+30);

    QPainter painter(this);//painter.setBrush(Qt::LinearGradientPattern);

    QPainterPath path;
    path.addRoundRect(this->rect(), 5); //圆角设置
    painter.fillPath(path, QColor(MAIN_WIDGET_COLOR,250));//QBrush(Qt::LinearGradientPattern)

    QLinearGradient gradient(left+width/4, top+height/2, left+width*3/4, top+height/2);
    gradient.setColorAt(0, QColor(100,150,200,250));
    gradient.setColorAt(1, QColor(90,130,160,250));
    //QBrush brush(gradient);

    QRectF rect(left, top, width, height);
    QRectF rect1(left+1, top+1, width-1, height-1);
    painter.drawRect(rect);
    painter.fillRect(rect1,QBrush(gradient));
    //painter.drawRoundedRect(rect,10.0,10.0);
    //反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
//g_MacType=2;
    if((g_MacType==2) || (g_MacType==4) || (g_MacType==6))    //双平台
    {
        painter.drawLine(left+width/2,top,left+width/2,top+height);
    }

    if(type==1 || type==2)
    {
        if(!weldProject)
        {
            drawCurrPoint(painter,point.x(),point.y());
            return;
        }
        for(int i=0;i<weldProject->listCommand.size();i++)
        {
            if(weldProject->listCommand[i]==QString(tr("点焊")) || weldProject->listCommand[i]==QString(tr("清锡")))
            {
                QPointF point;
                point.setX(weldProject->pointsAxisPos[i][0]);
                point.setY(weldProject->pointsAxisPos[i][1]);
                if(i==weldProject->currCommNum)
                    drawPoint(painter,point.x(),point.y(),true,i);
                else
                    drawPoint(painter,point.x(),point.y(),false,i);
            }
            else if(weldProject->listCommand[i]==QString(tr("拖焊")) || weldProject->listCommand[i]==QString(tr("多拖1"))
                    || weldProject->listCommand[i]==QString(tr("多拖2")) || weldProject->listCommand[i]==QString(tr("多拖3")))
            {
                QPointF point1,point2;
                point1.setX(weldProject->pointsAxisPos[i][0]);
                point1.setY(weldProject->pointsAxisPos[i][1]);
                point2.setX(weldProject->pointsAxisPos[i][4]);
                point2.setY(weldProject->pointsAxisPos[i][5]);
                if(i==weldProject->currCommNum)
                    drawLineWeld(painter,point1,point2,true,i);
                else
                    drawLineWeld(painter,point1,point2,false,i);
            }
            else if(weldProject->listCommand[i]==QString(tr("空点")))
            {
                QPointF point;
                point.setX(weldProject->pointsAxisPos[i][0]);
                point.setY(weldProject->pointsAxisPos[i][1]);
                if(i==weldProject->currCommNum)
                    drawBlankPoint(painter,point.x(),point.y(),true,i);
                else
                    drawBlankPoint(painter,point.x(),point.y(),false,i);
            }
            else if(weldProject->listCommand[i]==QString(tr("姿态")))
            {
                QPointF point;
                point.setX(weldProject->pointsAxisPos[i][0]);
                point.setY(weldProject->pointsAxisPos[i][1]);
                if(i==weldProject->currCommNum)
                    drawIronPosePoint(painter,point.x(),point.y(),true,i);
                else
                    drawIronPosePoint(painter,point.x(),point.y(),false,i);
            }
        }
        drawCurrPoint(painter,point.x(),point.y());
    }
    else if(type==3)
    {
        drawCurrPoint(painter,point.x(),point.y());
        drawPoints(painter);
    }
}

void TracksWidget::updateTracks()
{
    update();
}
