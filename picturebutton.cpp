//picturebutton.cpp
#include "picturebutton.h"

PictureButton::PictureButton(const QString &iconPath,const char *pressIconPath/* =0 */, QWidget *parent/* =0 */)
: QPushButton(parent)
{
    setPixmap(iconPath);
    if(pressIconPath)
        setPressedPixmap(pressIconPath);
    this->setMaximumSize(pixmap.size());
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setBkPalette(0);//设置背景完全透明
    setFlat(true);
    setAutoFillBackground(true);
}

PictureButton::PictureButton(QWidget *parent /* = 0 */)
: QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}


PictureButton::~PictureButton()
{

}

void PictureButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

QSize PictureButton::sizeHint() const
{
    return pixmap.size();
}

void PictureButton::setPixmap(const QString &path)
{
    pixmap.load(path);
    releasePixmap.load(path);
    update();
}

void PictureButton::setPressedPixmap(const QString &path)
{
    pressedPixmap.load(path);
//    update();
}

void PictureButton::setBkPalette(int transparency)//设置背景透明度releasePixmap
{
    transparency=0;//需要设置透明度时去掉
    QPalette palette;
    palette.setBrush(QPalette::Button,QBrush(QColor(255,255,255,transparency)));//
    setPalette(palette);
}
void PictureButton::enterEvent(QEvent *)
{
    setBkPalette(0);
}
void PictureButton::leaveEvent(QEvent *)
{
    setBkPalette(0);
}
void PictureButton::mousePressEvent(QMouseEvent *)
{
    setBkPalette(0);
    if(pressedPixmap.width()>0)
        pixmap=pressedPixmap;
    update();
    emit pressed();
}
void PictureButton::mouseReleaseEvent(QMouseEvent *)
{
    setBkPalette(0);
    pixmap=releasePixmap;
    update();
    emit clicked();//必须写上，否则不会发出clicked信号
}
