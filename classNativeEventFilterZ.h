#ifndef CLASSNATIVEEVENTFILTERZ_H
#define CLASSNATIVEEVENTFILTERZ_H

#include <QDebug>
#include <qabstractnativeeventfilter.h>
#include <QAbstractNativeEventFilter>

#include "keyboardsignal.h"
#include <windows.h>
//#pragma comment(lib, "user32.lib")

#define WM_GLOBALKEYBOARD WM_USER+100

class NativeEventFilterZ : public QAbstractNativeEventFilter
{
    //Q_OBJECT // ZC: 这个不需要...
public:
    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE
    {
        bool bFlag = false;
        //keyboard=new KeyboardSignal;
        MSG* pMsg = reinterpret_cast<MSG*>(message);
        switch(pMsg->message)
        {
        case WM_GLOBALKEYBOARD+1:
            key_1();
            //qDebug() << "emit WM_GLOBALKEYBOARD+1 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+11:
            key_11();
            //qDebug() << "emit WM_GLOBALKEYBOARD+11 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+2:
            key_2();
            //qDebug() << "emit WM_GLOBALKEYBOARD+2 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+12:
            key_12();
            //qDebug() << "emit WM_GLOBALKEYBOARD+12 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+3:
            key_3();
            //qDebug() << "emit WM_GLOBALKEYBOARD+1 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+13:
            key_13();
            //qDebug() << "emit WM_GLOBALKEYBOARD+11 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+4:
            key_4();
            //qDebug() << "emit WM_GLOBALKEYBOARD+2 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+14:
            key_14();
            //qDebug() << "emit WM_GLOBALKEYBOARD+12 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+5:
            key_5();
            //qDebug() << "emit WM_GLOBALKEYBOARD+1 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+15:
            key_15();
            //qDebug() << "emit WM_GLOBALKEYBOARD+11 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+6:
            key_6();
            //qDebug() << "emit WM_GLOBALKEYBOARD+2 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+16:
            key_16();
            //qDebug() << "emit WM_GLOBALKEYBOARD+12 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+7:
            key_7();
            //qDebug() << "emit WM_GLOBALKEYBOARD+1 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+17:
            key_17();
            //qDebug() << "emit WM_GLOBALKEYBOARD+11 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+8:
            key_8();
            //qDebug() << "emit WM_GLOBALKEYBOARD+2 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+18:
            key_18();
            //qDebug() << "emit WM_GLOBALKEYBOARD+12 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+9:
            key_9();
            //qDebug() << "emit WM_GLOBALKEYBOARD+2 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+19:
            key_19();
            //qDebug() << "emit WM_GLOBALKEYBOARD+12 msg.";
            bFlag = true;
            break;
        case WM_GLOBALKEYBOARD+90:
            key_90();
            bFlag = true;
            //qDebug() << "emit WM_GLOBALKEYBOARD+90 msg.";
            break;
        case WM_GLOBALKEYBOARD+80:
            key_80();
            bFlag = true;
            //qDebug() << "emit WM_GLOBALKEYBOARD+90 msg.";
            break;
        }
        return bFlag;
    }

public:
    void setKeyboardSignal(KeyboardSignal *skeyboard)
    {
        keyboard=skeyboard;
    }

    KeyboardSignal *getKeyboardSignal()
    {
        return keyboard;
    }

private:
    KeyboardSignal *keyboard;

    void key_1()
    {
        emit keyboard->press_key_1();
    }
    void key_11()
    {
        emit keyboard->release_key_1();
    }
    void key_2()
    {
        emit keyboard->press_key_2();
    }
    void key_12()
    {
        emit keyboard->release_key_2();
    }
    void key_3()
    {
        emit keyboard->press_key_3();
    }
    void key_13()
    {
        emit keyboard->release_key_3();
    }
    void key_4()
    {
        emit keyboard->press_key_4();
    }
    void key_14()
    {
        emit keyboard->release_key_4();
    }
    void key_5()
    {
        emit keyboard->press_key_5();
    }
    void key_15()
    {
        emit keyboard->release_key_5();
    }
    void key_6()
    {
        emit keyboard->press_key_6();
    }
    void key_16()
    {
        emit keyboard->release_key_6();
    }
    void key_7()
    {
        emit keyboard->press_key_7();
    }
    void key_17()
    {
        emit keyboard->release_key_7();
    }
    void key_8()
    {
        emit keyboard->press_key_8();
    }
    void key_18()
    {
        emit keyboard->release_key_8();
    }
    void key_9()
    {
        emit keyboard->press_key_9();
    }
    void key_19()
    {
        emit keyboard->release_key_9();
    }
    void key_90()
    {
        emit keyboard->release_key_NumLock();
    }
    void key_80()
    {
        emit keyboard->release_key_80();
    }
};

#endif // CLASSNATIVEEVENTFILTERZ_H
