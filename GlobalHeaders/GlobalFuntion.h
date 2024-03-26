#ifndef GLOBALFUNTION_H
#define GLOBALFUNTION_H
#include "GlobalStructure.h"
static int StringToHex(QString SetString)
{
    return SetString.toULong(0, 16);
}
static QPoint StringToPoint(QString str)
{
    QPoint pt;
    str.trimmed(); //去除字符串两侧空白字符
    QString text = str.mid(2, str.length() - 3);
    QStringList list = text.split(",");

    pt.setX(list[0].toInt());
    pt.setY(list[1].toInt());
    return pt;
}
static QColor StringToColor(QString str)
{
    if ((str == "red") || (str == "RED") || (str == "Red"))
    {
        return Qt::red;
    }
    else if ((str == "green") || (str == "GREEN") || (str == "Green"))
    {
        return Qt::green;
    }
    else if ((str == "yellow") || (str == "YELLOW") || (str == "Yellow"))
    {
        return Qt::yellow;
    }
    else if ((str == "black") || (str == "BLACK") || (str == "Black"))
    {
        return Qt::black;
    }
    else if ((str == "white") || (str == "WHITE") || (str == "White"))
    {
        return Qt::white;
    }
    else if ((str == "blue") || (str == "BLUE") || (str == "Blue"))
    {
        return Qt::blue;
    }
    else
    {
        return Qt::white;
    }
}
static QRect StringToRect(QString str)
{
    QRect rect;
    str.trimmed(); //去除字符串两侧空白字符
    QString text = str.mid(2, str.length() - 3);
    QStringList list = text.split(",");

    rect.setLeft(list[0].toInt());
    rect.setTop(list[1].toInt());
    rect.setRight(list[2].toInt());
    rect.setBottom(list[3].toInt());
    return rect;
}

static int StringGLBToInt(QString str) //公里标字符串转换为米单位整型
{
    str.trimmed(); //去除字符串两侧空白字符
    QString text = str.mid(1, str.length() - 1);
    QStringList list = text.split("+");
    int Meters = ((list[0].toInt()) * 1000) + (list[1].toInt());
    return Meters;
}
#include<QCoreApplication>
static QString GetWorkDirRoot()
{
    QString path = QString::fromLocal8Bit("%1").arg(QCoreApplication::applicationDirPath());
    path += "/";
    return path;
}
#endif // GLOBALFUNTION_H
