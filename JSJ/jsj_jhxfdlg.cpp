#include "jsj_jhxfdlg.h"
#include "ui_jsj_jhxfdlg.h"
#include<QBrush>

jsj_jhxfdlg::jsj_jhxfdlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_jhxfdlg)
{
    ui->setupUi(this);
    DBSERVICES.SetBackImage(this,"背景二");
    ui->cancleBtn->hide();
    ui->JIhuaNumber_Edit->setEnabled(true);
    ui->JIhuaNumber_Edit->setText(QString::number( DBSERVICES.generateRandomNumber()));
    ui->checi_Edit->setText(QString("T%1").arg(QString::number( DBSERVICES.generateRandomNumber())));
    ExamDetailModel = new QStandardItemModel(ui->tableView);
    DownDetailModel = new QStandardItemModel(ui->tableView_2);
    QStringList strHeader;
    strHeader.append(("序号"));
    strHeader.append("计划号");
    strHeader.append(("目标车站"));
    strHeader.append(("到达车次"));
    strHeader.append(("出发车次"));
    strHeader.append(("到达股道"));
    strHeader.append(("到达时间"));
    strHeader.append(("出发股道"));
    strHeader.append(("出发时间"));
    strHeader.append(("牵引类型"));
    strHeader.append(("超限信息"));
    strHeader.append(("列车类型"));
    strHeader.append(("运行类型"));
    DBSERVICES.Init(ui->tableView,ExamDetailModel,strHeader,false);
    DBSERVICES.Init(ui->tableView_2,DownDetailModel,strHeader,false);
    QString style = "::section{Background-color:rgb(0,156,255);"
                    "color:#ffffff;}";
    ui->tableView->horizontalHeader()->setStyleSheet(style);
     ui->tableView_2->horizontalHeader()->setStyleSheet(style);
    QFont font = QFont("微软雅黑", 14);
    ui->tableView->horizontalHeader()->setFont(font);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView_2->horizontalHeader()->setFont(font);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->yxlx_Combo->addItem("快速旅客列车",KSLK);
    ui->yxlx_Combo->addItem("普通旅客列车",PTLK);
    ui->yxlx_Combo->addItem("货物列车",HWLC);
    ui->yxlx_Combo->addItem("行包列车",XBLC);
    ui->yxlx_Combo->addItem("动车组",DCZ);
    ui->yxlx_Combo->addItem("特快货物列车",TKHWLC);
    ui->yxlx_Combo->addItem("快速货物列车",KSHWLC);
    ui->yxlx_Combo->addItem("特别旅客快车",TBLK);

    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(!DBSERVICES.CurBackInfoList[i].isAppend())
            continue;
        ui->ChezhanCombo->addItem(DBSERVICES.CurBackInfoList[i].StationName,
                                  DBSERVICES.CurBackInfoList[i].StationId);
    }

    ui->cx_combo->addItem("无超限",NO_CX);
    ui->cx_combo->addItem("一级超限",ONE_CX);
    ui->cx_combo->addItem("二级超限",TWO_CX);
    ui->cx_combo->addItem("三级超限",THREE_CX);
    ui->cx_combo->addItem("超级超限",Super_CX);

    ui->qytype_Combo->addItem("电力",Eletric);
    ui->qytype_Combo->addItem("非电力",UN_Eletric);

    ui->jfc_TypeCombo->addItem("接发车",ZC_);
    ui->jfc_TypeCombo->addItem("始发",SF_);
    ui->jfc_TypeCombo->addItem("终到",ZD_);
    ui->jfc_TypeCombo->addItem("通过",TG_);

    ui->fachetime_->setEnabled(false);
    ui->arrive_->setEnabled(false);

    ui->timeEdit->setTime(QTime::currentTime());
    ui->timeEdit_2->setTime(QTime::currentTime());

    ui->fachetime_->setDate(QDate::currentDate());
    ui->arrive_->setDate(QDate::currentDate());

    for(int i = 0; i < 13;++i)
    {
        QStandardItem * item_1 = new QStandardItem();
        item_1->setTextAlignment(Qt::AlignCenter);
        CurItemList.push_back(item_1);
    }
    ui->tableView->verticalHeader()->setDefaultSectionSize(70);        //设置行间距
    ui->tableView->verticalHeader()->setMinimumSectionSize(70);        //设置行间距

    ui->tableView->setSortingEnabled(true);
    ui->tableView->sortByColumn(0,Qt::DescendingOrder);

    ui->tableView_2->verticalHeader()->setDefaultSectionSize(70);        //设置行间距
    ui->tableView_2->verticalHeader()->setMinimumSectionSize(70);        //设置行间距

    ui->tableView_2->setSortingEnabled(true);
    ui->tableView_2->sortByColumn(0,Qt::DescendingOrder);

    connect(&TX_Send_SERVICES,SIGNAL(Set_jdjh_Slot(QString,QString)),this,SLOT(ChangeMLState(QString,QString)));

}

jsj_jhxfdlg::~jsj_jhxfdlg()
{
    delete ui;
}

void jsj_jhxfdlg::ClearList()
{
    for(int i = 0; i < CurItemList.size();++i)
    {
        CurItemList[i] = nullptr;
        CurItemList[i] = new  QStandardItem();
        CurItemList[i]->setTextAlignment(Qt::AlignCenter);
    }
}

void jsj_jhxfdlg::on_yxlx_Combo_currentIndexChanged(int index)
{
    ui->CCLX_Combo->clear();
    if(index == KSLK || index == PTLK || index == DCZ || index == TBLK)
    {
        for(int i = 0; i < DBSERVICES.CarDetailList.size();++i)
        {
            if(DBSERVICES.CarDetailList[i].TrainTyep == "客车")
            {
                for(int ii = 0 ; ii < DBSERVICES.CarDetailList[i].TrainList.size();++ii)
                {
                    ui->CCLX_Combo->addItem(DBSERVICES.CarDetailList[i].TrainList[ii].Name,
                                            DBSERVICES.CarDetailList[i].TrainList[ii].index);
                }
            }
        }
    }else
    {
        for(int i = 0; i < DBSERVICES.CarDetailList.size();++i)
        {
            if(DBSERVICES.CarDetailList[i].TrainTyep == "货车")
                for(int ii = 0 ; ii < DBSERVICES.CarDetailList[i].TrainList.size();++ii)
                {
                    ui->CCLX_Combo->addItem(DBSERVICES.CarDetailList[i].TrainList[ii].Name,
                                            DBSERVICES.CarDetailList[i].TrainList[ii].index);
                }
        }
    }
}
void jsj_jhxfdlg::setNull(QTimeEdit*e)
{

    QLineEdit *edit = e->findChild<QLineEdit*>("qt_spinbox_lineedit");
    edit->setText("");
    update();
}

void jsj_jhxfdlg::SetBack(QColor zor)
{
    for(int i = 0 ; i < CurItemList.size();++i)
        CurItemList[i]->setForeground(QBrush(zor));
}

void jsj_jhxfdlg::SetSatisified_Info()
{
    int CurId = ui->ChezhanCombo->currentData().toInt();
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        if(DBSERVICES.CurBackInfoList[i].StationId == CurId)
        {
            ui->gudaoCombo->clear();
            ui->jcfx_combo->clear();
            ui->fcfx_combo->clear();
            CurList = DBSERVICES.CurBackInfoList[i].TGList;
            for(int z = 0;z<CurList.size();++z)
            {
                ui->jcfx_combo->addItem(CurList[z].JC_Info);
            }
        }
    }
}

void jsj_jhxfdlg::InitJh_StationInfo()
{
    int flag=-1;
    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        flag =DBSERVICES.isExistCombo(ui->ChezhanCombo,DBSERVICES.CurBackInfoList[i].StationId);
        if(!DBSERVICES.CurBackInfoList[i].isAppend())
        {
            if(flag!=-1)
            {
                ui->ChezhanCombo->removeItem(flag);
            }
        }
        else
        {
            if(flag == -1)
            {
                ui->ChezhanCombo->addItem(DBSERVICES.CurBackInfoList[i].StationName,
                                          DBSERVICES.CurBackInfoList[i].StationId);
            }

        }
    }
}


void jsj_jhxfdlg::on_jfc_TypeCombo_currentIndexChanged(int index)
{
    ui->jcfx_combo->clear();
    ui->fcfx_combo->clear();
    ui->gudaoCombo->clear();
    int CurData = ui->jfc_TypeCombo->currentData().toInt();
    if(CurData ==JFC)
    {
        ui->timeEdit->setEnabled(true);
        ui->timeEdit_2->setEnabled(true);
        ui->gudaoCombo->setEnabled(true);
        ui->jcfx_combo->setEnabled(true);
        ui->fcfx_combo->setEnabled(true);
        ui->timeEdit->setTime(QTime::currentTime());
        ui->timeEdit_2->setTime(QTime::currentTime());
    }
    else if (CurData == SF)
    {
        ui->gudaoCombo->setEnabled(true);
        ui->jcfx_combo->setEnabled(false);
        ui->fcfx_combo->setEnabled(true);
        ui->timeEdit->setEnabled(true);
        ui->timeEdit_2->setEnabled(false);
        setNull(ui->timeEdit_2);
    }
    else if(CurData == ZD)
    {
        ui->gudaoCombo->setEnabled(true);
        ui->jcfx_combo->setEnabled(true);
        ui->fcfx_combo->setEnabled(false);
        ui->timeEdit->setEnabled(false);
        ui->timeEdit_2->setEnabled(true);
        setNull(ui->timeEdit);
    }
    if(CurData == JFC || CurData == SF || CurData == ZD)
    {
        for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
        {
            ui->gudaoCombo->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
        }
        for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
            ui->fcfx_combo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
        for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
            ui->jcfx_combo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
    }
    else if(CurData == TG)
    {
        ui->gudaoCombo->setEnabled(false);
        ui->jcfx_combo->setEnabled(true);
        ui->fcfx_combo->setEnabled(false);
        ui->timeEdit->setEnabled(true);
        ui->timeEdit_2->setEnabled(true);
        ui->timeEdit->setTime(QTime::currentTime());
        ui->timeEdit_2->setTime(QTime::currentTime());
        SetSatisified_Info();
    }
}

void jsj_jhxfdlg::sortByColumn(int column)
{
    int Size = ExamDetailModel->rowCount();
    for(int i = 0; i < ExamDetailModel->rowCount();++i)
    {

        ExamDetailModel->setData(ExamDetailModel->index(i,0),Size);
        Size--;
    }

    if (!ui->tableView->model())
        return;
    Qt::SortOrder order =Qt::DescendingOrder;
    ui->tableView->horizontalHeader()->setSortIndicator(column, order);
    ui->tableView->model()->sort(column, order);
}
#include"cmsgbox.h"
void jsj_jhxfdlg::on_AddButton_clicked()
{
    int Row = -1;
    tempJHINFO.jh_PlanNumber = ui->JIhuaNumber_Edit->text();
    tempJHINFO.jh_TargetStationName = ui->ChezhanCombo->currentText();
    tempJHINFO.jh_ArriveCheci = ui->checi_Edit->text();
    tempJHINFO.jh_ChufaCheci = ui->checi_Edit->text();
    tempJHINFO.jh_ArriveGD = ui->gudaoCombo->currentText();
    tempJHINFO.GD_Index = ui->gudaoCombo->currentData().toUInt();
    tempJHINFO.jh_ArriveTime = QString ("%1  %2").arg(ui->arrive_->text())
            .arg(ui->timeEdit->text());

    tempJHINFO.jfc_Type =  ui->jfc_TypeCombo->currentData().toUInt();
    tempJHINFO.jh_ChufaGD = ui->gudaoCombo->currentText();

    tempJHINFO.jh_ChuFaTime =   QString ("%1  %2").arg(ui->fachetime_->text())
            .arg(ui->timeEdit_2->text());

    tempJHINFO.JC_Str = ui->jcfx_combo->currentText();
    tempJHINFO.FC_Str = ui->fcfx_combo->currentText();
    tempJHINFO.JC_Index = ui->jcfx_combo->currentData().toUInt();
    tempJHINFO.Fc_Index = ui->fcfx_combo->currentData().toUInt();

    QDate chufadatime  = ui->fachetime_->date();

    tempJHINFO.ChuFaTime[0] = chufadatime.year();
    tempJHINFO.ChuFaTime[1] = chufadatime.month();
    tempJHINFO.ChuFaTime[2] = chufadatime.day();
    QTime chufatime = ui->timeEdit_2->time();
    tempJHINFO.ChuFaTime[3] = chufatime.hour();
    tempJHINFO.ChuFaTime[4] = chufatime.minute();
    tempJHINFO.ChuFaTime[5] = chufatime.second();


    QDate Arrivedatime  = ui->arrive_->date();
    tempJHINFO.Arrivetime[0] = Arrivedatime.year();
    tempJHINFO.Arrivetime[1] = Arrivedatime.month();
    tempJHINFO.Arrivetime[2] = Arrivedatime.day();
    QTime Arrivetime = ui->timeEdit->time();
    tempJHINFO.Arrivetime[3] = Arrivetime.hour();
    tempJHINFO.Arrivetime[4] = Arrivetime.minute();
    tempJHINFO.Arrivetime[5] = Arrivetime.second();


    tempJHINFO.jh_QyType = ui->qytype_Combo->currentText();
    tempJHINFO.qytype = ui->qytype_Combo->currentData().toUInt();
    tempJHINFO.jh_ChaoXInfo = ui->cx_combo->currentText();
    tempJHINFO.cxtype = ui->cx_combo->currentData().toUInt();
    tempJHINFO.jh_TrainType = ui->CCLX_Combo->currentText();
    tempJHINFO.StationId = ui->ChezhanCombo->currentData().toUInt();
    for(int i = 0; i < DBSERVICES.CarDetailList.size();++i)
    {
        CarInfo temp = DBSERVICES.CarDetailList[i];
        for(int z = 0 ; z < temp.TrainList.size();++z)
        {
            if(temp.TrainList[z].Name.contains(tempJHINFO.jh_TrainType))
            {
                if(temp.TrainTyep == "客车")
                    tempJHINFO.GetKind = 0x01;
                else if (temp.TrainTyep == "货车")
                    tempJHINFO.GetKind = 0x00;
                break;
            }

        }

    }
    tempJHINFO.YunxinType = ui->yxlx_Combo->currentData().toUInt();
    tempJHINFO.jh_YxType = ui->yxlx_Combo->currentText();
    tempJHINFO.TrainType = ui->CCLX_Combo->currentData().toUInt();
    QColor lor;
    if((tempJHINFO.jh_ArriveCheci.contains("T")||
        tempJHINFO.jh_ArriveCheci.contains("D")||
        tempJHINFO.jh_ArriveCheci.contains("G")||
        tempJHINFO.jh_ArriveCheci.contains("Z")||
        tempJHINFO.jh_ArriveCheci.contains("K"))
            ||tempJHINFO.jh_ArriveCheci.size()==0x04)
        lor.setRgb(255,0,0);
    else
        lor.setRgb(0,170,255);
    SetBack(lor);
    for(int i = 0; i < DBSERVICES.JHList.size();++i)
    {
        if( DBSERVICES.JHList[i].jh_PlanNumber ==tempJHINFO.jh_PlanNumber )
        {
            CMsgBox::showMsgBox("当前计划号已存在,请检查");
            return;
        }
    }
    int RowCout = ExamDetailModel->rowCount();
    CurItemList[0]->setText(QString::number(RowCout));
    CurItemList[1]->setText(tempJHINFO.jh_PlanNumber);
    CurItemList[2]->setText(tempJHINFO.jh_TargetStationName);
    CurItemList[3]->setText(tempJHINFO.jh_ArriveCheci);
    CurItemList[4]->setText(tempJHINFO.jh_ChufaCheci);
    CurItemList[5]->setText(tempJHINFO.jh_ArriveGD);
    CurItemList[6]->setText(tempJHINFO.jh_ArriveTime);
    CurItemList[7]->setText(tempJHINFO.jh_ChufaGD);
    CurItemList[8]->setText(tempJHINFO.jh_ChuFaTime);
    CurItemList[9]->setText(tempJHINFO.jh_QyType);
    CurItemList[10]->setText(tempJHINFO.jh_ChaoXInfo);
    CurItemList[11]->setText(tempJHINFO.jh_TrainType);
    CurItemList[12]->setText(tempJHINFO.jh_YxType);
    if(operateType == Edit)
    {
        Row = tempJHINFO.CurIndex;
        for(int z = 0; z < ExamDetailModel->columnCount();++z)
        {
            ExamDetailModel->setItem(Row,z,CurItemList[z]);
        }
        ui->AddButton->setText("添加");
        operateType = Add;
    }else
    {
        Row = ExamDetailModel->rowCount();
        tempJHINFO.CurIndex = Row;
        DBSERVICES.JHList.push_back(tempJHINFO);
          ExamDetailModel->insertRow(0,CurItemList);
    }
    ClearList();
    ui->tableView->setModel(ExamDetailModel);
    ui->JIhuaNumber_Edit->setText(QString::number(DBSERVICES.generateRandomNumber()));
    ui->checi_Edit->setText("");
}
#include<QMessageBox>
#include"cmsgbox.h"
void jsj_jhxfdlg::on_DelBtn_clicked()
{
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    if(selected.size() == 0)
    {
        CMsgBox::showMsgBox("请选择需要删除项",this);
        return;
    }
    QMap<int, int> rows;
    foreach (QModelIndex index, selected)
        rows.insert(index.row(), 0);
    QMapIterator<int, int> r(rows);
    r.toBack();
    while (r.hasPrevious()){
        r.previous();
        ExamDetailModel->removeRow(r.key());
    }
    sortByColumn(0);
}
void jsj_jhxfdlg::on_ChezhanCombo_currentTextChanged(const QString &arg1)
{
    DBSERVICES.info.initInfo();
    int id = ui->ChezhanCombo->currentData().toInt();

    QString StationName = "";
    if(id == 0x00)
        StationName = "Station";
    else
        StationName = QString("Station%1").arg(id);
    emit DBSERVICES.GetGDInfo(StationName);

    ui->gudaoCombo->clear();
    ui->jcfx_combo->clear();
    ui->fcfx_combo->clear();
    for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
        ui->gudaoCombo->addItem(DBSERVICES.info.GDL[i].GDName,DBSERVICES.info.GDL[i].GDIndex);
    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
        ui->fcfx_combo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
        ui->jcfx_combo->addItem(DBSERVICES.info.JCL[i].JCName,DBSERVICES.info.JCL[i].JCIndex);
}

void jsj_jhxfdlg::on_cancleBtn_clicked()
{
    emit DBSERVICES.ShowMainAnotherTwo();
}

unsigned int jsj_jhxfdlg::GetIndexByName(QString Name)
{
    for(int i = 0; i < DBSERVICES.info.GDL.size();++i)
    {
        if(DBSERVICES.info.GDL[i].GDName == Name)
            return DBSERVICES.info.GDL[i].GDIndex;
    }
    for(int i = 0; i < DBSERVICES.info.JCL.size();++i)
    {
        if(DBSERVICES.info.JCL[i].JCName == Name)
            return DBSERVICES.info.JCL[i].JCIndex;
    }
    for(int i = 0; i < DBSERVICES.info.FCL.size();++i)
    {
        if(DBSERVICES.info.FCL[i].FCName == Name)
            return DBSERVICES.info.FCL[i].FCIndex;
    }
}
void jsj_jhxfdlg::on_jcfx_combo_textActivated(const QString &arg1)
{

    int Curdata = ui->jfc_TypeCombo->currentData().toUInt();
    if(Curdata==TG)
    {
        ui->gudaoCombo->clear();
        ui->fcfx_combo->clear();
        for(int i = 0; i < CurList.size();++i)
        {
            if(CurList[i].JC_Info == arg1)
            {
                ui->gudaoCombo->addItem(CurList[i].GD_Info,GetIndexByName(CurList[i].GD_Info));
                ui->fcfx_combo->addItem(CurList[i].FC_Info,GetIndexByName(CurList[i].FC_Info));
            }
        }
    }
    //TG近路单独处理
    ////////////////////////////////////////////////////////////////////////////////////////
    else
    {
        if(arg1=="")
            return;
        if(arg1.contains("S") || arg1.contains("X"))
        {

            QList<Jc_XHJ> tempList = DBSERVICES.info.JCL;
            QString str = arg1.left(1);
            for(int i = 0 ; i < tempList.size();++i)
            {
                if(tempList[i].JCName.contains(str))
                {
                    tempList.removeAt(i);
                    i--;
                }
            }
            ui->fcfx_combo->clear();
            for(int z = 0; z < tempList.size();++z)
            {
                ui->fcfx_combo->addItem(tempList[z].JCName,tempList[z].JCIndex);
            }

        }
    }
}

void jsj_jhxfdlg::on_fcfx_combo_textActivated(const QString &arg1)
{
    qDebug()<<"FCFXINDEX:"<<ui->fcfx_combo->currentData().toInt();
    if(arg1=="")
        return;
    if(arg1.contains("S") || arg1.contains("X"))
    {
        QList<Jc_XHJ>tempList = DBSERVICES.info.JCL;
        QString str = arg1.left(1);
        for(int i = 0 ; i < tempList.size();++i)
        {
            Jc_XHJ temp = tempList[i];
            if(temp.JCName.contains(str) || temp.JCName== str)
            {
                tempList.removeAt(i);
                i--;
            }
        }
    }
}

void jsj_jhxfdlg::on_MLXFBtn_clicked()
{
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    QModelIndexList selected = selections->selectedRows();
    if(selected.size() == 0)
    {
        CMsgBox::showMsgBox("请选择需要下发项",this);
        return;
    }
    int Var = selected.size();
    for(int i = 0; i < Var;++i)
    {
        QModelIndex index = selected[i];
        QString indexs =  ExamDetailModel->item(index.row(),0)->text();
        QString Plan_Number = ExamDetailModel->item(index.row(),1)->text();

        JHINFO temp;
        for(int z = 0 ; z < DBSERVICES.JHList.size();++z)
        {
            if(DBSERVICES.JHList[z].jh_PlanNumber==Plan_Number)
            {
                temp = DBSERVICES.JHList[z];
                TX_Send_SERVICES.Send_Lone_StationJHto_CTC(temp,CTC);
                TX_Send_SERVICES.Send_Lone_StationJHto_CTC(temp,LS_INFO);
                break;
            }
        }
        CurItemList[0]->setText(indexs);
        CurItemList[1]->setText(temp.jh_PlanNumber);
        CurItemList[2]->setText(temp.jh_TargetStationName);
        CurItemList[3]->setText(temp.jh_ArriveCheci);
        CurItemList[4]->setText(temp.jh_ChufaCheci);
        CurItemList[5]->setText(temp.jh_ArriveGD);
        CurItemList[6]->setText(temp.jh_ArriveTime);
        CurItemList[7]->setText(temp.jh_ChufaGD);
        CurItemList[8]->setText(temp.jh_ChuFaTime);
        CurItemList[9]->setText(temp.jh_QyType);
        CurItemList[10]->setText(temp.jh_ChaoXInfo);
        CurItemList[11]->setText(temp.jh_TrainType);
        CurItemList[12]->setText(temp.jh_YxType);
        DownDetailModel->insertRow(0,CurItemList);
        ClearList();

    }
}
void jsj_jhxfdlg::ChangeMLState(QString s1, QString s2)
{
    int rows = ExamDetailModel->rowCount();
    for(int r = 0; r < rows; r++)
    {
        QString Plan =  ExamDetailModel->index(r,1).data().toString();
        if(Plan == s1)
        {
            for(int i = 0; i < ExamDetailModel->columnCount();++i)
            {
                ExamDetailModel->setData(ExamDetailModel->index(r,i),QBrush(Qt::blue),Qt::TextColorRole);
            }
            break;
        }
    }
}

void jsj_jhxfdlg::on_tableView_doubleClicked(const QModelIndex &index)
{
    QItemSelectionModel *selections = ui->tableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();
    for(int i = 0; i < selected.size();++i)
    {
        QModelIndex index = selected[i];
        int Row = index.row();
        tempJHINFO = DBSERVICES.JHList[Row];
        ui->JIhuaNumber_Edit->setText(tempJHINFO.jh_PlanNumber);
        ui->ChezhanCombo->setCurrentText(tempJHINFO.jh_TargetStationName);
        ui->checi_Edit->setText(tempJHINFO.jh_ArriveCheci);
        ui->gudaoCombo->setCurrentText(tempJHINFO.jh_ChufaGD);
        QStringList ArriveDatetime =tempJHINFO.jh_ArriveTime.split("  ");
        if(ArriveDatetime.size()>=2)
        {
            ui->arrive_->setDate(QDate::fromString(ArriveDatetime[0],"yyyy/M/d"));
            ui->timeEdit->setTime(QTime::fromString(ArriveDatetime[1],"hh:mm"));
        }
        QStringList ChufaDatetime =tempJHINFO.jh_ChuFaTime.split("  ");
        if(ChufaDatetime.size()>=2)
        {
            ui->fachetime_->setDate(QDate::fromString(ChufaDatetime[0],"yyyy/M/d"));
            ui->timeEdit_2->setTime(QTime::fromString(ChufaDatetime[1],"hh:mm"));
        }
        ui->qytype_Combo->setCurrentText(tempJHINFO.jh_QyType);
        ui->cx_combo->setCurrentText(tempJHINFO.jh_ChaoXInfo);
        ui->CCLX_Combo->setCurrentText(tempJHINFO.jh_TrainType);
        ui->yxlx_Combo->setCurrentText(tempJHINFO.jh_YxType);
        ui->jcfx_combo->setCurrentText(tempJHINFO.JC_Str);
        ui->fcfx_combo->setCurrentText(tempJHINFO.FC_Str);
    }
    ui->AddButton->setText("确认修改");
    operateType = Edit;
}
