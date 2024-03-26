#include "QjlogicDlg.h"
#include "ui_QjlogicDlg.h"

CQjlogicDlg::CQjlogicDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CQjlogicDlg)
{
    ui->setupUi(this);
}

CQjlogicDlg::~CQjlogicDlg()
{
    delete ui;
}
void CQjlogicDlg::init(int type)
{
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    if(type == 0x11)
    {
        ui->label_QJDir->setText("闭塞分区");
        setWindowTitle("确认无车占用");
    }
    else if(type == 0x22)
    {
        ui->label_QJDir->setText("区间方向口");
        setWindowTitle("区间总解锁");
    }
    else if(type == 0x33)
    {
        ui->label_QJDir->setText("区间方向口");
        setWindowTitle("开启逻辑检查");
    }
    else if(type == 0x44)
    {
        ui->label_QJDir->setText("区间方向口");
        setWindowTitle("关闭逻辑检查");
    }
}
