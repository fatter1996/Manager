#ifndef STATIONSHOWFROM_H
#define STATIONSHOWFROM_H

#pragma execution_character_set("utf-8")

#include <QWidget>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include"basestructure.h"
#include"testT/T_globaldata.h"
struct NodeInfo
{
    QString TopName;
    QString SelfName;
};
namespace Ui {
class stationshowfrom;
}
class QTreeView;
class stationshowfrom : public QWidget
{
    Q_OBJECT

public:
    explicit stationshowfrom(QWidget *parent = nullptr);
    ~stationshowfrom();

private:
    Ui::stationshowfrom *ui;

public:
    QList<NodeInfo>itemList;
   QVector<Province*> initData();
   void setModel(const zone* zone);
    QVector<zoneData> ReadZonelist(int id);
    void InitInfo(zoneData* zone);
    void ClearInfo();
    void GetGDInfo(QString Name);
#ifdef BatchOper
   mysql *currentSql;
#endif
   QString StationName;
   int  StationId;


   QString StationFile;
   zoneData* CurrectZone = nullptr;
   bool IsEditFlag;
   bool IsonlyData;



   QVector<zoneData> vect_zonename;
   void IniteFun();//初始化接口函数
   void ReadConfigFun();
   bool CZState_QYPLCZ;
   void ReadZoneData();
   void IniteTreeData();
   QStandardItemModel *TreeModel;
   bool IsMianSelect(QString name);
   void UpdataTreelist(zoneData* zone);
   void UpdataTreelist_Save(zoneData* zone);
   void UpdataTreelist_refresh(zoneData* zone);
   zonedev* UpdataTreelist(QString DevName,int *Type,zoneData* zone);
   void DevOperFun(int type);
   void DeleteDevFun(QString DevName,zoneData* zone);
   void DevOperFun_SIN();

   QMenu *DCMenu;
   QMenu *QDMenu;
   QMenu *XHMenu;
   QMenu *QJMenu;

   QAction* DCDSaction;
   QAction* DCDSDaction;
   QAction* DCDSFaction;
   QAction* DCFSaction;
   QAction* DCDeleteaction;

   QAction* QDFSaction;
   QAction* QDDeleteaction;

   QAction* QJFSaction;
   QAction* QJDeleteaction;

   QAction* XHFSaction;
   QAction* XHDeleteaction;

   void MeauInite();

   zonedev *GetCurrectDev;
  bool IsTopItem(QStandardItem* item);
   void SendData(unsigned char FUNType/*功能类型*/,unsigned int FUNSet_DeviceNum/*功能设置_设备编号*/);
   unsigned int IP_OppPort;//链接IP端口


   QStringList m_List_LSPLGZQ;
   QString comboBox_Name;

   /**存储comboBox列表中的信息**/
   typedef struct
   {
       QString Dev_Name[500];//设备名称
       QString Dev_Operation[500];//操作
       QString Dev_State[500];//状态
       int DCDev_NUM;//道岔设备数量
       int QDDev_NUM;//区段设备数量
       int QJDev_NUM;//区间设备数量
       int XHJDev_NUM;//信号机设备数量

   }Save_comboBox;
   Save_comboBox Save_comboBox_Array[20];
   int Save_comboBox_list_num;    //存储数量

   void on_loadzone_btn_clicked();


signals:
//   void Showsignal(QString StationName, int ID, QString fileName,int sizetype);
   void Showsignal(int ID, QString fileName,bool sizetype);
   void ClearStationState();
   void ClearState(QString type,QString Name);
//   void Signal_PLCZ_SendData();
//   void Signal_PLCZ_SendData_JF();
   void Signal_Refresh_ZC();
public slots:
   void SelectDevslot(QStringList QJList,QStringList XHList,QStringList QDList,QStringList DCList,QStringList QJList_code, QStringList XHList_code, QStringList QDList_code, QStringList DCList_code);
   void treeItemChanged(QStandardItem * item);
   void TreeViewClick(const QModelIndex &index);
   void onCustomContextMenuRequested(const QPoint &pos);


   void on_comboBox_currentIndexChanged(const QString &arg1);
//   void on_loadzone_btn_clicked();
   void on_pushButton_clicked();
   //void on_detelezone_btn_clicked();
   //void on_FSDev_btn_clicked();
   //void on_JFDev_btn_clicked();
   void onDCMenuTriggered(QAction *action);
   void onQDMenuTriggered(QAction *action);
   void onQJMenuTriggered(QAction *action);
   void onXHMenuTriggered(QAction *action);

private slots:
   //void on_close_btn_clicked();
   void on_comboBox_currentTextChanged(const QString &arg1);
   void on_refresh__btn_clicked();
   void on_Confirmbtn_clicked();
};

#endif // STATIONSHOWFROM_H
