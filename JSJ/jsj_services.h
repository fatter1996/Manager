#ifndef JSJ_SERVICES_H
#define JSJ_SERVICES_H
#pragma execution_character_set("utf-8")

#include<QHBoxLayout>
#include<vector>
#include <QObject>
#include<QDateTime>
#include<QTableWidget>
#include<QCheckBox>
#include"testT/basestructure.h"
#include"jsj_stationheart.h"
#define DDMLINFO        0x00
#define XSMLINFO       0x01
#define JHXFINFO       0x02
#define DRAWINFO       0x03
#define  EDIT       0x01
#define  ADD        0x02
#define Sended   0x04
#define Finished 0x06
#define Send_toBack 0x05
#define BLUE                    qRgb(10,95,189)
#define SAVE  0X01
#define SEND  0X02
#define SX    0x01
#define DX    0x02


#define JFC 0Xaa
#define SF 0xbb
#define ZD 0xcc
#define TG 0xdd

#define SF_ 0xbb
#define ZD_ 0xcc
#define TG_ 0xdd
#define ZC_ 0xaa

#define UpOne       0x01
#define MiddleOne   0x02
#define LastOne     0x03
#define Up_Ahead    0x04
#define Down_Ahead  0x05

#define Edit   0x01
#define Excute 0x02
#define Del    0x03
#define APPEND    0x04

#define  KSLK  0x00
#define  PTLK  0x01
#define  DCZ   0x04
#define TBLK   0x07
#define HWLC   0x02
#define XBLC   0x03
#define TKHWLC 0x05
#define KSHWLC 0x06
#define NO_CX  0x00
#define ONE_CX 0x01
#define TWO_CX 0x02
#define THREE_CX 0x03
#define Super_CX 0X04
#define Eletric  0x11
#define UN_Eletric 0x00

#define NEWLINE  "\r\n"
#include<QHostAddress>
#include<QComboBox>
class jsj_planNode;
using namespace std;
struct IPInfo
{
    QHostAddress m_LocalIP;
    QHostAddress m_OppIP;
    quint16 m_LocalPort;
    quint16 m_OppPort;

    QDateTime Time;

    QString LocalUdpIP = "";
    int TCCPort;
    int LocalUdpPort;
    int LocalUdpTrainPort;
    int ScrollSize_Width;
    int ScrollSize_Height;

   inline IPInfo(QString localip, QString localport, QString oppip, QString oppport)
    {
        m_LocalIP.setAddress(localip);
        m_OppIP.setAddress(oppip);
        m_LocalPort = localport.toUShort();
        m_OppPort = oppport.toUShort();
    }
    IPInfo()
    {
        m_LocalIP.setAddress("");
        m_OppIP.setAddress("");
        m_LocalPort = quint16();
        m_OppPort = quint16();
    }
};
struct DDINFO{
    QString DDMLCommond;
    QString Content;
   inline void initinfo()
    {
        DDMLCommond = "";
        Content = "";
    }
};
struct ControlInfo{
    bool zc_GZ_Set = true;
    bool ccnumberLose_GZ_Set = true;
    bool ctcshowMiss_GZ_Set = true;
    QString StationName;
    unsigned char BeforeCMD;
    unsigned char AfterCMD;
  inline  void initinfo(){
        StationName = "";
        BeforeCMD = 0x00;
        AfterCMD = 0X00;
    }
};
struct PalenInfo{
    unsigned int JCIndex;//接车
    unsigned int FCIndex;//发车
    unsigned int GDIndex;//股道索引
    unsigned int Stationid;
    int Arrivetime_[6] = {0};
    int ChuFaTime_[6] = {0};
    QString JC_Info;//接车信息
    QString FC_Info;//发车信息
    QString GDSetName;//所经过股道名称

    int SS_Stauts;//所属信息
    QString StationName;//站场名称
    QString SS_Time;//当前站下发时间
    QString jfc_TYPE;//接发车类型
    unsigned int JFC_Data;
    QString CheCi;//车次信息

    QString ArriveTime;//到达时刻
    QString ChuFaTime;//出发时间
    QString JieChe_Direct;//接车方向
    QString FaChe_Direct;//发车方向
    QString CX_Info;//超限信息
    QString QY_Type;//牵引类型
    QString CC_TYPE;//车次类型
};
struct SheBeiInfo{
    QString shebeiName;
    unsigned int shebeiCode;
};
struct DDML
{
    unsigned int Stationid;
    QString ML_T = "";//命令具体类型
    QString ML_Type = "";//命令类型
    QString ML_Number = "";//命令号
    QString ML_DDY;//命令调度员
    QString ML_DDT;//命令调度台
    QString ML_FLR = "枢纽调度员";
    QString ML_Content="";//调度内容
    QString ML_JLOne;//命令接令对象
    QString  ML_DownTime;//点击发送后自动写入时间
    QString ML_ZBZR;//值班主任
    bool ML_SQStatus = false;
    int Status = -1;
    unsigned int ML_Tpe = -1;
    QString ML_RecTime;//命令签收时间(需要当前登录者签名之后 才写入签收时间)
    QStringList ML_QSR;//命令签收人
    int Sec = 56;
    int Min = 15;
    int Hour = 3;
    int Day = 16;
    int Month = 4;
    int year = 2022;
    int EndTime[6];//结束时间  从0开始-----年/月/日/时/分/秒
    int StartTime[6];//开始时间 从0开始-----年/月/日/时/分/秒
    int SpeedRange;//限速范围 0---最小值  1最大值
    QList<SheBeiInfo> vect_dcDev;
    QList<SheBeiInfo> vect_qdDev;
    QList<SheBeiInfo> vect_qjDev;
    bool XsIsSend = false;
   inline QString GetCommondStr(int type)
    {
        if(type == DDMLINFO)
            return "调度命令";
        else
            return "限速命令";
    }
    void initInfo()
    {
        vect_dcDev.clear();
        vect_qdDev.clear();
        vect_qjDev.clear();
    }
};
struct TGInfo
{
    QString JC_Info;
    QString GD_Info;
    QString FC_Info;
};
struct BackInfo
{

    QString IPStr;
    int Port;
    jsj_Stationheart *Stationheart = nullptr;
    bool isConnected = false;//判断已经上线的

    QString StationName;
    int StationId;
    QStringList TeamList;
    QStringList TeamIdList;
    QStringList GangweiList;
    QStringList GangWeiIdList;
    QStringList ruleList;
    QStringList ruleIDList;
    QString AdminLoginName;
    QString AdminLoginPassword;
    QList<TGInfo>TGList;
    int LoginPostId;
    inline void initinfo()
    {
        TeamList.clear();
        TeamIdList.clear();
        GangweiList.clear();
        GangWeiIdList.clear();
        ruleList.clear();
        ruleIDList.clear();
        TGList.clear();
    }
   inline void GetStationHeartInit(int StationId){
        Stationheart = new jsj_Stationheart(StationId);
    }
   inline void InitMulti(){
        if(Stationheart!=nullptr)
            Stationheart->multiNum = 0;
    }
   inline bool GetIsOffLine(){
        if(Stationheart!=nullptr){
            if(Stationheart->multiNum>2)
            {
                qDebug()<<"掉线计数:"<<Stationheart->multiNum;
                return true;
            }
            else
                return false;
        }
         return false;
    }
   inline bool isAppend(){
        return isConnected;
    }
};
struct UserloginInfo
{
    QString UserName;
    QString LoginUpTime;
    QString OffTime;
    int userLoginedTime;
    int Month;
  inline  void getTime(QString s1,QString s2)
    {
        QDateTime  OffTime = QDateTime::fromString(s2, "yyyy-MM-dd hh:mm:ss");
        QDateTime LoginUpTime =   QDateTime::fromString(s1, "yyyy-MM-dd hh:mm:ss");
        userLoginedTime = LoginUpTime.secsTo(OffTime);
        userLoginedTime = userLoginedTime/60;
    }
   inline void GetCurMonth(QString Starttime)
    {
        QDateTime LoginUTime =   QDateTime::fromString(Starttime, "yyyy-MM-dd hh:mm:ss");
        Month = LoginUTime.date().month();
    }
};
struct MySQLlogin
{
    unsigned int Serial_Baund;//串口波特率
    QString IPName = ""; //IP地址
    QString LoginName = ""; //登录账号
    QString LoginPass = ""; //登录密码
    QString LoginSQLName = ""; //数据库名称
    QString ZCPic_Path;//战场平面图路径
    QString LuoJiIP;
    quint16 LuoJiPort;
    QString LocalIP;
    quint16 LocalPort;

    QString EquipIP;
    QString Equip_Port;
    QString Equip_LoginName;
    QString Equip_Password;
    //MessageGet UserMessageGet;
    QString DingDingPath;
    int Dort = -1; //端口
    QString gangweiId;

    QString SuperAdminName;
    QString SuperAdminPassWord;

    IPInfo info;
   inline void InitInfo()
    {
        IPName = "";
        LoginName = "";
        LoginPass = "";
        LoginSQLName = "";
        Dort = 0;
    }
};
struct OperateInfo
{
    QString UserLoginaName;
    QString LoginPassWord;
    QString Post;
};
struct UserInfo
{
    int UserKeyId;
    QString UserName;
    QString Password;
    QString PhoneNumber;
    QString UserId;
    QString UserNumber;
    QString UserTeam;
    QString UserGangWei;

    QString UserStation;
    QString GzNumber;
    QString ruleName;


   inline void InitInfo()
    {
        UserKeyId = -1;
        UserName="";
        Password = "";
        PhoneNumber = "";
        UserId = "";
        UserNumber = "";
        UserTeam = "";
        UserGangWei = "";
        UserStation = "";
        GzNumber = "";
        ruleName = "";
    }

};
struct Dept_Info
{
    QString DeptName;
    int DeptId;
};
struct gangwei_Info
{
    QString GangWeiName;
    int GangweiId;
};
struct Group_Info
{
    QString GroupName;
    int GrouptId;
};
struct DDSendInfo
{
    QString UserName;
    QString UserGangwei;
    int UserId;
    QCheckBox *Box;
};
struct CarInfo_Detail{
    int index;
    QString Name;
};
struct CarInfo
{
    QString TrainTyep;
    QStringList Train_DetailList;
    QList<CarInfo_Detail>TrainList;
};
struct GD_{
    QString GDName;
    unsigned int GDIndex;
};
struct Jc_XHJ{
    QString JCName;
    unsigned int JCIndex;
};
struct Fc_XHJ{
    QString FCName;
    unsigned int FCIndex;
};
struct GDInfo
{
    QList<GD_>GDL;
    QList<Jc_XHJ>JCL;
    QList<Fc_XHJ>FCL;

    void initInfo(){
        GDL.clear();
        JCL.clear();
        FCL.clear();
    }

};
struct Config_PC_SoftInfo{
    unsigned char SoftCode;
    QString SoftAddress;
};
struct Config_PCInfo
{
    QString PcName;
    QString IPAddress;
    unsigned int Port;
    QList<Config_PC_SoftInfo>SoftList;
    inline void InitInfo(){
        IPAddress = "";
        Port = 0x00;
        SoftList.clear();
    }
   inline QList<Config_PC_SoftInfo> GetLocalNum(QString Str){
     QStringList SoftList_ = Str.split("|");
     for(int i = 0; i < SoftList_.size();++i)
     {
         QString Str = SoftList_[i];
         QStringList tempStr = Str.split("-");
         Config_PC_SoftInfo temp_Soft;
         temp_Soft.SoftCode = tempStr[0].toInt(NULL,16);
         temp_Soft.SoftAddress = tempStr[1];
         SoftList.push_back(temp_Soft);
     }
     return SoftList;
    }
};
struct JHINFO
{
    int CurIndex = -1;
    QString JC_Str;
    QString FC_Str;
    unsigned int jfc_Type;
    QString jh_PlanNumber;
    QString jh_TargetStationName;
    QString jh_ArriveCheci;
    QString jh_ChufaCheci;
    QString jh_type;
    QString jh_ArriveGD;
    unsigned int JC_Index;
    unsigned int GD_Index;
    QString jh_ArriveTime;
    QDate ArrDate;
    QTime ArrT;
    QDate ChuFDate;
    QTime ChuT;
    int Arrivetime[6];
    int ChuFaTime[6];
    QString jh_ChufaGD;
    unsigned int Fc_Index;
    QString jh_ChuFaTime;
    QString jh_QyType;
    unsigned int qytype;
    QString jh_ChaoXInfo;
    unsigned int cxtype;
    QString jh_TrainType;
    unsigned int GetKind;//货车还是客车
    unsigned int TrainType;//列车类型
    unsigned int YunxinType;//运行类型
    unsigned int StationId;//站Id
    QString jh_YxType;
};
class JSJ_Services : public QObject
{
    Q_OBJECT
public:
    explicit JSJ_Services(QObject *parent = nullptr);
private:
    QString logFile;
    int lastID;
    QDateTime startTime;
public:
    void writeTxt(QString PATH,QString Content);
    QString GetTimeLong(QDateTime time1,QDateTime time2);
    QHostAddress GetLocalAddress();
    QStringList GetGD_ByStationName(QString StationName);
    void SetBackImage(QWidget*W, QString Name);
    static JSJ_Services &getDBService();
    void OpenUp();
    void loadDBConfig();
    void ReadSelfStartConfig();
    QString GetConnect_toDB();
    void LoadConfigExcel(QString Path);
    void UpdateInfo();
    bool isExist(QString Type);
    void GetNewAppendtoList(QStringList varlist,int index);
    void AppendToList(QStringList VarList,int index);
    void GetTrainTypeString(QString Name);
    void GetConfigInfo(QString Name);
    void GetddInfoList(QString FileName);
    void InitTable(QStringList strHeader,QTableWidget *wi);
    void Init(QTableView *view, QStandardItemModel * Model, QStringList str, bool isfine);
    QString  GetStationId(QString StationName);
    int isExistCombo(QComboBox*box, int id);
    int isExistTableWidget(QTableWidget * table,QString StationName);
public:
    QString GetSoftNameByCode(unsigned char SoftCode);
    QWidget *GetCellWidget(QString Name,QWidget *Parent,QString Address);
    int generateRandomNumber();
    int getSendInfoByNumber(QString ML_Number);
    bool GetSame_MlNumberIsExist(QString MLNumber,QString Type);
    void reFreshAllStatus(QString DDNumber,unsigned int type);
    void GetDdListBytime();
    void SetSpan();
    void ClearContent(QTableWidget *tableWidget);
    void SelectUserLoginInfoById(int UserId);
    void GetUserInfo();
    void GetDeptList();
    void GetGroupListByStationId(int Stationid);
    void GetgangweiList();
    bool AdddNewOne(UserInfo &info);
    int GetUserExist(QString IdNumber,QString WorkNumber);
    int GetUserIdByUserWorkId(QString UserWorkId);
    bool UpdateUserInfo(QString UserId);
    void AddSuperAdmin(UserInfo&info);
    bool DeleUserInfoByNumber(QString Number);
    int GetIdByStationName(QString StationName);
    int GetIdByPostName(QString PostName);
    int GetIdByRoleName(QString RoleName);
    int GetIdByGroupName(QString GroupName,int StationId);
    UserInfo &GetInfoByKeyId(QString KeyId);
    void GetOperateInfoByloginInfo(QString LoginName,QString LoginPassword);

public:
    QDateTime StartTime;
    QDateTime EndTime;
    QList<Config_PCInfo>PCInfo_List;
    ControlInfo tempControl;
    DDML GetDDML;
    OperateInfo OperateGet;
    UserInfo EditInfo;
    QList<CarInfo>CarDetailList;
    QList<DDINFO>DDINFOlist;
    QList<Dept_Info>DeptList;
    QList<DDSendInfo>DDSendList;
    QList<gangwei_Info>gangweiList;
    QList<Group_Info>GroupList;
    QList<UserInfo>InfoList;
    QList<UserInfo>UpDateList;
    QList<UserloginInfo>UserLoginedInfo;
    QList<BackInfo> CurBackInfoList;
    QList<JHINFO>JHList;
    QList<DDML>SavedList;
    MySQLlogin LoginSQLData;
    GDInfo info;
    //QStringList GDList;//多站阶段计划股道信息
    QList<jsj_planNode*>PlanNodeList;
signals:
    void Send_DD_Info(DDML ml,unsigned int kind,bool isGetOne);
    void Save_Info(unsigned int Kind,unsigned int type);
    void ClearAllWidgets(int type);
    void UpdateTableShow();
    void ShowMain(bool);
    void ShowMainAnotherTwo();
    void GetGDInfo(QString StationName);
    void UpdateMultiPlan(unsigned int Operate);
    void UpdateStation();
};

#define DBSERVICES (JSJ_Services::getDBService())
#endif // JSJ_SERVICES_H
