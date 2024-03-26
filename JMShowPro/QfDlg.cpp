#include "QfDlg.h"
#include "ui_QfDlg.h"

CQfDlg::CQfDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CQfDlg)
{
    ui->setupUi(this);
}

CQfDlg::~CQfDlg()
{
    delete ui;
}
void CQfDlg::init(int funCode, QString passWork, int type)
{
    strPassWork = passWork;
    setWindowFlags(windowFlags() | (Qt::FramelessWindowHint)); //去掉边框
    setFixedSize(this->width(), this->height());                    // 禁止拖动窗口大小
    ui->label_Title->setText(getStrInitOfCode(funCode, passWork, type));
    class QValidator *validator = new QIntValidator(0, 999, this);
    ui->lineEdit_PassWork->setValidator(validator);
    ui->lineEdit_PassWork->setMaxLength(passWork.length());
    setFocus();
}
void CQfDlg::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return)
    {
        on_pushButton_OK_clicked();
    }
    else if (event->key() == Qt::Key_Enter)
    {
        on_pushButton_OK_clicked();
    }
    else if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_Cancel_clicked();
    }
    else if (event->key() == Qt::Key_Backspace)
    {
        on_pushButton_DelOne_clicked();
    }
    else if (event->key() == Qt::Key_0)
    {
        on_pushButton_0_clicked();
    }
    else if (event->key() == Qt::Key_Launch0)
    {
        on_pushButton_0_clicked();
    }
    else if (event->key() == Qt::Key_1)
    {
        on_pushButton_1_clicked();
    }
    else if (event->key() == Qt::Key_Launch1)
    {
        on_pushButton_1_clicked();
    }
    else if (event->key() == Qt::Key_2)
    {
        on_pushButton_2_clicked();
    }
    else if (event->key() == Qt::Key_Launch2)
    {
        on_pushButton_2_clicked();
    }
    else if (event->key() == Qt::Key_3)
    {
        on_pushButton_3_clicked();
    }
    else if (event->key() == Qt::Key_Launch3)
    {
        on_pushButton_3_clicked();
    }
    else if (event->key() == Qt::Key_4)
    {
        on_pushButton_4_clicked();
    }
    else if (event->key() == Qt::Key_Launch4)
    {
        on_pushButton_4_clicked();
    }
    else if (event->key() == Qt::Key_5)
    {
        on_pushButton_5_clicked();
    }
    else if (event->key() == Qt::Key_Launch5)
    {
        on_pushButton_5_clicked();
    }
    else if (event->key() == Qt::Key_6)
    {
        on_pushButton_6_clicked();
    }
    else if (event->key() == Qt::Key_Launch6)
    {
        on_pushButton_6_clicked();
    }
    else if (event->key() == Qt::Key_7)
    {
        on_pushButton_7_clicked();
    }
    else if (event->key() == Qt::Key_Launch7)
    {
        on_pushButton_7_clicked();
    }
    else if (event->key() == Qt::Key_8)
    {
        on_pushButton_8_clicked();
    }
    else if (event->key() == Qt::Key_Launch8)
    {
        on_pushButton_8_clicked();
    }
    else if (event->key() == Qt::Key_9)
    {
        on_pushButton_9_clicked();
    }
    else if (event->key() == Qt::Key_Launch9)
    {
        on_pushButton_9_clicked();
    }
}
QString CQfDlg::getStrInitOfCode(int funCode, QString passWork, int type)
{
    QString strInfo = "功能按钮";
    switch (funCode)
    {
    case Fun_PDJS:
        strInfo = "坡道解锁";
        break;
    case Fun_ZRJ:
        strInfo = "总人解";
        break;
    case Fun_QGJ:
        strInfo = "区故解";
        break;
    case Fun_SYDZS:
        strInfo = "引导总锁";
        break;
    case Fun_XYDZS:
        strInfo = "引导总锁";
        break;
    case Fun_FCZK:
        strInfo = "非常站控";
        break;
    default:
        break;
    }

    if (type == 1)
    {
        strInfo += "-";
        strInfo += passWork;
    }
    strInfo = "  " + strInfo;
    return strInfo;
}

void CQfDlg::on_pushButton_0_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "0";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_1_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "1";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_2_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "2";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_3_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "3";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_4_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "4";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_5_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "5";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_6_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "6";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_7_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "7";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_8_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "8";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_9_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    str += "9";
    ui->lineEdit_PassWork->setText(str);
}

void CQfDlg::on_pushButton_DelOne_clicked()
{
    QString str = ui->lineEdit_PassWork->text();
    QString getString = str.left(str.length() - 1);
    ui->lineEdit_PassWork->setText(getString);
}

void CQfDlg::on_pushButton_OK_clicked()
{
    if (ui->lineEdit_PassWork->text() == strPassWork)
    {
        accept();
    }
    else
    {
        QMessageBox::information(NULL, VERSION, "口令错误!");
        close();
    }
}

void CQfDlg::on_pushButton_Cancel_clicked()
{
    close();
}
