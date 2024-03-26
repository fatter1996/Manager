#ifndef CTH_KB_H
#define CTH_KB_H
#include "JMBase.h"

class CTh_kb: public CJMBase
{
    Q_OBJECT
public:
    explicit CTh_kb();
    ~CTh_kb();

private:
    QMenu *pCom_menu;
    QAction *pCom;
    QAction *pClear;

public:
    QVector<QfListItem *>vect_QFTJArray;
    long nElapsed;  //定时器计数器

    QVector<FunBtnStr*>FunBtnArray;
    int nFunbtnDownTime;
    QPoint FunBtnPt;
    QPoint FunBtnPtConst;

    CAlarmDlg *alarmDlg;  //预警窗口
    CAlarmDlg *alarmBJ_Dlg;   //报警窗口
    CAlarmDlg *alarmState_Dlg;  //设备状态信息窗口
    CQftjDlg *qftj_dlg; //铅封统计对话框

    QPoint BJDlgPt;  //报警窗口基准点坐标位置
    QPoint YJDlgPt;   //预警窗口基准点坐标位置
    int yjDlgIsShowType;  //预警窗口隐藏显示类型  0-只隐藏预警窗口保留设备状态信息窗显示   1-设备状态信息窗跟随预警窗口一起隐藏

    bool flag_jtgd;
    bool flag_dcwz;
    bool flag_wcqdName;
    bool flag_dcqdName;
    bool flag_xhdName;
    bool flag_dcName;
public:
    void initQFTJInfo();
    void AddQFTJInfo(int qftype, int index);
    void readFunBtnData();  //读取功能按钮配置数据文件
    void draw_FunBtn(QPainter *painter, long nElapsed, double nDiploid);  //绘制功能按钮
    void draw_FCZK(QPainter *painter, long nElapsed, double nDiploid);  //绘制非常站控按钮
    int getFunCodeOfName(QString name1, QString name2);
    void setYJprBJDlgShowChange(int type);  //设置预紧或报警窗口隐藏显示状态改变  0x11-报警  0x22-预警
    void setAlarmTextADD(int type, QString str); //设置预警或报警窗口提示信息增加  0x11-报警  0x22-预警

public:
    virtual void initData(QWidget *w);//初始化
    virtual void initMenu();
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid);   //界面绘制
    virtual void Draw_DLB(QPainter *painter, long nElapsed, double nDiploid);
    virtual void readInterfaceData();//读取InterfaceData配置文件
    virtual int moveCursor(QPoint p);  //鼠标位置
    virtual bool OnLButtonDown(QPoint p);
    virtual bool OnRButtonDown(QPoint p);
    virtual void timePro(bool flag_JTGD, bool flag_DCWZ, bool flag_WCQDName, bool flag_DCQDName, bool flag_XHDName, bool flag_DCName);
    virtual void SetDLBStart(int id);
    virtual void DLBTimer();
    virtual void ANCmdOfCTC(int anFunType, int devCode1, int devCode2);
    void timePro();
public slots:
    virtual void slot_FuzhuMenuDlg_AN(int index);
    virtual void slot_YHZSBState(unsigned char yhzs_State, int stationID);
    virtual void onClearAction();
    void ClearMenueButton_Slot();
};

#endif // CTH_KB_H
