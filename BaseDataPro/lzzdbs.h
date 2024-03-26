#ifndef CLZZDBS_H
#define CLZZDBS_H
#include "GlobalHeaders/Global.h"
#include <QObject>
#include "BaseData.h"
typedef struct button
{
    QRect AnRect;
    QString name;
    unsigned char state;
    int anTime;
    button()
    {
        name = "";
        state = DOWN;
        anTime = 0;
    }
} ButtonStr_;
class CLZZDBS : public CBaseData
{
    Q_OBJECT
public:
    explicit CLZZDBS();

    QString Name;   //自动闭塞模拟设备名称
    QPoint BasePt;       //自动闭塞模拟设备绘制基准点坐标位置
    unsigned char m_nZDBSsx;  //自动闭塞模拟设备在屏幕左侧绘制还是右侧
    unsigned char m_nZDBSisNX;  //自动闭塞虚拟设备是否是逆向
    unsigned char ZFZAN;
    unsigned char FCFZAN;
    unsigned char JCFZAN;
    unsigned char XHZCJState;
    unsigned char GFANState;//改方按钮按下状态
    unsigned char FaCheArrow;
    unsigned char FaCheEnabled;
    int FCANTime;
    unsigned FJ;
    unsigned int m_nZDBSANtime;     //按钮按下保持时间，接辅发辐的时间通过配置数据读取，如果不一致可配置修改
    unsigned int m_nZDBSIDinLS;     //通过数据配置读取联锁中对应的自动闭塞设备数组下标以实现动态匹配

    unsigned char LZFaCheArrow;//获取邻站的状态
    unsigned char LZFaCheEnabled;//邻站允许发车状态，UP为允许发车，DOWN不允许发车，作为信号开放的判断条件
    unsigned char LZFCFZANflag;//邻站发车辅助按钮按下状态
    unsigned char LZJCFZANflag;//邻站接车辅助按钮按下状态
    unsigned char LZGFANState;//邻站改方按钮按下状态
    unsigned char LZXHZCJState;//信号照查继电器状态--教师机通过模拟按钮实现状态变化后传输给联锁
    unsigned char LZQJKXState;

    QTimer *pTimer;
    QVector<ButtonStr_> v_buttonArray;
    QRect rect_ZFZ;
    QRect rect_FCFZ;
    QRect rect_JCFZ;
    QRect rect_FCJL;
    QRect rect_GF;

    QPoint ArriowPoint;
    void InitCoordinate();
    void InitState();
    void Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx);

    unsigned int getDevType();
    int moveCursor(QPoint p);
    void DealwithClicked(int index);
signals:
private slots:
    void ontimer();

};

#endif // CLZZDBS_H
