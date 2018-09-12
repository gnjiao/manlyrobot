//myLineEdit.cpp
#include "myLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *parent):QLineEdit(parent)
{   //"QLineEdit:hover{ border-image:url(:/btn_background.png); font:10pt}"
    setStyleSheet("QLineEdit{ background:rgba(0,0,255,0%); border:1px; }"
                        "QLineEdit:hover{ background:rgba(0,255,0,200); }"
                        "QLineEdit:!hover{background:rgba(0,0,255,200);}"
                        "QLineEdit:disabled{background:rgba(50,50,50,0);color:green ;border:1px groove gray; border-radius:10px}"
                        "QLineEdit:enabled{background:rgba(255,0,255,0);color:white ;border:1px groove white; border-radius:10px}"
                        "QLineEdit:focus {background:rgba(0,0,255,200);color:white ;border-image:none; border:1px groove lightgray; border-radius:10px;padding:1px 10px;}");
    myParent=parent;
    setFocusPolicy(Qt::StrongFocus);
    //QRect rect=geometry();
    myPos = QPoint(280,-20);//QPoint(rect.left(), rect.top());
    parentHeight=480;
}

MyLineEdit::~MyLineEdit()
{

}

void MyLineEdit::focusInEvent(QFocusEvent *e)
{
    this->setCursorMoveStyle(Qt::LogicalMoveStyle);
    context->inputPanel->show();
    int left,top;
    //QRect rect=geometry();
    //myPos = QPoint(rect.left(), rect.top());
    //myPos = this->parentWidget()->mapToParent(myPos);
    if(myPos.x()<200)
        left=myPos.x()+150;
    else //if((rect.left()>=200) && (rect.left()<rect_parent.right()-200))
        left=myPos.x()-200;

    if(myPos.y()>parentHeight-200)
        top=myPos.y()-220;
    else
        top=myPos.y()+20;
    context->inputPanel->move(left,top);
        //context->inputPanel->resize(200,200);
       /*QPalette p=QPalette();
       p.setColor(QPalette::Base,Qt::red);    //QPalette::Base 对可编辑输入框有效，还有其他类型，具体的查看文档
       setPalette(p);*/
   QLineEdit::focusInEvent(e);
}

void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
    clearFocus();
    context->inputPanel->hide();
       /*QPalette p1=QPalette();
       p1.setColor(QPalette::Base,Qt::yellow);
       setPalette(p1);*/
   QLineEdit::focusOutEvent(e);
}

//void MyLineEdit::paintEvent(QPaintEvent */*event*/)
/*{
    QPainter painter(this);

    //QPainterPath path;
    //path.addRoundRect(this->rect(), 5); //圆角设置
    //painter.fillPath(path, QColor(125,145,200,250));

    //QRectF rect(left, top, width, height);
    //painter.drawRect(rect);
    painter.drawRoundedRect(this->rect(),10.0,10.0);
    //反走样
    painter.setRenderHint(QPainter::Antialiasing, true);
}
*/
