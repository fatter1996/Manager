#include "j_logindlg.h"
#include "ui_j_logindlg.h"

J_logindlg::J_logindlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::J_logindlg)
{
    ui->setupUi(this);
}

J_logindlg::~J_logindlg()
{
    delete ui;
}
