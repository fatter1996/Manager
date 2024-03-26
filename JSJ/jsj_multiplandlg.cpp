#include "jsj_multiplandlg.h"
#include "ui_jsj_multiplandlg.h"
jsj_MultiPlandlg::jsj_MultiPlandlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jsj_MultiPlandlg)
{
    ui->setupUi(this);
    ui->AddButton_3->hide();
    DBSERVICES.SetBackImage(this,"背景二");
    dlg = nullptr;
    PlanModel = new QStandardItemModel(ui->tableView);
    QStringList strHeader;
    strHeader.append(("计划名称"));
    strHeader.append("计划详情");
    strHeader.append(("状态"));
    strHeader.append(("操作"));
    QHeaderView* view = new QHeaderView(Qt::Horizontal);
    view->setModel(PlanModel);
    view->setStyleSheet("QHeaderView::section{"
                        "background-color:#022046;font:16px '微软雅黑';"
                        "color:#ffffff;}");
    ui->tableView->setHorizontalHeader(view);

    DBSERVICES.Init(ui->tableView,PlanModel,strHeader,false);
    ui->tableView->setColumnWidth(0,200);
    ui->tableView->setColumnWidth(1,1210);
    ui->tableView->setColumnWidth(2,150);
    ui->tableView->setColumnWidth(3,380);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(PlanModel, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)), ui->tableView, SLOT(update()));
    ui->tableView->verticalHeader()->setDefaultSectionSize(120);        //设置行间距
    ui->tableView->verticalHeader()->setMinimumSectionSize(120);        //设置行间距
    connect(&DBSERVICES,SIGNAL(UpdateMultiPlan(unsigned int)),this,SLOT(UpdateEditInfo(unsigned int)));


}

jsj_MultiPlandlg::~jsj_MultiPlandlg()
{
    delete ui;
}
#include"jsj_adp.h"
void jsj_MultiPlandlg::on_AddButton_clicked()
{
    QSize size(1379,732);
    jsj_adp * dlg = new jsj_adp(size);
    dlg->SetInit_Info(APPEND);
    dlg->Node = new jsj_planNode();
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->show();
    ui->tableView->setModel(PlanModel);
}

QString jsj_MultiPlandlg::Get_Add_StrInfo(int index_)
{
    QString Station = "",JF_Type = "",GD_Name = "",Time = "",lastStr = "";
    for(int z = 0; z < DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo.size();++z)
    {
        Station =  DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].StationName + "     ";
        JF_Type =  DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].jfc_TYPE + "     ";
        GD_Name = DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].GDSetName + "     ";
        if(JF_Type != "始发")
            Time = QString("%1|%2").arg(DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].ChuFaTime)
                    .arg(DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].ArriveTime+"     ");
        else
            Time = QString("%1|%2").arg(DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].ArriveTime)
                    .arg(DBSERVICES.PlanNodeList[index_]->CurMultiPlanInfo[z].ChuFaTime);
        QString tempStr = QString("%1%2%3%4").arg(Station).arg(JF_Type).arg(GD_Name).arg(Time);
        if(lastStr!="")
        {
            QString addstr = (QString("%1%2").arg("\r\n").arg(tempStr));
            lastStr = lastStr.append(addstr);
        }
        else
            lastStr = tempStr;
    }
    lastStr = lastStr.append("\r\n");
    qDebug()<<lastStr;
    return lastStr;
}

QWidget *jsj_MultiPlandlg::GetIndex_tableWidget( int index)
{
    QPushButton *Btn = new QPushButton("");
    QPushButton *excute_Btn = new QPushButton("");
    QPushButton *Del_Btn = new QPushButton("");
    Btn->setStyleSheet("border-image: url(:/new/jsj/rc/目录/编辑.png);");
    excute_Btn->setStyleSheet("border-image: url(:/new/jsj/rc/目录/执行.png);");
    Del_Btn->setStyleSheet("border-image: url(:/new/jsj/rc/目录/删除.png);");

    Btn->setMaximumSize(114,34);
    excute_Btn->setMaximumSize(114,34);
    Del_Btn->setMaximumSize(114,34);

    Btn->setMinimumSize(114,34);
    excute_Btn->setMinimumSize(114,34);
    Del_Btn->setMinimumSize(114,34);

    connect(Btn,SIGNAL(clicked()),this,SLOT(ClickedBtnSlot()));
    connect(excute_Btn,SIGNAL(clicked()),this,SLOT(ClickedBtnSlot()));
    connect(Del_Btn,SIGNAL(clicked()),this,SLOT(ClickedBtnSlot()));

    Btn->setProperty("type",Edit);
    excute_Btn->setProperty("type",Excute);
    Del_Btn->setProperty("type",Del);
    Btn->setProperty("row",index);
    excute_Btn->setProperty("row",index);
    Del_Btn->setProperty("row",index);

    QWidget *tmp_widget = new QWidget(ui->tableView);
    QHBoxLayout *tmp_layout = new QHBoxLayout(tmp_widget);
    tmp_layout->addWidget(Btn);
    tmp_layout->addWidget(excute_Btn);
    tmp_layout->addWidget(Del_Btn);
    tmp_layout->setMargin(0);

    return tmp_widget;


}

void jsj_MultiPlandlg::DelInfoByPlanName(QString PlanName)
{
    for(int i = 0 ; i < DBSERVICES.PlanNodeList.size();++i)
    {
        if(DBSERVICES.PlanNodeList[i]->PlanName == PlanName)
        {
            DBSERVICES.PlanNodeList.removeAt(i);
            return;
        }

    }
}
void jsj_MultiPlandlg::on_AddButton_2_clicked()
{
    QString path = QFileDialog::getOpenFileName(
                this, "选择需要导入的文件",
                "/",
                "文本文件 (*.xls *.xlsx ); 所有文件 (*.*);");
    if(path!="")
        LoadMultiPlan(path);
    for(int i = 0; i < DBSERVICES.PlanNodeList.size();++i)
    {

        if(!DBSERVICES.PlanNodeList[i]->isGet)
            continue;
        QString str =Get_Add_StrInfo(i);
        QString Name = DBSERVICES.PlanNodeList[i]->PlanName;
        QStandardItem * item1 = new QStandardItem(Name);
        QStandardItem * item2 = new QStandardItem(str);
        QStandardItem * item3 = new QStandardItem("未下发");

        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);

        PlanModel->setItem(i,0,item1);
        PlanModel->setItem(i,1,item2);
        PlanModel->setItem(i,2,item3);

        ui->tableView->setIndexWidget(PlanModel->index(i,3),GetIndex_tableWidget(i));
    }
}
#include<QMessageBox>
#include"cmsgbox.h"
#include<QModelIndexList>
#include"jsj_txservice.h"
void jsj_MultiPlandlg::ClickedBtnSlot()
{
    QPushButton *btn = (QPushButton *)sender();   //产生事件的对象
    int row = btn->property("row").toInt();  //取得按钮的行号属性
    int type = btn->property("type").toInt();  //获取按钮的fileName属性，其他的可以自行添加。
    if(type == Edit)
    {
        QSize size(1515,732);
        dlg = new jsj_adp(size);
        dlg->Node = DBSERVICES.PlanNodeList[row];
        dlg->CurDirect = dlg->Node->Direct;
        dlg->SetInit_Info(type);
        dlg->setAttribute(Qt::WA_DeleteOnClose);
        dlg->show();
    }
    else if (type == Del)
    {
        int mb = CMsgBox::showMsgBox("是否删除选择项?",this);
        if (mb == 0x00)
        {
            if(SelectRow==-1)
            {
                CMsgBox::showMsgBox("请先选中当前行,再点击删除按钮!",this);
                return;
            }
            QString Name = PlanModel->item(SelectRow,0)->text();
            DelInfoByPlanName(Name);
            PlanModel->removeRow(SelectRow);
        }
    }
    else if (type ==Excute)
    {
        int mb = CMsgBox::showMsgBox("是否执行当前阶段计划?",this);
        if (mb == 0x00)
        {
            if(DBSERVICES.PlanNodeList[row]->Status == N_ZX)
            {
                btn->setText("取消执行");
                DBSERVICES.PlanNodeList[row]->Status = Y_ZX;
                TX_Send_SERVICES.Send_MultiPlan_toCTC(*DBSERVICES.PlanNodeList[row],CTC);
                TX_Send_SERVICES.Send_MultiPlan_toCTC(*DBSERVICES.PlanNodeList[row],LS_INFO);
                PlanModel->item(row,2)->setText("已下发");
            }
            else if (DBSERVICES.PlanNodeList[row]->Status == Y_ZX)
            {
                btn->setText("执行");
                DBSERVICES.PlanNodeList[row]->Status = N_ZX;
                PlanModel->item(row,2)->setText("未下发");
            }
        }
    }
}

void jsj_MultiPlandlg::LoadMultiPlan(QString Path)
{
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
    }else if(Path.endsWith("xlsx",Qt::CaseInsensitive)){
        book = xlCreateXMLBook();
    }
    else
    {
        return;
    }
    book->setKey(L"GCCG", L"windows-282123090cc0e6036db16b60a1o3p0h9");
    qDebug()<<Path.utf16();
    bool f = book->load( reinterpret_cast<const wchar_t *>(Path.utf16()));
    if(!f)
        return;
    int Count = book->sheetCount();
    for(int i = 0; i < Count;++i)
    {
        jsj_planNode * Node = new jsj_planNode;
        libxl::Sheet * sheetread = book->getSheet(i);
        if(sheetread)
        {
            Node->PlanName = QString::fromWCharArray(sheetread->name());
            for(int ii = 2; ii < sheetread->lastRow();++ii)
            {
                Node->PlanName = QString::fromWCharArray(sheetread->readStr(ii, 1));
                Node->YunxingLeixing = QString::fromWCharArray(sheetread->readStr(ii, 2));
                Node->ChaoXianInfo = QString::fromWCharArray(sheetread->readStr(ii,8));
                Node->QY_type = QString::fromWCharArray(sheetread->readStr(ii,9));
                Node->Checi_Type = QString::fromWCharArray(sheetread->readStr(ii,10));
                Node->Checi = QString::fromWCharArray(sheetread->readStr(ii, 1));
                Node->tempPlan.StationName =  QString::fromWCharArray(sheetread->readStr(ii, 0));
                Node->tempPlan.CheCi =  QString::fromWCharArray(sheetread->readStr(ii, 1));
                Node->tempPlan.jfc_TYPE =  QString::fromWCharArray(sheetread->readStr(ii, 2));
                Node->tempPlan.GDSetName =  QString::fromWCharArray(sheetread->readStr(ii, 3));
                Node->tempPlan.ArriveTime =  QString::fromWCharArray(sheetread->readStr(ii, 4));
                Node->tempPlan.ChuFaTime =  QString::fromWCharArray(sheetread->readStr(ii, 5));
                Node->tempPlan.JieChe_Direct =  QString::fromWCharArray(sheetread->readStr(ii, 6));
                Node->tempPlan.FaChe_Direct  =  QString::fromWCharArray(sheetread->readStr(ii, 7));
                Node->tempPlan.CX_Info = QString::fromWCharArray(sheetread->readStr(ii,9));
                Node->tempPlan.QY_Type = QString::fromWCharArray(sheetread->readStr(ii,10));
                Node->tempPlan.CC_TYPE = QString::fromWCharArray(sheetread->readStr(ii,11));
                Node->CurMultiPlanInfo.push_back(Node->tempPlan);
            }
        }
        Node->isGet = true;
        DBSERVICES.PlanNodeList.push_back(Node);
    }
}
void jsj_MultiPlandlg::on_AddButton_3_clicked()
{
    emit DBSERVICES.ShowMainAnotherTwo();//增加接口 因为后面两个模块返回主界面与前面两模块触发方式不同
}
void jsj_MultiPlandlg::UpdateEditInfo(unsigned int operate)
{
    if(operate == Edit)
    {
        for(int i = 0; i < DBSERVICES.PlanNodeList.size();++i)
        {
            QString Gather_Info = Get_Add_StrInfo(i);

            PlanModel->setData(PlanModel->index(i,1),Gather_Info);
            PlanModel->item(i,1)->setTextAlignment(Qt::AlignCenter);
        }
    }
    else if (operate == APPEND)
    {
        int Size = DBSERVICES.PlanNodeList.size();
        QStandardItem * item1 = new QStandardItem(DBSERVICES.PlanNodeList[Size - 1]->PlanName);
        QStandardItem * item2 = new QStandardItem(Get_Add_StrInfo(Size-1));
        QStandardItem * item3 = new QStandardItem("未下发");

        item1->setTextAlignment(Qt::AlignCenter);
        item2->setTextAlignment(Qt::AlignCenter);
        item3->setTextAlignment(Qt::AlignCenter);


        PlanModel->setItem(Size - 1,0,item1);
        PlanModel->setItem(Size - 1,1,item2);
        PlanModel->setItem(Size - 1,2,item3);


        ui->tableView->setIndexWidget(PlanModel->index(Size - 1,3),GetIndex_tableWidget(Size - 1));
    }
}

void jsj_MultiPlandlg::on_tableView_clicked(const QModelIndex &index)
{
    SelectRow = index.row();
}
