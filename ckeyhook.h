#ifndef CKEYHOOK_H
#define CKEYHOOK_H

#include <windows.h>
#include <QObject>

class CKeyHook : public QObject
{
    Q_OBJECT
public:
    explicit CKeyHook(QObject *parent = 0);
    virtual ~CKeyHook();

    static LRESULT CALLBACK KeyProc(int nCode,WPARAM wparam,LPARAM lparam);
    bool StopHook();
    bool StartHook(HWND hWnd);

signals:

public slots:
};

#endif // CKEYHOOK_H
