#include "serialport.h"

SerialPort::SerialPort(QObject *parent) :
    QSerialPort(parent)
{
    receivedNum=0;
    isOpen=false;
    id=1;             //ID号:01或02
    currentTemp=0;    //当前实际温度：000~500
    targetTemp[0]=0;  //设定温度：200~500
    targetTemp[1]=0;  //设定温度：200~500
    targetTemp[2]=0;  //设定温度：200~500
    channel=0;        //加热通道：0~2
    status=3;         //温控器状态：0：正常工作状态；  1：S-E(传感器开路)；  2：H-E(发热芯损坏)；  3：OFF
    for(int i=0;i<MAX_DATA_LENGTH;i++) receivedData[i]=0;
    timer=new QTimer(this);
}

SerialPort::~SerialPort()
{
    delete timer;
}

void SerialPort::initSeialPort()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(serialRead()));   //连接槽
    connect(this,SIGNAL(ReceivedDataSig(QByteArray)),this,SLOT(analysisData(QByteArray)));   //接收数据
    connect(timer, SIGNAL(timeout()), this, SLOT(timerRead()));
    timer->setInterval(500);
    timer->stop();
/*    serialInfos = QSerialPortInfo::availablePorts();
    if(!serialInfos.isEmpty())
    {
        for(int i=0;i<serialInfos.size();i++)
        {
            if(serialInfos[i].isValid())
            {
                if(serialInfos[i].portName()==QString("COM7"))
                {
                    serialInfo=serialInfos[i];
                    break;
                }
            }
        }
    }
    else return;
    if(serialInfo.isNull()) return;*/
/*
    QMessageBox msgBox;
    //msgBox.setWindowFlags(windowFlags()&~Qt::WindowSystemMenuHint);
    msgBox.setWindowFlags(Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    QFont ft;
    ft.setPointSize(16);
    ft.setItalic(false);
    msgBox.setFont(ft);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(tr("测试串口名"));
    msgBox.addButton(tr("确定"), QMessageBox::AcceptRole);
    msgBox.setText(tr("串口名：%1.").arg(serialInfo.portName()));
    msgBox.exec();
*/
/*    setPort(serialInfo);
    //设置波特率
    setBaudRate(QSerialPort::Baud9600);
    //设置数据位数
    setDataBits(QSerialPort::Data8);
     //设置奇偶校验
    setParity(QSerialPort::NoParity);
    //设置停止位
    setStopBits(QSerialPort::OneStop);
    //设置流控制
    setFlowControl(QSerialPort::NoFlowControl);*/
}

void SerialPort::serialRead()
{
    QByteArray buf=readAll();
    int bytesNum=buf.size();
    if(buf[0]=='!')
    {
        for(int i=0;i<MAX_DATA_LENGTH;i++) receivedData[i]=0;
        if(bytesNum>19) bytesNum=19;
        for(int i=0;i<bytesNum;i++)
            receivedData[i]=buf[i];
        receivedNum=bytesNum;
        if(receivedNum>3)
        {
            if((buf[receivedNum-2]==char(CR)) && (buf[receivedNum-1]==char(LF)))
            {
                emit ReceivedDataSig(buf.left(receivedNum));
                return;
            }
        }
    }
    else
    {
        int indexOfStart=0;
        int indexOfEnd=0;
        indexOfStart=buf.indexOf('!');
        if(buf.contains('!'))
        {
            for(int i=0;i<MAX_DATA_LENGTH;i++) receivedData[i]=0;
            receivedNum=0;
            if(buf.contains(char(CR)) && buf.contains(char(LF)))
            {
                if((buf.indexOf(char(CR))>3) && (buf.indexOf(char(LF))==buf.indexOf(char(CR))+1))
                {
                    indexOfEnd=buf.indexOf(char(LF));
                    bytesNum=indexOfEnd-indexOfStart+1;
                    for(int i=indexOfStart;i<bytesNum;i++)
                        receivedData[i-indexOfStart]=buf[i];
                    receivedNum=bytesNum;
                    if(receivedNum>3)
                    {
                        if((buf[receivedNum-2]==char(CR)) && (buf[receivedNum-1]==char(LF)))
                        {
                            emit ReceivedDataSig(buf.left(receivedNum));
                            return;
                        }
                    }
                }
                else return;    //废除这包数据
            }
            else if(!buf.contains(char(CR)) && !buf.contains(char(LF)) && (buf.size()-indexOfStart<17))
            {
                for(int i=indexOfStart;i<buf.size();i++)
                    receivedData[i-indexOfStart]=buf[i];
                emit ReceivedDataSig(buf.mid(indexOfStart,receivedNum));
                return;
            }
            else return;    //废除这包数据
        }
        else if((buf.contains(char(CR)) && buf.contains(char(LF))) && (buf.indexOf(char(LF))==buf.indexOf(char(CR))+1))
        {
            if((buf.indexOf(char(CR))-indexOfStart<17))
            {
                indexOfEnd=buf.indexOf(char(LF));
                for(int i=0;i<indexOfEnd+1;i++)
                    receivedData[i+receivedNum]=buf[i];
                receivedNum+=indexOfEnd+1;
                emit ReceivedDataSig(QByteArray(receivedData,receivedNum));
                return;
            }
        }
        else if((receivedNum>0) && (buf.size()-indexOfStart<16) && !buf.contains(char(CR)) && !buf.contains(char(LF)))
        {
            for(int i=0;i<buf.size();i++)
                receivedData[i+receivedNum]=buf[i];
            receivedNum+=buf.size();
        }
        else return;    //废除这包数据
    }
}

bool SerialPort::openSeialPort()
{
    if(!isOpen)
        isOpen=open(QIODevice::ReadWrite);
    return isOpen;
}

void SerialPort::sendData(const QByteArray & byteArray)
{
    if(isOpen)
        write(byteArray);
}

void SerialPort::setSerialPortInfo(QSerialPortInfo info)
{
    serialInfo=info;
    setPort(serialInfo);
}
void SerialPort::readParaCmd(int id)
{
    if(id<1 || id>2) id=1;
    QByteArray byteData;
    byteData.append('#');
    byteData.append('0');
    byteData.append(0x30+id);
    byteData.append('r');
    byteData.append('0');
    byteData.append('0');
    byteData.append('0');
    byteData.append('0');
    byteData.append('0');
    byteData.append('0');
    byteData.append(CR);
    byteData.append(LF);

    sendData(byteData);
}

void SerialPort::writeParaCmd(int id,int T1,int T2,int T3,int channel)
{
    if(id<1 || id>2) id=1;
    if(channel<0 || channel>2) channel=0;
    if(T1<200) T1=200;
    if(T1>500) T1=500;
    if(T2<200) T2=200;
    if(T2>500) T2=500;
    if(T3<200) T3=200;
    if(T3>500) T3=500;
    QByteArray byteData;
    byteData.append('#');
    byteData.append('0');
    byteData.append(0x30+id);
    byteData.append('w');
    unsigned char t100,t10,t1;
    t100=T1/100;
    t10=T1/10%10;
    t1=T1%10;
    byteData.append(0x30+t100);
    byteData.append(0x30+t10);
    byteData.append(0x30+t1);
    t100=T2/100;
    t10=T2/10%10;
    t1=T2%10;
    byteData.append(0x30+t100);
    byteData.append(0x30+t10);
    byteData.append(0x30+t1);
    t100=T3/100;
    t10=T3/10%10;
    t1=T3%10;
    byteData.append(0x30+t100);
    byteData.append(0x30+t10);
    byteData.append(0x30+t1);
    byteData.append(0x30+channel);
    byteData.append('0');
    byteData.append(CR);
    byteData.append(LF);
    sendData(byteData);

    QThread::msleep(300);

    //power(0);
    //QThread::msleep(200);
    power(1);
}

void SerialPort::power(unsigned char st)  //开关机
{
    //if(st<0) st=0;
    if(st>1) st=1;
    QByteArray byteData;
/*    byteData.append('#');
    byteData.append('0');
    byteData.append('1');
    byteData.append('c');
    byteData.append(0x30+st);
    byteData.append(CR);
    byteData.append(LF);*/
    byteData.append(0x23);
    byteData.append(0x30);
    byteData.append(0x31);
    byteData.append(0x63);
    byteData.append(0x30+st);
    byteData.append(CR);
    byteData.append(LF);
    sendData(byteData);
}

int SerialPort::getID()        //ID号
{
    return id;
}

int SerialPort::getCurrentTemp()//当前实际温度
{
    return currentTemp;
}

int SerialPort::getChannel()   //加热通道
{
    return channel;
}

int SerialPort::getStatus()    //温控器状态
{
    return status;
}

int SerialPort::getT1()        //设定温度1
{
    return targetTemp[0];
}

int SerialPort::getT2()        //设定温度2
{
    return targetTemp[1];
}

int SerialPort::getT3()        //设定温度3
{
    return targetTemp[2];
}

void SerialPort::analysisData(QByteArray Data)
{
    QString strData=QString(Data.data());
    if((Data.size()==19) && (strData.left(3)=="!01"))//成功读回温度
    {
        status=strData.mid(16,1).toInt();
        if(status==0)
        {
            currentTemp=strData.mid(3,3).toInt();
            targetTemp[0]=strData.mid(6,3).toInt();
            targetTemp[1]=strData.mid(9,3).toInt();
            targetTemp[2]=strData.mid(12,3).toInt();
            channel=strData.mid(15,1).toInt();
            emit ReadParaSig();

            if(channel<0 || channel>2) channel=0;
/*            if(currentTemp==targetTemp[channel])
            {
                emit SetTempOk();
                autoReadPara(0);
            }
            else
                autoReadPara(1);*/
        }
    }
    else if((Data.size()==6) && (strData.left(3)=="!01"))
    {
        status=strData.mid(16,1).toInt();
    }
    else
        status=4;
}

void SerialPort::autoReadPara(int cmd)//自动读参数开关:1,读,0关
{
    if(cmd==1)
    {
        readParaCmd();
        timer->start();
    }
    else
    {
        timer->stop();
    }
}
void SerialPort::timerRead()  //定时读参数
{
    if(currentTemp != targetTemp[channel])
        readParaCmd();
    else
        timer->stop();
}
