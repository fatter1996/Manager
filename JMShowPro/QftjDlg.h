#ifndef QFTJDLG_H
#define QFTJDLG_H

#include <QDialog>
#include "JMGlobalStructure.h"
#include "GlobalHeaders/GlobalFuntion.h"

namespace Ui {
class CQftjDlg;
}

class CQftjDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CQftjDlg(QWidget *parent = nullptr);
    ~CQftjDlg();

private:
    Ui::CQftjDlg *ui;
    QStandardItemModel *model;

public:
    void init();

public slots:
    void slot_upDataList(QVector<QfListItem *>qftjInfo);
};

#endif // QFTJDLG_H
