#include "mainwindowforstationtrack.h"
#include "ui_mainwindowforstationtrack.h"

#include <QThread>
#include <QDebug>
//添加定时器
#include <QTimerEvent>
//添加对话框组
#include <QDialog>
#include <QMessageBox>
#include <QPainter>
#include <complex>
#include "math.h"
#include <QDateTime>
#include <QSplitter>//分割窗口类头文件
#include <QTextEdit>
#include <QDockWidget>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <Windows.h>
#include <QPushButton>
#include <QLabel>
#include <QToolBar>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QScrollArea>
#include <QIcon>
#include <QLayout>
#include <QWidget>
#include <QSettings>
#include <QListView>


#include "mainwindow.h"
#include "trackview_lable.h"
#include "dbservice.h"
#include "dialog_menu_actionjlcf_mx.h"

MainWindowForStationTrack::MainWindowForStationTrack(MainWindow *p,QWidget *parent) :
    pmainWindow(p),
    QMainWindow(parent),
    ui(new Ui::MainWindowForStationTrack)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("TDCS/CTC3.0 通用车站操作终端"+pmainWindow->Version));
//    this->setWindowFlags(Qt::Dialog/*|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint*/);
    this->setWindowFlags(Qt::Tool);
    this->move(0,0);

    Track_widget_line_Flag=0;
    TableViewReadOnly_Flag=false;
    TableViewReadOnly_state=0;
    RefreshGDViewFaceFlag=true;
//    label_scrollArea = new QLabel(ui->scrollArea);
//    label_scrollArea->setGeometry(0,0,1920,1010);
    for(int i=0;i<20;i++)
    {
        TrackArray_formainwindow[i].Track_TrainNumber=1;
        TrackArray_formainwindow[i].Track_TrainNumber_Flag=0;
        TrackArray_formainwindow[i].train_num_1=true;//第一辆列车
        TrackArray_formainwindow[i].train_num_2=false;//第二辆列车
        TrackArray_formainwindow[i].train_num_3=false;//第三辆列车
        for(int j=0;j<3;j++)
        {
            TrackArray_formainwindow[i].Track_TrainNumber_Save[j]=0;
        }


        TrackArray_formainwindow[i].NewTrain=false;
    }
//    TrackArray_formainwindow[0].Track_TrainNumber=2;

    TrackNum=pmainWindow->m_nStationGDNode;
    LoginState=pmainWindow->LoginState;
    for (int i=0;i<TrackNum;i++)
    {
        TrackArray_formainwindow[i].strGDName=pmainWindow->StationGDNode[i].strGDName;
    }
    widgetline_Teftside_M=0;
    widgetline_Topside_M=0;
//    OpenPlan2and3=false;
    vect_list_readFlag=false;
    EndFlag=false;
    temp_DDML_Flag=true;

    JDJHNum_JDJHQS_1=0;
    for (int i=0;i<100;i++)
    {
        list_stageplan_detail_JDJHQS[i].state_JDJHQS=false;
    }
    red_Flesh_times=0;
//    red_Flesh_times_DDML=0;
    DDPushButton_TG_Debug=false;


    ui->lineEdit->setText("进路序列模式：<只读>");
    ui->lineEdit->setStyleSheet("background-color:rgb(240, 200, 0)");//背景色
    ui->lineEdit->setReadOnly(true);

    //启动定时器
    Timer_ID_1 = startTimer(100);
    Timer_ID_1000 = startTimer(1000);
    Timer_ID_2000 = startTimer(2000);

    DealWindowAction();
//    DealTrackData_Main();
    DealTrackData_Main_1();
    DealTrackData_Main_2_3();
    DealTrackData_Down();
//    DealSequenceWindowAction();





    ui->label->installEventFilter(this);
//    ui->tableView->installEventFilter(this);
//    ui->scrollArea->installEventFilter(this);
//    ui->tableView_2->installEventFilter(this);
//    ui->tableView_3->installEventFilter(this);
}

MainWindowForStationTrack::~MainWindowForStationTrack()
{
    delete ui;
}

/***************************************************************/

void MainWindowForStationTrack::DealWindowAction()
{
//整体窗口：
    ui->splitter->setStyleSheet("background-color: rgb(220, 220, 220)");
//窗口1：


//窗口2：

    QStringList m_List_1;
    m_List_1 << "序";
    m_List_1 << "注意确认";
    m_List_1 << "车次";
    m_List_1 << "股道";
    m_List_1 << "自触";
    m_List_1 << "类型";
    m_List_1 << "状态";
    m_List_1 << "开始时间";
    m_List_1 << "计划时间";
    m_List_1 << "方向";
    m_List_1 << "序列描述";
    m_pModel_1 = new QStandardItemModel(ui->tableView_2);
    m_pModel_1->setHorizontalHeaderLabels(m_List_1);
    ui->tableView_2->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableView_2->setModel(m_pModel_1);
    ui->tableView_2->setColumnWidth(0,20);
    ui->tableView_2->setColumnWidth(1,100);
    ui->tableView_2->setColumnWidth(2,110);
    ui->tableView_2->setColumnWidth(3,70);
    ui->tableView_2->setColumnWidth(4,40);
    ui->tableView_2->setColumnWidth(5,70);
    ui->tableView_2->setColumnWidth(6,70);
    ui->tableView_2->setColumnWidth(7,110);
    ui->tableView_2->setColumnWidth(8,110);
    ui->tableView_2->setColumnWidth(9,130);
    ui->tableView_2->setColumnWidth(10,104);
    ui->tableView_2->horizontalHeader()->setHighlightSections(false);
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_2->setContextMenuPolicy(Qt::CustomContextMenu); //可弹出右键菜单
    connect(ui->tableView_2, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));

    pmainWindow->initBTJLinfo();//初始化变通进路




//窗口3：
    QStandardItemModel *m_pModel_2;
    QStringList m_List_2;
    m_List_2 << "勾";
    m_List_2 << "线路";
    m_List_2 << "方法";
    m_List_2 << "辆数";
    m_List_2 << "方向";
    m_List_2 << "牵出线";
    m_List_2 << "状态";
    m_List_2 << "迁出进路";
    m_List_2 << "推入进路";
    m_List_2 << "记事";
    m_pModel_2 = new QStandardItemModel(ui->tableView_3);
    m_pModel_2->setHorizontalHeaderLabels(m_List_2);
    ui->tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView_3->setModel(m_pModel_2);
    ui->tableView_3->setColumnWidth(0,20);
    ui->tableView_3->setColumnWidth(1,100);
    ui->tableView_3->setColumnWidth(2,60);
    ui->tableView_3->setColumnWidth(3,60);
    ui->tableView_3->setColumnWidth(4,60);
    ui->tableView_3->setColumnWidth(5,150);
    ui->tableView_3->setColumnWidth(6,80);
    ui->tableView_3->setColumnWidth(7,120);
    ui->tableView_3->setColumnWidth(8,120);
    ui->tableView_3->setColumnWidth(9,190);

//    QList<QStandardItem*> temp3;
//    temp3<<new QStandardItem(QString("1"));num2++;
//    temp3<<new QStandardItem(QString("客15"));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    temp3<<new QStandardItem(QString("0"));num2++;
//    temp3<<new QStandardItem(QString("南"));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    temp3<<new QStandardItem(QString("已完成"));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    m_pModel_2->appendRow(temp3);
//    temp3.clear();
//    for (int i=0;i<num2;i++)
//    {
//        temp3.removeFirst();
//    }
//    num2=0;

//    temp3<<new QStandardItem(QString("2"));num2++;
//    temp3<<new QStandardItem(QString("库2"));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    temp3<<new QStandardItem(QString("1"));num2++;
//    temp3<<new QStandardItem(QString("南"));num2++;
//    temp3<<new QStandardItem(QString("库2"));num2++;
//    temp3<<new QStandardItem(QString("已完成"));num2++;
//    temp3<<new QStandardItem(QString("XP15-D76"));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    temp3<<new QStandardItem(QString(""));num2++;
//    m_pModel_2->appendRow(temp3);
//    temp3.clear();
//    for (int i=0;i<num2;i++)
//    {
//        temp3.removeFirst();
//    }
//    num2=0;

////阶段计划窗口JDJH：
//        QStringList m_List_JDJH;
//        m_List_JDJH << "来向";
//        m_List_JDJH << "接车口";
//        m_List_JDJH << "车次";
//        m_List_JDJH << "到达时间";
//        m_List_JDJH << "出发时间";
//        m_List_JDJH << "去向";
//        m_List_JDJH << "发车口";
//        m_List_JDJH << "机车";
//        m_List_JDJH << "签收状态";

//        m_pModel_JDJH = new QStandardItemModel();
//        m_pModel_JDJH->setHorizontalHeaderLabels(m_List_JDJH);


//调度命令处理窗口DDML：
        QStringList m_List_DDML;
        m_List_DDML << "调度命令编号";
        m_List_DDML << "发送时间";
        m_List_DDML << "接收时间";
        m_List_DDML << "签收时间";
        m_List_DDML << "打印时间";
        m_List_DDML << "调度命令类型";

        m_pModel_DDML = new QStandardItemModel();
        m_pModel_DDML->setHorizontalHeaderLabels(m_List_DDML);
}

void MainWindowForStationTrack::SequenceViewFace()//初始化
{
    QString str;
    for(int i=0;i<vect_list_route.count();i++)
    {
        temp2_Flag=false;
        for(int j=0;j<route_id_save.count();j++)
        {
            if(route_id_save[j]==vect_list_route[i].route_id)
            {
                temp2_Flag=true;
            }
        }
        if(temp2_Flag==false)
        {
            route_id_save.append(vect_list_route[i].route_id);
//            temp2<<new QStandardItem(QString::number(vect_list_route[i].route_id));//"序";
            temp2<<new QStandardItem(QString::number(i+1));//"序";
            temp2<<new QStandardItem(QString(""));//"注意确认";
            temp2<<new QStandardItem(QString(vect_list_route[i].trainnum));//"车次";
            temp2<<new QStandardItem(QString(vect_list_route[i].track));//"股道";

            if(vect_list_route[i].autotouch==0)
            {
                str="";
            }
            else if(vect_list_route[i].autotouch==1)
            {
                str="   √";
            }
            temp2<<new QStandardItem(QString(str));//"自触";

            if(vect_list_route[i].route_type==0)
            {
                str="接车";
            }
            else if(vect_list_route[i].route_type==1)
            {
                str="发车";
            }
            temp2<<new QStandardItem(QString(str));//"类型";
            str=vect_list_route[i].route_stateDescrip;
            temp2<<new QStandardItem(QString(str));//"状态";
            str=vect_list_route[i].timereal.left(19).right(8);
            temp2<<new QStandardItem(QString(str));//"开始时间";
            str=vect_list_route[i].timeplan.left(19).right(8);
            temp2<<new QStandardItem(QString(str));//"计划时间";
            str=vect_list_route[i].direction;
            temp2<<new QStandardItem(QString(str));//"方向";
            str=vect_list_route[i].descrip;
            temp2<<new QStandardItem(QString(str));//"序列描述";
            m_pModel_1->appendRow(temp2);

            if(vect_list_route[i].kehuoflag==0)
            {
                for (int k=0;k<temp2.count();k++)
                {
                    m_pModel_1->item(i,k)->setForeground(QBrush(QColor(0, 0, 0)));
                }
            }
            else if(vect_list_route[i].kehuoflag==1)
            {
                for (int k=0;k<temp2.count();k++)
                {
                    m_pModel_1->item(i,k)->setForeground(QBrush(QColor(255, 0, 0)));
                }
            }
            for (int k=0;k<temp2.count();k++)
            {
                m_pModel_1->item(i,k)->setBackground(QColor(RGB(0,255,255)));
            }
            temp2.clear();
        }
    }
}

void MainWindowForStationTrack::SequenceViewFace_refresh()//实时刷新
{
    QString str;
//    for(int i=0;i<vect_list_route_save.count();i++)
    for(int i=0;i<vect_list_route.count();i++)
    {
        if(vect_list_route[i].kehuoflag==0)
        {
            for (int k=0;k<m_pModel_1->columnCount();k++)
            {
                m_pModel_1->item(i,k)->setForeground(QBrush(QColor(0, 0, 0)));
            }
        }
        else if(vect_list_route[i].kehuoflag==1)
        {
            for (int k=0;k<m_pModel_1->columnCount();k++)
            {
                m_pModel_1->item(i,k)->setForeground(QBrush(QColor(255, 0, 0)));
            }
        }
//        m_pModel_1->item(i,0)->setText(QString::number(vect_list_route[i].route_id));//"序";
        m_pModel_1->item(i,0)->setText(QString::number(i+1));//"序";
        m_pModel_1->item(i,1)->setText("");//"注意确认";
        m_pModel_1->item(i,2)->setText(QString(vect_list_route[i].trainnum));//"车次";
        m_pModel_1->item(i,3)->setText(QString(vect_list_route[i].track));//"股道";
        if(vect_list_route[i].autotouch==0)
        {
            str="";
        }
        else if(vect_list_route[i].autotouch==1)
        {
            str="   √";
        }
        m_pModel_1->item(i,4)->setText(QString(str));//"自触";
        if(vect_list_route[i].route_type==0)
        {
            str="接车";
        }
        else if(vect_list_route[i].route_type==1)
        {
            str="发车";
        }
        m_pModel_1->item(i,5)->setText(QString(str));//"类型";
        str=vect_list_route[i].route_stateDescrip;
        m_pModel_1->item(i,6)->setText(QString(str));//"状态";
        if((vect_list_route[i].route_state==1)||(vect_list_route[i].route_state==2)||(vect_list_route[i].route_state==3))
        {
            for (int k=0;k<11;k++)
            {
                m_pModel_1->item(i,k)->setBackground(QColor(RGB(0,255,0)));
            }
        }
        else if(vect_list_route[i].route_state==4)
        {
            for (int k=0;k<11;k++)
            {
                m_pModel_1->item(i,k)->setBackground(QColor(RGB(128,128,128)));
            }
        }
        else
        {
            for (int k=0;k<11;k++)
            {
                m_pModel_1->item(i,k)->setBackground(QColor(RGB(0,255,255)));
            }
        }

        str=vect_list_route[i].timereal.left(19).right(8);
        m_pModel_1->item(i,7)->setText(QString(str));//"开始时间";
        str=vect_list_route[i].timeplan.left(19).right(8);
        m_pModel_1->item(i,8)->setText(QString(str));//"计划时间";
        str=vect_list_route[i].direction;
        m_pModel_1->item(i,9)->setText(QString(str));//"方向";
        str=vect_list_route[i].descrip;
        m_pModel_1->item(i,10)->setText(QString(str));//"序列描述";
    }
//    vect_list_route_save=vect_list_route;
}

//void MainWindowForStationTrack::SequenceViewFace()
//{
//    QString str;
//    for(int i=0;i<vect_list_route.count();i++)
//    {
//        temp2_Flag=false;
//        for(int j=0;j<route_id_save.count();j++)
//        {
//            if(route_id_save[j]==vect_list_route[i].route_id)
//            {
//                temp2_Flag=true;
//            }
//        }
//        if(temp2_Flag==false)
//        {
//            route_id_save.append(vect_list_route[i].route_id);
//            temp2<<new QStandardItem(QString::number(vect_list_route[i].route_id));//"序";
//            temp2<<new QStandardItem(QString(""));//"注意确认";
//            temp2<<new QStandardItem(QString(vect_list_route[i].trainnum));//"车次";
//            temp2<<new QStandardItem(QString(vect_list_route[i].track));//"股道";

//            if(vect_list_route[i].autotouch==0)
//            {
//                str="";
//            }
//            else if(vect_list_route[i].autotouch==1)
//            {
//                str="   √";
//            }
//            temp2<<new QStandardItem(QString(str));//"自触";

//            if(vect_list_route[i].route_type==0)
//            {
//                str="接车";
//            }
//            else if(vect_list_route[i].route_type==1)
//            {
//                str="发车";
//            }
//            temp2<<new QStandardItem(QString(str));//"类型";
//            str=vect_list_route[i].route_stateDescrip;
//            temp2<<new QStandardItem(QString(str));//"状态";
//            str=vect_list_route[i].timereal.left(19).right(8);
//            temp2<<new QStandardItem(QString(str));//"开始时间";
//            str=vect_list_route[i].timeplan.left(19).right(8);
//            temp2<<new QStandardItem(QString(str));//"计划时间";
//            str=vect_list_route[i].direction;
//            temp2<<new QStandardItem(QString(str));//"方向";
//            str=vect_list_route[i].descrip;
//            temp2<<new QStandardItem(QString(str));//"序列描述";
//            m_pModel_1->appendRow(temp2);

//            if(vect_list_route[i].kehuoflag==0)
//            {
//                for (int k=0;k<temp2.count();k++)
//                {
//                    m_pModel_1->item(i,k)->setForeground(QBrush(QColor(0, 0, 0)));
//                }
//            }
//            else if(vect_list_route[i].kehuoflag==1)
//            {
//                for (int k=0;k<temp2.count();k++)
//                {
//                    m_pModel_1->item(i,k)->setForeground(QBrush(QColor(255, 0, 0)));
//                }
//            }
//            for (int k=0;k<temp2.count();k++)
//            {
//                m_pModel_1->item(i,k)->setBackground(QColor(RGB(0,255,255)));
//            }

//            temp2.clear();

//        }
//    }
//}

//void MainWindowForStationTrack::SequenceViewFace_refresh()
//{
//    QString str;
////    for(int i=0;i<vect_list_route_save.count();i++)
//    for(int i=0;i<vect_list_route.count();i++)
//    {
//        if(vect_list_route[i].kehuoflag==0)
//        {
//            for (int k=0;k<11;k++)
//            {
//                m_pModel_1->item(i,k)->setForeground(QBrush(QColor(0, 0, 0)));
//            }
//        }
//        else if(vect_list_route[i].kehuoflag==1)
//        {
//            for (int k=0;k<11;k++)
//            {
//                m_pModel_1->item(i,k)->setForeground(QBrush(QColor(255, 0, 0)));
//            }
//        }
//        m_pModel_1->item(i,0)->setText(QString::number(vect_list_route[i].route_id));//"序";
//        m_pModel_1->item(i,1)->setText("");//"注意确认";
//        m_pModel_1->item(i,2)->setText(QString(vect_list_route[i].trainnum));//"车次";
//        m_pModel_1->item(i,3)->setText(QString(vect_list_route[i].track));//"股道";
//        if(vect_list_route[i].autotouch==0)
//        {
//            str="";
//        }
//        else if(vect_list_route[i].autotouch==1)
//        {
//            str="   √";
//        }
//        m_pModel_1->item(i,4)->setText(QString(str));//"自触";
//        if(vect_list_route[i].route_type==0)
//        {
//            str="接车";
//        }
//        else if(vect_list_route[i].route_type==1)
//        {
//            str="发车";
//        }
//        m_pModel_1->item(i,5)->setText(QString(str));//"类型";
//        str=vect_list_route[i].route_stateDescrip;
//        m_pModel_1->item(i,6)->setText(QString(str));//"状态";
//        if((vect_list_route[i].route_state==1)||(vect_list_route[i].route_state==2)||(vect_list_route[i].route_state==3))
//        {
//            for (int k=0;k<11;k++)
//            {
//                m_pModel_1->item(i,k)->setBackground(QColor(RGB(0,255,0)));
//            }
//        }
//        else if(vect_list_route[i].route_state==4)
//        {
//            for (int k=0;k<11;k++)
//            {
//                m_pModel_1->item(i,k)->setBackground(QColor(RGB(128,128,128)));
//            }
//        }
//        else
//        {
//            for (int k=0;k<11;k++)
//            {
//                m_pModel_1->item(i,k)->setBackground(QColor(RGB(0,255,255)));
//            }
//        }

//        str=vect_list_route[i].timereal.left(19).right(8);
//        m_pModel_1->item(i,7)->setText(QString(str));//"开始时间";
//        str=vect_list_route[i].timeplan.left(19).right(8);
//        m_pModel_1->item(i,8)->setText(QString(str));//"计划时间";
//        str=vect_list_route[i].direction;
//        m_pModel_1->item(i,9)->setText(QString(str));//"方向";
//        str=vect_list_route[i].descrip;
//        m_pModel_1->item(i,10)->setText(QString(str));//"序列描述";
//    }
//    vect_list_route_save=vect_list_route;
//}

void MainWindowForStationTrack::DealSequenceWindowAction()//数据库可视化处理
{
    QString  sql = (" select * from plan_routeorder");
    query_route = DBSERVICE.QueryDB(sql);
    model=new QSqlTableModel(ui->widget_3);
    model->setTable("plan_routeorder");//指定使用的表

    ui->tableView_2->setModel(model);
    model->select();

    model->setHeaderData(0,Qt::Horizontal,"序");
    model->removeColumn(32);
    model->removeColumn(31);
    model->removeColumn(30);
    model->removeColumn(29);
    model->removeColumn(28);
    model->removeColumn(27);
    model->removeColumn(26);
    model->removeColumn(25);
    model->removeColumn(24);
    model->removeColumn(23);
    model->removeColumn(21);
    model->removeColumn(18);
    model->removeColumn(16);
    model->removeColumn(15);
    model->removeColumn(14);
    model->removeColumn(13);
    model->removeColumn(11);
    model->removeColumn(10);
    model->removeColumn(6);
    model->removeColumn(3);
    model->removeColumn(2);
    model->removeColumn(1);

//    model->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置手动修改
//    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不能修改数据库
}

//void MainWindowForStationTrack::JDJHReceiveWin(int p)//初始化
//{
//    QString str;
////    JDJHNum_1=0;
////    for (int i=0;i<vect_list_stageplan_detail.count();i++)
////    {
////        if(vect_list_stageplan[p].plan_num==vect_list_stageplan_detail[i].plan_num)
////        {
////            JDJHNum_1++;
////        }
////    }

////    if(vect_list.count()!=vect_list_stageplan_detail.count())
////    {
////        return;
////    }
//    for(int i=0;i<vect_list_stageplan_detail.count();i++)
//    {
//        if(vect_list_stageplan[p].plan_num==vect_list_stageplan_detail[i].plan_num)
//        {
//            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].reach_xhd));//来向
//            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].reach_xhd));//接车口
//            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
//            {
//                str=vect_list_stageplan_detail[i].reach_trainnum;
//            }
//            else
//            {
//                str=vect_list_stageplan_detail[i].depart_trainnum;
//            }
//            temp_JDJH<<new QStandardItem(QString(str));//"车次";
//            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
//            {
//                str=vect_list_stageplan_detail[i].reach_timeplan.left(19).replace("T"," ");
//            }
//            else
//            {
//                str="--";
//            }
//            temp_JDJH<<new QStandardItem(QString(str));//到达时间
//            if(vect_list_stageplan_detail[i].plan_jfctype!=0xCC)
//            {
//                str=vect_list_stageplan_detail[i].depart_timeplan.left(19).replace("T"," ");
//            }
//            else
//            {
//                str="--";
//            }
//            temp_JDJH<<new QStandardItem(QString(str));//出发时间

//            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].depart_xhd));//去向
//            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].depart_xhd));//发车口
//            temp_JDJH<<new QStandardItem(QString(""));//机车

//            if(list_stageplan_detail_JDJHQS[i].state_JDJHQS==false)
//            {
//                temp_JDJH<<new QStandardItem(QString(""));//签收状态
//            }
//            else
//            {
//                temp_JDJH<<new QStandardItem(QString("已签收"));//签收状态
//            }
//            m_pModel_JDJH->appendRow(temp_JDJH);

//            temp_JDJH.clear();

//        }
//    }
//}
//void MainWindowForStationTrack::JDJHReceiveWin_refresh(int p)//实时刷新
//{
//    QString str;
//    JDJHNum_1=0;
////    if(vect_list.count()!=vect_list_stageplan_detail.count())
////    {
////        return;
////    }
//    for(int i=0;i<vect_list_stageplan_detail.count();i++)
//    {
//        if(vect_list_stageplan[p].plan_num==vect_list_stageplan_detail[i].plan_num)
//        {
//            m_pModel_JDJH->item(JDJHNum_1,0)->setText(QString(vect_list_stageplan_detail[i].reach_xhd));//来向
//            m_pModel_JDJH->item(JDJHNum_1,1)->setText(QString(vect_list_stageplan_detail[i].reach_xhd));//接车口
//            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
//            {
//                str=vect_list_stageplan_detail[i].reach_trainnum;
//            }
//            else
//            {
//                str=vect_list_stageplan_detail[i].depart_trainnum;
//            }
//            m_pModel_JDJH->item(JDJHNum_1,2)->setText(QString(str));//"车次";
//            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
//            {
//                str=vect_list_stageplan_detail[i].reach_timeplan.left(19).replace("T"," ");
//            }
//            else
//            {
//                str="--";
//            }
//            m_pModel_JDJH->item(JDJHNum_1,3)->setText(QString(str));//到达时间
//            if(vect_list_stageplan_detail[i].plan_jfctype!=0xCC)
//            {
//                str=vect_list_stageplan_detail[i].depart_timeplan.left(19).replace("T"," ");
//            }
//            else
//            {
//                str="--";
//            }
//            m_pModel_JDJH->item(JDJHNum_1,4)->setText(QString(str));//出发时间
//            m_pModel_JDJH->item(JDJHNum_1,5)->setText(QString(vect_list_stageplan_detail[i].depart_xhd));//去向
//            m_pModel_JDJH->item(JDJHNum_1,6)->setText(QString(vect_list_stageplan_detail[i].depart_xhd));//发车口
////            m_pModel_JDJH->item(i,7)->setText(QString());//机车
//            if(list_stageplan_detail_JDJHQS[i].state_JDJHQS!=false)  //待确定什么状态为已签收状态
//            {
//                m_pModel_JDJH->item(JDJHNum_1,8)->setText(QString("已签收"));//签收状态
//                pmainWindow->JDJHQSState=false;
////                pmainWindow->JDJHQSState_1=false;

//            }


//            JDJHNum_1++;
//        }
//    }
//}

void MainWindowForStationTrack::JDJHReceiveWin(int p)//初始化
{
    QString str;
    for(int i=0;i<vect_list_stageplan_detail.count();i++)
    {
        if(vect_list_stageplan[p].plan_num==vect_list_stageplan_detail[i].plan_num)
        {
            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].reach_xhd));//来向
            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].reach_xhd));//接车口
            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
            {
                str=vect_list_stageplan_detail[i].reach_trainnum;
            }
            else
            {
                str=vect_list_stageplan_detail[i].depart_trainnum;
            }
            temp_JDJH<<new QStandardItem(QString(str));//"车次";
            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
            {
                str=vect_list_stageplan_detail[i].reach_timeplan.left(19).replace("T"," ");
            }
            else
            {
                str="--";
            }
            temp_JDJH<<new QStandardItem(QString(str));//到达时间
            if(vect_list_stageplan_detail[i].plan_jfctype!=0xCC)
            {
                str=vect_list_stageplan_detail[i].depart_timeplan.left(19).replace("T"," ");
            }
            else
            {
                str="--";
            }
            temp_JDJH<<new QStandardItem(QString(str));//出发时间

            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].depart_xhd));//去向
            temp_JDJH<<new QStandardItem(QString(vect_list_stageplan_detail[i].depart_xhd));//发车口
            temp_JDJH<<new QStandardItem(QString(""));//机车

            if(vect_list_stageplan_detail[i].signstate==0)
            {
                temp_JDJH<<new QStandardItem(QString(""));//签收状态
            }
            else
            {
                temp_JDJH<<new QStandardItem(QString("已签收"));//签收状态
            }
            m_pModel_JDJH->appendRow(temp_JDJH);

            temp_JDJH.clear();
        }
    }
}
void MainWindowForStationTrack::JDJHReceiveWin_refresh(int p)//实时刷新
{
    QString str;
    JDJHNum_1=0;
    for(int i=0;i<vect_list_stageplan_detail.count();i++)
    {
        if(vect_list_stageplan[p].plan_num==vect_list_stageplan_detail[i].plan_num)
        {
            m_pModel_JDJH->item(JDJHNum_1,0)->setText(QString(vect_list_stageplan_detail[i].reach_xhd));//来向
            m_pModel_JDJH->item(JDJHNum_1,1)->setText(QString(vect_list_stageplan_detail[i].reach_xhd));//接车口
            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
            {
                str=vect_list_stageplan_detail[i].reach_trainnum;
            }
            else
            {
                str=vect_list_stageplan_detail[i].depart_trainnum;
            }
            m_pModel_JDJH->item(JDJHNum_1,2)->setText(QString(str));//"车次";
            if(vect_list_stageplan_detail[i].plan_jfctype!=0xBB)
            {
                str=vect_list_stageplan_detail[i].reach_timeplan.left(19).replace("T"," ");
            }
            else
            {
                str="--";
            }
            m_pModel_JDJH->item(JDJHNum_1,3)->setText(QString(str));//到达时间
            if(vect_list_stageplan_detail[i].plan_jfctype!=0xCC)
            {
                str=vect_list_stageplan_detail[i].depart_timeplan.left(19).replace("T"," ");
            }
            else
            {
                str="--";
            }
            m_pModel_JDJH->item(JDJHNum_1,4)->setText(QString(str));//出发时间
            m_pModel_JDJH->item(JDJHNum_1,5)->setText(QString(vect_list_stageplan_detail[i].depart_xhd));//去向
            m_pModel_JDJH->item(JDJHNum_1,6)->setText(QString(vect_list_stageplan_detail[i].depart_xhd));//发车口
//            m_pModel_JDJH->item(i,7)->setText(QString());//机车
            if(vect_list_stageplan_detail[i].signstate==1)
            {
                m_pModel_JDJH->item(JDJHNum_1,8)->setText(QString("已签收"));//签收状态
                pmainWindow->JDJHQSState=false;
            }


            JDJHNum_1++;
        }
    }
}


void MainWindowForStationTrack::DDMLReceiveWin()//初始化
{
    QString str;
    for(int i=0;i<vect_list_disorder_recv.count();i++)
    {
        temp_DDML<<new QStandardItem(QString::number(vect_list_disorder_recv[i].order_num));//调度命令号码
        str=vect_list_disorder_recv[i].time_recv.left(19).replace("T"," ");
        temp_DDML<<new QStandardItem(QString(str));//发送时间
        temp_DDML<<new QStandardItem(QString(str));//接收时间
        str=vect_list_disorder_recv[i].sign_time.left(19).replace("T"," ");
        temp_DDML<<new QStandardItem(QString(str));//签收时间
        str="";
        temp_DDML<<new QStandardItem(QString(str));//打印时间
        str=vect_list_disorder_recv[i].order_type;
        temp_DDML<<new QStandardItem(QString(str));//调度命令类型
        m_pModel_DDML->appendRow(temp_DDML);
        temp_DDML.clear();
    }
}
void MainWindowForStationTrack::DDMLReceiveWin_refresh()//实时刷新
{
    QString str;
    for(int i=0;i<vect_list_disorder_recv.count();i++)
    {
        m_pModel_DDML->item(i,0)->setText(QString::number(vect_list_disorder_recv[i].order_num));//调度命令号码
        str=vect_list_disorder_recv[i].time_recv.left(19).replace("T"," ");
        m_pModel_DDML->item(i,1)->setText(QString(str));//发送时间
        m_pModel_DDML->item(i,2)->setText(QString(str));//接收时间
        str=vect_list_disorder_recv[i].sign_time.left(19).replace("T"," ");
        m_pModel_DDML->item(i,3)->setText(QString(str));//签收时间
        str="";
        m_pModel_DDML->item(i,4)->setText(QString(str));//打印时间
        str=vect_list_disorder_recv[i].order_type;
        m_pModel_DDML->item(i,5)->setText(QString(str));//调度命令类型
    }
}

/***************************************************************/

/***************************************************************/
//void MainWindowForStationTrack::mousePressEvent(QMouseEvent *event)
//{
//    //要继续保留QListWidget原有的点击事件.
//    QWidget::mousePressEvent(event);
//    //左键点击，然后跳出菜单.
//    if (event->button() == Qt::LeftButton)
//    {
//        m_menu_ZC->exec(event->pos());
//    }
//}
void MainWindowForStationTrack::slotContextMenu(QPoint pos)
{
    if(TableViewReadOnly_Flag)
    {
        auto index = ui->tableView_2->indexAt(pos);
        TableView_lineNum_Right=index.row();
    //    if (index.column()!=4)
        {
            m_menu_CFJL->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
        }
    }
}
void MainWindowForStationTrack::ContextMenu_LeftButton(const QModelIndex &index)
{
    if(TableViewReadOnly_Flag)
    {
        if (index.column()==4)
        {
            if(vect_list_route[TableView_lineNum_Left].autotouch==0)
            {
                m_delAction_ZC->setCheckable(true);
                m_delAction_ZC->setChecked(true);
                m_addAction_ZC->setCheckable(false);
                m_addAction_ZC->setChecked(false);
            }
            else if(vect_list_route[TableView_lineNum_Left].autotouch==1)
            {
                m_delAction_ZC->setCheckable(false);
                m_delAction_ZC->setChecked(false);
                m_addAction_ZC->setCheckable(true);
                m_addAction_ZC->setChecked(true);
            }
            m_menu_ZC->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
        }
    }
}
void MainWindowForStationTrack::ContextMenu_LeftButton_XLMS(const QModelIndex &index)
{
    if(TableViewReadOnly_Flag)
    {
        if (index.column()==10)
        {
//            ContextMenu_XLMS();
            bool JLMS_BTJL_Flag=false;
            JLMS_BTJL_num=0;
            QString descrip_save;
            QAction act[8];
            descrip_save=vect_list_route[TableView_lineNum_Left].descrip;
            for(int k=0;k<pmainWindow->StaConfigInfo.BTJLCount;k++)//如果进路没有进入该部分，则说明进路没有变通进路，所以进路将只显示单一进路
            {
                for (int l=0;l<pmainWindow->vectBTJL[k].vectBTJLChild.count();l++)
                {
                    if(vect_list_route[TableView_lineNum_Left].descripreal/*.replace("-",",")*/==pmainWindow->vectBTJL[k].vectBTJLChild[l])
                    {
                        JLMS_BTJL_Flag=true;
                    }
                }
            }
            if(JLMS_BTJL_Flag==false)
            {
                act[0].setText(descrip_save);
                m_addAction_JLMS.append(&act[0]);
                m_addAction_JLMS_Real.append(descrip_save);
                m_menu_JLMS->addAction(m_addAction_JLMS[0]);
                m_addAction_JLMS[0]->setCheckable(true);
                m_addAction_JLMS[0]->setChecked(true);
                m_addAction_JLMS[0]->setEnabled(false);

            }
            for(int k=0;k<pmainWindow->StaConfigInfo.BTJLCount;k++)//如果进路没有进入该部分，则说明进路没有变通进路，所以进路将只显示单一进路
            {
                for (int q=0;q<pmainWindow->vectBTJL[k].vectBTJLChild.count();q++)
                {
                    if(pmainWindow->vectBTJL[k].vectBTJLChild[q]==vect_list_route[TableView_lineNum_Left].descripreal)
                    {
                        for (int l=0;l<pmainWindow->vectBTJL[k].vectBTJLChild.count();l++)
                        {
    //                        JLMS_BTJL_num++;
    //                        descrip_save=pmainWindow->vectBTJL[k].vectBTJLChild[l].replace(",","-");
                            descrip_save=pmainWindow->vectBTJL[k].vectBTJLChild[l];
                            m_addAction_JLMS_Real.append(descrip_save);
                            descrip_save=descrip_save.replace(",","-");
                            act[JLMS_BTJL_num].setText(descrip_save);
                            m_addAction_JLMS.append(&act[JLMS_BTJL_num]);
                            m_menu_JLMS->addAction(m_addAction_JLMS[JLMS_BTJL_num]);
                            JLMS_BTJL_num++;
                        }
                        m_addAction_JLMS[q]->setCheckable(true);
                        m_addAction_JLMS[q]->setChecked(true);
                        m_addAction_JLMS[q]->setEnabled(false);

                    }
                }
            }
            for(int p=0;p<JLMS_BTJL_num;p++)
            {
                m_addAction_JLMS[p]->setData(p);
            }
            connect(m_menu_JLMS, &QMenu::triggered, this, &MainWindowForStationTrack::slotMenu_ActionJLMS);//此处不应该建立QAction与函数的链接，而应该建立QMenu与槽函数的链接
            m_menu_JLMS->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
            m_addAction_JLMS.clear();
            m_addAction_JLMS_Real.clear();
        }
    }
}

/***************************************************************/


/***************************************************************/
//站名显示
void MainWindowForStationTrack::StationNameShow()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    QString stationName;
    stationID = pmainwindow->STATIONID;
    stationName = pmainwindow->STATIONNAME;

    QPainter painter;
    painter.begin(ui->label);
//    painter.setRenderHint(QPainter::Antialiasing);//设置圆滑绘制风格（抗锯齿）
    QPen pen;
    pen.setWidth(5);
    pen.setColor(QColor(0,180,255));
    ui->label->setFont(QFont("Agency FB",12,75));
    painter.setPen(pen);
    str_Time=QDateTime::currentDateTime().toString("yyyy-MM-dd   hh:mm:ss   ddd");
    painter.drawText(QRect(750,5,800,30),stationName+"车站股道视图   "+str_Time);
    //ui->label->setGeometry(0,0,1920, 30);

}
void MainWindowForStationTrack::RailwayinterfaceShow()
{

}
void MainWindowForStationTrack::BorderShow_Up()
{
    QPainter painter;
    painter.begin(ui->scrollArea);
    ui->scrollArea->setGeometry(0,30,ui->widget->width(),ui->widget->height()-32);
//    ui->widget_line_02

}
void MainWindowForStationTrack::BorderShow_Left()
{
    QPainter painter;
    painter.begin(ui->tableView_2);
    ui->tableView_2->setGeometry(0,30,ui->widget_3->width(),ui->widget_3->height());
    ui->label_2->setGeometry(0,0,ui->widget_3->width(),30);
    //ui->tableView_2->setRowHidden()

}
void MainWindowForStationTrack::BorderShow_Right()
{
    QPainter painter;
    painter.begin(ui->tableView_3);
    ui->tableView_3->setGeometry(0,30,ui->widget_4->width(),ui->widget_4->height());
    ui->label_3->setGeometry(0,0,ui->widget_4->width(),30);

}
/***************************************************************/


/***************************************************************/
//过滤函数//用于在控件上绘制图形
bool MainWindowForStationTrack::eventFilter(QObject *watched, QEvent *event)   //用过滤器eventFilter（）拦截QLabel中的QEvent::Paint事件
{
    if(watched ==ui->label && event->type() == QEvent::Paint)
    {
        StationNameShow();
    }
//    if(watched ==ui->scrollArea && event->type() == QEvent::Paint)
//    {
//        BorderShow_Up();
//    }
//    if(watched ==ui->tableView_2 && event->type() == QEvent::Paint)
//    {
//        BorderShow_Left();
//    }
//    if(watched ==ui->tableView_3 && event->type() == QEvent::Paint)
//    {
//        BorderShow_Right();
//    }

    return QWidget::eventFilter(watched,event);
}

/***************************************************************/


/***************************************************************/
void MainWindowForStationTrack::timerEvent(QTimerEvent *event)//定时器
{
    ui->label->update();
//    ui->scrollArea->update();
//    ui->tableView_2->update();
//    ui->tableView_3->update();

    if(event->timerId()==Timer_ID_1000)
    {
//        QString str_Time_1=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
//        pmainWindow->SendUDPDataRegister_CTC();
        pmainWindow->SendUDPDataHeartBeat_CTC();
//        qDebug()<<str_Time_1;


//        for (int i=0;i<JDJHNum_JDJHQS_1;i++)
//        {
//            if(list_stageplan_detail_JDJHQS[i].state_JDJHQS==false)
//            {
//                pmainWindow->JDJHQSState=true;
//            }
//        }
//        if(pmainWindow->JDJHQSState==true)//阶段计划未签收时，按钮红闪
//        {
//            if(pmainWindow->JDJHQSState_Flag==true)
//            {
//                pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);}");
////                pmainWindow->btn9->show();
//                pmainWindow->JDJHQSState_Flag=false;
//            }
//            else if(pmainWindow->JDJHQSState_Flag==false)
//            {
//                pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                                    "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
//                                    "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                                    "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                                    "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
//                                    );
////                pmainWindow->btn9->show();
//                pmainWindow->JDJHQSState_Flag=true;
//            }
//        }



//        bool m_state=false;
//        for (int i=0;i<JDJHNum_JDJHQS_1;i++)
//        {
//            if(list_stageplan_detail_JDJHQS[i].state_JDJHQS==false)
//            {
//                m_state=true;
//            }
//            if(m_state==false)
//            {
//                pmainWindow->JDJHQSState=false;
//            }
//        }
//        if(pmainWindow->JDJHQSState==false)//阶段计划未签收时，按钮红闪
//        {
//            pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                                "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
//                                "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                                "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                                "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
//                                );
////            pmainWindow->JDJHQSState_1=true;
//        }


        int l=0;
        for (int i=0;i<vect_list_stageplan_detail.count();i++)
        {
            if(vect_list_stageplan_detail[i].signstate==0)
            {
                l++;
//                pmainWindow->JDJHQSState=true;
            }
            if(l>0)
            {
                pmainWindow->JDJHQSState=true;
                red_Flesh_times=5;
            }
            else
            {
                pmainWindow->JDJHQSState=false;
            }
        }
        for(int i=0;i<vect_list_station_list.count();i++)
        {
            if((vect_list_station_list[i].auto_sign_stage==0)&&(vect_list_station_list[i].station_id == pmainWindow->STATIONID))
            {
                if(pmainWindow->JDJHQSState==true)//阶段计划未签收时，按钮红闪
                {
                    if(pmainWindow->JDJHQSState_Flag==true)
                    {
                        pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);}");
                        pmainWindow->JDJHQSState_Flag=false;
                    }
                    else if(pmainWindow->JDJHQSState_Flag==false)
                    {
                        pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                            "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                            "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                            "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                            "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                            );
                        pmainWindow->JDJHQSState_Flag=true;
                    }
                }
                else
                {
                    pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                        );
                }
            }
            else if((vect_list_station_list[i].auto_sign_stage==1)&&(vect_list_station_list[i].station_id==pmainWindow->STATIONID))
            {
                if(red_Flesh_times>0)//阶段计划未签收时，按钮红闪
                {
                    if(pmainWindow->JDJHQSState_Flag==true)
                    {
                        pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);}");
                        pmainWindow->JDJHQSState_Flag=false;
                    }
                    else if(pmainWindow->JDJHQSState_Flag==false)
                    {
                        pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                            "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                            "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                            "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                            "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                            );
                        pmainWindow->JDJHQSState_Flag=true;
                    }
                    red_Flesh_times--;
                }
                else if(red_Flesh_times<=0)
                {
                    pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                        );
                }
            }
            if((vect_list_station_list[i].station_id == pmainWindow->STATIONID)&&(vect_list_station_list[i].route_permit==2))
            {
                TableViewReadOnly_state=2;
                JLXL_XGSQ_Buttonclicked();

            }
            else if((vect_list_station_list[i].station_id == pmainWindow->STATIONID)&&(vect_list_station_list[i].route_permit==1))
            {
                TableViewReadOnly_state=1;
                JLXL_ZDSQ_Buttonclicked();

            }
            else if((vect_list_station_list[i].station_id == pmainWindow->STATIONID)&&(vect_list_station_list[i].route_permit==0))
            {
                TableViewReadOnly_state=0;
                JLXL_ZDSQ_Buttonclicked();

            }
        }


        int r=0;
        for (int i=0;i<vect_list_disorder_recv.count();i++)
        {
            if(vect_list_disorder_recv[i].order_state!=3)
            {
                r++;
            }
            if(r>0)
            {
                pmainWindow->DDMLQSState=true;
            }
            else
            {
                pmainWindow->DDMLQSState=false;
            }
        }

        if(pmainWindow->DDMLQSState==true)//调度命令未签收时，按钮红闪
        {
            if(pmainWindow->DDMLQSState_Flag==true)
            {
                pmainWindow->btn10->setStyleSheet("QPushButton{background-color: rgb(255, 0, 0);}");
                pmainWindow->DDMLQSState_Flag=false;
            }
            else if(pmainWindow->DDMLQSState_Flag==false)
            {
                pmainWindow->btn10->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                                 "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                                 "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                                 "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                                 "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                                 );
                pmainWindow->DDMLQSState_Flag=true;
            }
        }
        else
        {
            pmainWindow->btn10->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                             "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                             "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                             "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                             "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                             );
        }


        //到点变绿
        for (int i=0;i<vect_list.count();i++)
        {
            if(vect_list[i].depart_timereal!=NULL)
            {
                for (int j=0;j<TrackNum;j++)
                {
                    if(trackView_LableArray[j]->FDChangeGreenFlag>0)
                    {
                        trackView_LableArray[j]->FDChangeGreenFlag--;
                    }
                    if(trackView_LableArray1[j]->FDChangeGreenFlag>0)
                    {
                        trackView_LableArray1[j]->FDChangeGreenFlag--;
                    }
                    if(trackView_LableArray2[j]->FDChangeGreenFlag>0)
                    {
                        trackView_LableArray2[j]->FDChangeGreenFlag--;
                    }
                }
            }
        }



    }
    if(event->timerId()==Timer_ID_2000)
    {
//        if(OpenPlan2and3==false)
//        {
//            OpenPlan2and3=true;
//            DealTrackData_Main_2_3();
//        }
        ReadSQLStagePlan();
        ReadSQLrouteorder();
        ReadSQLstageplan();
        ReadSQLstageplan_1();
        ReadSQLstageplan_detail();
        ReadSQLdisorder_recv();
        ReadSQLgd_antislip();
        ReadSQL_base_entrance_exit();
        ReadSQL_base_train_fixedroute();
        ReadSQL_station_list();

        SequenceViewFace();
        SequenceViewFace_refresh();

        FLSetView();



        if(temp_DDML_Flag==true)
        {
            m_pModel_DDML->removeRows(0,m_pModel_DDML->rowCount());
            DDMLReceiveWin();
            DDMLReceiveWin_refresh();
            temp_DDML_Flag=false;
        }


        if((vect_list_stageplan_detail.count()==0)&&(vect_list_stageplan.count()==0))
        {
            pmainWindow->JDJHQSState=false;
            pmainWindow->btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                );
//            temp_JDJH.clear();
//            m_pModel_JDJH->appendRow(temp_JDJH);
//            m_pModel_JDJH->clear();
        }
//        ResetGDViewFace_QK();
        if(vect_list.count()==0)
        {
            if(EndFlag==true)
            {

                ResetGDViewFace();
                for (int j=0;j<TrackNum;j++)
                {

                    trackView_LableArray[j]->ReachDateFromSql_End(0,j);
                    trackView_LableArray1[j]->ReachDateFromSql_End(0,j);
                    trackView_LableArray2[j]->ReachDateFromSql_End(0,j);

                    //因删除trackView_LableArray[j]，造成在重置站场后会产生溢出错误
//                    trackView_LableArray[j]->setParent(NULL);
//                    trackView_LableArray1[j]->setParent(NULL);
//                    trackView_LableArray2[j]->setParent(NULL);
//                    delete  trackView_LableArray[j];
//                    delete  trackView_LableArray1[j];
//                    delete  trackView_LableArray2[j];

                    TrackArray_formainwindow[j].Track_TrainNumber=1;
                }
//                QWidget *a;
//                a=ui->scrollArea->takeWidget();

//                widget_line_01->hide();
//                widget_line_02->hide();
//                widget_line_03->hide();

//                for (int i=0;i<TrackNum;i++)
//                {
//                    widget_line_01[i]->setParent(NULL);
//                    widget_line_02[i]->setParent(NULL);
//                    widget_line_03[i]->setParent(NULL);
//                    delete widget_line_01[i];
//                    delete widget_line_02[i];
//                    delete widget_line_03[i];
//                    widget_line_01[i]->close();
//                    widget_line_02[i]->close();
//                    widget_line_03[i]->close();
//                }

//                label_scrollArea->clear();
//                label_scrollArea->show();
//                ui->scrollArea->takeWidget();
//                ui->scrollArea->show();

                DealTrackData_Main_Refresh();

                m_pModel_1->removeRows(0,m_pModel_1->rowCount());
//                m_pModel_JDJH->removeRows(0,m_pModel_JDJH->rowCount());
                route_id_save.clear();
                vect_list_route_RowsNum_Save=0;



                EndFlag=false;
            }
        }

        if(temp_DDML_Flag==true)
        {
            m_pModel_DDML->removeRows(0,m_pModel_DDML->rowCount());
            DDMLReceiveWin();
            DDMLReceiveWin_refresh();
            temp_DDML_Flag=false;
        }

        if(vect_list_route_RowsNum_Save!=vect_list_route.count())
        {
            m_pModel_1->removeRows(0,m_pModel_1->rowCount());
            route_id_save.clear();
            vect_list_route_RowsNum_Save=vect_list_route.count();
            SequenceViewFace();
            SequenceViewFace_refresh();
            DealTrackData_Main_Refresh();
            DDPushButton_TG_Debug=true;
        }

        TrainNum_list_Flag.clear();
        for(int i=0;i<vect_list.count();i++)
        {
            TrainNum_list_Flag.append(0);
        }
        for(int i=0;i<vect_list.count();i++)
        {
            for(int j=0;j<TrainNum_list.count();j++)
            {
                if((vect_list[i].reach_trainnum==TrainNum_list[j])||(vect_list[i].depart_trainnum==TrainNum_list[j]))
                {
                    TrainNum_list_Flag[i]=1;
                }
            }
        }


        for(int i=0;i<vect_list.count();i++)
        {
            if(TrainNum_list_Flag[i]==0)
            {
                vect_list_readFlag=true;

//                TrainTrack_Save.clear();
//                for(int j=0;j<vect_list_route.count();j++)
//                {
//                    TrainTrack_Save.append(vect_list_route[j].track);
//                }
            }
        }

        for(int i=0;i<vect_list_route.count();i++)
        {
            if(TrainTrack_Save[i]!=vect_list_route[i].track)
            {
                vect_list_readFlag=true;
            }
        }
//        TrainTrack_Save.clear();
        for(int i=0;i<40;i++)
        {
            TrainTrack_Save[i]="";
        }
        for(int j=0;j<vect_list_route.count();j++)
        {
            TrainTrack_Save[j]=vect_list_route[j].track;
        }


        if(vect_list_readFlag==true)
        {
            DealTrackData_Main_Refresh();

            vect_list_readFlag=false;
            EndFlag=true;
            TrainNum_list.clear();
            for (int i=0;i<vect_list.count();i++)
            {
                if(vect_list[i].plan_jfctype!=0xBB)
                {
                    TrainNum_list.append(vect_list[i].reach_trainnum);
                }
                else
                {
                    TrainNum_list.append(vect_list[i].depart_trainnum);
                }
                JCstate.append("办理接车");
                FCstate.append("办理发车");
            }

//            for (int i=0;i<vect_list.count();i++)
//            {
//                for (int j=0;j<TrackNum;j++)
//                {
//                    if((vect_list[i].reach_track==TrackArray_formainwindow[j].strGDName)||(vect_list[i].depart_track==TrackArray_formainwindow[j].strGDName))
//                    {
//                        if(TrackArray_formainwindow[j].Track_TrainNumber==1)
//                        {
//                            trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
//                        }
//                        else if(TrackArray_formainwindow[j].Track_TrainNumber==2)
//                        {
//                            trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
//                            trackView_LableArray1[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
//                        }
//                        else if(TrackArray_formainwindow[j].Track_TrainNumber==3)
//                        {
//                            trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
//                            trackView_LableArray1[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
//                            trackView_LableArray2[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[2],j);
//                        }
//                    }
//                }
//            }
//            TrainTrack_Save.clear();
//            for(int j=0;j<vect_list_route.count();j++)
//            {
//                TrainTrack_Save.append(vect_list_route[j].track);
//            }
            for(int i=0;i<40;i++)
            {
                TrainTrack_Save[i]="";
            }
            for(int j=0;j<vect_list_route.count();j++)
            {
                TrainTrack_Save[j]=vect_list_route[j].track;
            }
        }

//        if(RefreshGDViewFaceFlag==true)
//        {
//            RefreshGDViewFace();
//            RefreshGDViewFaceFlag=false;
//        }
        RefreshGDViewFace();
        DDPushButton_TG_Debug=false;
        for (int i=0;i<vect_list.count();i++)
        {
            for (int j=0;j<TrackNum;j++)
            {
                if((vect_list[i].reach_track==TrackArray_formainwindow[j].strGDName)||(vect_list[i].depart_track==TrackArray_formainwindow[j].strGDName))
                {
//                    if(TrackArray_formainwindow[j].Track_TrainNumber==1)
//                    {
//                        trackView_LableArray[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
//                        trackView_LableArray1[j]->ReachDateFromSql_End(0,j);
//                        trackView_LableArray2[j]->ReachDateFromSql_End(0,j);
//                    }
//                    else if(TrackArray_formainwindow[j].Track_TrainNumber==2)
//                    {
//                        trackView_LableArray[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
//                        trackView_LableArray1[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
//                        trackView_LableArray2[j]->ReachDateFromSql_End(0,j);
//                    }
//                    else if(TrackArray_formainwindow[j].Track_TrainNumber==3)
//                    {
//                        trackView_LableArray[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
//                        trackView_LableArray1[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
//                        trackView_LableArray2[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[2],j);
//                    }
                    //接预关闭
                    if(trackView_LableArray[j]->DilogCloseFlag>0)
                    {
                        trackView_LableArray[j]->DilogCloseFlag--;
                    }
                    if(trackView_LableArray[j]->DilogCloseFlag==0)
                    {
                        if(trackView_LableArray[j]->TrackStrArray[j].JYWindowOpenFlag==true)
                        {
                            trackView_LableArray[j]->JY_FY_zuoyewindow->hide();
                            trackView_LableArray[j]->TrackStrArray[j].JYWindowOpenFlag=false;
                        }
                    }
                    if(trackView_LableArray1[j]->DilogCloseFlag>0)
                    {
                        trackView_LableArray1[j]->DilogCloseFlag--;
                    }
                    if(trackView_LableArray1[j]->DilogCloseFlag==0)
                    {
                        if(trackView_LableArray1[j]->TrackStrArray[j].JYWindowOpenFlag==true)
                        {
                            trackView_LableArray1[j]->JY_FY_zuoyewindow->hide();
                            trackView_LableArray1[j]->TrackStrArray[j].JYWindowOpenFlag=false;
                        }
                    }
                    if(trackView_LableArray2[j]->DilogCloseFlag>0)
                    {
                        trackView_LableArray2[j]->DilogCloseFlag--;
                    }
                    if(trackView_LableArray2[j]->DilogCloseFlag==0)
                    {
                        if(trackView_LableArray2[j]->TrackStrArray[j].JYWindowOpenFlag==true)
                        {
                            trackView_LableArray2[j]->JY_FY_zuoyewindow->hide();
                            trackView_LableArray2[j]->TrackStrArray[j].JYWindowOpenFlag=false;
                        }
                    }
                    //接路关闭
                    if(trackView_LableArray[j]->JieLuState==1)
                    {
                        if(trackView_LableArray[j]->DilogCloseFlag_JL>0)
                        {
                            trackView_LableArray[j]->DilogCloseFlag_JL--;
                        }
                        if(trackView_LableArray[j]->DilogCloseFlag_JL==0)
                        {
                            if(trackView_LableArray[j]->TrackStrArray[j].JLWindowOpenFlag==true)
                            {
                                trackView_LableArray[j]->jlblWindow_jc->hide();
                                trackView_LableArray[j]->TrackStrArray[j].JLWindowOpenFlag=false;
                            }
                        }
                        if(trackView_LableArray1[j]->DilogCloseFlag_JL>0)
                        {
                            trackView_LableArray1[j]->DilogCloseFlag_JL--;
                        }
                        if(trackView_LableArray1[j]->DilogCloseFlag_JL==0)
                        {
                            if(trackView_LableArray1[j]->TrackStrArray[j].JLWindowOpenFlag==true)
                            {
                                trackView_LableArray1[j]->jlblWindow_jc->hide();
                                trackView_LableArray1[j]->TrackStrArray[j].JLWindowOpenFlag=false;
                            }
                        }
                        if(trackView_LableArray2[j]->DilogCloseFlag_JL>0)
                        {
                            trackView_LableArray2[j]->DilogCloseFlag_JL--;
                        }
                        if(trackView_LableArray2[j]->DilogCloseFlag_JL==0)
                        {
                            if(trackView_LableArray2[j]->TrackStrArray[j].JLWindowOpenFlag==true)
                            {
                                trackView_LableArray2[j]->jlblWindow_jc->hide();
                                trackView_LableArray2[j]->TrackStrArray[j].JLWindowOpenFlag=false;
                            }
                        }
                    }
                    else if(trackView_LableArray[j]->JieLuState==2)
                    {
                        if(trackView_LableArray[j]->DilogCloseFlag_JL>0)
                        {
                            trackView_LableArray[j]->DilogCloseFlag_JL--;
                        }
                        if(trackView_LableArray[j]->DilogCloseFlag_JL==0)
                        {
                            if(trackView_LableArray[j]->TrackStrArray[j].JLWindowOpenFlag==true)
                            {
                                trackView_LableArray[j]->jlblWindow_fc->hide();
                                trackView_LableArray[j]->TrackStrArray[j].JLWindowOpenFlag=false;
                            }
                        }
                        if(trackView_LableArray1[j]->DilogCloseFlag_JL>0)
                        {
                            trackView_LableArray1[j]->DilogCloseFlag_JL--;
                        }
                        if(trackView_LableArray1[j]->DilogCloseFlag_JL==0)
                        {
                            if(trackView_LableArray1[j]->TrackStrArray[j].JLWindowOpenFlag==true)
                            {
                                trackView_LableArray1[j]->jlblWindow_fc->hide();
                                trackView_LableArray1[j]->TrackStrArray[j].JLWindowOpenFlag=false;
                            }
                        }
                        if(trackView_LableArray2[j]->DilogCloseFlag_JL>0)
                        {
                            trackView_LableArray2[j]->DilogCloseFlag_JL--;
                        }
                        if(trackView_LableArray2[j]->DilogCloseFlag_JL==0)
                        {
                            if(trackView_LableArray2[j]->TrackStrArray[j].JLWindowOpenFlag==true)
                            {
                                trackView_LableArray2[j]->jlblWindow_fc->hide();
                                trackView_LableArray2[j]->TrackStrArray[j].JLWindowOpenFlag=false;
                            }
                        }
                    }

                }
            }
        }
    }
}

/***************************************************************/


/***************************************************************/
void MainWindowForStationTrack::DealTrackData_Main()
{
//    int widgetline_Teftside=0;
//    int widgetline_Topside=0;

//    for (int i=0;i<TrackNum;i++)//显示五个股道
    {
//        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
//        if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==1)
//        {
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
////            widget_line_01->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
////            trackView_Lable->show();
//            trackView_LableArray.append(trackView_Lable);
//            widgetline_Topside+=40;
//        }
//        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==2)
//        {
////            trackView_LableArray.clear();
////            trackView_LableArray1.clear();
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->show();
//            trackView_LableArray.append(trackView_Lable);
//            widgetline_Topside+=40;

//            widget_line_02 = new QWidget(ui->scrollArea);
//            widget_line_02->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_02->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//            trackView_Lable1->show();
//            trackView_LableArray1.append(trackView_Lable1);
//            widgetline_Topside+=40;
//        }
//        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==3)
//        {
//            trackView_LableArray.clear();
//            trackView_LableArray1.clear();
//            trackView_LableArray2.clear();
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->show();
//            trackView_LableArray.append(trackView_Lable);
//            widgetline_Topside+=40;

//            widget_line_02 = new QWidget(ui->scrollArea);
//            widget_line_02->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_02->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//            trackView_Lable1->show();
//            trackView_LableArray1.append(trackView_Lable1);
//            widgetline_Topside+=40;

//            widget_line_03 = new QWidget(ui->scrollArea);
//            widget_line_03->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_03->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_03->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_03->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=true;//第三辆列车
//            TrackView_Lable *trackView_Lable2 = new TrackView_Lable(this,widget_line_03);
//            trackView_Lable2->show();
//            trackView_LableArray2.append(trackView_Lable2);
//            widgetline_Topside+=40;
//        }
    }
}
/***************************************************************/

/***************************************************************/
//void MainWindowForStationTrack::DealTrackData_Main_1()
//{
//    int widgetline_Teftside=0;
//    int widgetline_Topside=0;
//    for (int i=0;i<TrackNum;i++)//显示五个股道
//    {
//        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_LableArray.append(trackView_Lable);
//            widgetline_Topside+=40;
//    }
//}
//void MainWindowForStationTrack::DealTrackData_Main_2_3()
//{
//    for (int i=0;i<TrackNum;i++)//显示五个股道
//    {
//        widget_line_02 = new QWidget(ui->scrollArea);
//        TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//        trackView_LableArray1.append(trackView_Lable1);

//        widget_line_03 = new QWidget(ui->scrollArea);
//        TrackView_Lable *trackView_Lable2 = new TrackView_Lable(this,widget_line_03);
//        trackView_LableArray2.append(trackView_Lable2);
//    }
//}
void MainWindowForStationTrack::DealTrackData_Main_1()
{
    int widgetline_Teftside=0;
    int widgetline_Topside=0;
    for (int i=0;i<TrackNum;i++)//显示五个股道
    {
        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
            widget_line_01[i] = new QLabel(ui->scrollArea);
//        widget_line_01[i] = new QLabel(label_scrollArea);
//            widget_line_01 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_01);
        widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
        if(Track_widget_line_Flag%2==1)
        {
            widget_line_01[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
        }
        else
        {
            widget_line_01[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
        }
        TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
        TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
        TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
        TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01[i]);
        trackView_LableArray.append(trackView_Lable);
        widgetline_Topside+=40;
    }
}
void MainWindowForStationTrack::DealTrackData_Main_2_3()
{
    for (int i=0;i<TrackNum;i++)//显示五个股道
    {
        widget_line_02[i] = new QLabel(ui->scrollArea);
//        widget_line_02[i] = new QLabel(label_scrollArea);
//        widget_line_02 = new QLabel();
        TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02[i]);
        trackView_LableArray1.append(trackView_Lable1);

        widget_line_03[i] = new QLabel(ui->scrollArea);
//        widget_line_03[i] = new QLabel(label_scrollArea);
//        widget_line_03 = new QLabel();
        TrackView_Lable *trackView_Lable2 = new TrackView_Lable(this,widget_line_03[i]);
        trackView_LableArray2.append(trackView_Lable2);
    }
}
/***************************************************************/

//void MainWindowForStationTrack::DealTrackData_Main_Refresh()
//{
//    int widgetline_Teftside=0;
//    int widgetline_Topside=0;

//    for (int i=0;i<TrackNum;i++)//显示五个股道
//    {
//        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
//        if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==1)
//        {
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->show();
//            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
////            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
////            trackView_LableArray[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;
//        }
//        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==2)
//        {
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->show();
//            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
////            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
////            trackView_LableArray[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;

//            widget_line_02 = new QWidget(ui->scrollArea);
//            widget_line_02->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_02->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//            trackView_Lable1->show();
//            trackView_LableArray1[Track_widget_line_Flag]=trackView_Lable1;
////            trackView_LableArray1[Track_widget_line_Flag]->setParent(widget_line_02);
////            trackView_LableArray1[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;
//        }
//        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==3)
//        {
//            widget_line_01 = new QWidget(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->show();
//            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
////            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
////            trackView_LableArray[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;

//            widget_line_02 = new QWidget(ui->scrollArea);
//            widget_line_02->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_02->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//            trackView_Lable1->show();
//            trackView_LableArray1[Track_widget_line_Flag]=trackView_Lable1;
////            trackView_LableArray1[Track_widget_line_Flag]->setParent(widget_line_02);
////            trackView_LableArray1[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;

//            widget_line_03 = new QWidget(ui->scrollArea);
//            widget_line_03->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_03->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_03->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_03->show();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=true;//第三辆列车
//            TrackView_Lable *trackView_Lable2 = new TrackView_Lable(this,widget_line_03);
//            trackView_Lable2->show();
//            trackView_LableArray2[Track_widget_line_Flag]=trackView_Lable2;
////            trackView_LableArray2[Track_widget_line_Flag]->setParent(widget_line_03);
////            trackView_LableArray2[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;
//        }
//    }
////    for (int i=0;i<vect_list.count();i++)
////    {
////        for (int j=0;j<TrackNum;j++)
////        {
////            if((vect_list[i].reach_track==TrackArray_formainwindow[j].strGDName)||(vect_list[i].depart_track==TrackArray_formainwindow[j].strGDName))
////            {
////                if(TrackArray_formainwindow[j].Track_TrainNumber==1)
////                {
////                    trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
////                }
////                else if(TrackArray_formainwindow[j].Track_TrainNumber==2)
////                {
////                    trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
////                    trackView_LableArray1[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
////                }
////                else if(TrackArray_formainwindow[j].Track_TrainNumber==3)
////                {
////                    trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
////                    trackView_LableArray1[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
////                    trackView_LableArray2[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[2],j);
////                }
////            }
////        }
////    }
//}
void MainWindowForStationTrack::DealTrackData_Main_Refresh()
{
    int widgetline_Teftside=0;
    int widgetline_Topside=0;
    for (int i=0;i<TrackNum;i++)//显示五个股道
    {
        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
        if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==1)
        {
            widget_line_01[i] = new QLabel(ui->scrollArea);
//            widget_line_01[i] = new QLabel(label_scrollArea);
//            widget_line_01 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_01);
            widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            if(Track_widget_line_Flag%2==1)
            {
                widget_line_01[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
            }
            else
            {
                widget_line_01[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
            }
            widget_line_01[i]->show();
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01[i]);
            trackView_Lable->show();
            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
//            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
//            trackView_LableArray[Track_widget_line_Flag]->show();
            widgetline_Topside+=40;
        }
        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==2)
        {
            widget_line_01[i] = new QLabel(ui->scrollArea);
//            widget_line_01[i] = new QLabel(label_scrollArea);
//            widget_line_01 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_01);
            widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            if(Track_widget_line_Flag%2==1)
            {
                widget_line_01[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
            }
            else
            {
                widget_line_01[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
            }
            widget_line_01[i]->show();
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01[i]);
            trackView_Lable->show();
            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
//            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
//            trackView_LableArray[Track_widget_line_Flag]->show();
            widgetline_Topside+=40;

            widget_line_02[i] = new QLabel(ui->scrollArea);
//            widget_line_02[i] = new QLabel(label_scrollArea);
//            widget_line_02 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_02);
            widget_line_02[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            if(Track_widget_line_Flag%2==1)
            {
                widget_line_02[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
            }
            else
            {
                widget_line_02[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
            }
            widget_line_02[i]->show();
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02[i]);
            trackView_Lable1->show();
            trackView_LableArray1[Track_widget_line_Flag]=trackView_Lable1;
//            trackView_LableArray1[Track_widget_line_Flag]->setParent(widget_line_02);
//            trackView_LableArray1[Track_widget_line_Flag]->show();
            widgetline_Topside+=40;
        }
        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==3)
        {
            widget_line_01[i] = new QLabel(ui->scrollArea);
//            widget_line_01[i] = new QLabel(label_scrollArea);
//            widget_line_01 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_01);
            widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            if(Track_widget_line_Flag%2==1)
            {
                widget_line_01[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
            }
            else
            {
                widget_line_01[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
            }
            widget_line_01[i]->show();
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01[i]);
            trackView_Lable->show();
            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
//            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
//            trackView_LableArray[Track_widget_line_Flag]->show();
            widgetline_Topside+=40;

            widget_line_02[i] = new QLabel(ui->scrollArea);
//            widget_line_02[i] = new QLabel(label_scrollArea);
//            widget_line_02 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_02);
            widget_line_02[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            if(Track_widget_line_Flag%2==1)
            {
                widget_line_02[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
            }
            else
            {
                widget_line_02[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
            }
            widget_line_02[i]->show();
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02[i]);
            trackView_Lable1->show();
            trackView_LableArray1[Track_widget_line_Flag]=trackView_Lable1;
//            trackView_LableArray1[Track_widget_line_Flag]->setParent(widget_line_02);
//            trackView_LableArray1[Track_widget_line_Flag]->show();
            widgetline_Topside+=40;

            widget_line_03[i] = new QLabel(ui->scrollArea);
//            widget_line_03[i] = new QLabel(label_scrollArea);
//            widget_line_03 = new QLabel();
//            ui->scrollArea->setWidget(widget_line_03);
            widget_line_03[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            if(Track_widget_line_Flag%2==1)
            {
                widget_line_03[i]->setStyleSheet("background-color: rgb(190, 200, 215)");
            }
            else
            {
                widget_line_03[i]->setStyleSheet("background-color: rgb(200, 200, 200)");
            }
            widget_line_03[i]->show();
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=true;//第三辆列车
            TrackView_Lable *trackView_Lable2 = new TrackView_Lable(this,widget_line_03[i]);
            trackView_Lable2->show();
            trackView_LableArray2[Track_widget_line_Flag]=trackView_Lable2;
//            trackView_LableArray2[Track_widget_line_Flag]->setParent(widget_line_03);
//            trackView_LableArray2[Track_widget_line_Flag]->show();
            widgetline_Topside+=40;
        }
    }
    for (int i=0;i<vect_list.count();i++)
    {
        for (int j=0;j<TrackNum;j++)
        {
            if((vect_list[i].reach_track==TrackArray_formainwindow[j].strGDName)||(vect_list[i].depart_track==TrackArray_formainwindow[j].strGDName))
            {
                if(TrackArray_formainwindow[j].Track_TrainNumber==1)
                {
                    trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
                }
                else if(TrackArray_formainwindow[j].Track_TrainNumber==2)
                {
                    trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
                    trackView_LableArray1[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
                }
                else if(TrackArray_formainwindow[j].Track_TrainNumber==3)
                {
                    trackView_LableArray[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
                    trackView_LableArray1[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
                    trackView_LableArray2[j]->ReachDateFromSql_Init(TrackArray_formainwindow[j].Track_TrainNumber_Save[2],j);
                }
            }
        }
    }
}
/***************************************************************/
void MainWindowForStationTrack::DealTrackData_Down()
{
    QMenu *m_menu_SXX;//上下行下拉按钮
    QAction *m_addAction_SXX;
    QAction *m_delAction_SXX;
    m_menu_SXX = new QMenu(this);
    m_addAction_SXX = new QAction(m_menu_SXX);
    m_delAction_SXX = new QAction(m_menu_SXX);
    m_addAction_SXX->setText(QObject::tr("添加"));
    m_delAction_SXX->setText(QObject::tr("删除"));
    m_menu_SXX->addAction(m_addAction_SXX);
    m_menu_SXX->addAction(m_delAction_SXX);
//            connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//            connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    ui->pushButton_4->setMenu(m_menu_SXX);

    QMenu *m_menu_JCK;//接车口下拉按钮
    QAction *m_addAction_JCK;
    QAction *m_delAction_JCK;
    m_menu_JCK = new QMenu(this);
    m_addAction_JCK = new QAction(m_menu_JCK);
    m_delAction_JCK = new QAction(m_menu_JCK);
    m_addAction_JCK->setText(QObject::tr("添加"));
    m_delAction_JCK->setText(QObject::tr("删除"));
    m_menu_JCK->addAction(m_addAction_JCK);
    m_menu_JCK->addAction(m_delAction_JCK);
//            connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//            connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    ui->pushButton_5->setMenu(m_menu_JCK);

    QMenu *m_menu_FCK;//发车口下拉按钮
    QAction *m_addAction_FCK;
    QAction *m_delAction_FCK;
    m_menu_FCK = new QMenu(this);
    m_addAction_FCK = new QAction(m_menu_FCK);
    m_delAction_FCK = new QAction(m_menu_FCK);
    m_addAction_FCK->setText(QObject::tr("添加"));
    m_delAction_FCK->setText(QObject::tr("删除"));
    m_menu_FCK->addAction(m_addAction_FCK);
    m_menu_FCK->addAction(m_delAction_FCK);
//            connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//            connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    ui->pushButton_6->setMenu(m_menu_FCK);

    QMenu *m_menu_DC;//调车下拉按钮
    QAction *m_addAction_DC;
    QAction *m_delAction_DC;
    m_menu_DC = new QMenu(this);
    m_addAction_DC = new QAction(m_menu_DC);
    m_delAction_DC = new QAction(m_menu_DC);
    m_addAction_DC->setText(QObject::tr("添加"));
    m_delAction_DC->setText(QObject::tr("删除"));
    m_menu_DC->addAction(m_addAction_DC);
    m_menu_DC->addAction(m_delAction_DC);
//            connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//            connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    ui->pushButton_7->setMenu(m_menu_DC);

    QMenu *m_menu_JC;//机车下拉按钮
    QAction *m_addAction_JC;
    QAction *m_delAction_JC;
    m_menu_JC = new QMenu(this);
    m_addAction_JC = new QAction(m_menu_JC);
    m_delAction_JC = new QAction(m_menu_JC);
    m_addAction_JC->setText(QObject::tr("添加"));
    m_delAction_JC->setText(QObject::tr("删除"));
    m_menu_JC->addAction(m_addAction_JC);
    m_menu_JC->addAction(m_delAction_JC);
//            connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//            connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    ui->pushButton_9->setMenu(m_menu_JC);



    m_menu_ZC = new QMenu(this);
    m_addAction_ZC = new QAction(m_menu_ZC);
    m_delAction_ZC = new QAction(m_menu_ZC);
    m_addAction_ZC->setText(QObject::tr("设置自触"));
    m_delAction_ZC->setText(QObject::tr("取消自触"));
    m_menu_ZC->addAction(m_addAction_ZC);
    m_menu_ZC->addAction(m_delAction_ZC);
    connect(m_addAction_ZC, &QAction::triggered, this, &MainWindowForStationTrack::slotMenu_ActionSZZC);
    connect(m_delAction_ZC, &QAction::triggered, this, &MainWindowForStationTrack::slotMenu_ActionQXZC);

    m_menu_CFJL = new QMenu(this);
    m_addAction_CFJL = new QAction(m_menu_CFJL);
    m_addAction_CFJL_MX = new QAction(m_menu_CFJL);//明细
    m_addAction_CFJL_ZCXS = new QAction(m_menu_CFJL);//站场显示
    m_addAction_CFJL_SCXL = new QAction(m_menu_CFJL);//删除序列
    m_addAction_CFJL->setText(QObject::tr("触发进路"));
    m_addAction_CFJL_MX->setText(QObject::tr("明细"));//明细
    m_addAction_CFJL_ZCXS->setText(QObject::tr("站场显示"));//站场显示
    m_addAction_CFJL_ZCXS->setEnabled(false);//设置显灰
    m_addAction_CFJL_SCXL->setText(QObject::tr("删除序列"));//删除序列
    m_menu_CFJL->addAction(m_addAction_CFJL);
    m_menu_CFJL->addAction(m_addAction_CFJL_MX);
    m_menu_CFJL->addAction(m_addAction_CFJL_ZCXS);
    m_menu_CFJL->addSeparator();//设置分割线
    m_menu_CFJL->addAction(m_addAction_CFJL_SCXL);
    connect(m_addAction_CFJL, &QAction::triggered, this, &MainWindowForStationTrack::slotMenu_ActionJLCF);
    connect(m_addAction_CFJL_MX, &QAction::triggered, this, &MainWindowForStationTrack::slotMenu_ActionJLCF_MX);
    connect(m_addAction_CFJL_SCXL, &QAction::triggered, this, &MainWindowForStationTrack::slotMenu_ActionJLCF_SCXL);

    m_menu_JLMS = new QMenu(this);

}
/***************************************************************/

/***************************************************************/
QVector<MainWindowForStationTrack::trackArray> MainWindowForStationTrack::ReadSQLStagePlan()//实时读取数据库中的阶段计划
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;

    QString  sql = QString(" select * from plan_trafficlog WHERE station_id=%1 AND executeflag=0").arg(stationID);

    query = DBSERVICE.QueryDB(sql);
    if (query.exec())
    {
        vect_list.clear();
//        JCstate.clear();
//        FCstate.clear();
        {
            while(query.next())
            {
                list.log_ID=query.value("log_id").toInt();
                list.station_ID=query.value("station_id").toInt();
                list.plan_num=query.value("plan_num").toInt();
                list.staName=query.value("staName").toString();
                list.plan_jfctype=query.value("plan_jfctype").toInt();
                list.train_posistatus=query.value("train_posistatus").toString();
                list.train_nextproc=query.value("train_nextproc").toString();
                list.reach_trainnum=query.value("reach_trainnum").toString();
                list.reach_trainnumold=query.value("reach_trainnumold").toString();
                list.reach_track=query.value("reach_track").toString();
                list.reach_timeplan=query.value("reach_timeplan").toString();
                list.reach_timereal=query.value("reach_timereal").toString();
                list.reach_xhd=query.value("reach_xhd").toString();
                list.reach_routestatus=query.value("reach_routestatus").toInt();
                list.reach_direction=query.value("reach_direction").toString();
                list.depart_trainnum=query.value("depart_trainnum").toString();
                list.depart_trainnumold=query.value("depart_trainnumold").toString();
                list.depart_track=query.value("depart_track").toString();
                list.depart_timeplan=query.value("depart_timeplan").toString();
                list.depart_timereal=query.value("depart_timereal").toString();
                list.depart_xhd=query.value("depart_xhd").toString();
                list.depart_routestatus=query.value("depart_routestatus").toInt();
                list.depart_direction=query.value("depart_direction").toString();
                list.isdelete=query.value("isdelete").toInt();
                list.iselectric=query.value("iselectric").toInt();
                list.ultralimit_level=query.value("ultralimit_level").toInt();
                list.kehuoflag=query.value("kehuoflag").toInt();
                list.traintype=query.value("traintype").toString();
                list.runningtype=query.value("runningtype").toString();
                list.isxianlusuo=query.value("isxianlusuo").toInt();
                list.executeflag=query.value("executeflag").toInt();
                list.adjtstationfrom=query.value("adjtstationfrom").toInt();
                list.adjtstationto=query.value("adjtstationto").toInt();
                list.agradjtdepat_time=query.value("agradjtdepat_time").toString();
                list.adjtdepat_time=query.value("adjtdepat_time").toInt();
                list.adjtagrdepat_time=query.value("adjtagrdepat_time").toString();
                list.reach_ultralimitlevel=query.value("reach_ultralimitlevel").toInt();
                list.depart_ultralimitlevel=query.value("depart_ultralimitlevel").toInt();
                list.notes=query.value("notes").toInt();
                list.transportpassenger=query.value("transportpassenger").toInt();
                list.allowtracknotmatch=query.value("allowtracknotmatch").toInt();
                list.allowEntrnotmatch=query.value("allowEntrnotmatch").toInt();
                list.isArmy=query.value("isArmy").toInt();
                list.isImportant=query.value("isImportant").toInt();
                list.plan_checkState=query.value("plan_checkState").toInt();
                list.flowstatus_jiaoling=query.value("flowstatus_jiaoling").toInt();
                list.flowstatus_liejian=query.value("flowstatus_liejian").toInt();
                list.flowstatus_shangshui=query.value("flowstatus_shangshui").toInt();
                list.flowstatus_xiwu=query.value("flowstatus_xiwu").toInt();
                list.flowstatus_jiaopiao=query.value("flowstatus_jiaopiao").toInt();
                list.flowstatus_chengjiang=query.value("flowstatus_chengjiang").toInt();
                list.flowstatus_zhaigua=query.value("flowstatus_zhaigua").toInt();
                list.flowstatus_liewei=query.value("flowstatus_liewei").toInt();
                list.flowstatus_huojian=query.value("flowstatus_huojian").toInt();
                list.flowstatus_huancheng=query.value("flowstatus_huancheng").toInt();
                list.flowstatus_zhuangxie=query.value("flowstatus_zhuangxie").toInt();
                list.flowstatus_jiche=query.value("flowstatus_jiche").toInt();
                list.flowstatus_daokou=query.value("flowstatus_daokou").toInt();
                list.flowstatus_chehao=query.value("flowstatus_chehao").toInt();
                list.flowstatus_zongkong=query.value("flowstatus_zongkong").toInt();
                list.flowstatus_zhanwu=query.value("flowstatus_zhanwu").toInt();

                bool DateFlag=false;
                for(int i=0;i<vect_list.count();i++)
                {
                    if(list.plan_jfctype!=0xBB)
                    {
                        if(list.reach_trainnum==vect_list[i].reach_trainnum)
                        {
                            DateFlag=true;
                            vect_list[i]=list;
                        }
                    }
                    else
                    {
                        if(list.depart_trainnum==vect_list[i].depart_trainnum)
                        {
                            DateFlag=true;
                            vect_list[i]=list;
                        }
                    }
                }
                if(DateFlag==false)
                {
                    vect_list.push_back(list);
//                    JCstate.append("办理接车");
//                    FCstate.append("办理发车");
                    for (int i=0;i<TrackNum;i++)
                    {
                        TrackArray_formainwindow[i].Track_TrainNumber_Flag=0;
                    }
                    for (int i=0;i<vect_list.count();i++)
                    {
                        for (int j=0;j<TrackNum;j++)
                        {
                            if((vect_list[i].reach_track==TrackArray_formainwindow[j].strGDName)||(vect_list[i].depart_track==TrackArray_formainwindow[j].strGDName))
                            {
                                TrackArray_formainwindow[j].Track_TrainNumber_Save[TrackArray_formainwindow[j].Track_TrainNumber_Flag]=i;
                                TrackArray_formainwindow[j].Track_TrainNumber_Flag++;

                                if(TrackArray_formainwindow[j].Track_TrainNumber_Flag==1)
                                {
                                    TrackArray_formainwindow[j].Track_TrainNumber=1;
                                }
                                else if(TrackArray_formainwindow[j].Track_TrainNumber_Flag==2)
                                {
                                    TrackArray_formainwindow[j].Track_TrainNumber=2;
                                }
                                else if(TrackArray_formainwindow[j].Track_TrainNumber_Flag==3)
                                {
                                    TrackArray_formainwindow[j].Track_TrainNumber=3;
                                }
                            }
                        }
                    }
//                    DealTrackData_Main_Refresh();
                }
            }
        }
    }
    return vect_list;
}

QVector<MainWindowForStationTrack::trackArray_routeorder>  MainWindowForStationTrack::ReadSQLrouteorder()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from plan_routeorder WHERE station_id=%1").arg(stationID);
    query_route = DBSERVICE.QueryDB(sql);
    if (query_route.exec())
    {
        vect_list_route.clear();
        while(query_route.next())
        {
            list_route.route_id=query_route.value("route_id").toInt();
            list_route.station_ID=query_route.value("station_ID").toInt();
            list_route.plan_num=query_route.value("plan_num").toInt();
            list_route.plan_jfctype=query_route.value("plan_jfctype").toInt();
            list_route.route_type=query_route.value("route_type").toInt();
            list_route.track=query_route.value("track").toString();
            list_route.trainnum=query_route.value("trainnum").toString();
            list_route.trainnumold=query_route.value("trainnumold").toString();
            list_route.timeplan=query_route.value("timeplan").toString();
            list_route.timereal=query_route.value("timereal").toString();
            list_route.timeClear=query_route.value("timeClear").toString();
            list_route.entrance_xhd=query_route.value("entrance_xhd").toString();
            list_route.autotouch=query_route.value("autotouch").toInt();
            list_route.iselectric=query_route.value("iselectric").toInt();
            list_route.ultralimitlevel=query_route.value("ultralimitlevel").toInt();
            list_route.kehuoflag=query_route.value("kehuoflag").toInt();
            list_route.isxianlusuo=query_route.value("isxianlusuo").toInt();
            list_route.descrip=query_route.value("descrip").toString();
            list_route.descripreal=query_route.value("descripreal").toString();
            list_route.route_state=query_route.value("route_state").toInt();
            list_route.route_stateDescrip=query_route.value("route_stateDescrip").toString();
            list_route.successed=query_route.value("successed").toInt();
            list_route.direction=query_route.value("direction").toString();
            list_route.xhdstart=query_route.value("xhdstart").toString();
            list_route.xhdend=query_route.value("xhdend").toString();
            list_route.xhdyxend=query_route.value("xhdyxend").toString();
            list_route.isbtjl=query_route.value("isbtjl").toInt();
            list_route.arraybtjl=query_route.value("arraybtjl").toString();
            list_route.confirmed=query_route.value("confirmed").toInt();
            list_route.lsbRouteIndex=query_route.value("lsbRouteIndex").toInt();
            list_route.tempXhdArray=query_route.value("tempXhdArray").toString();
            list_route.tempXhdBtnArray=query_route.value("tempXhdBtnArray").toString();

            bool DateFlag=false;
            for(int i=0;i<vect_list_route.count();i++)
            {
                if((list_route.trainnum==vect_list_route[i].trainnum)&&(list_route.route_type==vect_list_route[i].route_type))
                {
                    DateFlag=true;
                    vect_list_route[i]=list_route;
                }
            }
            if(DateFlag==false)
            {
                vect_list_route.push_back(list_route);
            }
        }
    }
    return vect_list_route;
}

QVector<MainWindowForStationTrack::stageplan>  MainWindowForStationTrack::ReadSQLstageplan()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from plan_stageplan WHERE station_id=%1").arg(stationID);
    query_route = DBSERVICE.QueryDB(sql);
    if (query_route.exec())
    {
        vect_list_stageplan.clear();
        while(query_route.next())
        {
            list_stageplan.plan_id=query_route.value("plan_id").toInt();
            list_stageplan.station_id=query_route.value("station_id").toInt();
            list_stageplan.plan_num=query_route.value("plan_num").toInt();
            list_stageplan.plan_timerecv=query_route.value("plan_timerecv").toString();
            list_stageplan.dispatch=query_route.value("dispatch").toString();
            list_stageplan.dispatcher=query_route.value("dispatcher").toString();

            bool DateFlag=false;
            for(int i=0;i<vect_list_stageplan.count();i++)
            {
                if(list_stageplan.plan_num==vect_list_stageplan[i].plan_num)
                {
                    DateFlag=true;
                    vect_list_stageplan[i]=list_stageplan;
                }
            }
            if(DateFlag==false)
            {
                vect_list_stageplan.push_back(list_stageplan);
            }
        }
    }
    return vect_list_stageplan;
}

QVector<MainWindowForStationTrack::stageplan_1>  MainWindowForStationTrack::ReadSQLstageplan_1()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from plan_stageplan WHERE station_id=%1").arg(stationID);
    query_route = DBSERVICE.QueryDB(sql);
    if (query_route.exec())
    {
        while(query_route.next())
        {
            list_stageplan_1.plan_id=query_route.value("plan_id").toInt();
            list_stageplan_1.station_id=query_route.value("station_id").toInt();
            list_stageplan_1.plan_num=query_route.value("plan_num").toInt();

            bool DateFlag=false;
            for(int i=0;i<vect_list_stageplan_1.count();i++)
            {
                if(list_stageplan_1.plan_num==vect_list_stageplan_1[i].plan_num)
                {
                    DateFlag=true;
                    vect_list_stageplan_1[i]=list_stageplan_1;
                }
            }
            if(DateFlag==false)
            {
                vect_list_stageplan_1.push_back(list_stageplan_1);
                red_Flesh_times=5;
            }
        }
    }
    return vect_list_stageplan_1;
}

QVector<MainWindowForStationTrack::stageplan_detail>  MainWindowForStationTrack::ReadSQLstageplan_detail()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from plan_stageplan_detail WHERE station_id=%1").arg(stationID);
    query_route = DBSERVICE.QueryDB(sql);
    if (query_route.exec())
    {
        vect_list_stageplan_detail.clear();
        while(query_route.next())
        {
            list_stageplan_detail.detail_id=query_route.value("detail_id").toInt();
            list_stageplan_detail.plan_id=query_route.value("plan_id").toInt();
            list_stageplan_detail.station_id=query_route.value("station_id").toInt();
            list_stageplan_detail.plan_num=query_route.value("plan_num").toInt();
            list_stageplan_detail.plan_timerecv=query_route.value("plan_timerecv").toString();
            list_stageplan_detail.plan_kind=query_route.value("plan_kind").toInt();
            list_stageplan_detail.plan_jfctype=query_route.value("plan_jfctype").toInt();
            list_stageplan_detail.dispatch=query_route.value("dispatch").toString();
//            list_stageplan_detail.dispatcher=query_route.value("dispatcher").toString();
            list_stageplan_detail.signstate=query_route.value("signstate").toInt();
            list_stageplan_detail.reach_trainnum=query_route.value("reach_trainnum").toString();
            list_stageplan_detail.reach_trainnumold=query_route.value("reach_trainnumold").toString();
            list_stageplan_detail.reach_track=query_route.value("reach_track").toString();
            list_stageplan_detail.reach_timeplan=query_route.value("reach_timeplan").toString();
            list_stageplan_detail.reach_timereal=query_route.value("reach_timereal").toString();
            list_stageplan_detail.reach_xhd=query_route.value("reach_xhd").toString();
            list_stageplan_detail.depart_trainnum=query_route.value("depart_trainnum").toString();
            list_stageplan_detail.depart_trainnumold=query_route.value("depart_trainnumold").toString();
            list_stageplan_detail.depart_track=query_route.value("depart_track").toString();
            list_stageplan_detail.depart_timeplan=query_route.value("depart_timeplan").toString();
            list_stageplan_detail.depart_timereal=query_route.value("depart_timereal").toString();
            list_stageplan_detail.depart_xhd=query_route.value("depart_xhd").toString();
            list_stageplan_detail.isdelete=query_route.value("isdelete").toInt();
            list_stageplan_detail.iselectric=query_route.value("iselectric").toInt();
            list_stageplan_detail.ultralimitlevel=query_route.value("ultralimitlevel").toInt();
            list_stageplan_detail.kehuoflag=query_route.value("kehuoflag").toInt();
            list_stageplan_detail.traintype=query_route.value("traintype").toString();
            list_stageplan_detail.runningtype=query_route.value("runningtype").toString();
            list_stageplan_detail.isxianlusuo=query_route.value("isxianlusuo").toInt();

            bool DateFlag=false;
            for(int i=0;i<vect_list_stageplan_detail.count();i++)
            {
                if((list_stageplan_detail.reach_trainnum==vect_list_stageplan_detail[i].reach_trainnum)||(list_stageplan_detail.depart_trainnum==vect_list_stageplan_detail[i].depart_trainnum))
                {
                    DateFlag=true;
                    vect_list_stageplan_detail[i]=list_stageplan_detail;
                }
            }
            if(DateFlag==false)
            {
                vect_list_stageplan_detail.push_back(list_stageplan_detail);
            }
        }
        JDJHNum_JDJHQS_1 = vect_list_stageplan_detail.count();
    }
    return vect_list_stageplan_detail;
}

QVector<MainWindowForStationTrack::disorder_recv>  MainWindowForStationTrack::ReadSQLdisorder_recv()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from disorder_recv WHERE station_id=%1").arg(stationID);
    query_route = DBSERVICE.QueryDB(sql);
    if (query_route.exec())
    {
        vect_list_disorder_recv.clear();
        while(query_route.next())
        {
            list_disorder_recv.order_id=query_route.value("order_id").toInt();
            list_disorder_recv.station_id=query_route.value("station_id").toInt();
            list_disorder_recv.order_num=query_route.value("order_num").toInt();
            list_disorder_recv.time_recv=query_route.value("time_recv").toString();
            list_disorder_recv.disCenter=query_route.value("disCenter").toString();
            list_disorder_recv.disName=query_route.value("disName").toString();
            list_disorder_recv.order_type=query_route.value("order_type").toString();
            list_disorder_recv.content=query_route.value("content").toString();
            list_disorder_recv.recv_place=query_route.value("recv_place").toString();
            list_disorder_recv.sign_name=query_route.value("sign_name").toString();
            list_disorder_recv.sign_time=query_route.value("sign_time").toString();
            list_disorder_recv.readName=query_route.value("readName").toString();
            list_disorder_recv.order_state=query_route.value("order_state").toInt();

            bool DateFlag=false;
            for(int i=0;i<vect_list_disorder_recv.count();i++)
            {
                if(list_disorder_recv.order_id==vect_list_disorder_recv[i].order_id)
                {
                    DateFlag=true;
                    vect_list_disorder_recv[i]=list_disorder_recv;
                }
            }
            if(DateFlag==false)
            {
                vect_list_disorder_recv.push_back(list_disorder_recv);
            }
        }
    }
    return vect_list_disorder_recv;
}

QVector<MainWindowForStationTrack::gd_antislip>  MainWindowForStationTrack::ReadSQLgd_antislip()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from base_gd_info WHERE station_id=%1").arg(stationID);
    query_route = DBSERVICE.QueryDB(sql);
    if (query_route.exec())
    {
        vect_list_gd_antislip.clear();
        while(query_route.next())
        {
            list_gd_antislip.gd_id=query_route.value("gd_id").toInt();
            list_gd_antislip.station_id=query_route.value("station_id").toInt();
            list_gd_antislip.gdname=query_route.value("gdname").toString();
            list_gd_antislip.l_antislip=query_route.value("l_antislip").toInt();
            list_gd_antislip.l_txnum=query_route.value("l_txnum").toInt();
            list_gd_antislip.l_jgqnum=query_route.value("l_jgqnum").toInt();
            list_gd_antislip.l_meters=query_route.value("l_meters").toInt();
            list_gd_antislip.r_antislip=query_route.value("r_antislip").toInt();
            list_gd_antislip.r_txnum=query_route.value("r_txnum").toInt();
            list_gd_antislip.r_jgqnum=query_route.value("r_jgqnum").toInt();
            list_gd_antislip.r_meters=query_route.value("r_meters").toInt();
            list_gd_antislip.trainsnum=query_route.value("trainsnum").toInt();
            list_gd_antislip.gdattr=query_route.value("gdattr").toInt();//线路性质//0正线1到发线
            list_gd_antislip.gddir=query_route.value("gddir").toInt();//接发车方向 0上行1下行2上下行
            list_gd_antislip.gdjfattr=query_route.value("gdjfattr").toInt();//接发车类型 0客车1货车2客货车
            list_gd_antislip.gdoverlevel=query_route.value("gdoverlevel").toInt();//超限类型 0不能接发超限列车1一级超限2二级超限3超级超限
            list_gd_antislip.gdplatform=query_route.value("gdplatform").toInt();//站台 1高站台2低站台0无
            list_gd_antislip.gdisCRH=query_route.value("gdisCRH").toInt();//1允许动车组0不允许动车组
            list_gd_antislip.gdiswater=query_route.value("gdiswater").toInt();//1上水设备0无上水设备
            list_gd_antislip.gdisblowdown=query_route.value("gdisblowdown").toInt();//1排污设备0无排污设备
            list_gd_antislip.gdArmy=query_route.value("gdArmy").toInt();//1可军用列车 0不能接军用列车
            list_gd_antislip.gdeffectlength=query_route.value("gdeffectlength").toInt();//有效长
            list_gd_antislip.gdmaxtrain=query_route.value("gdmaxtrain").toInt();//最大容车数量
            list_gd_antislip.gdcatenary=query_route.value("gdcatenary").toInt();//1接触网0无接触网
            list_gd_antislip.gdheavytrain=query_route.value("gdheavytrain").toInt();//1接发重载列车0禁止重载列车
            list_gd_antislip.version=query_route.value("version").toString();//版本号

            bool DateFlag=false;
            for(int i=0;i<vect_list_gd_antislip.count();i++)
            {
                if(list_gd_antislip.gd_id==vect_list_gd_antislip[i].gd_id)
                {
                    DateFlag=true;
                    vect_list_gd_antislip[i]=list_gd_antislip;
                }
            }
            if(DateFlag==false)
            {
                vect_list_gd_antislip.push_back(list_gd_antislip);
            }
        }
    }
    return vect_list_gd_antislip;
}

QVector<MainWindowForStationTrack::base_entrance_exit>  MainWindowForStationTrack::ReadSQL_base_entrance_exit()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from base_entrance_exit WHERE station_id=%1").arg(stationID);
    query = DBSERVICE.QueryDB(sql);
    if (query.exec())
    {
        vect_list_entrance_exit.clear();
        while(query.next())
        {
            list_entrance_exit.id=query.value("id").toInt();
            list_entrance_exit.station_id=query.value("station_id").toInt();
            list_entrance_exit.jzxhname=query.value("jzxhname").toString();
            list_entrance_exit.enexname=query.value("enexname").toString();
            list_entrance_exit.enexdir=query.value("enexdir").toInt();//方向 0双向1下行进站2上行出站3下行出站4上行禁止。双线自动闭塞区段，按进站信号机的行别、属性 选择“下行进站/上行出站/下行出站/上行进站”。 单线自动闭塞、半自动闭塞区段，选择“双向”
            list_entrance_exit.Isoverlevel=query.value("Isoverlevel").toInt();//1允许一级超限 2允许二级超限 3允许三级超限 4-允许超级超限 0不允许超限列车
            list_entrance_exit.ispassengertrain=query.value("ispassengertrain").toInt();//1 允许旅客列车 0不允许旅客列车
            list_entrance_exit.isfreighttrain=query.value("isfreighttrain").toInt();//1 允许货物列车  0不允许货物列车

            bool DateFlag=false;
            for(int i=0;i<vect_list_entrance_exit.count();i++)
            {
                if(list_entrance_exit.id==vect_list_entrance_exit[i].id)
                {
                    DateFlag=true;
                    vect_list_entrance_exit[i]=list_entrance_exit;
                }
            }
            if(DateFlag==false)
            {
                vect_list_entrance_exit.push_back(list_entrance_exit);
            }
        }
    }
    return vect_list_entrance_exit;
}

QVector<MainWindowForStationTrack::station_list>  MainWindowForStationTrack::ReadSQL_station_list()
{
    QString  sql = (" select * from station_list");
    query = DBSERVICE.QueryDB(sql);
    if (query.exec())
    {
        vect_list_station_list.clear();
        while(query.next())
        {
            list_station_list.station_id=query.value("station_id").toInt();
            list_station_list.station_name=query.value("station_name").toString();
            list_station_list.route_permit=query.value("route_permit").toInt();//
            list_station_list.auto_sign_stage=query.value("auto_sign_stage").toInt();//
            list_station_list.fczk_mode=query.value("fczk_mode").toInt();//
            list_station_list.fszl_mode=query.value("fszl_mode").toInt();//

            bool DateFlag=false;
            for(int i=0;i<vect_list_station_list.count();i++)
            {
                if(list_station_list.station_id==vect_list_station_list[i].station_id)
                {
                    DateFlag=true;
                    vect_list_station_list[i]=list_station_list;
                }
            }
            if(DateFlag==false)
            {
                vect_list_station_list.push_back(list_station_list);
            }
        }
    }
    return vect_list_station_list;
}

QVector<MainWindowForStationTrack::base_train_fixedroute>  MainWindowForStationTrack::ReadSQL_base_train_fixedroute()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    unsigned int stationID;
    stationID = pmainwindow->STATIONID;
    QString  sql = QString(" select * from base_train_fixedroute WHERE station_id=%1").arg(stationID);
    query = DBSERVICE.QueryDB(sql);
    if (query.exec())
    {
        vect_list_train_fixedroute.clear();
        while(query.next())
        {
            list_train_fixedroute.id=query.value("id").toInt();
            list_train_fixedroute.station_id=query.value("station_id").toInt();
            list_train_fixedroute.gd_id=query.value("gd_id").toInt();
            list_train_fixedroute.arrivalnum=query.value("arrivalnum").toString();//到达车次
            list_train_fixedroute.arrivaltime=query.value("arrivaltime").toString();//到达时间
            list_train_fixedroute.departnum=query.value("departnum").toString();//出发车次
            list_train_fixedroute.departtime=query.value("departtime").toString();//出发时间
            list_train_fixedroute.entraid=query.value("entraid").toInt();//入口ID，始发0
            list_train_fixedroute.exitid=query.value("exitid").toInt();//发车ID，终到0
            list_train_fixedroute.beforestationid=query.value("beforestationid").toInt();//前场车站ID，始发0
            list_train_fixedroute.afterstationid=query.value("afterstationid").toInt();//后方车站ID，终到0
            list_train_fixedroute.stoppoint=query.value("stoppoint").toInt();//1 技术停点  0非技术停点  （后续列 0、1代表是否具有属性）
            list_train_fixedroute.deliveryorder=query.value("deliveryorder").toInt();//交令
            list_train_fixedroute.trianflag=query.value("trianflag").toInt();//机车
            list_train_fixedroute.cargocheck=query.value("cargocheck").toInt();//货检
            list_train_fixedroute.boardflag=query.value("boardflag").toInt();//乘降
            list_train_fixedroute.pickhang=query.value("pickhang").toInt();//摘挂
            list_train_fixedroute.transferflag=query.value("transferflag").toInt();//换乘
            list_train_fixedroute.Loadcargo=query.value("Loadcargo").toInt();//装卸
            list_train_fixedroute.dirtabsorption=query.value("dirtabsorption").toInt();//吸污
            list_train_fixedroute.crossflag=query.value("crossflag").toInt();//道口
            list_train_fixedroute.wagonnum=query.value("wagonnum").toInt();//车号
            list_train_fixedroute.waterflag=query.value("waterflag").toInt();//上水
            list_train_fixedroute.traincheck=query.value("traincheck").toInt();//列检
            list_train_fixedroute.Intercontrol=query.value("Intercontrol").toInt();//综控
            list_train_fixedroute.depotflag=query.value("depotflag").toInt();//站务
            list_train_fixedroute.payticket=query.value("payticket").toInt();//交票
            list_train_fixedroute.traintail=query.value("traintail").toInt();//列尾
            list_train_fixedroute.version=query.value("version").toInt();//版本号

            bool DateFlag=false;
            for(int i=0;i<vect_list_train_fixedroute.count();i++)
            {
                if(list_train_fixedroute.id==vect_list_train_fixedroute[i].id)
                {
                    DateFlag=true;
                    vect_list_train_fixedroute[i]=list_train_fixedroute;
                }
            }
            if(DateFlag==false)
            {
                vect_list_train_fixedroute.push_back(list_train_fixedroute);
            }
        }
    }
    return vect_list_train_fixedroute;
}
/***************************************************************/

/***************************************************************/
void MainWindowForStationTrack::slotMenu_ActionJLCF()//设置人工触发进路
{
    if(TableViewReadOnly_Flag)
    {
//        for (int k=0;k<11;k++)
//        {
//            m_pModel_1->item(TableView_lineNum_Right,k)->setBackground(QColor(RGB(0,255,0)));
//        }
//        QString strSql=QString("UPDATE plan_routeorder SET route_state=%1 WHERE station_id=%2 AND route_id ='%3'").arg(1).arg(1).arg(TableView_lineNum_Right+1);
//        query_route.exec(strSql);
//        strSql=QString("UPDATE plan_routeorder SET route_stateDescrip='%1' WHERE station_id=%2 AND route_id ='%3'").arg("正在触发...").arg(1).arg(TableView_lineNum_Right+1);
//        query_route.exec(strSql);

        if(vect_list_route[TableView_lineNum_Right].route_type==0)
        {
            if(vect_list[get_vect_list_IndexFROMvect_list_route(TableView_lineNum_Right)-1].agradjtdepat_time=="")
            {
                JL_FL_FCWindown("请先办理接预");
            }
            else
            {
                pmainWindow->JLXLCZ_JLXLID_RGCF=vect_list_route[TableView_lineNum_Right].route_id;
                pmainWindow->JLXLCZ_CFstate_RGCF=0xAA;
                pmainWindow->SendToTrainSimu_JLXLCZ_RGCF();
            }
        }
        else
        {
            if(vect_list[get_vect_list_IndexFROMvect_list_route(TableView_lineNum_Right)-1].adjtagrdepat_time=="")
            {
                JL_FL_FCWindown("请先办理发预");
            }
            else
            {
                pmainWindow->JLXLCZ_JLXLID_RGCF=vect_list_route[TableView_lineNum_Right].route_id;
                pmainWindow->JLXLCZ_CFstate_RGCF=0xAA;
                pmainWindow->SendToTrainSimu_JLXLCZ_RGCF();
            }
        }
    }
}

void MainWindowForStationTrack::slotMenu_ActionJLCF_MX()//打开明细
{
    if(TableViewReadOnly_Flag)
    {
        Dialog_Menu_ActionJLCF_MX *dialog_Menu_ActionJLCF_MX = new Dialog_Menu_ActionJLCF_MX(this);
        dialog_Menu_ActionJLCF_MX->exec();
    }
}

void MainWindowForStationTrack::slotMenu_ActionJLCF_SCXL()//删除序列
{
    if(TableViewReadOnly_Flag)
    {
        pmainWindow->JLXLCZ_JLXLID_SCXL=vect_list_route[TableView_lineNum_Right].route_id;
        pmainWindow->JLXLCZ_CFstate_SCXL=0xAA;
        pmainWindow->SendToTrainSimu_JLXLCZ_SCXL();
    }
}


void MainWindowForStationTrack::slotMenu_ActionSZZC()//设置自触
{
    if(TableViewReadOnly_Flag)
    {
//        QString strSql=QString("UPDATE plan_routeorder SET autotouch=%1 WHERE station_id=%2 AND route_id =%3").arg(1).arg(1).arg(TableView_lineNum_Left+1);
//        query_route.exec(strSql);

        if(vect_list_route[TableView_lineNum_Left].route_type==0)
        {
            if(vect_list[get_vect_list_IndexFROMvect_list_route(TableView_lineNum_Left)-1].agradjtdepat_time=="")
            {
//                QString str_Time_ZC=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//                pmainWindow->GDZYLC_ZYLX=0x02;//接预
//                pmainWindow->ZY_Time_year=str_Time_ZC.left(4).toInt();
//                pmainWindow->ZY_Time_mouth=str_Time_ZC.left(7).right(2).toInt();
//                pmainWindow->ZY_Time_day=str_Time_ZC.left(10).right(2).toInt();
//                pmainWindow->ZY_Time_hour=str_Time_ZC.left(13).right(2).toInt();
//                pmainWindow->ZY_Time_min=str_Time_ZC.left(16).right(2).toInt();
//                pmainWindow->ZY_Time_sec=str_Time_ZC.left(19).right(2).toInt();
//                pmainWindow->SendToTrainSimu_XCRZCZ();

                pmainWindow->JLXLCZ_JLXLID=vect_list_route[TableView_lineNum_Left].route_id;
                pmainWindow->JLXLCZ_CFFS=0xAA;
                pmainWindow->SendToTrainSimu_JLXLCZ_CFFSSZ();
            }
            else
            {
                pmainWindow->JLXLCZ_JLXLID=vect_list_route[TableView_lineNum_Left].route_id;
                pmainWindow->JLXLCZ_CFFS=0xAA;
                pmainWindow->SendToTrainSimu_JLXLCZ_CFFSSZ();
            }
        }
        else
        {
            if(vect_list[get_vect_list_IndexFROMvect_list_route(TableView_lineNum_Left)-1].adjtagrdepat_time=="")
            {
//                QString str_Time_ZC=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
//                pmainWindow->GDZYLC_ZYLX=0x01;//发预
//                pmainWindow->ZY_Time_year=str_Time_ZC.left(4).toInt();
//                pmainWindow->ZY_Time_mouth=str_Time_ZC.left(7).right(2).toInt();
//                pmainWindow->ZY_Time_day=str_Time_ZC.left(10).right(2).toInt();
//                pmainWindow->ZY_Time_hour=str_Time_ZC.left(13).right(2).toInt();
//                pmainWindow->ZY_Time_min=str_Time_ZC.left(16).right(2).toInt();
//                pmainWindow->ZY_Time_sec=str_Time_ZC.left(19).right(2).toInt();
//                pmainWindow->SendToTrainSimu_XCRZCZ();

                pmainWindow->JLXLCZ_JLXLID=vect_list_route[TableView_lineNum_Left].route_id;
                pmainWindow->JLXLCZ_CFFS=0xAA;
                pmainWindow->SendToTrainSimu_JLXLCZ_CFFSSZ();
            }
            else
            {
                pmainWindow->JLXLCZ_JLXLID=vect_list_route[TableView_lineNum_Left].route_id;
                pmainWindow->JLXLCZ_CFFS=0xAA;
                pmainWindow->SendToTrainSimu_JLXLCZ_CFFSSZ();
            }
        }
    }
}

void MainWindowForStationTrack::slotMenu_ActionQXZC()//取消自触
{
    if(TableViewReadOnly_Flag)
    {
//        QString strSql=QString("UPDATE plan_routeorder SET autotouch=%1 WHERE station_id=%2 AND route_id ='%3'").arg(0).arg(1).arg(TableView_lineNum_Left+1);
//        query_route.exec(strSql);

        pmainWindow->JLXLCZ_JLXLID=vect_list_route[TableView_lineNum_Left].route_id;
        pmainWindow->JLXLCZ_CFFS=0xBB;
        pmainWindow->SendToTrainSimu_JLXLCZ_CFFSSZ();
    }
}

void MainWindowForStationTrack::slotMenu_ActionJLMS(QAction *act)
{
    if(TableViewReadOnly_Flag)
    {
        for (int i=0;i<m_addAction_JLMS.count();i++)
        {
            if(act->data().toInt()==i)
            {
                QString str_BTJL;
                //str_BTJL=m_addAction_JLMS[i]->text().replace("-",",");
                str_BTJL=m_addAction_JLMS_Real[i];
//                QString strSql=QString("UPDATE plan_routeorder SET descrip='%1' WHERE station_id=%2 AND route_id ='%3'").arg(str_BTJL).arg(1).arg(TableView_lineNum_Left+1);
//                query_route.exec(strSql);
//                strSql=QString("UPDATE plan_routeorder SET descripreal='%1' WHERE station_id=%2 AND route_id ='%3'").arg(str_BTJL.replace("-",",")).arg(1).arg(TableView_lineNum_Left+1);
//                query_route.exec(strSql);
                pmainWindow->JLXLCZ_zfcNum_JLMS=str_BTJL.count();
                pmainWindow->JLXLCZ_str_JLMS=str_BTJL;
                pmainWindow->JLXLCZ_JLXLID_JLMS=vect_list_route[TableView_lineNum_Left].route_id;
                pmainWindow->SendToTrainSimu_JLXLCZ_XGJLMS();
            }
        }
    }
}

/**************************************************************/

void MainWindowForStationTrack::on_tableView_2_clicked(const QModelIndex &index)
{
    if(index.column()==4)
    {
        TableView_lineNum_Left=index.row();
        ContextMenu_LeftButton(index);
    }
    if(index.column()==10)
    {
        TableView_lineNum_Left=index.row();
        ContextMenu_LeftButton_XLMS(index);
    }
}

/**************************************************************/

void MainWindowForStationTrack::on_pushButton_clicked()
{
    if(TableViewReadOnly_state==0)
    {
        pmainWindow->JLXLCZ_XGZDSQ=0xAA;
        pmainWindow->SendToTrainSimu_JLXLCZ_XGZDSQ();
    }
}
void MainWindowForStationTrack::JLXL_XGSQ_Buttonclicked()
{
    ui->lineEdit->setText("进路序列模式：<可修改>");
    ui->lineEdit->setStyleSheet("background-color:green");//背景色
    ui->lineEdit->setReadOnly(true);
    TableViewReadOnly_Flag=true;
}

void MainWindowForStationTrack::on_pushButton_2_clicked()
{
    if(TableViewReadOnly_state==2)
    {
        pmainWindow->JLXLCZ_XGZDSQ=0xBB;
        pmainWindow->SendToTrainSimu_JLXLCZ_XGZDSQ();
    }
}

void MainWindowForStationTrack::JLXL_ZDSQ_Buttonclicked()
{
    ui->lineEdit->setText("进路序列模式：<只读>");
    ui->lineEdit->setStyleSheet("background-color:rgb(240, 200, 0)");//背景色
    ui->lineEdit->setReadOnly(true);
    TableViewReadOnly_Flag=false;
}
/**************************************************************/
void MainWindowForStationTrack::FLSetView()//防溜设置初始化
{
    QIcon icon_L;
    QIcon icon_R;
//    for(int i=0;i<TrackNum;i++)
    if(vect_list_gd_antislip.count()==TrackNum)
    {
        for(int i=0;i<vect_list_gd_antislip.count();i++)
        {
            if(vect_list_gd_antislip[i].l_antislip!=0)
            {
                if(vect_list_gd_antislip[i].l_antislip==1)
                {
                    icon_L.addFile("://data/res/左边铁鞋.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=1;
                }
                else if(vect_list_gd_antislip[i].l_antislip==2)
                {
                    icon_L.addFile("://data/res/制动机.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=2;
                }
                else if(vect_list_gd_antislip[i].l_antislip==4)
                {
                    icon_L.addFile("://data/res/紧固器.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=4;
                }
                else if(vect_list_gd_antislip[i].l_antislip==18)
                {
                    icon_L.addFile("://data/res/左边铁鞋+制动机.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=18;
                }
                else if(vect_list_gd_antislip[i].l_antislip==20)
                {
                    icon_L.addFile("://data/res/左边铁鞋+紧固器.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=20;
                }
                else
                {
                    icon_L.addFile("");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=0;
                }
                trackView_LableArray[i]->Btn2->setIcon(icon_L);
                trackView_LableArray[i]->Btn2->setIconSize(QSize(30,30));
                if(vect_list_gd_antislip[i].l_txnum!=0)
                {
                    trackView_LableArray[i]->label_FLSet_L->setText(QString::number(vect_list_gd_antislip[i].l_txnum));
                }
                else if(vect_list_gd_antislip[i].l_jgqnum!=0)
                {
                    trackView_LableArray[i]->label_FLSet_L->setText(QString::number(vect_list_gd_antislip[i].l_jgqnum));
                }
                else
                {
                    trackView_LableArray[i]->label_FLSet_L->setText("");
                }
            }
            else
            {
//                if(trackView_LableArray.count()>0)
//                {
//                    trackView_LableArray[i]->label_FLSet_L->setText("");
//                }
                trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_S=0;
            }
            if(vect_list_gd_antislip[i].r_antislip!=0)
            {
                if(vect_list_gd_antislip[i].r_antislip==1)
                {
                    icon_R.addFile("://data/res/右边铁鞋.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=1;
                }
                else if(vect_list_gd_antislip[i].r_antislip==2)
                {
                    icon_R.addFile("://data/res/制动机.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=2;
                }
                else if(vect_list_gd_antislip[i].r_antislip==4)
                {
                    icon_R.addFile("://data/res/紧固器.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=4;
                }
                else if(vect_list_gd_antislip[i].r_antislip==18)
                {
                    icon_R.addFile("://data/res/右边铁鞋+制动机.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=18;
                }
                else if(vect_list_gd_antislip[i].r_antislip==20)
                {
                    icon_R.addFile("://data/res/右边铁鞋+紧固器.bmp");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=20;
                }
                else
                {
                    icon_R.addFile("");
                    trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=0;
                }
                trackView_LableArray[i]->Btn37->setIcon(icon_R);
                trackView_LableArray[i]->Btn37->setIconSize(QSize(30,30));
                if(vect_list_gd_antislip[i].r_txnum!=0)
                {
                    trackView_LableArray[i]->label_FLSet_R->setText(QString::number(vect_list_gd_antislip[i].r_txnum));
                }
                else if(vect_list_gd_antislip[i].r_jgqnum!=0)
                {
                    trackView_LableArray[i]->label_FLSet_R->setText(QString::number(vect_list_gd_antislip[i].r_jgqnum));
                }
                else
                {
                    trackView_LableArray[i]->label_FLSet_R->setText("");
                }
            }
            else
            {
//                trackView_LableArray[i]->label_FLSet_R->setText("");
                trackView_LableArray[i]->TrackStrArray[trackView_LableArray[i]->Track_widget_line_Flag].TrainTX_State_X=0;
            }
        }
    }
}
/**************************************************************/

/**************************************************************/
void MainWindowForStationTrack::JL_FL_FCWindown(QString text)
{
//    MainWindow *pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    int length_Str=0;
    int length_label=0;
    length_Str=text.toLocal8Bit().length();
    length_label=length_Str*6;
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("TDCS/CTC3.0 通用车站操作终端");
    dialog->setGeometry(800,500,length_label+20,60);
    dialog->setStyleSheet("background-color: rgb(198, 210, 218);");
    QLabel *label = new QLabel(dialog);
    label->setGeometry(10,0,length_label,30);
    label->setText(text);
    label->setAlignment(Qt::AlignCenter);
    QPushButton *pushbutton = new QPushButton(dialog);
    pushbutton->setText("关闭");
    pushbutton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                              "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
                              "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                              "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                              "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");
    pushbutton->setGeometry(10,30,length_label,23);
    connect(pushbutton,&QPushButton::clicked,dialog,&QDialog::close);
    dialog->exec();
}
/**************************************************************/

/**************************************************************/
int MainWindowForStationTrack::get_vect_list_IndexFROMvect_list_route(int num)
{
    for(int i=0;i<vect_list.count();i++)
    {
        if(((vect_list_route[num].trainnum==vect_list[i].reach_trainnum)||(vect_list_route[num].trainnum==vect_list[i].depart_trainnum))
                &&((vect_list_route[num].track==vect_list[i].reach_track)||(vect_list_route[num].track==vect_list[i].depart_track)))
        {
            return(i+1);
        }
    }
    return(0);//返回0有可能会出错
}
int MainWindowForStationTrack::get_vect_list_route_IndexFROMvect_list(int num,int route_type)//route_type为0表示接车，route_type为1表示发车
{
    for(int i=0;i<vect_list_route.count();i++)
    {
        if(((vect_list_route[i].trainnum==vect_list[num].reach_trainnum)||(vect_list_route[i].trainnum==vect_list[num].depart_trainnum))
                &&((vect_list_route[i].track==vect_list[num].reach_track)||(vect_list_route[i].track==vect_list[num].depart_track))&&(vect_list_route[i].route_type==route_type))
        {
            return(i+1);
        }
    }
    return(0);//返回0有可能会出错
}
/**************************************************************/

/**************************************************************/
void MainWindowForStationTrack::RefreshGDViewFace()
{
    for (int i=0;i<vect_list.count();i++)
    {
        for (int j=0;j<TrackNum;j++)
        {
            if((vect_list[i].reach_track==TrackArray_formainwindow[j].strGDName)||(vect_list[i].depart_track==TrackArray_formainwindow[j].strGDName))
            {
                if(!((get_vect_list_route_IndexFROMvect_list(i,0)==0)&&(get_vect_list_route_IndexFROMvect_list(i,1)==0)))
                {
                    if(TrackArray_formainwindow[j].Track_TrainNumber==1)
                    {
                        trackView_LableArray[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
                        trackView_LableArray1[j]->ReachDateFromSql_End(0,j);
                        trackView_LableArray2[j]->ReachDateFromSql_End(0,j);
                    }
                    else if(TrackArray_formainwindow[j].Track_TrainNumber==2)
                    {
                        trackView_LableArray[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
                        trackView_LableArray1[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
                        trackView_LableArray2[j]->ReachDateFromSql_End(0,j);
                    }
                    else if(TrackArray_formainwindow[j].Track_TrainNumber==3)
                    {
                        trackView_LableArray[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[0],j);
                        trackView_LableArray1[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[1],j);
                        trackView_LableArray2[j]->ReachDateFromSql(TrackArray_formainwindow[j].Track_TrainNumber_Save[2],j);
                    }
                }
                else
                {
                    if(TrackArray_formainwindow[j].Track_TrainNumber==1)
                    {
                        trackView_LableArray[j]->ReachDateFromSql_End(0,j);
                    }
                    else if(TrackArray_formainwindow[j].Track_TrainNumber==2)
                    {
                        if(i==TrackArray_formainwindow[j].Track_TrainNumber_Save[0])
                        {
                            trackView_LableArray[j]->ReachDateFromSql_End(0,j);
                        }
                        if(i==TrackArray_formainwindow[j].Track_TrainNumber_Save[1])
                        {
                            trackView_LableArray1[j]->ReachDateFromSql_End(0,j);
                        }
                    }
                    else if(TrackArray_formainwindow[j].Track_TrainNumber==3)
                    {
                        if(i==TrackArray_formainwindow[j].Track_TrainNumber_Save[0])
                        {
                            trackView_LableArray[j]->ReachDateFromSql_End(0,j);
                        }
                        if(i==TrackArray_formainwindow[j].Track_TrainNumber_Save[1])
                        {
                            trackView_LableArray1[j]->ReachDateFromSql_End(0,j);
                        }
                        if(i==TrackArray_formainwindow[j].Track_TrainNumber_Save[2])
                        {
                            trackView_LableArray2[j]->ReachDateFromSql_End(0,j);
                        }
                    }
                }

            }
        }
    }
}
/**************************************************************/
//void MainWindowForStationTrack::ResetGDViewFace()
//{
//    int widgetline_Teftside=0;
//    int widgetline_Topside=0;
//    for (int i=0;i<TrackNum;i++)//显示五个股道
//    {
//        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
//        if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==1)
//        {
//            widget_line_01 = new QLabel(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->hide();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->hide();
//            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
////            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
////            trackView_LableArray[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;
//        }
//        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==2)
//        {
//            widget_line_01 = new QLabel(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->hide();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->hide();
//            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
////            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
////            trackView_LableArray[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;

//            widget_line_02 = new QLabel(ui->scrollArea);
//            widget_line_02->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_02->hide();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//            trackView_Lable1->hide();
//            trackView_LableArray1[Track_widget_line_Flag]=trackView_Lable1;
////            trackView_LableArray1[Track_widget_line_Flag]->setParent(widget_line_02);
////            trackView_LableArray1[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;
//        }
//        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==3)
//        {
//            widget_line_01 = new QLabel(ui->scrollArea);
//            widget_line_01->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_01->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_01->hide();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=false;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable = new TrackView_Lable(this,widget_line_01);
//            trackView_Lable->hide();
//            trackView_LableArray[Track_widget_line_Flag]=trackView_Lable;
////            trackView_LableArray[Track_widget_line_Flag]->setParent(widget_line_01);
////            trackView_LableArray[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;

//            widget_line_02 = new QLabel(ui->scrollArea);
//            widget_line_02->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_02->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_02->hide();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=false;//第三辆列车
//            TrackView_Lable *trackView_Lable1 = new TrackView_Lable(this,widget_line_02);
//            trackView_Lable1->hide();
//            trackView_LableArray1[Track_widget_line_Flag]=trackView_Lable1;
////            trackView_LableArray1[Track_widget_line_Flag]->setParent(widget_line_02);
////            trackView_LableArray1[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;

//            widget_line_03 = new QLabel(ui->scrollArea);
//            widget_line_03->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
//            if(Track_widget_line_Flag%2==1)
//            {
//                widget_line_03->setStyleSheet("background-color: rgb(190, 200, 215)");
//            }
//            else
//            {
//                widget_line_03->setStyleSheet("background-color: rgb(200, 200, 200)");
//            }
//            widget_line_03->hide();
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_1=true;//第一辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_2=true;//第二辆列车
//            TrackArray_formainwindow[Track_widget_line_Flag].train_num_3=true;//第三辆列车
//            TrackView_Lable *trackView_Lable2 = new TrackView_Lable(this,widget_line_03);
//            trackView_Lable2->hide();
//            trackView_LableArray2[Track_widget_line_Flag]=trackView_Lable2;
////            trackView_LableArray2[Track_widget_line_Flag]->setParent(widget_line_03);
////            trackView_LableArray2[Track_widget_line_Flag]->show();
//            widgetline_Topside+=40;
//        }
//    }
//}
void MainWindowForStationTrack::ResetGDViewFace()
{
    int widgetline_Teftside=0;
    int widgetline_Topside=0;
    for (int i=0;i<TrackNum;i++)//显示五个股道
    {
        Track_widget_line_Flag=i;//股道编号，是股道从上到下的唯一编号
        if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==1)
        {
//            widget_line_01[i] = new QLabel(label_scrollArea);
            widget_line_01[i] = new QLabel(ui->scrollArea);
            widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            widget_line_01[i]->setStyleSheet("background-color: rgb(220, 220, 220)");
            widget_line_01[i]->show();
            widgetline_Topside+=40;
        }
        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==2)
        {
//            widget_line_01[i] = new QLabel(label_scrollArea);
            widget_line_01[i] = new QLabel(ui->scrollArea);
            widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            widget_line_01[i]->setStyleSheet("background-color: rgb(220, 220, 220)");
            widget_line_01[i]->show();
            widgetline_Topside+=40;

//            widget_line_02[i] = new QLabel(label_scrollArea);
            widget_line_02[i] = new QLabel(ui->scrollArea);
            widget_line_02[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            widget_line_02[i]->setStyleSheet("background-color: rgb(220, 220, 220)");
            widget_line_02[i]->show();
            widgetline_Topside+=40;
        }
        else if(TrackArray_formainwindow[Track_widget_line_Flag].Track_TrainNumber==3)
        {
//            widget_line_01[i] = new QLabel(label_scrollArea);
            widget_line_01[i] = new QLabel(ui->scrollArea);
            widget_line_01[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            widget_line_01[i]->setStyleSheet("background-color: rgb(220, 220, 220)");
            widget_line_01[i]->show();
            widgetline_Topside+=40;

//            widget_line_02[i] = new QLabel(label_scrollArea);
            widget_line_02[i] = new QLabel(ui->scrollArea);
            widget_line_02[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            widget_line_02[i]->setStyleSheet("background-color: rgb(220, 220, 220)");
            widget_line_02[i]->show();
            widgetline_Topside+=40;

//            widget_line_03[i] = new QLabel(label_scrollArea);
            widget_line_03[i] = new QLabel(ui->scrollArea);
            widget_line_03[i]->setGeometry(widgetline_Teftside,widgetline_Topside,1920,40);
            widget_line_03[i]->setStyleSheet("background-color: rgb(220, 220, 220)");
            widget_line_03[i]->show();
            widgetline_Topside+=40;
        }
    }
}
/**************************************************************/

/**************************************************************/
void MainWindowForStationTrack::ResetGDViewFace_QK()
{
    if(vect_list.count()==0)
    {

            ResetGDViewFace();
            for (int j=0;j<TrackNum;j++)
            {

                trackView_LableArray[j]->ReachDateFromSql_End(0,j);
                trackView_LableArray1[j]->ReachDateFromSql_End(0,j);
                trackView_LableArray2[j]->ReachDateFromSql_End(0,j);

                //因删除trackView_LableArray[j]，造成在重置站场后会产生溢出错误
//                    trackView_LableArray[j]->setParent(NULL);
//                    trackView_LableArray1[j]->setParent(NULL);
//                    trackView_LableArray2[j]->setParent(NULL);
//                    delete  trackView_LableArray[j];
//                    delete  trackView_LableArray1[j];
//                    delete  trackView_LableArray2[j];

                TrackArray_formainwindow[j].Track_TrainNumber=1;
            }
//                QWidget *a;
//                a=ui->scrollArea->takeWidget();

//                widget_line_01->hide();
//                widget_line_02->hide();
//                widget_line_03->hide();

//                for (int i=0;i<TrackNum;i++)
//                {
//                    widget_line_01[i]->setParent(NULL);
//                    widget_line_02[i]->setParent(NULL);
//                    widget_line_03[i]->setParent(NULL);
//                    delete widget_line_01[i];
//                    delete widget_line_02[i];
//                    delete widget_line_03[i];
//                    widget_line_01[i]->close();
//                    widget_line_02[i]->close();
//                    widget_line_03[i]->close();
//                }

//                label_scrollArea->clear();
//                label_scrollArea->show();
//                ui->scrollArea->takeWidget();
//                ui->scrollArea->show();

            DealTrackData_Main_Refresh();

//            m_pModel_1->removeRows(0,m_pModel_1->rowCount());
////                m_pModel_JDJH->removeRows(0,m_pModel_JDJH->rowCount());
//            route_id_save.clear();
//            vect_list_route_RowsNum_Save=0;
        }
}
/**************************************************************/
/**************************************************************/
/**************************************************************/
