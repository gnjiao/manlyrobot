#ifndef PICTUREBUTTON_H
#define PICTUREBUTTON_H

//#include <QtGui>
//#include <QtCore>
#include <QPushButton>
#include <QEvent>
#include <QPainter>

class PictureButton : public QPushButton    //QAbstractButton
{
    Q_OBJECT
private:
    QPixmap pixmap;
    QPixmap releasePixmap;
    QPixmap pressedPixmap;
    void setBkPalette(int transparency);//设置透明度
protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual QSize sizeHint() const;
    void enterEvent(QEvent *);//鼠标进入事件
    void leaveEvent(QEvent *);//离开
    void mousePressEvent(QMouseEvent *e);//点击
    void mouseReleaseEvent(QMouseEvent *e);//释放
public:
    PictureButton(const QString &iconPath,const char *pressIconPath=0, QWidget *parent=0);
    PictureButton(QWidget *parent = 0);
    ~PictureButton();
    void setPixmap(const QString &path);
    void setPressedPixmap(const QString &path);
};

#endif // PICTUREBUTTON_H
