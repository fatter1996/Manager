#include "jsj_xsmldlg.h"
#include "ui_jsj_xsmldlg.h"
#include<QMessageBox>
#include<QCalendarWidget>
#include"cmsgbox.h"
jsj_xsmldlg::jsj_xsmldlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_xsmldlg)
{
    ui->setupUi(this);
    connect(&DBSERVICES,SIGNAL(Send_DD_Info(DDML,unsigned int,bool)),this,
            SLOT(XSMLMsgUpdate(DDML,unsigned int,bool)));

    ui->ml_NumberEdit->setMaxLength(6);
    ui->ml_NumberEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    m_formdlg = new stationshowfrom();
    m_formdlg->setAttribute(Qt::WA_DeleteOnClose);
    ui->verticalLayout_2->addWidget(m_formdlg);
    connect(&DBSERVICES,SIGNAL(GetGDInfo(QString)),this,SLOT(GetGDByStationName(QString)));
    for(int i = 0; i < 9;++i)
    {
        ui->Xs_Combo->addItem(QString("%1").arg(i*5+5));
    }
    ui->Xs_Combo->setCurrentIndex(1);
    ui->ml_Send_Time->setDate(QDate::currentDate());
    ui->ml_Send_Time->setTime(QTime::currentTime());

    ui->ml_End_Time->setDate(QDate::currentDate());
    ui->ml_End_Time->setTime(QTime::currentTime());

    ui->ml_Send_Time->setCalendarPopup(true);
    ui->ml_End_Time->setCalendarPopup(true);

    QCalendarWidget * wi = new QCalendarWidget(ui->ml_Send_Time);
    wi->setFixedSize(QSize(300,250));
    wi->setStyleSheet(QString("#qt_calendar_calendarview {background-color: rgba(10, 95, 189,1);"
                              "font: 12px;}"
                              "QCalendarWidget QTableView {alternate-background-color:rgba(10, 95, 189, 1); "
                              "color: rgba(255,255,255,1);}"
                              "QCalendarWidget QToolButton {height: 30px;icon-size: 30px, 30px;}"
                              "QCalendarWidget QWidget#qt_calendar_navigationbar{ background-color: rgba(4, 51, 114,1);}"
                              "QCalendarWidget QToolButton#qt_calendar_prevmonth {qproperty-icon: url(:/new/jsj/rc/新增/左箭头-2.png);}"
                              "QCalendarWidget QToolButton#qt_calendar_nextmonth {qproperty-icon: url(:/new/jsj/rc/新增/右箭头-2 (1).png);}"
                              "QCalendarWidget QAbstractItemView:enabled {font-size:12px;  color: rgb(255, 255, 255); }"
                              "QCalendarWidget QAbstractItemView:disabled { color: rgb(64, 64, 64); }"
                              "QCalendarWidget QMenu {width: 100px;left: 20px;color: white;font-size: 12px;"
                              "background-color: rgb(10, 95, 189);}"));

    QTextCharFormat format;
    format.setForeground(QColor(255,255,255));
    format.setBackground(QColor(10, 95,189));
    format.setFontFamily("Microsoft YaHei");
    wi->setWeekdayTextFormat(Qt::Saturday , format);
    wi->setWeekdayTextFormat(Qt::Sunday , format);




    ui->ml_Send_Time->setDate(QDate::currentDate());
    ui->ml_Send_Time->setTime(QTime::currentTime());
    ui->ml_Send_Time->setCalendarWidget(wi);



    QCalendarWidget * wi_ = new QCalendarWidget(ui->ml_Send_Time);
    wi_->setFixedSize(QSize(300,250));
    wi_->setStyleSheet(QString("#qt_calendar_calendarview {background-color: rgba(10, 95, 189,1);"
                               "font: 12px;}"
                               "QCalendarWidget QTableView {alternate-background-color:rgba(10, 95, 189, 1); "
                               "color: rgba(255,255,255,1);}"
                               "QCalendarWidget QToolButton {height: 30px;icon-size: 30px, 30px;}"
                               "QCalendarWidget QWidget#qt_calendar_navigationbar{ background-color: rgba(4, 51, 114,1);}"
                               "QCalendarWidget QToolButton#qt_calendar_prevmonth {qproperty-icon: url(:/new/jsj/rc/新增/左箭头-2.png);}"
                               "QCalendarWidget QToolButton#qt_calendar_nextmonth {qproperty-icon: url(:/new/jsj/rc/新增/右箭头-2 (1).png);}"
                               "QCalendarWidget QAbstractItemView:enabled {font-size:12px;  color: rgb(255, 255, 255); }"
                               "QCalendarWidget QAbstractItemView:disabled { color: rgb(64, 64, 64); }"
                               "QCalendarWidget QMenu {width: 100px;left: 20px;color: white;font-size: 12px;"
                               "background-color: rgb(10, 95, 189);}"));
    wi_->setWeekdayTextFormat(Qt::Saturday, format);
    wi_->setWeekdayTextFormat(Qt::Sunday, format);

    ui->ml_End_Time->setDate(QDate::currentDate());
    ui->ml_End_Time->setTime(QTime::currentTime());
    ui->ml_End_Time->setCalendarWidget(wi_);


    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(!DBSERVICES.CurBackInfoList[i].isAppend())
            continue;
        ui->comboBox->addItem(DBSERVICES.CurBackInfoList[i].StationName,
                              DBSERVICES.CurBackInfoList[i].StationId);
    }
    //InitStationInfo();
   // globals.SelectStationName = QString("Station%1").arg(ui->comboBox->currentData().toInt());//初始化站场界面绘制
}
jsj_xsmldlg::~jsj_xsmldlg()
{
    delete ui;
}
#include<QDebug>
void jsj_xsmldlg::XSMLMsgUpdate(DDML ML, unsigned int type, bool flag)
{

}
void jsj_xsmldlg::ClearInfo()
{

    DBSERVICES.GetDDML.vect_qdDev.clear();
    DBSERVICES.GetDDML.vect_dcDev.clear();
    DBSERVICES.GetDDML.vect_qjDev.clear();

    ui->ml_NumberEdit->clear();
    ui->Xs_Combo->setCurrentIndex(-1);
    m_formdlg->ClearInfo();
}
#include<QListView>
void jsj_xsmldlg::InitStation()
{
    int flag=-1;
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        flag =DBSERVICES.isExistCombo(ui->comboBox,DBSERVICES.CurBackInfoList[i].StationId);
        if(!DBSERVICES.CurBackInfoList[i].isAppend())
        {
            if(flag!=-1)
            {
                ui->comboBox->removeItem(flag);
            }
        }
        else
        {
            if(flag == -1)
            {
                ui->comboBox->addItem(DBSERVICES.CurBackInfoList[i].StationName,
                                      DBSERVICES.CurBackInfoList[i].StationId);
            }
        }
    }
}
void jsj_xsmldlg::GetMsgSave_XSML(unsigned int kind, unsigned int type)
{
    if(kind == XSMLINFO)
    {
        for(int i = 0; i <m_formdlg->CurrectZone->vect_dcDev.size();++i)
        {
            SheBeiInfo tempInfo;
            if(m_formdlg->CurrectZone->vect_dcDev[i].IsselectFlag)
            {
                tempInfo.shebeiName = m_formdlg->CurrectZone->vect_dcDev[i].Devname;
                tempInfo.shebeiCode = m_formdlg->CurrectZone->vect_dcDev[i].m_nCode;
                DBSERVICES.GetDDML.vect_dcDev.push_back(tempInfo);
            }

        }
        for(int i = 0; i <m_formdlg->CurrectZone->vect_qdDev.size();++i)
        {
            SheBeiInfo tempInfo;
            if(m_formdlg->CurrectZone->vect_qdDev[i].IsselectFlag)
            {
                tempInfo.shebeiName = m_formdlg->CurrectZone->vect_qdDev[i].Devname;
                tempInfo.shebeiCode = m_formdlg->CurrectZone->vect_qdDev[i].m_nCode;
                DBSERVICES.GetDDML.vect_qdDev.push_back(tempInfo);
            }
        }
        for(int i = 0; i <m_formdlg->CurrectZone->vect_qjDev.size();++i)
        {
            SheBeiInfo tempInfo;
            if(m_formdlg->CurrectZone->vect_qjDev[i].IsselectFlag)
            {
                tempInfo.shebeiName = m_formdlg->CurrectZone->vect_qjDev[i].Devname;
                tempInfo.shebeiCode = m_formdlg->CurrectZone->vect_qjDev[i].m_nCode;
                DBSERVICES.GetDDML.vect_qjDev.push_back(tempInfo);
            }
        }
        DBSERVICES.GetDDML.StartTime[0] = ui->ml_Send_Time->date().year();
        DBSERVICES.GetDDML.StartTime[1] = ui->ml_Send_Time->date().month();
        DBSERVICES.GetDDML.StartTime[2] = ui->ml_Send_Time->date().day();
        DBSERVICES.GetDDML.StartTime[3] = ui->ml_Send_Time->time().hour();
        DBSERVICES.GetDDML.StartTime[4] = ui->ml_Send_Time->time().minute();
        DBSERVICES.GetDDML.StartTime[5] = ui->ml_Send_Time->time().second();

        DBSERVICES.GetDDML.EndTime[0] = ui->ml_End_Time->date().year();
        DBSERVICES.GetDDML.EndTime[1] = ui->ml_End_Time->date().month();
        DBSERVICES.GetDDML.EndTime[2] = ui->ml_End_Time->date().day();
        DBSERVICES.GetDDML.EndTime[3] = ui->ml_End_Time->time().hour();
        DBSERVICES.GetDDML.EndTime[4] = ui->ml_End_Time->time().minute();
        DBSERVICES.GetDDML.EndTime[5] = ui->ml_End_Time->time().second();
        if(type ==SEND)
            DBSERVICES.GetDDML.Status = Sended;
        else
            DBSERVICES.GetDDML.Status = Send_toBack;
        DBSERVICES.GetDDML.ML_Number = ui->ml_NumberEdit->text();
        DBSERVICES.GetDDML.SpeedRange = ui->Xs_Combo->currentText().toInt();

        DBSERVICES.GetDDML.ML_Tpe = XSMLINFO;
        DBSERVICES.GetDDML.ML_T ="限速命令";

        if (DBSERVICES.GetDDML.ML_Number ==  "")
        {
            CMsgBox::showMsgBox("请检查命令号");
            return;
        }

        if (DBSERVICES.GetDDML.SpeedRange == 0)
        {
            CMsgBox::showMsgBox("请检查限速上限");
            return;
        }

        qDebug()<<"以保存命令:"<<DBSERVICES.GetDDML.ML_Number;
        if(!DBSERVICES.GetSame_MlNumberIsExist(DBSERVICES.GetDDML.ML_Number, DBSERVICES.GetDDML.ML_T))
        {
            CMsgBox::showMsgBox("相同命令号已存在,请检查!");
            return;
        }
        DBSERVICES.GetDDML.Stationid = ui->comboBox->currentData().toUInt();
        DBSERVICES.SavedList.push_back(DBSERVICES.GetDDML);
        DBSERVICES.GetDDML.initInfo();
        ClearInfo();
        return;
    }
}
void jsj_xsmldlg::GetGDByStationName(QString StatioName)
{
    QStringList list;
    m_formdlg->GetGDInfo(StatioName);
}
void jsj_xsmldlg::InitStationInfo()
{
    ClearInfo();
    QString CurText = ui->comboBox->currentText();
    int Index = ui->comboBox->currentData().toInt();
    QString StationName = QString("Station%1").arg(Index);
    emit SetSelectStation(CurText);
    globals.SelectStationName = StationName;
}
void jsj_xsmldlg::on_comboBox_activated(const QString &arg1)
{
    if(arg1!="")
    {
       InitStationInfo();
    }
    this->update();
}
