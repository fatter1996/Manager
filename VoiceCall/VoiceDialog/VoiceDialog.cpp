#include "VoiceDialog.h"
#include "VoiceCall/Universal.h"
#include <QDebug>
#include <QDateTime>
#include <QtConcurrent>

VoiceDialog::VoiceDialog(QObject *parent)
{
    // 设置音频采样的参数
    m_Format.setSampleRate(GlobalData::m_pConfigInfo->sampleRate);
    m_Format.setChannelCount(GlobalData::m_pConfigInfo->channelCount);
    m_Format.setSampleSize(GlobalData::m_pConfigInfo->sampleSize);
    m_Format.setCodec("audio/pcm");
    m_Format.setByteOrder(QAudioFormat::LittleEndian); // 字节序，小端模式
    m_Format.setSampleType(QAudioFormat::SignedInt);

    //获取默认音频设备
    QList<QAudioDeviceInfo> audioDevice = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    if(audioDevice.size() > 0)
    {
        QAudioDeviceInfo info = audioDevice.at(0);
        qDebug() << "info name : " << info.deviceName();
        //QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
        qDebug() << info.isNull();
        if (!info.isFormatSupported(m_Format))
        {
            qDebug() << "Default format not supported, trying to use the nearest.";
            m_Format = info.nearestFormat(m_Format);
        }


        // 用采样的参数来实例化一个QAudioInput对象
        m_pAudioInput = new QAudioInput(info, m_Format);
        // 用采样的参数来实例化一个QAudioOutput对象
        m_pAudioOutput = new QAudioOutput(m_Format, this);
    }

#if (WORKTYPE == WORKTYPE_SERVER)
    m_pTcpSocket = (TcpSocket*)new ServerSide;
    connect(m_pTcpSocket, &ServerSide::Signalling, this, &VoiceDialog::onSignalling);
    connect(m_pTcpSocket, &ServerSide::NewConnection, this, &VoiceDialog::onNewConnection);
    connect(m_pTcpSocket, &ServerSide::DisConnection, this, &VoiceDialog::MemberQuit);
#elif (WORKTYPE == WORKTYPE_CLIENT)
    m_pTcpSocket = (TcpSocket*)new ClientSide;
    connect(m_pTcpSocket, &ClientSide::Signalling, this, &VoiceDialog::onSignalling);
#endif

    m_pDialogWnd = new VoiceDialogWnd;
    connect(m_pDialogWnd, &VoiceDialogWnd::agreeBtn, this, &VoiceDialog::onAgreeBtn);
    connect(m_pDialogWnd, &VoiceDialogWnd::disAgreeBtn, this, &VoiceDialog::onDisAgreeBtn);
    connect(m_pDialogWnd, &VoiceDialogWnd::closeBtn, this, &VoiceDialog::onCloseBtn);

}

VoiceDialog::~VoiceDialog()
{
    if(m_pAudioInput)
    {
        m_pAudioInput->stop();
        delete m_pAudioInput;
        m_pAudioInput = nullptr;
    }

    if(m_pAudioOutput)
    {
        m_pAudioOutput->stop();
        delete m_pAudioOutput;
        m_pAudioOutput = nullptr;
    }

    if(m_pOutputDevice)
    {
        delete m_pOutputDevice;
        m_pOutputDevice = nullptr;
    }
}

void VoiceDialog::RequestCall()
{
    int nameLenght = GlobalData::m_pConfigInfo->name.toLocal8Bit().length();
    QByteArray array;
    array.append(QByteArray::fromHex("efefefef"));
    array.append(GlobalData::getLittleEndian_2Bit(29 + nameLenght));
    array.append(QByteArray::fromHex("0000"));
    array.append(QByteArray::fromHex("ee"));
    array.append(QByteArray::fromHex("8f"));
    array.append(GlobalData::getLittleEndian_2Bit(nameLenght));
    array.append(GlobalData::m_pConfigInfo->name.toLocal8Bit());
    array.append(GlobalData::getDateTimeArray(QDateTime::currentDateTime()));
    array.append(GlobalData::getIPeArray(GlobalData::m_pConfigInfo->serverIp));
    array.append(GlobalData::getLittleEndian_2Bit(GlobalData::m_pConfigInfo->serverPORT));
    array.append(QByteArray::fromHex("fefefefe"));

#if (WORKTYPE == WORKTYPE_SERVER)
    m_pTcpSocket->sendToClient(array, receiveIp, receivePORT);
#elif (WORKTYPE == WORKTYPE_CLIENT)
    m_pTcpSocket->sendToServer(array);
#endif

    m_pDialogWnd->WaitReply();
    m_pDialogWnd->show();
}


void VoiceDialog::onSignalling(QByteArray data)
{
    switch ((unsigned char)data.at(9))
    {
    case 0x8f : onRequestCall(data); break; //请求通话
    case 0x90 : onRequestReply(data); break; //请求回复
    case 0x91 : onResponse(data); break; //开始通话
    case 0x92 : onCalling(data.at(10)); break; //通话中
    case 0x93 : onHangUp(data); break; //挂断
    case 0x94 : onCallEnd(data); break; //通话结束
    case 0x95 : onCallTimeOut(); break; //呼叫超时
    default: break;
    }
}

void VoiceDialog::onNewConnection(QString ip, uint port, QString name)
{
    emit NewMember(ip, port, name);
}

void VoiceDialog::onDisConnection(QString ip, uint port)
{
    emit MemberQuit(ip, port);
}

void VoiceDialog::onCall(QString ip, uint port)
{
    if(!isRunning)
    {
        receiveIp = ip;
        receivePORT = port;
        RequestCall();
    }
}

void VoiceDialog::onRequestCall(QByteArray data)
{
    if(isRunning) //正在通话中,回复占线
    {
        return;
    }

    int len = GlobalData::getBigEndian_2Bit(data.mid(10, 2));
    QString name = QString::fromLocal8Bit(data.mid(12, len));
    m_pDialogWnd->NewCall(name);
    m_pDialogWnd->show();
}

void VoiceDialog::onRequestReply(QByteArray data)
{
    if(data.at(10) == 0x01)
    {
        //发送开始通话命令
        QByteArray array;
        array.append(QByteArray::fromHex("efefefef"));
        array.append(GlobalData::getLittleEndian_2Bit(27));
        array.append(QByteArray::fromHex("0000"));
        array.append(QByteArray::fromHex("ee"));
        array.append(QByteArray::fromHex("91"));
        array.append(GlobalData::getDateTimeArray(QDateTime::currentDateTime()));
        array.append(GlobalData::getIPeArray(GlobalData::m_pConfigInfo->localIp));
        array.append(GlobalData::getLittleEndian_2Bit(GlobalData::m_pConfigInfo->localPORT));
        array.append(QByteArray::fromHex("fefefefe"));

#if (WORKTYPE == WORKTYPE_SERVER)
    m_pTcpSocket->sendToClient(array, receiveIp, receivePORT);
#elif (WORKTYPE == WORKTYPE_CLIENT)
    m_pTcpSocket->sendToServer(array);
#endif

        //界面显示
        m_pDialogWnd->StartChat();
        m_pDialogWnd->show();
        //开始语音传输
        onResponse(data);
    }
    else
    {
        m_pDialogWnd->DisAgree();
    }
}

void VoiceDialog::onResponse(QByteArray data)
{
    //接通后开始发送语音数据
    m_pInputDevice = m_pAudioInput->start();
    //开始接收对方语音数据
    m_pOutputDevice = m_pAudioOutput->start();
    if(m_pInputDevice)
    {
#if (WORKTYPE == WORKTYPE_SERVER)
        QString ip = QString("%1.%2.%3.%4")
                .arg(QString::number((unsigned char)data.at(11), 10))
                .arg(QString::number((unsigned char)data.at(12), 10))
                .arg(QString::number((unsigned char)data.at(13), 10))
                .arg(QString::number((unsigned char)data.at(14), 10));

        uint port = GlobalData::getBigEndian_2Bit(data.mid(15, 2));
#elif (WORKTYPE == WORKTYPE_CLIENT)
        QString ip = QString("%1.%2.%3.%4")
                .arg(QString::number((unsigned char)data.at(17), 10))
                .arg(QString::number((unsigned char)data.at(18), 10))
                .arg(QString::number((unsigned char)data.at(19), 10))
                .arg(QString::number((unsigned char)data.at(20), 10));

        uint port = GlobalData::getBigEndian_2Bit(data.mid(21, 2));
#endif

        m_pUdpSocket = new UdpSocket(ip, port, m_pOutputDevice);
        connect(m_pUdpSocket, &UdpSocket::voiceReceive, this, [=](QByteArray data){
            if(data.length() > 0 && m_pOutputDevice)
                m_pOutputDevice->write(data);
            qDebug() << "receive voice  " << data.length();
        });

        QtConcurrent::run(this, &VoiceDialog::getVoiceData);
        m_pUdpSocket->start();
    }

}

void VoiceDialog::getVoiceData()
{
    isRunning = true;
    while(isRunning)
    {
        msleep(1);
        if(!m_pInputDevice) break;
        QByteArray arr = m_pInputDevice->peek(4096 * 2);
        if(arr.length() > 0)
        {
            //qDebug() << "success   " << arr.length();
            m_pUdpSocket->send(arr);
        }
    }
}

void VoiceDialog::onVoiceReceive(QByteArray data)
{
    m_pOutputDevice->write(data);
}

void VoiceDialog::onCalling(uint type)
{
    if(type == 0x10)
    {
#if (WORKTYPE == WORKTYPE_SERVER)
    m_pTcpSocket->sendToClient(HeartBeat(0x20), receiveIp, receivePORT);
#endif
    }
    else if(type == 0x20)
    {
        qDebug() << "In conversation.";
    }
}

void VoiceDialog::onHangUp(QByteArray data)
{
    isRunning = false;
    if(m_pUdpSocket)
    {
        m_pUdpSocket->stop();
        m_pUdpSocket->wait();
    }
    if(m_pAudioInput)
        m_pAudioInput->stop();
    if(m_pAudioOutput)
        m_pAudioOutput->stop();
}

void VoiceDialog::onCallEnd(QByteArray data)
{
    isRunning = false;
    if(m_pUdpSocket)
    {
        m_pUdpSocket->stop();
        m_pUdpSocket->wait();
    }
    if(m_pAudioInput)
        m_pAudioInput->stop();
    if(m_pAudioOutput)
        m_pAudioOutput->stop();
    if(m_pDialogWnd)
        m_pDialogWnd->ChatEnd();
}

void VoiceDialog::onCallTimeOut()
{

}

void VoiceDialog::onAgreeBtn()
{
    QByteArray array;
    array.append(QByteArray::fromHex("efefefef"));
    array.append(QByteArray::fromHex("1500"));
    array.append(QByteArray::fromHex("0000"));
    array.append(QByteArray::fromHex("ee"));
    array.append(QByteArray::fromHex("90"));
    array.append(QByteArray::fromHex("01"));
    array.append(GlobalData::getIPeArray(GlobalData::m_pConfigInfo->localIp));
    array.append(GlobalData::getLittleEndian_2Bit(GlobalData::m_pConfigInfo->localPORT));
    array.append(QByteArray::fromHex("fefefefe"));

#if (WORKTYPE == WORKTYPE_SERVER)
    m_pTcpSocket->sendToClient(array, receiveIp, receivePORT);
#elif (WORKTYPE == WORKTYPE_CLIENT)
    m_pTcpSocket->sendToServer(array);
#endif
    m_pDialogWnd->StartChat();
}

void VoiceDialog::onDisAgreeBtn()
{
    QByteArray array;
    array.append(QByteArray::fromHex("efefefef"));
    array.append(QByteArray::fromHex("0f00"));
    array.append(QByteArray::fromHex("0000"));
    array.append(QByteArray::fromHex("ee"));
    array.append(QByteArray::fromHex("90"));
    array.append(QByteArray::fromHex("00"));
    array.append(QByteArray::fromHex("fefefefe"));

#if (WORKTYPE == WORKTYPE_SERVER)
    m_pTcpSocket->sendToClient(array, receiveIp, receivePORT);
#elif (WORKTYPE == WORKTYPE_CLIENT)
    m_pTcpSocket->sendToServer(array);
#endif
}

void VoiceDialog::onCloseBtn(int timeLenght)
{
    QByteArray array;
    array.append(QByteArray::fromHex("efefefef"));
    array.append(QByteArray::fromHex("1000"));
    array.append(QByteArray::fromHex("0000"));

#if (WORKTYPE == WORKTYPE_SERVER)
        array.append(QByteArray::fromHex("ee"));
#elif (WORKTYPE == WORKTYPE_CLIENT)
        array.append(QByteArray::fromHex("ff"));
#endif

    if(m_pDialogWnd->State() == 1) //取消呼叫
    {
        array.append(QByteArray::fromHex("95"));
        array.append(QByteArray::fromHex("01"));
    }
    else if(m_pDialogWnd->State() == 2) //超时
    {
        array.append(QByteArray::fromHex("95"));
        array.append(QByteArray::fromHex("02"));
    }
    else if(m_pDialogWnd->State() == 3) //挂断
    {
        array.append(QByteArray::fromHex("94"));
        array.append(GlobalData::getLittleEndian_2Bit(timeLenght));
    }
    array.append(QByteArray::fromHex("fefefefe"));

#if (WORKTYPE == WORKTYPE_SERVER)
        m_pTcpSocket->sendToClient(array, receiveIp, receivePORT);
#elif (WORKTYPE == WORKTYPE_CLIENT)
        m_pTcpSocket->sendToServer(array);
#endif

    onCallEnd(array);
}

QByteArray VoiceDialog::HeartBeat(uint type)
{
    QByteArray array;
    array.append(QByteArray::fromHex("efefefef"));
    array.append(QByteArray::fromHex("0f00"));
    array.append(QByteArray::fromHex("0000"));
    array.append(QByteArray::fromHex("ee"));
    array.append(QByteArray::fromHex("92"));
    array.append(type);
    array.append(QByteArray::fromHex("fefefefe"));
    return array;
}

void VoiceDialog::setAddress(QString ip, uint port)
{
    receiveIp = ip;
    receivePORT = port;
}
