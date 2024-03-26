#include "mysql.h"

#include <QCoreApplication>
#include <QSettings>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "mainwindow.h"

mysql::mysql()
{
//    Readmysqlini();
}

mysql::~mysql()
{
    db.close();
}

void mysql::Readmysqlini()
{
    QString File = QString("%1/%2").arg(QDir::currentPath()).arg(sqlcongfigpath);
    QSettings *configIniRead = new QSettings(File, QSettings::IniFormat);
    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
    sqldata.HostName = configIniRead->value("/DB/HostName").toString();
    sqldata.UserName = configIniRead->value("/DB/UserName").toString();
    sqldata.PassWord = configIniRead->value("/DB/PassWord").toString();
    sqldata.DataBase = configIniRead->value("/DB/DataBase").toString();
    sqldata.Dort = configIniRead->value("/DB/Dort").toInt();
}

bool mysql::MysqlConnect(const QString &dbName)
{
    db = QSqlDatabase::addDatabase(("QMYSQL"));
    db.setHostName(sqldata.HostName);
    db.setPort(sqldata.Dort);
    db.setDatabaseName(sqldata.DataBase);
    db.setUserName(sqldata.UserName);
    db.setPassword(sqldata.PassWord);
    if (!db.open()) {
        return false;
    }
    else
        return true;
}
QString mysql::AddStationData(stationlist list) //车站数据添加
{
    QSqlQuery query(db);
    QString returnStr;
    query.prepare("INSERT INTO tb_stationdata (stationcode, stationname,stationfilename) VALUES (?, ?, ?)");
    QVariantList codes;
    codes << list.StationCode;
    query.addBindValue(codes);

    QVariantList names;
    names << list.stationname;
    query.addBindValue(names);

    QVariantList files;
    files << list.stationfilename;
    query.addBindValue(files);
    if (!query.execBatch()) {
        query.finish();
        return query.lastError().text();
    }
    query.finish();
    return  "";
}

QVector<stationlist> mysql::Readstationlist()
{
    QVector<stationlist>  vect_list;
    QString sqlstr;

    QSqlQuery result = db.exec(" select * from tb_stationdata");
    while(result.next()){
        stationlist list;
        list.InitInfo();
        list.stationname = result.value("stationname").toString();
        list.StationID = result.value("id").toInt();
        list.StationCode = result.value("stationcode").toInt();
        list.stationfilename = result.value("stationfilename").toString();
        vect_list.push_back(list);
    }
    return vect_list;
}
/********************GD数据库操作**************************/
QString mysql::AddGDData(QVariantList codes, QVariantList names, QVariantList types, int stationid)
{
    /****************判断是否包含已经有的设备 存在删除从新添加************************/
    QString GetInfo =DetleGDData(stationid,"tb_qddev") ;
    if(GetInfo != "")
    {
//        qDebug()<<GetInfo;
        return  GetInfo;
    }

    QSqlQuery query(db);
    QString returnStr;
    query.prepare("INSERT INTO tb_qddev (stationid,code,qdname,qdtype,isfsflag) VALUES (?,?,?,?,?)");
    QVariantList stationids;
    for(int i = 0;i<codes.size();i++)
    {
        stationids.push_back(stationid);
    }
    query.addBindValue(stationids);
    query.addBindValue(codes);

    query.addBindValue(names);

    query.addBindValue(types);

    QVariantList states;
    for(int i = 0;i<codes.size();i++)
    {
        states.push_back(0);
    }
    query.addBindValue(states);
    if (!query.execBatch()) {
        query.finish();
        return query.lastError().text();
    }
    query.finish();
    return  "";
}
QString mysql::DetleGDData(int stationid, QString FormName)
{
    QString sql = QString("delete from %1 where stationid='%2'").arg(FormName).arg(stationid);
    //开启一个事务
    db.transaction();
    // 执行sql语句
    QSqlQuery query;
    if (!query.exec(sql)) {
        query.finish();
        return query.lastError().text();
    }
    else
    {
        query.finish();
        db.commit();
        return "";
    }
}
/********************END**************************/

/********************QJ数据库操作**************************/
QString mysql::AddQJData(QVariantList codes, QVariantList names, int stationid)
{
    /****************判断是否包含已经有的设备 存在删除从新添加************************/
    QString GetInfo =DetleGDData(stationid,"tb_qjdev") ;
    if(GetInfo != "")
    {
        return  GetInfo;
    }

    QSqlQuery query(db);
    QString returnStr;

    query.prepare("INSERT INTO tb_qjdev (stationid, code,qdname,isfsflag) VALUES (? ,?, ?, ?)");

    QVariantList stationids;
    for(int i = 0;i<codes.size();i++)
    {
        stationids.push_back(stationid);
    }
    query.addBindValue(stationids);
    query.addBindValue(codes);

    query.addBindValue(names);

    QVariantList states;
    for(int i = 0;i<codes.size();i++)
    {
        states.push_back(0);
    }
    query.addBindValue(states);

    if (!query.execBatch()) {
        query.finish();
        return query.lastError().text();
    }
    query.finish();
    return  "";
}
/********************END**************************/

/********************GDDC数据库操作**************************/
QString mysql::AddGDDCData(QVariantList codes, QVariantList names, int stationid)
{
    /****************判断是否包含已经有的设备 存在删除从新添加************************/
    QString GetInfo =DetleGDData(stationid,"tb_dcdev") ;
    if(GetInfo != "")
    {
        return  GetInfo;
    }

    QSqlQuery query(db);
    QString returnStr;

    query.prepare("INSERT INTO tb_dcdev (stationid, code,dcname,isdcqdfsflag,isdcdsflag) VALUES (? ,?, ?, ?, ?)");

    QVariantList stationids;
    for(int i = 0;i<codes.size();i++)
    {
        stationids.push_back(stationid);
    }
    query.addBindValue(stationids);
    query.addBindValue(codes);

    query.addBindValue(names);

    QVariantList states;
    for(int i = 0;i<codes.size();i++)
    {
        states.push_back(0);
    }
    query.addBindValue(states);
    query.addBindValue(states);

    if (!query.execBatch()) {
        query.finish();
        return query.lastError().text();
    }
    query.finish();
    return  "";
}

/********************END**************************/
/********************XH数据库操作**************************/
QString mysql::AddXHData(QVariantList codes, QVariantList names, QVariantList types, int stationid)
{
    /****************判断是否包含已经有的设备 存在删除从新添加************************/
    QString GetInfo =DetleGDData(stationid,"tb_xhdev") ;
    if(GetInfo != "")
    {
        return  GetInfo;
    }

    QSqlQuery query(db);
    QString returnStr;

    query.prepare("INSERT INTO tb_xhdev (stationid, code,xhname,xhtype,isfsflag) VALUES (? ,?, ?, ?, ?)");

    QVariantList stationids;
    for(int i = 0;i<codes.size();i++)
    {
        stationids.push_back(stationid);
    }
    query.addBindValue(stationids);
    query.addBindValue(codes);

    query.addBindValue(names);
    query.addBindValue(types);

    QVariantList states;
    for(int i = 0;i<codes.size();i++)
    {
        states.push_back(0);
    }
    query.addBindValue(states);

    if (!query.execBatch()) {
        query.finish();
        return query.lastError().text();
    }
    query.finish();
    return  "";
}
/********************END**************************/
/********************GD基本参数数据库操作**************************/
QString mysql::AddGDBaseData(QVariantList names,int stationid)
{
    /****************判断是否包含已经有的设备 存在删除从新添加************************/
    QString GetInfo =DetleGDData(stationid,"tb_gddata") ;
    if(GetInfo != "")
    {
        return  GetInfo;
    }

    QSqlQuery query(db);
    QString returnStr;

    query.prepare("INSERT INTO tb_gddata (gdname, gdattr,gddir,"
                  "gdjfattr,gdoverlevel,gdplatform,"
                  "gdisCRH,gdiswater,gdisblowdown,"
                  "gdArmy,gdeffectlength,gdmaxtrain,"
                  "gdcatenary,gdheavytrain,stationid,"
                  "version) "
                  "VALUES (?,?,?,?,?,"
                  "?,?,?,?,?,"
                  "?,?,?,?,?,"
                  "?)");

    query.addBindValue(names);
    QVariantList code;
    QVariantList Versions;
    for(int i = 0;i<names.size();i++)
    {
        code.push_back(1);
        Versions.push_back("V0.1");
    }
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(code);
    query.addBindValue(Versions);



    if (!query.execBatch()) {
        query.finish();
        return query.lastError().text();
    }
    query.finish();
    return  "";
}
/********************END**************************/

/********************分区基本参数数据库操作**************************/
//QVector<zoneData> mysql::ReadZonelist(int id)
//{
//    QVector<zoneData>  vect_list;
//    QString buffInfo;
//    QString sqlstr;
//    sqlstr = QString(" select * from tb_zonedev where stationid='%1'").arg(id);
//    QSqlQuery result = db.exec(sqlstr);
//    while(result.next()){
//        zoneData   list;
//        list.InitInfo();
//        list.id = result.value("id").toInt();
//        list.stationid = result.value("stationid").toInt();
//        list.zonename = result.value("zonename").toString();
//        buffInfo= result.value("dcdevid").toString();
//        list.DevIdSpilt(buffInfo,0x11);
//        buffInfo = result.value("qddevid").toString();
//        list.DevIdSpilt(buffInfo,0x22);
//        buffInfo = result.value("qjdevid").toString();
//        list.DevIdSpilt(buffInfo,0x33);
//        buffInfo = result.value("xhdevid").toString();
//        list.DevIdSpilt(buffInfo,0x44);
//        vect_list.push_back(list);
//    }
//    return vect_list;
//}

QString mysql::LoadZoneData(zoneData *zone)
{
    /*************更新数据********************/
    QString returnStr;
    QString dcstr,qdstr,qjstr,xhstr;
    dcstr = qjstr = qdstr = xhstr = "";
    for(unsigned int i = 0;i<zone->vect_dcDev.size();i++)
    {
        if(i == zone->vect_dcDev.size()-1)
            dcstr.push_back(zone->vect_dcDev[i].Devname);
        else
            dcstr.push_back(zone->vect_dcDev[i].Devname+"&");
    }
    for(unsigned int i = 0;i<zone->vect_qdDev.size();i++)
    {
        if(i == zone->vect_qdDev.size()-1)
            qdstr.push_back(zone->vect_qdDev[i].Devname);
        else
            qdstr.push_back(zone->vect_qdDev[i].Devname+"&");
    }
    for(unsigned int i = 0;i<zone->vect_qjDev.size();i++)
    {
        if(i == zone->vect_qjDev.size()-1)
            qjstr.push_back(zone->vect_qjDev[i].Devname);
        else
            qjstr.push_back(zone->vect_qjDev[i].Devname+"&");
    }
    for(unsigned int i = 0;i<zone->vect_xhDev.size();i++)
    {
        if(i == zone->vect_xhDev.size()-1)
            xhstr.push_back(zone->vect_xhDev[i].Devname);
        else
            xhstr.push_back(zone->vect_xhDev[i].Devname+"&");
    }
    QString sqlstr;
    if(zone->id == 0xff)
    {
        sqlstr = QString("INSERT INTO tb_zonedev (stationid,"
                         "zonename,dcdevid,qddevid,qjdevid,"
                         "xhdevid) "
                         "VALUES (%1,'%2','%3','%4','%5','%6')")
                .arg(zone->stationid)
                .arg(zone->zonename)
                .arg(dcstr)
                .arg(qdstr)
                .arg(qjstr)
                .arg(xhstr);
    }
    else
    {
        sqlstr = QString("UPDATE tb_zonedev b SET "
                         "b.stationid = %1, "
                         "b.zonename = '%2', "
                         "b.dcdevid = '%3', "
                         "b.qddevid = '%4', "
                         "b.qjdevid = '%5', "
                         "b.xhdevid = '%6'"
                         "WHERE b.id = %7")
                .arg(zone->stationid)
                .arg(zone->zonename)
                .arg(dcstr)
                .arg(qdstr)
                .arg(qjstr)
                .arg(xhstr)
                .arg(zone->id);
//        qDebug()<<sqlstr;
    }
    db.transaction();
    // 执行sql语句
    QSqlQuery query;
    if (!query.exec(sqlstr)) {
        query.finish();
        return query.lastError().text();
    }
    else
    {
        query.finish();
        db.commit();
        return "";
    }
}
QString mysql::DetleZoneData(int Zoneid)
{
    if(Zoneid ==  0xff)
    {
        return QString::fromLocal8Bit("未上传至数据库中！");
    }
    QString sql = QString("delete from tb_zonedev where id='%2'").arg(Zoneid);
    //开启一个事务
    db.transaction();
    // 执行sql语句
    QSqlQuery query;
    if (!query.exec(sql)) {
        query.finish();
        return query.lastError().text();
    }
    else
    {
        query.finish();
        db.commit();
        return "";
    }
}
QString mysql::UpdataDevState(zoneData *zone) //设备状态更新
{
    QString returnStr;
    for(unsigned int i = 0;i<zone->vect_dcDev.size();i++)
    {
        int state;
        if(zone->vect_dcDev[i].OperStr == QString::fromLocal8Bit("单锁"))
            state = 1;
        if(zone->vect_dcDev[i].OperStr == QString::fromLocal8Bit("单锁至定位"))
            state = 2;
        if(zone->vect_dcDev[i].OperStr == QString::fromLocal8Bit("单锁至反位"))
            state = 3;
        if(zone->vect_dcDev[i].OperStr == QString::fromLocal8Bit("封"))
            state = 4;
        else
            state = 0;
//        returnStr = UpdataDevData("tb_dcdev",zone->stationid,zone->vect_dcDev[i].Devname,state);
    }
    for(unsigned int i = 0;i<zone->vect_qdDev.size();i++)
    {
        int state;
        if(zone->vect_qdDev[i].OperStr == QString::fromLocal8Bit("封"))
            state = 1;
        else
            state = 0;
//        returnStr += UpdataDevData("tb_qddev",zone->stationid,zone->vect_qdDev[i].Devname,state);
    }
    for(unsigned int i = 0;i<zone->vect_qjDev.size();i++)
    {
        int state;
        if(zone->vect_qjDev[i].OperStr == QString::fromLocal8Bit("封"))
            state = 1;
        else
            state = 0;
//        returnStr += UpdataDevData("tb_qjdev",zone->stationid,zone->vect_qjDev[i].Devname,state);
    }
    for(unsigned int i = 0;i<zone->vect_xhDev.size();i++)
    {
        int state;
        if(zone->vect_xhDev[i].OperStr.indexOf( QString::fromLocal8Bit("解")) == -1)
            state = 1;
        else
            state = 0;
//        returnStr += UpdataDevData("tb_xhdev",zone->stationid,zone->vect_xhDev[i].Devname,state);
    }
    return returnStr;
}
QString mysql::UpdataDevData(QString FromName,int stationid,QString Name, int state)
{
    QString  sqlstr;
    QString  devName,cloname;

    if("tb_dcdev" == FromName)
    {
        devName = "dcname";
        if(state == 4)
            cloname = "isdcdsflag";
        else
            cloname = "isdcqdfsflag";
    }
    else  if("tb_qddev" == FromName || "tb_qjdev" == FromName)
    {
        devName = "qdname";
        cloname = "isfsflag";
    }
    else  if("tb_xhdev" == FromName)
    {
        devName = "xhname";
        cloname = "isfsflag";
    }
    sqlstr = QString("UPDATE %1 b SET "
                     "b.%2 = %3 "
                     "WHERE b.stationid = %4 AND "
                     "b.%5 = '%6'")
            .arg(FromName)
            .arg(cloname)
            .arg(state)
            .arg(stationid)
            .arg(devName)
            .arg(Name);
    db.transaction();
    // 执行sql语句
    QSqlQuery query;
    if (!query.exec(sqlstr)) {
        query.finish();
        return query.lastError().text();
    }
    else
    {
        query.finish();
        db.commit();
        return "";
    }
}
/********************END**************************/

