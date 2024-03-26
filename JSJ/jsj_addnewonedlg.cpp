#include "jsj_addnewonedlg.h"
#include "ui_jsj_addnewonedlg.h"
#include<QMessageBox>
#include"cmsgbox.h"
jsj_addnewOnedlg::jsj_addnewOnedlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::jsj_addnewOnedlg)
{
    ui->setupUi(this);
    ui->loginEdit->setValidator(new QRegExpValidator(QRegExp("[a-zA-Z0-9]+$")));
    ui->loginEdit->setMaxLength(18);
    ui->gongzuoNumberEdit->setMaxLength(13);
    ui->gongzuoNumberEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    bgGroup = new QButtonGroup(this);
    bgGroup->addButton( ui->XY_radioButton,3 );
    bgGroup->addButton( ui->CZ_Radio,2);
    bgGroup->addButton( ui->zd_radio,1);
    connect(bgGroup,SIGNAL(buttonClicked(int)),this,SLOT(slot_RtnGroupChanged(int)));
    ui->gangweiCombo->clear();
    ui->ChezhanCombo->clear();
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    setWindowModality(Qt::ApplicationModal);
}

jsj_addnewOnedlg::~jsj_addnewOnedlg()
{
    delete ui;
}
void jsj_addnewOnedlg::on_pushButton_2_clicked()
{
    DBSERVICES.EditInfo.InitInfo();
    DBSERVICES.EditInfo.UserName = ui->NameEdit->text();
    if(DBSERVICES.EditInfo.UserName == "")
    {

        QString str = "请检查用户名输入";
        CMsgBox::showMsgBox(str);
        return;
    }
    DBSERVICES.EditInfo.UserNumber = ui->loginEdit->text();
    if(DBSERVICES.EditInfo.UserNumber  == "")
    {
        CMsgBox::showMsgBox("请检查登录账号输入");
        return;
    }
    DBSERVICES.EditInfo.UserGangWei = ui->gangweiCombo->currentText();
    if( DBSERVICES.EditInfo.UserGangWei == "")
    {
        CMsgBox::showMsgBox("请检查用户岗位输入");
        return;
    }

    JueSe = bgGroup->checkedId();
    if(JueSe == -1)
    {
        CMsgBox::showMsgBox("请检查用户角色选择");
        return;
    }
    else{
        DBSERVICES.EditInfo.ruleName = bgGroup->button(JueSe)->text();
    }
    DBSERVICES.EditInfo.UserStation = ui->ChezhanCombo->currentText();
    if(DBSERVICES.EditInfo.UserStation == "")
    {
        CMsgBox::showMsgBox("请检查车站选择");
        return;
    }
    DBSERVICES.EditInfo.Password = ui->passWordEdit->text();
    if( DBSERVICES.EditInfo.Password == "")
    {
        CMsgBox::showMsgBox("请检查登录密码输入");
        return;
    }
    DBSERVICES.EditInfo.GzNumber = ui->gongzuoNumberEdit->text();
    if( DBSERVICES.EditInfo.GzNumber == "")
    {
        CMsgBox::showMsgBox("请检查工作账号输入");
        return;
    }
    DBSERVICES.EditInfo.UserTeam = ui->TeamCombo->currentText();
    if(DBSERVICES.EditInfo.UserTeam == "")
    {
        CMsgBox::showMsgBox("请检查班组选择");
        return;
    }
    if(OperateType == ADD)
    {

        int result = DBSERVICES.GetUserExist
                (DBSERVICES.EditInfo.UserNumber,DBSERVICES.EditInfo.GzNumber);
        if(result == -1 || result == -2)
        {
            CMsgBox::showMsgBox("请勿添加重复人员");
            return;
        }
        if(DBSERVICES.AdddNewOne(DBSERVICES.EditInfo))
        {
            CMsgBox::showMsgBox("新增用户成功!");
            DBSERVICES.GetUserInfo();
            emit DBSERVICES.UpdateTableShow();
            this->close();
        }else
        {
            CMsgBox::showMsgBox("新增用户失败!");
        }

    }else
    {
        if(DBSERVICES.UpdateUserInfo(UserKeyId))
        {
            CMsgBox::showMsgBox("用户信息更新成功!");
            DBSERVICES.GetUserInfo();
            emit DBSERVICES.UpdateTableShow();
            this->close();
        }else
        {
            CMsgBox::showMsgBox("用户信息更新失败!");
        }

    }

}

void jsj_addnewOnedlg::slot_RtnGroupChanged(int s1)
{
    JueSe = s1;
}
void jsj_addnewOnedlg::GetCheckLight(QString &Name)
{
    for(int i = 0; i <bgGroup->buttons().size();++i)
    {
        if(bgGroup->buttons()[i]->text() == Name)
            bgGroup->buttons()[i]->setChecked(true);
    }
}
void jsj_addnewOnedlg::InitInfo()
{
    DBSERVICES.GetDeptList();
    for(int i = 0; i < DBSERVICES.DeptList.size();++i)
    {
        ui->ChezhanCombo->addItem(DBSERVICES.DeptList[i].DeptName,
                                  DBSERVICES.DeptList[i].DeptId);
    }
    DBSERVICES.GetgangweiList();
    for(int i = 0; i < DBSERVICES.gangweiList.size();++i)
    {
        ui->gangweiCombo->addItem(DBSERVICES.gangweiList[i].GangWeiName,
                                  DBSERVICES.gangweiList[i].GangweiId);
    }
    if(OperateType == EDIT)
    {
        setWindowTitle("用户信息编辑");
        if(UserKeyId!=-1)
            DBSERVICES.EditInfo = DBSERVICES.GetInfoByKeyId(UserKeyId);
        ui->NameEdit->setText(DBSERVICES.EditInfo.UserName);
        ui->loginEdit->setText(DBSERVICES.EditInfo.UserNumber);
        ui->gangweiCombo->setCurrentText(DBSERVICES.EditInfo.UserGangWei);
        ui->ChezhanCombo->setCurrentText(DBSERVICES.EditInfo.UserStation);
        ui->gongzuoNumberEdit->setText(DBSERVICES.EditInfo.GzNumber);
        ui->passWordEdit->setText(DBSERVICES.EditInfo.Password);
        ui->TeamCombo->setCurrentText(DBSERVICES.EditInfo.UserTeam);
        GetCheckLight(DBSERVICES.EditInfo.ruleName);
        ui->pushButton_2->setText("确认更新");
        isFileExist(DBSERVICES.EditInfo.UserStation,DBSERVICES.EditInfo.UserTeam,
                    DBSERVICES.EditInfo.UserNumber);
        for(int i = 0 ; i < bgGroup->buttons().size();++i)
        {
            if(bgGroup->buttons()[i]->text() == DBSERVICES.EditInfo.ruleName)
                bgGroup->buttons()[i]->setChecked(true);
        }
    }else
    {
        setWindowTitle("新增用户");
        ui->pushButton_2->setText("保存");
    }
}

void jsj_addnewOnedlg::on_pushButton_3_clicked()
{

    DBSERVICES.GetUserInfo();
    emit DBSERVICES.UpdateTableShow();
    this->close();

}

void jsj_addnewOnedlg::on_ChezhanCombo_currentIndexChanged(int index)
{
}
#include<QFileInfo>
#include<QImage>
#include<QDebug>
bool jsj_addnewOnedlg::isFileExist( QString StationName,QString TeamName, QString UserName)
{
    QString path = QString("%1/img/userimg/%2/%3/%4.png")
            .arg(QCoreApplication::applicationDirPath())
            .arg(StationName)
            .arg(TeamName)
            .arg(UserName);
    qDebug()<<"路径:"<<path;
    QFileInfo fileInfo(path);
    if(fileInfo.isFile())
    {
        QImage *image = new QImage();
        image->load(path);
        ui->label_19->resize(image->width(),image->height());
        ui->label_19->setPixmap(QPixmap::fromImage(*image));
        return true;
    }
    return false;
}

void jsj_addnewOnedlg::on_ChezhanCombo_currentTextChanged(const QString &arg1)
{
    int index = ui->ChezhanCombo->currentData().toInt();
    if(index!=0)
    {
        ui->TeamCombo->clear();
        DBSERVICES.GetGroupListByStationId(index);
        for(int i = 0; i < DBSERVICES.GroupList.size();++i)
        {
            ui->TeamCombo->addItem(DBSERVICES.GroupList[i].GroupName,DBSERVICES.GroupList[i].GrouptId);
        }
    }
}
#include<QFileDialog>
void jsj_addnewOnedlg::on_pushButton_clicked()
{

    QString path = QFileDialog::getOpenFileName(
                this, "选择需要导入的图片",
                "/",
                "文本文件 (*.png);; 所有文件 (*.*);; ");
    if(path!="")
    {
        QFileInfo fileInfo(path);
        if(fileInfo.isFile())
        {
            QImage *image = new QImage();
            image->load(path);
            ui->label_19->resize(image->width(),image->height());
            ui->label_19->setPixmap(QPixmap::fromImage(*image));
        }
    }
}
