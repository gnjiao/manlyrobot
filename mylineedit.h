#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H
#include <QLineEdit>
#include <QPainter>
#include <QWidget>
#include "myinputpanel.h"

class MyLineEdit : public QLineEdit
{
        Q_OBJECT

public:
       MyLineEdit(QWidget *parent=0);
       ~MyLineEdit();

       MyInputPanelContext *context;
       QPoint myPos;
       int parentHeight;
protected:
    //void paintEvent(QPaintEvent *event);
    virtual void focusInEvent(QFocusEvent *e);
    virtual void focusOutEvent(QFocusEvent *e);

private:
    QWidget *myParent;
};
#endif // MYLINEEDIT_H
