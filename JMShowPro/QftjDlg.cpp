#include "QftjDlg.h"
#include "ui_QftjDlg.h"

CQftjDlg::CQftjDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CQftjDlg)
{
    ui->setupUi(this);
}

CQftjDlg::~CQftjDlg()
{
    delete ui;
}

void CQftjDlg::init()
{
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    model=new QStandardItemModel();
    model->setHorizontalHeaderLabels({"名称","A机","B机","总计"});
    ui->tableView_QFTJ->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_QFTJ->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_QFTJ->verticalHeader()->hide();
}

void CQftjDlg::slot_upDataList(QVector<QfListItem *>qftjInfo)
{
    for(int i=0;i<qftjInfo.size();i++)
    {
        model->setItem(i,0,new QStandardItem(qftjInfo[i]->strName));
        model->setItem(i,1,new QStandardItem(QString("%1").arg(qftjInfo[i]->nQfCount)));
        model->setItem(i,2,new QStandardItem("0"));
        model->setItem(i,3,new QStandardItem(QString("%1").arg(qftjInfo[i]->nQfCount)));
    }
    ui->tableView_QFTJ->setModel(model);
}
