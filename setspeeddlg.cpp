#include "setspeeddlg.h"
#include "myrwxml.h"

SetSpeedDlg::SetSpeedDlg(QWidget *parent) :
    QDialog(parent)
{
    dlgWidth=700;
    dlgHeight=300;
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<21;j++)
            speedPara.maxSpeed[i][j]=50000;
    }
}

void SetSpeedDlg::initDialog()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Background, QColor(MAIN_WIDGET_COLOR));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    QFont ft;
    ft.setPointSize(DLG_LABEL_BUTTON_FONT);
    ft.setItalic(false);
    QLabel *axisLabel[9];
    for(int i=0;i<9;i++)
    {
        axisLabel[i]=new QLabel;
        axisLabel[i]->setFont(ft);
        axisLabel[i]->setAlignment(Qt::AlignCenter);
    }
    axisLabel[0]->setText(tr("距离编号"));
    axisLabel[1]->setText(tr("X轴"));
    axisLabel[2]->setText(tr("Y轴"));
    axisLabel[3]->setText(tr("Z轴"));
    axisLabel[4]->setText(tr("R轴"));
    axisLabel[5]->setText(tr("S轴"));
    axisLabel[6]->setText(tr("R1轴"));
    axisLabel[7]->setText(tr("R2轴"));
    axisLabel[8]->setText(tr("E轴"));

    QLabel *distanceLabel[21];//距离编号
    for(int i=0;i<21;i++)
    {
        distanceLabel[i]=new QLabel;
        distanceLabel[i]->setFont(ft);
        distanceLabel[i]->setText(QString("%1").arg(i));
    }

    for(int i=0;i<8;i++)
        for(int j=0;j<21;j++)
        {
            maxSpeedEdit[i][j]=new MyLineEdit;
            maxSpeedEdit[i][j]->setFont(ft);
        }

    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(onOkBtnClicked()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnCancelClicked()));

    QGridLayout *axisSpeedLayout = new QGridLayout;
    int i,j;
    for(i=0;i<9;i++)
    {
        if(i==0)
        {
            axisSpeedLayout->addWidget(axisLabel[0],0,0);
            for(j=0;j<21;j++)
                axisSpeedLayout->addWidget(distanceLabel[j],j+1,0);
        }
        else
        {
            axisSpeedLayout->addWidget(axisLabel[i],0,i);
            for(j=0;j<21;j++)
                axisSpeedLayout->addWidget(maxSpeedEdit[i-1][j],j+1,i);
        }
    }
    axisSpeedLayout->addWidget(okBtn,22,0,1,4);
    axisSpeedLayout->addWidget(cancelBtn,22,5,1,4);

    setLayout(axisSpeedLayout);
    setWindowTitle(tr("防冲击速度设置"));
    resize(dlgWidth,dlgHeight);

    context=new MyInputPanelContext(this);
    context->inputPanel->hide();
    context->inputPanel->resize(150,150);

    MyRWxml xml;

    xml.readSpeedPara("robotspeed.xml",speedPara);
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<21;j++)
        {
            maxSpeedEdit[i][j]->setText(QString("%1").arg(speedPara.maxSpeed[i][j]));
        }
    }

    okBtn->setFocus(Qt::OtherFocusReason);

    QTimer::singleShot(200, this, SLOT(loadMyLineEdit()));
}

void SetSpeedDlg::onOkBtnClicked()
{
    if(QFile::exists("robotspeed.xml"))
    {
        QDir dir;
        dir.remove("robotspeed.xml");
    }
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<21;j++)
        {
            speedPara.maxSpeed[i][j]=maxSpeedEdit[i][j]->text().trimmed().toInt();
        }
    }

    MyRWxml xml;
    xml.writeSpeedPara("robotspeed.xml",speedPara);
    done(QDialog::Accepted);
}

void SetSpeedDlg::OnCancelClicked()
{
    done(QDialog::Rejected);
}

void SetSpeedDlg::loadMyLineEdit()      //加载自定义文本框
{
    for(int i=0;i<8;i++)
        for(int j=0;j<21;j++)
    {
        maxSpeedEdit[i][j]->context=context;
        QRect rect=maxSpeedEdit[i][j]->geometry();
        maxSpeedEdit[i][j]->myPos=QPoint(rect.left()-100,rect.top());
        maxSpeedEdit[i][j]->parentHeight=geometry().height()+60;
    }
}
