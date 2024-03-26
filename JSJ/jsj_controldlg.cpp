#include "jsj_controldlg.h"
#include "ui_jsj_controldlg.h"
#include<QTableWidgetItem>
jsj_Controldlg::jsj_Controldlg(ControlInfo &info, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_Controldlg)
{
    ui->setupUi(this);
    setWindowTitle("控制模式转换");
    setAttribute(Qt::WA_ShowModal,true);
    //setWindowFlags(Qt::FramelessWindowHint);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setHidden(true);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->setColumnCount(5);
    InitTableInfo();
    GetCheckedInfo_Init(info.StationName,info.AfterCMD);
    if(info.StationName == "")
        ui->pushButton->setEnabled(false);
    else
        ui->pushButton->setEnabled(true);
    beforecmd = info.BeforeCMD;
    Aftercmd = info.AfterCMD;
}
jsj_Controldlg::~jsj_Controldlg()
{
    delete ui;
}
void jsj_Controldlg::InitTableInfo()
{
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(!DBSERVICES.CurBackInfoList[i].isAppend())
            continue;
        int Roucout = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(Roucout);
        ui->tableWidget->setItem(Roucout,0,new QTableWidgetItem( DBSERVICES.CurBackInfoList[i].StationName));
        ui->tableWidget->setCellWidget(Roucout,1,DBSERVICES.GetCellWidget(("中心控制"),this,""));
        ui->tableWidget->setCellWidget(Roucout,2,DBSERVICES.GetCellWidget(("车站控制"),this,""));
        ui->tableWidget->setCellWidget(Roucout,3,DBSERVICES.GetCellWidget(("分散自律"),this,""));
        ui->tableWidget->setCellWidget(Roucout,4,DBSERVICES.GetCellWidget(("同意转换"),this,""));
    }
}
void jsj_Controldlg::GetCheckedInfo_Init(QString StationName, unsigned char After)
{

    for(int i = 0; i < ui->tableWidget->rowCount();++i)
    {
        if(ui->tableWidget->item(i,0)->text() == StationName)
        {
            indexRow = i;
            break;
        }
    }
    if(indexRow!=-1)
    {
        QString ControlInfo = TX_Send_SERVICES.GetUserControlInfo(After);
        for(int i = 1 ; i < ui->tableWidget->columnCount();++i)
        {
            QWidget *pWidget =nullptr;
            pWidget =ui->tableWidget->cellWidget(indexRow, i);
            if(pWidget!=nullptr)
            {
                QCheckBox *checkbox=nullptr;
                checkbox = pWidget->findChild<QCheckBox *>();
                if(checkbox)
                {
                    if(checkbox->text() == ControlInfo)
                    {
                        checkbox->setCheckState(Qt::Checked);
                        break;
                    }
                }
            }
        }
    }

}
void jsj_Controldlg::on_pushButton_clicked()
{
    if(indexRow!=-1)
    {
        QWidget *pWidget =nullptr;
        pWidget =ui->tableWidget->cellWidget(indexRow, 4);
        if(pWidget!=nullptr)
        {
            QCheckBox *checkbox=nullptr;
            checkbox = pWidget->findChild<QCheckBox *>();
            if(checkbox)
            {
                QString StationName = ui->tableWidget->item(indexRow,0)->text();
                if(checkbox->checkState() == Qt::Checked)
                {
                    TX_Send_SERVICES.Send_ControlCmd_Back(StationName,beforecmd,Aftercmd,AGREE,CTC);
                    TX_Send_SERVICES.Send_ControlCmd_Back(StationName,beforecmd,Aftercmd,AGREE,LS_INFO);
                }else
                {
                    TX_Send_SERVICES.Send_ControlCmd_Back(StationName,beforecmd,Aftercmd,UNGREE,CTC);
                    TX_Send_SERVICES.Send_ControlCmd_Back(StationName,beforecmd,Aftercmd,UNGREE,LS_INFO);
                }
                this->close();
            }
        }
    }
}

void jsj_Controldlg::on_pushButton_2_clicked()
{
    this->close();
}
