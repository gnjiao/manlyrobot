#include "mainwidget.h"
#include "ckeyhook.h"
#include "classNativeEventFilterZ.h"
#include "keyboardsignal.h"

#include <QApplication>
#include <QTextCodec>
#include <QDir>

NativeEventFilterZ *nativeEventFilter;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QDir::setCurrent(QString("C:\app"));

    KeyboardSignal *keyboard=new KeyboardSignal;
    nativeEventFilter=new NativeEventFilterZ;
    nativeEventFilter->setKeyboardSignal(keyboard);
    a.installNativeEventFilter(nativeEventFilter); // ZC: 这里，注意该函数中有"Native"

    MainWidget w;
    a.connect( &w, SIGNAL(closeApp()), &a, SLOT(quit()) );
    w.initWidget();
    w.show();

    CKeyHook *kh=new CKeyHook();
    HWND hWnd=(HWND)w.winId();
    kh->StartHook(hWnd);

    return a.exec();
}
