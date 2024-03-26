#include "VoiceDialogWnd.h"
#include "ui_VoiceDialogWnd.h"
#include "VoiceCall/Universal.h"
#pragma execution_character_set("utf-8")

VoiceDialogWnd::VoiceDialogWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VoiceDialogWnd)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    ui->setupUi(this);
    ui->agreeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->disAgreeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeBtn->setCursor(QCursor(Qt::PointingHandCursor));
    ui->closeBtn_2->setCursor(QCursor(Qt::PointingHandCursor));

}

VoiceDialogWnd::~VoiceDialogWnd()
{
    delete ui;
}

void VoiceDialogWnd::WaitReply()
{
    ui->agreeBtn->hide();
    ui->disAgreeBtn->hide();
    ui->closeBtn->show();
    ui->label->setText(QString("正在呼叫"));
    state = 1;
}

void VoiceDialogWnd::NewCall(QString name)
{
    ui->agreeBtn->show();
    ui->disAgreeBtn->show();
    ui->closeBtn->hide();
    ui->label->setText(QString("%1正在请求通话").arg(name));
    timeLenght = 0;
    timerId2 = startTimer(30000);
    state = 2;
}

void VoiceDialogWnd::StartChat()
{
    ui->agreeBtn->hide();
    ui->disAgreeBtn->hide();
    ui->closeBtn->show();
    timeLenght = 0;
    ui->label->setText(QString("正在通话 %1:%2:%3").arg(timeLenght / 3600).arg(timeLenght % 3600 / 60).arg(timeLenght % 60));
    timerId = startTimer(1000);
    killTimer(timerId2);
    state = 3;
}

void VoiceDialogWnd::ChatEnd()
{
    killTimer(timerId2);
    killTimer(timerId);
    ui->agreeBtn->hide();
    ui->disAgreeBtn->hide();
    ui->closeBtn->hide();
    ui->label->setText(QString("通话结束 %1:%2:%3").arg(timeLenght / 3600).arg(timeLenght % 3600 / 60).arg(timeLenght % 60));
    state = 4;
}

void VoiceDialogWnd::TimeOut()
{
    ui->agreeBtn->hide();
    ui->disAgreeBtn->hide();
    ui->closeBtn->hide();
    ui->label->setText(QString("呼叫超时"));
    state = 5;
}

void VoiceDialogWnd::DisAgree()
{
    ui->agreeBtn->hide();
    ui->disAgreeBtn->hide();
    ui->closeBtn->hide();
    ui->label->setText(QString("对方已拒绝通话"));
    state = 5;
}

void VoiceDialogWnd::timerEvent(QTimerEvent * ev)
{
    if(ev->timerId() == timerId) //通话时长
    {
        timeLenght += 1;
        ui->label->setText(QString("正在通话 %1:%2:%3").arg(timeLenght / 3600).arg(timeLenght % 3600 / 60).arg(timeLenght % 60));
    }
    if(ev->timerId() == timerId2) //呼叫时长
    {
        //超时
        killTimer(timerId2);
        emit closeBtn(timeLenght);
    }
}

void VoiceDialogWnd::on_agreeBtn_clicked()
{
    //同意
    emit agreeBtn();
}

void VoiceDialogWnd::on_disAgreeBtn_clicked()
{
    //拒绝
    emit disAgreeBtn();
}

void VoiceDialogWnd::on_closeBtn_clicked()
{
    //挂断/取消通话
    killTimer(timerId2);
    emit closeBtn(timeLenght);
}

void VoiceDialogWnd::on_closeBtn_2_clicked()
{
    if(state == 1 || state == 3)
    {
        killTimer(timerId2);
        emit closeBtn(timeLenght);
    }
    else if(state == 2)
    {
        emit disAgreeBtn();
    }
    this->close();
}
