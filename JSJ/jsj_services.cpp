#include "jsj_services.h"
#include<QCoreApplication>
#include<QWidget>
#include<QDebug>
#include<QSettings>
JSJ_Services &JSJ_Services::getDBService()
{
    static JSJ_Services s_DBService;
    return  s_DBService;
}

void JSJ_Services::OpenUp()
{

}
#include<QHeaderView>
#include<QStandardItemModel>
#include<QScrollBar>
void JSJ_Services::InitTable(QStringList strHeader,QTableWidget *wi)
{

    QHeaderView* view = new QHeaderView(Qt::Horizontal);
    QStandardItemModel* model = new QStandardItemModel;

    model->setHorizontalHeaderLabels(strHeader);
    view->setModel(model);
    view->setStyleSheet("QHeaderView::section{"
                        "background-color:#009cff;font:17px '微软雅黑';"
                        "color:#ffffff;}");
    wi->setColumnCount(strHeader.count());
    wi->setHorizontalHeader(view);
    wi->horizontalHeader()->setMinimumHeight(30);
    wi->horizontalHeader()->setMinimumWidth(100);
    wi->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    wi->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑
    wi->horizontalHeader()->setStretchLastSection(true); //行头自适应表格
    wi->verticalHeader()->setHidden(true);
    wi->verticalScrollBar()->show();
    wi->horizontalHeader()->setHighlightSections(false);//处理塌陷
}
int JSJ_Services::getSendInfoByNumber(QString ML_Number)
{
    for(int i = 0; i <SavedList.size();++i)
    {
        if(SavedList[i].ML_Number == ML_Number)
            return i;
        else
            continue;
    }
    return -1;
}
int JSJ_Services::generateRandomNumber()
{
    qsrand(QTime(0,0,0,1).secsTo(QTime::currentTime()));
    for(int i=0; i<3; i++)
    {
        int test =qrand();
        return test;
    }
    return 0;
}
bool JSJ_Services::GetSame_MlNumberIsExist(QString MLNumber,QString Type)
{
    for(int i = 0; i < SavedList.size();++i)
    {
        if(SavedList[i].ML_Number == MLNumber && SavedList[i].ML_T == Type)
        {
            return false;
        }
    }
    return true;
}
void JSJ_Services::reFreshAllStatus(QString DDNumber, unsigned int type)
{
    for(int i = 0; i < SavedList.size();++i)
    {
        if(SavedList[i].ML_Number == DDNumber && SavedList[i].Status!=type)
        {
            SavedList[i].Status = type;
            break;
        }
    }
}
#include<windows.h>
#include<QMessageBox>
void setDate(QDateTime localTime2)
{
    SYSTEMTIME st;
    GetSystemTime(&st);    // Win32 API 获取系统当前时间，并存入结构体st中
    st.wYear=localTime2.date().year();
    st.wMonth=localTime2.date().month();
    st.wDay=localTime2.date().day();

    st.wHour=localTime2.time().hour();
    st.wMinute=localTime2.time().minute();
    st.wSecond=localTime2.time().second();
    st.wMilliseconds=0;

    int s = SetLocalTime(&st);    //Win32 API 设置系统时间
    qDebug()<<"Success："<<s;
}
#include<QTextCodec>
void JSJ_Services::loadDBConfig()
{
    QString path = QString("%1/data/IP配置.ini").arg(QCoreApplication::applicationDirPath());
    QSettings  iniFile(path, QSettings::IniFormat);
    iniFile.setIniCodec(QTextCodec::codecForName("utf-8"));
    iniFile.beginGroup(("DB"));
    LoginSQLData.IPName = iniFile.value(("HostName"), "").toString();
    LoginSQLData.LoginName = iniFile.value(("UserName"), "").toString();
    LoginSQLData.LoginPass = iniFile.value("PassWord", "").toString();
    LoginSQLData.LoginSQLName = iniFile.value("DataBase", "").toString();
    LoginSQLData.Dort = iniFile.value("Dort", "").toInt();
    LoginSQLData.info.LocalUdpIP = iniFile.value("COMMONIP", "").toString();
    iniFile.endGroup();

    iniFile.beginGroup("TCCIP");
    //LoginSQLData.info.m_LocalIP.setAddress(iniFile.value(("IP_LocalAdress"), "").toString());
    //LoginSQLData.info.m_OppIP.setAddress(iniFile.value(("IP_OppAdress"), "").toString());
    LoginSQLData.info.m_LocalPort = (iniFile.value(("CTCport"), "").toString().toUShort());
    //LoginSQLData.info.m_OppPort = (iniFile.value(("IP_OppPort"), "").toString().toUShort());
    iniFile.endGroup();

    iniFile.beginGroup("LSUDP");
    //LoginSQLData.info.LocalUdpIP = GetLocalAddress().toString();//自动获取LSIP
    LoginSQLData.info.LocalUdpPort = iniFile.value("Port", "").toInt();
    LoginSQLData.info.LocalUdpTrainPort = iniFile.value("TrainPort", "").toInt();
    iniFile.endGroup();

    iniFile.beginGroup("TIME");
    QString Time = iniFile.value("SETTIME", "").toString();
    iniFile.endGroup();

    iniFile.beginGroup("SCROLLSIZE");
    LoginSQLData.info.ScrollSize_Width = iniFile.value("SIZE_WIDTH", "").toInt();
    LoginSQLData.info.ScrollSize_Height = iniFile.value("SIZE_HEIGHT", "").toInt();
    iniFile.endGroup();
    if(Time!="")
    {
        LoginSQLData.info.Time = QDateTime::fromString(Time,"yyyy-MM-dd hh:mm:ss");
        setDate(LoginSQLData.info.Time);
    }
    GetConfigInfo("stationconfig");

}

#include<QHostInfo>
QHostAddress JSJ_Services::GetLocalAddress()
{
    QHostAddress addr;
    //通过遍历获取到本机ip
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(QHostAddress address,info.addresses())
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)
        {
            qDebug()<<address.toString(); //输出IPV4的地址
            addr=address;
            return addr;
        }
    }
}
QString JSJ_Services::GetSoftNameByCode(unsigned char SoftCode)
{
    if(SoftCode == 0x01)
        return "联锁";
    else if (SoftCode == 0x02)
        return "CTC";
    else if (SoftCode == 0x03)
        return "培训软件";
    else if (SoftCode == 0x04)
        return "数字调度台";
    else if (SoftCode == 0x05)
        return "电子簿册";
    else if (SoftCode == 0x06)
        return "区间软件";
    return "";
}
void JSJ_Services::ReadSelfStartConfig()
{
    QString Path = QString("%1/data/tccstartconfig.ini").arg(QCoreApplication::applicationDirPath());
    QSettings  iniFile(Path, QSettings::IniFormat);
    iniFile.setIniCodec("UTF-8");
    int menueNum = 0;
    iniFile.beginGroup("CONNECTADDRESS");
    menueNum = iniFile.value(("num"), "").toInt();
    iniFile.endGroup();
    Config_PCInfo pcSoft;
    for(int i = 0; i < menueNum;++i)
    {
        pcSoft.InitInfo();
        QString str = QString("CONNECT%1").arg(i);
        iniFile.beginGroup(str);
        pcSoft.PcName = iniFile.value(QString::fromLocal8Bit("PCNAME"), "").toString();
        pcSoft.IPAddress = iniFile.value(QString::fromLocal8Bit("IPADDRESS"), "").toString();
        pcSoft.Port = iniFile.value(QString::fromLocal8Bit("PORT"), "").toUInt();
        pcSoft.SoftList = pcSoft.GetLocalNum( iniFile.value(QString::fromLocal8Bit("ITEM"),"").toString());
        PCInfo_List.push_back(pcSoft);
        iniFile.endGroup();
    }
}

#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlError>
QString JSJ_Services::GetConnect_toDB()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(("QMYSQL"));
    if(LoginSQLData.IPName == "")
        return "检查数据库IP";
    db.setHostName(LoginSQLData.IPName);
    if(LoginSQLData.Dort ==-1)
        return "检查数据库端口";
    db.setPort(LoginSQLData.Dort);
    if(LoginSQLData.LoginSQLName =="")
        return "检查数据库名称";
    db.setDatabaseName(LoginSQLData.LoginSQLName);
    if(LoginSQLData.LoginName =="")
        return "检查数据库登录名称";
    db.setUserName(LoginSQLData.LoginName);
    if(LoginSQLData.LoginPass =="")
        return "检查数据库登录密码";
    db.setPassword(LoginSQLData.LoginPass);
    if (!db.open())
    {
        return db.lastError().text();
    }
    else
    {
        return  "";
    }
}
#include<QSqlQuery>
#include<QFile>
#include<QCoreApplication>
#include<../libxl.h>
void JSJ_Services::LoadConfigExcel(QString Path)
{

    UpDateList.clear();
    if(Path == "")
        return;
    QFile oFile(Path);
    if(!oFile.exists())
    {
        return;
    }
    libxl::Book *book = nullptr;
    if(Path.endsWith("xls",Qt::CaseSensitive)){
        book = xlCreateBook();
    }
    else if(Path.endsWith("xlsx",Qt::CaseInsensitive)){
        book = xlCreateXMLBook();
    }  else
    {
        return;
    }
    book->setKey(L"GCCG", L"windows-282123090cc0e6036db16b60a1o3p0h9");
    qDebug()<<Path.utf16();
    bool f = book->load( reinterpret_cast<const wchar_t *>(Path.utf16()));
    if(!f)
        return;
    libxl::Sheet * sheetread = book->getSheet(0);
    if(sheetread)
    {
        for(int i = 2; i < sheetread->lastRow();++i)
        {
            UserInfo info;
            info.UserName = QString::fromWCharArray(sheetread->readStr(i, 1));
            info.UserNumber = QString::fromWCharArray(sheetread->readStr(i, 2));
            info.UserTeam = QString::fromWCharArray(sheetread->readStr(i, 3));
            info.UserGangWei = QString::fromWCharArray(sheetread->readStr(i, 4));
            info.UserStation = QString::fromWCharArray(sheetread->readStr(i, 5));
            info.ruleName = QString::fromWCharArray(sheetread->readStr(i, 6));
            AdddNewOne(info);
            UpDateList.push_back(info);
        }
    }
}
void JSJ_Services::Init(QTableView *view, QStandardItemModel * Model, QStringList str, bool isfine)
{
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //Model->setRowCount(200);
    view->setContextMenuPolicy(Qt::CustomContextMenu);
    Model->setHorizontalHeaderLabels(str);
    //设置列数
    Model->setColumnCount(str.size());
    //隐藏列表头
    view->verticalHeader()->hide();
    if (isfine)
    {
        Model->setHorizontalHeaderLabels(str);
        //设置列数
        Model->setColumnCount(str.size());
        //设置行数
        // Model->setRowCount(200);
        //隐藏列表头
        view->verticalHeader()->hide();
        //DBSERVICE.setTableViewConfig(view);

    }
    view->horizontalHeader()->setHighlightSections(false);
    //view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    QString strHeaderQss;
    strHeaderQss = ("QHeaderView::section { background:green; color:white;min-height:3em;}");
    view->setModel(Model);

}

QString JSJ_Services::GetStationId(QString StationName)
{
    for(int i = 0 ; i < CurBackInfoList.size();++i)
    {
        if(CurBackInfoList[i].StationName == StationName)
        {
            int id = CurBackInfoList[i].StationId;
            if(id == 0)
                return "Station";
            else
            {
                QString StationName = QString("Station%1").arg(id);
                return StationName;
            }

        }
    }
    return "";
}

int JSJ_Services::isExistCombo(QComboBox *box, int id)
{
    if(box!=nullptr)
    {
        for(int i = 0 ; i < box->count();++i)
        {
            if(box->itemData(i).toInt() == id)
                return i;
            else
                continue;
        }
    }
    return -1;
}

int JSJ_Services::isExistTableWidget(QTableWidget *table, QString StationName)
{
    if(table!=nullptr)
    {
        int rowCout = table->rowCount();
        for(int i = 0 ; i < rowCout;++i)
        {
            //qDebug()<<table->item(i,1)->text();
            if(table->item(i,1)->text() == StationName)
            {
                return i;
            }else
                continue;
        }
        return -1;
    }
    return -1;
}
///-----------------------------------------------------------------------------
/** *Brief   :  补齐填充函数
    *Function:  slotAppectTask
    *Param   :  [in] QString str    --  需补齐字符串
    *        :  [in] int maxLen     --  需对齐的长度
    *        :  [in] QChar c        --  填充的字符
    *Retval  :  补齐后的字符串
-----------------------------------------------------------------------------  */
QString FillIn (QString str,int maxLen, QChar c)
{
    int len = str.length();
    if(len < maxLen)
    {
        for(int i =0 ; i < maxLen - len; i++)
            str +=  c;
    }
    return str;
};
void JSJ_Services::UpdateInfo()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = "";
    sql = "truncate sys_dept";
    query.prepare(sql);
    if(query.exec())
    {
        db.commit();
    }
    sql = "truncate t_dept_group";
    query.prepare(sql);
    if(query.exec())
        db.commit();
    sql = "truncate sys_post";
    query.prepare(sql);
    if(query.exec())
        db.commit();
    sql = "truncate sys_role";
    query.prepare(sql);
    if(query.exec())
        db.commit();
    for(int i = 0; i  < CurBackInfoList.size();++i)
    {
        sql = "insert into sys_dept (dept_code, dept_name) "
              "values (:id,:name)";
        query.prepare(sql);
        query.bindValue(":id",CurBackInfoList[i].StationId);
        query.bindValue(":name",CurBackInfoList[i].StationName);
        if(query.exec())
        {
            db.commit();
        }else
            db.rollback();
        for(int ii = 0; ii < CurBackInfoList[i].TeamList.size();++ii)
        {
            sql = "insert into t_dept_group (station_id,group_no,group_name) "
                  "values (:s_id,:g_no,:g_name)";
            query.prepare(sql);
            query.bindValue(":s_id",CurBackInfoList[i].StationId);
            query.bindValue(":g_no",CurBackInfoList[i].TeamIdList[ii]);
            query.bindValue(":g_name",CurBackInfoList[i].TeamList[ii]);
            if(query.exec())
                db.commit();
            else
                db.rollback();
        }
        for(int z = 0; z < CurBackInfoList[i].GangweiList.size();++z)
        {
            sql = "insert into sys_post (post_name) "
                  "values (:p_name)";
            query.prepare(sql);
            query.bindValue(":p_name",CurBackInfoList[i].GangweiList[z]);
            if(query.exec())
            {
                db.commit();
            }else
            {
                qDebug()<<query.lastError();
                db.rollback();
            }
        }
        for(int d = 0; d <  CurBackInfoList[i].ruleList.size();++d)
        {
            sql = "insert into sys_role (role_name) "
                  "values (:role_name)";
            query.prepare(sql);
            query.bindValue(":role_name", CurBackInfoList[i].ruleList[d]);
            if(query.exec())
                db.commit();
            else
            {
                qDebug()<<query.lastError();
                db.rollback();
            }
        }
    }
}
#include<QWidget>
QWidget *JSJ_Services::GetCellWidget(QString Name, QWidget *Parent, QString Address)
{
    QCheckBox * pCheckBox = nullptr;
    QWidget * pBoxWidget = nullptr;
    QHBoxLayout * hBox = nullptr;
    pCheckBox = new QCheckBox(Name);
    pBoxWidget = new QWidget(Parent);
    hBox = new QHBoxLayout();
    pCheckBox->setStyleSheet("QCheckBox::indicator{width:30px;height:30px}");
    hBox->addWidget(pCheckBox);
    pCheckBox->setChecked(false);
    pCheckBox->setProperty("Address",Address);
    hBox->setMargin(0);
    hBox->setAlignment(pCheckBox,Qt::AlignCenter);
    pBoxWidget->setLayout(hBox);
    return pBoxWidget;
}
bool JSJ_Services::isExist(QString Type)
{
    for(int i = 0; i < CarDetailList.size();++i)
    {
        if(CarDetailList[i].TrainTyep == Type)
            return false;
    }
    return true;
}

void JSJ_Services::GetNewAppendtoList(QStringList varlist, int index)
{
    for(int i = 0; i < CarDetailList.size();++i)
    {
        if(CarDetailList[i].TrainTyep == varlist[0])
        {
            CarInfo_Detail detail;
            detail.Name = varlist[1];
            detail.index = index;
            CarDetailList[i].TrainList.push_back(detail);
            return;
        }
    }
}

void JSJ_Services::AppendToList(QStringList VarList, int index)
{
    CarInfo info;
    info.TrainTyep = VarList[0];
    CarInfo_Detail det;
    det.index = index;
    det.Name = VarList[1];
    info.TrainList.push_back(det);
    CarDetailList.push_back(info);
}
void JSJ_Services::GetTrainTypeString(QString Name)
{
    QString path = QString::fromLocal8Bit("%1/data/%2.txt")
            .arg(QCoreApplication::applicationDirPath())
            .arg(Name);
    QFile oFile(path);
    QString line;
    CarInfo tempCar;
    if(oFile.open(QFile::ReadWrite))
    {
        QTextStream in(&oFile);
        for (int i = 0;; i++)
        {
            line  = in.readLine();
            if (line == ("####"))
                break;
            else
            {
                if(line.contains("|"))
                {
                    QStringList VarList = line.split("|");
                    if(isExist(VarList[0]))
                    {
                        AppendToList(VarList,i);
                    }else
                    {
                        GetNewAppendtoList(VarList,i);
                    }
                }
                continue;
            }
        }
    }
}
void JSJ_Services::writeTxt(QString PATH,QString Content)
{
    QString strAll;
    QStringList strList;
    QFile readFile(PATH);		//PATH是自定义读取文件的地址
    if(readFile.open((QIODevice::ReadOnly|QIODevice::Text)))
    {
        //把文件所有信息读出来
        QTextStream stream(&readFile);
        strAll=stream.readAll();
    }
    readFile.close();

    QFile writeFile(PATH);	//PATH是自定义写文件的地址
    if(writeFile.open(QIODevice::WriteOnly|QIODevice::Append))
    {
        QTextStream stream(&writeFile);
        strList=strAll.split("\n");           //以换行符为基准分割文本
        for(int i=0;i<Content.count();i++)    //遍历每一行
        {
            {
                if(i==Content.count()-1)
                {
                    stream<<Content.at(i);
                }
                else
                {
                    stream<<Content.at(i);
                }
            }
        }
    }
    writeFile.close();
}

QString JSJ_Services::GetTimeLong(QDateTime time1, QDateTime time2)
{
    qDebug()<<"开始时间:"<<time1;
    qDebug()<<"结束时间"<<time2;


    qint64 lasttime =time1.secsTo(time2); //初始化数据，时 分 秒 毫秒

    QTime time(0, 0); // 创建一个初始时间对象，设置小时和分钟为0
    // 使用addSecs()函数将秒数添加到时间对象中
    time = time.addSecs(lasttime);
    QString formattedTime = time.toString("hh:mm:ss"); // 将时间对象转换为字符串，指定格式为时:分:秒
    qDebug() << "Formatted Time:" << formattedTime;
    QString End = QString("%1小时   %2分钟   %3秒")
            .arg( QString::number( time.hour()))
            .arg(QString::number(time.minute()))
            .arg(QString::number(time.second()));
    return End;
}


void JSJ_Services::GetConfigInfo(QString Name)
{
    QString path = QString::fromLocal8Bit("%1/data/%2.txt").arg(QCoreApplication::applicationDirPath())
            .arg(Name);
    QFile oFile(path);
    QString line;
    BackInfo info;
    if (oFile.open(QFile::ReadWrite))
    {
        QTextStream in(&oFile);
        for (int i = 0;; i++)
        {

            line   = in.readLine();
            if (line == ("END"))
                break;
            else if (line == ("##"))
            {
                CurBackInfoList.push_back(info);
                info.initinfo();
            }
            else
            {
                if (line.contains(("STATIONS")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.StationName = line.mid(start, end - start);
                }
                else if (line.contains(("STATIONID")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.StationId = line.mid(start, end - start).toInt();
                }
                else if (line.contains(("TEAMNAME")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.TeamList = line.mid(start, end - start).split("|");
                }
                else if (line.contains(("TEAMID")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.TeamIdList = line.mid(start, end - start).split("|");
                }
                else if (line.contains(("GANGWEIS")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.GangweiList = line.mid(start, end - start).split("|");
                }
                else if (line.contains(("GANGWEIID")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.GangWeiIdList = line.mid(start, end - start).split("|");
                }
                else if (line.contains(("RULES")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.ruleList = line.mid(start, end - start).split("|");
                }
                else if (line.contains(("RULEID")))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    info.ruleIDList = line.mid(start, end - start).split("|");
                }
                else if (line.contains("LOGINNAME"))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    LoginSQLData.SuperAdminName = line.mid(start, end - start);
                }
                else if (line.contains("LOGINPASSWORD"))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    LoginSQLData.SuperAdminPassWord = line.mid(start, end - start);
                }
                else if (line.contains("LOGINPOSTID"))
                {
                    int start = line.indexOf('=', 0) + 1;
                    int end = line.indexOf(';', start);
                    LoginSQLData.gangweiId = line.mid(start, end - start);
                }
                else if (line.contains("##Num"))
                {
                    int start = line.indexOf("=",0) + 1;
                    int end = line.indexOf(';',start);
                    int num = line.mid(start, end - start).toInt();
                    for(int i = 0 ; i<num;++i)
                    {
                        TGInfo tempTG;
                        line = in.readLine();
                        QStringList TGl = line.split("|");
                        tempTG.JC_Info = TGl[0];
                        tempTG.GD_Info = TGl[1];
                        tempTG.FC_Info = TGl[2];
                        info.TGList.push_back(tempTG);
                    }
                }
            }

        }
    }
}

void JSJ_Services::GetddInfoList(QString FileName)
{
    QString path = QString::fromLocal8Bit("%1/data/%2.txt")
            .arg(QCoreApplication::applicationDirPath())
            .arg(FileName);
    QFile oFile(path);
    QString line;
    BackInfo info;
    DDINFO tempInfo;
    if (oFile.open(QFile::ReadWrite))
    {
        QString LineRead = oFile.readLine();
        if(LineRead.contains("Start"))
        {
            for(;;)
            {
                LineRead = oFile.readLine();
                if(LineRead.contains("END"))
                    return;

                if(LineRead.contains("##TITLE"))
                {
                    LineRead = oFile.readLine();
                    tempInfo.DDMLCommond = LineRead;
                    tempInfo.DDMLCommond.remove("\r\n");
                }
                else if (LineRead.contains("##CONTENT"))
                {
                    for(;;)
                    {
                        LineRead = oFile.readLine();
                        if(LineRead.contains("##"))
                            break;
                        else
                        {
                            tempInfo.Content =tempInfo.Content.append(LineRead);
                        }
                    }

                    DDINFOlist.push_back(tempInfo);
                    tempInfo.initinfo();
                }
            }
        }
    }
}


void JSJ_Services::GetDdListBytime()
{
    DDSendList.clear();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = QString("SELECT "
                          "a.group_no as id,"
                          "a.group_name as name "
                          "FROM "
                          "t_dept_group a "
                          "where"
                          " a.station_id ='%1'").arg("1");
    qDebug()<<sql;
    query.prepare(sql);
    if(query.exec()){
        while (query.next()) {
            DDSendInfo info;
            info.UserId = query.value("id").toInt();
            info.UserName = query.value("name").toString();
            DDSendList.push_back(info);
        }
    }
}
#include<QDateTime>
void JSJ_Services::SelectUserLoginInfoById(int UserId)
{
    UserLoginedInfo.clear();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = QString("SELECT "
                          "a.online_Id as id,"
                          "a.UpLineTime as uptime,"
                          "a.offlineTime as offtime,"
                          "a.expire_time as expire "
                          "from sys_userlongtime a "
                          "where user_id = '%1'").arg(UserId);
    query.prepare(sql);
    qDebug()<<sql;
    if(query.exec())
    {
        while (query.next()) {
            UserloginInfo logininfo;
            logininfo.LoginUpTime = (query.value("uptime").toString());
            logininfo.OffTime = (query.value("offtime").toString());
            logininfo.getTime(logininfo.LoginUpTime,logininfo.OffTime);
            logininfo.GetCurMonth(logininfo.LoginUpTime);
            UserLoginedInfo.push_back(logininfo);
        }
    }
    qDebug()<<query.lastError();

}

void JSJ_Services::ClearContent(QTableWidget *tableWidget)
{
    int num_row = tableWidget->rowCount();
    for(int i = 0; i<num_row ; i++)
    {
        //每次都清空第0行，清空row次
        tableWidget->removeRow(0);
    }
}
void JSJ_Services::GetUserInfo()
{

    InfoList.clear();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString Sql ="SELECT"
                 " a.user_id as id,"
                 " a.user_name as Name,"
                 "a.login_name as Login_Name,"
                 "a.password as pass,"
                 "a.work_Number as wnumber,"
                 "b.dept_name as Dept_Name,"
                 "c.group_name as Group_Name,"
                 "d.post_name as Post_Name,"
                 "e.role_name as role_name "
                 "FROM sys_user a "
                 "LEFT JOIN sys_dept b ON a.dept_id = b.dept_id "
                 "LEFT JOIN t_dept_group c ON a.group_id = c.group_no "
                 "LEFT JOIN sys_post d ON a.post_id = d.post_id "
                 "LEFT JOIN sys_role e ON a.role_id = e.role_id";
    query.prepare(Sql);
    if(query.exec())
    {

        while (query.next()) {
            UserInfo info;
            info.UserKeyId = query.value("id").toInt();
            info.UserName = query.value("Name").toString();
            info.UserNumber = query.value("Login_Name").toString();
            info.UserStation = query.value("dept_name").toString();
            info.UserTeam = query.value("Group_Name").toString();
            info.UserGangWei = query.value("Post_Name").toString();
            info.Password = query.value("pass").toString();
            info.GzNumber = query.value("wnumber").toString();
            info.ruleName = query.value("role_name").toString();
            InfoList.push_back(info);
        }
    }else
        qDebug()<<query.lastError();
}

void JSJ_Services::GetDeptList()
{
    DeptList.clear();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select * from sys_dept"));
    query.prepare(sql);
    if(query.exec())
    {
        while (query.next()) {
            Dept_Info info;
            info.DeptId = query.value("dept_id").toInt();
            info.DeptName = query.value("dept_name").toString();
            DeptList.push_back(info);
        }
    }
}

void JSJ_Services::GetGroupListByStationId(int Stationid)
{
    GroupList.clear();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select * from t_dept_group where station_id = '%1'").arg(Stationid));
    query.prepare(sql);
    if(query.exec())
    {
        while (query.next()) {
            Group_Info info;
            info.GrouptId = query.value("group_name").toInt();
            info.GroupName = query.value("group_name").toString();
            GroupList.push_back(info);
        }
    }
}
void JSJ_Services::SetSpan()
{
    QTime dieTime = QTime::currentTime().addMSecs(100);
    while( QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void JSJ_Services::GetgangweiList()
{
    gangweiList.clear();
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select * from sys_post"));
    query.prepare(sql);
    if(query.exec())
    {
        while (query.next()) {
            gangwei_Info info;
            info.GangweiId = query.value("post_id").toInt();
            info.GangWeiName = query.value("post_name").toString();
            gangweiList.push_back(info);
        }
    }
}
int JSJ_Services::GetIdByStationName(QString StationName)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select dept_id from sys_dept where dept_name = '%1'").arg(StationName));
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next()){
            return query.value("dept_id").toInt();
        }
    }
    return -1;

}

int JSJ_Services::GetIdByPostName(QString PostName)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select post_id from sys_post where post_name = '%1'").arg(PostName));
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next()){
            return query.value("post_id").toInt();
        }
    }
    return -1;
}

int JSJ_Services::GetIdByRoleName(QString RoleName)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select role_id from sys_role where role_name = '%1'").arg(RoleName));
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next()){
            return query.value("role_id").toInt();
        }
    }
    return -1;
}

int JSJ_Services::GetIdByGroupName(QString GroupName, int StationId)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql =(QString("select group_no from t_dept_group where group_name = '%1'"
                          " and station_id = '%2'").arg(GroupName).arg(StationId));
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next()){
            return query.value("group_no").toInt();
        }
    }
    return -1;
}

UserInfo &JSJ_Services::GetInfoByKeyId(QString KeyId)
{
    for(int i = 0; i < InfoList.size();++i)
    {
        if(InfoList[i].UserNumber == KeyId)
            return InfoList[i];
    }
}
void JSJ_Services::GetOperateInfoByloginInfo(QString LoginName, QString LoginPassword)
{

    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString Sql =QString("SELECT "
                         "role_name "
                         "from"
                         " sys_role AS B "
                         "WHERE "
                         "B.role_id = (select role_id FROM sys_user WHERE login_name = '%1' and password = '%2')")
            .arg(LoginName).arg(LoginPassword);
    qDebug()<<Sql;
    query.prepare(Sql);
    if(query.exec())
    {
        if (query.next()) {
            OperateGet.Post = query.value("role_name").toString();
        }
    }else
        qDebug()<<db.lastError();
}

bool JSJ_Services::AdddNewOne(UserInfo&info)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = ("INSERT INTO sys_user (user_name, login_name,dept_id, group_id, post_id,role_id,"
                   "password,work_Number)"
                   "VALUES (:ame,:login_name,:Dept_id, :group_id, :Post_id,:rule_id,:password,:work_Number)");
    query.prepare(sql);
    query.bindValue(":ame",info.UserName);
    query.bindValue(":login_name",info.UserNumber);
    query.bindValue(":Dept_id",GetIdByStationName(info.UserStation));
    query.bindValue(":group_id",GetIdByGroupName(info.UserTeam,GetIdByStationName(info.UserStation)));
    query.bindValue(":Post_id",GetIdByPostName(info.UserGangWei));
    query.bindValue(":rule_id",GetIdByRoleName(info.ruleName));
    query.bindValue(":password",info.Password);
    query.bindValue(":work_Number",info.GzNumber);
    if(query.exec())
    {
        db.commit();
        return true;
    }

    else
    {
        qDebug()<<db.lastError();
        db.rollback();
        return false;
    }

}

int JSJ_Services::GetUserExist(QString IdNumber, QString WorkNumber)
{
    QString Name = "";QString WorkNum = "";
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = QString("select user_name from sys_user where login_name = '%1'").arg(IdNumber);
    query.prepare(sql);
    if(query.exec())
    {
        if(query.next())
        {
            Name = query.value("user_name").toString();
        }else
        {
            sql =  QString("select user_name from sys_user where work_Number = '%1'").arg(WorkNumber);
            query.prepare(sql);
            if(query.exec())
            {
                if(query.next())
                {
                    WorkNum = query.value("user_name").toString();
                }
            }
        }
        if(Name == "" && WorkNum == "")
        {
            return 1;
        }
        else if (Name!="")
            return -1;
        else if (WorkNum!="")
            return -2;
    }
}

int JSJ_Services::GetUserIdByUserWorkId(QString UserWorkId)
{
    for(int i = 0; i < InfoList.size();++i)
    {
        if(InfoList[i].UserNumber == UserWorkId)
            return InfoList[i].UserKeyId;
    }
    return -1;
}

bool JSJ_Services::UpdateUserInfo(QString UserId)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = QString("UPDATE sys_user SET user_name='%1',login_name='%2',"
                          "role_id = '%3',dept_id='%4',group_id='%5',post_id='%6',"
                          "password='%7',work_Number='%8'"
                          " where login_name ='%9'").arg(EditInfo.UserName)
            .arg(EditInfo.UserNumber).arg(GetIdByRoleName(EditInfo.ruleName))
            .arg(GetIdByStationName(EditInfo.UserStation))
            .arg(GetIdByGroupName(EditInfo.UserTeam,GetIdByStationName(EditInfo.UserStation)))
            .arg(GetIdByPostName(EditInfo.UserGangWei))
            .arg(EditInfo.Password).arg(EditInfo.GzNumber).arg(UserId);
    query.prepare(sql);
    if(query.exec())
    {
        db.commit();
        return true;
    }else
    {
        qDebug()<<db.lastError();
        db.rollback();
        return false;
    }
}

void JSJ_Services::AddSuperAdmin(UserInfo &info)
{
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = QString("Delete from sys_user where role_id = 1");
    query.prepare(sql);
    if(query.exec())
    {
        sql = ("INSERT INTO sys_user (user_name, login_name,role_id, password)"
               "VALUES (:ame,:login_name,:role_id, :password)");
        query.prepare(sql);
        query.bindValue(":ame",info.UserName);
        query.bindValue(":login_name",info.UserNumber);
        query.bindValue(":role_id",GetIdByRoleName(info.UserGangWei));
        query.bindValue(":password",info.Password);
        if(query.exec())
            db.commit();
        else
        {
            qDebug()<<db.lastError();
            db.rollback();
        }
    }

}

bool JSJ_Services::DeleUserInfoByNumber(QString Number)
{
    if(Number<0)
        return false;
    QSqlDatabase db = QSqlDatabase::database();
    db.transaction();
    QSqlQuery query(db);
    QString sql = QString("delete from sys_user "
                          "where login_name = '%1'").arg((Number));
    qDebug()<<sql;
    query.prepare(sql);
    if(query.exec())
    {
        db.commit();
        return true;
    }else
        return false;
}

JSJ_Services::JSJ_Services(QObject *parent)
{

}


void JSJ_Services::SetBackImage(QWidget*W, QString Name)
{
    W->setWindowTitle("教师机");
    QString path = QString::fromLocal8Bit("%1/img/backimg/%2.png")
            .arg(QCoreApplication::applicationDirPath()).arg(Name);
    qDebug()<<path;
    QPixmap map(path);
    QSize dialogSize = W->size(); // 或者指定具体的dialog对象
    qDebug()<<"2:"<<dialogSize;
    map = map.scaled(dialogSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    W->setAutoFillBackground(true);
    QPalette pal = W->palette();
    pal.setBrush(W->backgroundRole(), map);
    W->setPalette(pal);
    W->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
}
