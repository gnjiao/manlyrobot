#include "global.h"
#include "testiodlg.h"

TestIoDlg::TestIoDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=650;
    dlgHeight=500;
    alpha=200;
}

void TestIoDlg::initDialog()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    QFont ft;
    ft.setPointSize(15);
    ft.setItalic(false);
    ft.setBold(true);
    checkButton=new QButtonGroup;
    connect(checkButton, SIGNAL(buttonClicked(int)), this, SLOT(OnCheckBtnGroupClicked(int)));
    //connect(checkButton, SIGNAL(buttonToggled(int , bool)), this, SLOT(OnBtnGroupToggled(int, bool)));
    connect(checkButton, SIGNAL(buttonPressed(int)), this, SLOT(OnBtnGroupPressed(int)));
    for(int i=0;i<16;i++)
    {
        buttonChecked[i]=false;
        QPixmap img(":/light/res/light1.png");
        gpiLabel[i]=new QLabel;
        inputLabel[i]=new QLabel;
        gpoLabel[i]=new QLabel;
        checkButton->addButton(new QPushButton(QString("%1").arg(i)),i);
        if(i<8)
        {
            orgLabel[i]=new QLabel;
            orgLabel[i]->setPixmap(img);
            orgLabel[i]->setAlignment(Qt::AlignCenter);
        }
        gpiLabel[i]->setPixmap(img);
        inputLabel[i]->setPixmap(img);
        gpoLabel[i]->setPixmap(img);
        gpiLabel[i]->setAlignment(Qt::AlignCenter);
        inputLabel[i]->setAlignment(Qt::AlignCenter);
        gpoLabel[i]->setAlignment(Qt::AlignCenter);
    }
    QPixmap img(":/light/res/light1.png");
    estopLabel=new QLabel;
    estopLabel->setPixmap(img);
    estopLabel->setAlignment(Qt::AlignCenter);

    QGridLayout *ioLayout = new QGridLayout;
    QLabel *lb=new QLabel("GPI");
    lb->setFont(ft);
    ioLayout->addWidget(lb,0,0,2,1);
    for(int i=0;i<8;i++)
    {
        ioLayout->addWidget(gpiLabel[i],0,i+1);
        ioLayout->addWidget(gpiLabel[i+8],1,i+1);
    }
    QLabel *lb2=new QLabel("LMIN");
    lb2->setFont(ft);
    ioLayout->addWidget(lb2,2,0,2,1);
    for(int i=0;i<8;i++)
    {
        ioLayout->addWidget(inputLabel[i],2,i+1);
        ioLayout->addWidget(inputLabel[i+8],3,i+1);
    }
    QLabel *lb3=new QLabel("ORG");
    lb3->setFont(ft);
    ioLayout->addWidget(lb3,4,0,1,1);
    for(int i=0;i<8;i++)
    {
        ioLayout->addWidget(orgLabel[i],4,i+1);
    }
    for(int i=0;i<8;i++)
    {
        ioLayout->addWidget(checkButton->button(i),5,i+1);
        ioLayout->addWidget(checkButton->button(i+8),6,i+1);
    }
    QLabel *lb4=new QLabel("GPO");
    lb4->setFont(ft);
    ioLayout->addWidget(lb4,7,0,2,1);
    for(int i=0;i<8;i++)
    {
        ioLayout->addWidget(gpoLabel[i],7,i+1);
        ioLayout->addWidget(gpoLabel[i+8],8,i+1);
    }
    QLabel *lbl=new QLabel(tr("急停信号"));
    lbl->setFont(ft);
    ioLayout->addWidget(lbl,9,0,1,2);
    ioLayout->addWidget(estopLabel,9,2);

    QPushButton *closeButton=new QPushButton(tr("关闭"));
    closeButton->setFont(ft);
    connect(closeButton, SIGNAL(clicked()), this, SLOT(OnCloseBtnClicked()));
    ioLayout->addWidget(closeButton,10,3,1,3);
    setLayout(ioLayout);
    resize(dlgWidth,dlgHeight);

    ioTimer=new QTimer(this);
    connect(ioTimer, SIGNAL(timeout()), this, SLOT(getIoStatus()));
    ioTimer->start(50);
}

void TestIoDlg::getIoStatus()
{
    QPixmap img1(":/light/res/light1.png");
    QPixmap img2(":/light/res/light2.png");
    for(int i=0;i<16;i++)
    {
        if(g_ioStatus.gpi[i]==0)
            gpiLabel[i]->setPixmap(img1);
        else
            gpiLabel[i]->setPixmap(img2);
        if(g_ioStatus.lmin[i]==0)
            inputLabel[i]->setPixmap(img1);
        else
            inputLabel[i]->setPixmap(img2);
        if(g_ioStatus.gpo[i]==0)
            gpoLabel[i]->setPixmap(img1);
        else
            gpoLabel[i]->setPixmap(img2);

        if(i<8)
        {
            if(g_ioStatus.org[i]==0)
                orgLabel[i]->setPixmap(img1);
            else
                orgLabel[i]->setPixmap(img2);
        }
    }
    if(g_ioStatus.eStop==0)    //急停信号输入读取
        estopLabel->setPixmap(img2);
    else
        estopLabel->setPixmap(img1);
}

void TestIoDlg::OnCheckBtnGroupClicked(int id)
{
    if(buttonChecked[id])
    {
        buttonChecked[id]=false;
        checkButton->button(id)->setStyleSheet("background-color:#CCCCCC");
        robot->gpoClear((unsigned char)id);     //开关量输出0
        printf("id=%d, checked=%d\n",id,0);
    }
    else
    {
        buttonChecked[id]=true;
        //checkButton->button(id)->setChecked(true);
        checkButton->button(id)->setStyleSheet("background-color:#00FF00");
        robot->gpoSet((unsigned char)id);       //开关量输出1
        printf("id=%d, checked=%d\n",id,1);
    }
}

void TestIoDlg::OnBtnGroupPressed(int /*id*/)
{
}

void TestIoDlg::OnBtnGroupToggled(int /*id*/, bool /*checked*/)
{
}

void TestIoDlg::OnCloseBtnClicked()
{
    ioTimer->stop();
    delete ioTimer;
    done(QDialog::Accepted);
}

void TestIoDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,alpha));
}
void TestIoDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}
void TestIoDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}
