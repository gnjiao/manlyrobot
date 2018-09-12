#include "myrwxml.h"

#include <QFile>
#include <QTextStream>

MyRWxml::MyRWxml()
{
}

bool MyRWxml::openXmlFile(QString xmlPath)
{
    bool ret=false;
    QFile file(xmlPath);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        ret=true;
        file.close();
    }
    return  ret;
}

bool MyRWxml::readXmlFile(QString xmlPath,QDomDocument* doc)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!(*doc).setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();


    return true;
}

bool MyRWxml::readXmlText(QString xmlPath,QString nodeName,QString* nodeText)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;
    for(QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        if(node.toElement().tagName() == nodeName)
        {
            QDomText t = node.firstChild().toText();
            if (!t.isNull())
            {
                *nodeText = t.data();
                ret=true;
            }
            break;
        }
    }

    return ret;
}

bool MyRWxml::readXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString* nodeText)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    for(QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        QString ss=node.toElement().tagName();
        if(node.toElement().tagName() == nodeName)
        {
            for(QDomNode childNode = node.firstChild(); !childNode.isNull(); childNode = childNode.nextSibling())
            {
                if(childNode.toElement().tagName() == childNodeName)
                {
                    QDomText t = childNode.firstChild().toText();//toText();
                    if (!t.isNull())
                    {
                        *nodeText = t.data();
                        ret=true;
                    }
                    break;
                }
            }
            break;
        }
    }

    return ret;
}

bool MyRWxml::readXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString childNodeName2,QString* nodeText)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    for(QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        QString ss=node.toElement().tagName();
        if(node.toElement().tagName() == nodeName)
        {
            for(QDomNode childNode = node.firstChild(); !childNode.isNull(); childNode = childNode.nextSibling())
            {
                if(childNode.toElement().tagName() == childNodeName)
                {
                    for(QDomNode childNode2 = childNode.firstChild(); !childNode2.isNull(); childNode2 = childNode2.nextSibling())
                    {
                        if(childNode2.toElement().tagName() == childNodeName2)
                        {
                            QDomText t = childNode2.firstChild().toText();//toText();
                            if (!t.isNull())
                            {
                                *nodeText = t.data();
                                ret=true;
                            }
                            break;
                        }
                    }
                }
            }
            break;
        }
    }

    return ret;
}

bool MyRWxml::writeXmlText(QString xmlPath,QString nodeName,QString nodeText)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))// | QIODevice::Truncate
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement old_root = doc.documentElement();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    for(QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        if(node.toElement().tagName() == nodeName)
        {
            QDomText t = node.firstChild().toText();
            node.removeChild(t);
            QDomText text_Node = doc.createTextNode(nodeText);
            node.appendChild(text_Node);
            ret=true;
            break;
        }
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return  false;
    doc.appendChild(root);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out, 4);
    file.close();

    return ret;
}

bool MyRWxml::writeXmlText(QString xmlPath,QString nodeName,QString childNodeName,QString nodeText)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))// | QIODevice::Truncate
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement old_root = doc.documentElement();
    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    for(QDomNode node = root.firstChild(); !node.isNull(); node = node.nextSibling())
    {
        if(node.toElement().tagName() == nodeName)
        {
            for(QDomNode childNode = node.firstChild(); !childNode.isNull(); childNode = childNode.nextSibling())
            {
                if(childNode.toElement().tagName() == childNodeName)
                {
                    QDomText t = childNode.firstChild().toText();
                    childNode.removeChild(t);
                    QDomText text_Node = doc.createTextNode(nodeText);
                    childNode.appendChild(text_Node);
                    //QString ss=text_Node.data();
                    ret=true;
                    break;
                }
            }
            break;
        }
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return  false;
    doc.appendChild(root);
    QTextStream out(&file);
    out.setCodec("UTF-8");
    doc.save(out, 4);
    file.close();

    return ret;
}

void MyRWxml::createXmlFile(QString xmlPath)
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("Axis_manual_speed");
     QDomElement childElement = doc.createElement("high_speed");
     QDomElement childElement2 = doc.createElement("X");
     QDomText text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("low_speed");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);

     //////////////////////////////////////////////////////2015-06-05添加//////////////////////////////////
     typeElement = doc.createElement("Axis_auto_speed");
     childElement = doc.createElement("high_speed");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode("7000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("low_speed");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);
     /////////////////////////////////////////////////////////////////////////////////////////////////

     typeElement = doc.createElement("Axis_limit_length");
     childElement = doc.createElement("X");
     text_Node = doc.createTextNode("300");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("Y");
     text_Node = doc.createTextNode("300");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("Z");
     text_Node = doc.createTextNode("90");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R_p");
     text_Node = doc.createTextNode("178");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R_n");
     text_Node = doc.createTextNode("-178");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R1_p");
     text_Node = doc.createTextNode("60");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R1_n");
     text_Node = doc.createTextNode("-60");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R2_p");
     text_Node = doc.createTextNode("60");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R2_n");
     text_Node = doc.createTextNode("-60");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);

     root.appendChild(typeElement);


     typeElement = doc.createElement("Manual_solder_speed");
     childElement = doc.createElement("out_solder");
     text_Node = doc.createTextNode("10");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("back_solder");
     text_Node = doc.createTextNode("10");
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);

     root.appendChild(typeElement);

     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}

void MyRWxml::createAXmlFile(QString xmlPath)
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("Max_pulse");
     QDomText text_Node = doc.createTextNode("200000");
     typeElement.appendChild(text_Node);
     root.appendChild(typeElement);

     typeElement = doc.createElement("hurry-up_stop");
     text_Node = doc.createTextNode(tr("1"));
     typeElement.appendChild(text_Node);
     root.appendChild(typeElement);

     typeElement = doc.createElement("Machine_type");
     text_Node = doc.createTextNode(tr("0"));
     typeElement.appendChild(text_Node);
     root.appendChild(typeElement);

     typeElement = doc.createElement("Control_para");
     QDomElement childElement = doc.createElement("Out_pulse_form");
     QDomElement childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode("0");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     typeElement.appendChild(childElement);
//
     childElement = doc.createElement("VLPPS");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode("100");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     typeElement.appendChild(childElement);
//

    childElement = doc.createElement("VH");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("1000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tacc");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tdec");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("300");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tsacc");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tsdec");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);

    root.appendChild(typeElement);


    typeElement = doc.createElement("Electrical_machinery_para");
    childElement = doc.createElement("PULSE_Per");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("0");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Max_speed");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("30000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Step_length");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("0.01");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("0.01");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("0.01");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("0.015");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("0.01");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("0.015");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("0.015");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("0.01");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);

    root.appendChild(typeElement);

    typeElement = doc.createElement("Return_origin_para");
    childElement = doc.createElement("VL_Home");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("VH_hm");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("2000");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("TAcc");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode("100");
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);


    root.appendChild(typeElement);


     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}
void MyRWxml::createIoXmlFile(QString xmlPath)
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("system_IO_set");
     QDomElement childElement = doc.createElement("system_input");
     QDomElement childElement2 = doc.createElement("key_Xn");
     QDomText text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_Yn");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_Xp");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_Yp");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_SEL_1");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_SEL_2");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_FT");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_BT");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_BL");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_FS");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_START");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_STOP");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_FIX_POS");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_NEXT");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_UP_POS");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_RESET");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_CLEAR");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE1");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE2");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE3");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE4");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE5");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE6");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE7");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("system_output");
     childElement2 = doc.createElement("CLEAR_O");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Cylinder_O");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("LED_START_O");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE1");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE2");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE3");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE4");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE5");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);

     //////////////////////////////////////////////////////客户IO//////////////////////////////////
     typeElement = doc.createElement("customer_IO_set");
     childElement = doc.createElement("customer_input");
     childElement2 = doc.createElement("IN0");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN1");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN2");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN3");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN4");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN5");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN6");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN7");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN8");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN9");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN10");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN11");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN12");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN13");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN14");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN15");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN16");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN17");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN18");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN19");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN20");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN21");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN22");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN23");
     text_Node = doc.createTextNode("32");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("customer_output");
     childElement2 = doc.createElement("OUT0");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT1");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT2");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT3");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT4");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT5");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT6");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT7");
     text_Node = doc.createTextNode("16");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);


     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}

void MyRWxml::createSpeedXmlFile(QString xmlPath)
{
    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("system_speed_set");
     QDomElement childElement = doc.createElement("X_axis");
     QDomElement childElement2 = doc.createElement("distance_0");
     QDomText text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_2");
     text_Node = doc.createTextNode("2000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_3");
     text_Node = doc.createTextNode("2300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_4");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_5");
     text_Node = doc.createTextNode("5000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_6");
     text_Node = doc.createTextNode("5500");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_7");
     text_Node = doc.createTextNode("6000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_8");
     text_Node = doc.createTextNode("6200");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_9");
     text_Node = doc.createTextNode("12000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_10");
     text_Node = doc.createTextNode("20000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_11");
     text_Node = doc.createTextNode("25000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_12");
     text_Node = doc.createTextNode("30000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_13");
     text_Node = doc.createTextNode("35000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_14");
     text_Node = doc.createTextNode("36000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_15");
     text_Node = doc.createTextNode("37000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_16");
     text_Node = doc.createTextNode("38000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_17");
     text_Node = doc.createTextNode("39000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_18");
     text_Node = doc.createTextNode("40000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_19");
     text_Node = doc.createTextNode("45000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_20");
     text_Node = doc.createTextNode("50000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("Y_axis");
     childElement2 = doc.createElement("distance_0");
     text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_2");
     text_Node = doc.createTextNode("2000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_3");
     text_Node = doc.createTextNode("2300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_4");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_5");
     text_Node = doc.createTextNode("5000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_6");
     text_Node = doc.createTextNode("5500");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_7");
     text_Node = doc.createTextNode("6000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_8");
     text_Node = doc.createTextNode("6200");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_9");
     text_Node = doc.createTextNode("12000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_10");
     text_Node = doc.createTextNode("20000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_11");
     text_Node = doc.createTextNode("25000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_12");
     text_Node = doc.createTextNode("30000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_13");
     text_Node = doc.createTextNode("35000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_14");
     text_Node = doc.createTextNode("36000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_15");
     text_Node = doc.createTextNode("37000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_16");
     text_Node = doc.createTextNode("38000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_17");
     text_Node = doc.createTextNode("39000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_18");
     text_Node = doc.createTextNode("40000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_19");
     text_Node = doc.createTextNode("45000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_20");
     text_Node = doc.createTextNode("50000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("Z_axis");
     childElement2 = doc.createElement("distance_0");
     text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_2");
     text_Node = doc.createTextNode("2000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_3");
     text_Node = doc.createTextNode("2300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_4");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_5");
     text_Node = doc.createTextNode("5000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_6");
     text_Node = doc.createTextNode("5500");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_7");
     text_Node = doc.createTextNode("6000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_8");
     text_Node = doc.createTextNode("6200");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_9");
     text_Node = doc.createTextNode("12000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_10");
     text_Node = doc.createTextNode("20000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_11");
     text_Node = doc.createTextNode("25000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_12");
     text_Node = doc.createTextNode("30000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_13");
     text_Node = doc.createTextNode("35000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_14");
     text_Node = doc.createTextNode("36000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_15");
     text_Node = doc.createTextNode("37000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_16");
     text_Node = doc.createTextNode("38000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_17");
     text_Node = doc.createTextNode("39000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_18");
     text_Node = doc.createTextNode("40000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_19");
     text_Node = doc.createTextNode("45000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_20");
     text_Node = doc.createTextNode("50000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("R_axis");
     childElement2 = doc.createElement("distance_0");
     text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_2");
     text_Node = doc.createTextNode("2000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_3");
     text_Node = doc.createTextNode("2300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_4");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_5");
     text_Node = doc.createTextNode("5000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_6");
     text_Node = doc.createTextNode("5500");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_7");
     text_Node = doc.createTextNode("6000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_8");
     text_Node = doc.createTextNode("6200");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_9");
     text_Node = doc.createTextNode("12000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_10");
     text_Node = doc.createTextNode("20000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_11");
     text_Node = doc.createTextNode("25000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_12");
     text_Node = doc.createTextNode("30000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_13");
     text_Node = doc.createTextNode("35000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_14");
     text_Node = doc.createTextNode("36000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_15");
     text_Node = doc.createTextNode("37000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_16");
     text_Node = doc.createTextNode("38000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_17");
     text_Node = doc.createTextNode("39000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_18");
     text_Node = doc.createTextNode("40000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_19");
     text_Node = doc.createTextNode("45000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_20");
     text_Node = doc.createTextNode("50000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("S_axis");

     typeElement.appendChild(childElement);

     childElement = doc.createElement("R1_axis");
     childElement2 = doc.createElement("distance_0");
     text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("R2_axis");
     childElement2 = doc.createElement("distance_0");
     text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("E_axis");
     childElement2 = doc.createElement("distance_0");
     text_Node = doc.createTextNode("300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_1");
     text_Node = doc.createTextNode("1000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_2");
     text_Node = doc.createTextNode("2000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_3");
     text_Node = doc.createTextNode("2300");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_4");
     text_Node = doc.createTextNode("3000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_5");
     text_Node = doc.createTextNode("5000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_6");
     text_Node = doc.createTextNode("5500");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_7");
     text_Node = doc.createTextNode("6000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_8");
     text_Node = doc.createTextNode("6200");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_9");
     text_Node = doc.createTextNode("12000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_10");
     text_Node = doc.createTextNode("20000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_11");
     text_Node = doc.createTextNode("25000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_12");
     text_Node = doc.createTextNode("30000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_13");
     text_Node = doc.createTextNode("35000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_14");
     text_Node = doc.createTextNode("36000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_15");
     text_Node = doc.createTextNode("37000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_16");
     text_Node = doc.createTextNode("38000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_17");
     text_Node = doc.createTextNode("39000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_18");
     text_Node = doc.createTextNode("40000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_19");
     text_Node = doc.createTextNode("45000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("distance_20");
     text_Node = doc.createTextNode("50000");
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);








     root.appendChild(typeElement);


     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}

bool MyRWxml::readSysPara(QString xmlPath,SYSTEM_PARAMS &systemPara)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    QDomElement elt = root.firstChildElement("Axis_manual_speed");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("high_speed").isNull())
        {
            QDomElement node=elt.firstChildElement("high_speed").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemPara.highSpeed[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
            /*int tmp=systemPara.highSpeed[4];
            systemPara.highSpeed[4]=systemPara.highSpeed[5];
            systemPara.highSpeed[5]=systemPara.highSpeed[6];
            systemPara.highSpeed[6]=tmp;*/
        }
        if(!elt.firstChildElement("low_speed").isNull())
        {
            QDomElement node=elt.firstChildElement("low_speed").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemPara.lowSpeed[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    //////////////////////////////////////////////////////2015-06-05添加//////////////////////////////////
    elt = root.firstChildElement("Axis_auto_speed");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("high_speed").isNull())
        {
            QDomElement node=elt.firstChildElement("high_speed").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemPara.aHighSpeed[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("low_speed").isNull())
        {
            QDomElement node=elt.firstChildElement("low_speed").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemPara.aLowSpeed[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////

    elt = root.firstChildElement("Axis_limit_length");
    if(!elt.isNull())
    {
        QDomElement node=elt.firstChildElement();
        for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
        {
            QDomText t = node.firstChild().toText();
            if (!t.isNull())
            {
                systemPara.limitPosLen[i] = t.data().toInt();
                ret=true;
            }
            else
            {
                ret=false;
                break;
            }
        }
    }
    elt = root.firstChildElement("Manual_solder_speed");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("out_solder").isNull())
        {
            QDomText t = elt.firstChildElement("out_solder").firstChild().toText();
            if (!t.isNull())
            {
                systemPara.outSolderSpeed = t.data().toDouble();
                ret=true;
            }
            else
            {
                ret=false;
            }
        }
        if(!elt.firstChildElement("back_solder").isNull())
        {
            QDomText t = elt.firstChildElement("back_solder").firstChild().toText();
            if (!t.isNull())
            {
                systemPara.backSolderSpeed = t.data().toDouble();
                ret=true;
            }
            else
            {
                ret=false;
            }
        }
    }
    return ret;
}

bool MyRWxml::readSysAdvPara(QString xmlPath,SYSTEM_ADV_PARAMS &systemAdvPara)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    QDomElement elt = root.firstChildElement("Max_pulse");
    if(!elt.isNull())
    {
        QDomText t = elt.firstChild().toText();
        if (!t.isNull())
        {
            systemAdvPara.maxPusle = t.data().toInt();
            ret=true;
        }
        else
        {
            ret=false;
        }
    }
    elt = root.firstChildElement("hurry-up_stop");
    if(!elt.isNull())
    {
        QDomText t = elt.firstChild().toText();
        if (!t.isNull())
        {
            systemAdvPara.stopStart = t.data().toInt();
            ret=true;
        }
        else
        {
            ret=false;
        }
    }
    elt = root.firstChildElement("Machine_type");
    if(!elt.isNull())
    {
        QDomText t = elt.firstChild().toText();
        if (!t.isNull())
        {
            systemAdvPara.macType = t.data().toInt();
            ret=true;
        }
        else
        {
            ret=false;
        }
    }

    elt = root.firstChildElement("Control_para");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("Out_pulse_form").isNull())
        {
            QDomElement node=elt.firstChildElement("Out_pulse_form").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.Out_pulse_form[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("VLPPS").isNull())
        {
            QDomElement node=elt.firstChildElement("VLPPS").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.VLPPS[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("VH").isNull())
        {
            QDomElement node=elt.firstChildElement("VH").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.VH[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Tacc").isNull())
        {
            QDomElement node=elt.firstChildElement("Tacc").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.Tacc[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Tdec").isNull())
        {
            QDomElement node=elt.firstChildElement("Tdec").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.Tdec[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Tsacc").isNull())
        {
            QDomElement node=elt.firstChildElement("Tsacc").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.Tsacc[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Tsdec").isNull())
        {
            QDomElement node=elt.firstChildElement("Tsdec").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.Tsdec[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    elt = root.firstChildElement("Electrical_machinery_para");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("PULSE_Per").isNull())
        {
            QDomElement node=elt.firstChildElement("PULSE_Per").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.PULSE_Per[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Max_speed").isNull())
        {
            QDomElement node=elt.firstChildElement("Max_speed").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.maxSpeed[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Step_length").isNull())
        {
            QDomElement node=elt.firstChildElement("Step_length").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.macStepLength[i] = t.data().toDouble();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }

    elt = root.firstChildElement("Return_origin_para");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("VL_Home").isNull())
        {
            QDomElement node=elt.firstChildElement("VL_Home").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.VL_Home[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("VH_hm").isNull())
        {
            QDomElement node=elt.firstChildElement("VH_hm").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.VH_hm[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("TAcc").isNull())
        {
            QDomElement node=elt.firstChildElement("TAcc").firstChildElement();
            for (int i=0; !node.isNull(); node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    systemAdvPara.TAcc[i] = t.data().toDouble();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    return ret;
}

void MyRWxml::writeSysPara(QString xmlPath,SYSTEM_PARAMS systemPara)
{
    if(QFile::exists(xmlPath))
        QFile::remove(xmlPath);

    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("Axis_manual_speed");
     QDomElement childElement = doc.createElement("high_speed");
     QDomElement childElement2 = doc.createElement("X");
     QDomText text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.highSpeed[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("low_speed");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.lowSpeed[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);

     //////////////////////////////////////////2015-06-07添加////////////////////////////////////////
     typeElement = doc.createElement("Axis_auto_speed");
     childElement = doc.createElement("high_speed");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aHighSpeed[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("low_speed");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.aLowSpeed[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);
     ///////////////////////////////////////////////////////////////////////////////////////////////

     typeElement = doc.createElement("Axis_limit_length");
     childElement = doc.createElement("X");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[0],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[1],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[2],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R_p");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[3],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R_n");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[4],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R1_p");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[5],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R1_n");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[6],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R2_p");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[7],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("R2_n");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.limitPosLen[8],0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);

     root.appendChild(typeElement);


     typeElement = doc.createElement("Manual_solder_speed");
     childElement = doc.createElement("out_solder");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.outSolderSpeed,0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);
     childElement = doc.createElement("back_solder");
     text_Node = doc.createTextNode(QString("%1").arg(systemPara.backSolderSpeed,0,'f',0));
     childElement.appendChild(text_Node);
     typeElement.appendChild(childElement);

     root.appendChild(typeElement);

     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}

void MyRWxml::writeSysAdvPara(QString xmlPath,SYSTEM_ADV_PARAMS systemAdvPara)
{
    if(QFile::exists(xmlPath))
        QFile::remove(xmlPath);

    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("Max_pulse");
     QDomText text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxPusle));
     typeElement.appendChild(text_Node);
     root.appendChild(typeElement);

     typeElement = doc.createElement("hurry-up_stop");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.stopStart));
     typeElement.appendChild(text_Node);
     root.appendChild(typeElement);

     typeElement = doc.createElement("Machine_type");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macType));
     typeElement.appendChild(text_Node);
     root.appendChild(typeElement);

     typeElement = doc.createElement("Control_para");
     QDomElement childElement = doc.createElement("Out_pulse_form");
     QDomElement childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Out_pulse_form[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     typeElement.appendChild(childElement);
//
     childElement = doc.createElement("VLPPS");
     childElement2 = doc.createElement("X");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Y");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("Z");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("S");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R1");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("R2");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     childElement2 = doc.createElement("E");
     text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VLPPS[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     typeElement.appendChild(childElement);
//

    childElement = doc.createElement("VH");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tacc");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tacc[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tdec");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tdec[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tsacc");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsacc[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Tsdec");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.Tsdec[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);

    root.appendChild(typeElement);


    typeElement = doc.createElement("Electrical_machinery_para");
    childElement = doc.createElement("PULSE_Per");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.PULSE_Per[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Max_speed");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.maxSpeed[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("Step_length");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[0],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[1],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[2],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[3],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[4],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[5],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[6],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.macStepLength[7],0,'f',5));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);

    root.appendChild(typeElement);

    typeElement = doc.createElement("Return_origin_para");
    childElement = doc.createElement("VL_Home");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VL_Home[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("VH_hm");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.VH_hm[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);
//

    childElement = doc.createElement("TAcc");
    childElement2 = doc.createElement("X");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[0]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Y");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[1]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("Z");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[2]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[3]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("S");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[4]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R1");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[5]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("R2");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[6]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);

    childElement2 = doc.createElement("E");
    text_Node = doc.createTextNode(QString("%1").arg(systemAdvPara.TAcc[7]));
    childElement2.appendChild(text_Node);
    childElement.appendChild(childElement2);
    typeElement.appendChild(childElement);


    root.appendChild(typeElement);


     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}

bool MyRWxml::readIoPara(QString xmlPath,IO_SET  &IOxb)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    QDomElement elt = root.firstChildElement("system_IO_set");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("system_input").isNull())
        {
            QDomElement node=elt.firstChildElement("system_input").firstChildElement();
            for (int i=0; !node.isNull() && i<24; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    IOxb.sysIxb[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("system_output").isNull())
        {
            QDomElement node=elt.firstChildElement("system_output").firstChildElement();
            for (int i=0; !node.isNull() && i<8; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    IOxb.sysOxb[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    //////////////////////////////////////////////////////客户IO读取//////////////////////////////////
    elt = root.firstChildElement("customer_IO_set");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("customer_input").isNull())
        {
            QDomElement node=elt.firstChildElement("customer_input").firstChildElement();
            for (int i=0; !node.isNull() && i<24; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    IOxb.cusIxb[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("customer_output").isNull())
        {
            QDomElement node=elt.firstChildElement("customer_output").firstChildElement();
            for (int i=0; !node.isNull() && i<8; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    IOxb.cusOxb[i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    return ret;
}

void MyRWxml::writeIoPara(QString xmlPath,IO_SET  IOxb)
{
    if(QFile::exists(xmlPath))
        QFile::remove(xmlPath);

    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("system_IO_set");
     QDomElement childElement = doc.createElement("system_input");
     QDomElement childElement2 = doc.createElement("key_Xn");
     QDomText text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_Yn");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_Xp");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_Yp");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_SEL_1");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_SEL_2");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_FT");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_BT");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_BL");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[8]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_FS");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[9]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_START");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[10]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_STOP");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[11]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_FIX_POS");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[12]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_NEXT");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[13]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_UP_POS");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[14]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_RESET");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[15]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("key_CLEAR");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[16]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE1");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[17]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE2");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[18]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE3");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[19]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE4");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[20]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE5");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[21]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE6");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[22]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IRE7");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysIxb[23]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("system_output");
     childElement2 = doc.createElement("CLEAR_O");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("Cylinder_O");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("LED_START_O");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE1");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE2");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE3");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE4");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("ORE5");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.sysOxb[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);

     //////////////////////////////////////////////////////客户IO//////////////////////////////////
     typeElement = doc.createElement("customer_IO_set");
     childElement = doc.createElement("customer_input");
     childElement2 = doc.createElement("IN0");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN1");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN2");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN3");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN4");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN5");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN6");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN7");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN8");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[8]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN9");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[9]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN10");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[10]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN11");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[11]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN12");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[12]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN13");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[13]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN14");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[14]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN15");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[15]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN16");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[16]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN17");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[17]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN18");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[18]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN19");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[19]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN20");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[20]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN21");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[21]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN22");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[22]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("IN23");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusIxb[23]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);

     childElement = doc.createElement("customer_output");
     childElement2 = doc.createElement("OUT0");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[0]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT1");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[1]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT2");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[2]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT3");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[3]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT4");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[4]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT5");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[5]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT6");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[6]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);
     childElement2 = doc.createElement("OUT7");
     text_Node = doc.createTextNode(QString("%1").arg(IOxb.cusOxb[7]));
     childElement2.appendChild(text_Node);
     childElement.appendChild(childElement2);

     typeElement.appendChild(childElement);
     root.appendChild(typeElement);


     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}

bool MyRWxml::readSpeedPara(QString xmlPath,SPEED_PARAMS  &speedPara)
{
    //打开文件
    bool ret=false;
    QFile file(xmlPath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return  ret;

    //
    QString errorStr;
    int errorLine;
    int errorColumn;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        return  ret;
    file.close();

    QDomElement root = doc.documentElement();
    if(root.tagName() != "Robot")
       return ret;

    QDomElement elt = root.firstChildElement("system_speed_set");
    if(!elt.isNull())
    {
        if(!elt.firstChildElement("X_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("X_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[0][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Y_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("Y_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[1][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("Z_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("Z_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[2][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("R_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("R_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[3][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
/*        if(!elt.firstChildElement("S_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("S_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[4][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }*/
        if(!elt.firstChildElement("R1_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("R1_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[5][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("R2_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("R2_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[6][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
        if(!elt.firstChildElement("E_axis").isNull())
        {
            QDomElement node=elt.firstChildElement("E_axis").firstChildElement();
            for (int i=0; !node.isNull() && i<21; node = node.nextSiblingElement(),i++)
            {
                QDomText t = node.firstChild().toText();
                if (!t.isNull())
                {
                    speedPara.maxSpeed[7][i] = t.data().toInt();
                    ret=true;
                }
                else
                {
                    ret=false;
                    break;
                }
            }
        }
    }

    return ret;
}

void MyRWxml::writeSpeedPara(QString xmlPath,SPEED_PARAMS  speedPara)
{
    if(QFile::exists(xmlPath))
        QFile::remove(xmlPath);

    QFile file(xmlPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

     QDomDocument doc;

     QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
     doc.appendChild(instruction);

     QDomElement root =  doc.createElement("Robot");
     doc.appendChild(root);
     QDomElement typeElement = doc.createElement("system_speed_set");
     QDomElement childElement = doc.createElement("X_axis");
     QDomElement childElement2 = doc.createElement("distance_0");
     QDomText text_Node = doc.createTextNode(QString("%1").arg(speedPara.maxSpeed[0][0]));
     QStringList axisStrList;
     axisStrList<<tr("X_axis")<<tr("Y_axis")<<tr("Z_axis")<<tr("R_axis")<<tr("S_axis")<<tr("R1_axis")<<tr("R2_axis")<<tr("E_axis");
     for(int i=0;i<8;i++)
     {
         childElement = doc.createElement(axisStrList[i]);
         for(int j=0;j<21;j++)
         {
             childElement2 = doc.createElement(QString("distance_%1").arg(j));
             text_Node = doc.createTextNode(QString("%1").arg(speedPara.maxSpeed[i][j]));
             childElement2.appendChild(text_Node);
             childElement.appendChild(childElement2);
         }
         typeElement.appendChild(childElement);
     }

     root.appendChild(typeElement);


     QTextStream out(&file);
     out.setCodec("UTF-8");
     doc.save(out, 4);
     file.close();
}
