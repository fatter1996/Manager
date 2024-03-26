#include "stationmanager.h"
#include "ui_stationmanager.h"
#include<QDesktopWidget>
#include "VoiceCall/Universal.h"
#include "VoiceCall/CallButton.h"
bool CStationManager::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->MenuBtn)//监控的对象
    {
        if (event->type() == QEvent::MouseMove)
        {
            ui->MenuBtn->show();
        }
    }else
        ui->MenuBtn->hide();
    return QWidget::eventFilter(obj,event);
}
int CStationManager::test_taskBar()
{
    int TaskHeight = 0;
    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect appRect = desktopWidget->availableGeometry();
    QRect fullScreenRect = desktopWidget->screenGeometry();
    TaskHeight = fullScreenRect.height()- appRect.height();
    if (appRect.bottom() != fullScreenRect.bottom())
    {
        qDebug() << u8"任务栏在底部"<<"高度:"<<TaskHeight;
    }
    else if (appRect.top() != fullScreenRect.top())
    {
        qDebug() << u8"任务栏在顶部"<<"高度:"<<TaskHeight;
    }
    else if (appRect.left() != fullScreenRect.left())
    {
        qDebug() << u8"任务栏在左边"<<"高度:"<<TaskHeight;
    }
    else if (appRect.right() != fullScreenRect.right())
    {
        qDebug() << u8"任务栏在右边"<<"高度:"<<TaskHeight;
    }
    else    //任务栏处于自动隐藏状态
    {
        qDebug() << u8"任务栏处于自动隐藏状态";
    }
    return TaskHeight;
}

void CStationManager::StationRest()
{

}

void CStationManager::All_StationRest()
{

}
#include<qdesktopwidget.h>
CStationManager::CStationManager(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CStationManager)
{
    ui->setupUi(this);
    test_taskBar();
    QRect trc = QApplication::desktop()->availableGeometry();
    qDebug()<<"尺寸:"<<trc;

    Height = test_taskBar();
    if(Height!=0)
        Height =trc.height()-110;
    else
        Height =1080-110;
    ui->page->setMaximumHeight(Height);
    ui->page->setMinimumHeight(Height);
    ui->scrollArea->setMaximumHeight(Height);
    ui->scrollArea->setMinimumHeight(Height);
    LastHeight = ui->scrollArea->height();

    ui->scrollAreaWidgetContents->setMinimumWidth(DBSERVICES.LoginSQLData.info.ScrollSize_Width);
    ui->scrollAreaWidgetContents->setMaximumWidth(DBSERVICES.LoginSQLData.info.ScrollSize_Width);

    ui->scrollAreaWidgetContents->setMinimumHeight(DBSERVICES.LoginSQLData.info.ScrollSize_Height);
    ui->scrollAreaWidgetContents->setMaximumHeight(DBSERVICES.LoginSQLData.info.ScrollSize_Height);

    qDebug()<<"滚动长度:"<<ui->scrollAreaWidgetContents->width();
    qDebug()<<"滚动高度:"<<ui->scrollAreaWidgetContents->height();
    ui->MenuBtn->hide();
    DBSERVICES.SetBackImage(this,"背景二");
    pCloseButton = new QPushButton(this);
    StationRestBtn = new QPushButton(this);
    All_StationRest_Btn = new QPushButton(this);
    StationRestBtn->setToolTip("站场重置");
    All_StationRest_Btn->setToolTip("全场重置");
    pCloseButton->setToolTip("关闭系统");
    m_Data = new CData(this);
    pJM = new CTh_kb();
    pJM->initData(this);
    ui->scrollAreaWidgetContents->pJM = pJM;
    ui->scrollAreaWidgetContents->InitData();
    m_Data->pJM = pJM;
    //m_Data->ReadStationData(0);
    ui->scrollAreaWidgetContents->resize(2500, 1800);
    //ui->scrollAreaWidgetContents->SetCurrentStation(m_Data->AllStation[0].pStationData);

    pMonitorDlg = new CTrainMonitorDlg(this, m_Data);
    pMonitorDlg->hide();
    //界面绘制刷新显示定时器
    nElapsed = 0;  //定时器计数器
    pShowT = new QTimer();
    pShowT->setInterval(100);
    connect(pShowT, SIGNAL(timeout()), this, SLOT(slotOntimeout()));
    pShowT->start();
    SetTreeData();

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    TreeModel = new QStandardItemModel(1, 1);
    connect(ui->treeView, SIGNAL(clicked(const QModelIndex)), m_Data, SLOT(TreeViewClick(const QModelIndex)));
    connect(pCloseButton, SIGNAL(clicked()), this, SLOT(closeWindow()));
    connect(StationRestBtn,SIGNAL(clicked()),this,SLOT(StationRest()));
    connect(All_StationRest_Btn,SIGNAL(clicked()),this,SLOT(All_StationRest()));
    pCloseButton->setStyleSheet("QPushButton{width:35;height:35;}"
                                "QPushButton{border-image: url(:/Pictures/退出.png);}"
                                "QPushButton{color: rgb(255, 255, 255);}"
                                "QPushButton{font: 16pt bold;}"
                                );
    StationRestBtn->setStyleSheet("QPushButton{width:35;height:35;}"
                                "QPushButton{border-image: url(:/Pictures/站场重置.png);}"
                                "QPushButton{color: rgb(255, 255, 255);}"
                                "QPushButton{font: 16pt bold;}"
                                );
    All_StationRest_Btn->setStyleSheet("QPushButton{width:35;height:35;}"
                                "QPushButton{border-image: url(:/Pictures/全场重置.png);}"
                                "QPushButton{color: rgb(255, 255, 255);}"
                                "QPushButton{font: 16pt bold;}"
                                );
    pCloseButton->setCursor(Qt::OpenHandCursor);
    StationRestBtn->setCursor(Qt::OpenHandCursor);
    All_StationRest_Btn->setCursor(Qt::OpenHandCursor);
    connect(&TX_Send_SERVICES,SIGNAL(ChangeControl_Cmd(ControlInfo&)),this,
            SLOT(UpdateControl_Info(ControlInfo&)));
    InitButtonLayout();
    InitVoiceCall();
    InitMenu();
}

CStationManager::~CStationManager()
{
    if(self_Startdlg!=nullptr)
    {
        delete self_Startdlg;
        self_Startdlg = nullptr;
    }
    if(controldlg!=nullptr)
    {
        delete controldlg;
        controldlg = nullptr;
    }
    delete ui;
}

void CStationManager::SetTreeData()
{
    QStandardItem* item_1 = new QStandardItem("车站");
    QStandardItem* item = nullptr;

    QStandardItem* btnItem1 = nullptr;
    QStandardItem* btnItem2 = nullptr;

    TreeModel = new QStandardItemModel(1, 3);
    TreeModel->setItem(0, 0, item_1);

    ui->treeView->setHeaderHidden(true);
    ui->treeView->setModel(TreeModel);
    ui->treeView->setColumnWidth(1, 24);
    ui->treeView->setColumnWidth(2, 24);

    //QPushButton* callBtn = nullptr;
    QPushButton* videoBtn = nullptr;
    CallButton* callBtn = nullptr;;



    for (int ii = 0; ii < m_Data->AllStation.size(); ++ii)
    {
        item = new QStandardItem(m_Data->AllStation[ii].StationName);
        item_1->appendRow(item);
        qDebug() << item->index();
        btnItem1 =  new QStandardItem();
        btnItem2 =  new QStandardItem();
        item_1->setChild(item->row(), 1, btnItem1);
        item_1->setChild(item->row(), 2, btnItem2);
        qDebug() << btnItem1->index() << btnItem2->index();
        //item->appendRow(item2);
        callBtn = new CallButton();
        connect(callBtn, &CallButton::Call, m_pVoice, &VoiceDialog::onCall);
        callBtn->setFixedSize(24, 24);
        callBtn->setIcon(QIcon(":/Pictures/call.png"));
        //connect(callBtn, &QPushButton::clicked, [=](){
        //
        //});

        videoBtn = new QPushButton;
        videoBtn->setFixedSize(24, 24);
        videoBtn->setIcon(QIcon(":/Pictures/video.png"));
        connect(videoBtn, &QPushButton::clicked, [ = ]()
        {
            videoBtn->setEnabled(false);
            RepetitionWidget* m_pRepetition = new RepetitionWidget();
            m_pRepetition->setWindowTitle(QString("%1界面复示").arg(m_Data->AllStation[ii].StationName));
            connect(m_pRepetition, &RepetitionWidget::repetitionEnd, [ = ]()
            {
                videoBtn->setEnabled(true);
            });
            m_pRepetition->show();
            qDebug() << "11111111111111111111111111111";
        });

        ui->treeView->setIndexWidget(btnItem1->index(), callBtn);
        ui->treeView->setIndexWidget(btnItem2->index(), videoBtn);
    }

    ui->treeView->expandAll();
    ui->treeView->update();

}

void CStationManager::UpdateTreeData()
{
    TreeModel->clear();
    SetTreeData();
    if (ui->scrollAreaWidgetContents->GetCurrentStation() == nullptr && m_Data->AllStation.size() > 0)
    {
        ui->scrollAreaWidgetContents->SetCurrentStation(m_Data->AllStation[0].pStationData);
    }
}
#include"JSJ/jsj_usermanagerdlg.h"
void CStationManager::UserManagerShowInfo()
{

    //InitMenu();
    QPushButton *pBtn = (QPushButton*)sender();
    if (pBtn)
    {
        QString text = pBtn->text();
        if (text == "用户管理")
        {
            isSHow = true;
            on_MenuBtn_clicked();
            ui->MenuBtn->hide();
            ui->stackedWidget->setCurrentIndex(1);
        }
        else if (text == "调度中心")
        {
            isSHow = true;
            on_MenuBtn_clicked();
            ui->MenuBtn->hide();
            ui->stackedWidget->setCurrentIndex(2);
        }
        else
        {
            ui->page->setFixedHeight(LastHeight);
            ui->page->setFixedHeight(LastHeight);
            ui->scrollArea->setFixedHeight(LastHeight);
            ui->scrollArea->setFixedHeight(LastHeight);
            ui->stackedWidget->setCurrentIndex(0);
            qDebug()<<"切换后Size："<<ui->page->height();
            qDebug()<<"切换后第二Size："<<ui->scrollArea->height();
            ui->MenuBtn->show();
        }
    }
}
#include"JSJ/cmsgbox.h"
void CStationManager::UpdateControl_Info(ControlInfo &info)
{
    QString Tip = QString("%1车务终端请求控制模式转换").arg(info.StationName);
    int mb = CMsgBox::showMsgBox(Tip,this);
    if(mb)
    {
        controldlg = new jsj_Controldlg(info);
        controldlg->show();
    }
}

void CStationManager::InitButtonLayout()
{
    QVector<MangerStr>::iterator it = m_Data->ManagerMenue.begin();
    for (; it < m_Data->ManagerMenue.end(); it++)
    {
        ui->horizontalLayout_menue->addWidget(it->MenueButton);
        connect(it->MenueButton, SIGNAL(clicked()), this, SLOT(UserManagerShowInfo()));
        if (it->menueName == "站场操作")
        {

            for (int i = 0; i < it->pSubGroup->buttons().size(); i++)
            {

                ui->horizontalLayout->addWidget(it->pSubGroup->buttons()[i]);
            }
            m_Data->setButtonChecked(it->MenueButton, true);
            m_Data->pButtonGroup = it->pSubGroup;
        }
    }
    ui->horizontalLayout_menue->addStretch();
    ui->horizontalLayout_menue->addWidget(StationRestBtn);
    ui->horizontalLayout_menue->addWidget(All_StationRest_Btn);
    ui->horizontalLayout_menue->addWidget(pCloseButton);
    ui->horizontalLayout->addStretch();
    if(m_Data->pButtonGroup!=nullptr)
        connect(m_Data->pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotPushButton(int)));
}

void CStationManager::UpdateButtonLayout(QPushButton *pBtn)
{
    QVector<MangerStr>::iterator it = m_Data->ManagerMenue.begin();
    for (; it < m_Data->ManagerMenue.end(); it++)
    {
        if (pBtn == it->MenueButton)
        {
            QLayoutItem *child;
            while ((child = ui->horizontalLayout->takeAt(0)) != nullptr)
            {
                if (child->widget())
                {
                    child->widget()->setParent(nullptr);
                    ui->horizontalLayout->removeWidget(child->widget());
                    delete child;
                }
            }
            m_Data->clearOtherButtonChecked(pBtn);
            for (int i = 0; i < it->pSubGroup->buttons().size(); i++)
            {
                if (it->pSubGroup->buttons()[i]->text() != "")
                    ui->horizontalLayout->addWidget(it->pSubGroup->buttons()[i]);

            }
            disconnect(m_Data->pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotPushButton(int)));
            m_Data->pButtonGroup = it->pSubGroup;
            break;
        }
    }
    connect(m_Data->pButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotPushButton(int)));
    ui->horizontalLayout->addStretch();
}

void CStationManager::ClearAndUpdate(CStationJSJ* pStation)
{
    UpdateTreeData();
    if (ui->scrollAreaWidgetContents->GetCurrentStation() == pStation && m_Data->AllStation.size() >= 1)
        ui->scrollAreaWidgetContents->SetCurrentStation(m_Data->AllStation[0].pStationData);
    else if (ui->scrollAreaWidgetContents->GetCurrentStation() == pStation && m_Data->AllStation.size() == 0)
        ui->scrollAreaWidgetContents->SetCurrentStation(nullptr);
}

void CStationManager::Connect(CStationJSJ* pStation)
{
    if (pStation)
        connect(pStation, SIGNAL(ClearMenueButton()), pJM, SLOT(ClearMenueButton_Slot()));
}

void CStationManager::SetCurrentStation(int index)
{
    if (index < m_Data->AllStation.size())
    {
        if (ui->scrollAreaWidgetContents->GetCurrentStation() != m_Data->AllStation[index].pStationData)
            ui->scrollAreaWidgetContents->SetCurrentStation(m_Data->AllStation[index].pStationData);
    }
}

void CStationManager::slotOntimeout()
{
    nElapsed++;
    if (nElapsed % 5 == 0)
    {
        ui->scrollAreaWidgetContents->update();
        ui->scrollAreaWidgetContents->ManagerOntimer();
        m_Data->timePro(nElapsed);
    }
    m_Data->Time100MS();
}
#include<QDateTime>
#include"JSJ/cmsgbox.h"
void CStationManager::slotPushButton(int index)
{
    QPushButton *pbtn = qobject_cast<QPushButton*> (m_Data->pButtonGroup->button(index));
    if(pbtn==nullptr)
        return;
    QString text = pbtn->text();
    QString str = "";
    int Id =  m_Data->GetId_ByStation(ui->scrollAreaWidgetContents->GetCurrentStation());
    if(Id == -1)
    {
        CMsgBox::showMsgBox( "当前未连接任何站场,请检查",this);
        Id = 0;
        return;
    }
    ++Id;//CTC接收时  Id是从1开始的
    if (text == "时钟同步")
    {
        TX_Send_SERVICES.Send_Clock_Same(1,QDateTime::currentDateTime(),CTC);
        TX_Send_SERVICES.Send_Clock_Same(1,QDateTime::currentDateTime(),LS_INFO);
    }
    else if (text == "站场重置")
    {
        TX_Send_SERVICES.Send_Resetto_CTC(Id);
    }

    else if (text == "控制模式转换")
    {
        controldlg = new jsj_Controldlg(DBSERVICES.tempControl);
        controldlg->show();
    }

    else if(text == "一键启动")
    {
        self_Startdlg = new jsj_Self_Startdlg();
        self_Startdlg->show();
    }
    else if (text == "自触故障")
    {
        if(DBSERVICES.tempControl.zc_GZ_Set)
            str = QString("请确定是否进行%1操作？")
                    .arg(text);
        else
            str = QString("请确定是否取消%1操作？")
                    .arg(text);
        int mb = CMsgBox::showMsgBox( str,this);
        if(!mb)
        {
            TX_Send_SERVICES.Send_GZInfo(DBSERVICES.tempControl.zc_GZ_Set,ZCWRONG,Id);//首先默认StationId为1
        }
    }
    else if (text == "一键关机")
    {
        str = QString("请确定是否进行%1操作？")
                .arg(text);
        int mb = CMsgBox::showMsgBox( str,this);
        if(!mb)
        {
            TX_Send_SERVICES.Send_ShutCmd_(Id);
            system("shutdown -s -t 3");//3秒后关机
        }
    }
    else if(text == "车次号丢失故障")
    {
        if(DBSERVICES.tempControl.ccnumberLose_GZ_Set)
            str = QString("请确定是否进行%1操作？")
                    .arg(text);
        else
            str = QString("请确定是否取消%1操作？")
                    .arg(text);
        int mb = CMsgBox::showMsgBox( str,this);
        if(!mb)
        {
            TX_Send_SERVICES.Send_GZInfo(DBSERVICES.tempControl.ccnumberLose_GZ_Set,CCHDS,Id);//首先默认StationId为1
        }
    }
    else if(text == "黑屏故障")
    {
        if(DBSERVICES.tempControl.ctcshowMiss_GZ_Set)
            str = QString("请确定是否进行%1操作？")
                    .arg(text);
        else
            str = QString("请确定是否取消%1操作？")
                    .arg(text);
        int mb = CMsgBox::showMsgBox( str,this);
        if(!mb)
            TX_Send_SERVICES.Send_GZInfo(DBSERVICES.tempControl.ctcshowMiss_GZ_Set,BSZD,Id);//首先默认StationId为1
    }
    else if (text == "一键下发")
    {
        TX_Send_SERVICES.Send_GZInfo(DBSERVICES.tempControl.ccnumberLose_GZ_Set,YJXF,Id);//首先默认StationId为1
    }
    if (ui->scrollAreaWidgetContents->GetCurrentStation())
    {
        if (text == "列车监控")
        {
            pMonitorDlg->MoveWindow();
            pMonitorDlg->show();
        }
        else
            ui->scrollAreaWidgetContents->GetCurrentStation()->ButtonClicked(pbtn);
    }
}

void CStationManager::InitVoiceCall()
{
    GlobalData::m_pConfigInfo = new ConfigInfo;
    GlobalData::m_pConfigInfo->init();

    m_pVoice = new VoiceDialog;
    connect(m_pVoice, &VoiceDialog::NewMember, [ = ](QString ip, uint port, QString name)
    {
        int row = getBtnItemRowByName(name);
        if (row >= 0)
        {
            CallButton* btn = (CallButton*)ui->treeView->indexWidget(TreeModel->item(0, 0)->child(row, 1)->index());
            if (btn)
            {
                btn->init(ip, port, name);
                btn->setEnabled(true);
            }
        }
    });
    connect(m_pVoice, &VoiceDialog::MemberQuit, [ = ](QString ip, uint port)
    {
        port = -1;
        //int row = getBtnItemRowByName(name);
        //if(row >= 0)
        //{
        //    CallButton* btn = (CallButton*)ui->treeView->indexWidget(TreeModel->index(row, 1));
        //    btn->setEnabled(false);
        //}
    });
}

int CStationManager::getBtnItemRowByName(QString name)
{
    for (int i = 0; i < m_Data->AllStation.size(); ++i)
    {
        qDebug() << "TreeModel" << TreeModel->item(0, 0)->child(i, 0)->text();
        if (TreeModel->item(0, 0)->child(i, 0)->text() == name)
            return i;
    }
    return -1;
}

void CStationManager::InitMenu()
{
    ui->MenuBtn->setStyleSheet("border-image: url(:/Pictures/弹出.png);");
    isSHow = false;
    ui->MenuBtn->show();
    ui->treeView->hide();
    // ui->horizontalLayout_4->addStretch();
}

void CStationManager::closeWindow()
{
    int mb = CMsgBox::showMsgBox( "确定退出系统吗?",this);
    if(!mb)
    {
        DBSERVICES.EndTime = QDateTime::currentDateTime();
        QString Str = QString("打开时间:%1          关闭时间:%2           本次使用时长:%3 %4")
                .arg(DBSERVICES.StartTime.toString("yyyy-MM-dd-hh:mm"))
                .arg(DBSERVICES.EndTime.toString("yyyy-MM-dd-hh:mm"))
                .arg(DBSERVICES.GetTimeLong(DBSERVICES.StartTime,DBSERVICES.EndTime))
                .arg("\r\n");

        QString Path = QString("%1/data/TCCuserlog.txt").arg(QCoreApplication::applicationDirPath());
        DBSERVICES.writeTxt(Path,Str);
        qApp->quit();
    }

}

#include"JSJ/jsj_txservice.h"
void CStationManager::on_pushButton_clicked()
{
    qDebug()<<"2333";
}

void CStationManager::on_MenuBtn_clicked()
{
    if(isSHow)
    {
        ui->MenuBtn->setStyleSheet("border-image: url(:/Pictures/弹出.png);");
        //ui->MenuBtn->move(QPoint(0,ui->MenuBtn->y()));
        isSHow = false;
        ui->treeView->hide();
        //ui->horizontalLayout_4->addStretch();
        ui->scrollArea->setFixedSize(QSize(1840,ui->scrollArea->height()));
        //qDebug()<<"Height1::"<<ui->scrollArea->height();
    }else
    {
        ui->MenuBtn->setStyleSheet("border-image: url(:/Pictures/隐藏.png);");
        isSHow = true;
        ui->treeView->show();
        ui->scrollArea->setFixedSize(QSize(1700,ui->scrollArea->height()));
        // qDebug()<<"Height2::"<<ui->scrollArea->height();
    }
}
