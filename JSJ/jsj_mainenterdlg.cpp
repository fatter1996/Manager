#include "jsj_mainenterdlg.h"
#include "ui_jsj_mainenterdlg.h"

jsj_MainEnterdlg::jsj_MainEnterdlg(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::jsj_MainEnterdlg)
{
    ui->setupUi(this);
}

jsj_MainEnterdlg::~jsj_MainEnterdlg()
{
    delete ui;
}
