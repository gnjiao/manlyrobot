#include "myinputpanel.h"

/**Reimplemented NumberButton class*/
NumberButton::NumberButton(const QString &iconPath,const char *pressIconPath/* =0 */, QWidget *parent/* =0 */)
: QPushButton(parent)
{
    setPixmap(iconPath);
    if(pressIconPath)
        setPressedPixmap(pressIconPath);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setBkPalette(0);//设置背景完全透明
    setFlat(true);
    setAutoFillBackground(true);
}

NumberButton::NumberButton(QWidget *parent /* = 0 */)
: QPushButton(parent)
{
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
}


NumberButton::~NumberButton()
{

}

void NumberButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, width(), height(), pixmap);
}

QSize NumberButton::sizeHint() const
{
    return pixmap.size();
}

void NumberButton::setPixmap(const QString &path)
{
    pixmap.load(path);
    releasePixmap.load(path);
    update();
}

void NumberButton::setPressedPixmap(const QString &path)
{
    pressedPixmap.load(path);
//    update();
}

void NumberButton::setBkPalette(int transparency)//设置背景透明度releasePixmap
{
    QPalette palette;
    palette.setBrush(QPalette::Button,QBrush(QColor(255,255,255,transparency)));
    setPalette(palette);
}

void NumberButton::mousePressEvent(QMouseEvent *)
{
    setBkPalette(150);
    if(pressedPixmap.width()>0)
        pixmap=pressedPixmap;
    emit pressed();
}
void NumberButton::mouseReleaseEvent(QMouseEvent *)
{
    setBkPalette(0);
    pixmap=releasePixmap;
    emit clicked();//必须写上，否则不会发出clicked信号
}
/*
void NumberButton::buttonClicked(QWidget *w)
{
    QChar chr = qvariant_cast<QChar>(w->property("buttonValue"));
    emit characterGenerated(chr);
}
*/
/**Reimplemented MyInputPanel class*/
MyInputPanel::MyInputPanel(QWidget *parent) :
    QWidget(parent), lastFocusedWidget(0)
{
    //printf( "Execute the MyInputPanel::MyInputPanel() function.\n");
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    panelButton_1=new NumberButton(":/picture/res/num1_1.png",":/picture/res/num1_2.png");
    panelButton_2=new NumberButton(":/picture/res/num2_1.png",":/picture/res/num2_2.png");
    panelButton_3=new NumberButton(":/picture/res/num3_1.png",":/picture/res/num3_2.png");
    panelButton_4=new NumberButton(":/picture/res/num4_1.png",":/picture/res/num4_2.png");
    panelButton_5=new NumberButton(":/picture/res/num5_1.png",":/picture/res/num5_2.png");
    panelButton_6=new NumberButton(":/picture/res/num6_1.png",":/picture/res/num6_2.png");
    panelButton_7=new NumberButton(":/picture/res/num7_1.png",":/picture/res/num7_2.png");
    panelButton_8=new NumberButton(":/picture/res/num8_1.png",":/picture/res/num8_2.png");
    panelButton_9=new NumberButton(":/picture/res/num9_1.png",":/picture/res/num9_2.png");
    panelButton_dot=new NumberButton(":/picture/res/dot1.png",":/picture/res/dot2.png");
    panelButton_0=new NumberButton(":/picture/res/num0_1.png",":/picture/res/num0_2.png");
    panelButton_bp=new NumberButton(":/picture/res/bp1.png",":/picture/res/bp2.png");
    panelButton_1->setProperty("buttonValue",QVariant(QChar('1')));
    panelButton_2->setProperty("buttonValue",QVariant(QChar('2')));
    panelButton_3->setProperty("buttonValue",QVariant(QChar('3')));
    panelButton_4->setProperty("buttonValue",QVariant(QChar('4')));
    panelButton_5->setProperty("buttonValue",QVariant(QChar('5')));
    panelButton_6->setProperty("buttonValue",QVariant(QChar('6')));
    panelButton_7->setProperty("buttonValue",QVariant(QChar('7')));
    panelButton_8->setProperty("buttonValue",QVariant(QChar('8')));
    panelButton_9->setProperty("buttonValue",QVariant(QChar('9')));
    panelButton_dot->setProperty("buttonValue",QVariant(QChar(Qt::Key_Period)));//0x2e
    panelButton_0->setProperty("buttonValue",QVariant(QChar('0')));
    panelButton_bp->setProperty("buttonValue",QVariant(QChar(Qt::Key_Backspace)));//0x01000003
    panelButton_1->setFocusPolicy(Qt::NoFocus);
    panelButton_2->setFocusPolicy(Qt::NoFocus);
    panelButton_3->setFocusPolicy(Qt::NoFocus);
    panelButton_4->setFocusPolicy(Qt::NoFocus);
    panelButton_5->setFocusPolicy(Qt::NoFocus);
    panelButton_6->setFocusPolicy(Qt::NoFocus);
    panelButton_7->setFocusPolicy(Qt::NoFocus);
    panelButton_8->setFocusPolicy(Qt::NoFocus);
    panelButton_9->setFocusPolicy(Qt::NoFocus);
    panelButton_dot->setFocusPolicy(Qt::NoFocus);
    panelButton_0->setFocusPolicy(Qt::NoFocus);
    panelButton_bp->setFocusPolicy(Qt::NoFocus);

    mainLayout = new QGridLayout;
    mainLayout->addWidget(panelButton_1, 0, 0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_2, 0, 1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_3, 0, 2,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_4, 1, 0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_5, 1, 1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_6, 1, 2,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_7, 2, 0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_8, 2, 1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_9, 2, 2,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_dot, 3, 0,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_0, 3, 1,1,1,Qt::AlignCenter);
    mainLayout->addWidget(panelButton_bp, 3, 2,1,1,Qt::AlignCenter);
    this->setLayout(mainLayout);

//    connect(parent, SIGNAL(focusChanged(QWidget*,QWidget*)),this, SLOT(saveFocusWidget(QWidget*,QWidget*)));

    signalMapper.setMapping(panelButton_1, panelButton_1);
    signalMapper.setMapping(panelButton_2, panelButton_2);
    signalMapper.setMapping(panelButton_3, panelButton_3);
    signalMapper.setMapping(panelButton_4, panelButton_4);
    signalMapper.setMapping(panelButton_5, panelButton_5);
    signalMapper.setMapping(panelButton_6, panelButton_6);
    signalMapper.setMapping(panelButton_7, panelButton_7);
    signalMapper.setMapping(panelButton_8, panelButton_8);
    signalMapper.setMapping(panelButton_9, panelButton_9);
    signalMapper.setMapping(panelButton_dot, panelButton_dot);
    signalMapper.setMapping(panelButton_0, panelButton_0);
    signalMapper.setMapping(panelButton_bp, panelButton_bp);

    connect(panelButton_1, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_2, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_3, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_4, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_5, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_6, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_7, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_8, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_9, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_dot, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_0, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));
    connect(panelButton_bp, SIGNAL(clicked()),
            &signalMapper, SLOT(map()));

    connect(&signalMapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(buttonClicked(QWidget*)));

    //printf( "Leave the MyInputPanel::MyInputPanel() function.\n");
}

//! [0]

bool MyInputPanel::event(QEvent *e)
{
    switch (e->type())
    {
    //! [1]
    case QEvent::WindowActivate:
        if (lastFocusedWidget)
            lastFocusedWidget->activateWindow();
        break;
    //! [1]
//    case QEvent::FocusIn:
//        emit focusChanged(lastFocusedWidget,)
        break;

    default:
        break;
    }

    return QWidget::event(e);
}

void MyInputPanel::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(25,100,100,200));
}

void MyInputPanel::mousePressEvent(QMouseEvent */*event*/)
{
    //this->windowPos = this->pos();
    //this->mousePos = event->globalPos();
    //this->dPos = mousePos - windowPos;
}

void MyInputPanel::mouseMoveEvent(QMouseEvent */*event*/)
{
    //this->move(event->globalPos() - this->dPos);
}

//! [2]

void MyInputPanel::saveFocusWidget(QWidget * /*oldFocus*/, QWidget *newFocus)
{
    //printf( "Execute the MyInputPanel::saveFocusWidget() function.\n");
    if (newFocus != 0 && !this->isAncestorOf(newFocus))
    {
        lastFocusedWidget = newFocus;
    }
}

//! [2]

//! [3]

void MyInputPanel::buttonClicked(QWidget *w)
{
//    printf( "Execute the MyInputPanel::buttonClicked() function.\n");
    QChar chr = qvariant_cast<QChar>(w->property("buttonValue"));
    emit characterGenerated(chr);
}

//! [3]


/**Reimplemented MyInputPanelContext class*/
//! [0]

MyInputPanelContext::MyInputPanelContext(QWidget *parent)
{
    //printf( "Execute the MyInputPanelContext::MyInputPanelContext() function.\n");
    inputPanel = new MyInputPanel(parent);
    connect(inputPanel, SIGNAL(characterGenerated(QChar)), SLOT(sendCharacter(QChar)));
    myParent=parent;
}

//! [0]

MyInputPanelContext::~MyInputPanelContext()
{
    delete inputPanel;
}

//! [1]

bool MyInputPanelContext::filterEvent(const QEvent* event)
{
    printf( "Execute the MyInputPanelContext::filterEvent() function.\n");
    if (event->type() == QEvent::RequestSoftwareInputPanel) {
        //updatePosition();
        inputPanel->show();
        return true;
    } else if (event->type() == QEvent::CloseSoftwareInputPanel) {
        inputPanel->hide();
        return true;
    }
    return false;
}

//! [1]

QString MyInputPanelContext::identifierName()
{
    return "MyInputPanelContext";
}

void MyInputPanelContext::reset()
{
}

bool MyInputPanelContext::isComposing() const
{
    return false;
}

QString MyInputPanelContext::language()
{
    return "en_US";
}

//! [2]

void MyInputPanelContext::sendCharacter(QChar character)
{
    //printf( "Execute the MyInputPanelContext::sendCharacter() function.The char is '%c'\n",character.toLatin1());
    QPointer<QWidget> w = myParent->focusWidget();

    if (!w)
        return;

    //QWSServer::sendKeyEvent(0x01000003,Qt::Key_Backspace,Qt::NoModifier,true,false);
    if(character.toLatin1() == 3)  // backspace
    {
        QKeyEvent keyPress(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
        QCoreApplication::sendEvent(w, &keyPress);
    }
    else
    {
        QKeyEvent keyPress(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString(character));
        QCoreApplication::sendEvent(w, &keyPress);
    }
/*****不知为啥，要是把上面的keyPress封住，只用下面的keyRelease事件，则不能输入。如果两个都用，则每次输入两个字符.
    if (!w)
        return;

    if(character.toAscii() == 3)  // backspace
    {
        QKeyEvent keyRelease(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier, QString());
        QApplication::sendEvent(w, &keyRelease);
    }
    else
    {
        QKeyEvent keyRelease(QEvent::KeyPress, character.unicode(), Qt::NoModifier, QString());
        QApplication::sendEvent(w, &keyRelease);
    }*/
}

//! [2]

//! [3]

void MyInputPanelContext::updatePosition()
{
/*    QWidget *widget = focusWidget();
    if (!widget)
        return;

    QRect widgetRect = widget->rect();
    QPoint panelPos = QPoint(widgetRect.left(), widgetRect.bottom() + 2);
    panelPos = widget->mapToGlobal(panelPos);
    inputPanel->move(panelPos);*/
}

//! [3]
