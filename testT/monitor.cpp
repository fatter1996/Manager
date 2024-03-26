#include"monitor.h"
#include "ui_monitor.h"
#include "TGlobal.h"
#include <QtDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
#include <QMessageBox>
#include <QTimer>
#include <QDir>
#include <QMenu>

Monitor::Monitor(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Monitor)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Tool);
    this->move(0,0);

    //dataAcquisition(); //读取数据库数据并初始化站场基础数据
    //    QString FileName = QString::fromLocal8Bit("../data/station.txt");
    QString FileName = QString::fromLocal8Bit("./station.txt");
    //StationRead(FileName);//读取本地数据文件初始化

    ui->centralwidget_ZC->setMouseTracking(true);
    setMouseTracking(true);

    /*通过属性区别进路*/
    CurrentExamID = -1;

    UpdateTimer = new QTimer(this);
    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(UPdate()));
    //    UpdateTimer->start(500);//开启刷新计时器

    JLProcTimer = new QTimer(this);
    connect(JLProcTimer, SIGNAL(timeout()), this, SLOT(JLProc()));
    //    JLProcTimer->start(1000);//开启刷新计时器
    nDiploid = 1;
    nElapsed = 0;

    ANProcTimer = new QTimer(this);
    connect(ANProcTimer, SIGNAL(timeout()), this, SLOT(ANProc()));
    //    ANProcTimer->start(3000);//开启刷新计时器

    nDiploid = 1;
    nElapsed = 0;
    showType = 0;
    sizeType = 0;
    FCZKFlag = false;
    CZState=false;
    QDList_SAVE.clear();
    QJList_SAVE.clear();
    DCList_SAVE.clear();
    XHList_SAVE.clear();

    IsstationLoadFlag = false;
    ResetRectSelect();
    //    IsEditFlag = false;
    IsEditFlag = true;

}

Monitor::Monitor(int ExamID, unsigned char show, unsigned char size, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Monitor)
{
    ui->setupUi(this);

    //dataAcquisition(); //读取数据库数据并初始化站场基础数据
    //    QString FileName = QString::fromLocal8Bit("../data/station.txt");
    QString FileName = QString::fromLocal8Bit("./station.txt");
    //StationRead(FileName);//读取本地数据文件初始化

    ui->centralwidget_ZC->setMouseTracking(true);
    setMouseTracking(true);

    //    /*通过属性区别进路*/
    ////    CurrentExamID = ExamID;
    ////    for (int i = 0; i < station->v_JL.size(); i++)
    ////    {
    ////        if (station->v_JL[i].ExamID == CurrentExamID)
    ////        {
    ////            station->CurrentJL = station->v_JL[i];
    ////            break;
    ////        }
    ////    }

    //    UpdateTimer = new QTimer(this);
    //    connect(UpdateTimer, SIGNAL(timeout()), this, SLOT(UPdate()));
    ////    UpdateTimer->start(500);//开启刷新计时器

    //    JLProcTimer = new QTimer(this);
    //    connect(JLProcTimer, SIGNAL(timeout()), this, SLOT(JLProc()));
    ////    JLProcTimer->start(1000);//开启刷新计时器
    //    nDiploid = 1;
    //    nElapsed = 0;

    //    ANProcTimer = new QTimer(this);
    //    connect(ANProcTimer, SIGNAL(timeout()), this, SLOT(ANProc()));
    ////    ANProcTimer->start(3000);//开启刷新计时器

    //    nDiploid = 1;
    //    nElapsed = 0;
    //    showType = 0;
    //    sizeType = 0;
    //    FCZKFlag = false;
    //    CZState=false;

    ////    showType = show;
    ////    sizeType = size;
    ////    if (show == 0 && size == 2)
    ////        FCZKFlag = false;
    ////    else
    ////        FCZKFlag = true;
}
void Monitor::InitScene(int TitleID, unsigned char show, unsigned char size)
{
    /*通过属性区别进路*/
    station->CancelCurrentJL();
    station->ClearScene();
    CurrentExamID = TitleID;
    for (int i = 0; i < station->v_JL.size(); i++)
    {
        if (station->v_JL[i].ExamID == CurrentExamID)
        {
            station->CurrentJL = station->v_JL[i];
            break;
        }
    }
    showType = show;
    sizeType = size;
    if (show == 0 && size == 2)
        FCZKFlag = false;
    else
        FCZKFlag = true;
}

void Monitor::ClearStatesName(QString Type, QString Name)
{

    if(tempStation==nullptr)
        return;

    if (Type == "股道/无岔" || Type == "区间")
    {
        for (int i=0;i<tempStation->GDArray.count();i++)
        {
            if(tempStation->GDArray[i]->m_strName == Name)
                tempStation->GDArray[i]->GDisPLCZ=false;
        }
    }
    else if (Type == "道岔")
    {
        for (int i=0;i<tempStation->GDDCArray.count();i++)
        {
            if(tempStation->GDDCArray[i]->m_strName == Name)
                tempStation->GDDCArray[i]->DCisPLCZ=false;
        }
    }
    this->update();
    //    for (int i=0;i<zcArray[0]->GDArray.count();i++)
    //    {
    //        T_GD=zcArray[0]->GDArray[i];
    //        T_GD->GDisPLCZ=false;
    //    }
}

void Monitor::ClearAll()
{
    if(tempStation==nullptr)
        return;
    for (int i=0;i<tempStation->GDArray.count();i++)
    {
        tempStation->GDArray[i]->GDisPLCZ=false;
    }
    for (int i=0;i<tempStation->GDDCArray.count();i++)
    {
        tempStation->GDDCArray[i]->DCisPLCZ=false;
    }
    this->update();
}
Monitor::~Monitor()
{
    delete ui;
}
void Monitor::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPen pen; //画笔

    pen.setColor(QColor(0, 0, 0));
    QBrush brush(QColor(0, 0, 0, 255)); //画刷

    //painter.scale(0.6, 0.6);
    painter.setPen(pen); //添加画笔
    painter.setBrush(brush); //添加画刷
    painter.setRenderHint(QPainter::Antialiasing);
    //    painter.drawRect(geometry());
    //this->setWindowFlags(Qt::FramelessWindowHint);                //取消标题栏

    painter.drawRect(0, 0, width(), height()); //绘制矩形
    //    if (showType == 0 && sizeType == 2)
    //    {
    //        ui->QGJButton->setHidden(false);
    //        ui->SYDButton->setHidden(false);
    //        ui->PDJSButton->setHidden(false);
    //        ui->ZDWButton->setHidden(false);
    //        ui->ZFWButton->setHidden(false);
    //        ui->ZQXButton->setHidden(false);
    //        ui->FCZKButton->setHidden(false);
    //        ui->ZRJButton->setHidden(false);
    //    }
    //    else if (showType == 0)
    //    {
    //        ui->QGJButton->setHidden(true);
    //        ui->SYDButton->setHidden(true);
    //        ui->PDJSButton->setHidden(true);
    //        ui->ZDWButton->setHidden(true);
    //        ui->ZFWButton->setHidden(true);
    //        ui->ZQXButton->setHidden(true);
    //        ui->FCZKButton->setHidden(true);
    //        ui->ZRJButton->setHidden(true);
    //    }
    if (sizeType == 0)
    {
        //        setFixedSize(int(1500), int(858));
        setFixedSize(int(1920), int(1010));
        painter.translate(0, 100);
        m_Transform = painter.transform();
        //        ui->QGJButton->setHidden(true);
        //        ui->SYDButton->setHidden(true);
        //        ui->PDJSButton->setHidden(true);
        //        ui->ZDWButton->setHidden(true);
        //        ui->ZFWButton->setHidden(true);
        //        ui->ZQXButton->setHidden(true);
        //        ui->FCZKButton->setHidden(true);
        //        ui->ZRJButton->setHidden(true);
    }
    else if (sizeType == 1)
    {
        setFixedSize(int(3000), int(3000));
        painter.translate(0, 100);
        painter.scale(1.1,1.1);
        m_Transform = painter.transform();
    }
    else if (sizeType == 2)
    {
        setFixedSize(int(740), int(1080));
        painter.translate(0, 200);
        m_Transform = painter.transform();
    }
    else if (sizeType == 3)
    {
        setFixedSize(int(800), int(600));
        painter.translate(0, 50);
        m_Transform = painter.transform();
        //        if (showType == 0)
        //            ui->FCZKButton->setHidden(true);
    }
    else if (sizeType == 4)
    {
        setFixedSize(int(1800), int(350));
        painter.translate(0, 50);
        m_Transform = painter.transform();
    }

    BaseRectDraw(&painter,1,QColor(220, 220, 220),QColor(5, 5, 120), SelectRect);
    for (int i = 0; i < zcArray.size(); i++) //绘制站场界面
    {
        if(globals.SelectStationName == zcArray[i]->StationName)
        {
            tempStation = zcArray[i];
            break;
        }

    }
    if(tempStation!=nullptr)
        tempStation->drawStation(&painter, nElapsed, 1, showType);
    painter.end();
    this->update();


}
void Monitor::dataAcquisition()
{
    connectionName = "StationDev_V1.0.0.00.db"; //数据库名
    m_sqliteDB = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", connectionName));
    m_sqliteDB->setDatabaseName("StationDev_V1.0.0.00.db");
    if (!m_sqliteDB->open())
    {
        qFatal("failed to connect.") ;
    }
    else
    {
        QSqlQuery sql_query(*m_sqliteDB);
        sql_query.exec("SELECT * FROM Station_List");
        while (sql_query.next())
        {
            QString stationID = "";
            TStation *pStation = new TStation;
            stationID = sql_query.value(1).toString(); //临时存储stationID字符串，用于读取设备表时比较
            pStation->setStationID(HexStringToInt(sql_query.value(1).toString(), sql_query.value(1).toString().length()));
            pStation->setStationName(sql_query.value(2).toString());
            pStation->setStationType(HexStringToInt(sql_query.value(3).toString(), sql_query.value(3).toString().length()));
            pStation->setLSProducer(sql_query.value(4).toString());
            pStation->setIsGT(sql_query.value(5).toBool());
            pStation->setIsZCZX(sql_query.value(6).toBool());
            pStation->setRBCCode(sql_query.value(7).toInt());
            pStation->setOffSet(StringToPoint(sql_query.value(8).toString()));
            //读取文本表Dev_TEXT_List
            QSqlQuery sql_query_Dev(*m_sqliteDB);
            QString strSelect = QString("SELECT * FROM Dev_TEXT_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                T_Text *pText = new T_Text;
                pText->setStationID(HexStringToInt(sql_query_Dev.value(1).toString(), sql_query_Dev.value(1).toString().length()));
                pText->setTextName(sql_query_Dev.value(2).toString());
                pText->setTextType(sql_query_Dev.value(3).toString());
                if (pText->getTextType() == DCQDNAMETEXT)
                {
                    pText->setGLQDNode(sql_query_Dev.value(4).toInt());
                }
                pText->setTextSize(sql_query_Dev.value(5).toInt());
                pText->setTextColor(sql_query_Dev.value(6).toString());
                pText->setTextRect(StringToRect(sql_query_Dev.value(7).toString()));
                //qDebug()<<pText->getStationID()<<pText->getTextName()<<pText->getTextType()<<pText->getTextSize()<<pText->getTextColor()<<pText->m_textRect;
                pText->TextInit();
                pStation->TextArray.append(*pText);
            }

            //读取车挡表Dev_JTX_List
            strSelect = QString("SELECT * FROM Dev_JTX_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                T_jtx *jtx = new T_jtx();
                jtx->setStationID(HexStringToInt(sql_query_Dev.value(1).toString(), sql_query_Dev.value(1).toString().length()));
                jtx->setType(sql_query_Dev.value(2).toInt());
                jtx->setJTXName(sql_query_Dev.value(3).toString());
                jtx->setJTXType(sql_query_Dev.value(4).toInt());
                jtx->setp1(StringToPoint(sql_query_Dev.value(5).toString()));
                jtx->setp2(StringToPoint(sql_query_Dev.value(6).toString()));
                jtx->setp3(StringToPoint(sql_query_Dev.value(7).toString()));
                jtx->setp4(StringToPoint(sql_query_Dev.value(8).toString()));
                jtx->setpCenter(StringToPoint(sql_query_Dev.value(9).toString()));
                //qDebug()<<jtx->getStationID()<<jtx->getJTXName()<<jtx->getJTXType()<<jtx->p1<<jtx->p2<<jtx->p3<<jtx->p4<<jtx->pCenter;
                jtx->JTXInit();
                pStation->JTXArray.append(*jtx);
            }

            //读取轨道表Dev_GD_List
            strSelect = QString("SELECT * FROM Dev_GD_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                T_GD *gd = new T_GD();
                gd->setStationID(HexStringToInt(sql_query_Dev.value(1).toString(), sql_query_Dev.value(1).toString().length()));
                gd->setType(sql_query_Dev.value(2).toInt());
                gd->setGDName(sql_query_Dev.value(3).toString());
                gd->setCode(sql_query_Dev.value(4).toInt());
                gd->setGDType(sql_query_Dev.value(5).toString());
                gd->setSX(sql_query_Dev.value(6).toInt());
                gd->setZ(sql_query_Dev.value(7).toInt());
                gd->setTextRect(StringToRect(sql_query_Dev.value(8).toString()));
                gd->setp1(StringToPoint(sql_query_Dev.value(9).toString()));
                gd->setp2(StringToPoint(sql_query_Dev.value(10).toString()));
                gd->setp3(StringToPoint(sql_query_Dev.value(11).toString()));
                gd->setp4(StringToPoint(sql_query_Dev.value(12).toString()));
                gd->setp12(StringToPoint(sql_query_Dev.value(13).toString()));
                gd->setp34(StringToPoint(sql_query_Dev.value(14).toString()));
                gd->setpz12(StringToPoint(sql_query_Dev.value(15).toString()));
                gd->setpz34(StringToPoint(sql_query_Dev.value(16).toString()));
                gd->setpCenter(StringToPoint(sql_query_Dev.value(17).toString()));
                gd->setGLB_QDleft(StringGLBToInt(sql_query_Dev.value(18).toString()));
                gd->setGLB_QDright(StringGLBToInt(sql_query_Dev.value(19).toString()));
                gd->setIsSXDC(sql_query_Dev.value(20).toBool());
                gd->setIsZXGD(sql_query_Dev.value(21).toBool());
                gd->setIsCXGD(sql_query_Dev.value(22).toBool());
                gd->setIsJJGD(sql_query_Dev.value(23).toBool());
                gd->setGD_LCode(sql_query_Dev.value(24).toInt());
                gd->setGD_RCode(sql_query_Dev.value(25).toInt());
                gd->setCarrierFrequency(sql_query_Dev.value(26).toString());
                gd->setDir_DMH(sql_query_Dev.value(27).toString());
                gd->GDInit();
                //qDebug()<<gd->getStationID()<<gd->getGDName()<<gd->getGDType()<<gd->p1<<gd->p2<<gd->p3<<gd->p4<<gd->pCenter<<gd->getGLB_QDleft()<<gd->getGLB_QDright()<<gd->getGD_LCode()<<gd->getCarrierFrequency();
                pStation->GDArray.append(gd);
            }

            //读取信号表Dev_XHD_List
            strSelect = QString("SELECT * FROM Dev_XHD_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                T_XHD *xhd = new T_XHD();
                xhd->setStationID(HexStringToInt(sql_query_Dev.value(1).toString(), sql_query_Dev.value(1).toString().length()));
                xhd->setType(sql_query_Dev.value(2).toInt());
                xhd->setXHDName(sql_query_Dev.value(3).toString());
                xhd->setCode(sql_query_Dev.value(4).toInt());
                xhd->setXHDType(sql_query_Dev.value(5).toString());
                xhd->setSX(sql_query_Dev.value(6).toInt());
                xhd->setSignalType(sql_query_Dev.value(7).toBool());
                xhd->setTextRect(StringToRect(sql_query_Dev.value(8).toString()));
                xhd->setIsHigh(sql_query_Dev.value(9).toBool());
                xhd->setIsYDSD(sql_query_Dev.value(10).toBool());
                xhd->setSafeLamp(sql_query_Dev.value(11).toString());
                xhd->setLCAN_Rect(StringToRect(sql_query_Dev.value(12).toString()));
                xhd->setDCAN_Rect(StringToRect(sql_query_Dev.value(13).toString()));
                xhd->setYDAN_Rect(StringToRect(sql_query_Dev.value(14).toString()));
                xhd->setSignalDCAN(sql_query_Dev.value(15).toBool());
                xhd->setGLB_XHD(StringGLBToInt(sql_query_Dev.value(16).toString()));
                if ((xhd->getXHDType() == YG_XHJ) || (xhd->getXHDType() == JLFS_XHJ) || (xhd->getXHDType() == JZFS_XHJ) || (xhd->getXHDType() == DCFS_XHJ))
                {
                    xhd->setGLXHNode(sql_query_Dev.value(17).toInt());
                }
                xhd->setDir_XH(sql_query_Dev.value(18).toString());
                xhd->setXH_LCode(sql_query_Dev.value(19).toInt());
                xhd->setXH_RCode(sql_query_Dev.value(20).toInt());
                xhd->setisHDDSJC(sql_query_Dev.value(21).toBool());
                xhd->setXH_WZInfo(sql_query_Dev.value(22).toString());
                xhd->setXH_Show(sql_query_Dev.value(23).toInt());
                xhd->setXH_LCSD(sql_query_Dev.value(24).toBool());
                xhd->setXH_LCZD(sql_query_Dev.value(25).toBool());
                xhd->setXH_DCSD(sql_query_Dev.value(26).toBool());
                xhd->setXH_DCZD(sql_query_Dev.value(27).toBool());
                xhd->setXH_YXSD(sql_query_Dev.value(28).toBool());
                xhd->setXH_YXZD(sql_query_Dev.value(29).toBool());
                xhd->setXH_CYFX(sql_query_Dev.value(30).toBool());
                xhd->setXH_BGAN(sql_query_Dev.value(31).toBool());
                xhd->setpCenter(StringToPoint(sql_query_Dev.value(32).toString()));
                xhd->setXH_PDJZ(sql_query_Dev.value(33).toBool());
                xhd->setXH_LCZH(sql_query_Dev.value(34).toBool());
                xhd->XHDInit();
                //qDebug()<<xhd->getStationID()<<xhd->getXHDName()<<xhd->getXHDType()<<xhd->pCenter<<xhd->m_textRect<<xhd->getDir_XH()<<xhd->getXH_Show()<<xhd->getXH_WZInfo()<<xhd->getXH_LCZH()<<xhd->getXH_DCZD()<<xhd->getGLB_XHD();
                pStation->XHDArray.append(xhd);
            }

            //读取道岔表Dev_DC_List
            strSelect = QString("SELECT * FROM Dev_DC_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                //                T_GDDC *gddc = new T_GDDC;
                //                T_GDDC *gddc = new T_GDDC();
                //                gddc->setStationID(HexStringToInt(sql_query_Dev.value(1).toString(), sql_query_Dev.value(1).toString().length()));
                //                gddc->setType(sql_query_Dev.value(2).toInt());
                //                gddc->setGDDCName(sql_query_Dev.value(3).toString());
                //                gddc->setCode(sql_query_Dev.value(4).toInt());
                //                gddc->setQDCode(sql_query_Dev.value(5).toInt());
                //                gddc->setCxjy(sql_query_Dev.value(6).toInt());
                //                gddc->setJyj(sql_query_Dev.value(7).toInt());
                //                gddc->setIsSDDC(sql_query_Dev.value(8).toBool());
                //                if (gddc->getIsSDDC() == true)
                //                {
                //                    gddc->setSDDCCode(sql_query_Dev.value(9).toInt());
                //                }
                //                gddc->setZ(sql_query_Dev.value(10).toInt());
                //                gddc->setSX(sql_query_Dev.value(11).toInt());
                //                gddc->setOneToMore(sql_query_Dev.value(12).toInt());
                //                gddc->setTextRect(StringToRect(sql_query_Dev.value(13).toString()));
                //                gddc->setp1(StringToPoint(sql_query_Dev.value(14).toString()));
                //                gddc->setp2(StringToPoint(sql_query_Dev.value(15).toString()));
                //                gddc->setp3(StringToPoint(sql_query_Dev.value(16).toString()));
                //                gddc->setp4(StringToPoint(sql_query_Dev.value(17).toString()));
                //                gddc->setp5(StringToPoint(sql_query_Dev.value(18).toString()));
                //                gddc->setp6(StringToPoint(sql_query_Dev.value(19).toString()));
                //                gddc->setp12(StringToPoint(sql_query_Dev.value(20).toString()));
                //                gddc->setp34(StringToPoint(sql_query_Dev.value(21).toString()));
                //                gddc->setp56(StringToPoint(sql_query_Dev.value(22).toString()));
                //                gddc->setpCenter(StringToPoint(sql_query_Dev.value(23).toString()));
                //                gddc->setpz12(StringToPoint(sql_query_Dev.value(24).toString()));
                //                gddc->setpz34(StringToPoint(sql_query_Dev.value(25).toString()));
                //                gddc->setpz56(StringToPoint(sql_query_Dev.value(26).toString()));
                //                gddc->setIsMainGD(sql_query_Dev.value(27).toBool());
                //                gddc->setCQdev(sql_query_Dev.value(28).toInt());
                //                gddc->setDWdev(sql_query_Dev.value(29).toInt());
                //                gddc->setFWdev(sql_query_Dev.value(30).toInt());
                //                gddc->setSameQDDCCode(sql_query_Dev.value(31).toInt());
                //                gddc->setIsWGDW(sql_query_Dev.value(32).toBool());
                //                gddc->setGLB_DCCQ(StringGLBToInt(sql_query_Dev.value(33).toString()));
                //                gddc->setGLB_DCCX(StringGLBToInt(sql_query_Dev.value(34).toString()));
                //                gddc->setGLB_DCDW(StringGLBToInt(sql_query_Dev.value(35).toString()));
                //                gddc->setGLB_DCFW(StringGLBToInt(sql_query_Dev.value(36).toString()));
                //                gddc->setIsTSDC(sql_query_Dev.value(37).toBool());
                //                gddc->setCarrierFrequency(sql_query_Dev.value(38).toString());
                //                gddc->setDir_DMH(sql_query_Dev.value(39).toString());
                //                gddc->GDDCInit();
                //                //qDebug()<<gddc->getStationID()<<gddc->getGDDCName()<<gddc->getQDCode()<<gddc->pCenter<<gddc->m_textRect<<gddc->p1<<gddc->getGLB_DCCQ()<<gddc->getGLB_DCCX()<<gddc->getGLB_DCDW()<<gddc->getGLB_DCFW()<<gddc->getCarrierFrequency()<<gddc->getDir_DMH();
                //                pStation->GDDCArray.append(gddc);
            }

            //读取站内区段成员表Dev_QDChild_List
            strSelect = QString("SELECT * FROM Dev_QDChild_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                TStation::QD_Str *qd = new TStation::QD_Str();
                qd->m_nChildNum = sql_query_Dev.value(2).toInt();
                for (int index = 0; index < qd->m_nChildNum; index++)
                {
                    qd->m_nChild[index] = sql_query_Dev.value(3 + index).toInt();
                }
                //qDebug()<<qd->m_nChildNum<<qd->m_nChild[0]<<qd->m_nChild[1]<<qd->m_nChild[2]<<qd->m_nChild[3];
                pStation->QDArray.append(*qd);
            }

            //读取应答器设备表Dev_YDQ_List
            strSelect = QString("SELECT * FROM Dev_YDQ_List WHERE (m_nStationID like '%%1')").arg(stationID);
            sql_query_Dev.exec(strSelect);
            while (sql_query_Dev.next())
            {
                T_YDQ *ydq = new T_YDQ();
                ydq->setStationID(HexStringToInt(sql_query_Dev.value(1).toString(), sql_query_Dev.value(1).toString().length()));
                ydq->setType(sql_query_Dev.value(2).toInt());
                ydq->setYDQName(sql_query_Dev.value(3).toString());
                ydq->setCode(sql_query_Dev.value(4).toInt());
                ydq->setNumDQ(sql_query_Dev.value(5).toInt());
                ydq->setNumFQ(sql_query_Dev.value(6).toInt());
                ydq->setNumCZ(sql_query_Dev.value(7).toInt());
                ydq->setNumDY(sql_query_Dev.value(8).toInt());
                ydq->setNumZN(sql_query_Dev.value(9).toInt());
                ydq->setName_Record(sql_query_Dev.value(10).toString());
                ydq->setFuncType(sql_query_Dev.value(11).toString());
                ydq->setGLQDNode(sql_query_Dev.value(12).toInt());
                ydq->setPYL(sql_query_Dev.value(13).toInt());
                ydq->setYDQType(sql_query_Dev.value(14).toString());
                ydq->setGLB_YDQ(StringGLBToInt(sql_query_Dev.value(15).toString()));
                //qDebug()<<ydq->getYDQName()<<ydq->getName_Record()<<ydq->getFuncType()<<ydq->getGLQDNode()<<ydq->getYDQType()<<ydq->getGLB_YDQ();
                pStation->YDQArray.append(*ydq);
            }

            pStation->stationInit();
            zcArray.append(pStation);
        }
    }
    m_sqliteDB->close();
}
int Monitor::HexStringToInt(QString str, int length)
{
    int   num[16] = {0};
    int   count = 1;
    int   result = 0;
    QByteArray ba = str.toLatin1();
    char *revstr = ba.data();
    for (int i = length - 1; i >= 0; i--)
    {
        if ((revstr[i] >= '0') && (revstr[i] <= '9'))
            num[i] = revstr[i] - 48; //字符0的ASCII值为48
        else if ((revstr[i] >= 'a') && (revstr[i] <= 'f'))
            num[i] = revstr[i] - 'a' + 10;
        else if ((revstr[i] >= 'A') && (revstr[i] <= 'F'))
            num[i] = revstr[i] - 'A' + 10;
        else
            num[i] = 0;
        result = result + num[i] * count;
        count = count * 16; //十六进制(如果是八进制就在这里乘以8)
    }
    return result;
}
QPoint Monitor::StringToPoint(QString str)
{
    QPoint pt;
    str.trimmed(); //去除字符串两侧空白字符
    QString text = str.mid(2, str.length() - 3);
    QStringList list = text.split(",");

    pt.setX(list[0].toInt());
    pt.setY(list[1].toInt());
    return pt;
}
QRect Monitor::StringToRect(QString str)
{
    QRect rect;
    str.trimmed(); //去除字符串两侧空白字符
    QString text = str.mid(2, str.length() - 3);
    QStringList list = text.split(",");

    rect.setLeft(list[0].toInt());
    rect.setTop(list[1].toInt());
    rect.setRight(list[2].toInt());
    rect.setBottom(list[3].toInt());
    return rect;
}
int Monitor::StringGLBToInt(QString str) //公里标字符串转换为米单位整型
{
    str.trimmed(); //去除字符串两侧空白字符
    QString text = str.mid(1, str.length() - 1);
    QStringList list = text.split("+");
    int Meters = ((list[0].toInt()) * 1000) + (list[1].toInt());
    return Meters;
}

void Monitor::setScene(int TitleID)
{
    station->setScene(TitleID);
}


void Monitor::StationRead(QString FileName)
{
    TStation *pStation = new TStation;
    pStation->StationName = FileName;
    QString MarkName = FileName;
    int Id = MarkName.remove("Station").toInt()-1;
    if(Id == 0)
        MarkName = "Station";
    else
        MarkName = QString("Station%1").arg(Id);
    QString fileName = QString("%1/data/%2.txt").arg(QCoreApplication::applicationDirPath()).arg(MarkName);
    QFile tFile(fileName);
    if (!tFile.exists())
    {
        QMessageBox::information(nullptr, "提示", "station文件不存在！");
        return;
    }
    if (!tFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream tText(&tFile);

    QString m_str{};
    QString layerName{};
    QString  Getstring{};
    tText.setCodec("UTF-8");
    int DCnum;//文件读取道岔数量
    int XHnum;//文件读取信号机数量
    int GDnum;//文件读取区段数量
    DCnum = 0;
    XHnum = 0;
    GDnum = 0;
    while (!tText.atEnd())
    {
        m_str = tText.readLine();
        if (m_str == "####")
        {
            break;
        }
        else if (m_str.left(5) == "##XH")
        {
            T_XHD *  pXHD = new T_XHD ;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();

                if (m_str.left(12) == "xhd.m_nType=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_nType = Getstring.toUInt();
                }
                else if (m_str.left(14) == "xhd.m_strName=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pXHD->m_strName = Getstring;
                }
                else if (m_str.left(12) == "xhd.m_nCode=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(16) == "xhd.Module_Code=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pXHD->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(11) == "xhd.center=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->pCenter = StringToPoint(Getstring);
                }
                else if (m_str.left(15) == "xhd.m_nXHDType=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->setXHDType(Getstring);
                }
                else if (m_str.left(15) == "xhd.m_textRect=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(10) == "xhd.m_nSX=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(15) == "xhd.SignalType=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->isSignalType = Getstring.toUInt();
                }
                else if (m_str.left(17) == "xhd.D_B_C_Signal=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pXHD->D_B_C_Signal=Getstring.toUInt();
                }
                else if (m_str.left(17) == "xhd.DC_LC_Signal=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pXHD->DC_LC_Signal=Getstring.toUInt();
                }
                else if (m_str.left(13) == "xhd.safeLamp=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->setSafeLamp(Getstring);
                }
                else if (m_str.left(11) == "xhd.isHigh=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    if (Getstring == "FALSE")
                    {
                        pXHD->isHigh = false;
                    }
                    else if (Getstring == "TURE")
                    {
                        pXHD->isHigh = true;
                    }
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pXHD->XHDInit();
                    pStation->XHDArray.push_back(pXHD);
                    // pXHD = nullptr;
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##DC")
        {



            T_GDDC * dc = new T_GDDC ;
            for (int j = 0;; j++)
            {

                m_str = tText.readLine();
                if (m_str.left(13) == "gddc.m_nType=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->setType(Getstring.toUInt());
                }
                else if (m_str.left(15) == "gddc.m_strName=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    dc->m_strName = Getstring;
                }
                else if (m_str.left(13) == "gddc.m_nCode=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(15) == "gddc.m_nQDCode=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nQDCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(13) == "gddc.m_nCxjy=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->setCxjy(Getstring.toUInt());
                }
                else if (m_str.left(15) == "gddc.m_nDSCode=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nSDDCCode = Getstring.toInt();
                }
                else if (m_str.left(17) == "gddc.Module_Code=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //T_GDDC->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(17) == "gddc.m_nQDMKCode=")
                {
                    //                    Getstring=m_str.mid(17,m_str.indexOf((";"),0)-m_str.indexOf(("="),0)-1);
                    //                    T_GDDC->m_nQDMKCode=HexStringToInt(Getstring);
                }
                else if (m_str.left(10) == "gddc.m_nZ=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nZ = Getstring.toUInt();
                }
                else if (m_str.left(11) == "gddc.m_nSX=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(12) == "gddc.m_nJyj=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nJyj = Getstring.toUInt();
                }
                else if (m_str.left(17) == "gddc.oneToMore=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nOneToMore = Getstring.toInt();
                }
                else if (m_str.left(16) == "gddc.m_textRect=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(8) == "gddc.p1=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p1 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gddc.p2=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p2 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gddc.p3=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p3 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gddc.p4=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p4 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gddc.p5=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p5 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gddc.p6=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p6 = StringToPoint(Getstring);
                }
                else if (m_str.left(9) == "gddc.p12=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p12 = StringToPoint(Getstring);
                }
                else if (m_str.left(9) == "gddc.p34=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p34 = StringToPoint(Getstring);
                }
                else if (m_str.left(9) == "gddc.p56=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->p56 = StringToPoint(Getstring);

                }
                else if (m_str.left(12) == "gddc.center=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->pCenter = StringToPoint(Getstring);
                }

                else if (m_str.left(10) == "gddc.pZ12=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->pz12 = StringToPoint(Getstring);

                }
                else if (m_str.left(10) == "gddc.pZ34=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->pz34 = StringToPoint(Getstring);
                }
                else if (m_str.left(10) == "gddc.pZ56=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //AfxMessageBox(T_GDDC->m_strName);
                    dc->pz56 = StringToPoint(Getstring);
                }
                else if (m_str.left(15) == "gddc.m_bMainGD=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_bDrawGD = Getstring.toUInt();
                }
                else if (m_str.left(10) == "gddc.m_nQ=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nQ = Getstring.toInt();
                }
                else if (m_str.left(10) == "gddc.m_nD=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nD = Getstring.toInt();
                }
                else if (m_str.left(10) == "gddc.m_nF=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    dc->m_nF = Getstring.toInt();
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    //DCArray.Add(Getstring.toUInt());
                    dc->GDDCInit();
                    dc->DCPowerSupply=1;
                    pStation->GDDCArray.push_back(dc);
                    break;
                }
            }
            //            delete T_GDDC;
            //            T_GDDC = nullptr;
        }
        else if (m_str.left(5) == "##GD")
        {

            T_GD *  gd = new T_GD;
            for (int j=0;;j++)
            {

                m_str = tText.readLine();
                if (m_str.left(11) == "gd.m_nType=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->m_nType = Getstring.toUInt();
                }
                else if (m_str.left(13) == "gd.m_strName=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    gd->m_strName = Getstring;

                }
                else if (m_str.left(11) == "gd.m_nCode=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(15) == "gd.Module_Code=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //T_GD->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(16) == "gd.GD_Type=GD_QD")
                {
                    gd->m_nGDType = GD_QD;
                }
                else if (m_str.left(16) == "gd.GD_Type=JJ_QD")
                {
                    gd->m_nGDType = JJ_QD;
                }
                else if (m_str.left(8) == "gd.m_nZ=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->m_nZ = Getstring.toUInt();
                }
                else if (m_str.left(9) == "gd.m_nSX=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(14) == "gd.m_textRect=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(6) == "gd.p1=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->p1 = StringToPoint(Getstring);
                }
                else if (m_str.left(6) == "gd.p2=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->p2 = StringToPoint(Getstring);
                }
                else if (m_str.left(6) == "gd.p3=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->p3 = StringToPoint(Getstring);
                }
                else if (m_str.left(6) == "gd.p4=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->p4 = StringToPoint(Getstring);
                }
                else if (m_str.left(7) == "gd.p12=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->p12 = StringToPoint(Getstring);
                }
                else if (m_str.left(7) == "gd.p34=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->p34 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gd.pz12=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->pz12 = StringToPoint(Getstring);
                }
                else if (m_str.left(8) == "gd.pz34=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->pz34 = StringToPoint(Getstring);
                }
                else if (m_str.left(10) == "gd.center=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    gd->pCenter = StringToPoint(Getstring);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    //GDArray.Add(Getstring.toUInt());
                    gd->GDInit();
                    //T_GD *T_GD_ =  &T_GD;
                    pStation->GDArray.push_back(gd);
                    //                    delete  T_GD;
                    //                    T_GD = nullptr;
                    break;
                }
            }

        }
        else if (m_str.left(5) == "##TX")
        {

            for (int j=0;;j++)
            {

                m_str = tText.readLine();
                if (m_str.left(14) == "txt.m_strName=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pText.m_strName = Getstring;
                }
                else if (m_str.left(12) == "txt.m_nSize=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pText.m_nSize = Getstring.toUInt();
                    if(pText.m_nSize>=30)
                        StationaName = pText.m_strName;
                    //                    else
                    //                        StationaName = pText.m_strName;
                }
                else if (m_str.left(15) == "txt.m_textRect=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pText.m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pText.TextInit();
                    pStation->TextArray.push_back(pText);
                    //delete  pText;
                    // pText = nullptr;

                    break;
                }
            }

        }
        else if (m_str.left(5) == "##BS")
        {
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(13) == "bs.m_strName=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pBS->m_strName=Getstring;
                }
                else if (m_str.left(11) == "bs.m_nCode=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->m_nCode=HexStringToInt(Getstring);
                }
                else if (m_str.left(11) == "bs.RelayQD=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pBS->RelayQD=Getstring;
                }
                else if (m_str.left(15) == "bs.Module_Code=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(14) == "bs.m_textRect=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->m_textRect=StringToRect(Getstring);
                    //AfxMessageBox((""));
                }
                else if (m_str.left(9) == "bs.m_nSX=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->m_nSX=Getstring.toUInt();
                }
                else if (m_str.left(8) == "bs.isXN=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->isVirtual=Getstring.toUInt();
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    break;
                }
            }
        }
        else if (m_str.left(9) == "##BuRect")
        {
            //CBuRect *pBuRect=new CBuRect;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(15) == "BuRect.m_nType=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBuRect->m_nType=Getstring.toUInt();
                }
                else if (m_str.left(17) == "BuRect.m_strName=")
                {
                    Getstring = m_str.mid(18, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pBuRect->m_strName=Getstring;
                }
                else if (m_str.left(15) == "BuRect.m_nCode=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBuRect->m_nCode=HexStringToInt(Getstring);
                }
                else if (m_str.left(10) == "BuRect.p1=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBuRect->p1=StringToPoint(Getstring);
                }
                else if (m_str.left(18) == "BuRect.m_textRect=")
                {
                    Getstring = m_str.mid(18, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBuRect->m_textRect=StringToRect(Getstring);
                    //AfxMessageBox((""));
                }
                else if (m_str.left(14) == "BuRect.m_nTZB=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBuRect->m_nTZB=HexStringToInt(Getstring);
                    //pBuRect->BuRInit();
                    //zcArray.Add(pBuRect);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##LS")
        {
            //CLS *pLS=new CLS;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(11) == "ls.m_nType=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->m_nType=Getstring.toUInt();
                }
                else if (m_str.left(11) == "ls.m_nCode=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->m_nCode=HexStringToInt(Getstring);
                }
                else if (m_str.left(16) == "ls.m_nModuleNum=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->m_nModuleNum=Getstring.toUInt();
                }
                else if (m_str.left(16) == "ls.m_nModule[0]=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->m_nModule[0]=HexStringToInt(Getstring);
                    //AfxMessageBox((""));
                }
                else if (m_str.left(12) == "ls.mName[0]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[0]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[1]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[1]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[2]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[2]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[3]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[3]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[4]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[4]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[5]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[5]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[6]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[6]=Getstring;
                }
                else if (m_str.left(12) == "ls.mName[7]=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pLS->mName[7]=Getstring;
                }
                else if (m_str.left(13) == "ls.mRsbjRect=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mRsbjRect=StringToRect(Getstring);
                }
                else if (m_str.left(13) == "ls.mZFdyRect=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mZFdyRect=StringToRect(Getstring);
                }
                else if (m_str.left(16) == "ls.mGdtdRect[0]=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mGdtdRect[0]=StringToRect(Getstring);
                }
                else if (m_str.left(16) == "ls.mGdtdRect[1]=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mGdtdRect[1]=StringToRect(Getstring);
                }
                else if (m_str.left(17) == "ls.mYdzsbRect[0]=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mYdzsbRect[0]=StringToRect(Getstring);
                }
                else if (m_str.left(17) == "ls.mYdzsbRect[1]=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mYdzsbRect[1]=StringToRect(Getstring);
                }
                else if (m_str.left(16) == "ls.mDsdsRect[0]=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mDsdsRect[0]=StringToRect(Getstring);
                }
                else if (m_str.left(16) == "ls.mDsdsRect[1]=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pLS->mDsdsRect[1]=StringToRect(Getstring);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    break;
                    //AfxMessageBox(("杂项"));
                }
            }
        }
        else if (m_str.left(5) == "##JTX")
        {

            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(13) == "jtx.JTX_Type=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.m_nJTXType = Getstring.toUInt();
                }
                else if (m_str.left(15) == "jtx.m_strName=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pJTX.m_strName = Getstring;
                }
                else if (m_str.left(12) == "jtx.m_nType=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.m_nType = Getstring.toUInt();
                }
                else if (m_str.left(7) == "jtx.p1=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.p1 = StringToPoint(Getstring);
                }
                else if (m_str.left(7) == "jtx.p2=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.p2 = StringToPoint(Getstring);
                }
                else if (m_str.left(7) == "jtx.p3=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.p3 = StringToPoint(Getstring);
                }
                else if (m_str.left(7) == "jtx.p4=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.p4 = StringToPoint(Getstring);
                }
                else if (m_str.left(11) == "jtx.center=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX.pCenter = StringToPoint(Getstring);

                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pStation->JTXArray.push_back(pJTX);
                    break;
                }
            }

        }
        else if (m_str.left(5) == "##QD")
        {

            m_str = tText.readLine();
            if (m_str.left(15) == "qd.m_nChildNum=")
            {
                Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                pQD.m_nChildNum = Getstring.toInt();
            }
            for (int j = 0; j < pQD.m_nChildNum; j++)
            {
                m_str = tText.readLine();
                QString n_str;
                n_str = QString::fromLocal8Bit("qd.m_nChild[%1]=").arg(j);
                if (m_str.left(15) == n_str)
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pQD.m_nChild[j] = Getstring.toUInt();
                }
            }
            pStation->QDArray.push_back(pQD);
        }
    }
    pStation->InitAllFactor();
    pStation->stationInit();
    zcArray.push_back(pStation);
    tFile.close();
    station = pStation;
}

void Monitor::mouseMoveEvent(QMouseEvent *event)
{
    //    TStation* m_station;
    //    T_XHD *xhd;
    //    int x = event->globalX();
    //    int y = event->globalY();
    //    QPoint pt = event->pos();
    //    pt = m_Transform.inverted().map(event->pos());
    //    int size = zcArray.size();

    //    station->m_Selected.Init();
    //    for (int i = 0; i < size; i++)
    //    {
    //        m_station = zcArray[i];
    //        for (int j = 0; j < m_station->XHDArray.size(); j++)
    //        {
    //            xhd = m_station->XHDArray[j];
    //            if (xhd->MouseMove(pt) == 1)
    //            {
    //                //station = &m_station;
    //                station->m_Selected.type = 0x11;
    //                station->m_Selected.selected = j;
    //            }
    //            else if (xhd->MouseMove(pt) == 2)
    //            {
    //                station->m_Selected.type = 0x22;
    //                station->m_Selected.selected = j;
    //            }
    //        }
    //    }
    //    if (station->m_Selected.type != 0 && station->m_Selected.selected != -1)
    //    {
    //        setCursor(Qt::PointingHandCursor);
    //    }
    //    else
    //    {
    //        setCursor(Qt::ArrowCursor);
    //    }

    if(!IsstationLoadFlag)
        return ;
    if(SelectFlag && IsEditFlag)
    {
        FtartPt = QPoint (event->x(),event->y());
        //       SelectDevFun();
        SelectRect = CountRectFun();
        this->update();
    }
}

void Monitor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        OnLButtonDown(event);
    }
    else if (event->button() == Qt::RightButton)
    {
        OnRButtonDown(event);
    }
}
void Monitor::OnLButtonDown(QMouseEvent *event)
{
    //    T_XHD *pXHD = nullptr;
    //    QString Record;
    //    if (station->m_Selected.type != 0 && station->m_Selected.selected != -1)
    //    {
    //        if ((station->m_Selected.type == 0x11) && (station->m_Selected.selected < station->XHDArray.size()))
    //        {
    //            pXHD = station->XHDArray[station->m_Selected.selected];
    //            if (!pXHD->getIsLCDown())
    //            {
    //                station->XHDArray[station->m_Selected.selected]->setIsLCDown(true);
    //                station->DeaLWithAN(station->XHDArray[station->m_Selected.selected]);
    //            }
    //            if (station->cancelFlag)
    //            {
    //                station->v_ANArray.clear();
    //                station->XHDArray[station->m_Selected.selected]->setIsLCDown(false);
    //                if ((station->CurrentJL.v_AN.size() > 0) && (pXHD == station->CurrentJL.v_AN[0]))
    //                {
    //                    station->CancelCurrentJL();
    //                    emit SendCancelCMD();
    //                }
    //                station->cancelFlag = false;
    //            }
    //            Record = QString("%1列车按钮按下\n#LCAN,%1").arg(pXHD->getXHDName());
    ////            DBSERVICE.UpdateOperationRecord(Record);
    //        }
    //        else if ((station->m_Selected.type == 0x22) && (station->m_Selected.selected < station->XHDArray.size()))
    //        {
    //            pXHD = station->XHDArray[station->m_Selected.selected];
    //            if (!pXHD->getIsDCDown())
    //            {
    //                station->XHDArray[station->m_Selected.selected]->setIsDCDown(true);
    //                station->DeaLWithAN(station->XHDArray[station->m_Selected.selected]);
    //            }
    //            if (station->cancelFlag)
    //            {
    //                station->v_ANArray.clear();
    //                station->XHDArray[station->m_Selected.selected]->setIsDCDown(false);
    //                station->cancelFlag = false;
    //            }
    //            Record = QString("%1调车按钮按下\n#DCAN,%1").arg(pXHD->getXHDName());
    ////            DBSERVICE.UpdateOperationRecord(Record);
    //        }
    //    }

    if(!IsstationLoadFlag)
        return ;
    //    if(6 == sizeType)//选取设备操作
    if(CZState)
    {
        if(IsEditFlag)
        {
            ResetRectSelect();
            StartPt = QPoint (event->x(),event->y());
            FtartPt = StartPt;
            SelectFlag  =true;

        }
    }
}

void Monitor::OnRButtonDown(QMouseEvent *event)
{
    if(!IsstationLoadFlag)
        return ;
    //    if(6 == sizeType)//设备操作
    if(CZState)
    {
        QMenu* menu= new QMenu(this);


        QAction* Action1 = new QAction(menu);
        QAction* Action2 = new QAction(menu);
        QAction* Action3 = new QAction(menu);
        //        if(zcArray[0]->QDNameFlag)
        //         Action1->setText(QString::fromLocal8Bit("隐藏区段名称"));
        //        else
        //         Action1->setText(QString::fromLocal8Bit("显示区段名称"));

        //        if(zcArray[0]->XHNameFlag)
        //         Action2->setText(QString::fromLocal8Bit("隐藏信号机名称"));
        //        else
        //         Action2->setText(QString::fromLocal8Bit("显示信号机名称"));

        //        if(zcArray[0]->DCNameFlag)
        //         Action3->setText(QString::fromLocal8Bit("隐藏道岔名称"));
        //        else
        //         Action3->setText(QString::fromLocal8Bit("显示道岔名称"));

        if(zcArray[0]->QDNameFlag)
            Action1->setText(QString("隐藏区段名称"));
        else
            Action1->setText(QString("显示区段名称"));

        if(zcArray[0]->XHNameFlag)
            Action2->setText(QString("隐藏信号机名称"));
        else
            Action2->setText(QString("显示信号机名称"));

        if(zcArray[0]->DCNameFlag)
            Action3->setText(QString("隐藏道岔名称"));
        else
            Action3->setText(QString("显示道岔名称"));

        menu->addAction(Action1);
        menu->addAction(Action2);
        menu->addAction(Action3);

        connect(Action1, SIGNAL(triggered()),zcArray[0], SLOT(QDNameShowOrHide()));
        connect(Action2, SIGNAL(triggered()),zcArray[0], SLOT(XHNameShowOrHide()));
        connect(Action3, SIGNAL(triggered()),zcArray[0], SLOT(DCNameShowOrHide()));
        menu->exec(QCursor::pos());
        this->update();
    }
}

void Monitor::mouseReleaseEvent(QMouseEvent *event)
{
    if(!IsstationLoadFlag)
        return ;
    //    if(6 == sizeType)//选取设备操作
    if(CZState)//选取设备操作
    {
        if(SelectFlag &&IsEditFlag)
        {
            FtartPt = QPoint (event->x(),event->y());
            SelectDevFun();
            SelectFlag =false;
            ResetRectSelect();
        }

    }
}
void Monitor::UPdate()
{
    this->update();
    nElapsed++;
}

void Monitor::JLProc()
{
    zcArray[0]->JLProcOnTimer();
}

void Monitor::ANProc()
{
    station->HandleANOnTimer();
}

//void Monitor::on_ZQXButton_clicked()
//{
//    QString Record;
//    station->SetCancle();
//    Record = QString("总取消按钮按下\n");
////    DBSERVICE.UpdateOperationRecord(Record);
//}

void Monitor::DealWithCTCCmd(QVector<T_XHD *> tempv_ANArray)
{
    station->ConvertCTCCmd(tempv_ANArray);
}

void Monitor::SetCancleByInterlock()
{
    station->CancelCurrentJL();
}


//void Monitor::UpdataStationdata(QString StationName, int ID, QString fileName,int sizetype)
void Monitor::UpdataStationdata(int ID, QString fileName, bool sizetype)
{
    if(fileName == "")
        return;
    //zcArray.clear();
    // QString StationFileName;
    //    StationFileName = QString("%1/stationdata/%2").arg(QDir::currentPath()).arg(fileName);
    // StationFileName = QString("%1/%2").arg(QDir::currentPath()).arg(fileName);

    StationRead(fileName);
    IsstationLoadFlag = true;
    //    sizeType = sizetype;
    sizeType = ID;
    CZState = sizetype;
    this->update();
}

void Monitor::ResetRectSelect()
{
    SelectRect = QRect(0,0,0,0);
    StartPt = QPoint(0,0);
    FtartPt = QPoint(0,0);
    SelectFlag = false;
    this->update();
}

void Monitor::SelectDevFun()
{
    SelectRect = CountRectFun();
    QStringList XHList,DCList,QJList,QDList,XHdev_code,DCdev_code,QJdev_code,QDdev_code;
    //    zcArray[0]->GetSelectDevFun(SelectRect,&QJList,&DCList,&XHList,&QDList);
    GetSelectDev(SelectRect,&XHList,&DCList,&QJList,&QDList,&XHdev_code,&DCdev_code,&QJdev_code,&QDdev_code);
    if(QJList.size() == 0 && XHList.size()== 0 && DCList.size()== 0
            && QDList.size()== 0)
        return ;
    emit SendSelectDevsignal(XHList,DCList,QJList,QDList,XHdev_code,DCdev_code,QJdev_code,QDdev_code);

}
void Monitor::ClearSelectFun()
{
    //    zcArray[0]->ClearSelectState();
    this->update();
}
QRect Monitor::CountRectFun()
{
    return m_Transform.inverted().mapRect(QRect(StartPt.x(),StartPt.y(),(FtartPt.x()-StartPt.x()),(FtartPt.y()-StartPt.y())));//坐标返回为缩小前的坐标
}
void Monitor::BaseRectDraw(QPainter *painter, int LineType,QColor LineColor,QColor BlackColor, QRect rect)
{
    QPen pen;
    QBrush brush;
    pen.setColor(LineColor);
    pen.setWidth(LineType);
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);//设置画笔形式
    //    if(BlackColor == NILLColour)
    //        painter->setBrush(Qt::NoBrush);
    //    else
    painter->setBrush(BlackColor);//设置画刷bai，如果不画实现的直du接把Brush设置为setBrush(Qt::NoBrush);
    painter->drawRect(rect);
}
void Monitor::GetSelectDev(QRect Rect,  QStringList *XHdev,QStringList *DCdev,QStringList *QJdev,QStringList *QDdev,QStringList *XHdev_code,QStringList *DCdev_code,QStringList *QJdev_code,QStringList *QDdev_code)
{
    // T_XHD *pXHD;
    T_GDDC *T_GDDC;
    T_GD *T_GD;
    //    for (int i=0;i<zcArray[0]->XHDArray.count();i++)
    //    {
    //        pXHD=zcArray[0]->XHDArray[i];
    //        if((pXHD->pCenter.x()>Rect.x())&&(pXHD->pCenter.y()>Rect.y())&&(pXHD->pCenter.x()<(Rect.x()+Rect.width()))&&(pXHD->pCenter.y()<(Rect.y()+Rect.height())))
    //        {
    //            if(pXHD->XHisPLCZ==false)
    //            {
    //                XHdev->push_back(pXHD->m_strName);
    //                XHdev_code->push_back(QString::number(pXHD->m_nCode));
    //                XHList_SAVE.push_back(pXHD->m_strName);
    //                pXHD->XHisPLCZ=true;
    //            }
    //        }
    //    }


    if(tempStation == nullptr)
        return;
    for (int i=0;i<tempStation->GDDCArray.count();i++)
    {
        T_GDDC=tempStation->GDDCArray[i];
        if((T_GDDC->pCenter.x()>Rect.x())&&(T_GDDC->pCenter.y()>Rect.y())&&(T_GDDC->pCenter.x()<(Rect.x()+Rect.width()))&&(T_GDDC->pCenter.y()<(Rect.y()+Rect.height())))
        {
            if(T_GDDC->DCisPLCZ==false)
            {
                DCdev->push_back(T_GDDC->m_strName);
                DCdev_code->push_back(QString::number(T_GDDC->m_nCode));
                DCList_SAVE.push_back(T_GDDC->m_strName);
                T_GDDC->DCisPLCZ=true;
            }
        }
    }
    for (int i=0;i<tempStation->GDArray.count();i++)
    {
        T_GD=tempStation->GDArray[i];
        if((T_GD->pCenter.x()>Rect.x())&&(T_GD->pCenter.y()>Rect.y())&&(T_GD->pCenter.x()<(Rect.x()+Rect.width()))&&(T_GD->pCenter.y()<(Rect.y()+Rect.height())))
        {
            if(T_GD->m_nGDType==JJ_QD)
            {
                if(T_GD->GDisPLCZ==false)
                {
                    QJdev->push_back(T_GD->m_strName);
                    QJdev_code->push_back(QString::number(T_GD->m_nCode));
                    QJList_SAVE.push_back(T_GD->m_strName);
                    T_GD->GDisPLCZ=true;
                }
            }
            else
            {
                if(T_GD->GDisPLCZ==false)
                {
                    QDdev->push_back(T_GD->m_strName);
                    QDdev_code->push_back(QString::number(T_GD->m_nCode));
                    QDList_SAVE.push_back(T_GD->m_strName);
                    T_GD->GDisPLCZ=true;
                }
            }
        }
    }
}

void Monitor::setZCRefresh()
{
    T_XHD *pXHD;
    T_GDDC *T_GDDC;
    T_GD *T_GD;
    for (int i=0;i<zcArray[0]->XHDArray.count();i++)
    {
        pXHD=zcArray[0]->XHDArray[i];
        pXHD->XHisPLCZ=false;
    }
    for (int i=0;i<zcArray[0]->GDDCArray.count();i++)
    {
        T_GDDC=zcArray[0]->GDDCArray[i];
        T_GDDC->DCisPLCZ=false;
    }
    for (int i=0;i<zcArray[0]->GDArray.count();i++)
    {
        T_GD=zcArray[0]->GDArray[i];
        T_GD->GDisPLCZ=false;
    }
    this->update();
}
























