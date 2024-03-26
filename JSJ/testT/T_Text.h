#ifndef T_Text_H
#define T_Text_H

#pragma execution_character_set("utf-8")

#include <QPaintEvent>
#include <QPainter>
#include <QtMath>
class T_Text
{
public:
    T_Text();

    //成员变量声明
public:
    QRectF m_textRect;          //轨道区段文本区域
    QRectF m_textRectConst;     //预留站场缩放数据存储结构
    QString m_strName;        //文本内容
    QString m_strDCName;         //关联道岔名称
    unsigned int m_nQDNode;      //关联道岔设备编号
    unsigned int m_nSize;     //文本大小
    QColor textColor;       //文本显示颜色
    unsigned int m_nTextType;
    int m_nStationID;

    bool IsQDTextShowFlag;

    //成员变量封装函数声明
public:
    void setTextName(QString name);
    QString getTextName();
    void setGLDCName(QString name);
    QString getGLDCName();
    void setGLQDNode(unsigned int node);
    unsigned int getGLQDNode();
    void setTextSize(unsigned int size);
    unsigned int getTextSize();
    void setTextColor(QString strColor);
    QColor getTextColor();
    void setTextType(QString type);
    unsigned int getTextType();
    void setStationID(int id);
    int getStationID();
    void setTextRect(QRect rect);

    //成员功能函数声明
public:
    void TextInit(); //初始化
    void Draw(QPainter *painter, unsigned int nElapsed, double nDiploid);
};

#endif // T_Text_H
