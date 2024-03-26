#include "dialog_powersupply.h"
#include "ui_dialog_powersupply.h"

#include "mainwindow.h"
#include <QFile>

Dialog_PowerSupply::Dialog_PowerSupply(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_PowerSupply)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("电力臂状态操作 - TDCS/CTC3.0 通用车站操作终端"));
    InitStationPower();
    InitTree(ui->treeView);
    this->move(700,250);

}

Dialog_PowerSupply::~Dialog_PowerSupply()
{
    delete ui;
}

void Dialog_PowerSupply::treeItem_checkAllChild(QStandardItem * item, bool check)
{
    if (item == nullptr)
        return;
    int rowCount = item->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems, check);
    }
    if (item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}
void Dialog_PowerSupply::treeItem_checkAllChild_recursion(QStandardItem * item, bool check)
{
    if (item == nullptr)
        return;
    int rowCount = item->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        QStandardItem* childItems = item->child(i);
        treeItem_checkAllChild_recursion(childItems, check);
    }
    if (item->isCheckable())
        item->setCheckState(check ? Qt::Checked : Qt::Unchecked);
}
///
/// \brief 根据子节点的改变，更改父节点的选择情况
/// \param item
///
void Dialog_PowerSupply::treeItem_CheckChildChanged(QStandardItem * item)
{
    if (nullptr == item)
        return;
    Qt::CheckState siblingState = checkSibling(item);
    QStandardItem * parentItem = item->parent();
    if (nullptr == parentItem)
        return;
    if (Qt::PartiallyChecked == siblingState)
    {
        if (parentItem->isCheckable() && parentItem->isTristate())
            parentItem->setCheckState(Qt::PartiallyChecked);
    }
    else if (Qt::Checked == siblingState)
    {
        if (parentItem->isCheckable())
            parentItem->setCheckState(Qt::Checked);
    }
    else
    {
        if (parentItem->isCheckable())
            parentItem->setCheckState(Qt::Unchecked);
    }
    treeItem_CheckChildChanged(parentItem);
}
///
/// \brief 测量兄弟节点的情况，如果都选中返回Qt::Checked，都不选中Qt::Unchecked,不完全选中返回Qt::PartiallyChecked
/// \param item
/// \return 如果都选中返回Qt::Checked，都不选中Qt::Unchecked,不完全选中返回Qt::PartiallyChecked
///
Qt::CheckState Dialog_PowerSupply::checkSibling(QStandardItem * item)
{
    //先通过父节点获取兄弟节点
    QStandardItem * parent = item->parent();
    if (nullptr == parent)
        return item->checkState();
    int brotherCount = parent->rowCount();
    int checkedCount(0), unCheckedCount(0);
    Qt::CheckState state;
    for (int i = 0; i < brotherCount; ++i)
    {
        QStandardItem* siblingItem = parent->child(i);
        state = siblingItem->checkState();
        if (Qt::PartiallyChecked == state)
            return Qt::PartiallyChecked;
        else if (Qt::Unchecked == state)
            ++unCheckedCount;
        else
            ++checkedCount;
        if (checkedCount > 0 && unCheckedCount > 0)
            return Qt::PartiallyChecked;
    }
    if (unCheckedCount > 0)
        return Qt::Unchecked;
    return Qt::Checked;
}

int Dialog_PowerSupply::GetStationIndex(unsigned int stationid, QString StationName)
{
    int size = v_StationPower.size();
    StationPower *stationdata;
    for (int i = 0; i < size; i++)
    {
        if (v_StationPower[i]->StationID == stationid)
        {
            return i;
        }
    }
    stationdata = new StationPower();
    stationdata->Init();
    stationdata->StationID = stationid;
    stationdata->StationName = StationName;
    v_StationPower.push_back(stationdata);
    return v_StationPower.size() - 1;
}
//void Dialog_PowerSupply::InitStationPower()//读取数据库
//{
//    QString  sql = ("SELECT t1.stationname, t2.stationid,t2.id,t2.powername,t2.powerflag FROM tb_stationdata AS t1 "
//                    "JOIN tb_powersupply AS t2 ON t1.id = t2.stationid");
//    //QString  sql = ("SELECT * FROM tb_qddev") ;
//    query = DBSERVICE.QueryDB(sql);
//    int index = -1;
//    unsigned int stationID;
//    QString stationName;
//    if (query.exec())
//    {
//        while (query.next())
//        {
//            PowerStr *powerdata;
//            powerdata = new PowerStr();
//            powerdata->Init();
//            powerdata->PowerName = query.value("powername").toString();
//            powerdata->PowerNode = query.value("id").toUInt();
//            powerdata->PowerFlag = query.value("powerflag").toUInt();
//            stationID = query.value("stationid").toUInt();
//            stationName = query.value("stationname").toString();
//            index = GetStationIndex(stationID, stationName);
//            if (index < v_StationPower.size())
//            {
//                v_StationPower[index]->v_PowerArray.push_back(powerdata);
//            }
//        }
//    }
//}
void Dialog_PowerSupply::InitStationPower()//读取文件
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    QByteArray m_str;
    int index = -1;
    unsigned int stationID;
    QString stationName;

//    QFile file("../data/PowerSupplyArm.txt");
    QFile file("./PowerSupplyArm.txt");
    file.open(QIODevice::ReadOnly);
    for(;;)
    {
        PowerStr *powerdata = new PowerStr();
        powerdata->Init();
        m_str = file.readLine();
        if (m_str.left(4) == "####" || m_str == "")
        {
            break;
        }
        if(m_str!="##>\r\n")
        {
            pmainwindow->KeyDialogbox("错误！");
            file.close();
            return;
        }
        else if(m_str=="##>\r\n")
        {
            //读取PowerSupplyArm


            m_str = file.readLine();
            QString strData;
            strData= QString::fromLocal8Bit(m_str.data());
            if(m_str.left(5)=="Name:")
            {
                powerdata->PowerName=strData.right(strData.length()-5).replace(";","").replace("\r\n","");
            }
            m_str = file.readLine();
            if(m_str.left(8)=="RangeGD:")
            {
                powerdata->PowerDEVName.clear();
                QString PowerStr;
                PowerStr=m_str.right(m_str.length()-8).replace(";","").replace("\r\n","");
                QString ch = ",";
                pmainwindow->StringSplit(PowerStr, ch, powerdata->PowerDEVName);
            }
            m_str = file.readLine();
            if(m_str.left(8)=="RangeDC:")
            {
                powerdata->PowerDCName.clear();
                QString PowerStr;
                PowerStr=m_str.right(m_str.length()-8).replace(";","").replace("\r\n","");
                QString ch = ",";
//                pmainwindow->StringSplit(PowerStr, ch, powerdata->PowerDEVName);
                pmainwindow->StringSplit(PowerStr, ch, powerdata->PowerDCName);
            }
        }
//        stationID = 1;
//        stationName = "标准站";
        stationID = pmainwindow->STATIONID;
        stationName = pmainwindow->STATIONNAME;
        index = GetStationIndex(stationID, stationName);
        if (index < v_StationPower.size())
        {
            v_StationPower[index]->v_PowerArray.push_back(powerdata);
        }
    }
    file.close();
}
void Dialog_PowerSupply::InitTree(QTreeView *pTree)
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    PowerStr *power;
    CStation *pStation = pmainwindow->monitor->zcArray[0];
    CGDDC *pDC;
    CGD *pGD;
    int GD_num=pStation->GDArray.count();
    int DC_num=pStation->GDDCArray.count();
    bool GDPowerSupplyFlag;
    bool DCPowerSupplyFlag;
    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
    TreeModel = new QStandardItemModel(4, 2);
    pTree->header()->hide();
    for (int i = 0; i < v_StationPower.size(); i++)
    {
        QStandardItem *item1 = new QStandardItem(v_StationPower[i]->StationName);
        QStandardItem *item2 = new QStandardItem("状态");
        item1->setCheckable(true);
        for (int j = 0; j < v_StationPower[i]->v_PowerArray.size(); j++)
        {
            GDPowerSupplyFlag=true;
            DCPowerSupplyFlag=true;
            QList<QStandardItem*> list;
            v_StationPower[i]->v_PowerArray[j]->Item = new QStandardItem(v_StationPower[i]->v_PowerArray[j]->PowerName);
            v_StationPower[i]->v_PowerArray[j]->Item->setCheckable(true);
            list.append(v_StationPower[i]->v_PowerArray[j]->Item);
            QString Value;
            power = v_StationPower[i]->v_PowerArray[j];
            for (int p=0;p<power->PowerDEVName.count();p++)
            {
                for (int q=0;q<GD_num;q++)
                {
                    pGD = (CGD *)(pStation->GDArray[q]);
                    if(power->PowerDEVName[p]==pGD->m_strName)
                    {
                        if(pGD->GDPowerSupply==1)
                        {}
                        else
                        {
                            GDPowerSupplyFlag=false;
                        }
                    }
                }
            }
            if(power->PowerDEVName.count()==0){GDPowerSupplyFlag=true;}
            for (int p=0;p<power->PowerDCName.count();p++)
            {
                for (int o=0;o<DC_num;o++)
                {
                    pDC = (CGDDC *)(pStation->GDDCArray[o]);
                    if(power->PowerDCName[p]==pDC->m_strName)
                    {
                        if(pDC->DCPowerSupply==1)
                        {}
                        else
                        {
                            DCPowerSupplyFlag=false;
                        }
                    }
                }
            }
            if(power->PowerDCName.count()==0){DCPowerSupplyFlag=true;}
            if((GDPowerSupplyFlag==true)&&(DCPowerSupplyFlag==true))
            {
                v_StationPower[i]->v_PowerArray[j]->PowerFlag=1;
            }
            else
            {
                v_StationPower[i]->v_PowerArray[j]->PowerFlag=0;
            }
            v_StationPower[i]->v_PowerArray[j]->PowerFlag ? Value = ("有电") : Value = "停电";
            v_StationPower[i]->v_PowerArray[j]->Item1 = new QStandardItem(Value);
            list.append(v_StationPower[i]->v_PowerArray[j]->Item1);
            item1->appendRow(list);
        }
        TreeModel->setItem(i, 0, item1);
        TreeModel->setItem(i, 1, item2);
    }
    pTree->setModel(TreeModel);
    ui->treeView->setColumnWidth(0, 350);
    ui->treeView->setColumnWidth(1, 50);
    connect(pTree, SIGNAL(clicked(const QModelIndex)), this, SLOT(TreeViewClick(const QModelIndex)));
    connect(TreeModel, &QStandardItemModel::itemChanged, this, &Dialog_PowerSupply::treeItemChanged);
}

void Dialog_PowerSupply::GetPowerSupplyState()
{

}

void Dialog_PowerSupply::UpdatePowerData(PowerStr power)
{
//    QString  sql = QString::fromLocal8Bit("UPDATE tb_powersupply SET powerflag=%1 where id=%2").arg(power.PowerFlag).arg(power.PowerNode);
//    DBSERVICE.QueryDB(sql);
}
void Dialog_PowerSupply::on_PowerButton_clicked()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    PowerStr *power;
    CStation *pStation = pmainwindow->monitor->zcArray[0];
    CGDDC *pDC;
    CGD *pGD;
    int GD_num=pStation->GDArray.count();
    int DC_num=pStation->GDDCArray.count();
    for (int i = 0; i < v_StationPower.size(); i++)
    {
        for (int j = 0; j < v_StationPower[i]->v_PowerArray.size(); j++)
        {
            power = v_StationPower[i]->v_PowerArray[j];
            if (power->Item->checkState() == Qt::Checked)
            {
                QString value;
                power->PowerFlag = true;
                power->PowerFlag ? value = QString("有电") : value = QString("停电");
                power->Item1->setText(value);
//                UpdatePowerData(*power);
//                power->Item->setCheckState(Qt::Unchecked);


                for (int p=0;p<power->PowerDEVName.count();p++)
                {
                    pmainwindow->FUNSet_DeviceNum=0xffff;
                    pmainwindow->FUNType_GDBSet=0x46;
                    for (int q=0;q<GD_num;q++)
                    {
                        pGD = (CGD *)(pStation->GDArray[q]);
                        if(power->PowerDEVName[p]==pGD->m_strName)
                        {
                            pmainwindow->FUNSet_DeviceNum=pGD->m_nCode;//需要采集它的设备编号
                            pGD->GDPowerSupply=1;
                        }
                    }
                    pmainwindow->SendToTrainSimu_FUNSet();
                }
                for (int p=0;p<power->PowerDCName.count();p++)
                {
                    pmainwindow->FUNSet_DeviceNum=0xffff;
                    pmainwindow->FUNType_GDBSet=0x46;
                    for (int o=0;o<DC_num;o++)
                    {
                        pDC = (CGDDC *)(pStation->GDDCArray[o]);
                        if(power->PowerDCName[p]==pDC->m_strName)
                        {
                            pmainwindow->FUNSet_DeviceNum=pDC->m_nCode;//需要采集它的设备编号
                            pDC->DCPowerSupply=1;
                        }
                    }
                    pmainwindow->SendToTrainSimu_FUNSet();
                }
            }
        }
    }
}

void Dialog_PowerSupply::on_TDButton_clicked()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    PowerStr *power;
    CStation *pStation = pmainwindow->monitor->zcArray[0];
    CGDDC *pDC;
    CGD *pGD;
    int GD_num=pStation->GDArray.count();
    int DC_num=pStation->GDDCArray.count();
    for (int i = 0; i < v_StationPower.size(); i++)
    {
        for (int j = 0; j < v_StationPower[i]->v_PowerArray.size(); j++)
        {
            power = v_StationPower[i]->v_PowerArray[j];
            if (power->Item->checkState() == Qt::Checked)
            {
                QString value;
                power->PowerFlag = false;
                power->PowerFlag ? value = QString("有电") : value = QString("停电");
                power->Item1->setText(value);
//                UpdatePowerData(*power);
//                power->Item->setCheckState(Qt::Unchecked);

//                for (int p=0;p<power->PowerDEVName.count();p++)
//                {
//                    pmainwindow->FUNType_GDBSet=0x45;
//                    for (int q=0;q<GD_num;q++)
//                    {
//                        pGD = (CGD *)(pStation->GDArray[q]);
//                        if(power->PowerDEVName[p]==pGD->m_strName)
//                        {
//                            pmainwindow->FUNSet_DeviceNum=pGD->m_nCode;//需要采集它的设备编号
//                        }
//                    }
//                    for (int o=0;o<DC_num;o++)
//                    {
//                        pDC = (CGDDC *)(pStation->GDDCArray[o]);
//                        if(power->PowerDEVName[p]==pDC->m_strName)
//                        {
//                            pmainwindow->FUNSet_DeviceNum=pDC->m_nCode;//需要采集它的设备编号
//                        }
//                    }
//                    pmainwindow->SendToTrainSimu_FUNSet();
//                }
                for (int p=0;p<power->PowerDEVName.count();p++)
                {
                    pmainwindow->FUNSet_DeviceNum=0xffff;
                    pmainwindow->FUNType_GDBSet=0x45;
                    for (int q=0;q<GD_num;q++)
                    {
                        pGD = (CGD *)(pStation->GDArray[q]);
                        if(power->PowerDEVName[p]==pGD->m_strName)
                        {
                            pmainwindow->FUNSet_DeviceNum=pGD->m_nCode;//需要采集它的设备编号
                            pGD->GDPowerSupply=0;
                        }
                    }
                    pmainwindow->SendToTrainSimu_FUNSet();
                }
                for (int p=0;p<power->PowerDCName.count();p++)
                {
                    pmainwindow->FUNSet_DeviceNum=0xffff;
                    pmainwindow->FUNType_GDBSet=0x45;
                    for (int o=0;o<DC_num;o++)
                    {
                        pDC = (CGDDC *)(pStation->GDDCArray[o]);
                        if(power->PowerDCName[p]==pDC->m_strName)
                        {
                            pmainwindow->FUNSet_DeviceNum=pDC->m_nCode;//需要采集它的设备编号
                            pDC->DCPowerSupply=0;
                        }
                    }
                    pmainwindow->SendToTrainSimu_FUNSet();
                }
            }
        }
    }
}
void Dialog_PowerSupply::TreeViewClick(const QModelIndex &index)
{
    QStandardItem *Item = TreeModel->itemFromIndex(index);
}

void Dialog_PowerSupply::on_UpdateButton_clicked()
{

}

void Dialog_PowerSupply::on_CloseButton_clicked()
{
    hide();
}

void Dialog_PowerSupply::treeItemChanged(QStandardItem * item)
{
    if (item == nullptr)
        return ;
    if (item->isCheckable())
    {
        //如果条目是存在复选框的，那么就进行下面的操作
        Qt::CheckState state = item->checkState();  //获取当前的选择状态
#if 0
        if (item->isTristate())
        {
            //如果条目是三态的，说明可以对子目录进行全选和全不选的设置
            if (state != Qt::PartiallyChecked)
            {
                //当前是选中状态，需要对其子项目进行全选
                treeItem_checkAllChild(item, state == Qt::Checked ? true : false);
            }
        }
        else
        {
            //说明是两态的，两态会对父级的三态有影响
            //判断兄弟节点的情况
            treeItem_CheckChildChanged(item);
        }
#endif
        if (item->hasChildren())
        {
            Dialog_PowerSupply::treeItem_checkAllChild(item, state == Qt::Checked ? true : false);
        }
        else
        {
            Dialog_PowerSupply::treeItem_CheckChildChanged(item);
        }
    }
}

//void Dialog_PowerSupply::PLCZ_SendData_DealFromPowerSupply()
//{
//    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
//    CStation *pStation = pmainwindow->monitor->zcArray[0];
//    CGD *pGD;
//    CGDDC *pDC;
//    CXHD *pXHD;
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_dcDev.size();i++)
//    {
//        if((stationDevOper->CurrectZone->vect_dcDev[i].OperStr==QString("单锁"))&&(stationDevOper->CurrectZone->vect_dcDev[i].StateStr == QString("单锁")))
//        {
//            for(int j=0;j<pStation->GDDCArray.count();j++)
//            {
//                pDC=pStation->GDDCArray[j];
//                if(stationDevOper->CurrectZone->vect_dcDev[i].Devname==pDC->m_strName)
//                {
//                    pDC->DCFSState=true;
//                }
//            }
//        }
//    }
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_qdDev.size();i++)
//    {
//        if((stationDevOper->CurrectZone->vect_qdDev[i].OperStr==QString("封"))&&(stationDevOper->CurrectZone->vect_qdDev[i].StateStr == QString("封")))
//        {
//            for(int j=0;j<pStation->GDArray.count();j++)
//            {
//                pGD=pStation->GDArray[j];
//                if(stationDevOper->CurrectZone->vect_qdDev[i].Devname==pGD->m_strName)
//                {
//                    pGD->GDFSState=true;
//                }
//            }
//        }
//    }
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_qjDev.size();i++)
//    {
//        if((stationDevOper->CurrectZone->vect_qjDev[i].OperStr==QString("封"))&&(stationDevOper->CurrectZone->vect_qjDev[i].StateStr == QString("封")))
//        {
//            for(int j=0;j<pStation->GDArray.count();j++)
//            {
//                pGD=pStation->GDArray[j];
//                if(stationDevOper->CurrectZone->vect_qjDev[i].Devname==pGD->m_strName)
//                {
//                    pGD->GDFSState=true;
//                }
//            }
//        }
//    }
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_xhDev.size();i++)
//    {
//        if(((stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("封(列)封(调)"))||(stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("封(调)")))
//            &&((stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("封(列)封(调)"))||(stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("封(调)"))))
//        {
//            for(int j=0;j<pStation->XHDArray.count();j++)
//            {
//                pXHD=pStation->XHDArray[j];
//                if(stationDevOper->CurrectZone->vect_xhDev[i].Devname==pXHD->m_strName)
//                {
//                    pXHD->XHFSState=true;
//                }
//            }
//        }
//    }
//}








