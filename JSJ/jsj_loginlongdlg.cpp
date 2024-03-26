#include "jsj_loginlongdlg.h"
#include "ui_jsj_loginlongdlg.h"
#include<QStandardItemModel>
#include"jsj_services.h"
#include"cmsgbox.h"
jsj_loginlongdlg::jsj_loginlongdlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_loginlongdlg)
{
    ui->setupUi(this);

    QHeaderView* view = new QHeaderView(Qt::Horizontal);
    QStandardItemModel* model = new QStandardItemModel;
    QStringList strHeader;
    strHeader << ("用户姓名") << ("登录时间") << ("退出时间")<< ("登录时长");
    model->setHorizontalHeaderLabels(strHeader);
    view->setModel(model);
    view->setStyleSheet("QHeaderView::section{"
                        "background-color:#022046;font:16px '微软雅黑';"
                        "color:#ffffff;}");
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeader(view);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(60);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); //禁止编辑
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); //行头自适应表格
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->verticalHeader()->setFrameShape(QFrame::NoFrame);
    ui->tableWidget->verticalHeader()->setHidden(true);
    setWindowTitle("使用记录");
    setWindowModality(Qt::ApplicationModal);

}

jsj_loginlongdlg::~jsj_loginlongdlg()
{
    delete ui;
}
#include<QMessageBox>
void jsj_loginlongdlg::InitInfo()
{

    InsertInfo();
    int Id =  DBSERVICES.GetUserIdByUserWorkId(UserKeyId);
    if(Id!=-1)
        DBSERVICES.SelectUserLoginInfoById(Id);
    else
        QMessageBox::information(this,"提示","无法匹配合适人员");
    showDonInfo(false);
}

#include"gen_exceloutput.h"
void jsj_loginlongdlg::on_pushButton_clicked()
{
    gen_Exceloutput pt;
    QString title = QString ("%1使用时长统计").arg(UserName);
    pt.Table2ExcelByHtml(ui->tableWidget,title);
}

void jsj_loginlongdlg::on_pushButton_2_clicked()
{
    this->close();
}

void jsj_loginlongdlg::InsertInfo()
{
    for(int i = 1; i < 13;++i)
    {
        ui->comboBox->addItem(QString("%1月").arg(i));
    }
}

void jsj_loginlongdlg::showDonInfo(bool isDel)
{
    int Size = DBSERVICES.UserLoginedInfo.length();
    if(Size == 0)
        return;
    else if(index >= Size)
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
        {
            return;
        }
        int t_rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(t_rowCount);
        QTableWidgetItem * item_1 = nullptr;
        QTableWidgetItem * item_2 = nullptr;
        QTableWidgetItem * item_3 =nullptr;
        QTableWidgetItem *item_4 = nullptr;
        item_1 = new QTableWidgetItem(UserName);
        item_2 = new QTableWidgetItem(DBSERVICES.UserLoginedInfo[index].LoginUpTime);
        item_3 = new QTableWidgetItem(DBSERVICES.UserLoginedInfo[index].OffTime);
        item_4 = new QTableWidgetItem(QString::number
                                      (DBSERVICES.UserLoginedInfo[index].userLoginedTime));
        item_1->setTextAlignment(Qt::AlignCenter);
        item_2->setTextAlignment(Qt::AlignCenter);
        item_3->setTextAlignment(Qt::AlignCenter);
        item_4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(t_rowCount,0,item_1);
        ui->tableWidget->setItem(t_rowCount,1,item_2);
        ui->tableWidget->setItem(t_rowCount,2,item_3);
        ui->tableWidget->setItem(t_rowCount,3,item_4);
    }
}

void jsj_loginlongdlg::ShowUpInfo()
{
    int Size = DBSERVICES.UserLoginedInfo.size();
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
    QTableWidgetItem * item_1 = nullptr;
    QTableWidgetItem * item_2 = nullptr;
    QTableWidgetItem *  item_3 = nullptr;
    QTableWidgetItem *item_4 = nullptr;
    for (;index<(MaxSize);++index) {
        int t_rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(t_rowCount);
        item_1 = new QTableWidgetItem(UserName);
        item_2 = new QTableWidgetItem(DBSERVICES.UserLoginedInfo[index].LoginUpTime);
        item_3 = new QTableWidgetItem(DBSERVICES.UserLoginedInfo[index].OffTime);
        item_4 = new QTableWidgetItem( QString::number(DBSERVICES.UserLoginedInfo[index].userLoginedTime));
        item_1->setTextAlignment(Qt::AlignCenter);
        item_2->setTextAlignment(Qt::AlignCenter);
        item_3->setTextAlignment(Qt::AlignCenter);
        item_4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(t_rowCount,0,item_1);
        ui->tableWidget->setItem(t_rowCount,1,item_2);
        ui->tableWidget->setItem(t_rowCount,2,item_3);
        ui->tableWidget->setItem(t_rowCount,3,item_4);
    }
}
void jsj_loginlongdlg::on_comboBox_currentTextChanged(const QString &arg1)
{
    int timeLong = 0;
    int times = 0;
    if(arg1!="")
    {
        for(int i = 0; i < DBSERVICES.UserLoginedInfo.size();++i)
        {
            QString  Month = QString::number( DBSERVICES.UserLoginedInfo[i].Month);
            int Long = DBSERVICES.UserLoginedInfo[i].userLoginedTime;
            if(arg1.contains(Month))
            {
                times++;
                timeLong = timeLong + Long;
            }
        }
        ui->label_2->setText(QString("月登录时长:%1分钟").arg(timeLong));
        ui->label_4->setText(QString("月登录次数:%1次").arg(times));
    }
}

void jsj_loginlongdlg::on_NextBtn_clicked()
{
    showDonInfo(false);
}

void jsj_loginlongdlg::on_UpBtn_clicked()
{
    ShowUpInfo();
}
