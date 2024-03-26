#include "jsj_maindlg.h"
#include "ui_jsj_maindlg.h"
#include"jsj_services.h"
JsJ_Maindlg::JsJ_Maindlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JsJ_Maindlg)
{
    ui->setupUi(this);
    DBSERVICES.SetBackImage(this,"背景");
    m_Group = new QButtonGroup(this);
    m_Group->addButton(ui->UserManagerBtn,0x01);
    m_Group->addButton(ui->DDZXBtn,0x02);
    m_Group->addButton(ui->ZCGLBtn,0x03);
    m_Group->addButton(ui->KSGLBtn,0x04);
    connect(m_Group,SIGNAL( buttonClicked(int)),this,SLOT(SetSlot(int )));
    connect(&DBSERVICES,SIGNAL(ShowMain(bool)),this,SLOT(SetMainWindow(bool)));
    m_Managerdlg = new jsj_UserManagerdlg();
    dlg = new JSJ_ddmldlg();
    m_Managerdlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    m_Managerdlg->hide();
    dlg->hide();
}

JsJ_Maindlg::~JsJ_Maindlg()
{
    delete ui;
}

void JsJ_Maindlg::SetSlot(int v)
{
    switch (v) {
    case 0x01:{
        m_Managerdlg->show();
        DBSERVICES.SetSpan();
        this->hide();
        break;
    }
    case 0x02:{
        dlg->SetCurrentIndex();
        dlg->show();
        DBSERVICES.SetSpan();
        this->hide();
        break;
    }
    }
}

void JsJ_Maindlg::on_pushButton_clicked()
{
    DBSERVICES.UpdateInfo();
    UserInfo info;
    info.UserName = DBSERVICES.LoginSQLData.SuperAdminName;;
    info.Password = DBSERVICES.LoginSQLData.SuperAdminPassWord;
    info.UserGangWei = DBSERVICES.LoginSQLData.gangweiId;
    info.UserNumber = DBSERVICES.LoginSQLData.SuperAdminName;
    DBSERVICES.AddSuperAdmin(info);
}
#include<QMessageBox>
#include"cmsgbox.h"
void JsJ_Maindlg::on_pushButton_2_clicked()
{
    if (CMsgBox::showMsgBox( "是否退出系统？") == QMessageBox::NoButton)
    {
        this->close();
    }
}
void JsJ_Maindlg::on_DDZXBtn_clicked()
{

}
void JsJ_Maindlg::SetMainWindow(bool isShow)
{
    if(isShow)
        this->show();
    else
        this->hide();
}
