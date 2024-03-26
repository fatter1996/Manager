#include "treeitem.h"
#include "basestructure.h"
#include <QFont>
#include <QColor>

TreeItem::TreeItem(TreeItem *parent)
    : _parent(parent),
      _type(UNKNOWN),
      _ptr(nullptr),
      _row(-1)
{
}

TreeItem::~TreeItem()
{
    removeChildren();
}

// 在本节点下添加子节点
void TreeItem::addChild(TreeItem *item)
{
    item->setRow(_children.size());
    _children.append(item);
}

// 清空所有子节点
void TreeItem::removeChildren()
{
    qDeleteAll(_children);
    _children.clear();
}

// 获取本节点第column列的数据
QVariant TreeItem::data(int column) const
{
    if(_type == PROVINCE) // 此节点是"省份"信息
    {
        if (column == COLUMN_NAME) // 省份，只有"name"字段，且显示在第一列
        {
            Province* p = (Province*)_ptr;
            return p->name;
        }
    }
    else if(_type == PERSON) // 此节点是"人口"信息
    {

    }
    else if(_type == DEVTYPE) // 此节点是"省份"信息
    {
        if (column == COLUMN_NAME) // 省份，只有"name"字段，且显示在第一列
        {
            zoneType* p = (zoneType*)_ptr;
            return p->Typename;
        }
    }
    else if(_type == DEVNAME) // 此节点是"省份"信息
    {
        zonedev* p = (zonedev*)_ptr;
        switch (column)
        {
        case COLUMN_NAME:       return p->Devname;
        case COLUMN_Oper:        return p->OperStr;
        case COLUMN_State:        return p->StateStr;
        default:                return QVariant();
        }
    }
    return QVariant();
}

QVariant TreeItem::getFont(int column)
{
    if (column == COLUMN_NAME)
    {
        QFont f;
        f.setBold(true);
        return f;
    }
    return QVariant();
}

QVariant TreeItem::getColor(int column)
{
    if (column == COLUMN_Oper)
    {
        return QColor(Qt::red);
    }
    else if (column == COLUMN_State)
    {
        return QColor(Qt::green);
    }
    return QVariant();
}

QVariant TreeItem::getBackground(int column)
{
    if (row() % 2 == 0) // 偶数行
    {
        return QColor(Qt::lightGray);
    }
    return QVariant();
}

QVariant TreeItem::getTextAlign(int column)
{
    if (column == COLUMN_Oper)
    {
        return QVariant(Qt::AlignCenter);
    }
    else if (column == COLUMN_State) {
        return QVariant(Qt::AlignVCenter | Qt::AlignRight);
    }
    return QVariant();
}
