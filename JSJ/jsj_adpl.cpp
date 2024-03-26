#include "jsj_adpl.h"

jsj_adpl::jsj_adpl(QObject *parent) : QObject(parent)
{

}

jsj_adpl::~jsj_adpl()
{

}

void jsj_adpl::AddStaion(QString StationName, unsigned int Position, unsigned int index)
{

}

void jsj_adpl::Get_YindaoLine_Show(QPushButton *Btn)
{
    for(int i = 0; i < NodeAppendList.size();++i)
    {
        if(NodeAppendList[i].Btn == Btn)
        {
            if(NodeAppendList[i].isfirst_or_LastOne == UpOne)
            {
                NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png);");
                NodeAppendList[i+1].direct = Down_Ahead;
                CurDirect = Down_Ahead;
                NodeAppendList[i].GetNextlead->show();
                break;
            }
            else if (NodeAppendList[i].isfirst_or_LastOne == LastOne)
            {
                NodeAppendList[i-1].direct = Up_Ahead;
                CurDirect = Up_Ahead;
                NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                NodeAppendList[i].GetNextlead->show();
                break;
            }else
            {
                unsigned int ahead = NodeAppendList[i].direct;
                if(CurDirect == -1)//说明第一次不是从头或尾开始
                {
                    int tw = GetAhead_twards(i);
                    if(tw == -1)
                    {
                        NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向下.png);");
                        NodeAppendList[i].GetNextlead->show();
                        NodeAppendList[i+1].direct = Down_Ahead;


                        NodeAppendList[i-1].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                        NodeAppendList[i-1].GetNextlead->show();
                        NodeAppendList[i-1].direct = Up_Ahead;
                    }


                }else
                {
                    if(CurDirect == Up_Ahead)
                    {
                        NodeAppendList[i].GetNextlead->setStyleSheet("border-image: url(:/new/jsj/rc/目录/向上.png);");
                        NodeAppendList[i].GetNextlead->show();
                        NodeAppendList[i-1].direct = Up_Ahead;
                    }
                }
            }
        }
    }
}

unsigned int jsj_adpl::GetAhead_twards(int index)
{

    unsigned int UpIndex = NodeAppendList[index-1].direct;
    unsigned int DownIndex = NodeAppendList[index+1].direct;
    if(UpIndex == -1 && DownIndex == -1)
        return -1;
    else if (UpIndex!=-1)
        return UpIndex;
    else if (DownIndex!=-1)
        return DownIndex;

    return -1;
}
#include<QEvent>
bool jsj_adpl::eventFilter(QObject *target, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        int Size = group->buttons().size();
        for(int i = 0; i < Size;++i)
        {
            if(target == group->buttons()[i])
            {
                Get_YindaoLine_Show(static_cast<QPushButton*>( group->buttons()[i]));
            }
        }
    }
    return false;
}
