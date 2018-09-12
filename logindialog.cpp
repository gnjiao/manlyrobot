#include "global.h"
#include "logindialog.h"
#include "msqldb.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    program=0;

    setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    QFont ft;
    palette.setColor(QPalette::WindowText, QColor(20,95,120));
    ft.setPointSize(15);
    ft.setItalic(false);

    QLabel *userNameLabel = new QLabel(tr("用户名"));//用户名
    QLabel *passwordLabel = new QLabel(tr("密码"));//密码

    userNameLabel->setAlignment(Qt::AlignVCenter);
    passwordLabel->setAlignment(Qt::AlignVCenter);

    userNameLabel->setFont(ft);
    userNameLabel->setPalette(palette);
    passwordLabel->setFont(ft);
    passwordLabel->setPalette(palette);

    userNameComboBox = new QComboBox();
    passwordEdit = new QLineEdit();
    userNameComboBox->setFont(ft);
    passwordEdit->setFont(ft);
    passwordEdit->setEchoMode(QLineEdit::Password);

    btnLogin=new QPushButton(tr("登录"));
    btnLogin->setFont(ft);
    connect(btnLogin,SIGNAL(clicked()),this,SLOT(clickedBtnLogin()));
    btnCancel=new QPushButton(tr("取消"));
    btnCancel->setFont(ft);
    connect(btnCancel,SIGNAL(clicked()),this,SLOT(clickedBtnCancel()));

    QVBoxLayout *lbLayout=new QVBoxLayout;
    lbLayout->addWidget(userNameLabel);
    lbLayout->addWidget(passwordLabel);
    QVBoxLayout *dataLayout=new QVBoxLayout;
    dataLayout->addWidget(userNameComboBox);
    dataLayout->addWidget(passwordEdit);
    QHBoxLayout *paraLayout=new QHBoxLayout;
    paraLayout->addLayout(lbLayout);
    paraLayout->addLayout(dataLayout);
    QHBoxLayout *btnLayout=new QHBoxLayout;
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnLogin);
    btnLayout->addStretch(1);
    btnLayout->addWidget(btnCancel);
    btnLayout->addStretch(1);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addLayout(paraLayout);
    layout->addLayout(btnLayout);

    setLayout(layout);
    setWindowTitle(tr(APPLICATION_NAME));
    setMinimumSize(400, 200);
    passwordEdit->setFocus(Qt::OtherFocusReason);

    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    //QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    QString strSQL=QString("SELECT * FROM Users ");
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(strSQL);
    if(model->rowCount()>0)
    {
        for(int i=0;i<model->rowCount();i++)
        {
            userNameComboBox->addItem(model->record(i).value("UserName").toString());
        }
    }
    db.closeDB();
    delete model;

    program=new QProcess(this);
    program->start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
    program->waitForStarted();
    program->waitForFinished();
/*    QProcess p(0);
    //p.start("%CommonProgramFiles%/Common~1/Micros~1/Ink/TabTip.exe");//
    p.start("C:/Progra~1/Common~1/Micros~1/ink/TabTip.exe");
    p.waitForStarted();
    p.waitForFinished();

    if (QFile::exists("robotrun"))
    {
        QFile file("robotrun");
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);    // read the data serialized from the file
        QString curUserName;

        in >> curUserName >> curUserRight;           // extract "the answer is" and 42
        userNameComboBox->setCurrentText(curUserName);
        passwordEdit->setFocus();
        file.close();
        g_userName=curUserName;
        g_userRight=curUserRight;
    }
    */
}

LoginDialog::~LoginDialog()
{
    if(program)
        delete program;
}

void LoginDialog::clickedBtnLogin()
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(MESSAGEBOX_FONT);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr(APPLICATION_NAME));
    msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);


    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败！"));
        msgBox.exec();
        return;
    }
    QString userName=userNameComboBox->currentText().trimmed();
    QString editPassword=passwordEdit->text();
    if(userName.isEmpty())
    {
        msgBox.setText(tr("用户名不能为空值."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    QString strSQL=QString("SELECT * FROM Users where UserName='%1'").arg(userName);
    QSqlQueryModel *model=new QSqlQueryModel;
    model->setQuery(strSQL);
    if(model->rowCount()==0)
    {
        msgBox.setText(tr("该用户不存在."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    QString password=model->record(0).value("UserPassword").toString();
    qint32 right=model->record(0).value("Right").toInt();
    delete model;

    if(password==passwordEdit->text())
    {
        g_userName=userName;
        g_userRight=right;

        QFile file("robotrun");
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);   // we will serialize the data into the file
        out<<userName<<(qint32)right;
        file.close();
        accept();
    }
    else
    {
        msgBox.setText(tr("密码错误."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    db.closeDB();
}

void LoginDialog::clickedBtnCancel()
{
    done(0);
}
