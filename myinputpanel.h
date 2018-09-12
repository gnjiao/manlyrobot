#ifndef MYINPUTPANEL_H
#define MYINPUTPANEL_H

#include <qpa/qplatforminputcontext.h>
#include <QtGui>
#include <QWidget>
#include <QtCore>
#include <QGridLayout>
#include <QPushButton>
//#include <QPlatformInputContext>
#include <QDialog>

class NumberButton : public QPushButton
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
    void mousePressEvent(QMouseEvent *e);//按下
    void mouseReleaseEvent(QMouseEvent *e);//释放
//    void buttonClicked(QWidget *w);//点击
public:
    QChar buttonValue;
    NumberButton(const QString &iconPath,const char *pressIconPath=0, QWidget *parent=0);
    NumberButton(QWidget *parent = 0);
    ~NumberButton();
    void setPixmap(const QString &path);
    void setPressedPixmap(const QString &path);
};

class MyInputPanel : public QWidget
{
    Q_OBJECT
public:
    explicit MyInputPanel(QWidget *parent = 0);
    
signals:
    void characterGenerated(QChar character);
    void focusChanged(QWidget *oldFocus,QWidget *newFocus);
protected:
    bool event(QEvent *e);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void saveFocusWidget(QWidget *oldFocus, QWidget *newFocus);
    void buttonClicked(QWidget *w);

private:
    QWidget *lastFocusedWidget;
    QSignalMapper signalMapper;

    QGridLayout *mainLayout;
    NumberButton* panelButton_1;
    NumberButton* panelButton_2;
    NumberButton* panelButton_3;
    NumberButton* panelButton_4;
    NumberButton* panelButton_5;
    NumberButton* panelButton_6;
    NumberButton* panelButton_7;
    NumberButton* panelButton_8;
    NumberButton* panelButton_9;
    NumberButton* panelButton_0;
    NumberButton* panelButton_dot;
    NumberButton* panelButton_bp;

    QPoint windowPos;
    QPoint mousePos;
    QPoint dPos;
};

class MyInputPanelContext : public QPlatformInputContext
{
    Q_OBJECT

public:
    MyInputPanelContext(QWidget *parent);  //
    ~MyInputPanelContext();

    bool filterEvent(const QEvent* event);

    QString identifierName();
    QString language();

    bool isComposing() const;

    void reset();
    MyInputPanel *inputPanel;

private slots:
    void sendCharacter(QChar character);

private:
    void updatePosition();

private:
    QWidget *myParent;
};

#endif // MYINPUTPANEL_H
