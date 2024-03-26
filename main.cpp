#include "mainwindow.h"

#include <QApplication>
#include"QDesktopWidget"
#include "stationmanager.h"
#include"JSJ/jsj_services.h"
#include"JSJ/jsj_txservice.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    QString Time = "12:20";
    QTime t = QTime::fromString(Time,"hh:mm");
    qDebug()<<t;

    DBSERVICES.ReadSelfStartConfig();
    DBSERVICES.loadDBConfig();
    TX_Send_SERVICES.bindPort();
    DBSERVICES.GetTrainTypeString("TrainNumType");
    qApp->setStyleSheet("QToolTip{border: 0px solid black;background:rgb(255,255,127);}");
    QString str = DBSERVICES.GetConnect_toDB();
    if(str == "")
    {
        DBSERVICES.GetddInfoList("调度命令计划");
    }
    else
    {
        QMessageBox::information(nullptr,"提示",QString("数据库连接失败,原因%1,请联系管理员处理").arg(str));
    }
    CStationManager * manager = new CStationManager();
//    QRect trc = QApplication::desktop()->availableGeometry();
//    trc.setHeight(trc.height()-5);
//    manager->setFixedSize(trc.size());



    manager->show();
    DBSERVICES.StartTime = QDateTime::currentDateTime();
    return a.exec();
}
