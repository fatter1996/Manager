#ifndef CBASEDATA_H
#define CBASEDATA_H
#include "GlobalHeaders/GlobalFuntion.h"
class CBaseData : public QObject
{
    Q_OBJECT
public:
    explicit CBaseData(QObject *parent = nullptr);

//封装成员变量
public:
    QPointF pCenter;            //设备中心点
    QPointF pCenterConst;       //缩放及偏移使用
    unsigned int m_nType;       //设备类型
    QString m_strName;          //设备名称
    unsigned int m_nSX;         //上下行咽喉
    unsigned int m_nCode;       //设备编号
    QRectF m_textRect;          //设备名称文本区域
    QRectF m_textRectConst;     //设备名称文本区域
    unsigned int m_nStationID;           //所属站ID
    unsigned int m_nBelongToTCC;         //所属TCC ID
    unsigned int m_nBelongToRBC;         //所属RBC ID
    bool isDisPlayName;         //是否显示名称
    bool m_bToolTip;            //是否显示鼠标提示信息
    QString m_strToolTip;       //鼠标提示信息

    int GZType = 0;

//封装成员变量接口函数
public:
    void setGZType(int gzAttr);
    bool getGZType(int gzAttr);
    void clearGZType(int gzAttr);
    void clearAllGZ();
    void setCenterPt(QPointF pt);  //设置设备中心点
    QPointF getCenterPt();         //获取设备中心点
    void setType(unsigned int nType);   //设置设备类型
    unsigned int getType();             //获取设备类型
    void setName(QString name);         //设置设备名称
    QString getName();                  //获取设备名称
    void setSX(unsigned int nSX);       //设置上下行咽喉
    unsigned int getSX();               //获取上下行咽喉
    void setCode(unsigned int code);    //设置设备编号
    unsigned int getCode();             //获取设备编号
    void setTextRect(QRectF rect);      //设置设备名称文本区域
    QRectF getTextRect();               //获取设备名称文本区域
    void setStationID(unsigned int id); //设置所属站ID
    unsigned int getStationID();        //获取所属站ID
    void setBelongToTCC(unsigned int id);    //设置所属TCC ID
    unsigned int getBelongToTCC();           //获取所属TCC ID
    void setBelongToRBC(unsigned int id);    //设置所属RBC ID
    unsigned int getBelongToRBC();           //获取所属RBC ID
    void setDisplayName(bool nFlag);    //设置是否显示名称
    bool getDisplayName();              //获取是否显示名称
    void setToolTipFlag(bool flag);     //设置是否显示鼠标提示信息
    bool getToolTipFlag();              //获取是否显示鼠标提示信息
    void setToolTipStr(QString str);    //设置鼠标提示信息
    QString getToolTipStr();            //获取鼠标提示信息

//封装虚函数用于实现多态
public:
    virtual void Draw_ToolTip(QPainter *painter, double nDiploid);
    virtual void Draw(QPainter *painter, long nElapsed, double nDiploid, QPoint offset, int type, bool zczx);
    virtual void setDevStateToSafe();
    virtual unsigned int getDevType();
    virtual int moveCursor(QPoint p);
    virtual void setVollover(QPoint pt_Base);

    void draw_Pixmap(QPainter *painter, QString picName, int x, int y, int width, int heigh, int nDiploid); //绘制图片
};

#endif // CBASEDATA_H
