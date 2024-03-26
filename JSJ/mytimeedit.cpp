#include "mytimeedit.h"
#include<QDebug>
MyTimeEdit::MyTimeEdit(QWidget *parent)
{

}

MyTimeEdit::MyTimeEdit(const QTime &time, QWidget *parent)
{
    setTime(time);
}

void MyTimeEdit::SetTimeByUserSelect(int Index)
{
    if(Index == ZD)
    {
        if(Types == "到达")
        {
            setReadOnly(false);
            setEnabled(true);
            //setTime(QTime::currentTime());
        }
        else if(Types == "出发")
        {
            setReadOnly(true);
            setEnabled(false);
        }
    }
    else if (Index == SF)
    {
        if(Types == "到达")
        {
            setReadOnly(true);
            setEnabled(false);

        }
        else if(Types == "出发")
        {
            setReadOnly(false);
            setEnabled(true);
            setTime(QTime::currentTime());

        }
    }
    else if (Index == TG)
    {

    }

}

void MyTimeEdit::GetType(QString Type)
{
    Types = Type;
}
