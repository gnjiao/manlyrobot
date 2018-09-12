#include "global.h"
#include "userwidget.h"
#include "msqldb.h"

UserWidget::UserWidget(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    createUserGroupBox();
    createChgPwdGroupBox();
    createDelUserGroupBox();
    QVBoxLayout *mainLayout=new QVBoxLayout;
    mainLayout->addWidget(creatUserGroupBox);
    mainLayout->addWidget(chgPwdGroupBox);
    mainLayout->addWidget(delUserGroupBox);
    setLayout(mainLayout);
    setWindowTitle(tr("用户管理"));
    if (QFile::exists("robotrun"))
    {
        QFile file("robotrun");
        file.open(QIODevice::ReadOnly);
        QDataStream in(&file);    // read the data serialized from the file

        in >> curUserName >> curUserRight;           // extract "the answer is" and 42
        file.close();
        if(curUserRight<USER_RIGHT_ADMIN)
        {
            creatUserGroupBox->setEnabled(false);
            delUserGroupBox->setEnabled(false);
        }
    }
    if(curUserRight<USER_RIGHT_ADMIN)
        rightCombo2->setEnabled(true);
    else
        rightCombo2->setEnabled(false);
}

void UserWidget::createUserGroupBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    QFont ft;
    palette.setColor(QPalette::WindowText, QColor(20,95,120));
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *userNameLabel = new QLabel(tr("用户名"));//用户名
    QLabel *passwordLabel = new QLabel(tr("密码"));//密码
    QLabel *cfPasswordLabel = new QLabel(tr("确认密码"));//确认密码
    QLabel *rightLabel = new QLabel(tr("权限"));//权限

    userNameLabel->setAlignment(Qt::AlignVCenter);
    passwordLabel->setAlignment(Qt::AlignVCenter);
    cfPasswordLabel->setAlignment(Qt::AlignVCenter);
    rightLabel->setAlignment(Qt::AlignVCenter);

    userNameLabel->setFont(ft);
    userNameLabel->setPalette(palette);
    passwordLabel->setFont(ft);
    passwordLabel->setPalette(palette);
    cfPasswordLabel->setFont(ft);
    cfPasswordLabel->setPalette(palette);
    rightLabel->setFont(ft);
    rightLabel->setPalette(palette);

    userNameEdit1 = new QLineEdit();
    passwordEdit1 = new QLineEdit();
    cfPasswordEdit1 = new QLineEdit();
    userNameEdit1->setFont(ft);
    passwordEdit1->setFont(ft);
    cfPasswordEdit1->setFont(ft);
    passwordEdit1->setEchoMode(QLineEdit::Password);
    cfPasswordEdit1->setEchoMode(QLineEdit::Password);
    rightCombo1=new QComboBox;
    rightCombo1->setFont(ft);
    rightCombo1->addItem(tr("操作员"));
    rightCombo1->addItem(tr("设计师"));
    rightCombo1->setCurrentIndex(-1);

    btnCreateUser=new QPushButton(tr("创建用户"));
    btnCreateUser->setFont(ft);
    connect(btnCreateUser,SIGNAL(clicked()),this,SLOT(clickedBtnCreateUser()));

    QVBoxLayout *lbLayout=new QVBoxLayout;
    lbLayout->addWidget(userNameLabel);
    lbLayout->addWidget(passwordLabel);
    lbLayout->addWidget(cfPasswordLabel);
    lbLayout->addWidget(rightLabel);
    QVBoxLayout *dataLayout=new QVBoxLayout;
    dataLayout->addWidget(userNameEdit1);
    dataLayout->addWidget(passwordEdit1);
    dataLayout->addWidget(cfPasswordEdit1);
    dataLayout->addWidget(rightCombo1);
    QHBoxLayout *paraLayout=new QHBoxLayout;
    paraLayout->addLayout(lbLayout);
    paraLayout->addLayout(dataLayout);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addLayout(paraLayout);
    layout->addWidget(btnCreateUser);

    palette.setColor(QPalette::WindowText, QColor(120,20,95));
    ft.setPointSize(18);
    creatUserGroupBox=new QGroupBox(tr("创建用户"));
    creatUserGroupBox->setPalette(palette);
    creatUserGroupBox->setFont(ft);
    creatUserGroupBox->setLayout(layout);
}

void UserWidget::createChgPwdGroupBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    palette.setColor(QPalette::WindowText, QColor(20,95,120));
    QFont ft;
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *userNameLabel = new QLabel(tr("用户名"));//用户名
    QLabel *passwordLabel = new QLabel(tr("密码"));//密码
    QLabel *newPwdLabel = new QLabel(tr("新密码"));//新密码
    QLabel *cfPasswordLabel = new QLabel(tr("确认新密码"));//确认新密码
    QLabel *rightLabel = new QLabel(tr("权限"));//权限

    userNameLabel->setAlignment(Qt::AlignVCenter);
    passwordLabel->setAlignment(Qt::AlignVCenter);
    cfPasswordLabel->setAlignment(Qt::AlignVCenter);
    rightLabel->setAlignment(Qt::AlignVCenter);

    userNameLabel->setFont(ft);
    userNameLabel->setPalette(palette);
    passwordLabel->setFont(ft);
    passwordLabel->setPalette(palette);
    newPwdLabel->setFont(ft);
    newPwdLabel->setPalette(palette);
    cfPasswordLabel->setFont(ft);
    cfPasswordLabel->setPalette(palette);
    rightLabel->setFont(ft);
    rightLabel->setPalette(palette);

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

    userNameCombo2 = new QComboBox();
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
            userNameCombo2->addItem(model->record(i).value("UserName").toString());
        }
    }
    db.closeDB();
    delete model;

    connect(userNameCombo2,SIGNAL(currentIndexChanged(int)),this,SLOT(whenUserNameEnter()));
    passwordEdit2 = new QLineEdit();
    newPwdEdit = new QLineEdit();
    cfPasswordEdit2 = new QLineEdit();
    userNameCombo2->setFont(ft);
    passwordEdit2->setFont(ft);
    newPwdEdit->setFont(ft);
    cfPasswordEdit2->setFont(ft);
    passwordEdit2->setEchoMode(QLineEdit::Password);
    newPwdEdit->setEchoMode(QLineEdit::Password);
    cfPasswordEdit2->setEchoMode(QLineEdit::Password);
    rightCombo2=new QComboBox;
    rightCombo2->setFont(ft);
    rightCombo2->addItem(tr("操作员"));
    rightCombo2->addItem(tr("设计师"));
    rightCombo2->setCurrentIndex(-1);

    btnChgPassword=new QPushButton(tr("修改用户"));
    btnChgPassword->setFont(ft);
    connect(btnChgPassword,SIGNAL(clicked()),this,SLOT(clickedBtnChgPwd()));

    QVBoxLayout *lbLayout=new QVBoxLayout;
    lbLayout->addWidget(userNameLabel);
    lbLayout->addWidget(passwordLabel);
    lbLayout->addWidget(newPwdLabel);
    lbLayout->addWidget(cfPasswordLabel);
    lbLayout->addWidget(rightLabel);
    QVBoxLayout *dataLayout=new QVBoxLayout;
    dataLayout->addWidget(userNameCombo2);
    dataLayout->addWidget(passwordEdit2);
    dataLayout->addWidget(newPwdEdit);
    dataLayout->addWidget(cfPasswordEdit2);
    dataLayout->addWidget(rightCombo2);
    QHBoxLayout *paraLayout=new QHBoxLayout;
    paraLayout->addLayout(lbLayout);
    paraLayout->addLayout(dataLayout);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addLayout(paraLayout);
    layout->addWidget(btnChgPassword);

    palette.setColor(QPalette::WindowText, QColor(120,20,95));
    ft.setPointSize(18);
    chgPwdGroupBox=new QGroupBox(tr("修改用户"));
    chgPwdGroupBox->setPalette(palette);
    chgPwdGroupBox->setFont(ft);
    chgPwdGroupBox->setLayout(layout);
}

void UserWidget::whenUserNameEnter()
{
    if(curUserRight<USER_RIGHT_ADMIN)
        rightCombo2->setEnabled(true);
    else
        rightCombo2->setEnabled(false);
    if(curUserName==userNameCombo2->currentText().trimmed())
    {
        passwordEdit2->setEnabled(true);
        newPwdEdit->setEnabled(true);
        cfPasswordEdit2->setEnabled(true);
        rightCombo2->setEnabled(false);
    }
    else
    {
        passwordEdit2->setEnabled(false);
        newPwdEdit->setEnabled(false);
        cfPasswordEdit2->setEnabled(false);
        if((userNameCombo2->currentText().trimmed() != QString("admin")) && (userNameCombo2->currentText().trimmed() != QString("Manly")))
        {
            rightCombo2->setEnabled(true);
            rightCombo2->setCurrentIndex(-1);
        }
    }
}

void UserWidget::createDelUserGroupBox()
{
    QPalette palette = QPalette();  //  得到窗口部件的调色板
    QFont ft;
    palette.setColor(QPalette::WindowText, QColor(20,95,120));
    ft.setPointSize(LABEL_BUTTON_FONT);
    ft.setItalic(false);

    QLabel *userNameLabel = new QLabel(tr("用户名"));//用户名

    userNameLabel->setAlignment(Qt::AlignVCenter);

    userNameLabel->setFont(ft);
    userNameLabel->setPalette(palette);

    userNameCombo3 = new QComboBox();
    userNameCombo3->setFont(ft);

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
            userNameCombo3->addItem(model->record(i).value("UserName").toString());
        }
    }
    db.closeDB();
    delete model;

    btnDelUser=new QPushButton(tr("删除用户"));
    btnDelUser->setFont(ft);
    connect(btnDelUser,SIGNAL(clicked()),this,SLOT(clickedBtnDelUser()));

    QVBoxLayout *lbLayout=new QVBoxLayout;
    lbLayout->addWidget(userNameLabel);
    QVBoxLayout *dataLayout=new QVBoxLayout;
    dataLayout->addWidget(userNameCombo3);
    QHBoxLayout *paraLayout=new QHBoxLayout;
    paraLayout->addLayout(lbLayout);
    paraLayout->addLayout(dataLayout);
    QVBoxLayout *layout=new QVBoxLayout;
    layout->addLayout(paraLayout);
    layout->addWidget(btnDelUser);

    palette.setColor(QPalette::WindowText, QColor(120,20,95));
    ft.setPointSize(18);
    delUserGroupBox=new QGroupBox(tr("删除用户"));
    delUserGroupBox->setPalette(palette);
    delUserGroupBox->setFont(ft);
    delUserGroupBox->setLayout(layout);
}

void UserWidget::clickedBtnCreateUser()
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
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    QString userName=userNameEdit1->text().trimmed();
    QString editPassword=passwordEdit1->text();
    QString cfEditPassword=cfPasswordEdit1->text();
    if(userName.isEmpty())
    {
        msgBox.setText(tr("用户名不允许空值."));
        msgBox.exec();
        db.closeDB();
        return;
    }
/*    if(editPassword.isEmpty() || cfEditPassword.isEmpty())
    {
        msgBox.setText(tr("Password can't be allow null values."));
        msgBox.exec();
        db.closeDB();
        return;
    }*/
    if(editPassword != cfEditPassword)
    {
        msgBox.setText(tr(" 您录入的密码与确认密码不一致."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    int right=rightCombo1->currentIndex()+1;
    if(right<1)
    {
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(tr("请选择用户权限."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    QString strSQL=QString("INSERT INTO Users (UserName, UserPassword, right) VALUES ('%1', '%2', %3)").arg(userName).arg(editPassword).arg(right);
    QSqlQuery query;
    if(!query.exec(strSQL))
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("该用户名已经存在，请输入其它名字."));
        msgBox.exec();
        db.closeDB();
    }
    else
    {
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setText(tr("创建用户成功."));
        msgBox.exec();
        db.closeDB();
    }
}

void UserWidget::clickedBtnChgPwd()
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
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    QString userName=userNameCombo2->currentText().trimmed();
    QString editPassword=passwordEdit2->text();
    QString editNewPassword=newPwdEdit->text();
    QString cfEditPassword=cfPasswordEdit2->text();
    if(userName.isEmpty())
    {
        msgBox.setText(tr("用户名不允许空值."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    int right=rightCombo2->currentIndex()+1;
    QString strSQL;
    if(editPassword.isEmpty() && editNewPassword.isEmpty() && cfEditPassword.isEmpty())
    {
        if(right<1)
        {
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(tr("请选择权限."));
            msgBox.exec();
            db.closeDB();
            return;
        }

        strSQL=QString("UPDATE Users SET right = %1  WHERE UserName = '%2'").arg(right).arg(userName);
        QSqlQuery query;
        if(!query.exec(strSQL))
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(tr("修改用户失败."));
            msgBox.exec();
            db.closeDB();
        }
        else
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("修改用户成功."));
            msgBox.exec();
            db.closeDB();
        }
    }
    else
    {
        strSQL=QString("SELECT * FROM Users where UserName='%1' and UserPassword='%2'").arg(userName).arg(editPassword);
        QSqlQueryModel *model=new QSqlQueryModel;
        model->setQuery(strSQL);
        if(model->rowCount()==0)
        {
            msgBox.setText(tr("用户名或密码不正确."));
            msgBox.exec();
            db.closeDB();
            delete model;
            return;
        }

        if(editNewPassword != cfEditPassword)
        {
            msgBox.setText(tr(" 您录入的密码与确认密码不一致."));
            msgBox.exec();
            db.closeDB();
            return;
        }
        else
        {
            strSQL=QString("UPDATE Users SET UserPassword = '%1',right=%2  WHERE UserName = '%3'").arg(editNewPassword).arg(right).arg(userName);
            QSqlQuery query;
            if(!query.exec(strSQL))
            {
                msgBox.setIcon(QMessageBox::Critical);
                msgBox.setText(tr("修改用户失败."));
                msgBox.exec();
                db.closeDB();
            }
            else
            {
                msgBox.setIcon(QMessageBox::Information);
                msgBox.setText(tr("修改用户成功."));
                msgBox.exec();
                db.closeDB();
            }
        }
    }
}

void UserWidget::clickedBtnDelUser()
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
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);

    SqlDB db;
    if(!db.openDB("robot.db"))
    {
        msgBox.setText(tr("打开数据库失败."));
        msgBox.exec();
        return;
    }
    QString userName=userNameCombo3->currentText().trimmed();
    if(userName.isEmpty())
    {
        msgBox.setText(tr("用户名不允许空值."));
        msgBox.exec();
        db.closeDB();
        return;
    }
    if((userName != QString("admin")) && (userName != QString("Manly")))
    {
        QString strSQL=QString("DELETE  FROM Users  WHERE UserName = '%1'").arg(userName);
        QSqlQuery query;
        if(!query.exec(strSQL))
        {
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.setText(tr("删除用户失败."));
            msgBox.exec();
        }
        else
        {
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setText(tr("删除用户成功."));
            msgBox.exec();
        }
    }
    else
    {
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText(tr("删除用户失败."));
        msgBox.exec();
    }
    db.closeDB();
}
