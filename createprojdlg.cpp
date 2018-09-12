#include "global.h"
#include "createprojdlg.h"

CreateProjDlg::CreateProjDlg(QWidget *parent) :
    QDialog(parent)
{
    isOK=false;
    inoutFlag=0;
    initDialog();
}
void CreateProjDlg::initDialog()
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowOpacity(0.5);
    setStyleSheet("QLineEdit { background-color: QColor(255,255,0,10) ; color:white; border:1px groove white;border-radius:10px; padding:2px 4px;} \
                   QPushButton { background-color: qconicalgradient(cx:0.5, cy:0.522909, angle:179.9, stop:0.494318 rgba(214, 214, 214, 100), stop:0.5 rgba(236, 236, 236, 100)); \
                                 border: 1px solid rgb(124, 124, 124); border-radius:7px; color:black;}" \
                  "QPushButton:pressed{ background:rgba(0,0,255,150); }");
    //setStyleSheet("QPushButton { background-color: QColor(255,255,0,10) ; color:black;}");
    //setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::Button, QColor(255,0,0,100));
    //palette.setColor(QPalette::Background, QColor(DIALOG_BACKGROUND_COLOR));
    //this->setAutoFillBackground(true);
    //this->setPalette(palette);
    //palette.setColor(QPalette::WindowText, QColor(0,0,0));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *projNameLabel=new QLabel(tr("项目名称"));
    projNameLabel->setFont(ft);
    nameEdit=new QLineEdit;
    nameEdit->setFont(ft);
    //palette.setColor (QPalette::Background, QColor(255,0,0,100));
    //nameEdit->setPalette(palette);
    //nameEdit->setStyleSheet("border:1px groove white;border-radius:10px; padding:2px 4px;");
    nameEdit->setAlignment(Qt::AlignCenter);
    QLabel *directionLabel=new QLabel(tr("镀锡面方向"));
    directionLabel->setFont(ft);
    directEdit=new QLineEdit;
    directEdit->setFont(ft);
    //directEdit->setStyleSheet("border:1px groove white;border-radius:10px;padding:1px 1px;");
    directEdit->setAlignment(Qt::AlignCenter);

    QHBoxLayout *projNameLayout = new QHBoxLayout;
    projNameLayout->addWidget(projNameLabel);
    projNameLayout->addWidget(nameEdit);
    projNameLayout->addStretch();

    QHBoxLayout *directionLayout = new QHBoxLayout;
    directionLayout->addWidget(directionLabel);
    directionLayout->addWidget(directEdit);

    upButton=new QPushButton(tr("前面"));
    upButton->setFont(ft);
    downButton=new QPushButton(tr("后面"));
    downButton->setFont(ft);
    leftButton=new QPushButton(tr("左面"));
    leftButton->setFont(ft);
    rightButton=new QPushButton(tr("右面"));
    rightButton->setFont(ft);
    upButton->setFixedSize(this->geometry().width()*2/3,this->geometry().height()/2);
    downButton->setFixedSize(this->geometry().width()*2/3,this->geometry().height()/2);
    leftButton->setFixedSize(this->geometry().width()*2/3,this->geometry().height()/2);
    rightButton->setFixedSize(this->geometry().width()*2/3,this->geometry().height()/2);
    connect(upButton, SIGNAL(clicked()), this, SLOT(OnClickedUpButton()));
    connect(downButton, SIGNAL(clicked()), this, SLOT(OnClickedDownButton()));
    connect(leftButton, SIGNAL(clicked()), this, SLOT(OnClickedLeftButton()));
    connect(rightButton, SIGNAL(clicked()), this, SLOT(OnClickedRightButton()));

    QGridLayout *btnLayout = new QGridLayout;
    btnLayout->addWidget(upButton, 0, 1);
    btnLayout->addWidget(leftButton, 1, 0);
    btnLayout->addWidget(downButton, 2, 1);
    btnLayout->addWidget(rightButton, 1, 2);

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addLayout(btnLayout);
    midLayout->addLayout(directionLayout);

    QPushButton *okBtn=new QPushButton(tr("确定"));
    okBtn->setFont(ft);
    okBtn->setMinimumWidth(this->geometry().width()/5);
    okBtn->setFixedSize(this->geometry().width()*2/3,this->geometry().height()/2);
    //okBtn->setPalette(palette);
    //okBtn->setStyleSheet("border:1px groove gray;border-radius:10px;padding:2px 4px;");
    connect(okBtn, SIGNAL(clicked()), this, SLOT(OnClickedOKButton()));
    QPushButton *cancelBtn=new QPushButton(tr("取消"));
    cancelBtn->setFont(ft);
    cancelBtn->setMinimumWidth(this->geometry().width()/5);
    cancelBtn->setFixedSize(this->geometry().width()*2/3,this->geometry().height()/2);
    //cancelBtn->setPalette(palette);
    connect(cancelBtn, SIGNAL(clicked()), this, SLOT(OnClickedCancelButton()));

    QLabel *caption=new QLabel(tr("新建项目"));
    ft.setPointSize(LABEL_BUTTON_FONT+5);
    caption->setFont(ft);
    caption->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ocbtnLayout = new QHBoxLayout;
    ocbtnLayout->addStretch();
    ocbtnLayout->addWidget(okBtn);
    ocbtnLayout->addStretch();
    ocbtnLayout->addWidget(cancelBtn);
    ocbtnLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(caption);
    layout->addStretch();
    layout->addLayout(projNameLayout);
    layout->addLayout(midLayout);
    layout->addStretch();
    layout->addLayout(ocbtnLayout);
    setLayout(layout);
    setWindowTitle(tr("新建项目"));
    resize(400,200);

    nameEdit->setFocus(Qt::OtherFocusReason);

    currentAlpha = 20;
    fadeTimes = 1000;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()),this, SLOT(update()));

    //setAttribute(Qt::WA_DeleteOnClose);
    startIn();

    QProcess p(0);
    p.start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
    p.waitForStarted();
    p.waitForFinished();
}

void CreateProjDlg::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);

    QPainterPath path;
    path.addRoundRect(this->rect(), 25); //圆角设置

    if(inoutFlag==1)
    {
        currentAlpha += 200 * timer->interval() / fadeTimes;
        if (currentAlpha >= 200)
        {
            inoutFlag=0;
            timer->stop();
        }
    }
    else if(inoutFlag==2)
    {
        currentAlpha -= 200 * timer->interval() / fadeTimes;
        if (currentAlpha <= 20)
        {
            inoutFlag=0;
            timer->stop();
            if(isOK)
                done(QDialog::Accepted);
            else
                done(QDialog::Rejected);
            return;
        }
    }

    QColor currentColor = startColor;
    currentColor.setAlpha(currentAlpha);
    //painter.fillRect(rect(), currentColor);

    //半透明背景
    painter.fillPath(path, currentColor);   //QColor(DIALOG_BACKGROUND_COLOR,200)
}

void CreateProjDlg::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();
    this->mousePos = event->globalPos();
    this->dPos = mousePos - windowPos;
}

void CreateProjDlg::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}

void CreateProjDlg::startIn() //淡入
{
    inoutFlag=1;
    startColor=QColor(DIALOG_BACKGROUND_COLOR);
    currentAlpha = 20;
    timer->start(100);
}

void CreateProjDlg::startOut() //淡出
{
    inoutFlag=2;
    timer->start(100);
}

void CreateProjDlg::OnClickedOKButton()
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    projectName=nameEdit->text();
    if(projectName.isEmpty())
    {
        msgBox.setText(tr("项目名称不能为空."));
        msgBox.exec();
        return;
    }
    QString str=directEdit->text();
    if(str.isEmpty())
    {
        msgBox.setText(tr("焊锡面朝向不能为空."));
        msgBox.exec();
        return;
    }
    //if(str==tr("左面")) direction=QString("left");
    //else if(str==tr("右面")) direction=QString("right");
    //else if(str==tr("前面")) direction=QString("up");
    //else direction=QString("down");
    isOK=true;
    startOut();
    //done(QDialog::Accepted);
}

void CreateProjDlg::OnClickedCancelButton()
{
    isOK=false;
    startOut();
    //done(QDialog::Rejected);
}

void CreateProjDlg::OnClickedUpButton()
{
    direction=QString("up");
    directEdit->setText(tr("前面"));
}

void CreateProjDlg::OnClickedDownButton()
{
    direction=QString("down");
    directEdit->setText(tr("后面"));
}

void CreateProjDlg::OnClickedLeftButton()
{
    direction=QString("left");
    directEdit->setText(tr("左面"));
}

void CreateProjDlg::OnClickedRightButton()
{
    direction=QString("right");
    directEdit->setText(tr("右面"));
}
