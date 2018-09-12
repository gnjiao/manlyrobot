#ifndef KEYBOARDSIGNAL_H
#define KEYBOARDSIGNAL_H

#include <QObject>

class KeyboardSignal : public QObject
{
    Q_OBJECT
public:
    explicit KeyboardSignal(QObject *parent = 0);

signals:
    void press_key_1();
    void release_key_1();
    void press_key_2();
    void release_key_2();
    void press_key_3();
    void release_key_3();
    void press_key_4();
    void release_key_4();
    void press_key_5();
    void release_key_5();
    void press_key_6();
    void release_key_6();
    void press_key_7();
    void release_key_7();
    void press_key_8();
    void release_key_8();
    void press_key_9();
    void release_key_9();
    void release_key_80();
    void release_key_NumLock();

public slots:
};

#endif // KEYBOARDSIGNAL_H
