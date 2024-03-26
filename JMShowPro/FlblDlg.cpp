#include "FlblDlg.h"
#include "ui_FlblDlg.h"

CFLBLDlg::CFLBLDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CFLBLDlg)
{
    ui->setupUi(this);
}

CFLBLDlg::~CFLBLDlg()
{
    delete ui;
}

void CFLBLDlg::init(int zcCount,int devCount,int type,bool flag1,bool flag2,bool flag3)
{
    nType=type;
    nZCIndex=zcCount;
    nDevIndex=devCount;
    if(type == 0x11)
    {
        ui->pushButton_FLBL->setVisible(false);
        ui->checkBox_FLBL->setVisible(false);
        if(flag1 == true)
        {
            ui->checkBox_CQFLBL->setChecked(true);
        }
        else
        {
            ui->checkBox_CQFLBL->setChecked(false);
        }
        if(flag2 == true)
        {
            ui->checkBox_DWFLBL->setChecked(true);
        }
        else
        {
            ui->checkBox_DWFLBL->setChecked(false);
        }
        if(flag3 == true)
        {
            ui->checkBox_FWFLBL->setChecked(true);
        }
        else
        {
            ui->checkBox_FWFLBL->setChecked(false);
        }
    }
    else if(type == 0x22)
    {
        ui->pushButton_CQFLBL->setVisible(false);
        ui->checkBox_CQFLBL->setVisible(false);
        ui->pushButton_DWFLBL->setVisible(false);
        ui->checkBox_DWFLBL->setVisible(false);
        ui->pushButton_FWFLBL->setVisible(false);
        ui->checkBox_FWFLBL->setVisible(false);
        ui->pushButton_OK->move(3,32);
        ui->pushButton_Cancel->move(51,32);
        resize(width(),66);
        if(flag1 == true)
        {
            ui->checkBox_FLBL->setChecked(true);
        }
        else
        {
            ui->checkBox_FLBL->setChecked(false);
        }
    }
    setWindowFlags(windowFlags()|(Qt::FramelessWindowHint));  //去掉边框
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    setFocus();
    move(mapFromGlobal(cursor().pos()));
}

void CFLBLDlg::on_checkBox_FLBL_stateChanged(int arg1)
{
    ui->checkBox_FLBL->setChecked(arg1);
}

void CFLBLDlg::on_pushButton_FLBL_clicked()
{
    if(ui->checkBox_FLBL->isChecked() == true)
    {
        ui->checkBox_FLBL->setChecked(false);
    }
    else
    {
        ui->checkBox_FLBL->setChecked(true);
    }
}

void CFLBLDlg::on_pushButton_CQFLBL_clicked()
{
    if(ui->checkBox_CQFLBL->isChecked() == true)
    {
        ui->checkBox_CQFLBL->setChecked(false);
    }
    else
    {
        ui->checkBox_CQFLBL->setChecked(true);
    }
}

void CFLBLDlg::on_checkBox_CQFLBL_stateChanged(int arg1)
{
    ui->checkBox_CQFLBL->setChecked(arg1);
}

void CFLBLDlg::on_pushButton_DWFLBL_clicked()
{
    if(ui->checkBox_DWFLBL->isChecked() == true)
    {
        ui->checkBox_DWFLBL->setChecked(false);
    }
    else
    {
        ui->checkBox_DWFLBL->setChecked(true);
    }
}

void CFLBLDlg::on_checkBox_DWFLBL_stateChanged(int arg1)
{
    ui->checkBox_DWFLBL->setChecked(arg1);
}

void CFLBLDlg::on_pushButton_FWFLBL_clicked()
{
    if(ui->checkBox_FWFLBL->isChecked() == true)
    {
        ui->checkBox_FWFLBL->setChecked(false);
    }
    else
    {
        ui->checkBox_FWFLBL->setChecked(true);
    }
}

void CFLBLDlg::on_checkBox_FWFLBL_stateChanged(int arg1)
{
    ui->checkBox_FWFLBL->setChecked(arg1);
}

void CFLBLDlg::on_pushButton_OK_clicked()
{
    bool flag1=false;
    bool flag2=false;
    bool flag3=false;
    if(nType == 0x11)
    {
        flag1=ui->checkBox_CQFLBL->isChecked();
        flag2=ui->checkBox_DWFLBL->isChecked();
        flag3=ui->checkBox_FWFLBL->isChecked();
    }
    else if(nType == 0x22)
    {
        flag1=ui->checkBox_FLBL->isChecked();
    }
    emit signal_FlblDlgOK(nZCIndex,nDevIndex,flag1,flag2,flag3);
    accept();
}

void CFLBLDlg::on_pushButton_Cancel_clicked()
{
    close();
}
void CFLBLDlg::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Return)
    {
        on_pushButton_OK_clicked();
    }
    else if(event->key() == Qt::Key_Enter)
    {
        on_pushButton_OK_clicked();
    }
    else if(event->key() == Qt::Key_Escape)
    {
        on_pushButton_Cancel_clicked();
    }

}
