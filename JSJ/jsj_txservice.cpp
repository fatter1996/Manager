#include "jsj_txservice.h"
#include<QUdpSocket>
#include<QTimer>
jsj_txService::jsj_txService(QObject *parent)
{

}




bool jsj_txService::bindPort()
{
    if(bind(QHostAddress(DBSERVICES.LoginSQLData.info.LocalUdpIP),DBSERVICES.LoginSQLData.info.m_LocalPort))
    {
        connect(this,SIGNAL(readyRead()),
                this,SLOT(onSocketReadyRead()));
        HeartTimer = new QTimer (this);
        HeartTimer->setInterval(1000);
        HeartTimer->start();
        connect(HeartTimer,SIGNAL(timeout()),this,SLOT(HeartStartInfo()));
        return true;
    }
}
jsj_txService &jsj_txService::getDBService()
{
    static jsj_txService s_DBService;
    return  s_DBService;
}

void jsj_txService::UpDate_WrongSetStatus(unsigned char Cmd)
{

    if(Cmd == ZCWRONG)
        DBSERVICES.tempControl.zc_GZ_Set = true;
    else if (Cmd == CCHDS)
        DBSERVICES.tempControl.ccnumberLose_GZ_Set = true;
    else if (Cmd == BSZD)
        DBSERVICES.tempControl.ctcshowMiss_GZ_Set = true;
}

void jsj_txService::SetStatus(bool &Status)
{
    Status = !Status;
}
void jsj_txService::Send_ShutCmd_(unsigned int StationId)
{
    GetAddressByStationId(StationId);
    unsigned char RecArray[15] = { 0 };
    RecArray[10] = StationId;
    InitCmd(RecArray,15,CTC,ShutDown,StationId);
    SendCMD(RecArray, 15);
}

void jsj_txService::Send_GZInfo(bool &isSet, unsigned char gzCode, unsigned int StationId)
{
    unsigned char SendArray[15000] = { 0 };
    if(isSet)
        SendArray[10] = GZ_Set;
    else
        SendArray[10] = GZ_No_Set;
    SendArray[11] = 0x01;
    SendArray[12] = gzCode;
    GetAddressByStationId(StationId);
    InitCmd(SendArray,17,CTC,GZ_SetCode,StationId);
    SendCMD(SendArray, 17);
}
void str_cpy_01(char* dest, char* resource) {

    int length = strlen(resource);				// strlen()函数传入的是数组指针或指针变量
    for (int i = 0; i < length; i++) {
        dest[i] = resource[i];
    }
    dest[length] = '\0';
};
char * GetByUn(unsigned char temp[]){
    void* v8 = static_cast<void*>(temp);
    char* c8 = static_cast<char*>(v8);
    return c8;
}
int jsj_txService::SendCMD(unsigned char *Arrary, int FrameLen)
{
    unsigned char DataArray[15000] = {0};
    memcpy(DataArray, Arrary, FrameLen);
    m_IPInfo.m_OppIP.setAddress(tempAddress.Ip);
    m_IPInfo.m_OppPort = tempAddress.Port;
    qDebug()<<"发送IP:"<< m_IPInfo.m_OppIP<<"     "<< m_IPInfo.m_OppPort;
    int len = writeDatagram((char*)DataArray, FrameLen,m_IPInfo.m_OppIP, m_IPInfo.m_OppPort);
    return  len;
}
//ef ef ef ef length1 length2 01(本机地址码) 03(目标地址码) 0xbb(软件类型) ... fe fe fe fe
void jsj_txService::Send_Currentto_CTC(DDML&ml)
{
    int len = 0;
    unsigned char RecArray[15000] = { 0 };
    QByteArray byte = ml.ML_Number.toUtf8();

    memcpy(RecArray+10,byte,10);
    memcpy(RecArray+20,&ml.StartTime[0],2);
    RecArray[22] = ml.StartTime[1];
    RecArray[23] = ml.StartTime[2];
    RecArray[24] = ml.StartTime[3];
    RecArray[25] = ml.StartTime[4];
    RecArray[26] = ml.StartTime[5];
    memcpy(RecArray+27,ml.ML_FLR.toLocal8Bit(),20);//命令发送对象是谁？
    memcpy(RecArray+47,ml.ML_Type.toLocal8Bit(),40);
    int Length = ml.ML_Content.toLocal8Bit().length();
    memcpy(RecArray+87,&Length,2);
    memcpy(RecArray+89,(ml.ML_Content.toLocal8Bit()),(Length));
    len = Length+95;
    GetAddressByStationId(ml.Stationid);
    InitCmd(RecArray,len,CTC,DDML_,ml.Stationid);
    SendCMD(RecArray, len);
    return;
}
void jsj_txService::Send_StartSoft(QString Ip, unsigned int Port, QString PathAll, bool isOffLine)
{
    unsigned char SendArray[15000] = { 0 };
    SendArray[0] = 0xef;
    int length = strlen(PathAll.toUtf8().data());
    memcpy(&SendArray[1],&length,2);
    memcpy(&SendArray[3],&isOffLine,1);
    memcpy(&SendArray[4],PathAll.toUtf8().data(),length);
    SendArray[length+10] = 0xfe;
    SendArray[length+11] = 0xfe;
    tempAddress.Ip = Ip;
    tempAddress.Port = Port;
    QByteArray array;
    SendCMD(SendArray,length+11);
}
void jsj_txService::Send_xsmlto_CTC(DDML &ML, unsigned char SetCmd, unsigned char Startflag)
{
    unsigned char RecArray[15000] = { 0 };
    int CMN_Length = ML.ML_Number.toUtf8().length();
    RecArray[10] =CMN_Length;
    memcpy(RecArray+11,ML.ML_Number.toUtf8(),CMN_Length);
    int NowIndex = 10 + CMN_Length;
    RecArray[NowIndex+1] = SetCmd;
    memcpy(RecArray+NowIndex+2,&ML.SpeedRange,2);
    //NowIndex +=1;
    memcpy(RecArray+NowIndex+4,&ML.StartTime[0],2);
    RecArray[NowIndex+6] = ML.StartTime[1];
    RecArray[NowIndex+7] = ML.StartTime[2];
    RecArray[NowIndex+8] = ML.StartTime[3];
    RecArray[NowIndex+9] = ML.StartTime[4];
    RecArray[NowIndex+10] = ML.StartTime[5];
    memcpy(RecArray+NowIndex+11,&ML.EndTime[0],2);
    RecArray[NowIndex+13] = ML.EndTime[1];
    RecArray[NowIndex+14] = ML.EndTime[2];
    RecArray[NowIndex+15] = ML.EndTime[3];
    RecArray[NowIndex+16] = ML.EndTime[4];
    RecArray[NowIndex+17] = ML.EndTime[5];

    RecArray[NowIndex + 18] = Startflag;
    int Size =  ML.vect_dcDev.length();
    RecArray[NowIndex + 19] = Size;
    int Var = NowIndex + 20;
    for(int i = 0; i < ML.vect_dcDev.size();++i)
    {
        memcpy(RecArray+Var,&ML.vect_dcDev[i].shebeiCode,2);
        Var+=2;
    }
    RecArray[Var] = ML.vect_qdDev.length();
    Var++;
    for(int i = 0; i < ML.vect_qdDev.size();++i)
    {

        memcpy(RecArray+Var,&ML.vect_qdDev[i].shebeiCode,2);
        Var+=2;
    }
    RecArray[Var] = ML.vect_qjDev.length();
    Var++;
    for(int i = 0; i < ML.vect_qjDev.size();++i)
    {
        memcpy(RecArray+Var,&ML.vect_qjDev[i].shebeiCode,2);
        Var+=2;
    }
    GetAddressByStationId(ML.Stationid);
    Var +=4;
    InitCmd(RecArray,Var,CTC,XSML_,ML.Stationid);
    SendCMD(RecArray, Var);

}
void jsj_txService::InitCmd(unsigned char *Array, int FrameLen, unsigned char targetCode,
                            unsigned int sendcode, unsigned int StationId)
{
    unsigned char temp[1000];
    for (int i = 0; i < 4; i++)
        Array[i] = 0xEF;//帧头
    memcpy(Array + 4, &FrameLen, 2);//长度
    Array[6] = StationId;
    Array[7] = targetCode;
    Array[8] = TYPE;
    Array[9] = sendcode;
    for (int i = 0; i < 4; i++)
        Array[FrameLen - 1 - i] = 0xFE; //帧尾
    memcpy(temp,Array,FrameLen);
}
void jsj_txService::Send_Lone_StationJHto_CTC(JHINFO &iNFO, unsigned char TargetCode)
{

    unsigned char RecArray[15000] = { 0 };

    int PlanNumber = iNFO.jh_PlanNumber.toUInt();
    memcpy(RecArray+10, &PlanNumber,4);
    unsigned int le = iNFO.jh_ChufaCheci.length();
    RecArray[14] =  le;
    memcpy(RecArray+15,(unsigned char*)qstrdup(iNFO.jh_ChufaCheci.toLatin1().constData()),le);//字符串拷贝
    unsigned int Size = 15 + le;
    RecArray[Size] = le;
    memcpy(RecArray+Size+1,(unsigned char*)qstrdup(iNFO.jh_ChufaCheci.toLatin1().constData()),le);//字符串拷贝
    Size = Size+le+1;
    RecArray[Size] = 0x11;//添加
    memcpy(RecArray+Size+1,&iNFO.GD_Index,2);
    memcpy(RecArray+Size+3,&iNFO.Arrivetime[0],2);
    memcpy(RecArray+Size+5,&iNFO.Arrivetime[1],1);
    memcpy(RecArray+Size+6,&iNFO.Arrivetime[2],1);
    memcpy(RecArray+Size+7,&iNFO.Arrivetime[3],1);
    memcpy(RecArray+Size+8,&iNFO.Arrivetime[4],1);
    memcpy(RecArray+Size+9,&iNFO.Arrivetime[5],1);
    Size = Size+10;
    memcpy(RecArray+Size,&iNFO.GD_Index,2);
    memcpy(RecArray+Size+2,&iNFO.ChuFaTime[0],2);
    memcpy(RecArray+Size+4,&iNFO.ChuFaTime[1],1);
    memcpy(RecArray+Size+5,&iNFO.ChuFaTime[2],1);
    memcpy(RecArray+Size+6,&iNFO.ChuFaTime[3],1);
    memcpy(RecArray+Size+7,&iNFO.ChuFaTime[4],1);
    memcpy(RecArray+Size+8,&iNFO.ChuFaTime[5],1);

    Size = Size+9;
    memcpy(RecArray+Size,&iNFO.jfc_Type,1);
    memcpy(RecArray+Size+1,&iNFO.JC_Index,2);
    memcpy(RecArray+Size+3,&iNFO.Fc_Index,2);
    memcpy(RecArray+Size+5,&iNFO.qytype,1);
    memcpy(RecArray+Size+6,&iNFO.cxtype,1);
    memcpy(RecArray+Size+7,&iNFO.GetKind,1);
    memcpy(RecArray+Size+8,&iNFO.TrainType,1);
    memcpy(RecArray+Size+9,&iNFO.YunxinType,1);
    int lenth = Size + 14;
    GetAddressByStationId(iNFO.StationId);
    InitCmd(RecArray,lenth,TargetCode,JDJH,iNFO.StationId);
    SendCMD(RecArray, lenth);
    //前列后运
}
#include"jsj_plannode.h"
void jsj_txService::Send_MultiPlan_toCTC(jsj_planNode &sendNode,unsigned char TargetCode)
{
    for(int i = 0 ; i < sendNode.CurMultiPlanInfo.size();++i)
    {
        PalenInfo iNFO = sendNode.CurMultiPlanInfo[i];
        unsigned char RecArray[15000] = { 0 };

        int PlanNumber = sendNode.PlanName.toUInt();
        memcpy(RecArray+10, &PlanNumber,4);
        unsigned int le = sendNode.Checi.length();
        RecArray[14] =  le;
        memcpy(RecArray+15,(unsigned char*)qstrdup(sendNode.Checi.toLatin1().constData()),le);//字符串拷贝
        unsigned int Size = 15 + le;
        RecArray[Size] = le;
        memcpy(RecArray+Size+1,(unsigned char*)qstrdup(sendNode.Checi.toLatin1().constData()),le);//字符串拷贝
        Size = Size+le+1;
        RecArray[Size] = 0x11;//添加
        memcpy(RecArray+Size+1,&iNFO.GDIndex,2);
        memcpy(RecArray+Size+3,&iNFO.Arrivetime_[0],2);
        memcpy(RecArray+Size+5,&iNFO.Arrivetime_[1],1);
        memcpy(RecArray+Size+6,&iNFO.Arrivetime_[2],1);
        memcpy(RecArray+Size+7,&iNFO.Arrivetime_[3],1);
        memcpy(RecArray+Size+8,&iNFO.Arrivetime_[4],1);
        memcpy(RecArray+Size+9,&iNFO.Arrivetime_[5],1);
        Size +=10;
        memcpy(RecArray+Size,&iNFO.GDIndex,2);
        memcpy(RecArray+Size+2,&iNFO.ChuFaTime_[0],2);
        memcpy(RecArray+Size+4,&iNFO.ChuFaTime_[1],1);
        memcpy(RecArray+Size+5,&iNFO.ChuFaTime_[2],1);
        memcpy(RecArray+Size+6,&iNFO.ChuFaTime_[3],1);
        memcpy(RecArray+Size+7,&iNFO.ChuFaTime_[4],1);
        memcpy(RecArray+Size+8,&iNFO.ChuFaTime_[5],1);

        Size+=9;
        memcpy(RecArray+Size,&iNFO.JFC_Data,1);
        memcpy(RecArray+Size+1,&iNFO.JCIndex,2);
        memcpy(RecArray+Size+3,&iNFO.FCIndex,2);
        memcpy(RecArray+Size+5,&sendNode.QY_Index,1);
        memcpy(RecArray+Size+6,&sendNode.cxindex,1);
        memcpy(RecArray+Size+7,&sendNode.Type,1);
        memcpy(RecArray+Size+8,&sendNode.liecheType_,1);
        memcpy(RecArray+Size+9,&sendNode.YunxinType_,1);

        int lenth = Size + 14;
        GetAddressByStationId(iNFO.Stationid);
        InitCmd(RecArray,lenth,TargetCode,JDJH,iNFO.Stationid);
        SendCMD(RecArray, lenth);
        //前列后运
    }
}

void jsj_txService::Send_Resetto_CTC(unsigned int StationId)
{
    GetAddressByStationId(StationId);
    unsigned char RecArray[15] = { 0 };
    RecArray[10] = StationId;
    InitCmd(RecArray,15,CTC,ZCCZ,StationId);
    SendCMD(RecArray, 15);
}
#include<QDateTime>
void jsj_txService::Send_Clock_Same(unsigned int StationId, QDateTime time, unsigned char TargetCode)
{
    GetAddressByStationId(StationId);
    unsigned char RecArray[1000] = { 0 };
    unsigned int year = time.date().year();
    memcpy(RecArray+10,&year,2);
    unsigned int month = time.date().month();
    memcpy(RecArray+12,&month,1);
    unsigned int Day = time.date().day();
    memcpy(RecArray+13,&Day,1);
    unsigned int Hour = time.time().hour();
    unsigned int Sec = time.time().second();
    unsigned int Min = time.time().minute();
    memcpy(RecArray+14,&Hour,1);
    memcpy(RecArray+15,&Min,1);
    memcpy(RecArray+16,&Sec,1);
    InitCmd(RecArray,21,TargetCode,CLOCKSAME,StationId);
    SendCMD(RecArray, 21);
}

unsigned int jsj_txService::GetIdByStationName(QString StationName)
{
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(DBSERVICES.CurBackInfoList[i].StationName == StationName)
            return DBSERVICES.CurBackInfoList[i].StationId;
    }
}

void jsj_txService::Send_ControlCmd_Back(QString StationName, unsigned char before, unsigned char After, unsigned char agreeflag, unsigned char targerCode)
{
    int StationId = GetIdByStationName(StationName);
    GetAddressByStationId(StationId);
    unsigned char RecArray[1000] = { 0 };
    memcpy(RecArray+10,&before,1);
    memcpy(RecArray+11,&After,1);
    memcpy(RecArray+12,&agreeflag,1);
    InitCmd(RecArray,17,targerCode,TCC_TOCTC_CONTROL,StationId);
    SendCMD(RecArray, 17);
}
SendAddress jsj_txService::GetAddressByStationId(int StationId)
{
    tempAddress.initInfo();
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(DBSERVICES.CurBackInfoList[i].StationId == StationId)
        {
            tempAddress.Ip = DBSERVICES.CurBackInfoList[i].IPStr;
            tempAddress.Port = DBSERVICES.CurBackInfoList[i].Port;
            break;
        }
    }
    return tempAddress;
}
#include<qglobal.h>
void jsj_txService::processTheDatagram(QByteArray array, QHostAddress Address, int Port)
{
    QByteArray array_Rec;
    array_Rec.resize(array.size());
    array_Rec = array;
    const int length = array_Rec.size();
    if ((unsigned char)array_Rec[0] == 0xef && ((unsigned char)array_Rec[length-1] == 0xfe))

    {
        if ((unsigned char)array_Rec[9] == JDJH) //阶段计划接收信息
        {
            char buf[1000];//数组
            int len_array = array_Rec.size();
            int len_buf = sizeof(buf);
            int len = qMin( len_array, len_buf );
            // 转化
            memcpy( buf, array_Rec,  len );
            int Plannumber;QString CheciNumber;
            memcpy(&Plannumber,&buf[10],4);
            int length = array[14];
            CheciNumber = array_Rec.mid(14,length);
            emit Set_jdjh_Slot(QString::number(Plannumber),CheciNumber);
        }
        else if ((unsigned char) array_Rec[9] == DDML_)
        {
            QString Number = array_Rec.mid(10,10);
            Number.remove(QRegExp("\\s"));
            emit SetDDML_Slot(Number);
            qDebug()<<"接收到调度命令信息:"<<Number;
        }
        else if ((unsigned char) array_Rec[9] == XSML_)
        {
            int length = array[10];
            QString NUmber = array_Rec.mid(11,length);
            emit SetDDML_Slot(NUmber);
            qDebug()<<"接收到限速命令信息:"<<NUmber;
        }
        else if ((unsigned char) array_Rec[9] == CONTROLCMD)
        {
            DBSERVICES.tempControl.initinfo();
            DBSERVICES.tempControl.StationName =  GetStationName(Port);
            DBSERVICES.tempControl.BeforeCMD = array_Rec[10];
            DBSERVICES.tempControl.AfterCMD = array_Rec[11];
            emit ChangeControl_Cmd(DBSERVICES.tempControl);
        }
        else if ((unsigned char)array_Rec[9] ==GZ_No_Set||
                 (unsigned char)array_Rec[9] ==GZ_Set)
        {

            UpDate_WrongSetStatus(array_Rec[12]);
        }
        else
        {
            if((unsigned char) array_Rec[9] == HEARTBEAT)
            {
                Port_ = Port;
                Address_ = Address;
                StationId_ = array_Rec[6];
                qDebug()<<"HeatBeat:"<<StationId_;
                GetHeartBeatJudge(StationId_,Address_,Port_);
            }
        }
    }
}
QString jsj_txService::GetUserControlInfo(unsigned char After)
{
    if (After == CENTRT_CONTROL)
        return "中心控制";
    else if (After == Station_CONTROL)
        return "车站控制";
    else if (After == FSZL_CONTROL)
        return "分散自律";
    return "";
}

QString jsj_txService::GetStationName(unsigned int Port)
{
    for ( int index = 0; index <DBSERVICES.CurBackInfoList.size();index++)
    {
        if(DBSERVICES.CurBackInfoList[index].Port == Port)
            return DBSERVICES.CurBackInfoList[index].StationName;
    }
    return "";
}
void jsj_txService::GetHeartBeatJudge(int StationId, QHostAddress Address, int Port)
{
    if(Address.toString() == "")
        return;
    for ( int index = 0; index <DBSERVICES.CurBackInfoList.size();index++)
    {
        if(StationId == DBSERVICES.CurBackInfoList[index].StationId)
        {
            //qDebug()<<"连上站场ID:"<<StationId<<"连上站场名称:"<<DBSERVICES.CurBackInfoList[index].StationName;
            DBSERVICES.CurBackInfoList[index].isConnected = true;
            if(DBSERVICES.CurBackInfoList[index].Stationheart!=nullptr)
            {
                DBSERVICES.CurBackInfoList[index].InitMulti();
            }else
            {
                DBSERVICES.CurBackInfoList[index].IPStr = Address.toString();
                DBSERVICES.CurBackInfoList[index].Port = Port;
                DBSERVICES.CurBackInfoList[index].GetStationHeartInit(StationId);
            }
        }
        else{
            if(DBSERVICES.CurBackInfoList[index].Stationheart!=nullptr)
            {
                if(DBSERVICES.CurBackInfoList[index].GetIsOffLine())//则说明掉线了
                {
                    DBSERVICES.CurBackInfoList[index].isConnected = false;
                    // qDebug()<<"已经掉线2";
                }
            }
        }
    }
    emit DBSERVICES.UpdateStation();
}
void jsj_txService::onSocketReadyRead()
{
    while (hasPendingDatagrams())
    {
        // 数据缓冲区，类似 char [ ]
        QByteArray datagram;
        // 获取当前数据包的大小，从到来的套接字数据包
        datagram.resize(pendingDatagramSize());   //这里为什么会阻塞
        QHostAddress sender;  // 保存客户端的IP地址
        quint16 senderPort;  // 保存客户端的端口号
        // 读取数据，.data ( )完成的是数据类型转换为 char *
        readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        // 解析和处理...
        processTheDatagram(datagram,sender,senderPort);
    }
}

void jsj_txService::HeartStartInfo()
{
    GetHeartBeatJudge(StationId_,Address_,Port_);
    StationId_ = -1;
}
