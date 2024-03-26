#ifndef JSJ_MULTIPLANDLG_H
#define JSJ_MULTIPLANDLG_H

#include"jsj_services.h"
#include"jsj_plannode.h"

#include <QMainWindow>
#include<QStandardItemModel>
#include<QFileDialog>
#include<QCoreApplication>
#include<../libxl.h>

#include"jsj_adp.h"

namespace Ui {
class jsj_MultiPlandlg;
}

class jsj_MultiPlandlg : public QMainWindow
{
    Q_OBJECT

public:
    explicit jsj_MultiPlandlg(QWidget *parent = nullptr);
    ~jsj_MultiPlandlg();
    QString Get_Add_StrInfo(int index_);
    QWidget *GetIndex_tableWidget(int index);
    void DelInfoByPlanName(QString PlanName);
private slots:
    void on_AddButton_clicked();
    void on_AddButton_2_clicked();
    void ClickedBtnSlot();
    void on_AddButton_3_clicked();
    void UpdateEditInfo(unsigned int operate);
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::jsj_MultiPlandlg *ui;
    int SelectRow = -1;
    jsj_adp * dlg;
    QStandardItemModel * PlanModel;
    QList<QStandardItem*>CurItemList;

private:
    void LoadMultiPlan(QString Path);
};

#endif // JSJ_MULTIPLANDLG_H
