#ifndef JSJ_JHXFDLG_H
#define JSJ_JHXFDLG_H

#include <QWidget>
#include<QStandardItemModel>
#include"jsj_services.h"
#include"QTimeEdit"
#include"jsj_txservice.h"
#define Add         0x00
#define Edit        0x01
namespace Ui {
class jsj_jhxfdlg;

}
class jsj_jhxfdlg : public QWidget
{
    Q_OBJECT

public:
    explicit jsj_jhxfdlg(QWidget *parent = nullptr);
    ~jsj_jhxfdlg();
    void ClearList();
    void sortByColumn(int column);
    void setNull(QTimeEdit*e);
    void SetBack(QColor zor);
    void SetSatisified_Info();
    void InitJh_StationInfo();
    unsigned int GetIndexByName(QString Name);
private slots:
    void on_yxlx_Combo_currentIndexChanged(int index);
    void on_jfc_TypeCombo_currentIndexChanged(int index);
    void on_AddButton_clicked();
    void on_DelBtn_clicked();
    void on_ChezhanCombo_currentTextChanged(const QString &arg1);
    void on_cancleBtn_clicked();
    void on_jcfx_combo_textActivated(const QString &arg1);
    void on_fcfx_combo_textActivated(const QString &arg1);
    void on_MLXFBtn_clicked();
    void ChangeMLState(QString s1,QString s2);
    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::jsj_jhxfdlg *ui;
    unsigned int operateType = -1;
    int index = 0;
    QList<TGInfo>CurList;
    JHINFO tempJHINFO;
    QList<QStandardItem*>CurItemList;
    QStandardItemModel * ExamDetailModel;
    QStandardItemModel * DownDetailModel;
};

#endif // JSJ_JHXFDLG_H
