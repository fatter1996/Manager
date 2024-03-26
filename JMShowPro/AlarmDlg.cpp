#include "AlarmDlg.h"
#include "ui_AlarmDlg.h"
#include <qdatetime.h>
#include <QtDebug>
#include <qlineedit.h>

CAlarmDlg::CAlarmDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAlarmDlg)
{
    ui->setupUi(this);
}

CAlarmDlg::~CAlarmDlg()
{
    delete ui;
}

void CAlarmDlg::init(int type,QPoint pt)
{
    count=0;
    DlgType=type;
    if(type==0x11)  //标准界面左侧报警信息窗口，红底黑字
    {
        Qt::WindowFlags nFlags=this->windowFlags();
        this->setWindowFlags(nFlags|Qt::FramelessWindowHint);

        nFlags=ui->scrollArea->windowFlags();
        ui->scrollArea->setWindowFlags(nFlags|Qt::FramelessWindowHint);

        this->move(pt);
        ui->scrollAreaWidgetContents->setStyleSheet("background-color:red;");
    }
    else if(type==0x22)  //标准界面右侧提示信息窗口，白底黑字
    {
        //设置无边框
        Qt::WindowFlags nFlags=this->windowFlags();
        this->setWindowFlags(nFlags|Qt::FramelessWindowHint);

        nFlags=ui->scrollArea->windowFlags();
        ui->scrollArea->setWindowFlags(nFlags|Qt::FramelessWindowHint);
        this->resize(width()-20,height()-20);
        this->move(pt.x(),pt.y()+20);
        ui->scrollAreaWidgetContents->setStyleSheet("background-color:gray;");
    }
    else if(type==0x33)  //卡斯柯界面提示信息窗口
    {
        //设置无边框
        Qt::WindowFlags nFlags=this->windowFlags();
        this->setWindowFlags(nFlags|Qt::FramelessWindowHint);
        ui->scrollArea->setVisible(false);
        this->resize(width()-20,20);
        this->move(pt);
        //this->setStyleSheet("background-color:gray;");
        lineEdit_State_DevA=new QLineEdit(this);
        lineEdit_State_DevA->setText("主机A");
        lineEdit_State_DevA->resize(width()/5+1,height());
        lineEdit_State_DevA->move(0,0);
        lineEdit_State_DevA->setStyleSheet("background-color:rgb(0,255,0);");
        lineEdit_State_DevA->setFont(QFont( "黑体" , 10 ,  QFont::Medium) );
        lineEdit_State_DevA->setReadOnly(true);
        lineEdit_State_DevA->setAlignment(Qt::AlignCenter);

        lineEdit_State_DevB=new QLineEdit(this);
        lineEdit_State_DevB->setText("主机B");
        lineEdit_State_DevB->resize(width()/5+1,height());
        lineEdit_State_DevB->move(width()/5+1,0);
        lineEdit_State_DevB->setStyleSheet("background-color:yellow;");
        lineEdit_State_DevB->setFont(QFont( "黑体" , 10 ,  QFont::Medium) );
        lineEdit_State_DevB->setReadOnly(true);
        lineEdit_State_DevB->setAlignment(Qt::AlignCenter);

        lineEdit_State_OpA=new QLineEdit(this);
        lineEdit_State_OpA->setText("操作A");
        lineEdit_State_OpA->resize(width()/5+1,height());
        lineEdit_State_OpA->move(2*(width()/5+1),0);
        lineEdit_State_OpA->setStyleSheet("background-color:rgb(0,255,0);");
        lineEdit_State_OpA->setFont(QFont( "黑体" , 10 ,  QFont::Medium) );
        lineEdit_State_OpA->setReadOnly(true);
        lineEdit_State_OpA->setAlignment(Qt::AlignCenter);

        lineEdit_State_OpB=new QLineEdit(this);
        lineEdit_State_OpB->setText("操作B");
        lineEdit_State_OpB->resize(width()/5+1,height());
        lineEdit_State_OpB->move(3*(width()/5+1),0);
        lineEdit_State_OpB->setStyleSheet("background-color:yellow;");
        lineEdit_State_OpB->setFont(QFont( "黑体" , 10 ,  QFont::Medium) );
        lineEdit_State_OpB->setReadOnly(true);
        lineEdit_State_OpB->setAlignment(Qt::AlignCenter);

        lineEdit_State_WXJ=new QLineEdit(this);
        lineEdit_State_WXJ->setText("维修机");
        lineEdit_State_WXJ->resize(width()/5+1,height());
        lineEdit_State_WXJ->move(4*(width()/5+1),0);
        lineEdit_State_WXJ->setStyleSheet("background-color:rgb(0,255,0);");
        lineEdit_State_WXJ->setFont(QFont( "黑体" , 10 ,  QFont::Medium) );
        lineEdit_State_WXJ->setReadOnly(true);
        lineEdit_State_WXJ->setAlignment(Qt::AlignCenter);
    }
}
void CAlarmDlg::add_TSXX(QString str,QColor color)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr =time.toString("yyyy.MM.dd hh:mm:ss");
    QString showStr=timeStr+"  "+str;
    QString strColor="black";

    if((DlgType!=0x11)&&(DlgType!=0x22))
    {
        return;
    }
    QLineEdit *edit=new QLineEdit(ui->scrollAreaWidgetContents);
    edit->resize(800,20);
    edit->move(0,20*count);
    edit->setFont(QFont( "Timers" , 9 ,  QFont::Normal) );

    if(DlgType == 0x22)
    {
        edit->setStyleSheet("background-color:rgba(255,0,0,1);");
    }
    else
    {
        edit->setStyleSheet("background-color:rgba(127,127,127,1);");
    }

    if(color == Qt::red)
    {
        strColor="red";
    }
    else if(color == Qt::green)
    {
        strColor="rgb(0,255,0)";
    }
    else if(color == Qt::yellow)
    {
        strColor="yellow";
    }
    else if(color == Qt::black)
    {
        strColor="black";
    }
    edit->setStyleSheet(QString("color:%1;border: none;").arg(strColor));

    edit->setReadOnly(true);

    edit->setText(showStr);
    edit->show();
    count++;
    if(20*count > ui->scrollAreaWidgetContents->height())
    {
        ui->scrollAreaWidgetContents->resize(400,20*count);
    }
    //qDebug()<<showStr;
}
