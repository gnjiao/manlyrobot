#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>
#include <QTimer>
#include <QMessageBox>
#include <QFont>

#define CR     13       //回车符
#define LF     10       //换行符
#define MAX_DATA_LENGTH     20       //缓冲区长度

//接收到的参数：ID号、实际温度、设定温度：T1、T2、T3、加热通道、温控器状态
class SerialPort : public QSerialPort
{
    Q_OBJECT
public:
    explicit SerialPort(QObject *parent = 0);
    virtual ~SerialPort();

    void initSeialPort();
    bool openSeialPort();
    void readParaCmd(int id=1);//读参数
    void writeParaCmd(int id=1,int T1=380,int T2=380,int T3=380,int channel=0);
    void power(unsigned char st);
    void autoReadPara(int cmd);//自动读参数开关:1,读,0关

    void setTargetTemp(int T1,int T2,int T3);
    int getID();        //ID号
    int getCurrentTemp();//当前实际温度
    int getChannel();   //加热通道
    int getStatus();    //温控器状态
    int getT1();        //设定温度1
    int getT2();        //设定温度2
    int getT3();        //设定温度3

    bool isOpen;
    char receivedData[MAX_DATA_LENGTH];
    unsigned char toSendData[MAX_DATA_LENGTH];

signals:
    void ReceivedDataSig(QByteArray);
    void ReadParaSig();
    void SetTempOk();

public slots:

    void timerRead();  //定时读参数
    void serialRead();//接收温控器数据
    void sendData(const QByteArray & byteArray);
    void setSerialPortInfo(QSerialPortInfo info);
    void analysisData(QByteArray Data);

private:
    QList<QSerialPortInfo>  serialInfos;
    QSerialPortInfo serialInfo;
    int receivedNum;
    QTimer *timer;

    int id;             //ID号:01或02
    int currentTemp;    //当前实际温度：000~500
    int targetTemp[3];  //设定温度：200~500
    int channel;        //加热通道：0~2
    int status;         //温控器状态：0：正常工作状态；  1：S-E(传感器开路)；  2：H-E(发热芯损坏)；  3：OFF
};

#endif // SERIALPORT_H
