#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QSqlRecord>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QComboBox>
#include <QProcess>

class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);
    virtual ~LoginDialog();

    QComboBox *userNameComboBox;
    QLineEdit *passwordEdit;
signals:

public slots:
    void clickedBtnLogin();
    void clickedBtnCancel();

protected:
    //virtual void focusInEvent(QFocusEvent *event);
private:
    QPushButton *btnLogin;
    QPushButton *btnCancel;
    qint32 curUserRight;
    QProcess *program;
};

#endif // LOGINDIALOG_H
