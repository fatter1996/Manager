#include "jsj_ddmldlg.h"
#include "ui_jsj_ddmldlg.h"
#include"jsj_services.h"
#include<QToolBar>
#include"jsj_receiveslot.h"
#include<QDesktopWidget>
JSJ_ddmldlg::JSJ_ddmldlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::JSJ_ddmldlg)
{
    ui->setupUi(this);





    DBSERVICES.SetBackImage(this,"背景二");
    QRect trc = QApplication::desktop()->availableGeometry();
    trc.setHeight(trc.height()-5);
    setMaximumSize(trc.size());
    setMinimumSize(trc.size());

    ui->ml_CloseBtn_2->hide();
    setWindowTitle("命令管理");
    QStringList strHeader;
    strHeader <<("命令号")<<("命令标题")<<("命令内容");
    DBSERVICES.InitTable(strHeader,ui->HC_tableWidget);
    DBSERVICES.InitTable(strHeader,ui->Send_tableWidget);
    strHeader.clear();
    strHeader <<("命令号")<<("标题")<<("内容")<<("接收时间")<<("签收人");
    DBSERVICES.InitTable(strHeader,ui->Finish_tableWidget);

    connect(&DBSERVICES,SIGNAL(ClearAllWidgets(int)),this,SLOT(UpdateAllDDML(int)));

    strHeader.clear();
    strHeader <<("命令类型")<< ("接令对象") << ("发送时间")<<("接令状态");
    DBSERVICES.InitTable(strHeader,ui->ml_showTable);
    ui->ml_showTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//单独处理

    DBSERVICES.GetDdListBytime();

    m_xsmldlg = new jsj_xsmldlg();
    m_ddmldlg = new ddml_ddlg();
    m_jhxfdlg = new jsj_jhxfdlg();
    multiPlandlg = new jsj_MultiPlandlg();
    m_jhxfdlg->setAttribute(Qt::WA_DeleteOnClose);
    m_ddmldlg->setAttribute(Qt::WA_DeleteOnClose);
    m_xsmldlg->setAttribute(Qt::WA_DeleteOnClose);
    multiPlandlg->setAttribute(Qt::WA_DeleteOnClose);

    InitTabelInfo();
    connect(&DBSERVICES,SIGNAL(Save_Info(unsigned int,unsigned int)),
            m_ddmldlg,SLOT(GetMsgSave(unsigned int,unsigned int)));
    connect(&DBSERVICES,SIGNAL(Send_DD_Info(DDML,unsigned int,bool)),m_ddmldlg,
            SLOT(GetMsgUpdate(DDML,unsigned int,bool)));

    connect(&DBSERVICES,SIGNAL(Save_Info(unsigned int,unsigned int)),m_xsmldlg,
            SLOT(GetMsgSave_XSML(unsigned int,unsigned int)));

    connect(&DBSERVICES,SIGNAL(Send_DD_Info(DDML,unsigned int,bool)),m_xsmldlg,
            SLOT(XSMLMsgUpdate(DDML,unsigned int,bool)));

    connect(&DBSERVICES,SIGNAL(ShowMainAnotherTwo()),this,SLOT(ShoMainT()));

    connect(m_xsmldlg,SIGNAL(SetSelectStation(QString)),this,SLOT(SetcheckedStation(QString)));

    connect(&DBSERVICES,SIGNAL(UpdateStation()),this,SLOT(UpdateStationInfo()));

    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);

    ui->tabWidget->insertTab(XSMLINFO,m_xsmldlg,"限速命令");
    ui->tabWidget->insertTab(DDMLINFO,m_ddmldlg,"调度命令");
    ui->tabWidget->insertTab(JHXFINFO,m_jhxfdlg,"单站阶段计划");
    ui->tabWidget->insertTab(DRAWINFO,multiPlandlg,"多站阶段计划");
    ui->tabWidget->setCurrentIndex(DDMLINFO);



    ui->HC_tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);

    //右键
    m_tabMenu = new QMenu(ui->HC_tableWidget);
    action= new QAction("设置",this);
    action->setCheckable(true);
    m_tabMenu->addAction(action);

    connect(action,SIGNAL(triggered(bool)),this,SLOT(slotActionInsert()));
    on_tabWidget_currentChanged(DDMLINFO);

    ui->HC_tableWidget->setMouseTracking(true);//开启捕获鼠标功能
    ui->Send_tableWidget->setMouseTracking(true);
    connect(ui->HC_tableWidget,SIGNAL(cellEntered(int, int)),this,SLOT(slotCellEnter(int , int )));
    connect(ui->Send_tableWidget,SIGNAL(cellEntered(int, int)),this,SLOT(slotCellEnter_Send(int , int )));

    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);

    ui->HC_tableWidget->viewport()->installEventFilter(this);

    ui->Send_tableWidget->viewport()->installEventFilter(this);

    connect(ui->ml_showTable, SIGNAL(cellClicked(int,int)), this, SLOT(changeTest(int,int)));
    connect(&TX_Send_SERVICES,SIGNAL(SetDDML_Slot(QString)),this,SLOT(SetCurStatus(QString)));

    m_xsmldlg->InitStationInfo();//初始化限速命令战场图

}
#include<QCheckBox>
void JSJ_ddmldlg::InitTabelInfo()
{
    for(int i = 0 ; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        int t_rowCount = ui->ml_showTable->rowCount();
        ui->ml_showTable->insertRow(t_rowCount);
        QTableWidgetItem * item_ = new QTableWidgetItem("");
        QTableWidgetItem * item_1 = new QTableWidgetItem("00:00");
        QTableWidgetItem * item_2 = new QTableWidgetItem("可接收");
        QTableWidgetItem *checkBox = new QTableWidgetItem(DBSERVICES.CurBackInfoList[i].StationName);
        checkBox->setCheckState(Qt::Unchecked);
        ui->ml_showTable->setItem(t_rowCount,0,item_);
        ui->ml_showTable->setItem(t_rowCount,1,checkBox);
        ui->ml_showTable->setItem(t_rowCount,2,item_1);
        ui->ml_showTable->setItem(t_rowCount,3,item_2);
        ui->ml_showTable->setRowHidden(t_rowCount,true);
    }
}
JSJ_ddmldlg::~JSJ_ddmldlg()
{
    delete ui;
}
#include"jsj_maindlg.h"
#include<QDebug>
void JSJ_ddmldlg::on_ml_CloseBtn_2_clicked()
{

    ShoMainT();
}

void JSJ_ddmldlg::on_ml_SendBtn_2_clicked()
{

    QList<QTableWidgetItem*> items;
    items = ui->HC_tableWidget->selectedItems();
    if(ui->tabWidget->currentIndex() == DDMLINFO)
    {
        //首先判断缓存箱有没有被选中的 。在看当前是否有发送的
        if(!items.empty()){
            QString Number = items.at(0)->text();
            int index =  DBSERVICES.getSendInfoByNumber(Number);
            TX_Send_SERVICES.Send_Currentto_CTC(DBSERVICES.SavedList[index]);
            DBSERVICES.reFreshAllStatus(items.at(0)->text(),Sended);
            emit DBSERVICES.ClearAllWidgets(ui->tabWidget->currentIndex());
        }
        else{
            CMsgBox::showMsgBox("请选择需要下发的命令");
            return;
        }
    }else
    {
        if(!items.empty()){
            QString Number = items.at(0)->text();
            int index =  DBSERVICES.getSendInfoByNumber(Number);
            //在这里直接发送(分情况 是设置限速还是取消限速)
            {
                DBSERVICES.SavedList[index].XsIsSend = true;
                isSet = 0x11;
                TX_Send_SERVICES.Send_xsmlto_CTC(DBSERVICES.SavedList[index],isSet,0x11);
            }
            DBSERVICES.reFreshAllStatus(items.at(0)->text(),Sended);
            emit DBSERVICES.ClearAllWidgets(ui->tabWidget->currentIndex());
        }else{
            CMsgBox::showMsgBox("请选择需要下发的命令");
            return;
        }
    }
    QSR_List.clear();
    ClearState();
    ChangeIndexSlot(ui->tabWidget->currentIndex());
    SetCurTime();
}

QString JSJ_ddmldlg::GetItemInfo(int index)
{
    QString str;
    int Size = DBSERVICES.SavedList[index].vect_dcDev.size();
    for (int z = 0 ; z < Size;++z)
    {
        str = str.append(DBSERVICES.SavedList[index].vect_dcDev[z].shebeiName);
        str = str.append("|");
    }
    str = str.remove(str.size()-1,1);
    str.append("\r\n");
    str.append("区间:");
    for (int z = 0 ; z < DBSERVICES.SavedList[index].vect_qjDev.size();++z)
    {
        str = str.append(DBSERVICES.SavedList[index].vect_qjDev[z].shebeiName);
        str = str.append("|");
    }
    str = str.remove(str.size()-1,1);
    str.append("\r\n");
    str.append("股道/无岔:");
    for (int z = 0 ; z < DBSERVICES.SavedList[index].vect_qdDev.size();++z)
    {
        str = str.append(DBSERVICES.SavedList[index].vect_qdDev[z].shebeiName);
        str = str.append("|");
    }

    str = str.remove(str.size()-1,1);
    str.append("\r\n");
    str.append("区域限速");
    return str;
}
void JSJ_ddmldlg::UpdateAllDDML(int type)
{

    qDebug()<<"TTYPE:"<<type;
    DBSERVICES.ClearContent(ui->HC_tableWidget);
    DBSERVICES.ClearContent(ui->Send_tableWidget);
    DBSERVICES.ClearContent(ui->Finish_tableWidget);
    for(int i = 0; i < DBSERVICES.SavedList.size();++i)
    {
        if(DBSERVICES.SavedList[i].ML_Tpe != type)
            continue;
        else
        {
            if(DBSERVICES.SavedList[i].Status==Send_toBack)
            {

                int RowCout = ui->HC_tableWidget->rowCount();
                ui->HC_tableWidget->insertRow(RowCout);
                ui->HC_tableWidget->setItem(RowCout,0, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Number));
                if(type ==  XSMLINFO)
                {
                    ui->HC_tableWidget->setItem(RowCout,1, new QTableWidgetItem("限速命令"));
                    ui->HC_tableWidget->setItem(RowCout,2, new QTableWidgetItem(GetItemInfo(i)));
                }else
                {
                    ui->HC_tableWidget->setItem(RowCout,1, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Type));
                    ui->HC_tableWidget->setItem(RowCout,2, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Content));
                }

            }
            else if (DBSERVICES.SavedList[i].Status == Sended)
            {
                int RowCout = ui->Send_tableWidget->rowCount();
                ui->Send_tableWidget->insertRow(RowCout);
                ui->Send_tableWidget->setItem(RowCout,0, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Number));
                if(type ==  DDMLINFO)
                {
                    ui->Send_tableWidget->setItem(RowCout,1, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Type));
                    ui->Send_tableWidget->setItem(RowCout,2, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Content));
                }else
                {
                    ui->Send_tableWidget->setItem(RowCout,1, new QTableWidgetItem("限速命令"));
                    ui->Send_tableWidget->setItem(RowCout,2, new QTableWidgetItem(GetItemInfo(i)));
                }
            }
            else if (DBSERVICES.SavedList[i].Status == Finished)
            {
                int Roucout = ui->Finish_tableWidget->rowCount();
                ui->Finish_tableWidget->insertRow(Roucout);
                if(type ==  DDMLINFO)
                {
                    ui->Finish_tableWidget->setItem(Roucout,1, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Type));
                    ui->Finish_tableWidget->setItem(Roucout,2, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Content));
                }else
                {
                    ui->Finish_tableWidget->setItem(Roucout,0,new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Number));
                    ui->Finish_tableWidget->setItem(Roucout,1, new QTableWidgetItem("限速命令"));
                    ui->Finish_tableWidget->setItem(Roucout,2, new QTableWidgetItem(GetItemInfo(i)));
                }
            }
        }
    }
}

void JSJ_ddmldlg::SetSendedInfo(int type)
{
    for(int i = 0; i < DBSERVICES.SavedList.size();++i)
    {
        if(DBSERVICES.SavedList[i].ML_Tpe != type)
            continue;
        else
        {
            QString str = "道岔:";
            if(DBSERVICES.SavedList[i].Status==Sended)
            {
                int RowCout = ui->Send_tableWidget->rowCount();
                ui->Send_tableWidget->insertRow(RowCout);
                ui->Send_tableWidget->setItem(RowCout,0, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Number));
                if(type ==  DDMLINFO)
                {
                    ui->Send_tableWidget->setItem(RowCout,1, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Type));
                    ui->Send_tableWidget->setItem(RowCout,2, new QTableWidgetItem(DBSERVICES.SavedList[i].ML_Content));
                }else
                {
                    ui->Send_tableWidget->setItem(RowCout,1, new QTableWidgetItem("限速命令"));
                    ui->Send_tableWidget->setItem(RowCout,2, new QTableWidgetItem(GetItemInfo(i)));
                }
            }
        }
    }
}
void JSJ_ddmldlg::ChangeIndexSlot(int index)
{
    UpdateAllDDML(index);
}
#include<QMessageBox>
#include"cmsgbox.h"
void JSJ_ddmldlg::on_ml_SaveBtn_2_clicked()
{

    if(QSR_List.size() == 0)
    {
        CMsgBox::showMsgBox("请选择命令签收人");
        return;
    }
    DBSERVICES.GetDDML.ML_QSR = QSR_List;
    if(ui->tabWidget->currentIndex() == DDMLINFO)
        emit DBSERVICES.Save_Info(DDMLINFO,SAVE);
    else
        emit DBSERVICES.Save_Info(XSMLINFO,SAVE);


    QSR_List.clear();
    ClearState();
    ChangeIndexSlot(ui->tabWidget->currentIndex());
    SetCurTime();
}

void JSJ_ddmldlg::changeTest(int row, int col)
{
    if(ui->ml_showTable->item(row, col)->checkState() == Qt::Checked)
    {
        QSR_List.push_back(ui->ml_showTable->item(row, col)->text());
    }
}
void JSJ_ddmldlg::ClearState()
{
    for(int i = 0; i < ui->ml_showTable->rowCount();++i)
    {
        ui->ml_showTable->item(i,1)->setCheckState(Qt::Unchecked);
    }
}

void JSJ_ddmldlg::slot_DL_RcOnTw(QPoint pos)
{

    QTableWidgetItem * m_pSelectItem = ui->HC_tableWidget->itemAt(pos);
    if(m_pSelectItem != nullptr){

        if(action->isChecked())
        {
            action->setText("取消");
            //isSet =  true;
        }else
        {
            action->setText("设置");
            //isSet = false;
        }
        Number = ui->HC_tableWidget->item(m_pSelectItem->row(),0)->text();
        m_tabMenu->exec(QCursor::pos());
        qDebug()<<action->isChecked();
    }
}

void JSJ_ddmldlg::slotActionInsert()
{
    for(int i = 0; i < DBSERVICES.SavedList.size();++i)
    {
        if(DBSERVICES.SavedList[i].ML_Number == Number)
        {
            if(DBSERVICES.SavedList[i].ML_Tpe == XSMLINFO)
            {
                TX_Send_SERVICES.Send_xsmlto_CTC(DBSERVICES.SavedList[i],isSet,0x11);
                break;
                //第三个参数需要确认
            }
            else
            {
                TX_Send_SERVICES.Send_Currentto_CTC(DBSERVICES.SavedList[i]);
                break;
            }
        }
    }
}
void JSJ_ddmldlg::SetCurTime()
{
    for(int i = 0; i < ui->ml_showTable->rowCount();++i)
    {
        ui->ml_showTable->item(i,2)->setText("00:00");
    }
}

void JSJ_ddmldlg::SetCurrentIndex()
{
    ui->tabWidget->setCurrentIndex(XSMLINFO);
    on_tabWidget_currentChanged(DDMLINFO);
}
void JSJ_ddmldlg::on_ml_showTable_cellClicked(int row, int column)
{
    if(row!=-1)
    {
        if(ui->ml_showTable->item(row,1)->checkState() == Qt::Checked)
            ui->ml_showTable->item(row, 1)->setCheckState(Qt::Unchecked);
        else
        {
            ui->ml_showTable->item(row,1)->setCheckState(Qt::Checked);
            QString time = m_ddmldlg->GetSendTime();
            qDebug()<<time;
            ui->ml_showTable->item(row,2)->setText(time);
        }

    }
}

void JSJ_ddmldlg::ShoMainT()
{
    emit DBSERVICES.ShowMain(true);
    DBSERVICES.SetSpan();
    this->hide();
}
void JSJ_ddmldlg::ShowIndex(int index)
{
     Position = ui->tabWidget->y();
    if(index==DDMLINFO || index == XSMLINFO)
    {
        ui->tabWidget->move(ui->tabWidget->x(),1);
        ui->tabWidget->setMinimumSize(1571,781);
        ui->tabWidget->setMaximumSize(1571,781);
        ui->tabWidget->show();
        ui->ml_showTable->show();
        ui->HC_tableWidget->show();
        ui->Send_tableWidget->show();
        ui->Finish_tableWidget->show();
        ui->ml_SaveBtn_2->show();
        ui->ml_SendBtn_2->show();
        ui->cancle_Btn->show();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
    }else
    {
        //ui->tabWidget->move(ui->tabWidget->x(),ui->tabWidget->y()-10);
        ui->tabWidget->setMinimumSize(1920,1089);
        ui->tabWidget->setMaximumSize(1920,1089);
        ui->ml_showTable->hide();
        ui->HC_tableWidget->hide();
        ui->Send_tableWidget->hide();
        ui->Finish_tableWidget->hide();
        ui->ml_SaveBtn_2->hide();
        ui->ml_SendBtn_2->hide();
        ui->cancle_Btn->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->ml_CloseBtn_2->hide();
    }
}
void JSJ_ddmldlg::on_tabWidget_currentChanged(int index)
{
    ShowIndex(index);
    if (index == JHXFINFO)
    {
        m_jhxfdlg->show();
    }
//    else if (index == DRAWINFO)
//    {
//        multiPlandlg->show();
//    }else
//    {
//        for(int i = 0; i < ui->ml_showTable->rowCount();++i)
//        {
//            if(index==DDMLINFO)
//            {
//                ui->ml_showTable->item(i,0)->setText("调度命令");
//                ui->ml_SendBtn_2->setText("发送");
//            }
//            else if (index==XSMLINFO)
//            {
//                ui->ml_showTable->item(i,0)->setText("限速命令");
//                ui->ml_showTable->setEnabled(false);
//            }
//        }
//    }
//    ChangeIndexSlot(index);
}

void JSJ_ddmldlg::SetCurStatus(QString Number)
{
    DBSERVICES.reFreshAllStatus(Number,Finished);
    emit DBSERVICES.ClearAllWidgets(ui->tabWidget->currentIndex());
}

void JSJ_ddmldlg::InitTable_Info()
{

    for(int i = 0; i < DBSERVICES.CurBackInfoList.size();++i)
    {
        int flag =(DBSERVICES.isExistTableWidget(ui->ml_showTable,DBSERVICES.CurBackInfoList[i].StationName));
        if(!DBSERVICES.CurBackInfoList[i].isAppend())
        {
            if(flag!=-1)
            {
                ui->ml_showTable->setRowHidden(flag,true);
            }
        }
        else
        {
            if(flag == -1)
            {

            }else
            {
                ui->ml_showTable->setRowHidden(flag,false);
            }

        }
    }
}
#include<QToolTip>
QString ToolTipWrap(QString strSrc,QFont font,int width)
{
    int iCurrent = 0;
    QString strToolTip = "";
    QString strTemp = "";
    QFontMetrics fontMetrics(font);
    while(iCurrent < strSrc.size())
    {
        strTemp  += strSrc.at(iCurrent);
        if(fontMetrics.width(strTemp)>=width)
        {
            strToolTip +=strTemp;//或HTML标签<br/>
            strTemp.clear();
        }
        ++iCurrent;
    }
    return strToolTip;
}

QString TMyTextLabel(const QString &org,QFont fm_,int width)
{
    QString result;
    QFontMetrics fm(fm_);
    int textWidthInPxs = fm.width(org);
    const int rear = org.length();
    int pre = 0, vernier = 1;
    unsigned int pickUpWidthPxs = 0;
    QString pickUp;
    unsigned int curLen = 0;

    if (textWidthInPxs <= width)
    {
        result = org;
        return result;
    }

    while (vernier <= rear)
    {
        curLen = vernier - pre;
        pickUp = org.mid(pre, curLen);
        pickUpWidthPxs = fm.width(pickUp);
        if (pickUpWidthPxs >= width)
        {
            result += pickUp + "\n";  // 插入换行符，或者使用<br/>标签
            pre = vernier;
            pickUpWidthPxs = 0;
        }
        ++vernier;
    }
    if (pickUpWidthPxs < width && !pickUp.isEmpty())
    {

        result += pickUp;
    }

    return result;
}
void JSJ_ddmldlg::slotCellEnter(int r, int c)
{
    QTableWidgetItem *item = ui->HC_tableWidget->item(r, c);
    if (item == nullptr) {
        return;
    }
    QFont font;
    QString str = TMyTextLabel(item->text(),font,300);
    QToolTip::showText(QCursor::pos(), str);
}

void JSJ_ddmldlg::slotCellEnter_Send(int r, int c)
{
    QTableWidgetItem *item = ui->Send_tableWidget->item(r, c);
    if (item == nullptr) {
        return;
    }
    QFont font;
    QString str = TMyTextLabel(item->text(),font,300);
    QToolTip::showText(QCursor::pos(), str);
}

void JSJ_ddmldlg::SetcheckedStation(QString station)
{
    for(int i = 0 ; i < ui->ml_showTable->rowCount();++i)
    {
        ui->ml_showTable->item(i,1)->setCheckState(Qt::Unchecked);
        if(ui->ml_showTable->item(i,1)->text() == station)
        {
            ui->ml_showTable->item(i,1)->setCheckState(Qt::Checked);
            QSR_List.push_back(ui->ml_showTable->item(i,1)->text());
        }
    }
}

void JSJ_ddmldlg::UpdateStationInfo()
{
    m_xsmldlg->InitStation();
    InitTable_Info();
    m_jhxfdlg->InitJh_StationInfo();

}
bool JSJ_ddmldlg::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == ui->Send_tableWidget->viewport() || obj == ui->HC_tableWidget->viewport())
    {
        if (e->type() == QEvent::MouseButtonPress){
            ui->cancle_Btn->setStyleSheet("background-color: rgb(46, 96, 142);"
                                          "color: rgb(255, 255, 255);"
                                          "border-radius: 6px;"
                                          "font: 14pt 黑体;");
            ui->cancle_Btn->setEnabled(true);
        }
    }else
    {
        ui->cancle_Btn->setStyleSheet("background-color: rgb(179, 179, 179);"
                                      "color: rgb(255, 255, 255);"
                                      "border-radius: 6px;"
                                      "font: 14pt 黑体;");
        ui->cancle_Btn->setEnabled(false);
    }
    return false;
}
void JSJ_ddmldlg::on_cancle_Btn_clicked()
{
    int index = -1;
    QList<QTableWidgetItem*> items = ui->Finish_tableWidget->selectedItems();
    if(!items.empty()){
        QString Number = items.at(0)->text();
        index =  DBSERVICES.getSendInfoByNumber(Number);
        DBSERVICES.SavedList[index].XsIsSend = false;
        isSet = false;
        if(ui->tabWidget->currentIndex() == XSMLINFO)
            TX_Send_SERVICES.Send_xsmlto_CTC(DBSERVICES.SavedList[index],0x22,0x11);
        ui->Send_tableWidget->removeRow(items[0]->row());
        DBSERVICES.SavedList.removeAt(index);
    }else
    {
        items = ui->HC_tableWidget->selectedItems();
        if(!items.empty()){
            QString Number = items.at(0)->text();
            index =  DBSERVICES.getSendInfoByNumber(Number);
            ui->HC_tableWidget->removeRow(items[0]->row());
            DBSERVICES.SavedList.removeAt(index);
        }
    }
    ui->cancle_Btn->setStyleSheet("background-color: rgb(179, 179, 179);"
                                  "color: rgb(255, 255, 255);"
                                  "border-radius: 6px;"
                                  "font: 14pt 黑体;");

    ui->cancle_Btn->setEnabled(false);
}
