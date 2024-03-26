#ifndef DIALOG_POWERSUPPLY_H
#define DIALOG_POWERSUPPLY_H

#pragma execution_character_set("utf-8")

#include <QDialog>
#include <QStandardItemModel>
#include <QSqlQuery>
#include <QTreeView>
#include <QVector>


namespace Ui {
class Dialog_PowerSupply;
}

class Dialog_PowerSupply : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_PowerSupply(QWidget *parent = nullptr);
    ~Dialog_PowerSupply();

//    void PLCZ_SendData_DealFromPowerSupply();
    void static treeItem_checkAllChild(QStandardItem * item, bool check);
    void static treeItem_checkAllChild_recursion(QStandardItem * item, bool check);
    void static treeItem_CheckChildChanged(QStandardItem * item);
    Qt::CheckState static checkSibling(QStandardItem * item);

    typedef struct
    {
        QString PowerName;
        bool PowerFlag;
        unsigned int PowerNode;
//        QVector<QString*> PowerDEVName;
        QStringList PowerDEVName;
        QStringList PowerDCName;
        QStandardItem *Item;
        QStandardItem *Item1;
        void Init()
        {
            PowerName = "";
            PowerFlag = 1;
            PowerNode = 0xffff;
            Item = nullptr;
            Item1 =  nullptr;
        }
    } PowerStr;

    typedef struct
    {
        unsigned int StationID;
        QString StationName;
        QVector<PowerStr*> v_PowerArray;
        void Init()
        {
            StationID = 0xffff;
            StationName = "";
            v_PowerArray.clear();
        }
    } StationPower;
    void GetPowerSupplyState();

//    typedef  struct
//    {
//        QString Devname_Pow;
//        QString StateStr_Pow;
//        int m_nCode_Pow;
//        void InitInfo(){
//            m_nCode_Pow=0xffff;
//            Devname_Pow = StateStr_Pow = "";
//        }
//    }zonedev_Pow;
//    QVector<zonedev_Pow> vect_dcDev_Pow;
//    QVector<zonedev_Pow> vect_qdDev_Pow;
//    QVector<zonedev_Pow> vect_qjDev_Pow;
//    QVector<zonedev_Pow> vect_xhDev_Pow;

private slots:
    void on_PowerButton_clicked();

    void on_TDButton_clicked();

    void on_UpdateButton_clicked();

    void on_CloseButton_clicked();

    void TreeViewClick(const QModelIndex &index);

    void treeItemChanged(QStandardItem * item);

private:
    Ui::Dialog_PowerSupply *ui;

    int GetStationIndex(unsigned int stationid, QString StationName);
    QSqlQuery query;
    QVector<StationPower*> v_StationPower;
    QStandardItemModel *TreeModel;

    void InitStationPower();
    void InitTree(QTreeView *pTree);
    void UpdatePowerData(PowerStr PowerStr);
};

#endif // DIALOG_POWERSUPPLY_H
