#include "MarkDlg.h"
#include "ui_MarkDlg.h"

CMarkDlg::CMarkDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CMarkDlg)
{
    ui->setupUi(this);
}

CMarkDlg::~CMarkDlg()
{
    delete ui;
}
void CMarkDlg::init(int zcCount, int devCount)
{
    nZCCount = zcCount;
    nDevCount = devCount;
    nfocus = 0;
    setWindowFlags(windowFlags() | (Qt::FramelessWindowHint)); //去掉边框
    setFixedSize(this->width(), this->height());                    // 禁止拖动窗口大小
    setFocus();
    myMapper = new QSignalMapper(this);
    QPushButton *button[53] = {ui->pushButton_0, ui->pushButton_1, ui->pushButton_2, ui->pushButton_3, ui->pushButton_4, ui->pushButton_5, ui->pushButton_6, ui->pushButton_7, ui->pushButton_8, ui->pushButton_9,
                               ui->pushButton_A, ui->pushButton_B, ui->pushButton_C, ui->pushButton_D, ui->pushButton_E, ui->pushButton_F, ui->pushButton_G, ui->pushButton_H, ui->pushButton_I, ui->pushButton_J,
                               ui->pushButton_K, ui->pushButton_L, ui->pushButton_M, ui->pushButton_N, ui->pushButton_O, ui->pushButton_P, ui->pushButton_Q, ui->pushButton_R, ui->pushButton_S, ui->pushButton_T,
                               ui->pushButton_U, ui->pushButton_V, ui->pushButton_W, ui->pushButton_X, ui->pushButton_Y, ui->pushButton_Z, ui->pushButton_DaYu, ui->pushButton_XiaoYu, ui->pushButton_JiaHao, ui->pushButton_JianHao,
                               ui->pushButton_KongGe, ui->pushButton_Feng, ui->pushButton_Chao, ui->pushButton_Ting, ui->pushButton_Dian, ui->pushButton_Ji, ui->pushButton_Zhan, ui->pushButton_Xian, ui->pushButton_Che, ui->pushButton_Jin,
                               ui->pushButton_Fang, ui->pushButton_Tuo, ui->pushButton_Jian
                              };

    for (int i = 0; i < 53; i++)
    {
        button_Input[i] = button[i];
    }

    for (int i = 0; i < 53; i++)
    {
        connect(button_Input[i], SIGNAL(clicked(bool)), myMapper, SLOT(map()));
        myMapper->setMapping(button_Input[i], i);
    }

    connect(myMapper, SIGNAL(mapped(int)), this, SLOT(getText(int)));

    ui->lineEdit->setMaxLength(2);
    ui->lineEdit_2->setMaxLength(8);

    pTime = new QTimer();
    pTime->setInterval(500);
    connect(pTime, SIGNAL(timeout()), this, SLOT(slot_onTimeOut()));
    pTime->start();
}
void CMarkDlg::getText(int i)
{
    if (i >= 53)
    {
        return;
    }
    QString strMark1 = "";
    QString strMark2 = "";
    strMark1 = ui->lineEdit->text();
    strMark2 = ui->lineEdit_2->text();
    if (i <= 39)
    {
        strMark2 += button_Input[i]->text();
        nfocus = 2;
    }
    else if (i == 40)
    {
        strMark2 += " ";
        nfocus = 2;
    }
    else if ((i >= 41) && (i <= 52))
    {
        strMark1 += button_Input[i]->text();
        nfocus = 1;
    }
    ui->lineEdit->setText(strMark1);
    ui->lineEdit_2->setText(strMark2);
}
void CMarkDlg::keyPressEvent(QKeyEvent *event)
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
        on_pushButton_TuiGe_clicked();
    }
}

void CMarkDlg::on_pushButton_TuiGe_clicked()
{
    if (nfocus == 1)
    {
        QString str = ui->lineEdit->text();
        QString getString = str.left(str.length() - 1);
        ui->lineEdit->setText(getString);
    }
    else if (nfocus == 2)
    {
        QString str = ui->lineEdit_2->text();
        QString getString = str.left(str.length() - 1);
        ui->lineEdit_2->setText(getString);
    }
}

void CMarkDlg::on_pushButton_QingKong_clicked()
{
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
}

void CMarkDlg::on_pushButton_Cancel_clicked()
{
    close();
}

void CMarkDlg::on_pushButton_OK_clicked()
{
    signal_MarkDlgOK(nZCCount, nDevCount, ui->lineEdit->text(), ui->lineEdit_2->text());
    accept();
}
void CMarkDlg::slot_onTimeOut()
{
    if (ui->lineEdit->hasFocus())
    {
        nfocus = 1;
    }
    else if (ui->lineEdit_2->hasFocus())
    {
        nfocus = 2;
    }
    if (nfocus == 1)
    {
        ui->lineEdit->setFocus();
    }
    else if (nfocus == 2)
    {
        ui->lineEdit_2->setFocus();
    }
}
