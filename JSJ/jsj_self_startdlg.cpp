#include "jsj_self_startdlg.h"
#include "ui_jsj_self_startdlg.h"

jsj_Self_Startdlg::jsj_Self_Startdlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_Self_Startdlg)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_ShowModal,true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setWindowTitle("一键启动");
    ui->tableWidget->horizontalHeader()->setHidden(true);
    ui->tableWidget->verticalHeader()->setHidden(true);
    InitTable_SelfStart_Info();

}

jsj_Self_Startdlg::~jsj_Self_Startdlg()
{
    delete ui;
}

void jsj_Self_Startdlg::InitTable_SelfStart_Info()
{
    for(int i = 0; i < DBSERVICES.PCInfo_List.size();++i)
    {
        ui->tableWidget->setColumnCount( DBSERVICES.PCInfo_List[i].SoftList.size()+1);
        int Roucout = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(Roucout);
        QTableWidgetItem * item = new QTableWidgetItem(DBSERVICES.PCInfo_List[i].PcName);
        ui->tableWidget->setItem(Roucout,0,item);
        for(int z = 0 ; z < DBSERVICES.PCInfo_List[i].SoftList.size();++z)
        {
            QString Address = DBSERVICES.PCInfo_List[i].SoftList[z].SoftAddress;
            QString CheckName = DBSERVICES.GetSoftNameByCode(DBSERVICES.PCInfo_List[i].SoftList[z].SoftCode);
            QWidget *Wi = DBSERVICES.GetCellWidget((CheckName),this,Address);
            Wi->setProperty("Port",DBSERVICES.PCInfo_List[i].Port);
            Wi->setProperty("IP",DBSERVICES.PCInfo_List[i].IPAddress);
            ui->tableWidget->setCellWidget(Roucout,z+1,Wi);
        }
    }
}
void jsj_Self_Startdlg::on_pushButton_clicked()
{
    QString IP;int Port;
    for(int i = 0 ; i < ui->tableWidget->rowCount();++i)
    {
        QStringList PathList;
        for(int z = 1; z< ui->tableWidget->columnCount();++z)
        {
            pWidget =ui->tableWidget->cellWidget(i, z);
            if(pWidget!=nullptr)
            {
                QCheckBox *checkbox=nullptr;
                checkbox = pWidget->findChild<QCheckBox *>();
                qDebug()<<checkbox->checkState();
                if(checkbox && checkbox->checkState() == Qt::Checked)
                {
                    IP=pWidget->property("IP").toString();
                    Port = pWidget->property("Port").toInt();
                    QString Path = checkbox->property("Address").toString();
                    PathList.push_back(Path);
                }
            }
        }
        if(PathList.size()>0)
        {
            QString Paths = PathList.join("|");
            qDebug()<<"IP:"<<IP;
            TX_Send_SERVICES.Send_StartSoft(IP,Port,Paths,true);
            pWidget = nullptr;
        }else
            continue;
    }
}

void jsj_Self_Startdlg::on_pushButton_2_clicked()
{
    this->close();
}
bool isSelectOpen = false;
void jsj_Self_Startdlg::on_AllSelectBtn_clicked()
{
    for(int i = 0 ; i < ui->tableWidget->rowCount();++i)
    {
        QStringList PathList;
        for(int z = 1; z< ui->tableWidget->columnCount();++z)
        {
            pWidget =ui->tableWidget->cellWidget(i, z);
            if(pWidget!=nullptr)
            {

                QCheckBox *checkbox=nullptr;
                checkbox = pWidget->findChild<QCheckBox *>();
                if(checkbox){
                    if(isSelectOpen)
                        checkbox->setCheckState(Qt::Unchecked);
                    else
                        checkbox->setCheckState(Qt::Checked);
                }
            }
        }
    }
    if(!isSelectOpen){
        ui->AllSelectBtn->setText("取消全选");
        isSelectOpen = true;
    }else
    {
        ui->AllSelectBtn->setText("一键全选");
        isSelectOpen = false;
    }
}
