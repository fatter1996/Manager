#ifndef JSJ_DDMLDLG_H
#define JSJ_DDMLDLG_H

#include <QMainWindow>
#include<QHeaderView>
#include<QStandardItemModel>
#include<QTableWidget>
#include<QMenu>
#include<QAction>
#include"jsj_txservice.h"
#include"jsj_xsmldlg.h"
#include"ddml_ddlg.h"
#include"jsj_jhxfdlg.h"
#include"QMouseEvent"
#include<QFontMetrics>
#include"cmsgbox.h"
#include"jsj_multiplandlg.h"
namespace Ui {
class JSJ_ddmldlg;
}

class JSJ_ddmldlg : public QMainWindow
{
    Q_OBJECT

protected:
    bool eventFilter(QObject *obj, QEvent *e);
public:
    explicit JSJ_ddmldlg(QWidget *parent = nullptr);
    ~JSJ_ddmldlg();
public:
    void SetSendedInfo(int type);
    void ShowIndex(int index);
    void SetCurTime();
    void SetCurrentIndex();
    QString GetItemInfo(int index);
private slots:
    void on_ml_CloseBtn_2_clicked();

    void on_ml_SendBtn_2_clicked();
    void changeTest(int row, int col);
    void on_ml_SaveBtn_2_clicked();
    void UpdateAllDDML(int type);
    void ChangeIndexSlot(int index);
    void InitTabelInfo();
    void ClearState();
    void slot_DL_RcOnTw(QPoint pos);
    void slotActionInsert();
    void on_ml_showTable_cellClicked(int row, int column);
    void ShoMainT();
    void on_tabWidget_currentChanged(int index);
    void SetCurStatus(QString Number);
private:
    Ui::JSJ_ddmldlg *ui;
    int Position;
    unsigned char isSet = 0x11;
    QString Number;
    QMenu * m_tabMenu;
    jsj_MultiPlandlg * multiPlandlg;
    QAction * action;
    QAction * Cancle_action;
    jsj_xsmldlg *m_xsmldlg;
    ddml_ddlg * m_ddmldlg;
    jsj_jhxfdlg * m_jhxfdlg;
    QStringList QSR_List;
private:
    void InitTable_Info();
private slots:
    void slotCellEnter(int r, int c);
    void slotCellEnter_Send(int r, int c);
    void SetcheckedStation(QString station);
    void UpdateStationInfo();
    void on_cancle_Btn_clicked();
};

#endif // JSJ_DDMLDLG_H
