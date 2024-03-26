#include "trainadddlg.h"
#include "ui_trainadddlg.h"
#include "data.h"
CTrainAddDlg::CTrainAddDlg(QWidget *parent, CData *data, unsigned int code, CStationJSJ *p):
    QDialog(parent),
    ui(new Ui::CTrainAddDlg)
{
    ui->setupUi(this);
    pData = data;
    devCode = code;
    pStation = p;
    addFlag = false;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    if (pData)
    {
        for (int i = 0; i < pData->v_TrainType.size(); i++)
        {
            ui->comboBox_6->addItem(pData->v_TrainType[i].strTypeName);
        }
    }
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
}

CTrainAddDlg::~CTrainAddDlg()
{
    delete ui;
}


void CTrainAddDlg::on_pushButton_clicked()
{
    int index = 0;
    unsigned char cCommand[60] = {0};

    QString Trainnum = ui->lineEdit->text();
    int lenth = Trainnum.length();
    memcpy(cCommand + index, &lenth, 1);
    index++;
    memcpy(cCommand + index, Trainnum.toLatin1(), lenth);
    index = index + lenth;
    if (Trainnum == "")
    {
        QMessageBox::information(nullptr, "提示", "请输入车次号！");
        return;
    }
    QString str = ui->comboBox->currentText();
    if (str == "调车")
        cCommand[index++] = 0x11;
    else
        cCommand[index++] = 0x22;
    str = ui->comboBox_3->currentText();
    if (str == "8组编组")
        cCommand[index++] = 0x02;
    else if (str == "16组编组")
        cCommand[index++] = 0x03;
    str = ui->comboBox_2->currentText();
    if (str == "左行")
        cCommand[index++] = 0xa5;
    else if (str == "右行")
        cCommand[index++] = 0x5a;
    int time;
    time = ui->horizontalSlider->value();
    //time = 50 + (ui->horizontalSlider->value()) * 2;
    memcpy(cCommand + index, &time, 2);
    if (devCode == 0xffff)
        return;
    index += 2;
    memcpy(cCommand + index, &devCode, 2);
    index += 2;
    str = ui->comboBox_4->currentText();
    if (str == "电力")
        cCommand[index++] = 0x01;
    else
        cCommand[index++] = 0x00;
    str = ui->comboBox_5->currentText();
    if (str == "无超限")
        cCommand[index++] = 0x00;
    else if (str == "一级超限")
        cCommand[index++] = 0x01;
    else if (str == "二级超限")
        cCommand[index++] = 0x02;
    else if (str == "三级超限")
        cCommand[index++] = 0x03;
    else if (str == "超级超限")
        cCommand[index++] = 0x04;

    int typeIndex = ui->comboBox_5->currentIndex();
    cCommand[index++] = typeIndex;
    if (pData && pStation)
        pData->PackTrainAddMessage(cCommand, index, pStation);
    addFlag = true;
    close();
}

void CTrainAddDlg::onSliderValueChanged(int value)
{
    ui->label_11->setText(QString::number(50 + value * 2) + "km/s");
}

void CTrainAddDlg::on_pushButton_2_clicked()
{
    close();
}
