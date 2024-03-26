#include "ddml_ddlg.h"
#include "ui_ddml_ddlg.h"
#include<QHeaderView>
#include<QStandardItemModel>
#include"cmsgbox.h"
#include<QDebug>
#include<QCalendarWidget>
#include"cmsgbox.h"
ddml_ddlg::ddml_ddlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ddml_ddlg)
{
    ui->setupUi(this);
    ui->ml_Send_Time->setCalendarPopup(true);

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
    format.setBackground(QColor(10, 95, 189));
    format.setFontFamily("Microsoft YaHei");
    wi->setWeekdayTextFormat(Qt::Saturday, format);
    wi->setWeekdayTextFormat(Qt::Sunday, format);

    ui->ml_Send_Time->setDate(QDate::currentDate());
    ui->ml_Send_Time->setTime(QTime::currentTime());
    ui->ml_Send_Time->setCalendarWidget(wi);

    ui->ml_NumberEdit->setMaxLength(6);
    ui->ml_NumberEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

    ui->ml_Dd_Combo->addItem("枢纽调度员",1);
    ui->ml_ddt_Combo->addItem("枢纽调度台",1);
    for(int i = 0 ; i < DBSERVICES.DDINFOlist.size();++i)
    {

        ui->ml_type_Combo->addItem(DBSERVICES.DDINFOlist[i].DDMLCommond
                                   ,DBSERVICES.DDINFOlist[i].Content);
    }
}

ddml_ddlg::~ddml_ddlg()
{
    delete ui;
}

void ddml_ddlg::InitStation_ddml()
{

}

void ddml_ddlg::GetMsgUpdate(DDML ML, unsigned int Kind, bool is)
{
    ui->ml_Send_Time->setDate(QDate::currentDate());
    ui->ml_Send_Time->setTime(QTime::currentTime());

    ML.StartTime[0] = ui->ml_Send_Time->date().year();
    ML.StartTime[1] = ui->ml_Send_Time->date().month();
    ML.StartTime[2] = ui->ml_Send_Time->date().day();
    ML.StartTime[3] = ui->ml_Send_Time->time().hour();
    ML.StartTime[4] = ui->ml_Send_Time->time().minute();
    ML.StartTime[5] = ui->ml_Send_Time->time().second();
}

#include<QTextBlock>
QString ddml_ddlg::GetPlaintextContent()
{
    QString resulut = "";

    QTextDocument *doc = ui->plainTextEdit->document();

    int nCnt = doc->blockCount();

    for(int i=0;i<nCnt;++i)
    {
        QTextBlock textBlock = doc->findBlockByNumber(i);
        QString strtext = textBlock.text();
        resulut = resulut.append(strtext);
    }

    return resulut;
}
QString ddml_ddlg::GetSendTime()
{
    QString Year = QString::number(ui->ml_Send_Time->date().year());
    QString Month = QString::number(ui->ml_Send_Time->date().month());
    QString Day = QString::number(ui->ml_Send_Time->date().day());
    QString hour  = QString::number(ui->ml_Send_Time->time().hour());
    QString Min = QString::number(ui->ml_Send_Time->time().minute());
    QString Last = QString("%1-%2-%3-%4:%5").arg(Year).arg(Month).arg(Day).arg(hour).arg(Min);
    return Last;
}

int ddml_ddlg::GetIdByName(QString Name)
{
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(DBSERVICES.CurBackInfoList[i].StationName == Name)
            return DBSERVICES.CurBackInfoList[i].StationId;
    }
    return -1;
}
void ddml_ddlg::ClearInfo()
{
    ui->ml_Send_Time->clear();
    ui->ml_type_Combo->setCurrentIndex(-1);
    ui->ml_NumberEdit->setText("");
}
void ddml_ddlg::GetMsgSave(unsigned int Kind, unsigned int type)
{
    if(Kind == DDMLINFO)
    {
        ui->ml_Send_Time->setDate(QDate::currentDate());
        ui->ml_Send_Time->setTime(QTime::currentTime());
        DBSERVICES.GetDDML.StartTime[0] = ui->ml_Send_Time->date().year();
        DBSERVICES.GetDDML.StartTime[1] = ui->ml_Send_Time->date().month();
        DBSERVICES.GetDDML.StartTime[2] = ui->ml_Send_Time->date().day();
        DBSERVICES.GetDDML.StartTime[3] = ui->ml_Send_Time->time().hour();
        DBSERVICES.GetDDML.StartTime[4] = ui->ml_Send_Time->time().minute();
        DBSERVICES.GetDDML.StartTime[5] = ui->ml_Send_Time->time().second();

        DBSERVICES.GetDDML.ML_Type = ui->ml_type_Combo->currentText();
        DBSERVICES.GetDDML.ML_Number = ui->ml_NumberEdit->text();
        DBSERVICES.GetDDML.ML_Content =  GetPlaintextContent();
        DBSERVICES.GetDDML.ML_Tpe = DDMLINFO;
        DBSERVICES.GetDDML.ML_T ="调度命令";
        if(type ==SEND)
            DBSERVICES.GetDDML.Status = Sended;
        else
            DBSERVICES.GetDDML.Status = Send_toBack;
        if(DBSERVICES.GetDDML.ML_Type == "")
        {
            CMsgBox::showMsgBox("请选择命令类型");
            return;
        }
        else if (DBSERVICES.GetDDML.ML_Number ==  "")
        {
            CMsgBox::showMsgBox("请填写命令号");
            return;
        }
        else if (DBSERVICES.GetDDML.ML_Content == "")
        {
            CMsgBox::showMsgBox("请检查命令内容");
            return;
        }
        if(!DBSERVICES.GetSame_MlNumberIsExist(DBSERVICES.GetDDML.ML_Number,DBSERVICES.GetDDML.ML_T))
        {
            CMsgBox::showMsgBox("相同命令号已存在,请检查!");
            return;
        }
        DBSERVICES.GetDDML.Stationid = GetIdByName(DBSERVICES.GetDDML.ML_QSR[0]);
        DBSERVICES.SavedList.push_back(DBSERVICES.GetDDML);
        ClearInfo();
        return;
    }

}

void ddml_ddlg::on_ml_type_Combo_currentTextChanged(const QString &arg1)
{
    ui->plainTextEdit->setPlainText(ui->ml_type_Combo->currentData().toString());
}
