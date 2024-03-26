#ifndef CJMBASE_H
#define CJMBASE_H
#include <QObject>
#include "QfDlg.h"
#include "AlarmDlg.h"
#include "QftjDlg.h"
#include "FuzhuMenuDlg.h"
#include <QtTextToSpeech/QtTextToSpeech>
class CJMBase  : public QObject
{
    Q_OBJECT
public:
    explicit CJMBase(QObject *parent = nullptr);
    QVector<DLB_Data*>vect_DLBArray;

    QPoint fczkPt;
    int fczkType;
    QRect rect_FCZK;
    int ModeType;  //0x22-非常站控   0x11-自律控制
    bool isAllowZL;  //允许自律
    bool isHaveRBtnMenu;

    int nQFDlgTitleType;
    QString strQFPassWork;
    QVector<LSD_Data>vect_LSDArray;
    QVector<Line_Data>vect_LineArray;
    QVector<Text_Data>vect_TextArray;

public:
    int nCurrentCode;
    virtual void initData(QWidget *w);
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid);   //界面绘制
    virtual void readInterfaceData();
    virtual int moveCursor(QPoint p);
    virtual bool OnLButtonDown(QPoint p);
    virtual bool OnRButtonDown(QPoint p);
    virtual void timePro(bool flag_JTGD, bool flag_DCWZ, bool flag_WCQDName, bool flag_DCQDName, bool flag_XHDName, bool flag_DCName);
    virtual void Draw_DLB(QPainter *painter, long nElapsed, double nDiploid);
    void draw_Pixmap(QPainter *painter, QString picName, int x, int y, int width, int heigh, int nDiploid); //绘制图片
    void draw_LSD(QPainter *painter, long nElapsed, double nDiploid);  //绘制零散灯
    void draw_Line(QPainter *painter, long nElapsed, double nDiploid);  //绘制线段
    void draw_Text(QPainter *painter, long nElapsed, double nDiploid);  //绘制文本
    bool isQFDlgOK(int funCode, QString passWork, int type); //铅封
    QTextToSpeech *tts;
    QVector<SpeechData *>vect_SpeechArr;
    void setSpeechText(QString str, int index);
    void speechTextPro();
    virtual void SetDLBStart(int id);
    virtual void DLBTimer();
    virtual void ANCmdOfCTC(int anFunType, int devCode1, int devCode2);
    virtual void initMenu();

signals:
    void signal_ShowJM(int type);
    void signal_upDataQFTJList(QVector<QfListItem *>qftjInfo);
    void signal_SendFunBtn(int funcode, int id);

public slots:
    virtual void slot_FuzhuMenuDlg_AN(int index);
    virtual void slot_YHZSBState(unsigned char yhzs_State, int stationID);
    virtual void onClearAction();
};

#endif // CJMBASE_H
