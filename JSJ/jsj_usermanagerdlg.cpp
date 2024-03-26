#include "jsj_usermanagerdlg.h"
#include "ui_jsj_usermanagerdlg.h"
#include"jsj_services.h"
#include<QStandardItemModel>
#include<QDebug>
#include"cmsgbox.h"
#include"jsj_maindlg.h"
#include<QDesktopWidget>
jsj_UserManagerdlg::jsj_UserManagerdlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_UserManagerdlg)
{
    ui->setupUi(this);
    ui->ExitBtn->hide();
    DBSERVICES.SetBackImage(this,"用户管理2");
    QStringList strHeader;
    QHeaderView* view = new QHeaderView(Qt::Horizontal);
    QStandardItemModel* model = new QStandardItemModel;
    strHeader << ("序号") << ("姓名") << ("工号")<< ("班组")<< ("岗位")<< ("车站")<<("权限");
    model->setHorizontalHeaderLabels(strHeader);
    view->setModel(model);
    view->setStyleSheet("QHeaderView::section{"
                        "background-color:#009cff;font:16px '微软雅黑';"
                        "color:#ffffff;}");
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setHorizontalHeader(view);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(60);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //行头自适应表格
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->verticalHeader()->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->verticalHeader()->setHidden(true);
    ui->tableWidget->hideColumn(6);
    InitUserInfo();
    connect(&DBSERVICES,SIGNAL(UpdateTableShow()),this,SLOT(SetUpdatedTable()));

}

jsj_UserManagerdlg::~jsj_UserManagerdlg()
{
    delete ui;
}

void jsj_UserManagerdlg::InitUserInfo()
{

    DBSERVICES.GetUserInfo();


    showDonInfo(false);
}



void jsj_UserManagerdlg::on_ExitBtn_clicked()
{

    emit DBSERVICES.ShowMain(true);
    DBSERVICES.SetSpan();
    this->hide();
}

void jsj_UserManagerdlg::on_SearchBtn_clicked()
{
    DBSERVICES.GetUserInfo();
    QString UserName = ui->UserNameEdit->text();
    QString StationName = ui->StationEdit->text();
    InitTable(StationName,UserName);
    index = 0;PageIndex = 0;
    showDonInfo(false);
}
void jsj_UserManagerdlg::InitTable(QString StationName, QString UserName)
{
    DBSERVICES.ClearContent(ui->tableWidget);
    if (UserName!="" && StationName == "")
    {
        for(int i = 0; i < DBSERVICES.InfoList.size();++i)
        {
            if(!DBSERVICES.InfoList[i].UserName.contains(UserName))
            {
                DBSERVICES.InfoList.removeAt(i);
                i--;
            }
        }

    }

    else if (UserName == "" && StationName!="")
    {
        for(int i = 0; i < DBSERVICES.InfoList.size();++i)
        {
            if(DBSERVICES.InfoList[i].UserStation.contains(StationName))
            {
                continue;
            }else
            {
                DBSERVICES.InfoList.removeAt(i);
                i--;
            }
        }
    }
    else if (UserName != "" && StationName!="")
    {
        for(int i = 0; i < DBSERVICES.InfoList.size();++i)
        {
            if(DBSERVICES.InfoList[i].UserStation.contains(StationName)
                    && DBSERVICES.InfoList[i].UserName .contains(UserName))
            {
                continue;
            }
            else
            {
                DBSERVICES.InfoList.removeAt(i);
                i--;
            }
        }
    }
    qDebug()<<"Size :"<< DBSERVICES.InfoList.size();
}
void jsj_UserManagerdlg::on_ReSetBtn_clicked()
{
    ui->StationEdit->clear();
    ui->UserNameEdit->clear();
    DBSERVICES.GetUserInfo();
    qDebug()<<"22Size:"<<DBSERVICES.InfoList.size();
    InitTable("","");
    index = 0;PageIndex = 0;
    showDonInfo(false);

}
#include<QTableWidgetSelectionRange>

void jsj_UserManagerdlg::on_DelBtn_clicked()
{

    if(DBSERVICES.OperateGet.Post!="")
    {
        while(ui->tableWidget->selectedItems().count() > 0){
            QList<QTableWidgetSelectionRange> ranges = ui->tableWidget->selectedRanges();
            int count = ranges.count();
            for(int i=0;i<count;i++)
            {
                int topRow=ranges.at(i).topRow();

                int bottomRow=ranges.at(i).bottomRow();

                for(int j=topRow;j<=bottomRow;j++)
                {
                    QString NUmber =ui->tableWidget->item(j,2)->text();

                    qDebug()<<NUmber;
                    if(DBSERVICES. LoginSQLData.SuperAdminName==(NUmber))
                    {
                        CMsgBox::showMsgBox("无法删除管理员账号");
                        return;
                    }
                    if(DBSERVICES.DeleUserInfoByNumber(NUmber))
                    {
                        qDebug()<<"号码:"<<NUmber;
                        ui->tableWidget->removeRow(j);
                        CMsgBox::showMsgBox("删除选中人员信息成功");
                    }else
                    {
                        CMsgBox::showMsgBox("删除选中人员信息失败");
                    }
                }
            }
        }

        DBSERVICES.GetUserInfo();
        SetUpdatedTable();
    }else
    {
        ShowInit();
    }
}
#include"gen_exceloutput.h"
void jsj_UserManagerdlg::on_DaochuBtn_clicked()
{

    qDebug()<<ui->tableWidget->horizontalHeader()->model()->headerData(0,Qt::Horizontal).toString();
    if(DBSERVICES.OperateGet.Post!="")
    {
        gen_Exceloutput pt;
        pt.Table2ExcelByHtml(ui->tableWidget,"用户管理");
    }else
    {
        ShowInit();
    }

}
void jsj_UserManagerdlg::on_DaoruBtn_clicked()
{
    if(DBSERVICES.OperateGet.Post!="")
    {
        QString path = QFileDialog::getOpenFileName(
                    this, "选择需要导入的文件",
                    "/",
                    "文本文件 (*.xls *.xlsx );; 所有文件 (*.*);; ");
        if(path!="")
            DBSERVICES.LoadConfigExcel(path);
    }else
    {
        ShowInit();
    }

}

void jsj_UserManagerdlg::on_AddNewBtn_clicked()
{
    if(DBSERVICES.OperateGet.Post!="")
    {
        jsj_addnewOnedlg * dlg = new jsj_addnewOnedlg();
        dlg->OperateType = ADD;
        dlg->InitInfo();
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();
    }else
    {
        ShowInit();
    }
}
void jsj_UserManagerdlg::ShowInit()
{
    jsj_logindlg * dlg = new jsj_logindlg();
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
}
void jsj_UserManagerdlg::showDonInfo(bool isDel)
{
    int Size = DBSERVICES.InfoList.length();
    if(index >= Size)
    {
        CMsgBox::showMsgBox("无更多展示项");
        return;
    }else
        DBSERVICES.ClearContent(ui->tableWidget);
    int d=Size%10;
    int Page = Size/10;
    if(d > 0)
        Page++;
    if(PageIndex < Page && !isDel)
    {
        PageIndex++;
    }
    QFont nullFont;
    nullFont.setBold(true);
    nullFont.setFamily("微软雅黑");
    nullFont.setPointSize(15);
    for (;index<PageIndex*10;++index) {
        if(index >=Size)
            return;
        int t_rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(t_rowCount);
        ui->tableWidget->setRowHeight(t_rowCount,50);

        QTableWidgetItem * item_0 = new QTableWidgetItem(QString::number(index+1));
        QTableWidgetItem * item_1 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserName);
        QTableWidgetItem * item_2 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserNumber);
        QTableWidgetItem * item_3 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserTeam);
        QTableWidgetItem * item_4 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserGangWei);
        QTableWidgetItem * item_5 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserStation);
        QTableWidgetItem * item_6 = new QTableWidgetItem(DBSERVICES.InfoList[index].PhoneNumber);

        item_0->setTextAlignment(Qt::AlignCenter);
        item_1->setTextAlignment(Qt::AlignCenter);
        item_2->setTextAlignment(Qt::AlignCenter);
        item_3->setTextAlignment(Qt::AlignCenter);
        item_4->setTextAlignment(Qt::AlignCenter);
        item_5->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(t_rowCount,0,item_0);
        ui->tableWidget->setItem(t_rowCount,1,item_1);
        ui->tableWidget->setItem(t_rowCount,2,item_2);
        ui->tableWidget->setItem(t_rowCount,3,item_3);
        ui->tableWidget->setItem(t_rowCount,4,item_4);
        ui->tableWidget->setItem(t_rowCount,5,item_5);
        ui->tableWidget->setItem(t_rowCount,6,item_6);
    }
}
void jsj_UserManagerdlg::on_EditBtn_clicked()
{
    if(DBSERVICES.OperateGet.Post!="")
    {
        jsj_addnewOnedlg * dlg = new jsj_addnewOnedlg();//不支持多条操作
        dlg->OperateType = EDIT;
        QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();
        int count=ranges.count();
        for(int i=0;i<count;i++)
        {
            int topRow=ranges.at(i).topRow();
            int bottomRow=ranges.at(i).bottomRow();
            for(int j=topRow;j<=bottomRow;j++)
            {
                QString NUmber =ui->tableWidget->item(j,2)->text();
                dlg->UserKeyId = NUmber;
                dlg->setAttribute(Qt::WA_DeleteOnClose);
                dlg->InitInfo();
                dlg->show();
                return;
            }
        }
    }else
    {

        ShowInit();
    }
}
#include"jsj_loginlongdlg.h"
void jsj_UserManagerdlg::on_RecordBtn_clicked()
{
    if(DBSERVICES.OperateGet.Post!="")
    {

        jsj_loginlongdlg * dlg = new jsj_loginlongdlg();
        QList<QTableWidgetSelectionRange>ranges=ui->tableWidget->selectedRanges();
        int count=ranges.count();
        for(int i=0;i<count;i++)
        {
            int topRow=ranges.at(i).topRow();
            int bottomRow=ranges.at(i).bottomRow();
            for(int j=topRow;j<=bottomRow;j++)
            {
                QString NUmber = ui->tableWidget->item(j,2)->text();
                dlg->UserName  = ui->tableWidget->item(j,1)->text();
                dlg->UserKeyId = NUmber;
                dlg->setAttribute(Qt::WA_DeleteOnClose);
                dlg->InitInfo();
                dlg->show();
                return;
            }
        }
    }else
        ShowInit();
}

void jsj_UserManagerdlg::on_NextBtn_clicked()
{
    showDonInfo(false);
}

void jsj_UserManagerdlg::on_UpBtn_clicked()
{
    qDebug()<<"ROU COUT :"<<ui->tableWidget->rowCount();
    int Size = DBSERVICES.InfoList.size();
    int d=Size%10;
    int Page = Size/10;
    if(d > 0)
        Page++;

    int cout = ui->tableWidget->rowCount();
    if(ui->tableWidget->rowCount()<10)
        index = index-cout;
    if(PageIndex >1)
    {
        DBSERVICES.ClearContent(ui->tableWidget);
        PageIndex--;
    }else
    {
        CMsgBox::showMsgBox("无更多展示项");
        return;
    }
    QFont nullFont;
    nullFont.setBold(true);
    nullFont.setFamily("微软雅黑");
    nullFont.setPointSize(15);
    int MaxSize = 0;
    if(PageIndex!=1)
    {
        MaxSize = 10*PageIndex;
        index = (PageIndex - 1)*10;
    }else
    {
        index = 0;
        MaxSize = 10;
    }
    for (;index<(MaxSize);++index) {
        int t_rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(t_rowCount);
        ui->tableWidget->setRowHeight(t_rowCount,50);
        QTableWidgetItem * item_0 = new QTableWidgetItem(QString::number(index+1));
        QTableWidgetItem * item_1 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserName);
        QTableWidgetItem * item_2 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserNumber);
        QTableWidgetItem * item_3 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserTeam);
        QTableWidgetItem * item_4 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserGangWei);
        QTableWidgetItem * item_5 = new QTableWidgetItem(DBSERVICES.InfoList[index].UserStation);
        QTableWidgetItem * item_6 = new QTableWidgetItem(DBSERVICES.InfoList[index].PhoneNumber);

        item_0->setTextAlignment(Qt::AlignCenter);
        item_1->setTextAlignment(Qt::AlignCenter);
        item_2->setTextAlignment(Qt::AlignCenter);
        item_3->setTextAlignment(Qt::AlignCenter);
        item_4->setTextAlignment(Qt::AlignCenter);
        item_5->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->setItem(t_rowCount,0,item_0);
        ui->tableWidget->setItem(t_rowCount,1,item_1);
        ui->tableWidget->setItem(t_rowCount,2,item_2);
        ui->tableWidget->setItem(t_rowCount,3,item_3);
        ui->tableWidget->setItem(t_rowCount,4,item_4);
        ui->tableWidget->setItem(t_rowCount,5,item_5);
        ui->tableWidget->setItem(t_rowCount,6,item_6);
    }
}

void jsj_UserManagerdlg::SetUpdatedTable()
{
    index = PageIndex*10-10;
    showDonInfo(true);
}

void jsj_UserManagerdlg::on_pushButton_clicked()
{
    DBSERVICES.UpdateInfo();
    UserInfo info;
    info.UserName = DBSERVICES.LoginSQLData.SuperAdminName;;
    info.Password = DBSERVICES.LoginSQLData.SuperAdminPassWord;
    info.UserGangWei = DBSERVICES.LoginSQLData.gangweiId;
    info.UserNumber = DBSERVICES.LoginSQLData.SuperAdminName;
    DBSERVICES.AddSuperAdmin(info);
}
