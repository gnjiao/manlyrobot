#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QDataStream>

class UserWidget : public QDialog
{
    Q_OBJECT
public:
    explicit UserWidget(QWidget *parent = 0);

signals:

public slots:
    void clickedBtnCreateUser();
    void clickedBtnChgPwd();
    void clickedBtnDelUser();
    void whenUserNameEnter();

private:
    QGroupBox *creatUserGroupBox;
    QGroupBox *chgPwdGroupBox;
    QGroupBox *delUserGroupBox;
    QLineEdit *userNameEdit1;
    QComboBox *userNameCombo2;
    QComboBox *userNameCombo3;
    QLineEdit *passwordEdit1;
    QLineEdit *passwordEdit2;
    QLineEdit *newPwdEdit;
    QLineEdit *cfPasswordEdit1;
    QLineEdit *cfPasswordEdit2;
    QComboBox *rightCombo1;
    QComboBox *rightCombo2;
    QPushButton *btnCreateUser;
    QPushButton *btnChgPassword;
    QPushButton *btnDelUser;

    QString curUserName;
    qint32 curUserRight;

    void createUserGroupBox();
    void createChgPwdGroupBox();
    void createDelUserGroupBox();
};

#endif // USERWIDGET_H
