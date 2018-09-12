#-------------------------------------------------
#
# Project created by QtCreator 2015-03-27T17:01:07
#
#-------------------------------------------------

QT       += qml quick gui-private widgets
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Robot
TEMPLATE = app
QT += sql
QT += xml
QT += core
QT += serialport
LIBS += -L$$PWD/./ -ldiskid32

SOURCES += main.cpp\
        mainwidget.cpp \
    msqldb.cpp \
    editparapage.cpp \
    operatepage.cpp \
    picturebutton.cpp \
    trackswidget.cpp \
    logindialog.cpp \
    userwidget.cpp \
    manlyrobot.cpp \
    myrwxml.cpp \
    weldproject.cpp \
    createprojdlg.cpp \
    pointwelddlg.cpp \
    linewelddlg.cpp \
    technicsdlg.cpp \
    logicdialog.cpp \
    matrixdlg.cpp \
    movekeydlg.cpp \
    setiodlg.cpp \
    testiodlg.cpp \
    setsystemdlg.cpp \
    setsystemadvdlg.cpp \
    clearsolderdlg.cpp \
    optimizedlg.cpp \
    myinputpanel.cpp \
    mylineedit.cpp \
    correctdlg.cpp \
    projmanagedlg.cpp \
    setspeeddlg.cpp \
    envparadlg.cpp \
    parawidget.cpp \
    serialport.cpp \
    keyboardsignal.cpp \
    ckeyhook.cpp

HEADERS  += mainwidget.h \
    msqldb.h \
    editparapage.h \
    operatepage.h \
    picturebutton.h \
    trackswidget.h \
    global.h \
    logindialog.h \
    userwidget.h \
    manlyrobot.h \
    myrwxml.h \
    weldproject.h \
    createprojdlg.h \
    pointwelddlg.h \
    linewelddlg.h \
    technicsdlg.h \
    logicdialog.h \
    matrixdlg.h \
    movekeydlg.h \
    setiodlg.h \
    testiodlg.h \
    setsystemdlg.h \
    setsystemadvdlg.h \
    clearsolderdlg.h \
    optimizedlg.h \
    myinputpanel.h \
    mylineedit.h \
    correctdlg.h \
    projmanagedlg.h \
    setspeeddlg.h \
    envparadlg.h \
    parawidget.h \
    serialport.h \
    classNativeEventFilterZ.h \
    keyboardsignal.h \
    ckeyhook.h

RESOURCES += \
    robot_res.qrc
