#include "trainmonitordlg.h"
#include "ui_trainmonitordlg.h"
#include "GlobalHeaders/Global.h"
#include "data.h"
#include <QGraphicsOpacityEffect>
CTrainMonitorDlg::CTrainMonitorDlg(QWidget *parent, CData *p) :
    QDialog(parent),
    ui(new Ui::CTrainMonitorDlg)
{
    pData = p;
    ui->setupUi(this);

    float opacity = 0.85; // 淡入的时候用
    //float opacitx = 1.0; //淡出的时候用
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(); // 透明函数
    setWindowOpacity(opacity); //设置透明等级
    //setWindowOpacity(opacity); //设置透明等级
    //this->setGraphicsEffect(effect);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    //setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

    setFixedSize(this->width(), this->height());

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    standItemModel = new QStandardItemModel();
    standItemModel->setColumnCount(10);
    standItemModel->setRowCount(200);
    standItemModel->setHeaderData(0, Qt::Horizontal, "序");  //设置表头内容
    standItemModel->setHeaderData(1, Qt::Horizontal, "车次");
    standItemModel->setHeaderData(2, Qt::Horizontal, "方向");
    standItemModel->setHeaderData(3, Qt::Horizontal, "位置");
    standItemModel->setHeaderData(4, Qt::Horizontal, "车站");
    standItemModel->setHeaderData(5, Qt::Horizontal, "机车属性");
    standItemModel->setHeaderData(6, Qt::Horizontal, "运行状态");
    standItemModel->setHeaderData(7, Qt::Horizontal, "牵引类型");
    standItemModel->setHeaderData(8, Qt::Horizontal, "超限信息");
    standItemModel->setHeaderData(9, Qt::Horizontal, "列车类型");

    //隐藏列表头
    ui->tableView->verticalHeader()->hide();
    //ui->tableView->setStyleSheet("QTableView::item{text-align:center;}");
    ui->tableView->setStyleSheet("QTableView { \
                                border: 1px solid gray; \
                                font-size: 14px; \
                            } \
                            QTableView::item { \
                                padding: 6px; \
                                text-align: center; \
                            }");
    //ui->tableView->horizontalHeader()->setHighlightSections(false); 选中正行
//    QString strHeaderQss;
//    strHeaderQss = ("QHeaderView::section { background:green; color:white;min-height:3em;}");
//    ui->tableView->setStyleSheet(strHeaderQss);
//    ui->tableView->setFont(QFont("黑体, 19, QFont::Bold)"));
    //挂载表格模型
    ui->tableView->setModel(standItemModel);
    ui->tableView->verticalHeader()->setDefaultSectionSize(15);

    ui->tableView->setColumnWidth(0, 20);
    ui->tableView->setColumnWidth(1, 70);
    ui->tableView->setColumnWidth(2, 68);
    ui->tableView->setColumnWidth(3, 70);
    ui->tableView->setColumnWidth(4, 70);
    ui->tableView->setColumnWidth(5, 70);
    ui->tableView->setColumnWidth(6, 70);
    ui->tableView->setColumnWidth(7, 70);
    ui->tableView->setColumnWidth(8, 70);
    ui->tableView->setColumnWidth(9, 120);

    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(UpdateTrainMsg()));
    updateTimer->start(1000);//开启刷新计时器
#if 0
    if (pData)
    {
        for (int i = 0; i < pData->v_TrainType.size(); i++)
        {
            ui->comboBox->addItem(pData->v_TrainType[i].strTypeName);
        }
    }
#endif
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(onSliderValueChanged(int)));
    //ui->lineEdit_2->setEnabled(false);
#if 0
    ui->lineEdit_station->setEnabled(false);
    ui->lineEdit_position->setEnabled(false);
#endif

    row = 9999;
    clearAllView();
    ui->label_14->setText("");

    InitMenue();
    connect(ui->tableView, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(slot_tableViewMenu(const QPoint &)));
    setGeometry(160, 730, this->width(), this->height());
}

CTrainMonitorDlg::~CTrainMonitorDlg()
{
    delete ui;
}

void CTrainMonitorDlg::clearAllView()
{
    row = 9999;
    ui->lineEdit->setText("");
#if 0
    ui->lineEdit_station->setText("");
    ui->lineEdit_position->setText("");

    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    ui->lcButton->setEnabled(false);
    ui->dcButton->setEnabled(false);
    ui->fcButton->setEnabled(false);
    ui->sfcButton->setEnabled(false);
    ui->tcButton->setEnabled(false);
    ui->dlButton->setEnabled(false);
    ui->fdlButton->setEnabled(false);
#endif
    ui->pushButton->setEnabled(false);
    ui->zhaiguaButton->setEnabled(false);
    ui->lianguaButton->setEnabled(false);
    ui->speedButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    ui->pushButton->setEnabled(false);
}

void CTrainMonitorDlg::InitMenue()
{
    DirectionAction = new QAction("左行", this);
    DirectionAction->setData(0);

    TypeAction = new QAction("调车", this);
    TypeAction->setData(0);

    FCAction = new QAction("发车", this);
    FCAction->setData(0);
    SFCAction = new QAction("手发车", this);
    SFCAction->setData(1);

    DLAction = new QAction("电力", this);
    DLAction->setData(0);

    CXAction[0] = new QAction("无超限", this);
    CXAction[1] = new QAction("一级超限", this);
    CXAction[2] = new QAction("二级超限", this);
    CXAction[3] = new QAction("三级超限", this);
    CXAction[4] = new QAction("超级超限", this);
    for (int i = 0; i < 5; i++)
    {
        CXAction[i]->setData(i);
    }
}
void CTrainMonitorDlg::UpdateTrainMsg()
{
    CTrain *pTrain;
    int num = pData->AllTrain.size();
    standItemModel->removeRows(0, standItemModel->rowCount()); // 删除所有行
    ui->tableView->viewport()->update();
    QString CXInfo;
    for (int i = 0; i < num; ++i)
    {
        pTrain = pData->AllTrain[i];
        CXInfo = "";
        switch (pTrain->nCX_Info)
        {
        case 0:
            CXInfo = "无超限";
            break;
        case 1:
            CXInfo = "一级超限";
            break;
        case 2:
            CXInfo = "二级超限";
            break;
        case 3:
            CXInfo = "三级超限";
            break;
        case 4:
            CXInfo = "超级超限";
            break;
        default:
            break;
        }

        standItemModel->setItem(i, 0, new QStandardItem(QString::number(i + 1)));
        standItemModel->setItem(i, 1, new QStandardItem((pTrain->TrainNumber == pTrain->Train_Position) ? "" : pTrain->TrainNumber));
        standItemModel->setItem(i, 2, new QStandardItem(pTrain->Train_SX));
        standItemModel->setItem(i, 3, new QStandardItem(pTrain->Train_Position));
        standItemModel->setItem(i, 4, new QStandardItem(pTrain->Train_Staion));
        standItemModel->setItem(i, 5, new QStandardItem(pTrain->Train_Type));
        standItemModel->setItem(i, 6, new QStandardItem(pTrain->Stop_state));
        standItemModel->setItem(i, 7, new QStandardItem(pTrain->nQY_Type == 1 ? "电力" : "非电力"));
        standItemModel->setItem(i, 8, new QStandardItem(CXInfo));
        if (pTrain->nTrainNumType_Count < pData->v_TrainType.size())
            standItemModel->setItem(i, 9, new QStandardItem(pData->v_TrainType[pTrain->nTrainNumType_Count].strTypeName));
        for (int j = 0; j < 10; j++)
        {
            standItemModel->item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
        if (pTrain->Stop_state == "停车")
        {
            for (int j = 0; j < 10; j++)
            {
                standItemModel->item(i, j)->setForeground(QBrush(QColor(Qt::red)));
            }
        }
        else
        {
            for (int j = 0; j < 10; j++)
            {
                standItemModel->item(i, j)->setForeground(QBrush(QColor(Qt::green)));
            }
        }
    }
    ui->tableView->setModel(standItemModel);
    if (row < num)
        ui->tableView->selectRow(row);
}


void CTrainMonitorDlg::on_tableView_clicked(const QModelIndex &index)
{
    CTrain *pTrain;
    row = index.row();
    if (row < pData->AllTrain.size())
    {
//        ui->lineEdit_2->setEnabled(true);
        ui->zhaiguaButton->setEnabled(true);
        ui->lianguaButton->setEnabled(true);
        ui->speedButton->setEnabled(true);
        ui->delButton->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushButton->setEnabled(true);
//        ui->sfcButton->setEnabled(true);

        pTrain = pData->AllTrain[row];
        ui->lineEdit->setText(pTrain->TrainNumber == pTrain->Train_Position ? "" : pTrain->TrainNumber);
#if 0
        if (pTrain->Train_SX == "左行")
        {
            ui->leftButton->setEnabled(false);
            ui->rightButton->setEnabled(true);
        }
        else if (pTrain->Train_SX == "右行")
        {
            ui->rightButton->setEnabled(false);
            ui->leftButton->setEnabled(true);
        }
        if (pTrain->Train_Type == "列车")
        {
            ui->lcButton->setEnabled(false);
            ui->dcButton->setEnabled(true);
        }
        else if (pTrain->Train_Type == "调车")
        {
            ui->dcButton->setEnabled(false);
            ui->lcButton->setEnabled(true);
        }
        if (pTrain->Stop_state == "停车")
        {
            ui->tcButton->setEnabled(false);
            ui->fcButton->setEnabled(true);
        }
        else if (pTrain->Stop_state == "运行")
        {
            ui->fcButton->setEnabled(false);
            ui->tcButton->setEnabled(true);
        }
        if (pTrain->nQY_Type == 0)
        {
            ui->dlButton->setEnabled(true);
            ui->fdlButton->setEnabled(false);
        }
        else if (pTrain->nQY_Type == 1)
        {
            ui->fdlButton->setEnabled(true);
            ui->dlButton->setEnabled(false);
        }
#endif
        int speed = 110 - pTrain->nSpeed;
        if (speed >= -1 && speed <= 150)
        {
            ui->horizontalSlider->setValue(speed);
            ui->label_14->setText(QString::number(50 + 2 * speed) + "km/s");
        }
        else if (speed == 254) //旧联锁帧尾标识0xFE
        {
            ui->horizontalSlider->setValue(0);
        }
    }
}

void CTrainMonitorDlg::on_cxButton_clicked()
{
#if 0
    int CXInfo = -1;
    if (ui->radioButton->isChecked())
        CXInfo = 0;
    else if (ui->radioButton_2->isChecked())
        CXInfo = 1;
    else if (ui->radioButton_3->isChecked())
        CXInfo = 2;
    else if (ui->radioButton_4->isChecked())
        CXInfo = 3;
    else if (ui->radioButton_5->isChecked())
        CXInfo = 4;
    if (row < pData->AllTrain.size())
    {
        if (CXInfo != -1 && CXInfo != pData->AllTrain[row]->nCX_Info)
        {
            pData->UpdateTrainInfo(row, 0x8B, CXInfo);
        }
    }
#endif
}

void CTrainMonitorDlg::on_delButton_clicked()
{
    if (row < pData->AllTrain.size())
    {
        QMessageBox::StandardButton result = (QMessageBox::information(nullptr, "警告", "确定删除车次" + pData->AllTrain[row]->TrainNumber + "吗?", QMessageBox::Ok | QMessageBox::No));
        if (result == QMessageBox::Ok)
        {
            pData->UpdateTrainInfo(row, 0x44);
            clearAllView();
        }
    }
}

void CTrainMonitorDlg::on_pushButton_clicked()
{
#if 1
    QString newTrainNum = ui->lineEdit->text();
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0xA9, 0xff, newTrainNum);
        clearAllView();
    }
#endif
}

void CTrainMonitorDlg::on_loseButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x77);
    }
#endif
}

void CTrainMonitorDlg::on_dlButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x88);
        ui->dlButton->setEnabled(false);
        ui->fdlButton->setEnabled(true);
    }
#endif
}

void CTrainMonitorDlg::on_fdlButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x89);
        ui->fdlButton->setEnabled(false);
        ui->dlButton->setEnabled(true);
    }
#endif
}

void CTrainMonitorDlg::on_typeButton_clicked()
{
#if 0
    int index = ui->comboBox->currentIndex();
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x8A, index);
    }
#endif
}

void CTrainMonitorDlg::on_leftButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x55);
        ui->leftButton->setEnabled(false);
        ui->rightButton->setEnabled(true);
    }
#endif
}

void CTrainMonitorDlg::on_rightButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x55);
        ui->leftButton->setEnabled(true);
        ui->rightButton->setEnabled(false);
    }
#endif
}

void CTrainMonitorDlg::on_lcButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x66);
        ui->lcButton->setEnabled(false);
        ui->dcButton->setEnabled(true);
    }
#endif
}

void CTrainMonitorDlg::on_dcButton_clicked()
{
#if 0
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x66);
        ui->dcButton->setEnabled(false);
        ui->lcButton->setEnabled(true);
    }
#endif
}

void CTrainMonitorDlg::on_sfcButton_clicked()
{
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x33);
    }
}

void CTrainMonitorDlg::onSliderValueChanged(int value)
{
    ui->label_14->setText(QString::number(50 + value * 2) + "km/s");
}

void CTrainMonitorDlg::on_speedButton_clicked()
{
    if (row < pData->AllTrain.size())
    {
        pData->UpdateTrainInfo(row, 0x99, ui->horizontalSlider->value());
    }
}

void CTrainMonitorDlg::on_zhaiguaButton_clicked()
{
    CTrain* pTrain = nullptr;
    if (row < pData->AllTrain.size())
    {
        pTrain = pData->AllTrain[row];
        CTrainAddDlg TrainAdd(this, pData, pTrain->DevCode, pData->AllStation[pTrain->StationIndex].pStationData);
        TrainAdd.exec();
        if (TrainAdd.addFlag)
            pData->UpdateTrainInfo(row, 0xA9, 0xff, pTrain->Train_Position);
    }
    clearAllView();
}

void CTrainMonitorDlg::on_lianguaButton_clicked()
{
    CTrain *pTrain = nullptr;
    if (row < pData->AllTrain.size() && pData->AllTrain[row]->TrainNumber != pData->AllTrain[row]->Train_Position)
    {
        for (int i = 0; i < pData->AllTrain.size(); i++)
        {
            pTrain = pData->AllTrain[i];
            if (i != row  && pTrain->Train_Position == pData->AllTrain[row]->Train_Position
                    && pTrain->Train_Position == pTrain->TrainNumber)
            {
                pData->UpdateTrainInfo(i, 0x44);
                break;
            }
        }

    }
    clearAllView();
}

void CTrainMonitorDlg::slot_tableViewMenu(QPoint pt)
{
    auto index = ui->tableView->indexAt(pt);
    QMenu menu;
    QAction *action = nullptr;
    if (index.isValid())
    {
        ModifyAttMenue(index.column());
    }
}

void CTrainMonitorDlg::on_tableView_pressed(const QModelIndex &index)
{
    on_tableView_clicked(index);
}
void CTrainMonitorDlg::ModifyAttMenue(int column)
{
    QMenu menu;
    QAction *action = nullptr;
    int ID;
    if (row < pData->AllTrain.size())
    {
        QStandardItem *item = standItemModel->item(row, column);
        if (column == 2)
        {
            if (item->text() == "右行")
                DirectionAction->setText("左行");
            else if (item->text() == "左行")
                DirectionAction->setText("右行");
            menu.addAction(DirectionAction);
            action = menu.exec(QCursor::pos());
            if (action != nullptr)
            {
                ID = action->data().toInt();
                if (ID == 0)
                {
                    pData->UpdateTrainInfo(row, 0x55);
                }
            }
        }
        else  if (column == 5)
        {
            if (item->text() == "列车")
                TypeAction->setText("调车");
            else if (item->text() == "调车")
                TypeAction->setText("列车");
            menu.addAction(TypeAction);
            action = menu.exec(QCursor::pos());
            if (action != nullptr)
            {
                ID = action->data().toInt();
                if (ID == 0)
                {
                    pData->UpdateTrainInfo(row, 0x66);
                }
            }
        }
        else  if (column == 6)
        {
            if (item->text() == "停车")
                FCAction->setText("发车");
            else if (item->text() == "运行")
                FCAction->setText("停车");
            menu.addAction(FCAction);
            menu.addAction(SFCAction);
            action = menu.exec(QCursor::pos());
            if (action != nullptr)
            {
                ID = action->data().toInt();
                if (ID == 0)
                {
                    pData->UpdateTrainInfo(row, 0x22);
                }
                else if (ID == 1)
                {
                    pData->UpdateTrainInfo(row, 0x33);
                }
            }
        }
        else  if (column == 7)
        {
            if (item->text() == "电力")
                DLAction->setText("非电力");
            else if (item->text() == "非电力")
                DLAction->setText("电力");
            menu.addAction(DLAction);
            action = menu.exec(QCursor::pos());
            if (action != nullptr)
            {
                ID = action->data().toInt();
                if (ID == 0)
                {
                    if (DLAction->text() == "电力")
                        pData->UpdateTrainInfo(row, 0x88);
                    else if (DLAction->text() == "非电力")
                        pData->UpdateTrainInfo(row, 0x89);
                }
            }
        }
        else  if (column == 8)
        {
            for (int i = 0; i < 5; i++)
            {
                menu.addAction(CXAction[i]);
            }
            action = menu.exec(QCursor::pos());
            if (action != nullptr)
            {
                ID = action->data().toInt();
                pData->UpdateTrainInfo(row, 0x8B, ID);
            }
        }
    }
}

void CTrainMonitorDlg::MoveWindow()
{
    setGeometry(160, 730, this->width(), this->height());
}

void CTrainMonitorDlg::on_fcButton_clicked()
{

}
