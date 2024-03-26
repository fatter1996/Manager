#include "stationdirection.h"
#include "ui_stationdirection.h"

StationDirection::StationDirection(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StationDirection)
{
    ui->setupUi(this);
    this->setWindowTitle(QString("方向显示设置 - TDCS/CTC3.0 通用车站操作终端"));
    this->setWindowFlags(Qt::Dialog);
    this->move(550,250);
}

StationDirection::~StationDirection()
{
    delete ui;
}

void StationDirection::on_pushButton_6_clicked()
{
    hide();
}
