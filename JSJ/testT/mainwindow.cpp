#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "comm.h"
#include "Global.h"
#include "mypushbutton.h"
//#include "mainwindowforstationtrack.h"
#include "m_ReadStationData.h"
//#include "monitor.h"
#include "stationshowfrom.h"

#include "cmystation.h"
#include "zxbviewset.h"
#include "gdstviewset.h"
#include "stationdirection.h"
#include "functionpushbuttonset.h"
#include "bjtsset.h"
#include "jy_fy_zuoyewindow.h"
#include "dialog_jsjdjh.h"
#include "dialog_bwl.h"
#include "dialog_ddmlcl.h"
#include "dialog_powersupply.h"
#include "dialog_blockoperation.h"



//添加线程
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
#include <QStandardItemModel>
#include <QIcon>
#include <QLayout>
#include <QTextToSpeech>
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>
#include <QComboBox>

//#define Version  V1.2.1

MainWindow * MainWindow::pMainWindow = nullptr;//在其他类中调用MainWindow中变量的方法

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pMainWindow = this;//在其他类中调用MainWindow中变量的方法

    Version="V1.2.1";
    commState=false;
    LoginID="";
    LoginName="";
    TrackBMP_JLNum=0;
    LoginState=1;
    for (int i=0;i<300;i++)
    {
        BMPArray[i].JLIndex=0;
        BMPArray[i].JLType="";
        BMPArray[i].JLPushButton="";
        BMPArray[i].JLBMPPath="";
    }
    m_nStationGDNode=0;
    for (int i=0;i<50;i++)
    {
        StationGDNode[i].nCode=0;
        StationGDNode[i].strGDName="";
        StationGDNode[i].strLeftNode="";
        StationGDNode[i].strRightNode="";
    }
    //变通进路数量
    StaConfigInfo.BTJLCount=0;

    ReadIPConfig();
    InitUDPCommCenter_CTC();
    ReadRouteBmp();
    ReadGDConfig();
    ReadTrainNumTypeTXT();
    ReadConfigInfo();

    //mainWindowForStationTrack = new MainWindowForStationTrack(this);
    monitor=new Monitor();

    Menu_SetButton();
    Menu_T3Button();
    SplitWindow();

    JDJHQSState=false;
//    JDJHQSState_1=false;
    JDJHQSState_Flag=false;
    DDMLQSState=false;
    DDMLQSState_Flag=false;






}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow *MainWindow::getMainWinPtr()//在其他类中调用MainWindow中变量的方法
{
    return pMainWindow;
}

/***************************************************************/
////按钮窗口
//void MainWindow::SplitWindow()
//{
//    mainWindowForStationTrack = new MainWindowForStationTrack;
//    mainWindowForStationTrack->showMaximized();
//    mainWindowForStationTrack->show();

//    this->setStyleSheet("background-color: rgb(150, 150, 150)");
//    //按钮1：
//    MyPushButton *btn1 = new MyPushButton("://data/res/火车_外阴影4.png");
//    btn1->setParent(this);
//    btn1->move(5,5);
//    connect(btn1,&MyPushButton::clicked,[=](){
//        qDebug()<<"点击了1号按钮";
//        //this->hide();//隐藏自身
//    });

//    //按钮2：
//    MyPushButton *btn2 = new MyPushButton("://data/res/T3_外阴影.png","://data/res/T3_内阴影.png");
//    btn2->setParent(this);
//    btn2->move(35,5);
//    connect(btn2,&MyPushButton::clicked,[=](){qDebug()<<"点击了2号按钮";});

//    //按钮3：
//    MyPushButton *btn3 = new MyPushButton("://data/res/1_外阴影.png","://data/res/1_内阴影.png");
//    btn3->setParent(this);
//    btn3->move(65,5);
//    connect(btn3,&MyPushButton::clicked,[=](){qDebug()<<"点击了3号按钮";});

//    //按钮4：
//    MyPushButton *btn4 = new MyPushButton("://data/res/火车_外阴影4.png","://data/res/火车_内阴影.png");
//    btn4->setParent(this);
//    btn4->move(95,5);
//    connect(btn4,&MyPushButton::clicked,[=](){qDebug()<<"点击了4号按钮";});

//    //按钮5：
//    MyPushButton *btn5 = new MyPushButton("://data/res/火车_外阴影4.png");
//    btn5->setParent(this);
//    btn5->move(125,5);
//    connect(btn5,&MyPushButton::clicked,[=](){
//        qDebug()<<"点击了5号按钮";
//        mainWindowForStationTrack->showMaximized();
//        mainWindowForStationTrack->show();
//    });

//    //按钮6：
//    if(commState==false)
//    {
//        MyPushButton *btn6 = new MyPushButton("://data/res/断开.png");
//        btn6->setParent(this);
//        btn6->move(155,5);
//        connect(btn6,&MyPushButton::clicked,[=](){qDebug()<<"点击了6号按钮";});
//    }
//    else
//    {
//        MyPushButton *btn6 = new MyPushButton("://data/res/连接.png");
//        btn6->setParent(this);
//        btn6->move(155,5);
//        connect(btn6,&MyPushButton::clicked,[=](){qDebug()<<"点击了6号按钮";});
//    }

//    //按钮7：
//    MyPushButton *btn7 = new MyPushButton("://data/res/2_外阴影.png","://data/res/2_内阴影.png");
//    btn7->setParent(this);
//    btn7->move(185,5);
//    connect(btn7,&MyPushButton::clicked,[=](){qDebug()<<"点击了7号按钮";});

//    //登录按钮：
//    QPushButton *btn_Enter = new QPushButton(this);//登录按钮
//    btn_Enter->setGeometry(215,4,300,22);
//    btn_Enter->setText("**值班员A - 未登录");
//    btn_Enter->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                        "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
//                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");
//    connect(btn_Enter,&MyPushButton::clicked,[=](){
//        qDebug()<<"点击了btn_Enter号按钮";
//    });

//    //按钮8：
//    MyPushButton *btn8 = new MyPushButton("://data/res/火车_外阴影4.png");
//    btn8->setParent(this);
//    btn8->move(520,5);
//    connect(btn8,&MyPushButton::clicked,[=](){qDebug()<<"点击了8号按钮";});

//    //按钮9：
//    MyPushButton *btn9 = new MyPushButton("://data/res/火车_外阴影4.png");
//    btn9->setParent(this);
//    btn9->move(550,5);
//    connect(btn9,&MyPushButton::clicked,[=](){qDebug()<<"点击了9号按钮";});

//    //按钮10：
//    MyPushButton *btn10 = new MyPushButton("://data/res/火车_外阴影4.png");
//    btn10->setParent(this);
//    btn10->move(580,5);
//    connect(btn10,&MyPushButton::clicked,[=](){qDebug()<<"点击了10号按钮";});

//    //按钮11：
//    MyPushButton *btn11 = new MyPushButton("://data/res/设置_外阴影.png","://data/res/设置_内阴影.png");
//    btn11->setParent(this);
//    btn11->move(610,5);
//    connect(btn11,&MyPushButton::clicked,[=](){
//        qDebug()<<"点击了11号按钮";
//        QMenu *m_menu_Set=new QMenu(this);
//        QAction *m_ZXBST_Set=new QAction(m_menu_Set);//占线板视图设置
//        QAction *m_GDSTXS_Set=new QAction(m_menu_Set);//股道视图显示设置
//        QAction *m_FXXS_Set=new QAction(m_menu_Set);//方向显示设置
//        QAction *m_DCCS_Set=new QAction(m_menu_Set);//调车参数配置
//        QAction *m_DCYXJ_Set=new QAction(m_menu_Set);//调车优先级配置
//        QAction *m_DCGZ_Set=new QAction(m_menu_Set);//调车规则配置
//        QAction *m_GNAN_Set=new QAction(m_menu_Set);//功能按钮设置
//        QAction *m_ZCJMXS_Set=new QAction(m_menu_Set);//站场界面显示设置
//        QAction *m_BJTS_Set=new QAction(m_menu_Set);//报警提示配置
//        QAction *m_Version=new QAction(m_menu_Set);//版本信息

//        m_ZXBST_Set->setText(QObject::tr("占线板视图设置"));
//        m_GDSTXS_Set->setText(QObject::tr("股道视图显示设置"));
//        m_FXXS_Set->setText(QObject::tr("方向显示设置"));
//        m_DCCS_Set->setText(QObject::tr("调车参数配置"));
//        m_DCYXJ_Set->setText(QObject::tr("调车优先级配置"));
//        m_DCGZ_Set->setText(QObject::tr("调车规则配置"));
//        m_GNAN_Set->setText(QObject::tr("功能按钮设置"));
//        m_ZCJMXS_Set->setText(QObject::tr("站场界面显示设置"));
//        m_BJTS_Set->setText(QObject::tr("报警提示配置"));
//        m_Version->setText(QObject::tr("版本信息"));

//        m_menu_Set->addAction(m_ZXBST_Set);
//        m_menu_Set->addAction(m_GDSTXS_Set);
//        m_menu_Set->addAction(m_FXXS_Set);
//        m_menu_Set->addAction(m_DCCS_Set);
//        m_menu_Set->addAction(m_DCYXJ_Set);
//        m_menu_Set->addAction(m_DCGZ_Set);
//        m_menu_Set->addAction(m_GNAN_Set);
//        m_menu_Set->addAction(m_ZCJMXS_Set);
//        m_menu_Set->addAction(m_BJTS_Set);
//        m_menu_Set->addAction(m_Version);
////        connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
////        connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
//        btn11->setMenu(m_menu_Set);
//    });

//    //按钮12：
//    MyPushButton *btn12 = new MyPushButton("://data/res/关机_外阴影.png","://data/res/关机_内阴影.png");
//    btn12->setParent(this);
//    btn12->move(640,5);
//    connect(btn12,&MyPushButton::clicked,[=](){
//        KeyDialogbox("是否关闭占线板？");
//    });
//}

//按钮窗口
void MainWindow::SplitWindow()
{

    int ButtonLenth=0;
    int ButtonSide=0;
//    mainWindowForStationTrack->showMaximized();
    //mainWindowForStationTrack->show();
    this->setStyleSheet("QWidget#centralwidget{ background-color: rgb(150, 150, 150);}");
//    this->setStyleSheet("background-color: rgb(150, 150, 150);");





//    //按钮1：
//    ButtonSide=ButtonSide+4+ButtonLenth;
//    ButtonLenth=26;
//    QPushButton *btn1 = new QPushButton();
//    btn1->setParent(this);
//    btn1->setGeometry(ButtonSide,2,ButtonLenth,26);
//    QIcon icon1;
//    icon1.addFile("://data/res/8.png");
//    btn1->setIcon(icon1);
//    btn1->setIconSize(QSize(18,18));
//    btn1->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
//                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
//                        );
//    connect(btn1,&QPushButton::clicked,[=](){
//        qDebug()<<"点击了1号按钮";
//        //this->hide();//隐藏自身
//    });

    //按钮2：
    ButtonSide=ButtonSide+4+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn2 = new QPushButton();
    btn2->setParent(this);
    btn2->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon2;
    icon2.addFile("://data/res/T3.png");
    btn2->setIcon(icon2);
    btn2->setIconSize(QSize(25,25));
    btn2->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        "QPushButton::menu-indicator{image:none;width:0px;}");
    btn2->setMenu(m_menu_T3);

//    //按钮3：
//    ButtonSide=ButtonSide+4+ButtonLenth;
//    ButtonLenth=26;
//    QPushButton *btn3 = new QPushButton();
//    btn3->setParent(this);
//    btn3->setGeometry(ButtonSide,2,ButtonLenth,26);
//    QIcon icon3;
//    icon3.addFile("://data/res/7.png");
//    btn3->setIcon(icon3);
//    btn3->setIconSize(QSize(18,18));
//    btn3->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
//                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
//                        );
//    connect(btn3,&QPushButton::clicked,[=](){qDebug()<<"点击了3号按钮";});

    //按钮4：
//    ButtonSide=ButtonSide+6+ButtonLenth;
//    ButtonLenth=34;
//    QPushButton *btn4 = new QPushButton();
//    btn4->setParent(this);
//    btn4->setGeometry(ButtonSide,2,ButtonLenth,34);
//    QIcon icon4;
//    icon4.addFile("://data/res/火车.png");
//    btn4->setIcon(icon4);
//    btn4->setIconSize(QSize(25,25));
//    btn4->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
//                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
//                        );
//    connect(btn4,&QPushButton::clicked,[=](){
//        monitor->activateWindow();
//        monitor->setWindowState((monitor->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
//        monitor->show();
//    });

//    connect(btn4, &QPushButton::clicked, this, &MainWindow::clicked_btn4_openMointer);

    //按钮5：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn5 = new QPushButton();
    btn5->setParent(this);
    btn5->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon5;
    icon5.addFile("://data/res/1.png");
    btn5->setIcon(icon5);
    btn5->setIconSize(QSize(25,25));
    btn5->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        );
//    connect(btn5,&QPushButton::clicked,[=](){
//        mainWindowForStationTrack->activateWindow();
//        mainWindowForStationTrack->setWindowState((mainWindowForStationTrack->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
//        mainWindowForStationTrack->showMaximized();
//        mainWindowForStationTrack->show();
//    });
    connect(btn5, &QPushButton::clicked, this, &MainWindow::clicked_btn5_openmainWindowForStationTrack);

    //按钮6：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    if(commState==false)
    {
        QPushButton *btn6 = new QPushButton();
        btn6->setParent(this);
        btn6->setGeometry(ButtonSide,2,ButtonLenth,34);
        QIcon icon6;
        icon6.addFile("://data/res/断开.png");
        btn6->setIcon(icon6);
        btn6->setIconSize(QSize(25,25));
        btn6->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                            "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                            "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                            "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                            "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                            );
//        connect(btn6,&QPushButton::clicked,[=](){
//            Dialog_NetStatButton();
//        });
        connect(btn6, &QPushButton::clicked, this, &MainWindow::Dialog_NetStatButton);
    }
    else
    {
        QPushButton *btn6 = new QPushButton();
        btn6->setParent(this);
        btn6->setGeometry(ButtonSide,2,ButtonLenth,34);
        QIcon icon6;
        icon6.addFile("://data/res/连接.png");
        btn6->setIcon(icon6);
        btn6->setIconSize(QSize(25,25));
        btn6->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                            "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                            "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                            "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                            "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                            );
//        connect(btn6,&QPushButton::clicked,[=](){
//            Dialog_NetStatButton();
//        });
        connect(btn6, &QPushButton::clicked, this, &MainWindow::Dialog_NetStatButton);
    }

    //按钮7：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn7 = new QPushButton();
    btn7->setParent(this);
    btn7->move(185,5);
    btn7->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon7;
    icon7.addFile("://data/res/9.png");
    btn7->setIcon(icon7);
    btn7->setIconSize(QSize(25,25));
    btn7->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        );
//    connect(btn7,&QPushButton::clicked,[=](){
//        Dialog_BWL *dialog_bwl = new Dialog_BWL(this);
//        dialog_bwl->exec();
//    });
    connect(btn7, &QPushButton::clicked, this, &MainWindow::clicked_btn7_openBWL);

    //登录按钮：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=300;
    btn_Enter = new QPushButton(this);//登录按钮
    btn_Enter->setGeometry(ButtonSide,4,ButtonLenth,30);
    btn_Enter->setText("**值班员A - 未登录");
    btn_Enter->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                             "QPushButton{border:1px groove gray;border-radius:4px;padding:2px 4px;border-style:outset;}"
                             "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                             "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                             "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}");
//    connect(btn_Enter,&QPushButton::clicked,[=](){
//        Dialog_LoginButton();
//    });
    connect(btn_Enter, &QPushButton::clicked, this, &MainWindow::Dialog_LoginButton);

//    //按钮8：
//    ButtonSide=ButtonSide+6+ButtonLenth;
//    ButtonLenth=26;
//    QPushButton *btn8 = new QPushButton();
//    btn8->setParent(this);
//    btn8->setGeometry(ButtonSide,2,ButtonLenth,26);
//    QIcon icon8;
//    icon8.addFile("://data/res/5.png");
//    btn8->setIcon(icon8);
//    btn8->setIconSize(QSize(18,18));
//    btn8->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
//                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
//                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
//                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
//                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
//                        );
//    connect(btn8,&QPushButton::clicked,[=](){qDebug()<<"点击了8号按钮";});

    //按钮9：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
//    QPushButton *btn9 = new QPushButton();
    btn9 = new QPushButton();
    btn9->setParent(this);
    btn9->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon9;
    icon9.addFile("://data/res/签.png");
    btn9->setIcon(icon9);
    btn9->setIconSize(QSize(25,25));
    btn9->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                        "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                        "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                        "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                        "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        );
//    connect(btn9,&QPushButton::clicked,[=](){
//        Dialog_JSJDJH *dialog_jsjdjh = new Dialog_JSJDJH(this);
//        dialog_jsjdjh->exec();
//    });
    connect(btn9, &QPushButton::clicked, this, &MainWindow::clicked_btn9_openJSJDJH);

    //按钮10：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
//    QPushButton *btn10 = new QPushButton();
    btn10 = new QPushButton();
    btn10->setParent(this);
    btn10->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon10;
    icon10.addFile("://data/res/令.png");
    btn10->setIcon(icon10);
    btn10->setIconSize(QSize(25,25));
    btn10->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                         "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                         "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                         "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                         "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                        );
//    connect(btn10,&QPushButton::clicked,[=](){
//        Dialog_DDMLCL *dialog_ddmlcl = new Dialog_DDMLCL(this);
//        dialog_ddmlcl->exec();
//    });
    connect(btn10, &QPushButton::clicked, this, &MainWindow::clicked_btn10_openDDMLCL);

    //按钮11：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn11 = new QPushButton();
    btn11->setParent(this);
    btn11->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon11;
    icon11.addFile("://data/res/设置.png");
    btn11->setIcon(icon11);
    btn11->setIconSize(QSize(25,25));
    btn11->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                         "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                         "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                         "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                         "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                         "QPushButton::menu-indicator{image:none;width:0px;}");
    btn11->setMenu(m_menu_Set);


    //按钮12：
    ButtonSide=ButtonSide+6+ButtonLenth;
    ButtonLenth=34;
    QPushButton *btn12 = new QPushButton();
    btn12->setParent(this);
    btn12->setGeometry(ButtonSide,2,ButtonLenth,34);
    QIcon icon12;
    icon12.addFile("://data/res/关机.png");
    btn12->setIcon(icon12);
    btn12->setIconSize(QSize(25,25));
    btn12->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                         "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                         "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                         "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                         "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                         );
//    connect(btn12,&QPushButton::clicked,[=](){
//        KeyDialogbox("是否关闭占线板？");
//    });
    connect(btn12, &QPushButton::clicked, this, &MainWindow::clicked_btn12_KeyDialogbox);

//    ButtonSide=ButtonSide+6+ButtonLenth;
//    this->setGeometry(0,0,ButtonSide,30);
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)//移动工具条窗口//QT5实战项目视频
{
//    QMainWindow::mouseMoveEvent(event);
    QWidget::mouseMoveEvent(event);
    QPoint y=event->globalPos();//鼠标相对于桌面左上角的位置，鼠标的全局位置
    QPoint x=y-this->z;
    this->move(x);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
//    QMainWindow::mousePressEvent(event);
    QWidget::mousePressEvent(event);
    QPoint x=this->geometry().topLeft();//窗口左上角相对于桌面左上角的位置，窗口位置
    QPoint y=event->globalPos();//鼠标相对于桌面左上角的位置，鼠标的全局位置
    this->z=y-x;//定值//z是鼠标相对于窗口的位置
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
//    QMainWindow::mouseReleaseEvent(event);
    QWidget::mouseReleaseEvent(event);
    this->z=QPoint();
}
/***************************************************************/

/***************************************************************/
void MainWindow::KeyDialogbox(QString str)//弹出对话框
{
//    QTextToSpeech *tts = new QTextToSpeech(this);
//    tts->setLocale(QLocale::Chinese);//设置语言环境
//    tts->setRate(0.0);//设置语速-1.0到1.0
//    tts->setPitch(1.0);//设置音高-1.0到1.0
//    tts->setVolume(1.0);//设置音量0.0-1.0
//    if(tts->state()==QTextToSpeech::Ready)
//    {
//        tts->say(str);
//    }
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowTitle("提示：");
    msgBox->setText(str);
    msgBox->setStyleSheet("background-color: rgb(240,240,240);");
    QPushButton *btn_sure = msgBox->addButton("确定", QMessageBox::AcceptRole);
    QPushButton *bun_cancel = msgBox->addButton("取消", QMessageBox::RejectRole);
    btn_sure->setStyleSheet("background-color: rgb(220,220,220);");
    bun_cancel->setStyleSheet("background-color: rgb(220,220,220);");
//    connect(btn_sure,&MyPushButton::clicked,[=](){
//        mainWindowForStationTrack->close();
//        monitor->close();
//        this->close();
//    });
    connect(btn_sure, &QPushButton::clicked, this, &MainWindow::clicked_btn12_Close);
    msgBox->move(800,300);
    msgBox->exec();

}
//void MainWindow::KeyDialogbox(QString str)//弹出对话框
//{
//    QDialog dlg;
//    dlg.setGeometry(700,300,400,250);
//    QLabel dialoglable;
//    dialoglable.setParent(&dlg);
//    //dialoglable.setText("请先选择车辆对象，再进行参数设置!");
//    dialoglable.setText(str);
//    dialoglable.move(100,100);
//    dlg.exec();
//}
void MainWindow::AlarmSpeech(QString str)//语音告警
{
    QTextToSpeech *tts = new QTextToSpeech(this);
    tts->setLocale(QLocale::Chinese);//设置语言环境
    tts->setRate(0.0);//设置语速-1.0到1.0
    tts->setPitch(1.0);//设置音高-1.0到1.0
    tts->setVolume(1.0);//设置音量0.0-1.0
    if(tts->state()==QTextToSpeech::Ready)
    {
        tts->say("警告!"+str);
    }
}
/***************************************************************/

/***************************************************************/
void MainWindow::Menu_SetButton()
{
    m_menu_Set=new QMenu(this);
    m_menu_Set->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_ZXBST_Set=new QAction(m_menu_Set);//占线板视图设置
    m_GDSTXS_Set=new QAction(m_menu_Set);//股道视图显示设置
    m_FXXS_Set=new QAction(m_menu_Set);//方向显示设置
    m_DCCS_Set=new QAction(m_menu_Set);//调车参数配置
    m_DCYXJ_Set=new QAction(m_menu_Set);//调车优先级配置
    m_DCGZ_Set=new QAction(m_menu_Set);//调车规则配置
    m_GNAN_Set=new QAction(m_menu_Set);//功能按钮设置
    m_ZCJMXS_Set=new QAction(m_menu_Set);//站场界面显示设置
    m_BJTS_Set=new QAction(m_menu_Set);//报警提示配置
    m_Version=new QAction(m_menu_Set);//版本信息

    m_ZXBST_Set->setText(QObject::tr("占线板视图设置"));
    m_GDSTXS_Set->setText(QObject::tr("股道视图显示设置"));
    m_FXXS_Set->setText(QObject::tr("方向显示设置"));
    m_DCCS_Set->setText(QObject::tr("调车参数配置"));
    m_DCYXJ_Set->setText(QObject::tr("调车优先级配置"));
    m_DCGZ_Set->setText(QObject::tr("调车规则配置"));
    m_GNAN_Set->setText(QObject::tr("功能按钮设置"));
    m_ZCJMXS_Set->setText(QObject::tr("站场界面显示设置"));
    m_BJTS_Set->setText(QObject::tr("报警提示配置"));
    m_Version->setText(QObject::tr("版本信息"));

    m_menu_Set->addAction(m_ZXBST_Set);
    m_menu_Set->addAction(m_GDSTXS_Set);
    m_menu_Set->addAction(m_FXXS_Set);
    m_menu_Set->addAction(m_DCCS_Set);
    m_menu_Set->addAction(m_DCYXJ_Set);
    m_menu_Set->addAction(m_DCGZ_Set);
    m_menu_Set->addAction(m_GNAN_Set);
    m_menu_Set->addAction(m_ZCJMXS_Set);
    m_menu_Set->addAction(m_BJTS_Set);
    m_menu_Set->addAction(m_Version);
    connect(m_ZXBST_Set, &QAction::triggered, this, &MainWindow::ZXBViewSetFun);
    connect(m_GDSTXS_Set, &QAction::triggered, this, &MainWindow::GDSTViewSetFun);
    connect(m_BJTS_Set, &QAction::triggered, this, &MainWindow::BJTSSetFun);

    QMenu *m_menu_FX;//方向下拉菜单
    QAction *m_addAction_Station1;
    QAction *m_addAction_Station2;
    m_menu_FX = new QMenu(this);
    m_menu_FX->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_Station1 = new QAction(m_menu_FX);
    m_addAction_Station2 = new QAction(m_menu_FX);
    m_addAction_Station1->setText(QObject::tr("111"));
    m_addAction_Station2->setText(QObject::tr("222"));
    m_menu_FX->addAction(m_addAction_Station1);
    m_menu_FX->addAction(m_addAction_Station2);
    connect(m_addAction_Station1, &QAction::triggered, this, &MainWindow::StationDirectionFun);
    connect(m_addAction_Station2, &QAction::triggered, this, &MainWindow::StationDirectionFun);
    m_FXXS_Set->setMenu(m_menu_FX);

    QMenu *m_menu_DCCSPZ;//调车参数配置下拉菜单
    QAction *m_addAction_1;
    QAction *m_addAction_2;
    m_menu_DCCSPZ = new QMenu(this);
    m_menu_DCCSPZ->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_1 = new QAction(m_menu_DCCSPZ);
    m_addAction_2 = new QAction(m_menu_DCCSPZ);
    m_addAction_1->setText(QObject::tr("1"));
    m_addAction_2->setText(QObject::tr("2"));
    m_menu_DCCSPZ->addAction(m_addAction_1);
    m_menu_DCCSPZ->addAction(m_addAction_2);
//    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    m_DCCS_Set->setMenu(m_menu_DCCSPZ);

    QMenu *m_menu_DCYXJPZ;//调车优先级配置下拉菜单
    QAction *m_addAction_11;
    QAction *m_addAction_22;
    m_menu_DCYXJPZ = new QMenu(this);
    m_menu_DCYXJPZ->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_11 = new QAction(m_menu_DCYXJPZ);
    m_addAction_22 = new QAction(m_menu_DCYXJPZ);
    m_addAction_11->setText(QObject::tr("11"));
    m_addAction_22->setText(QObject::tr("22"));
    m_menu_DCYXJPZ->addAction(m_addAction_11);
    m_menu_DCYXJPZ->addAction(m_addAction_22);
//    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    m_DCYXJ_Set->setMenu(m_menu_DCYXJPZ);

    QMenu *m_menu_DCGZPZ;//调车规则配置下拉菜单
    QAction *m_addAction_111;
    QAction *m_addAction_222;
    m_menu_DCGZPZ = new QMenu(this);
    m_menu_DCGZPZ->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_111 = new QAction(m_menu_DCGZPZ);
    m_addAction_222 = new QAction(m_menu_DCGZPZ);
    m_addAction_111->setText(QObject::tr("111"));
    m_addAction_222->setText(QObject::tr("222"));
    m_menu_DCGZPZ->addAction(m_addAction_111);
    m_menu_DCGZPZ->addAction(m_addAction_222);
//    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    m_DCGZ_Set->setMenu(m_menu_DCGZPZ);

    QMenu *m_menu_GNAN;//功能按钮设置下拉菜单
    QAction *m_addAction_FunSetStation1;
    QAction *m_addAction_FunSetStation2;
    m_menu_GNAN = new QMenu(this);
    m_menu_GNAN->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_FunSetStation1 = new QAction(m_menu_GNAN);
    m_addAction_FunSetStation2 = new QAction(m_menu_GNAN);
    m_addAction_FunSetStation1->setText(QObject::tr("1111"));
    m_addAction_FunSetStation2->setText(QObject::tr("2222"));
    m_menu_GNAN->addAction(m_addAction_FunSetStation1);
    m_menu_GNAN->addAction(m_addAction_FunSetStation2);
    connect(m_addAction_FunSetStation1, &QAction::triggered, this, &MainWindow::FunctionPushButtonFun);
    connect(m_addAction_FunSetStation2, &QAction::triggered, this, &MainWindow::FunctionPushButtonFun);
    m_GNAN_Set->setMenu(m_menu_GNAN);
}
/***************************************************************/


/***************************************************************/
void MainWindow::Menu_T3Button()
{
    m_menu_T3=new QMenu(this);//T3按钮下拉菜单
    m_menu_T3->setStyleSheet("background-color: rgb(204, 228, 247);");

    m_GDBState_Set=new QAction(m_menu_T3);//设置供电臂状态
    m_FSSBCZ=new QAction(m_menu_T3);//封锁设备操作
    m_QYPLSBCZ=new QAction(m_menu_T3);//区域批量设备操作
    m_ZCCGL=new QAction(m_menu_T3);//站存车管理
    m_SBYXFX=new QAction(m_menu_T3);//设备影响分析
    m_CZRZCX=new QAction(m_menu_T3);//操作日志查询
    m_FLSZCX=new QAction(m_menu_T3);//防溜设置查询
    m_XXTS=new QAction(m_menu_T3);//信息提示

    m_GDBState_Set->setText(QObject::tr("设置供电臂状态"));
    m_FSSBCZ->setText(QObject::tr("封锁设备操作"));
    m_QYPLSBCZ->setText(QObject::tr("区域批量设备操作"));
    m_ZCCGL->setText(QObject::tr("站存车管理"));
    m_SBYXFX->setText(QObject::tr("设备影响分析"));
    m_CZRZCX->setText(QObject::tr("操作日志查询"));
    m_FLSZCX->setText(QObject::tr("防溜设置查询"));
    m_XXTS->setText(QObject::tr("信息提示"));


    m_menu_T3->addAction(m_GDBState_Set);
    m_menu_T3->addAction(m_FSSBCZ);
    m_menu_T3->addAction(m_QYPLSBCZ);
    m_menu_T3->addAction(m_ZCCGL);
    m_menu_T3->addAction(m_SBYXFX);
    m_menu_T3->addAction(m_CZRZCX);
    m_menu_T3->addAction(m_FLSZCX);
    m_menu_T3->addAction(m_XXTS);

    connect(m_GDBState_Set, &QAction::triggered, this, &MainWindow::GDBState_SetFun);
    connect(m_FSSBCZ, &QAction::triggered, this, &MainWindow::FSSBCZFun);
    connect(m_QYPLSBCZ, &QAction::triggered, this, &MainWindow::QYPLSBOper_Fun);

//    QMenu *m_menu_QYPLSBCZ;//区域批量设备操作下拉菜单
//    QAction *m_addAction_3;
//    QAction *m_addAction_4;
//    m_menu_QYPLSBCZ = new QMenu(this);
//    m_menu_QYPLSBCZ->setStyleSheet("background-color: rgb(204, 228, 247);");
//    m_addAction_3 = new QAction(m_menu_QYPLSBCZ);
//    m_addAction_4 = new QAction(m_menu_QYPLSBCZ);
//    m_addAction_3->setText(QObject::tr("3"));
//    m_addAction_4->setText(QObject::tr("4"));
//    m_menu_QYPLSBCZ->addAction(m_addAction_3);
//    m_menu_QYPLSBCZ->addAction(m_addAction_4);
////    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
////    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
//    m_QYPLSBCZ->setMenu(m_menu_QYPLSBCZ);

    QMenu *m_menu_ZCCGL;//站存车管理下拉菜单
    QAction *m_addAction_33;
    QAction *m_addAction_44;
    m_menu_ZCCGL = new QMenu(this);
    m_menu_ZCCGL->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_33 = new QAction(m_menu_ZCCGL);
    m_addAction_44 = new QAction(m_menu_ZCCGL);
    m_addAction_33->setText(QObject::tr("33"));
    m_addAction_44->setText(QObject::tr("44"));
    m_menu_ZCCGL->addAction(m_addAction_33);
    m_menu_ZCCGL->addAction(m_addAction_44);
//    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    m_ZCCGL->setMenu(m_menu_ZCCGL);

    QMenu *m_menu_SBYXFX;//设备影响分析下拉菜单
    QAction *m_addAction_333;
    QAction *m_addAction_444;
    m_menu_SBYXFX = new QMenu(this);
    m_menu_SBYXFX->setStyleSheet("background-color: rgb(204, 228, 247);");
    m_addAction_333 = new QAction(m_menu_SBYXFX);
    m_addAction_444 = new QAction(m_menu_SBYXFX);
    m_addAction_333->setText(QObject::tr("333"));
    m_addAction_444->setText(QObject::tr("444"));
    m_menu_SBYXFX->addAction(m_addAction_333);
    m_menu_SBYXFX->addAction(m_addAction_444);
//    connect(m_addAction, &QAction::triggered, this, &ButtonMenu::addFunc);
//    connect(m_delAction, &QAction::triggered, this, &ButtonMenu::delFunc);
    m_SBYXFX->setMenu(m_menu_SBYXFX);
}
/***************************************************************/

/***************************************************************/
void MainWindow::Dialog_NetStatButton()
{
    QDialog *dlg=new QDialog(this);
    dlg->setGeometry(700,300,400,250);
    //dlg->setStyleSheet("background-color: rgb(220, 220, 220);");
    dlg->setMinimumSize(QSize(400,250));
    dlg->setMaximumSize(QSize(400,250));
    dlg->move(760,250);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    QTableWidget *tableWidget = new QTableWidget(dlg);
    tableWidget->setGeometry(5,10,389,200);
    tableWidget->setColumnCount(3);
    tableWidget->setRowCount(6);
    tableWidget->setStyleSheet("background-color: rgb(220, 220, 220);");
    tableWidget->setFont(QFont("宋体",9,50));
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: rgb(200,200,200);}");//设置表头颜色
    tableWidget->setAlternatingRowColors(true);//设置相邻行颜色交替显示
    tableWidget->verticalHeader()->setVisible(false);//垂直表头不显示
    tableWidget->verticalHeader()->setDefaultSectionSize(10);//设置行高
    tableWidget->horizontalHeader()->setDefaultSectionSize(129);//设置列宽

    QStringList m_List_State;
    m_List_State << "名称";
    m_List_State << "状态";
    m_List_State << "状态描述";
    tableWidget->setHorizontalHeaderLabels(m_List_State);
    QStringList m_List_Item;
    m_List_Item << "Commtask0";
    m_List_Item << "Commtask1";
    m_List_Item << "**站变量服务";
    m_List_Item << "**站自律机";
    m_List_Item << "**站RT";
    m_List_Item << "中心shuntsvr";
    for (int i=0;i<6;i++)
    {
        int col=0;
        tableWidget->setItem(i,col++,new QTableWidgetItem(m_List_Item[i]));//记住使用中括号
    }


    QPushButton *colseButton = new QPushButton(dlg);
    colseButton->setGeometry(250,215,50,25);
    colseButton->setText("关闭");
    colseButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                               "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                               "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                               "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                               "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                               );
//    connect(colseButton,&QPushButton::clicked,[=](){
//        dlg->close();
//    });
    connect(colseButton, SIGNAL(clicked()), dlg, SLOT(close()));

    QPushButton *refreshButton = new QPushButton(dlg);
    refreshButton->setGeometry(100,215,50,25);
    refreshButton->setText("刷新");
    refreshButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                 "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                 "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                 "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                 "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                 );
    dlg->exec();
}
/***************************************************************/

/***************************************************************/
void MainWindow::Dialog_LoginButton()
{
//    LoginState=JY_FY_zuoyeWindow.LoginState;
    QDialog *dlg=new QDialog(this);
    dlg->setGeometry(850,580,250,150);
    dlg->setMinimumSize(QSize(250,150));
    dlg->setMaximumSize(QSize(250,150));
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->move(800,300);

    QLabel *label_ID=new QLabel(dlg);
    QLabel *label_Name=new QLabel(dlg);
    label_ID->setGeometry(20,30,50,25);
    label_ID->setText("用户 ID");
    label_Name->setGeometry(20,65,50,25);
    label_Name->setText("用户名称");

    comboBox_ID=new QComboBox(dlg);
    comboBox_ID->setGeometry(80,30,160,25);
    comboBox_ID->setStyleSheet("background-color: rgb(240, 240, 240);");
    comboBox_ID->clear();
    comboBox_ID->setEditable(true);
    QStringList strList_ID;
    strList_ID<<"1";
    comboBox_ID->addItems(strList_ID);

    comboBox_Name=new QComboBox(dlg);
    comboBox_Name->setGeometry(80,65,160,25);
    comboBox_Name->setStyleSheet("background-color: rgb(240, 240, 240);");
    comboBox_Name->clear();
    comboBox_Name->setEditable(true);
    QStringList strList_Name;
    strList_Name<<"太阳研究所";
    comboBox_Name->addItems(strList_Name);


    QPushButton *LoginButton = new QPushButton(dlg);
    LoginButton->setGeometry(75,110,50,25);
    LoginButton->setText("登录");
    LoginButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                               "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                               "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                               "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                               "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                               );
//    connect(LoginButton,&QPushButton::clicked,[=](){
//        LoginID=QString(comboBox_ID->currentText());
//        LoginName=QString(comboBox_Name->currentText());
//        if((LoginName=="")&&(LoginID==""))
//        {
//            btn_Enter->setText("**值班员A - 未登录");
//        }
//        else
//        {
//            btn_Enter->setText("**值班员CTC3.0终端 - <"+LoginName+">");
//        }
//        dlg->close();
//    });

    connect(LoginButton, &QPushButton::clicked, this, &MainWindow::clicked_LoginButton);
    connect(LoginButton, SIGNAL(clicked()), dlg, SLOT(close()));

    QPushButton *LogoutButton = new QPushButton(dlg);
    LogoutButton->setGeometry(130,110,50,25);
    LogoutButton->setText("注销");
    LogoutButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                                 "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                                 "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                                 "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                                 "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                                 );
//    connect(LogoutButton,&QPushButton::clicked,[=](){
//        LoginID="";
//        LoginName="";
//        if((LoginName=="")&&(LoginID==""))
//        {
//            btn_Enter->setText("**值班员A - 未登录");
//        }
//        else
//        {
//            btn_Enter->setText("**值班员CTC3.0终端 - <"+LoginName+">");
//        }
//        dlg->close();
//    });
    connect(LogoutButton, &QPushButton::clicked, this, &MainWindow::clicked_LogoutButton);
    connect(LogoutButton, SIGNAL(clicked()), dlg, SLOT(close()));

    QPushButton *colseButton = new QPushButton(dlg);
    colseButton->setGeometry(185,110,50,25);
    colseButton->setText("关闭");
    colseButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}"
                               "QPushButton{border:1px groove gray;border-radius:4px;padding:1px 2px;border-style:outset;}"
                               "QPushButton:hover{background-color: rgb(229, 241, 251);color:black}"
                               "QPushButton:pressed{background-color: rgb(204, 228, 247);border-style:inset}"
                               "QPushButton{background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(220, 220, 220, 255), stop:1 rgba(250, 250, 250, 255));}"
                               );
//    connect(colseButton,&QPushButton::clicked,[=](){
//        dlg->close();
//    });
    connect(colseButton, SIGNAL(clicked()), dlg, SLOT(close()));

    dlg->exec();
}
/***************************************************************/

/***************************************************************/
//占线板视图设置功能：
void MainWindow::ZXBViewSetFun()
{
    ZXBViewSet *zxbViewSet = new ZXBViewSet(this);
    zxbViewSet->setWindowModality(Qt::WindowModal);//设置模态
    zxbViewSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    zxbViewSet->show();
}
/***************************************************************/

/***************************************************************/
//股道视图显示设置功能：
void MainWindow::GDSTViewSetFun()
{
    GDSTViewSet *gdstViewSet = new GDSTViewSet(this);
    gdstViewSet->setWindowModality(Qt::WindowModal);//设置模态
    gdstViewSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    gdstViewSet->show();
}
/***************************************************************/

/***************************************************************/
//方向显示设置：
void MainWindow::StationDirectionFun()
{
    StationDirection *stationDirection = new StationDirection(this);
    stationDirection->setWindowModality(Qt::WindowModal);//设置模态
    stationDirection->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    stationDirection->show();
}
/***************************************************************/

/***************************************************************/
//功能按钮设置：
void MainWindow::FunctionPushButtonFun()
{
    FunctionPushButtonSet *functionPushButtonSet = new FunctionPushButtonSet(this);
    functionPushButtonSet->setWindowModality(Qt::WindowModal);//设置模态
    functionPushButtonSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    functionPushButtonSet->show();
}
/***************************************************************/

/***************************************************************/
//报警提示配置：
void MainWindow::BJTSSetFun()
{
    BJTSSet *bjtsSet = new BJTSSet(this);
    bjtsSet->setWindowModality(Qt::WindowModal);//设置模态
    bjtsSet->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    bjtsSet->show();
}
/***************************************************************/

/***************************************************************/
//void MainWindow::trigerMenu(QAction *pAction)
//{

//    if (pAction->text().contains("封锁设备"))
//    {
//        Dialog_Blockoperation *pBlock = new Dialog_Blockoperation;
//        pBlock->UpdateData();
////        pBlock->show();
//        pBlock->exec();
//    }
//    else if (pAction->text().contains("供电臂"))
//    {
//        Dialog_PowerSupply *pPowerSupply = new Dialog_PowerSupply;
////        pPowerSupply->show();
//        pPowerSupply->exec();
//    }
//    else if (pAction->text().contains("批量"))
//    {
////        stationDevOper->show();
////        stationDevOper->exec();
//    }
//}
/***************************************************************/
void MainWindow::GDBState_SetFun()
{
    Dialog_PowerSupply *pPowerSupply = new Dialog_PowerSupply(this);
    pPowerSupply->exec();
}
/***************************************************************/

/***************************************************************/
void MainWindow::FSSBCZFun()
{
    Dialog_Blockoperation *pBlock = new Dialog_Blockoperation(this);
    pBlock->UpdateData();
    pBlock->exec();
}
/***************************************************************/

/***************************************************************/
void MainWindow::QYPLSBOper_Fun()
{
    stationshowfrom *stationDevOper;
    stationDevOper = new stationshowfrom(this);
    stationDevOper->udpSocket=udpSocket;
    stationDevOper->IP_OppAdress=IP_OppAdress;
    stationDevOper->IP_OppPort=IP_OppPort;
//    connect(stationDevOper,SIGNAL(Signal_PLCZ_SendData()),this,SLOT(PLCZ_SendDate()));
//    connect(stationDevOper,SIGNAL(Signal_PLCZ_SendData_JF()),this,SLOT(PLCZ_SendDate_JF()));
    stationDevOper->setWindowModality(Qt::WindowModal);//设置模态
    stationDevOper->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    stationDevOper->show();
}

void MainWindow::QYPLSBOper_Fun_ZDLC()//重点列车道岔单锁
{
//    CStation *pStation = monitor->zcArray[0];
//    CGDDC *pDC;
//    QStringList XHList,DCList,QJList,QDList,XHdev_code,DCdev_code,QJdev_code,QDdev_code;
//    for(int i=0;i<pStation->GDDCArray.count();i++)
//    {
//        pDC=pStation->GDDCArray[i];
//        if(pDC->gdColor==WHITE)
//        {
//            DCList.push_back(pDC->m_strName);
//            DCdev_code.push_back(QString::number(pDC->m_nCode));
//        }
//    }
//    emit monitor->SendSelectDevsignal(XHList,DCList,QJList,QDList,XHdev_code,DCdev_code,QJdev_code,QDdev_code);

    stationshowfrom *stationDevOper;
    stationDevOper = new stationshowfrom(this);
    stationDevOper->udpSocket=udpSocket;
    stationDevOper->IP_OppAdress=IP_OppAdress;
    stationDevOper->IP_OppPort=IP_OppPort;
//    connect(stationDevOper,SIGNAL(Signal_PLCZ_SendData()),this,SLOT(PLCZ_SendDate()));
//    connect(stationDevOper,SIGNAL(Signal_PLCZ_SendData_JF()),this,SLOT(PLCZ_SendDate_JF()));
    stationDevOper->setWindowModality(Qt::WindowModal);//设置模态
    stationDevOper->setAttribute(Qt::WA_DeleteOnClose);//关闭后自动释放窗口
    stationDevOper->show();
}
/***************************************************************/

/***************************************************************/
int MainWindow::StringSplit(QString source, QString ch,QStringList &strarr)
{
    QString TmpStr;
    QString TmpStr_1;
    strarr.clear();
    if(source.isEmpty() || ch.isEmpty())
        return 0;
    int len=ch.length();
    int findi=0;
    int findj=0;
//    int findk=0;
    bool findn=0;
    int sum=1;

    findn=source.contains(ch);
    if (findn==true)
    {
        for (int i=0;i<source.length();i++)
        {
            if(source.left(i).right(len)==ch)
            {
                findj=i;
                TmpStr=source.mid(findi,findj-findi-len);
                strarr.append(TmpStr);
                findi=findj;
                sum++;
            }
        }
        TmpStr=source.mid(findi,source.length());
        TmpStr=TmpStr.replace("\r\n","");
        strarr.append(TmpStr);
        return sum;
    }
    else
    {
        strarr.append(source);
        return sum;
    }
}

/***************************************************************/

/***************************************************************/
void MainWindow::PLCZ_SendDate()
{
    CStation *pStation = monitor->zcArray[0];
    CGD *pGD;
    CGDDC *pDC;
    CXHD *pXHD;
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
    for(int i=0;i<pStation->GDArray.count();i++)
    {
        pGD=pStation->GDArray[i];
        if(pGD->GDFSState==true)
        {
            FUNType_GDBSet=0x10;
            FUNSet_DeviceNum=pGD->m_nCode;
            SendToTrainSimu_FUNSet();
        }
    }
    for(int i=0;i<pStation->GDDCArray.count();i++)
    {
        pDC=pStation->GDDCArray[i];
        if(pDC->DCFSState==true)
        {
            FUNType_GDBSet=0x10;
            FUNSet_DeviceNum=pDC->m_nCode;
            SendToTrainSimu_FUNSet();
        }
    }
    for(int i=0;i<pStation->XHDArray.count();i++)
    {
        pXHD=pStation->XHDArray[i];
        if(pXHD->XHFSState==true)
        {
            FUNType_GDBSet=0x10;
            FUNSet_DeviceNum=pXHD->m_nCode;
            SendToTrainSimu_FUNSet();
        }
    }
}
void MainWindow::PLCZ_SendDate_JF()
{
    CStation *pStation = monitor->zcArray[0];
    CGD *pGD;
    CGDDC *pDC;
    CXHD *pXHD;
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_dcDev.size();i++)
//    {
//        if((stationDevOper->CurrectZone->vect_dcDev[i].OperStr==QString("解锁"))&&(stationDevOper->CurrectZone->vect_dcDev[i].StateStr == QString("解锁")))
//        {
//            for(int j=0;j<pStation->GDDCArray.count();j++)
//            {
//                pDC=pStation->GDDCArray[j];
//                if(stationDevOper->CurrectZone->vect_dcDev[i].Devname==pDC->m_strName)
//                {
//                    pDC->DCFSState=false;
//                }
//            }
//        }
//    }
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_qdDev.size();i++)
//    {
//        if((stationDevOper->CurrectZone->vect_qdDev[i].OperStr==QString("解封"))&&(stationDevOper->CurrectZone->vect_qdDev[i].StateStr == QString("解封")))
//        {
//            for(int j=0;j<pStation->GDArray.count();j++)
//            {
//                pGD=pStation->GDArray[j];
//                if(stationDevOper->CurrectZone->vect_qdDev[i].Devname==pGD->m_strName)
//                {
//                    pGD->GDFSState=false;
//                }
//            }
//        }
//    }
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_qjDev.size();i++)
//    {
//        if((stationDevOper->CurrectZone->vect_qjDev[i].OperStr==QString("解封"))&&(stationDevOper->CurrectZone->vect_qjDev[i].StateStr == QString("解封")))
//        {
//            for(int j=0;j<pStation->GDArray.count();j++)
//            {
//                pGD=pStation->GDArray[j];
//                if(stationDevOper->CurrectZone->vect_qjDev[i].Devname==pGD->m_strName)
//                {
//                    pGD->GDFSState=false;
//                }
//            }
//        }
//    }
//    for(unsigned int i = 0 ;i<stationDevOper->CurrectZone->vect_xhDev.size();i++)
//    {
//        if(((stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("解封(列)解封(调)"))||(stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("解封(调)")))
//            &&((stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("解封(列)解封(调)"))||(stationDevOper->CurrectZone->vect_xhDev[i].OperStr == QString("解封(调)"))))
//        {
//            for(int j=0;j<pStation->XHDArray.count();j++)
//            {
//                pXHD=pStation->XHDArray[j];
//                if(stationDevOper->CurrectZone->vect_xhDev[i].Devname==pXHD->m_strName)
//                {
//                    pXHD->XHFSState=false;
//                }
//            }
//        }
//    }
    for(int i=0;i<pStation->GDArray.count();i++)
    {
        pGD=pStation->GDArray[i];
        if(pGD->GDFSState==false)
        {
            FUNType_GDBSet=0x11;
            FUNSet_DeviceNum=pGD->m_nCode;
            SendToTrainSimu_FUNSet();
        }
    }
    for(int i=0;i<pStation->GDDCArray.count();i++)
    {
        pDC=pStation->GDDCArray[i];
        if(pDC->DCFSState==false)
        {
            FUNType_GDBSet=0x11;
            FUNSet_DeviceNum=pDC->m_nCode;
            SendToTrainSimu_FUNSet();
        }
    }
    for(int i=0;i<pStation->XHDArray.count();i++)
    {
        pXHD=pStation->XHDArray[i];
        if(pXHD->XHFSState==false)
        {
            FUNType_GDBSet=0x11;
            FUNSet_DeviceNum=pXHD->m_nCode;
            SendToTrainSimu_FUNSet();
        }
    }
}
/***************************************************************/
void MainWindow::clicked_btn4_openMointer()
{
    monitor->activateWindow();
    monitor->setWindowState((monitor->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    monitor->show();
}
/***************************************************************/
void MainWindow::clicked_btn5_openmainWindowForStationTrack()
{
//    mainWindowForStationTrack->activateWindow();
//    mainWindowForStationTrack->setWindowState((mainWindowForStationTrack->windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
////    mainWindowForStationTrack->showMaximized();
//    mainWindowForStationTrack->show();
}
/***************************************************************/
void MainWindow::clicked_btn7_openBWL()
{
    Dialog_BWL *dialog_bwl = new Dialog_BWL(this);
    dialog_bwl->exec();
}
/***************************************************************/
void MainWindow::clicked_btn9_openJSJDJH()
{
    Dialog_JSJDJH *dialog_jsjdjh = new Dialog_JSJDJH(this);
    dialog_jsjdjh->exec();
}
/***************************************************************/
void MainWindow::clicked_btn10_openDDMLCL()
{
    Dialog_DDMLCL *dialog_ddmlcl = new Dialog_DDMLCL(this);
    dialog_ddmlcl->exec();
}
/***************************************************************/
void MainWindow::clicked_btn12_Close()
{
    mainWindowForStationTrack->close();
    monitor->close();
    this->close();
}
/***************************************************************/
void MainWindow::clicked_btn12_KeyDialogbox()
{
    KeyDialogbox("是否关闭占线板？");
}
/***************************************************************/
void MainWindow::clicked_LoginButton()
{
    LoginID=QString(comboBox_ID->currentText());
    LoginName=QString(comboBox_Name->currentText());
    if((LoginName=="")&&(LoginID==""))
    {
        btn_Enter->setText("**值班员A - 未登录");
    }
    else
    {
        btn_Enter->setText("**值班员CTC3.0终端 - <"+LoginName+">");
    }
}
/***************************************************************/
void MainWindow::clicked_LogoutButton()
{
    LoginID="";
    LoginName="";
    if((LoginName=="")&&(LoginID==""))
    {
        btn_Enter->setText("**值班员A - 未登录");
    }
    else
    {
        btn_Enter->setText("**值班员CTC3.0终端 - <"+LoginName+">");
    }
}
/***************************************************************/

/***************************************************************/

/***************************************************************/

/***************************************************************/

/***************************************************************/

