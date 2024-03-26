#include "stationshowfrom.h"
#include "ui_stationshowfrom.h"
#include <QSettings>
#include <QDir>

#include <QMessageBox>
#include "dialog_powersupply.h"
#include <QLineEdit>
#include <QInputDialog>
#include <QHeaderView>
#include <QLabel>
#include "treemodel.h"
#include "treeitem.h"
#include"JSJ/jsj_services.h"


stationshowfrom::stationshowfrom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::stationshowfrom)
{
    ui->setupUi(this);
    DBSERVICES.
    ui->comboBox->hide();
    ui->pushButton->hide();
    this->setWindowTitle(QString("区域批量设备操作 - TDCS/CTC3.0 通用车站操作终端"));
    this->setWindowFlags(Qt::Dialog);
    this->move(250,150);
    DBSERVICES.SetBackImage(this,"背景二");
    ui->stationweight->setWindowFlags(Qt::Widget/*SubWindow*/|Qt::FramelessWindowHint);


    connect(this,SIGNAL(Showsignal(int,QString,bool)),ui->stationweight,SLOT(UpdataStationdata(int,QString,bool)));
    connect(ui->stationweight,SIGNAL(SendSelectDevsignal(QStringList,QStringList,QStringList,QStringList,QStringList,QStringList,QStringList,QStringList)),
            this,SLOT(SelectDevslot(QStringList,QStringList,QStringList,QStringList,QStringList,QStringList,QStringList,QStringList)));
    connect(this,SIGNAL(ClearStationState()),ui->stationweight,SLOT(ClearSelectFun()));
    connect(this,SIGNAL(Signal_Refresh_ZC()),ui->stationweight,SLOT(setZCRefresh()));
    connect(this,SIGNAL(ClearState(QString,QString)),ui->stationweight,SLOT(ClearStatesName(QString,QString)));
    IsonlyData = false;

    IniteFun();
    IsEditFlag  = false;
    //ui->radioButton->setDown(false);

    //    Save_comboBox_list_num=0;
    //    for (int i=0;i<20;i++)
    //    {
    //        for (int j=0;j<500;j++)
    //        {
    //            Save_comboBox_Array[i].Dev_Name[j]="";
    //            Save_comboBox_Array[i].Dev_Operation[j]="";
    //            Save_comboBox_Array[i].Dev_State[j]="";

    //        }
    //        Save_comboBox_Array[i].DCDev_NUM=0;
    //        Save_comboBox_Array[i].QDDev_NUM=0;
    //        Save_comboBox_Array[i].QJDev_NUM=0;
    //        Save_comboBox_Array[i].XHJDev_NUM=0;

    //    }
    setWindowFlag(Qt::FramelessWindowHint);  // 去掉标题栏
}

stationshowfrom::~stationshowfrom()
{
    delete ui;
}

void stationshowfrom::IniteFun()
{
    ReadConfigFun();
    ReadZoneData();
    //MeauInite();
}

//void stationshowfrom::ReadConfigFun()
//{
//    QString File = QString("%1/%2").arg(QDir::currentPath()).arg(SHOWPATH);
//    QSettings *configIniRead = new QSettings(File, QSettings::IniFormat);
//    //将读取到的ini文件保存在QString中，先取值，然后通过toString()函数转换成QString类型
//    StationId = configIniRead->value("/config/stataionid").toInt();



//    QVector<stationlist> vect_stationlist;
//#ifdef BatchOper
//    currentSql = new mysql();
//    currentSql->MysqlConnect(DBName);
//    vect_stationlist = currentSql->Readstationlist();
//#else
//    vect_stationlist = global.currentSql->Readstationlist();
//#endif
//    bool  Flag = false;
//    for (int i=0;i<vect_stationlist.size();i++)
//    {
//        if(StationId == vect_stationlist[i].StationID)
//        {
//            StationName = vect_stationlist[i].stationname;
//            StationFile = vect_stationlist[i].stationfilename;
//            Flag = true;
//            break;
//        }
//    }
//    if(!Flag)
//    {
//        QMessageBox::warning(this, QString("报警消息"),
//                             QString("无法获取配置的车站的基本数据，请确定数据库中存在该车站数据！"));
//        return;
//    }

//}
#include"jsj_services.h"
void stationshowfrom::ReadConfigFun()
{
    //    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    //    unsigned int stationID;
    //    QString stationName;
    //    stationID = pmainwindow->STATIONID;
    //    stationName = pmainwindow->STATIONNAME;


    //    if(CZState_QYPLCZ==true)
    //    {

    //    }
    //    else
    //    {

    //    }

    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
         QString StationName;
        if(DBSERVICES.CurBackInfoList[i].StationId == 0)
            StationName = "Station";
        else
            StationName= QString("Station%1").arg(DBSERVICES.CurBackInfoList[i].StationId);
        emit Showsignal(1,StationName,true);//其中1表示放大类型，是sizeType的值，不是stationID
        DBSERVICES.CurBackInfoList[i].StationName = ui->stationweight->StationaName;
    }
}

QVector<zoneData> stationshowfrom::ReadZonelist(int id)
{
    //MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    //unsigned int stationID;
    QString stationName;
    //stationID = pmainwindow->STATIONID;
    // stationName = pmainwindow->STATIONNAME;

    QVector<zoneData>vect_list;
    zoneData list;
    zonedev zone;

    list.InitInfo();
    list.stationid = id;
    list.zonename = "标准站";
    // list.stationid = stationID;
    // list.zonename = stationName;



    //    QByteArray m_str;
    //    QByteArray str_Port;
    //    QFile file("../data/StationDev.txt");
    //    file.open(QIODevice::ReadOnly);
    //    m_str = file.readLine();
    //    if(m_str.left(14)=="#define GDNUM ")
    //    {
    //        list.GDNUM=m_str.right(m_str.length()-14).replace("\r\n","").toInt();
    //    }
    //    m_str = file.readLine();
    //    if(m_str.left(14)=="#define DCNUM ")
    //    {
    //        list.DCNUM=m_str.right(m_str.length()-14).replace("\r\n","").toInt();
    //    }
    //    m_str = file.readLine();
    //    if(m_str.left(14)=="#define XHNUM ")
    //    {
    //        list.XHNUM=m_str.right(m_str.length()-14).replace("\r\n","").toInt();
    //    }

    //    for(int i=0;i<list.DCNUM;i++)
    //    {
    //        zone.InitInfo();
    //        m_str = file.readLine();
    //        if(m_str=="##DC\r\n")
    //        {
    //            m_str = file.readLine();
    //            if(m_str.left(15)=="gddc.m_strName=")
    //            {
    //                zone.Devname=m_str.right(m_str.length()-15).replace(";\r\n","").replace("","");
    //            }
    //            m_str = file.readLine();
    //            if(m_str.left(13)=="gddc.m_nCode=")
    //            {
    //                zone.m_nCode=m_str.right(m_str.length()-13).replace(";\r\n","").replace("","").toInt();
    //            }
    //        }
    //        list.vect_dcDev.push_back(zone);
    //    }
    //    for(int i=0;i<list.XHNUM;i++)
    //    {
    //        zone.InitInfo();
    //        m_str = file.readLine();
    //        if(m_str=="##XH\r\n")
    //        {
    //            m_str = file.readLine();
    //            if(m_str.left(14)=="xhd.m_strName=")
    //            {
    //                zone.Devname=m_str.right(m_str.length()-14).replace(";\r\n","").replace("","");
    //            }
    //            m_str = file.readLine();
    //            if(m_str.left(12)=="xhd.m_nCode=")
    //            {
    //                zone.m_nCode=m_str.right(m_str.length()-12).replace(";\r\n","").replace("","").toInt();
    //            }
    //        }
    //        list.vect_xhDev.push_back(zone);
    //    }
    //    for(int i=0;i<list.GDNUM;i++)
    //    {
    //        zone.InitInfo();
    //        m_str = file.readLine();
    //        if(m_str=="##GD\r\n")
    //        {
    //            m_str = file.readLine();
    //            if(m_str.left(13)=="gd.m_strName=")
    //            {
    //                zone.Devname=m_str.right(m_str.length()-13).replace(";\r\n","").replace("","");
    //            }
    //            m_str = file.readLine();
    //            if(m_str.left(11)=="gd.m_nCode=")
    //            {
    //                zone.m_nCode=m_str.right(m_str.length()-11).replace(";\r\n","").replace("","").toInt();
    //            }
    //        }
    //        if(zone.Devname.contains("_QJ"))
    //        {
    //            zone.Devname.replace("_QJ","");
    //            list.vect_qjDev.push_back(zone);
    //        }
    //        else
    //        {
    //            list.vect_qdDev.push_back(zone);
    //        }
    //    }
    vect_list.push_back(list);
    return vect_list;
}
void stationshowfrom::ReadZoneData()
{
#ifdef BatchOper
    vect_zonename = currentSql->ReadZonelist();
#else
    vect_zonename = ReadZonelist(StationId);
#endif

    m_List_LSPLGZQ << "标准站";
    //    m_List_LSPLGZQ << "临时批量工作区2";
    //    m_List_LSPLGZQ << "临时批量工作区3";
    //    m_List_LSPLGZQ << "临时批量工作区4";
    //    for(int i = 0;i<vect_zonename.size();i++)
    //    {
    //        m_List_LSPLGZQ << vect_zonename[i].zonename;
    //    }
    Save_comboBox_list_num=0;
    //ui->comboBox->addItems(m_List_LSPLGZQ);
    // comboBox_Name=ui->comboBox->currentText();


    IniteTreeData();
    //    ui->comboBox->setCurrentIndex(0);

    //    for(int i = 0;i<vect_zonename.size();i++)
    //    {
    //        ui->comboBox->addItem(vect_zonename[i].zonename);
    //    }
    //    ui->comboBox->setCurrentIndex(0);
}

void stationshowfrom::IniteTreeData()
{

    TreeModel = new QStandardItemModel(4, 1);
    TreeModel->setHeaderData(0, Qt::Horizontal, QString("设备"));
    //    TreeModel->setHeaderData(1, Qt::Horizontal, QString("操作"));
    //    TreeModel->setHeaderData(2, Qt::Horizontal, QString("状态"));


    if(vect_zonename.size()>0)
    {
        CurrectZone = &vect_zonename[0];
    }
    ui->treeView->setModel(TreeModel);
    ui->treeView->setColumnWidth(0, 150);
    ui->treeView->setColumnWidth(1, 100);

    ui->treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->treeView, SIGNAL(clicked(const QModelIndex)), this, SLOT(TreeViewClick(const QModelIndex)));
    connect(TreeModel, &QStandardItemModel::itemChanged, this, &stationshowfrom::treeItemChanged);
    connect(ui->treeView,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(onCustomContextMenuRequested(const QPoint&)));
    UpdataTreelist(CurrectZone);

}

bool stationshowfrom::IsMianSelect(QString name)
{
    for (int i = 0; i < vect_zonename.size(); i++)
    {
        if(name == vect_zonename[i].zonename)
        {
            return true;
        }
    }
    return false;
}

void stationshowfrom::UpdataTreelist(zoneData* zone)
{
    if(zone == nullptr)
        return;
    int i_num=0;
    int Row = 0;
    //    comboBox_Name=ui->comboBox->currentText();
    //    //    QStandardItem *buffitem  = new QStandardItem(zone->zonename);
    // QStandardItem *buffitem  = new QStandardItem("");
    //    buffitem->setCheckable(true);

    //    if(zone->IsSelect)
    //        buffitem->setCheckState(Qt::Checked);
    //    else
    //        buffitem->setCheckState(Qt::Unchecked);
    //buffitem->setEditable(false);

    //Row++;
    QStandardItem *buffdcitem  = new QStandardItem(QString("道岔"));
    //    buffdcitem->setCheckable(false);
    //    if(zone->IsdcSelect)
    //        buffdcitem->setCheckState(Qt::Checked);
    //    else
    //        buffdcitem->setCheckState(Qt::Unchecked);
    buffdcitem->setEditable(false);
    //buffitem->appendRow(buffdcitem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].DCDev_NUM=zone->vect_dcDev.size();
    for(unsigned int i = 0;i<zone->vect_dcDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_dcDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_dcDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_dcDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_dcDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_dcDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_dcDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_dcDev[i].StateStr;
        buffdcitem->appendRow(list);
        Row++;
        i_num++;
    }
    TreeModel->setItem(0, 0,buffdcitem);
    QStandardItem *buffqditem  = new QStandardItem(QString("股道/无岔"));
    //    buffqditem->setCheckable(false);
    //    if(zone->IsqdSelect)
    //        buffqditem->setCheckState(Qt::Checked);
    //    else
    //        buffqditem->setCheckState(Qt::Unchecked);
    buffqditem->setEditable(false);
    // buffitem->appendRow(buffqditem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].QDDev_NUM=zone->vect_qdDev.size();
    for(unsigned int i = 0;i<zone->vect_qdDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_qdDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_qdDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_qdDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_qdDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_qdDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_qdDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_qdDev[i].StateStr;
        buffqditem->appendRow(list);
        Row++;
        i_num++;
    }
    TreeModel->setItem(1, 0,buffqditem);
    QStandardItem *buffqjitem  = new QStandardItem(QString("区间"));
    //    buffqjitem->setCheckable(false);
    //    if(zone->IsqjSelect)
    //        buffqjitem->setCheckState(Qt::Checked);
    //    else
    //        buffqjitem->setCheckState(Qt::Unchecked);
    buffqjitem->setEditable(false);
    //buffitem->appendRow(buffqjitem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].QJDev_NUM=zone->vect_qjDev.size();
    for(unsigned int i = 0;i<zone->vect_qjDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_qjDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_qjDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_qjDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_qjDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_qjDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_qjDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_qjDev[i].StateStr;
        buffqjitem->appendRow(list);
        Row++;
        i_num++;
    }
    TreeModel->setItem(2, 0,buffqjitem);
    //    QStandardItem *buffxhitem  = new QStandardItem(QString("信号机"));
    //    buffxhitem->setCheckable(true);
    //    if(zone->IsxhSelect)
    //        buffxhitem->setCheckState(Qt::Checked);
    //    else
    //        buffxhitem->setCheckState(Qt::Unchecked);
    //    buffxhitem->setEditable(false);
    //    buffitem->appendRow(buffxhitem);
    //    Row++;
    //    Save_comboBox_Array[Save_comboBox_list_num].XHJDev_NUM=zone->vect_xhDev.size();
    //    for(unsigned int i = 0;i<zone->vect_xhDev.size();i++)
    //    {
    //        QList<QStandardItem*> list;
    //        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_xhDev[i].Devname);
    //        buffchiditem->setCheckable(true);
    //        buffchiditem->setEditable(false);
    //        if(zone->vect_xhDev[i].IsselectFlag)
    //            buffchiditem->setCheckState(Qt::Checked);
    //        else
    //            buffchiditem->setCheckState(Qt::Unchecked);
    //        list.append(buffchiditem);
    //        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_xhDev[i].Devname;

    //        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_xhDev[i].OperStr);
    //        QBrush bush;
    //        bush.setColor(Qt::red);
    //        buffOperitem->setForeground(bush);
    //        list.append(buffOperitem);
    //        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_xhDev[i].OperStr;

    //        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_xhDev[i].StateStr);
    //        QBrush bush1;
    //        bush1.setColor(Qt::green);
    //        buffstateitem->setForeground(bush1);
    //        list.append(buffstateitem);
    //        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_xhDev[i].StateStr;
    //        buffxhitem->appendRow(list);
    //        Row++;
    //        i_num++;
    //    }
    //TreeModel->setItem(0, 0,buffitem);
    ui->treeView->expandAll();
}

void stationshowfrom::UpdataTreelist_Save(zoneData* zone)
{
    if(zone == nullptr)
        return;
    int i_num=0;
    int Row = 0;
    comboBox_Name=ui->comboBox->currentText();
    Save_comboBox_list_num=ui->comboBox->currentIndex();
    //    QStandardItem *buffitem  = new QStandardItem(zone->zonename);
    QStandardItem *buffitem  = new QStandardItem(comboBox_Name);
    buffitem->setCheckable(true);

    if(zone->IsSelect)
        buffitem->setCheckState(Qt::Checked);
    else
        buffitem->setCheckState(Qt::Unchecked);
    buffitem->setEditable(false);

    Row++;
    QStandardItem *buffdcitem  = new QStandardItem(QString("道岔"));
    buffdcitem->setCheckable(true);
    if(zone->IsdcSelect)
        buffdcitem->setCheckState(Qt::Checked);
    else
        buffdcitem->setCheckState(Qt::Unchecked);
    buffdcitem->setEditable(false);
    buffitem->appendRow(buffdcitem);
    Row++;
    for(unsigned int i = 0;i<Save_comboBox_Array[Save_comboBox_list_num].DCDev_NUM;i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        //        if(zone->vect_dcDev[i].IsselectFlag)
        buffchiditem->setCheckState(Qt::Checked);
        //        else
        //            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);

        QStandardItem *buffOperitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);

        QStandardItem *buffstateitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        buffdcitem->appendRow(list);
        Row++;
        i_num++;
    }

    QStandardItem *buffqditem  = new QStandardItem(QString("股道/无岔"));
    buffqditem->setCheckable(true);
    if(zone->IsqdSelect)
        buffqditem->setCheckState(Qt::Checked);
    else
        buffqditem->setCheckState(Qt::Unchecked);
    buffqditem->setEditable(false);
    buffitem->appendRow(buffqditem);
    Row++;
    for(unsigned int i = 0;i<Save_comboBox_Array[Save_comboBox_list_num].QDDev_NUM;i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        //        if(zone->vect_qdDev[i].IsselectFlag)
        buffchiditem->setCheckState(Qt::Checked);
        //        else
        //            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);

        QStandardItem *buffOperitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);

        QStandardItem *buffstateitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        buffqditem->appendRow(list);
        Row++;
        i_num++;
    }

    QStandardItem *buffqjitem  = new QStandardItem(QString("区间"));
    buffqjitem->setCheckable(true);
    if(zone->IsqjSelect)
        buffqjitem->setCheckState(Qt::Checked);
    else
        buffqjitem->setCheckState(Qt::Unchecked);
    buffqjitem->setEditable(false);
    buffitem->appendRow(buffqjitem);
    Row++;
    for(unsigned int i = 0;i<Save_comboBox_Array[Save_comboBox_list_num].QJDev_NUM;i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        //        if(zone->vect_qjDev[i].IsselectFlag)
        buffchiditem->setCheckState(Qt::Checked);
        //        else
        //            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);

        QStandardItem *buffOperitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);

        QStandardItem *buffstateitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        buffqjitem->appendRow(list);
        Row++;
        i_num++;
    }

    QStandardItem *buffxhitem  = new QStandardItem(QString("信号机"));
    buffxhitem->setCheckable(true);
    if(zone->IsxhSelect)
        buffxhitem->setCheckState(Qt::Checked);
    else
        buffxhitem->setCheckState(Qt::Unchecked);
    buffxhitem->setEditable(false);
    buffitem->appendRow(buffxhitem);
    Row++;
    for(unsigned int i = 0;i<Save_comboBox_Array[Save_comboBox_list_num].XHJDev_NUM;i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        //        if(zone->vect_xhDev[i].IsselectFlag)
        buffchiditem->setCheckState(Qt::Checked);
        //        else
        //            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);

        QStandardItem *buffOperitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);

        QStandardItem *buffstateitem  = new QStandardItem(Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        buffxhitem->appendRow(list);
        Row++;
        i_num++;
    }
    TreeModel->setItem(0, 0,buffitem);
    ui->treeView->expandAll();
}

void stationshowfrom::UpdataTreelist_refresh(zoneData* zone)
{
    if(zone == nullptr)
        return;
    int i_num=0;
    int Row = 0;
    comboBox_Name=ui->comboBox->currentText();
    //    QStandardItem *buffitem  = new QStandardItem(zone->zonename);
    QStandardItem *buffitem  = new QStandardItem(comboBox_Name);
    buffitem->setCheckable(true);

    if(zone->IsSelect)
        buffitem->setCheckState(Qt::Checked);
    else
        buffitem->setCheckState(Qt::Unchecked);
    buffitem->setEditable(false);

    Row++;
    QStandardItem *buffdcitem  = new QStandardItem(QString("道岔"));
    buffdcitem->setCheckable(true);
    if(zone->IsdcSelect)
        buffdcitem->setCheckState(Qt::Checked);
    else
        buffdcitem->setCheckState(Qt::Unchecked);
    buffdcitem->setEditable(false);
    buffitem->appendRow(buffdcitem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].DCDev_NUM=zone->vect_dcDev.size();
    for(unsigned int i = 0;i<zone->vect_dcDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_dcDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_dcDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_dcDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_dcDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_dcDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_dcDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_dcDev[i].StateStr;
        buffdcitem->appendRow(list);
        Row++;
        i_num++;
    }

    QStandardItem *buffqditem  = new QStandardItem(QString("股道/无岔"));
    buffqditem->setCheckable(true);
    if(zone->IsqdSelect)
        buffqditem->setCheckState(Qt::Checked);
    else
        buffqditem->setCheckState(Qt::Unchecked);
    buffqditem->setEditable(false);
    buffitem->appendRow(buffqditem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].QDDev_NUM=zone->vect_qdDev.size();
    for(unsigned int i = 0;i<zone->vect_qdDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_qdDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_qdDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_qdDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_qdDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_qdDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_qdDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_qdDev[i].StateStr;
        buffqditem->appendRow(list);
        Row++;
        i_num++;
    }

    QStandardItem *buffqjitem  = new QStandardItem(QString("区间"));
    buffqjitem->setCheckable(true);
    if(zone->IsqjSelect)
        buffqjitem->setCheckState(Qt::Checked);
    else
        buffqjitem->setCheckState(Qt::Unchecked);
    buffqjitem->setEditable(false);
    buffitem->appendRow(buffqjitem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].QJDev_NUM=zone->vect_qjDev.size();
    for(unsigned int i = 0;i<zone->vect_qjDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_qjDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_qjDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_qjDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_qjDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_qjDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_qjDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_qjDev[i].StateStr;
        buffqjitem->appendRow(list);
        Row++;
        i_num++;
    }

    QStandardItem *buffxhitem  = new QStandardItem(QString("信号机"));
    buffxhitem->setCheckable(true);
    if(zone->IsxhSelect)
        buffxhitem->setCheckState(Qt::Checked);
    else
        buffxhitem->setCheckState(Qt::Unchecked);
    buffxhitem->setEditable(false);
    buffitem->appendRow(buffxhitem);
    Row++;
    Save_comboBox_Array[Save_comboBox_list_num].XHJDev_NUM=zone->vect_xhDev.size();
    for(unsigned int i = 0;i<zone->vect_xhDev.size();i++)
    {
        QList<QStandardItem*> list;
        QStandardItem *buffchiditem  = new QStandardItem(zone->vect_xhDev[i].Devname);
        buffchiditem->setCheckable(true);
        buffchiditem->setEditable(false);
        if(zone->vect_xhDev[i].IsselectFlag)
            buffchiditem->setCheckState(Qt::Checked);
        else
            buffchiditem->setCheckState(Qt::Unchecked);
        list.append(buffchiditem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Name[i_num]=zone->vect_xhDev[i].Devname;

        QStandardItem *buffOperitem  = new QStandardItem(zone->vect_xhDev[i].OperStr);
        QBrush bush;
        bush.setColor(Qt::red);
        buffOperitem->setForeground(bush);
        list.append(buffOperitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_Operation[i_num]=zone->vect_xhDev[i].OperStr;

        QStandardItem *buffstateitem  = new QStandardItem(zone->vect_xhDev[i].StateStr);
        QBrush bush1;
        bush1.setColor(Qt::green);
        buffstateitem->setForeground(bush1);
        list.append(buffstateitem);
        Save_comboBox_Array[Save_comboBox_list_num].Dev_State[i_num]=zone->vect_xhDev[i].StateStr;
        buffxhitem->appendRow(list);
        Row++;
        i_num++;
    }
    TreeModel->setItem(0, 0,buffitem);
    ui->treeView->expandAll();
}

void stationshowfrom::SelectDevslot(QStringList XHList,QStringList DCList,QStringList QJList,QStringList QDList,QStringList XHdev_code,QStringList DCdev_code,QStringList QJdev_code,QStringList QDdev_code)
{
    if(CurrectZone == nullptr)
        return ;
    CurrectZone->AddDevList(XHList,DCList,QJList,QDList,XHdev_code,DCdev_code,QJdev_code,QDdev_code);
    UpdataTreelist(CurrectZone);
}
bool stationshowfrom::IsTopItem(QStandardItem* item)
{
    if (!item)
        return false;
    if (!item->hasChildren())
        return true;
    return false;
}

QStringList strList;
void stationshowfrom::treeItemChanged(QStandardItem *item)
{
    if (CurrectZone == nullptr || item == nullptr)
        return ;
    //    qDebug()<<item->text();
    if (item->isCheckable() && !IsonlyData)
    {
        //如果条目是存在复选框的，那么就进行下面的操作

        if(CurrectZone !=nullptr)
        {
            IsonlyData = false;
            CurrectZone->SetCheck(item->text(),(item->checkState()));
        }
        if(IsTopItem(item))
        {
            if(item->checkState() == Qt::Unchecked)
            {
                NodeInfo tempInfo;
                tempInfo.TopName = item->parent()->text();
                tempInfo.SelfName = item->text();
                itemList.push_back(tempInfo);
            }


        }
        //if(item!=nullptr)
        //ui->treeView->model()->removeRow(item->row());

        //qDebug()<<"ChecState:"<<item->text();

#if 0
        if (item->isTristate())
        {
            //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
            if (state != Qt::PartiallyChecked)
            {
                //当前是选中状态，需要对其子项目进行全选
                treeItem_checkAllChild(item, state == Qt::Checked ? true : false);
            }
        }
        else
        {
            //说明是两态的，两态会对父级的三态有影响
            //判断兄弟节点的情况
            treeItem_CheckChildChanged(item);
        }
#endif
    }

}
void stationshowfrom::InitInfo(zoneData* zone)
{
    if(zone == nullptr)
        return;
    int i_num=0;
    int Row = 0;
    for(int i = 0; i < itemList.size();++i)
    {
        QString topName = itemList[i].TopName;
        if(topName == "股道/无岔")
        {
            for(int var = 0; var < zone->vect_qdDev.size();++var)
            {
                if(zone->vect_qdDev[var].Devname == itemList[i].SelfName)
                {
                    zone->vect_qdDev.erase(zone->vect_qdDev.begin()+var);
                    emit ClearState(topName,itemList[i].SelfName);
                    break;
                }
            }
        }
        else if (topName == "道岔")
        {
            for(int var = 0; var < zone->vect_dcDev.size();++var)
            {
                if(zone->vect_dcDev[var].Devname == itemList[i].SelfName)
                {
                    zone->vect_dcDev.erase(zone->vect_dcDev.begin()+var);
                    emit ClearState(topName,itemList[i].SelfName);
                    break;
                }
            }
        }
        else if (topName == "区间")
        {
            for(int var = 0; var < zone->vect_qjDev.size();++var)
            {
                if(zone->vect_qjDev[var].Devname == itemList[i].SelfName)
                {
                    zone->vect_qjDev.erase(zone->vect_qjDev.begin()+var);
                    emit ClearState(topName,itemList[i].SelfName);
                    break;
                }
            }
        }
    }
    UpdataTreelist(zone);
}

void stationshowfrom::ClearInfo()
{
    if(CurrectZone!=nullptr)
    {
        CurrectZone->vect_dcDev.clear();
        CurrectZone->vect_qdDev.clear();
        CurrectZone->vect_qjDev.clear();
        CurrectZone->vect_xhDev.clear();

        ui->stationweight->ClearAll();
        itemList.clear();
        UpdataTreelist(CurrectZone);
    }
}
void stationshowfrom::TreeViewClick(const QModelIndex &index)
{
    if(CurrectZone==nullptr)
        return ;

}






//void stationshowfrom::on_AddZone_btn_clicked() //添加分区
//{
//    if(IsEditFlag==false)
//    {
//        return;
//    }
//    QString NewName;
//    for(int i = 1;;i++)
//    {
//        QString Name = QString("自定义区域%1").arg(i);
//        if(!IsMianSelect(Name))
//        {
//            NewName = Name;
//            break;
//        }
//    }
//    QString dlgTitle=QString("新增分区");
//    QString txtLabel=QString("请输入分区名称");
//    QString defaultInput= NewName;
//    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
//    //QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入
//    bool ok=false;
//    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
//    if (ok && !text.isEmpty())
//        NewName= text;
//    zoneData zone;
//    zone.InitInfo();
//    zone.stationid = StationId;
//    zone.zonename = NewName;
//    vect_zonename.push_back(zone);
//    CurrectZone = &zone;
//    ui->comboBox->addItem(zone.zonename);
//    ui->comboBox->setCurrentText(zone.zonename);
//    //    IsEditFlag = true;
//    //    ui->radioButton->setChecked(true);
//    //    ui->stationweight->IsEditFlag = IsEditFlag;
//    UpdataTreelist(CurrectZone);
//    emit ClearStationState();
//}



void stationshowfrom::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(IsEditFlag==false)
    {
        return;
    }
    for (int i = 0; i < vect_zonename.size(); i++)
    {
        if(arg1 == vect_zonename[i].zonename)
        {
            // vect_zonename[i].SetAllCheck();
            CurrectZone = &vect_zonename[i];
            UpdataTreelist(CurrectZone);
            DevOperFun(0xff);
            break;
        }
    }
}

void stationshowfrom::on_loadzone_btn_clicked()
{
    if(CurrectZone == nullptr)
        return ;
    if(IsEditFlag==false)
    {
        return;
    }
    //    IsEditFlag =false; //关闭编辑按钮
    //    ui->radioButton->setChecked(false);
    QString dlgTitle=QString("注意-TDCS/CTC 3.0 通用车站操作窗口");
    QString strInfo=QString("确定要上传分区定义到服务器");
    QMessageBox::StandardButton result;//返回选择的按钮
    result=QMessageBox::question(this, dlgTitle, strInfo,
                                 QMessageBox::Yes|QMessageBox::No);
    if (result==QMessageBox::Yes)
    {
#ifdef BatchOper
        //        QString Info = currentSql->LoadZoneData(CurrectZone);
#else
        //        QString Info = global.currentSql->LoadZoneData(CurrectZone);
        QString Info = "";
#endif
        if(Info == "")
        {
            dlgTitle=QString("提示信息-TDCS/CTC 3.0 通用车站操作窗口");
            strInfo=QString("分区定义上传成功");
            QMessageBox::information(this, dlgTitle, strInfo,
                                     QMessageBox::Ok,QMessageBox::NoButton);
        }
        else
        {
            QMessageBox::warning(this, QString("分区数据库操作提示"),
                                 Info);
        }
    }

}

void stationshowfrom::on_pushButton_clicked() //重命名
{
    if(CurrectZone == nullptr)
        return ;
    if(IsEditFlag==false)
    {
        return;
    }
    QString dlgTitle=QString("重命名分区");
    QString txtLabel=QString("请输入分区名称");
    QString defaultInput=CurrectZone->zonename;
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;//正常文字输入
    //QLineEdit::EchoMode echoMode=QLineEdit::Password;//密码输入
    bool ok=false;
    QString text = QInputDialog::getText(this, dlgTitle,txtLabel, echoMode,defaultInput, &ok);
    if (ok && !text.isEmpty())
    {
        CurrectZone->zonename = text;

        ui->comboBox->clear();
        for(int i = 0;i<vect_zonename.size();i++)
        {
            ui->comboBox->addItem(vect_zonename[i].zonename);
        }
        ui->comboBox->setCurrentText(CurrectZone->zonename);
        UpdataTreelist(CurrectZone);
    }

}

//void stationshowfrom::on_detelezone_btn_clicked()
//{
//    if(CurrectZone == nullptr)
//        return ;
//    if(IsEditFlag==false)
//    {
//        return;
//    }
//    //    IsEditFlag =false; //关闭编辑按钮
//    QString dlgTitle=QString("注意-TDCS/CTC 3.0 通用车站操作窗口");
//    QString strInfo=QString("确定要删除：%1").arg(ui->comboBox->currentText());
//    QMessageBox::StandardButton result;//返回选择的按钮
//    result=QMessageBox::warning(this, dlgTitle, strInfo,
//                                QMessageBox::Yes|QMessageBox::No);
//    if (result==QMessageBox::Yes)
//    {
//#ifdef BatchOper
//        currentSql->DetleZoneData(CurrectZone->id);
//#else
//        //        global.currentSql->DetleZoneData(CurrectZone->id);
//#endif

//        if((ui->comboBox->currentIndex()==0)||
//                (ui->comboBox->currentIndex()==1)||
//                (ui->comboBox->currentIndex()==2)||
//                (ui->comboBox->currentIndex()==3)||
//                (ui->comboBox->currentIndex()==4)||
//                (ui->comboBox->currentIndex()==5))
//        {
//            QMessageBox::warning(this, dlgTitle, "该分区不能删除，只能删除新增分区",QMessageBox::Cancel);
//            return;
//        }


//        int index = 0xff;
//        for (int i = 0; i < vect_zonename.size(); i++)
//        {
//            if(ui->comboBox->currentText() == vect_zonename[i].zonename)
//            {
//                index = i;
//                break;
//            }
//        }
//        ui->comboBox->removeItem(ui->comboBox->currentIndex());
//        vect_zonename.remove(index);
//        index=ui->comboBox->currentIndex();
//        ui->comboBox->setCurrentIndex(0);



//        for (int j=0;j<500;j++)
//        {
//            Save_comboBox_Array[index].Dev_Name[j]="";
//            Save_comboBox_Array[index].Dev_Operation[j]="";
//            Save_comboBox_Array[index].Dev_State[j]="";
//        }
//        Save_comboBox_Array[index].DCDev_NUM=0;
//        Save_comboBox_Array[index].QDDev_NUM=0;
//        Save_comboBox_Array[index].QJDev_NUM=0;
//        Save_comboBox_Array[index].XHJDev_NUM=0;
//        CurrectZone=nullptr;
//    }
//}


QVector<Province*> stationshowfrom::initData()
{
    // 初始化数据，5个省，每个省5人
    QVector<Province*> proList;
    int provinceCount = 5;
    int personCount = 5;
    for(int i = 0; i < provinceCount; i++)
    {
        Province* pro = new Province();
        pro->name = QString("Province%1").arg(i);
        for(int j = 0; j < personCount; j++)
        {
            Person* per = new Person();
            per->name = QString("name%1").arg(j);
            per->sex = "man";
            per->age = 25;
            per->phone = "123456789";
            pro->people.append(per);
        }
        proList.append(pro);
    }
    return proList;
}

void stationshowfrom::setModel(const zone* zone)
{
    //    QStringList headers;
    //    headers << QString("设备")
    //            << QString("操作")
    //            << QString("状态");


    //    TreeModel* model = new TreeModel(headers, ui->treeView);

    //    TreeItem* root = model->root();
    //    for(unsigned int i= 0;i<zone->vect_typeDev.size();i++)
    //    {
    //        TreeItem* devtype = new TreeItem(root);
    //        devtype->setPtr(zone->vect_typeDev[i]); // 保存数据指针
    //        devtype->setType(TreeItem::DEVTYPE); // 设置节点类型为PROVINCE
    //        root->addChild(devtype);

    //        for(unsigned int ii= 0;ii<zone->vect_typeDev[i]->vect_DevDev.size();ii++)
    //        {
    //            TreeItem* devdata = new TreeItem(devtype);
    //            devdata->setPtr(zone->vect_typeDev[i]->vect_DevDev[ii]); // 保存数据指针
    //            devdata->setType(TreeItem::DEVNAME); // 设置节点类型为PROVINCE
    //            devtype->addChild(devtype);
    //        }
    //    }

    //    ui->treeView->setModel(model);

}

//void stationshowfrom::on_FSDev_btn_clicked()//单锁
//{
//    if(CurrectZone == nullptr)
//        return ;
//    if(IsEditFlag==false)
//    {
//        return;
//    }
//    DevOperFun(0x11);
//#ifdef BatchOper
//    QString Info = currentSql->UpdataDevState(CurrectZone);
//#else
//    //    QString Info =global.currentSql->UpdataDevState(CurrectZone);
//    QString Info = "";
//#endif
//    if(Info == "")
//    {
//        DevOperFun(0x33);
//        IsonlyData = true;
//        UpdataTreelist(CurrectZone);
//        global.SenddataFun();
//        //        emit Signal_PLCZ_SendData();
//    }
//    else
//    {
//        QMessageBox::warning(this, QString("命令下发失败"),
//                             Info);
//    }
//}

void stationshowfrom::DevOperFun(int type)
{
    if(IsEditFlag==false)
    {
        return;
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_dcDev.size();i++)
    {
        if(!CurrectZone->vect_dcDev[i].IsselectFlag)
        {
            CurrectZone->vect_dcDev[i].OperStr = "";
            CurrectZone->vect_dcDev[i].StateStr = "";
            continue;
        }
        if(type == 0x11)
            CurrectZone->vect_dcDev[i].OperStr = QString("封锁/单锁");
        else   if(type == 0x22)
            CurrectZone->vect_dcDev[i].OperStr = QString("解封/解锁");
        else   if(type == 0x33)
        {
            CurrectZone->vect_dcDev[i].StateStr = QString("封锁/单锁");
            SendData(0x08,CurrectZone->vect_dcDev[i].m_nCode);
            SendData(0x10,CurrectZone->vect_dcDev[i].m_nCode);
        }
        else   if(type == 0x44)
        {
            CurrectZone->vect_dcDev[i].StateStr = QString("解封/解锁");
            SendData(0x09,CurrectZone->vect_dcDev[i].m_nCode);
            SendData(0x11,CurrectZone->vect_dcDev[i].m_nCode);
        }
        else
            CurrectZone->vect_dcDev[i].OperStr = "";
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_qdDev.size();i++)
    {
        if(!CurrectZone->vect_qdDev[i].IsselectFlag)
        {
            CurrectZone->vect_qdDev[i].OperStr = "";
            CurrectZone->vect_qdDev[i].StateStr = "";
            continue;
        }
        if(type == 0x11)
            CurrectZone->vect_qdDev[i].OperStr = QString("封");
        else  if(type == 0x22)
            CurrectZone->vect_qdDev[i].OperStr = QString("解封");
        else   if(type == 0x33)
        {
            CurrectZone->vect_qdDev[i].StateStr = QString("封");
            SendData(0x10,CurrectZone->vect_qdDev[i].m_nCode);
        }
        else   if(type == 0x44)
        {
            CurrectZone->vect_qdDev[i].StateStr = QString("解封");
            SendData(0x11,CurrectZone->vect_qdDev[i].m_nCode);
        }
        else
            CurrectZone->vect_qdDev[i].OperStr = "";
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_qjDev.size();i++)
    {
        if(!CurrectZone->vect_qjDev[i].IsselectFlag)
        {
            CurrectZone->vect_qjDev[i].OperStr = "";
            CurrectZone->vect_qjDev[i].StateStr = "";
            continue;
        }
        if(type == 0x11)
            CurrectZone->vect_qjDev[i].OperStr = QString("封");
        else  if(type == 0x22)
            CurrectZone->vect_qjDev[i].OperStr = QString("解封");
        else   if(type == 0x33)
        {
            CurrectZone->vect_qjDev[i].StateStr = QString("封");
            SendData(0x10,CurrectZone->vect_qjDev[i].m_nCode);
        }
        else   if(type == 0x44)
        {
            CurrectZone->vect_qjDev[i].StateStr = QString("解封");
            SendData(0x11,CurrectZone->vect_qjDev[i].m_nCode);
        }
        else
            CurrectZone->vect_qjDev[i].OperStr = "";
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_xhDev.size();i++)
    {
        if(!CurrectZone->vect_xhDev[i].IsselectFlag)
        {
            CurrectZone->vect_xhDev[i].OperStr = "";
            CurrectZone->vect_xhDev[i].StateStr = "";
            continue;
        }
        if(type == 0x11)
        {
            if(CurrectZone->vect_xhDev[i].Devname.indexOf("D") == -1)
                CurrectZone->vect_xhDev[i].OperStr = QString("封(列)封(调)");
            else
                CurrectZone->vect_xhDev[i].OperStr = QString("封(调)");

        }
        else if(type == 0x22)
        {
            if(CurrectZone->vect_xhDev[i].Devname.indexOf("D") == -1)
                CurrectZone->vect_xhDev[i].OperStr = QString("解封(列)解封(调)");
            else
                CurrectZone->vect_xhDev[i].OperStr = QString("解封(调)");
        }
        else if(type == 0x33)
        {
            if(CurrectZone->vect_xhDev[i].Devname.indexOf("D") == -1)
                CurrectZone->vect_xhDev[i].StateStr = QString("封(列)封(调)");
            else
                CurrectZone->vect_xhDev[i].StateStr = QString("封(调)");
            SendData(0x10,CurrectZone->vect_xhDev[i].m_nCode);

        }
        else if(type == 0x44)
        {
            if(CurrectZone->vect_xhDev[i].Devname.indexOf("D") == -1)
                CurrectZone->vect_xhDev[i].StateStr = QString("解封(列)解封(调)");
            else
                CurrectZone->vect_xhDev[i].StateStr = QString("解封(调)");
            SendData(0x11,CurrectZone->vect_xhDev[i].m_nCode);
        }
        else
            CurrectZone->vect_xhDev[i].OperStr = "";
    }
}

void stationshowfrom::DevOperFun_SIN()
{
    if(IsEditFlag==false)
    {
        return;
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_dcDev.size();i++)
    {
        if(!CurrectZone->vect_dcDev[i].IsselectFlag)
        {
            CurrectZone->vect_dcDev[i].OperStr = "";
            CurrectZone->vect_dcDev[i].StateStr = "";
            continue;
        }

        if(CurrectZone->vect_dcDev[i].StateStr == QString("单锁"))
        {
            SendData(0x08,CurrectZone->vect_dcDev[i].m_nCode);
        }
        else if(CurrectZone->vect_dcDev[i].StateStr == QString("解锁"))
        {
            SendData(0x09,CurrectZone->vect_dcDev[i].m_nCode);
        }
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_qdDev.size();i++)
    {
        if(!CurrectZone->vect_qdDev[i].IsselectFlag)
        {
            CurrectZone->vect_qdDev[i].OperStr = "";
            CurrectZone->vect_qdDev[i].StateStr = "";
            continue;
        }
        if(CurrectZone->vect_qdDev[i].StateStr == QString("封"))
        {
            SendData(0x10,CurrectZone->vect_qdDev[i].m_nCode);
        }
        else if(CurrectZone->vect_qdDev[i].StateStr == QString("解封"))
        {
            SendData(0x11,CurrectZone->vect_qdDev[i].m_nCode);
        }
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_qjDev.size();i++)
    {
        if(!CurrectZone->vect_qjDev[i].IsselectFlag)
        {
            CurrectZone->vect_qjDev[i].OperStr = "";
            CurrectZone->vect_qjDev[i].StateStr = "";
            continue;
        }
        if(CurrectZone->vect_qjDev[i].StateStr == QString("封"))
        {
            SendData(0x10,CurrectZone->vect_qjDev[i].m_nCode);
        }
        else if(CurrectZone->vect_qjDev[i].StateStr == QString("解封"))
        {
            SendData(0x11,CurrectZone->vect_qjDev[i].m_nCode);
        }
    }
    for(unsigned int i = 0 ;i<CurrectZone->vect_xhDev.size();i++)
    {
        if(!CurrectZone->vect_xhDev[i].IsselectFlag)
        {
            CurrectZone->vect_xhDev[i].OperStr = "";
            CurrectZone->vect_xhDev[i].StateStr = "";
            continue;
        }
        if((CurrectZone->vect_xhDev[i].StateStr == QString("封(列)封(调)"))||(CurrectZone->vect_xhDev[i].StateStr == QString("封(调)")))
        {
            SendData(0x10,CurrectZone->vect_xhDev[i].m_nCode);
        }
        else if((CurrectZone->vect_xhDev[i].StateStr == QString("解封(列)解封(调)"))||(CurrectZone->vect_xhDev[i].StateStr == QString("解封(调)")))
        {
            SendData(0x11,CurrectZone->vect_xhDev[i].m_nCode);
        }
    }
}


void stationshowfrom::MeauInite()
{
    DCMenu = new QMenu();
    DCMenu->setStyleSheet("background-color: rgb(174,216,230);");
    QDMenu = new QMenu();
    XHMenu = new QMenu();
    QJMenu = new QMenu();
    QDMenu->setStyleSheet("background-color: rgb(174,216,230);");
    XHMenu->setStyleSheet("background-color: rgb(174,216,230);");
    QJMenu->setStyleSheet("background-color: rgb(174,216,230);");

    connect(DCMenu,SIGNAL(triggered(QAction *)),this,SLOT(onDCMenuTriggered(QAction *)));
    connect(QDMenu,SIGNAL(triggered(QAction *)),this,SLOT(onQDMenuTriggered(QAction *)));
    connect(XHMenu,SIGNAL(triggered(QAction *)),this,SLOT(onXHMenuTriggered(QAction *)));
    connect(QJMenu,SIGNAL(triggered(QAction *)),this,SLOT(onQJMenuTriggered(QAction *)));



    DCDSaction = new QAction(QString("单锁"),this);
    DCDSaction->setCheckable(true);
    DCMenu->addAction(DCDSaction);

    DCDSDaction = new QAction(QString("单锁至定位"),this);
    DCDSDaction->setCheckable(true);
    DCMenu->addAction(DCDSDaction);

    DCDSFaction = new QAction(QString("单锁至反位"),this);
    DCDSFaction->setCheckable(true);
    DCMenu->addAction(DCDSFaction);

    DCFSaction = new QAction(QString("封锁"),this);
    DCFSaction->setCheckable(true);
    DCMenu->addAction(DCFSaction);

    DCMenu->addSeparator();

    DCDeleteaction = new QAction(QString("删除设备"),this);
    //    DCDeleteaction->setCheckable(true);
    DCMenu->addAction(DCDeleteaction);


    QDFSaction = new QAction(QString("封"),this);
    QDFSaction->setCheckable(true);
    QDMenu->addAction(QDFSaction);
    QDDeleteaction = new QAction(QString("删除设备"),this);
    QDMenu->addAction(QDDeleteaction);


    QJFSaction = new QAction(QString("封"),this);
    QJFSaction->setCheckable(true);
    QJMenu->addAction(QJFSaction);
    QJDeleteaction = new QAction(QString("删除设备"),this);
    QJMenu->addAction(QJDeleteaction);

    XHFSaction = new QAction(QString("封(列)封(调)"),this);
    XHFSaction->setCheckable(true);
    XHMenu->addAction(XHFSaction);
    XHDeleteaction = new QAction(QString("删除设备"),this);
    XHMenu->addAction(XHDeleteaction);

}

//void stationshowfrom::on_JFDev_btn_clicked()
//{
//    if(CurrectZone == nullptr)
//        return ;
//    if(IsEditFlag==false)
//    {
//        return;
//    }
//    DevOperFun(0x22);
//#ifdef BatchOper
//    QString Info = currentSql->UpdataDevState(CurrectZone);
//#else
//    //    QString Info = global.currentSql->UpdataDevState(CurrectZone);
//    QString Info = "";
//#endif
//    if(Info == "")
//    {

//        QDialog *dialog = new QDialog(this);
//        dialog->setWindowTitle("TDCS/CTC3.0 通用车站操作终端");
//        dialog->setGeometry(800,500,150,85);
//        dialog->setStyleSheet("background-color: rgb(198, 210, 218);");
//        QLabel *label = new QLabel(dialog);
//        label->setGeometry(10,0,80,23);
//        label->setText("请输入密码：");
//        label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
//        QLineEdit *lineEdit = new QLineEdit(dialog);
//        lineEdit->setGeometry(10,25,130,23);
//        lineEdit->setStyleSheet("background-color: rgb(240, 240, 240);");
//        lineEdit->setEchoMode(QLineEdit::Password);
//        QPushButton *pushbutton = new QPushButton(dialog);
//        pushbutton->setText("关闭");
//        pushbutton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                                  "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
//                                  "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                                  "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                                  "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");
//        pushbutton->setGeometry(80,55,60,23);
//        QPushButton *pushbutton_OK = new QPushButton(dialog);
//        pushbutton_OK->setText("确定");
//        pushbutton_OK->setGeometry(10,55,60,23);
//        pushbutton_OK->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                                     "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
//                                     "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                                     "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                                     "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");

//        connect(pushbutton_OK,&QPushButton::clicked,[=](){
//            QString Str=lineEdit->text();
//            //            qDebug()<<Str;
//            if(Str=="1234")
//            {
//                DevOperFun(0x44);
//                IsonlyData = true;
//                UpdataTreelist(CurrectZone);
//                global.SenddataFun();
//                //                emit Signal_PLCZ_SendData_JF();
//            }
//        });
//        connect(pushbutton_OK,&QPushButton::clicked,dialog,&QDialog::close);
//        connect(pushbutton,&QPushButton::clicked,dialog,&QDialog::close);
//        dialog->exec();

//        //        DevOperFun(0x44);
//        //        IsonlyData = true;
//        //        UpdataTreelist(CurrectZone);
//        //        global.SenddataFun();
//        //        emit Signal_PLCZ_SendData_JF();

//    }
//    else
//    {
//        QMessageBox::warning(this, QString("命令下发失败"),
//                             Info);
//    }
//}


void stationshowfrom::onCustomContextMenuRequested(const QPoint &pos)
{
    if(CurrectZone == nullptr)
        return ;
    if(IsEditFlag==false)
    {
        return;
    }
    QModelIndex idx = ui->treeView->indexAt(pos);
    QPoint tmpPos;
    tmpPos.setX(pos.x());
    tmpPos.setY(pos.y() + 25);
    QString DevName =idx.data().toString();
    int Type = 0;

    GetCurrectDev = UpdataTreelist(DevName,&Type,CurrectZone);
    if(GetCurrectDev == nullptr)
        return ;
    if(Type == DCDev)
    {
        if(GetCurrectDev->OperStr == QString("单锁"))
        {
            DCDSaction->setChecked(true);
        }
        else
        {
            DCDSaction->setChecked(false);
        }
        if(GetCurrectDev->OperStr == QString("封锁"))
        {
            DCFSaction->setChecked(true);
        }
        else
        {
            DCFSaction->setChecked(false);
        }
        if(GetCurrectDev->OperStr == QString("单锁至定位"))
        {
            DCDSDaction->setChecked(true);
        }
        else
        {
            DCDSDaction->setChecked(false);
        }
        if(GetCurrectDev->OperStr == QString("单锁至反位"))
        {
            DCDSFaction->setChecked(true);
        }
        else
        {
            DCDSFaction->setChecked(false);
        }
        DCMenu->popup(ui->treeView->mapToGlobal(tmpPos));
    }
    else if(Type == QDDev)
    {
        if(GetCurrectDev->OperStr == QString("封"))
        {
            QDFSaction->setChecked(true);
        }
        else
        {
            QDFSaction->setChecked(false);
        }

        QDMenu->popup(ui->treeView->mapToGlobal(tmpPos));
    }
    else if(Type == QJDev)
    {
        if(GetCurrectDev->OperStr == QString("封"))
        {
            QJFSaction->setChecked(true);
        }
        else
        {
            QJFSaction->setChecked(false);
        }

        QJMenu->popup(ui->treeView->mapToGlobal(tmpPos));
    }
    else if(Type == XHDev)
    {
        if(GetCurrectDev->Devname.indexOf("D") == -1)
            XHFSaction->setText(QString("封(列)封(调)"));
        else
            XHFSaction->setText(QString("封(调)"));
        if(GetCurrectDev->OperStr.indexOf(QString("封"))!=-1)
        {
            XHFSaction->setChecked(true);
        }
        else
        {
            XHFSaction->setChecked(false);
        }

        XHMenu->popup(ui->treeView->mapToGlobal(tmpPos));
    }
}
zonedev *stationshowfrom::UpdataTreelist(QString DevName,int *Type, zoneData *zone)
{
    for(unsigned int i = 0;i<zone->vect_dcDev.size();i++)
    {
        if(zone->vect_dcDev[i].Devname == DevName)
        {
            *Type = DCDev;
            return &zone->vect_dcDev[i];
        }
    }
    for(unsigned int i = 0;i<zone->vect_qdDev.size();i++)
    {
        if(zone->vect_qdDev[i].Devname == DevName)
        {
            *Type = QDDev;
            return &zone->vect_qdDev[i];
        }
    }
    for(unsigned int i = 0;i<zone->vect_qjDev.size();i++)
    {
        if(zone->vect_qjDev[i].Devname == DevName)
        {
            *Type = QJDev;
            return &zone->vect_qjDev[i];
        }
    }
    for(unsigned int i = 0;i<zone->vect_xhDev.size();i++)
    {
        if(zone->vect_xhDev[i].Devname == DevName)
        {
            *Type = XHDev;
            return &zone->vect_xhDev[i];
        }
    }

    return nullptr;
}

void stationshowfrom::onDCMenuTriggered(QAction *action)
{
    if(GetCurrectDev == nullptr || CurrectZone == nullptr)
        return;
    if(IsEditFlag==false)
    {
        return;
    }
    if (action == DCDSaction)
    {//单锁
        //       GetCurrectDev->OperStr = action->text();
        //       GetCurrectDev->StateStr = action->text();
        ////       global.currentSql->UpdataDevState(CurrectZone);
        ////       global.SenddataFun();
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr = action->text();
            GetCurrectDev->StateStr = action->text();
            DCDSaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x08,GetCurrectDev->m_nCode);
        }
        else if(GetCurrectDev->isChecked==true)
        {
            GetCurrectDev->OperStr = "解锁";
            GetCurrectDev->StateStr = "解锁";
            DCDSaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x09,GetCurrectDev->m_nCode);
        }
    }
    else if (action == DCDSDaction)
    {//单锁至定位
        //        GetCurrectDev->OperStr = action->text();
        //        GetCurrectDev->StateStr = action->text();
        ////        global.currentSql->UpdataDevState(CurrectZone);
        ////        global.SenddataFun();
        //        SendData(0x10,GetCurrectDev->m_nCode);
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr = action->text();
            GetCurrectDev->StateStr = action->text();
            DCDSDaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x08,GetCurrectDev->m_nCode);
        }
        else if(GetCurrectDev->isChecked==true)
        {
            GetCurrectDev->OperStr = "解锁";
            GetCurrectDev->StateStr = "解锁";
            DCDSDaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x09,GetCurrectDev->m_nCode);
        }
    }
    else if(action == DCDSFaction)
    {//单锁至反位
        //        GetCurrectDev->OperStr = action->text();
        //        GetCurrectDev->StateStr = action->text();
        ////        global.currentSql->UpdataDevState(CurrectZone);
        ////        global.SenddataFun();
        //        SendData(0x10,GetCurrectDev->m_nCode);
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr = action->text();
            GetCurrectDev->StateStr = action->text();
            DCDSFaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x08,GetCurrectDev->m_nCode);
        }
        else if(GetCurrectDev->isChecked==true)
        {
            GetCurrectDev->OperStr = "解锁";
            GetCurrectDev->StateStr = "解锁";
            DCDSFaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x09,GetCurrectDev->m_nCode);
        }
    }
    else if(action == DCFSaction)
    {//封锁
        //        GetCurrectDev->OperStr = action->text();
        //        GetCurrectDev->StateStr = action->text();
        ////        global.currentSql->UpdataDevState(CurrectZone);
        ////        global.SenddataFun();
        //        SendData(0x10,GetCurrectDev->m_nCode);
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr = action->text();
            GetCurrectDev->StateStr = action->text();
            DCFSaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x10,GetCurrectDev->m_nCode);
        }
        else if(GetCurrectDev->isChecked==true)
        {
            GetCurrectDev->OperStr = "解锁";
            GetCurrectDev->StateStr = "解锁";
            DCFSaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x11,GetCurrectDev->m_nCode);
        }
    }
    else if(action == DCDeleteaction)
    {//删除设备
        DeleteDevFun(GetCurrectDev->Devname,CurrectZone);
        //       global.currentSql->LoadZoneData(CurrectZone);
    }
    //    DevOperFun(0x33);
    IsonlyData = true;
    UpdataTreelist(CurrectZone);
    //global.SenddataFun();
    //    emit Signal_PLCZ_SendData();
    //    UpdataTreelist(CurrectZone);
    DevOperFun_SIN();
}

void stationshowfrom::onQDMenuTriggered(QAction *action)
{
    if(GetCurrectDev == nullptr || CurrectZone == nullptr)
        return;
    if(IsEditFlag==false)
    {
        return;
    }
    if (action == QDFSaction)
    {//封锁
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr =action->text();
            GetCurrectDev->StateStr = action->text();
            //        global.currentSql->UpdataDevState(CurrectZone);
            //        global.SenddataFun();
            QDFSaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x10,GetCurrectDev->m_nCode);
        }
        else
        {
            GetCurrectDev->OperStr = "解封";
            GetCurrectDev->StateStr = "解封";
            QDFSaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x11,GetCurrectDev->m_nCode);
        }

    }
    else if (action == QDDeleteaction)
    {//删除设备
        DeleteDevFun(GetCurrectDev->Devname,CurrectZone);
        //        global.currentSql->LoadZoneData(CurrectZone);
    }
    IsonlyData = true;
    UpdataTreelist(CurrectZone);
    //global.SenddataFun();
    //    emit Signal_PLCZ_SendData();
    //    UpdataTreelist(CurrectZone);
    DevOperFun_SIN();
}

void stationshowfrom::onQJMenuTriggered(QAction *action)
{
    if(IsEditFlag==false)
    {
        return;
    }
    if (action == QJFSaction)
    {//封锁
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr = action->text();
            GetCurrectDev->StateStr = action->text();
            //        global.currentSql->UpdataDevState(CurrectZone);
            //        global.SenddataFun();
            QJFSaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x10,GetCurrectDev->m_nCode);
        }
        else
        {
            GetCurrectDev->OperStr = "解封";
            GetCurrectDev->StateStr = "解封";
            //        global.currentSql->UpdataDevState(CurrectZone);
            //        global.SenddataFun();
            QJFSaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x11,GetCurrectDev->m_nCode);
        }

    }
    else if (action == QJDeleteaction)
    {//删除设备
        DeleteDevFun(GetCurrectDev->Devname,CurrectZone);
        //        global.currentSql->LoadZoneData(CurrectZone);
    }
    IsonlyData = true;
    UpdataTreelist(CurrectZone);
    //global.SenddataFun();
    //    emit Signal_PLCZ_SendData();
    //    UpdataTreelist(CurrectZone);
    DevOperFun_SIN();
}

void stationshowfrom::onXHMenuTriggered(QAction *action)
{
    if(IsEditFlag==false)
    {
        return;
    }
    if (action == XHFSaction)
    {//封锁
        if(GetCurrectDev->isChecked==false)
        {
            GetCurrectDev->OperStr =action->text();
            GetCurrectDev->StateStr = action->text();
            XHFSaction->setChecked(true);
            GetCurrectDev->isChecked=true;
            GetCurrectDev->IsselectFlag=true;
            SendData(0x10,GetCurrectDev->m_nCode);
        }
        else
        {
            GetCurrectDev->OperStr = "解封";
            GetCurrectDev->StateStr = "解封";
            XHFSaction->setChecked(false);
            GetCurrectDev->isChecked=false;
            GetCurrectDev->IsselectFlag=false;
            SendData(0x11,GetCurrectDev->m_nCode);
        }

    }
    else if (action == XHDeleteaction)
    {//删除设备
        DeleteDevFun(GetCurrectDev->Devname,CurrectZone);
    }
    IsonlyData = true;
    UpdataTreelist(CurrectZone);
    //global.SenddataFun();
    DevOperFun_SIN();
}
void stationshowfrom::DeleteDevFun(QString DevName, zoneData *zone)
{
    if(IsEditFlag==false)
    {
        return;
    }
    for(unsigned int i = 0;i<zone->vect_dcDev.size();i++)
    {
        if(zone->vect_dcDev[i].Devname == DevName)
        {
            zone->vect_dcDev.erase(zone->vect_dcDev.begin()+i);
            return ;
        }
    }
    for(unsigned int i = 0;i<zone->vect_qdDev.size();i++)
    {
        if(zone->vect_qdDev[i].Devname == DevName)
        {
            zone->vect_qdDev.erase(zone->vect_qdDev.begin()+i);
            return ;
        }
    }
    for(unsigned int i = 0;i<zone->vect_qjDev.size();i++)
    {
        if(zone->vect_qjDev[i].Devname == DevName)
        {
            zone->vect_qjDev.erase(zone->vect_qjDev.begin()+i);
            return ;
        }
    }
    for(unsigned int i = 0;i<zone->vect_xhDev.size();i++)
    {
        if(zone->vect_xhDev[i].Devname == DevName)
        {
            zone->vect_xhDev.erase(zone->vect_xhDev.begin()+i);
            return ;
        }
    }
}

void stationshowfrom::SendData(unsigned char FUNType/*功能类型*/,unsigned int FUNSet_DeviceNum/*功能设置_设备编号*/)
{
    char SendArray[40]={0};//设备状态数据数组
    unsigned char SendArray1[40]={0};
    unsigned int frame_number;//帧长度
    SendArray1[0]=SendArray1[1]=SendArray1[2]=SendArray1[3]=0xEF;//帧头
    //    SendArray1[4]=0xFF;//帧长度
    //    SendArray1[5]=0xFF;
    SendArray1[6]=0x01;//本站地址码//本站ID
    SendArray1[7]=0x02;//目标地址码//预留
    SendArray1[8]=0xCD;//数据标志//0xAA联锁，0xBB教师机，0xCC为CTC，0xDD为集控，0xCD占线板，0xAB培训软件
    SendArray1[9]=0x88;
    SendArray1[10]=FUNType;//功能类型_供电臂设置
    memcpy(SendArray1+11,&FUNSet_DeviceNum,2);//设备号
    SendArray1[13]=0x00;
    SendArray1[14]=SendArray1[15]=SendArray1[16]=SendArray1[17]=0xFE;
    frame_number=18;
    memcpy(SendArray1+4,&frame_number,2);//帧长度
    memcpy(SendArray,SendArray1,18);
    //udpSocket->writeDatagram(SendArray,18,QHostAddress(IP_OppAdress),IP_OppPort);
}

void stationshowfrom::on_comboBox_currentTextChanged(const QString &arg1)
{
    UpdataTreelist_Save(CurrectZone);
    emit Signal_Refresh_ZC();

}

void stationshowfrom::on_refresh__btn_clicked()
{
    //emit Signal_Refresh_ZC();
    InitInfo(CurrectZone);
    itemList.clear();
}

void stationshowfrom::on_Confirmbtn_clicked()
{

}
void stationshowfrom::GetGDInfo(QString Name){

    QStringList tempList;
    for(int i = 0;i<ui->stationweight->zcArray.size();++i)
    {
        if(ui->stationweight->zcArray[i]->StationName == Name)
        {
            for(int ii = 0; ii <  ui->stationweight->zcArray[i]->GDArray.size();++ii)
            {
                if(ui->stationweight->zcArray[i]->GDArray[ii]->m_nGDType == GD_QD)
                {
                    GD_ L;
                    L.GDName = ui->stationweight->zcArray[i]->GDArray[ii]->m_strName;
                    L.GDIndex = ui->stationweight->zcArray[i]->GDArray[ii]->m_nCode;
                    DBSERVICES.info.GDL.push_back(L);
                }
//                  DBSERVICES.info.GDList.append(ui->stationweight->zcArray[i]->GDArray[ii]->m_strName);
            }
            for(int ii = 0; ii < ui->stationweight->zcArray[i]->XHDArray.size();++ii)
            {
                if(ui->stationweight->zcArray[i]->XHDArray[ii]->m_nXHDType == JZ_XHJ)
                {
                    Jc_XHJ jc;
                    jc.JCName = ui->stationweight->zcArray[i]->XHDArray[ii]->m_strName;
                    jc.JCIndex = ui->stationweight->zcArray[i]->XHDArray[ii]->m_nCode;
                    DBSERVICES.info.JCL.push_back(jc);
                }
                     //DBSERVICES.info.jcList.append(ui->stationweight->zcArray[i]->XHDArray[ii]->m_strName);
                else if (ui->stationweight->zcArray[i]->XHDArray[ii]->m_nXHDType == CZ_XHJ)
                {
                    Fc_XHJ Fc;
                    Fc.FCName = ui->stationweight->zcArray[i]->XHDArray[ii]->m_strName;
                    Fc.FCIndex = ui->stationweight->zcArray[i]->XHDArray[ii]->m_nCode;
                     DBSERVICES.info.FCL.push_back(Fc);
                }
                      //DBSERVICES.info.FcList.append(ui->stationweight->zcArray[i]->XHDArray[ii]->m_strName);
            }
        }
    }
}

