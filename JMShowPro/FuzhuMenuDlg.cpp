#include "FuzhuMenuDlg.h"
#include "ui_FuzhuMenuDlg.h"

CFuzhuMenuDlg::CFuzhuMenuDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFuzhuMenuDlg)
{
    ui->setupUi(this);
}

CFuzhuMenuDlg::~CFuzhuMenuDlg()
{
    delete ui;
}

void CFuzhuMenuDlg::on_pushButton_Cancel_clicked()
{
    close();
}
void CFuzhuMenuDlg::init(bool isYJFlag,bool isBJFlag,bool isQFTJFlag,bool isJTGD,bool isDCWZ,bool isWCQDName,bool isDCQDName,bool isXHDName,bool isDCName)
{
    setWindowFlags(windowFlags()|(Qt::FramelessWindowHint));  //去掉边框
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    setFocus();
    move(mapFromGlobal(cursor().pos()).x(),mapFromGlobal(cursor().pos()).y()-height());

    myMapper =new QSignalMapper(this);
    QPushButton *button[14]={ui->pushButton_LFShow,ui->pushButton_XTWH,ui->pushButton_YJDlgShow,ui->pushButton_BJDlgShow,ui->pushButton_QPCZ,
                             ui->pushButton_SPCZ,ui->pushButton_QFTJShow,ui->pushButton_SpeekStop,ui->pushButton_JTGD,ui->pushButton_DCWZShow,
                             ui->pushButton_WCQDNameShow,ui->pushButton_DCQDNameShow,ui->pushButton_XHDNameShow,ui->pushButton_DCNameShow};

    if(isYJFlag == true)
    {
        ui->pushButton_YJDlgShow->setText("预警窗口显示");
    }
    else
    {
        ui->pushButton_YJDlgShow->setText("预警窗口隐藏");
    }
    if(isBJFlag == true)
    {
        ui->pushButton_BJDlgShow->setText("报警窗口显示");
    }
    else
    {
        ui->pushButton_BJDlgShow->setText("报警窗口隐藏");
    }
    if(isQFTJFlag == true)
    {
        ui->pushButton_QFTJShow->setText("破封统计显示");
    }
    else
    {
        ui->pushButton_QFTJShow->setText("破封统计隐藏");
    }
    if(isJTGD == true)
    {
        ui->pushButton_JTGD->setText("接通光带隐藏");
    }
    else
    {
        ui->pushButton_JTGD->setText("接通光带显示");
    }
    if(isDCWZ == true)
    {
        ui->pushButton_DCWZShow->setText("道岔位置隐藏");
    }
    else
    {
        ui->pushButton_DCWZShow->setText("道岔位置显示");
    }
    if(isWCQDName == true)
    {
        ui->pushButton_WCQDNameShow->setText("无岔区段名称隐藏");
    }
    else
    {
        ui->pushButton_WCQDNameShow->setText("无岔区段名称显示");
    }
    if(isDCQDName == true)
    {
        ui->pushButton_DCQDNameShow->setText("道岔区段名称隐藏");
    }
    else
    {
        ui->pushButton_DCQDNameShow->setText("道岔区段名称显示");
    }
    if(isXHDName == true)
    {
        ui->pushButton_XHDNameShow->setText("信号机名称隐藏");
    }
    else
    {
        ui->pushButton_XHDNameShow->setText("信号机名称显示");
    }
    if(isDCName == true)
    {
        ui->pushButton_DCNameShow->setText("道岔名称隐藏");
    }
    else
    {
        ui->pushButton_DCNameShow->setText("道岔名称显示");
    }


    for(int i=0;i<14;i++)
    {
        button_Input[i]=button[i];
    }

    for(int i=0;i<14;i++)
    {
        connect(button_Input[i],SIGNAL(clicked(bool)),myMapper,SLOT(map()));
        myMapper->setMapping(button_Input[i],i);
    }

    connect(myMapper,SIGNAL(mapped(int)),this,SLOT(slot_AnPro(int)));
}
void CFuzhuMenuDlg::slot_AnPro(int i)
{
    if(i>=14)
    {
        return;
    }
    close();
    emit signal_FuzhuMenuDlg_AN(i);
}
