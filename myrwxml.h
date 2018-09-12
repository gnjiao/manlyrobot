#ifndef MYRWXML_H
#define MYRWXML_H

#include <QObject>
#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>
#include <QVariant>
//#include <QString>

#include "global.h"

class MyRWxml : public QObject
{
public:
    MyRWxml();

//    QDomDocument doc;
//    QDomElement element;
//    QDomElement childElement;
//    QDomText textElement;

    bool openXmlFile(QString xmlPath);
    bool readXmlFile(QString xmlPath,QDomDocument* doc);
    bool readXmlText(QString xmlPath,QString nodeName,QString* nodeText);
    bool readXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString* nodeText);
    bool readXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString childNodeName2,QString* nodeText);
    bool writeXmlText(QString xmlPath,QString nodeName,QString nodeText);
    bool writeXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString nodeText);
    bool writeXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString childNodeName2,QString nodeText);
    void createXmlFile(QString xmlPath);
    void createAXmlFile(QString xmlPath);
    void createIoXmlFile(QString xmlPath);
    void createSpeedXmlFile(QString xmlPath);
    bool readSysPara(QString xmlPath,SYSTEM_PARAMS &systemPara);
    void writeSysPara(QString xmlPath,SYSTEM_PARAMS systemPara);
    bool readSysAdvPara(QString xmlPath,SYSTEM_ADV_PARAMS &systemAdvPara);
    void writeSysAdvPara(QString xmlPath,SYSTEM_ADV_PARAMS systemAdvPara);
    bool readIoPara(QString xmlPath,IO_SET  &IOxb);
    void writeIoPara(QString xmlPath,IO_SET  IOxb);
    bool readSpeedPara(QString xmlPath,SPEED_PARAMS  &speedPara);
    void writeSpeedPara(QString xmlPath,SPEED_PARAMS  speedPara);
};

#endif // MYRWXML_H
