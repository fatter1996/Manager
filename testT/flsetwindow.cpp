#include "flsetwindow.h"
#include "ui_flsetwindow.h"


#include "mainwindow.h"

FLSetWindow::FLSetWindow(TrackView_Lable *p,QWidget *parent) :
    trackview_Lable(p),
    QMainWindow(parent),
    ui(new Ui::FLSetWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("防溜设置 - TDCS/CTC3.0 通用车站操作终端"));
    this->setWindowFlags(Qt::Dialog);
    this->move(600,250);


    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法

    LoginState=trackview_Lable->LoginState;
    JieFaCheState=trackview_Lable->JieLuState;
    Track_widget_line_Flag=trackview_Lable->Track_widget_line_Flag;
    trafficlog_OrderLineNum=trackview_Lable->vect_list_i;

    ui->radioButton->setText("上行铁鞋");
    ui->radioButton_2->setText("下行铁鞋");
    ui->radioButton_7->setText("上行铁鞋");
    ui->radioButton_8->setText("下行铁鞋");
    ui->radioButton_13->setText("上行铁鞋");
    ui->radioButton_14->setText("下行铁鞋");

    ui->label_2->setText("  "+pmainwindow->mainWindowForStationTrack->TrackArray_formainwindow[Track_widget_line_Flag].strGDName);
    ui->label_2->setFont(QFont("Agency FB",12,75));
    ui->label_2->setEnabled(false);
    ui->tabWidget->setTabEnabled(1,false);
    ui->tabWidget->setTabEnabled(2,false);
    if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip.count()==pmainwindow->mainWindowForStationTrack->TrackNum)
    {
        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_antislip==1)
        {
            ui->radioButton->setChecked(true);
            ui->radioButton_4->setChecked(false);
            ui->radioButton_6->setChecked(false);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_antislip==2)
        {
            ui->radioButton->setChecked(false);
            ui->radioButton_4->setChecked(true);
            ui->radioButton_6->setChecked(false);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_antislip==4)
        {
            ui->radioButton->setChecked(false);
            ui->radioButton_4->setChecked(false);
            ui->radioButton_6->setChecked(true);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_antislip==18)
        {
            ui->radioButton->setChecked(true);
            ui->radioButton_4->setChecked(true);
            ui->radioButton_6->setChecked(false);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_antislip==20)
        {
            ui->radioButton->setChecked(true);
            ui->radioButton_4->setChecked(false);
            ui->radioButton_6->setChecked(true);
        }
        else
        {
            ui->radioButton->setChecked(false);
            ui->radioButton_4->setChecked(false);
            ui->radioButton_6->setChecked(false);
        }

        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_antislip==1)
        {
            ui->radioButton_2->setChecked(true);
            ui->radioButton_3->setChecked(false);
            ui->radioButton_5->setChecked(false);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_antislip==2)
        {
            ui->radioButton_2->setChecked(false);
            ui->radioButton_3->setChecked(true);
            ui->radioButton_4->setChecked(false);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_antislip==4)
        {
            ui->radioButton_2->setChecked(false);
            ui->radioButton_3->setChecked(false);
            ui->radioButton_5->setChecked(true);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_antislip==18)
        {
            ui->radioButton_2->setChecked(true);
            ui->radioButton_3->setChecked(true);
            ui->radioButton_5->setChecked(false);
        }
        else if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_antislip==20)
        {
            ui->radioButton_2->setChecked(true);
            ui->radioButton_3->setChecked(false);
            ui->radioButton_5->setChecked(true);
        }
        else
        {
            ui->radioButton_2->setChecked(false);
            ui->radioButton_3->setChecked(false);
            ui->radioButton_5->setChecked(false);
        }

    //    QIntValidator onlyInt(0, 5000, this);
    //    ui->lineEdit->setValidator(&onlyInt);
    //    ui->lineEdit_2->setValidator(&onlyInt);
    //    ui->lineEdit_3->setValidator(&onlyInt);
    //    ui->lineEdit_4->setValidator(&onlyInt);
    //    ui->lineEdit_5->setValidator(&onlyInt);
    //    ui->lineEdit_6->setValidator(&onlyInt);
    //    ui->lineEdit_7->setValidator(&onlyInt);
        ui->lineEdit->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
        ui->lineEdit_2->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
        ui->lineEdit_3->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
        ui->lineEdit_4->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
        ui->lineEdit_5->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
        ui->lineEdit_6->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));
        ui->lineEdit_7->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$")));


        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_txnum!=0)
        {
            ui->lineEdit->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_txnum));
        }
        else
        {
            ui->lineEdit->setText(NULL);
        }
        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_txnum!=0)
        {
            ui->lineEdit_2->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_txnum));
        }
        else
        {
            ui->lineEdit_2->setText(NULL);
        }
        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].trainsnum!=0)
        {
            ui->lineEdit_3->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].trainsnum));
        }
        else
        {
            ui->lineEdit_3->setText(NULL);
        }

        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_jgqnum!=0)
        {
            ui->lineEdit_4->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_jgqnum));
        }
        else
        {
            ui->lineEdit_4->setText(NULL);
        }

        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_jgqnum!=0)
        {
            ui->lineEdit_5->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_jgqnum));
        }
        else
        {
            ui->lineEdit_5->setText(NULL);
        }

        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_meters!=0)
        {
            ui->lineEdit_6->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].l_meters));
        }
        else
        {
            ui->lineEdit_6->setText(NULL);
        }

        if(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_meters!=0)
        {
            ui->lineEdit_7->setText(QString::number(pmainwindow->mainWindowForStationTrack->vect_list_gd_antislip[Track_widget_line_Flag].r_meters));
        }
        else
        {
            ui->lineEdit_7->setText(NULL);
        }
    }
}

FLSetWindow::~FLSetWindow()
{
    delete ui;
}

void FLSetWindow::on_pushButton_5_clicked()
{
    hide();
}

void FLSetWindow::on_pushButton_4_clicked()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    pmainwindow->FLSZ_GDNum=pmainwindow->StationGDNode[Track_widget_line_Flag].nCode;

    if((ui->radioButton->isChecked()==true)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("://data/res/左边铁鞋.bmp");
//        trackview_Lable->Btn2->setIcon(icon);
//        trackview_Lable->Btn2->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_S=1;
        pmainwindow->FLSZ_Side=0xAA;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x01;
        pmainwindow->FLSZ_TXNum=ui->lineEdit->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_4->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_6->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    else if((ui->radioButton->isChecked()==false)&&(ui->radioButton_4->isChecked()==true)&&(ui->radioButton_6->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("://data/res/制动机.bmp");
//        trackview_Lable->Btn2->setIcon(icon);
//        trackview_Lable->Btn2->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_S=4;
        pmainwindow->FLSZ_Side=0xAA;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x02;
        pmainwindow->FLSZ_TXNum=ui->lineEdit->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_4->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_6->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    else if((ui->radioButton->isChecked()==false)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==true))
    {
//        QIcon icon;
//        icon.addFile("://data/res/紧固器.bmp");
//        trackview_Lable->Btn2->setIcon(icon);
//        trackview_Lable->Btn2->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_S=5;
        pmainwindow->FLSZ_Side=0xAA;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x04;
        pmainwindow->FLSZ_TXNum=ui->lineEdit->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_4->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_6->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    else if((ui->radioButton->isChecked()==true)&&(ui->radioButton_4->isChecked()==true)&&(ui->radioButton_6->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("://data/res/左边铁鞋+制动机.bmp");
//        trackview_Lable->Btn2->setIcon(icon);
//        trackview_Lable->Btn2->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_S=2;
        pmainwindow->FLSZ_Side=0xAA;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x12;
        pmainwindow->FLSZ_TXNum=ui->lineEdit->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_4->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_6->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    else if((ui->radioButton->isChecked()==true)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==true))
    {
//        QIcon icon;
//        icon.addFile("://data/res/左边铁鞋+紧固器.bmp");
//        trackview_Lable->Btn2->setIcon(icon);
//        trackview_Lable->Btn2->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_S=3;
        pmainwindow->FLSZ_Side=0xAA;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x14;
        pmainwindow->FLSZ_TXNum=ui->lineEdit->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_4->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_6->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    else if((ui->radioButton->isChecked()==false)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("");
//        trackview_Lable->Btn2->setIcon(icon);
//        trackview_Lable->Btn2->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_S=0;
        pmainwindow->FLSZ_Side=0xAA;
        pmainwindow->FLSZ_Set=0xBB;
        pmainwindow->FLSZ_Type=0x00;
        pmainwindow->FLSZ_TXNum=ui->lineEdit->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_4->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_6->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }

    QString str1;
    QString str2;
    QString str3;
    QString str4;
    if(pmainwindow->FLSZ_Side==0xAA)
    {
        str1="上行";
    }
    else if(pmainwindow->FLSZ_Side==0xBB)
    {
        str1="下行";
    }

    if(pmainwindow->FLSZ_Type==0x01)
    {
        str2=QString("铁鞋(%1)").arg(pmainwindow->FLSZ_TXNum);
    }
    else if(pmainwindow->FLSZ_Type==0x02)
    {
        str2=QString("人力制动机(%1)").arg(pmainwindow->FLSZ_JGQNum);
    }
    else if(pmainwindow->FLSZ_Type==0x04)
    {
        str2=QString("人力制动机紧固器(%1)").arg(pmainwindow->FLSZ_JGQNum);
    }
    else if(pmainwindow->FLSZ_Type==0x12)
    {
        str2=QString("铁鞋(%1),人力制动机(%2)").arg(pmainwindow->FLSZ_TXNum).arg(pmainwindow->FLSZ_JGQNum);
    }
    else if(pmainwindow->FLSZ_Type==0x14)
    {
        str2=QString("铁鞋(%1),人力制动机紧固器(%2)").arg(pmainwindow->FLSZ_TXNum).arg(pmainwindow->FLSZ_JGQNum);
    }

    if(pmainwindow->FLSZ_JNMS>0)
    {
        str3=QString("警内%1米").arg(pmainwindow->FLSZ_JNMS);
    }
    else
    {
        str3="";
    }

    if(pmainwindow->FLSZ_CCXX>0)
    {
        str4=QString("存车信息%1").arg(pmainwindow->FLSZ_CCXX);
    }
    else
    {
        str4="";
    }

    str_QP_L = QString(str1+":"+str2+" "+str3+" "+str4);

    if(pmainwindow->FLSZ_Type==0)
    {
        str_QP_L="";
    }

    trackview_Lable->Btn2->setToolTip(str_QP_L);

    if((ui->radioButton_2->isChecked()==true)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("://data/res/右边铁鞋.bmp");
//        trackview_Lable->Btn37->setIcon(icon);
//        trackview_Lable->Btn37->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_X=1;
        pmainwindow->FLSZ_Side=0xBB;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x01;
        pmainwindow->FLSZ_TXNum=ui->lineEdit_2->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_5->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_7->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    if((ui->radioButton_2->isChecked()==false)&&(ui->radioButton_3->isChecked()==true)&&(ui->radioButton_5->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("://data/res/制动机.bmp");
//        trackview_Lable->Btn37->setIcon(icon);
//        trackview_Lable->Btn37->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_X=4;
        pmainwindow->FLSZ_Side=0xBB;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x02;
        pmainwindow->FLSZ_TXNum=ui->lineEdit_2->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_5->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_7->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    if((ui->radioButton_2->isChecked()==false)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==true))
    {
//        QIcon icon;
//        icon.addFile("://data/res/紧固器.bmp");
//        trackview_Lable->Btn37->setIcon(icon);
//        trackview_Lable->Btn37->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_X=5;
        pmainwindow->FLSZ_Side=0xBB;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x04;
        pmainwindow->FLSZ_TXNum=ui->lineEdit_2->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_5->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_7->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    if((ui->radioButton_2->isChecked()==true)&&(ui->radioButton_3->isChecked()==true)&&(ui->radioButton_5->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("://data/res/右边铁鞋+制动机.bmp");
//        trackview_Lable->Btn37->setIcon(icon);
//        trackview_Lable->Btn37->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_X=2;
        pmainwindow->FLSZ_Side=0xBB;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x12;
        pmainwindow->FLSZ_TXNum=ui->lineEdit_2->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_5->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_7->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    if((ui->radioButton_2->isChecked()==true)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==true))
    {
//        QIcon icon;
//        icon.addFile("://data/res/右边铁鞋+紧固器.bmp");
//        trackview_Lable->Btn37->setIcon(icon);
//        trackview_Lable->Btn37->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_X=3;
        pmainwindow->FLSZ_Side=0xBB;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x14;
        pmainwindow->FLSZ_TXNum=ui->lineEdit_2->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_5->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_7->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }
    if((ui->radioButton_2->isChecked()==false)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==false))
    {
//        QIcon icon;
//        icon.addFile("");
//        trackview_Lable->Btn37->setIcon(icon);
//        trackview_Lable->Btn37->setIconSize(QSize(30,30));
//        trackview_Lable->TrackStrArray[Track_widget_line_Flag].TrainTX_State_X=0;
        pmainwindow->FLSZ_Side=0xBB;
        pmainwindow->FLSZ_Set=0xAA;
        pmainwindow->FLSZ_Type=0x00;
        pmainwindow->FLSZ_TXNum=ui->lineEdit_2->text().toInt();
        pmainwindow->FLSZ_JGQNum=ui->lineEdit_5->text().toInt();
        pmainwindow->FLSZ_JNMS=ui->lineEdit_7->text().toInt();
        pmainwindow->FLSZ_CCXX=ui->lineEdit_3->text().toInt();
        pmainwindow->SendToTrainSimu_FLSZ();
    }

    if(pmainwindow->FLSZ_Type==0x01)
    {
        str2=QString("铁鞋(%1)").arg(pmainwindow->FLSZ_TXNum);
    }
    else if(pmainwindow->FLSZ_Type==0x02)
    {
        str2=QString("人力制动机(%1)").arg(pmainwindow->FLSZ_JGQNum);
    }
    else if(pmainwindow->FLSZ_Type==0x04)
    {
        str2=QString("人力制动机紧固器(%1)").arg(pmainwindow->FLSZ_JGQNum);
    }
    else if(pmainwindow->FLSZ_Type==0x12)
    {
        str2=QString("铁鞋(%1),人力制动机(%2)").arg(pmainwindow->FLSZ_TXNum).arg(pmainwindow->FLSZ_JGQNum);
    }
    else if(pmainwindow->FLSZ_Type==0x14)
    {
        str2=QString("铁鞋(%1),人力制动机紧固器(%2)").arg(pmainwindow->FLSZ_TXNum).arg(pmainwindow->FLSZ_JGQNum);
    }

    if(pmainwindow->FLSZ_Side==0xAA)
    {
        str1="上行";
    }
    else if(pmainwindow->FLSZ_Side==0xBB)
    {
        str1="下行";
    }
    if(pmainwindow->FLSZ_JNMS>0)
    {
        str3=QString("警内%1米").arg(pmainwindow->FLSZ_JNMS);
    }
    else
    {
        str3="";
    }

    if(pmainwindow->FLSZ_CCXX>0)
    {
        str4=QString("存车信息%1").arg(pmainwindow->FLSZ_CCXX);
    }
    else
    {
        str4="";
    }

    str_QP_R = QString(str1+":"+str2+" "+str3+" "+str4);
    if(pmainwindow->FLSZ_Type==0)
    {
        str_QP_R="";
    }
    trackview_Lable->Btn37->setToolTip(str_QP_R);

    if(ui->radioButton_7->isChecked()==true)
    {

    }
    if(ui->radioButton_8->isChecked()==true)
    {

    }
    if(ui->radioButton_9->isChecked()==true)
    {

    }
    if(ui->radioButton_10->isChecked()==true)
    {

    }
    if(ui->radioButton_11->isChecked()==true)
    {

    }
    if(ui->radioButton_12->isChecked()==true)
    {

    }
    if(ui->radioButton_13->isChecked()==true)
    {

    }
    if(ui->radioButton_14->isChecked()==true)
    {

    }
    if(ui->radioButton_15->isChecked()==true)
    {

    }
    if(ui->radioButton_16->isChecked()==true)
    {

    }
    if(ui->radioButton_17->isChecked()==true)
    {

    }
    if(ui->radioButton_18->isChecked()==true)
    {

    }
    hide();
}

void FLSetWindow::on_pushButton_3_clicked()
{
    MainWindow * pmainwindow = MainWindow::getMainWinPtr();  //在其他类中调用MainWindow中变量的方法
    pmainwindow->FLSZ_GDNum=pmainwindow->StationGDNode[Track_widget_line_Flag].nCode;

    if((ui->radioButton->isChecked()==true)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x01;
    }
    else if((ui->radioButton->isChecked()==false)&&(ui->radioButton_4->isChecked()==true)&&(ui->radioButton_6->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x02;
    }
    else if((ui->radioButton->isChecked()==false)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==true))
    {
        pmainwindow->FLSZ_Type=0x04;
    }
    else if((ui->radioButton->isChecked()==true)&&(ui->radioButton_4->isChecked()==true)&&(ui->radioButton_6->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x12;
    }
    else if((ui->radioButton->isChecked()==true)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==true))
    {
        pmainwindow->FLSZ_Type=0x14;
    }
    else if((ui->radioButton->isChecked()==false)&&(ui->radioButton_4->isChecked()==false)&&(ui->radioButton_6->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x00;
    }

    if((ui->radioButton_2->isChecked()==true)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x01;
    }
    if((ui->radioButton_2->isChecked()==false)&&(ui->radioButton_3->isChecked()==true)&&(ui->radioButton_5->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x02;
    }
    if((ui->radioButton_2->isChecked()==false)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==true))
    {
        pmainwindow->FLSZ_Type=0x04;
    }
    if((ui->radioButton_2->isChecked()==true)&&(ui->radioButton_3->isChecked()==true)&&(ui->radioButton_5->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x12;
    }
    if((ui->radioButton_2->isChecked()==true)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==true))
    {
        pmainwindow->FLSZ_Type=0x14;
    }
    if((ui->radioButton_2->isChecked()==false)&&(ui->radioButton_3->isChecked()==false)&&(ui->radioButton_5->isChecked()==false))
    {
        pmainwindow->FLSZ_Type=0x00;
    }
    QIcon icon;
    icon.addFile("");
    trackview_Lable->Btn2->setIcon(icon);
    trackview_Lable->Btn2->setIconSize(QSize(30,30));
    trackview_Lable->Btn37->setIcon(icon);
    trackview_Lable->Btn37->setIconSize(QSize(30,30));
    trackview_Lable->label_FLSet_L->setText("");
    trackview_Lable->label_FLSet_R->setText("");

    pmainwindow->FLSZ_Set=0xBB;

    pmainwindow->FLSZ_Side=0xAA;
    pmainwindow->FLSZ_TXNum=0;
    pmainwindow->FLSZ_JGQNum=0;
    pmainwindow->FLSZ_JNMS=0;
    pmainwindow->FLSZ_CCXX=0;
    pmainwindow->SendToTrainSimu_FLSZ();

    pmainwindow->FLSZ_Side=0xBB;
    pmainwindow->FLSZ_TXNum=0;
    pmainwindow->FLSZ_JGQNum=0;
    pmainwindow->FLSZ_JNMS=0;
    pmainwindow->FLSZ_CCXX=0;
    pmainwindow->SendToTrainSimu_FLSZ();


    hide();
}
