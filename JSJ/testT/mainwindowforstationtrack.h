#ifndef MAINWINDOWFORSTATIONTRACK_H
#define MAINWINDOWFORSTATIONTRACK_H

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include <QSqlTableModel>


class MainWindow;

namespace Ui {
class MainWindowForStationTrack;
}

class MainWindowForStationTrack : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowForStationTrack(MainWindow *p,QWidget *parent = nullptr);
    ~MainWindowForStationTrack();

    MainWindow *pmainWindow;
    void DealWindowAction();
    void DealSequenceWindowAction();


    void timerEvent(QTimerEvent *event);//定时器
    bool eventFilter(QObject *watched, QEvent *event);
//    void mousePressEvent(QMouseEvent *event);


    void StationNameShow();
    void BorderShow_Up();
    void BorderShow_Left();
    void BorderShow_Right();
    void RailwayinterfaceShow();

    void DealTrackData_Main();

    void DealTrackData_Main_1();
    void DealTrackData_Main_2_3();
    void DealTrackData_Main_Refresh();
    void DealTrackData_Down();
//    void DealTrackData_MainWindow();
//    void DealTrackData_MainWindow_1();
//    void DealTrackData_MainWindow_2();
//    void DealTrackData_MainWindow_3();
//    void DealTrackData_MainWindow_4();
//    void DealTrackData_MainWindow_5();
//    void DealTrackData_MainWindow_6();
    void JL_FL_FCWindown(QString text);

    int widgetline_Teftside_M;
    int widgetline_Topside_M;
    bool OpenPlan2and3;


    //数据结构
    /**股道数据结构**/
    typedef struct
    {
        int Track_TrainNumber;//一条股道上排列了几辆车
        int Track_TrainNumber_Flag;
        int Track_TrainNumber_Save[3];
        bool train_num_1;//第一辆列车
        bool train_num_2;//第二辆列车
        bool train_num_3;//第三辆列车
        bool NewTrain;//是否是新车

        int nCode;
        QString strGDName;//车站内拥有的股道名称
        QString strLeftNode;
        QString strRightNode;

        //该部分为TrackArray_formainwindow使用
        QString TrackName;//股道名称//阶段计划下发的股道名称
        QString TrackSkate;//铁鞋
        int TrackLevel;//
        QString TrackTrainName;//股道中车辆名称
        bool TrackStateShow_KY;//股道车辆客运状态显示
        bool TrackStateShow_CX;//股道车辆超限状态显示
        bool TrackStateShow_JY;//股道车辆军运状态显示
        bool TrackStateShow_ZD;//股道车辆重点状态显示
        QString TrackTrainRunState;//列车运行状态
        QString TrackTrainReceptionAndDepartureState;//列车接发车状态
        int TrackJYState;//股道接预状态显示
        int TrackJLState;//股道接路状态显示
        int TrackDDState;//股道到点状态显示
        QString TrackTrainReceptionAndDepartureTime;//列车接发车时间
        QString TrackTrainReceptionAndDepartureTime_TD;//列车图定接发车时间
        QString TrackTrainUpDownRun;//列车上下行方向显示
        int TrackFYState;//股道发预状态显示
        int TrackFLState;//股道发路状态显示
        int TrackFDState;//股道发点状态显示

        int Track_Shangshui_State;//股道上水状态显示//1为正常色，2位红色，3位橘色，4位绿色
        int Track_Xiwu_State;//股道吸污状态显示
        int Track_Jiche_State;//股道机车状态显示
        int Track_Liejian_State;//股道列检状态显示
        int Track_Jiaoling_State;//股道交令状态显示
        int Track_Jiaopiao_State;//股道交票状态显示
        int Track_Liewei_State;//股道列尾状态显示
        int Track_Huojian_State;//股道货检状态显示
        int Track_Chengjiang_State;//股道乘降状态显示
        int Track_Zhaigua_State;//股道摘挂状态显示
        int Track_Huancheng_State;//股道换乘状态显示
        int Track_Zhuangxie_State;//股道装卸状态显示
        int Track_Daokou_State;//股道道口状态显示
        int Track_Chehao_State;//股道车号状态显示
        int Track_Zhanwu_State;//股道站务状态显示
        int Track_Zongkong_State;//股道综控状态显示

        int TrackTrainState;//股道中车辆是客车还是货车 客车为1，货车为2
//        bool FinishButtonDown;

    }trackArray1;
    trackArray1 TrackArray_formainwindow[20];

    typedef struct
    {
        QString NewTrain;
        //该部分为list使用
        int log_ID;
        int station_ID;
        int plan_num;
        QString staName;
        int plan_jfctype;
        QString train_posistatus;
        QString train_nextproc;
        QString reach_track;
        QString reach_trainnum;
        QString reach_trainnumold;
        QString reach_timeplan;
        QString reach_timereal;
        QString reach_xhd;
        int reach_routestatus;
        QString reach_direction;
        QString depart_trainnum;
        QString depart_trainnumold;
        QString depart_track;
        QString depart_timeplan;
        QString depart_timereal;
        QString depart_xhd;
        int depart_routestatus;
        QString depart_direction;
        int isdelete;
        int iselectric;
        int ultralimit_level;
        int kehuoflag;
        QString traintype;
        QString runningtype;
        int isxianlusuo;
        int executeflag;
        int adjtstationfrom;
        int adjtstationto;
        QString agradjtdepat_time;
//        QDateTime agradjtdepat_time;
        int adjtdepat_time;
        QString adjtagrdepat_time;
        int reach_ultralimitlevel;
        int depart_ultralimitlevel;
        int notes;
        int transportpassenger;
        int allowtracknotmatch;
        int allowEntrnotmatch;
        int isArmy;
        int isImportant;
        int plan_checkState;
        int flowstatus_jiaoling;
        int flowstatus_liejian;
        int flowstatus_shangshui;
        int flowstatus_xiwu;
        int flowstatus_jiaopiao;
        int flowstatus_chengjiang;
        int flowstatus_zhaigua;
        int flowstatus_liewei;
        int flowstatus_huojian;
        int flowstatus_huancheng;
        int flowstatus_zhuangxie;
        int flowstatus_jiche;
        int flowstatus_daokou;
        int flowstatus_chehao;
        int flowstatus_zongkong;
        int flowstatus_zhanwu;

//        bool FinishButtonDown;

    }trackArray;
    trackArray list;

    int TrackNum;
    int Track_widget_line_Flag;

    int LoginState;

    //定时器
    int Timer_ID_1;
    int Timer_ID_1000;
    int Timer_ID_2000;

    QString str_Time;
    QSqlQuery query;
    QSqlDatabase db;

    QSqlTableModel *model;

    QVector<trackArray>  vect_list;
    QVector<trackArray>  ReadSQLStagePlan();//实时读取数据库中的阶段计划
    bool vect_list_readFlag;
    bool EndFlag;
    QVector<QString>  TrainNum_list;//车次号
    QVector<int>  TrainNum_list_Flag;//车次号

//    QVector<QString>  TrainTrack_Save;//储存原有股道号，等待股道变化
    QString  TrainTrack_Save[40];//储存原有股道号，等待股道变化

    typedef struct
    {
        //该部分为list使用
        int route_id;
//        int route_id_save;
        int station_ID;
        int plan_num;
        int plan_jfctype;
        int route_type;
        QString track;
        QString trainnum;
        QString trainnumold;
        QString timeplan;
        QString timereal;
        QString timeClear;
        QString entrance_xhd;
        int autotouch;
        int iselectric;
        int ultralimitlevel;
        int kehuoflag;
        int isxianlusuo;
        QString descrip;
        QString descripreal;
        int route_state;
        QString route_stateDescrip;
        int successed;
        QString direction;
        int isyxjl;
        QString xhdstart;
        QString xhdend;
        QString xhdyxend;
        int isbtjl;
        QString arraybtjl;
        int confirmed;
        int lsbRouteIndex;
        QString tempXhdArray;
        QString tempXhdBtnArray;

    }trackArray_routeorder;
    trackArray_routeorder list_route;
    QSqlQuery query_route;
    QVector<trackArray_routeorder>  vect_list_route;
    QVector<trackArray_routeorder>  vect_list_route_save;
    QVector<trackArray_routeorder>  ReadSQLrouteorder();//实时读取数据库中的进路序列

    QVector<int>  route_id_save;
    QVector<QString>  JCstate;
    QVector<QString>  FCstate;

    QVector<TrackView_Lable*> trackView_LableArray;
    QVector<TrackView_Lable*> trackView_LableArray1;
    QVector<TrackView_Lable*> trackView_LableArray2;
//    TrackView_Lable *trackView_LableArray[20]; //与TrackArray_formainwindow[20]相互对应，0-19分别为从上到下的20个股道
//    TrackView_Lable *trackView_LableArray1[20];
//    TrackView_Lable *trackView_LableArray2[20];

    QWidget *widget_line_01[20];
    QWidget *widget_line_02[20];
    QWidget *widget_line_03[20];
//    QLabel *label_scrollArea;
//    TrackView_Lable *trackView_Lable;
//    TrackView_Lable *trackView_Lable1;
//    TrackView_Lable *trackView_Lable2;

    void SequenceViewFace();
    void SequenceViewFace_refresh();
    QStandardItemModel *m_pModel_1;
    QList<QStandardItem*> temp2;
    bool temp2_Flag;

//    QComboBox *comBox_XLMS;//序列描述下拉框
//    QVector<QComboBox*> comBox_XLMS_save;//序列描述下拉框

    QMenu *m_menu_ZC;//自触菜单
    QAction *m_addAction_ZC;
    QAction *m_delAction_ZC;
//    QVector<QAction *> m_addAction_ZC;
//    QVector<QAction *> m_delAction_ZC;

    QMenu *m_menu_CFJL;//触发进路
    QAction *m_addAction_CFJL;
    QAction *m_addAction_CFJL_MX;//明细
    QAction *m_addAction_CFJL_ZCXS;//站场显示
    QAction *m_addAction_CFJL_SCXL;//删除序列

    int TableView_lineNum_Left;
    int TableView_lineNum_Right;

    QMenu *m_menu_JLMS;//进路描述
    QVector<QAction*> m_addAction_JLMS;
    QVector<QString> m_addAction_JLMS_Real;
    int JLMS_BTJL_num;
    int JLMS_BTJL_p;

    bool TableViewReadOnly_Flag;
    int TableViewReadOnly_state;

    int vect_list_route_RowsNum_Save;

    void ContextMenu_LeftButton(const QModelIndex &index);
    void ContextMenu_LeftButton_XLMS(const QModelIndex &index);

    typedef struct
    {
        int plan_id;
        int station_id;
        int plan_num;
        QString plan_timerecv;
        QString dispatch;
        QString dispatcher;

    }stageplan;
    stageplan list_stageplan;
    QVector<stageplan>  vect_list_stageplan;
    QVector<stageplan>  vect_list_stageplan_save;
    QVector<stageplan>  ReadSQLstageplan();//实时读取数据库中的阶段计划

    typedef struct
    {
        int plan_id;
        int station_id;
        int plan_num;

    }stageplan_1;
    stageplan_1 list_stageplan_1;
    QVector<stageplan_1>  vect_list_stageplan_1;
    QVector<stageplan_1>  ReadSQLstageplan_1();//实时读取数据库中的阶段计划中的plan_num

    void JDJHReceiveWin(int p);
    void JDJHReceiveWin_refresh(int p);
    int currentIndex_JDJH_ss;
    QList<QStandardItem*> temp_JDJH;
    QStandardItemModel *m_pModel_JDJH;
    bool temp_JDJH_Flag;

    typedef struct
    {
        int detail_id;
        int station_id;
        int plan_id;
        int plan_num;
        QString plan_timerecv;
        int plan_kind;
        int plan_jfctype;
        int signstate;
        QString dispatch;
//        QString dispatcher;
        QString reach_trainnum;
        QString reach_trainnumold;
        QString reach_track;
        QString reach_timeplan;
        QString reach_timereal;
        QString reach_xhd;
        QString depart_trainnum;
        QString depart_trainnumold;
        QString depart_track;
        QString depart_timeplan;
        QString depart_timereal;
        QString depart_xhd;
        int isdelete;
        int iselectric;
        int ultralimitlevel;
        int kehuoflag;
        QString traintype;
        QString runningtype;
        int isxianlusuo;

    }stageplan_detail;
    stageplan_detail list_stageplan_detail;
    QVector<stageplan_detail>  vect_list_stageplan_detail;
    QVector<stageplan_detail>  vect_list_stageplan_detail_save;
    QVector<stageplan_detail>  ReadSQLstageplan_detail();//实时读取数据库中的阶段计划
    int JDJHNum_1;

    typedef struct
    {
        bool state_JDJHQS;

    }stageplan_detail_JDJHQS;//阶段计划签收
    stageplan_detail_JDJHQS list_stageplan_detail_JDJHQS[100];
    int JDJHNum_JDJHQS_1;



    typedef struct
    {
        int order_id;
        int station_id;
        int order_num;
        QString time_recv;
        QString disCenter;
        QString disName;
        QString order_type;
        QString content;
        QString recv_place;
        QString sign_name;
        QString sign_time;
        QString readName;
        int order_state;

    }disorder_recv;
    disorder_recv list_disorder_recv;
    QVector<disorder_recv>  vect_list_disorder_recv;
    QVector<disorder_recv>  vect_list_disorder_recv_save;
    QVector<disorder_recv>  ReadSQLdisorder_recv();//实时读取数据库中的阶段计划

    int currentIndex_DDML_ss;
    QList<QStandardItem*> temp_DDML;
    QStandardItemModel *m_pModel_DDML;
    bool temp_DDML_Flag;
    void DDMLReceiveWin();
    void DDMLReceiveWin_refresh();



    typedef struct
    {
        int gd_id;
        int station_id;
        QString gdname;
        int l_antislip;
        int l_txnum;
        int l_jgqnum;
        int l_meters;
        int r_antislip;
        int r_txnum;
        int r_jgqnum;
        int r_meters;
        int trainsnum;//存车信息
        int gdattr;//线路性质//0正线1到发线
        int gddir;//接发车方向 0上行1下行2上下行
        int gdjfattr;//接发车类型 0客车1货车2客货车
        int gdoverlevel;//超限类型 0不能接发超限列车1一级超限2二级超限3超级超限
        int gdplatform;//站台 1高站台2低站台0无
        int gdisCRH;//1允许动车组0不允许动车组
        int gdiswater;//1上水设备0无上水设备
        int gdisblowdown;//1排污设备0无排污设备
        int gdArmy;//1可军用列车 0不能接军用列车
        int gdeffectlength;//有效长
        int gdmaxtrain;//最大容车数量
        int gdcatenary;//1接触网0无接触网
        int gdheavytrain;//1接发重载列车0禁止重载列车
        QString version;//版本号


    }gd_antislip;
    gd_antislip list_gd_antislip;
    QVector<gd_antislip>  vect_list_gd_antislip;
    QVector<gd_antislip>  vect_list_gd_antislip_save;
    QVector<gd_antislip>  ReadSQLgd_antislip();//实时读取数据库中的阶段计划
    void FLSetView();

    typedef struct
    {
        int id;
        int station_id;
        QString jzxhname;//进站信号机名称
        QString enexname;//出入口名称
        int enexdir;//方向 0双向1下行进站2上行出站3下行出站4上行禁止。双线自动闭塞区段，按进站信号机的行别、属性 选择“下行进站/上行出站/下行出站/上行进站”。 单线自动闭塞、半自动闭塞区段，选择“双向”
        int Isoverlevel;//1允许一级超限 2允许二级超限 3允许三级超限 4-允许超级超限 0不允许超限列车
        int ispassengertrain;//1 允许旅客列车 0不允许旅客列车
        int isfreighttrain;//1 允许货物列车  0不允许货物列车


    }base_entrance_exit;
    base_entrance_exit list_entrance_exit;
    QVector<base_entrance_exit>  vect_list_entrance_exit;
    QVector<base_entrance_exit>  vect_list_entrance_exit_save;
    QVector<base_entrance_exit>  ReadSQL_base_entrance_exit();//实时读取数据库中的
    void Entrance_exitSetView();

    typedef struct
    {
        int station_id;
        QString station_name;//车站名称
        int route_permit;//进路权限，0无权限，1权限在CTC，2权限在占线板
        int auto_sign_stage;//自动签收阶段计划
        int fczk_mode;//0分散自律模式，1非常站控模式
        int fszl_mode;//分散自律模式，0中心控制，1车站控制，2车站调车

    }station_list;
    station_list list_station_list;
    QVector<station_list>  vect_list_station_list;
    QVector<station_list>  vect_list_station_list_save;
    QVector<station_list>  ReadSQL_station_list();//实时读取数据库中的
    void station_listSetView();

    typedef struct
    {
        int id;
        int station_id;
        int gd_id;
        QString arrivalnum;//到达车次
        QString arrivaltime;//到达时间
        QString departnum;//出发车次
        QString departtime;//出发时间
        int entraid;//入口ID，始发0
        int exitid;//发车ID，终到0
        int beforestationid;//前场车站ID，始发0
        int afterstationid;//后方车站ID，终到0
        int stoppoint;//1 技术停点  0非技术停点  （后续列 0、1代表是否具有属性）
        int deliveryorder;//交令
        int trianflag;//机车
        int cargocheck;//货检
        int boardflag;//乘降
        int pickhang;//摘挂
        int transferflag;//换乘
        int Loadcargo;//装卸
        int dirtabsorption;//吸污
        int crossflag;//道口
        int wagonnum;//车号
        int waterflag;//上水
        int traincheck;//列检
        int Intercontrol;//综控
        int depotflag;//站务
        int payticket;//交票
        int traintail;//列尾
        int version;//版本号
    }base_train_fixedroute;
    base_train_fixedroute list_train_fixedroute;
    QVector<base_train_fixedroute>  vect_list_train_fixedroute;
    QVector<base_train_fixedroute>  vect_list_train_fixedroute_save;
    QVector<base_train_fixedroute>  ReadSQL_base_train_fixedroute();//实时读取数据库中的
    void Train_fixedrouteSetView();

    int get_vect_list_IndexFROMvect_list_route(int num);
    int get_vect_list_route_IndexFROMvect_list(int num,int route_type);
    bool RefreshGDViewFaceFlag;
    void RefreshGDViewFace();

    void ResetGDViewFace();
    void ResetGDViewFace_QK();

    int red_Flesh_times;
//    int red_Flesh_times_DDML;
    bool DDPushButton_TG_Debug;


private:
    Ui::MainWindowForStationTrack *ui;

private slots:
//public slots:
     //右键菜单响应函数
    void slotContextMenu(QPoint pos);
    void slotMenu_ActionJLCF();
    void slotMenu_ActionJLCF_MX();
    void slotMenu_ActionJLCF_SCXL();

    void slotMenu_ActionSZZC();
    void slotMenu_ActionQXZC();
    void slotMenu_ActionJLMS(QAction *act);
    void on_tableView_2_clicked(const QModelIndex &index);
    void on_pushButton_clicked();
    void JLXL_XGSQ_Buttonclicked();
    void on_pushButton_2_clicked();
    void JLXL_ZDSQ_Buttonclicked();
};

#endif // MAINWINDOWFORSTATIONTRACK_H
