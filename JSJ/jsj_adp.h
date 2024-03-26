#ifndef JSJ_ADP_H
#define JSJ_ADP_H

#include <QDialog>
#include<QPushButton>
#include<QLabel>
#include<QButtonGroup>
#include<QComboBox>
#include"arrowlabel.h"
#include"jsj_services.h"
#include"testT/comfirm_btn.h"
#include"mytimeedit.h"
using namespace std;
class jsj_planNode;
struct GetNodeAppend{
    unsigned int isfirst_or_LastOne;//判断是第一个还是最后一个
    Comfirm_Btn * Btn;//当前所填写阶段计划的站场按钮
    ArrowLabel * GetNextlead;//获取下一站场阶段计划的站场按钮
    int LeadIndex = -1;//箭头布局位置
    QHBoxLayout * NextHboxLayout;//获取下一站场阶段计划的组
    int direct = -1;//来自上方或下方的信息 0x02 往下走 0x01 往上走
    bool isGet = false;
};

namespace Ui {
class jsj_adp;
}
class jsj_adp : public QDialog
{
    Q_OBJECT

public:
    explicit jsj_adp( QSize&Size, QWidget *parent = nullptr);
    ~jsj_adp();
    void AddStaion(QString StationName,unsigned int Position,unsigned int index);
    void Get_YindaoLine_Show(QPushButton * Btn);
    int GetFirst_Clicked_Btn(QPushButton * Btn);
    void Get_ArrowShow( int Arrow_Forward,int var, Comfirm_Btn * Btn);
    unsigned int GetAhead_twards(int index);
    void SetInit_Info(unsigned int type);
    void EditExistPlan();
    void AddExist_Station(PalenInfo&Plae,int index,bool isEnd);
    void UpdateExistInfo();
    void Insert_Append_PlanInfo();
    void GetInsertLay( QVBoxLayout * formLayout,PalenInfo&tempPale);
    void SetOtherInfoAppend(QHBoxLayout *lay,PalenInfo&tempPale);
    void GetTimeAppend(int OperateCmd);
    void SetTimeDetailInfo(int var,PalenInfo&tempPale);
    void GetTimeInit(PalenInfo &tempPale);
    bool Bool_ExistSameOne(QString PlanName);
    void GetOtherInfo(QHBoxLayout *lay, QComboBox *box_);
    unsigned int GetIndexByName_Multi(QString Name);//获取TG接发车类型下  设备的索引
    QList<Jc_XHJ> GetItem(QString arg1);
    void SetSatisified_Info_multiPlan(QString StationName);
    QString GetTypeByLayout(QHBoxLayout *lay);
public:
    jsj_planNode * Node;
    int CurDirect = -1;//当前方向(用户在进行操作时 是否之前已经选取了计划的走向)
private:
    Ui::jsj_adp *ui;
    int TypeOpreate;
    int TakeFirst_One = -1;//第一个点击确定的按钮

    QVBoxLayout * layout;
    QButtonGroup * group;
    QList<GetNodeAppend>NodeAppendList;
     QList<TGInfo>CurList;
protected:
    bool eventFilter(QObject *target, QEvent *event);
private slots:
    void on_pushButton_clicked();
    void getPlanType(int index);
     void getPlanType_JCFX(int index);
};

#endif // JSJ_ADP_H
