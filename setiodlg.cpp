#include "setiodlg.h"
#include "myrwxml.h"

SetIoDlg::SetIoDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=800;
    dlgHeight=400;
    //alpha=200;
    for(int i=0;i<33;i++)
        inItemSelect[i]=false;
    for(int i=0;i<17;i++)
        outItemSelect[i]=false;
    currIndex=-1;
}

void SetIoDlg::paintEvent(QPaintEvent */*event*/)
{
/*    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 10); //圆角设置
    //半透明背景
    painter.fillPath(path, QColor(DIALOG_BACKGROUND_COLOR,alpha));*/
}
/*
void SetIoDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}
void SetIoDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}
*/


void SetIoDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);

    //////////////////////////////////////////////////////////////////////////系统功能IO配置////////////////////////////////////////////
    QLabel *sysLabel=new QLabel(tr("系统功能IO配置"));
    sysLabel->setFont(ft);

    QList <QLabel *> listLabel,listLabel2;
    QStringList hListStr,vListStr;
    hListStr<<tr("功能项")<<tr("对应I定义")<<tr("功能项")<<tr("对应I定义")<<tr("功能项")<<tr("对应I定义")<<tr("功能项")<<tr("对应OUT定义");
    vListStr<<tr("X轴-")<<tr("Y轴-")<<tr("X轴+")<<tr("Y轴+")<<tr("手柄选择_1")<<tr("手柄选择_2")<<tr("送锡")<<tr("退锡");
    for(int i=0;i<8;i++)
    {
        listLabel.append(new QLabel(hListStr[i]));
        listLabel[i]->setFont(ft);
        listLabel2.append(new QLabel(hListStr[i]));
        listLabel2[i]->setFont(ft);
    }

    QLabel *funLabel[8],*funLabel2[8],*funLabel3[8],*funLabel4[8];
    for(int i=0;i<8;i++)
    {
        funLabel[i]=new QLabel;
        funLabel[i]->setFont(ft);
        funLabel[i]->setText(vListStr[i]);
        sysInputComBox1[i]=new QComboBox;
        sysInputComBox1[i]->setFont(ft);
        sysInputComBox2[i]=new QComboBox;
        sysInputComBox2[i]->setFont(ft);
        sysInputComBox3[i]=new QComboBox;
        sysInputComBox3[i]->setFont(ft);
        sysOutputComBox[i]=new QComboBox;
        sysOutputComBox[i]->setFont(ft);
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<16;j++)
        {
            sysInputComBox1[i]->addItem(QString("GPI%1").arg(j));
            sysInputComBox2[i]->addItem(QString("GPI%1").arg(j));
            sysInputComBox3[i]->addItem(QString("GPI%1").arg(j));
            sysOutputComBox[i]->addItem(QString("GPO%1").arg(j));
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<16;j++)
        {
            sysInputComBox1[i]->addItem(QString("INPUT%1").arg(j));
            sysInputComBox2[i]->addItem(QString("INPUT%1").arg(j));
            sysInputComBox3[i]->addItem(QString("INPUT%1").arg(j));
        }
        sysInputComBox1[i]->addItem(tr("未配置"));
        sysInputComBox2[i]->addItem(tr("未配置"));
        sysInputComBox3[i]->addItem(tr("未配置"));
        sysOutputComBox[i]->addItem(tr("未配置"));
    }
    vListStr.clear();
    vListStr<<tr("key_BL")<<tr("key_FS")<<tr("key_START")<<tr("启动/停止按钮")<<tr("FIX_POS")<<tr("key_NEXT")<<tr("UP_POS")<<tr("复位");
    for(int i=0;i<8;i++)
    {
        funLabel2[i]=new QLabel;
        funLabel2[i]->setFont(ft);
        funLabel2[i]->setText(vListStr[i]);
    }
    vListStr.clear();
    vListStr<<tr("清锡按钮")<<tr("mac_TYPE0")<<tr("mac_TYPE1")<<tr("IRE1")<<tr("IRE2")<<tr("IRE3")<<tr("IRE4")<<tr("IRE5");
    for(int i=0;i<8;i++)
    {
        funLabel3[i]=new QLabel;
        funLabel3[i]->setFont(ft);
        funLabel3[i]->setText(vListStr[i]);
    }
    vListStr.clear();
    vListStr<<tr("清锡继电器")<<tr("启动/停止灯")<<tr("Cylinder")<<tr("ORE1")<<tr("ORE2")<<tr("ORE3")<<tr("ORE4")<<tr("ORE5");
    for(int i=0;i<8;i++)
    {
        funLabel4[i]=new QLabel;
        funLabel4[i]->setFont(ft);
        funLabel4[i]->setText(vListStr[i]);
    }

    QGridLayout *inLayout = new QGridLayout;
    inLayout->addWidget(sysLabel,0,0,1,8);
    for(int i=0;i<8;i++)
        inLayout->addWidget(listLabel[i],1,i);
    for(int i=0;i<8;i++)
    {
        inLayout->addWidget(funLabel[i],i+2,0);
        inLayout->addWidget(sysInputComBox1[i],i+2,1);
        inLayout->addWidget(funLabel2[i],i+2,2);
        inLayout->addWidget(sysInputComBox2[i],i+2,3);
        inLayout->addWidget(funLabel3[i],i+2,4);
        inLayout->addWidget(sysInputComBox3[i],i+2,5);
        inLayout->addWidget(funLabel4[i],i+2,6);
        inLayout->addWidget(sysOutputComBox[i],i+2,7);
    }
    //////////////////////////////////////////////////////////////////////////客户功能IO配置////////////////////////////////////////////
    QLabel *cusLabel=new QLabel(tr("客户可使用IO配置"));
    cusLabel->setFont(ft);

    vListStr.clear();
    vListStr<<tr("IN0")<<tr("IN1")<<tr("IN2")<<tr("IN3")<<tr("IN4")<<tr("IN5")<<tr("IN6")<<tr("IN7");
    QLabel *cfunLabel[8],*cfunLabel2[8],*cfunLabel3[8],*cfunLabel4[8];
    for(int i=0;i<8;i++)
    {
        cfunLabel[i]=new QLabel;
        cfunLabel[i]->setFont(ft);
        cfunLabel[i]->setText(vListStr[i]);
        cusInputComBox1[i]=new QComboBox;
        cusInputComBox1[i]->setFont(ft);
        cusInputComBox2[i]=new QComboBox;
        cusInputComBox2[i]->setFont(ft);
        cusInputComBox3[i]=new QComboBox;
        cusInputComBox3[i]->setFont(ft);
        cusOutputComBox[i]=new QComboBox;
        cusOutputComBox[i]->setFont(ft);
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<16;j++)
        {
            cusInputComBox1[i]->addItem(QString("GPI%1").arg(j));
            cusInputComBox2[i]->addItem(QString("GPI%1").arg(j));
            cusInputComBox3[i]->addItem(QString("GPI%1").arg(j));
            cusOutputComBox[i]->addItem(QString("GPO%1").arg(j));
        }
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<16;j++)
        {
            cusInputComBox1[i]->addItem(QString("INPUT%1").arg(j));
            cusInputComBox2[i]->addItem(QString("INPUT%1").arg(j));
            cusInputComBox3[i]->addItem(QString("INPUT%1").arg(j));
        }
        cusInputComBox1[i]->addItem(tr("未配置"));
        cusInputComBox2[i]->addItem(tr("未配置"));
        cusInputComBox3[i]->addItem(tr("未配置"));
        cusOutputComBox[i]->addItem(tr("未配置"));
    }
    vListStr.clear();
    vListStr<<tr("IN8")<<tr("IN9")<<tr("IN10")<<tr("IN11")<<tr("IN12")<<tr("IN13")<<tr("IN14")<<tr("IN15");
    for(int i=0;i<8;i++)
    {
        cfunLabel2[i]=new QLabel;
        cfunLabel2[i]->setFont(ft);
        cfunLabel2[i]->setText(vListStr[i]);
    }
    vListStr.clear();
    vListStr<<tr("IN16")<<tr("IN17")<<tr("IN18")<<tr("IN19")<<tr("IN20")<<tr("IN21")<<tr("IN22")<<tr("IN23");
    for(int i=0;i<8;i++)
    {
        cfunLabel3[i]=new QLabel;
        cfunLabel3[i]->setFont(ft);
        cfunLabel3[i]->setText(vListStr[i]);
    }
    vListStr.clear();
    vListStr<<tr("OUT0")<<tr("OUT1")<<tr("OUT2")<<tr("OUT3")<<tr("OUT4")<<tr("OUT5")<<tr("OUT6")<<tr("OUT7");
    for(int i=0;i<8;i++)
    {
        cfunLabel4[i]=new QLabel;
        cfunLabel4[i]->setFont(ft);
        cfunLabel4[i]->setText(vListStr[i]);
    }

    QGridLayout *inOLayout = new QGridLayout;
    inOLayout->addWidget(cusLabel,0,0,1,8);
    for(int i=0;i<8;i++)
        inOLayout->addWidget(listLabel2[i],1,i);
    for(int i=0;i<8;i++)
    {
        inOLayout->addWidget(cfunLabel[i],i+2,0);
        inOLayout->addWidget(cusInputComBox1[i],i+2,1);
        inOLayout->addWidget(cfunLabel2[i],i+2,2);
        inOLayout->addWidget(cusInputComBox2[i],i+2,3);
        inOLayout->addWidget(cfunLabel3[i],i+2,4);
        inOLayout->addWidget(cusInputComBox3[i],i+2,5);
        inOLayout->addWidget(cfunLabel4[i],i+2,6);
        inOLayout->addWidget(cusOutputComBox[i],i+2,7);
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////

    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelClicked()));


    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(inLayout,0,0,10,1);
    mainLayout->addLayout(inOLayout,11,0,10,1);
    mainLayout->addLayout(btnLayout,22,0,1,1);

    setLayout(mainLayout);
    setWindowTitle(tr("IO配置"));
    resize(dlgWidth,dlgHeight);

    MyRWxml xml;
    IO_SET  IOxb;
    xml.readIoPara("robotio.xml",IOxb);
    for(int i=0;i<8;i++)
    {
        sysInputComBox1[i]->setCurrentIndex(IOxb.sysIxb[i]);
        sysInputComBox2[i]->setCurrentIndex(IOxb.sysIxb[i+8]);
        sysInputComBox3[i]->setCurrentIndex(IOxb.sysIxb[i+16]);
        sysOutputComBox[i]->setCurrentIndex(IOxb.sysOxb[i]);
        cusInputComBox1[i]->setCurrentIndex(IOxb.cusIxb[i]);
        cusInputComBox2[i]->setCurrentIndex(IOxb.cusIxb[i+8]);
        cusInputComBox3[i]->setCurrentIndex(IOxb.cusIxb[i+16]);
        cusOutputComBox[i]->setCurrentIndex(IOxb.cusOxb[i]);
    }
    for(int i=0;i<24;i++)   //为输入QComboBox都选了哪个选项设置标记
    {
        if(IOxb.sysIxb[i]<32)
            inItemSelect[IOxb.sysIxb[i]]=true;
        if(IOxb.cusIxb[i]<32)
            inItemSelect[IOxb.cusIxb[i]]=true;
    }
    for(int i=0;i<8;i++)   //为输出QComboBox都选了哪个选项设置标记highlighted
    {
        if(IOxb.sysOxb[i]<16)
            outItemSelect[IOxb.sysOxb[i]]=true;
        if(IOxb.cusOxb[i]<16)
            outItemSelect[IOxb.cusOxb[i]]=true;
    }
    for(int i=0;i<8;i++)
    {
        connect(sysInputComBox1[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onInSelectItem(int )));
        connect(sysInputComBox2[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onInSelectItem(int )));
        connect(sysInputComBox3[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onInSelectItem(int )));
        connect(cusInputComBox1[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onInSelectItem(int )));
        connect(cusInputComBox2[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onInSelectItem(int )));
        connect(cusInputComBox3[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onInSelectItem(int )));

        connect(sysInputComBox1[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
        connect(sysInputComBox2[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
        connect(sysInputComBox3[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
        connect(cusInputComBox1[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
        connect(cusInputComBox2[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
        connect(cusInputComBox3[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
    }
    for(int i=0;i<8;i++)
    {
        connect(sysOutputComBox[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onOutSelectItem(int )));
        connect(cusOutputComBox[i], SIGNAL(currentIndexChanged(int)), this, SLOT(onOutSelectItem(int )));
        connect(sysOutputComBox[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
        connect(cusOutputComBox[i], SIGNAL(highlighted(int)), this, SLOT(onHighlightedItem(int)));
    }
}

void SetIoDlg::onOkBtnClicked()
{
    if(QFile::exists("robotio.xml"))
    {
        QDir dir;
        dir.remove("robotio.xml");
    }
    IO_SET  IOxb;
    for(int i=0;i<8;i++)
    {
        IOxb.sysIxb[i]=sysInputComBox1[i]->currentIndex();
        IOxb.sysIxb[i+8]=sysInputComBox2[i]->currentIndex();
        IOxb.sysIxb[i+16]=sysInputComBox3[i]->currentIndex();
        IOxb.sysOxb[i]=sysOutputComBox[i]->currentIndex();
        IOxb.cusIxb[i]=cusInputComBox1[i]->currentIndex();
        IOxb.cusIxb[i+8]=cusInputComBox2[i]->currentIndex();
        IOxb.cusIxb[i+16]=cusInputComBox3[i]->currentIndex();
        IOxb.cusOxb[i]=cusOutputComBox[i]->currentIndex();
    }
    MyRWxml xml;
    xml.writeIoPara("robotio.xml",IOxb);
    xml.readIoPara("robotio.xml",g_IOxb);
    done(QDialog::Accepted);
}

void SetIoDlg::OnCancelClicked()
{
    done(QDialog::Rejected);
}

void SetIoDlg::onInSelectItem(int index)
{
    QComboBox * box=(QComboBox *)sender();
    if(!inItemSelect[index])
    {
        if(index<32)
        {
            box->setCurrentIndex(index);
            inItemSelect[currIndex]=false;
            inItemSelect[index]=true;
        }
        else if(index==32)
        {
            inItemSelect[currIndex]=false;
        }
    }
    else
    {
        if(index==32)
            box->setCurrentIndex(index);
        else
            box->setCurrentIndex(currIndex);
    }
}

void SetIoDlg::onOutSelectItem(int index)
{
    QComboBox * box=(QComboBox *)sender();
    if(!outItemSelect[index])
    {
        if(index<16)
        {
            box->setCurrentIndex(index);
            outItemSelect[currIndex]=false;
            outItemSelect[index]=true;
        }
        else if(index==16)
        {
            outItemSelect[currIndex]=false;
        }
    }
    else
    {
        if(index==16)
            box->setCurrentIndex(index);
        else
            box->setCurrentIndex(currIndex);
    }
}

void SetIoDlg::onHighlightedItem(int /*index*/) //主要是为了获得组合框中当前项的索引
{
    QComboBox * box=(QComboBox *)sender();
    currIndex=box->currentIndex();
}
