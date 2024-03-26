#include "jsj_logindlg.h"
#include "ui_jsj_logindlg.h"
#include<QMessageBox>
#include"jsj_services.h"
jsj_logindlg::jsj_logindlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::jsj_logindlg)
{
    ui->setupUi(this);
     DBSERVICES.SetBackImage(this,"登录背景");
     setWindowModality(Qt::ApplicationModal);
}

jsj_logindlg::~jsj_logindlg()
{
    delete ui;
}
#include<QDebug>
#include"cmsgbox.h"
void jsj_logindlg::on_pushButton_clicked()
{
    QString LoginName = ui->lineEdit->text();
    QString LoginPassword = ui->lineEdit_2->text();
    if(LoginName==""||LoginPassword == "")
    {
      CMsgBox::showMsgBox("请检查账号或密码输入错误");
      return;
    }else
    {
        DBSERVICES.GetOperateInfoByloginInfo(LoginName,LoginPassword);

        if(DBSERVICES.OperateGet.Post!="")
        {
             this->close();
        }else
        {
            CMsgBox::showMsgBox("请检查管理员账号或密码输入错误");
            return;
        }
    }
}

void jsj_logindlg::on_pushButton_2_clicked()
{
    this->close();
}
