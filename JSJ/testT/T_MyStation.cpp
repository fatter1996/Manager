#include "T_MyStation.h"
#include "ui_T_MyStation.h"

T_MyStation::T_MyStation(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::T_MyStation)
{
    ui->setupUi(this);
}

T_MyStation::~T_MyStation()
{
    delete ui;
}
//自定义分隔函数
int T_MyStation::StringSplit(QString source, QString ch,QStringList &strarr)
{
    /*---------------------------------------------------------
    * 函数介绍： 从原字符串里按照指定的分隔字符串进行分割,将分隔的结果存放到字符串数组里
    * 输入参数：
    source -- 原字符串
    ch -- 指定的分隔字符串
    strarr -- 外部引用的一个字符串数组
    * 输出参数：
    * 返回值 ：总共分隔了多少段字符串.
    -----------------------------------------------------------*/
    QString TmpStr;
    QString TmpStr_1;
    strarr.clear();
    if(source.isEmpty() || ch.isEmpty())
        return 0;
    int len=ch.length();
    int findi=0;
    int findj=0;
    int findk=0;
    bool findn=0;
    int sum=1;

    findn=source.contains(ch);
    if (findn==true)
    {
        for (int i=0;i<source.length();i++)
        {
            if(source.left(i).right(len)==ch)
            {
//                findj=i;
//                TmpStr=source.mid(findi,findj-len);
//                strarr.append(TmpStr);
//                findi=findj;
//                sum++;
//                findk=i;
                findj=i;
                TmpStr=source.mid(findi,findj-findi-len);
//                TmpStr_1=TmpStr.mid();
                strarr.append(TmpStr);
                findi=findj;
                sum++;
            }
        }
        TmpStr=source.mid(findi,source.length());
        TmpStr=TmpStr.replace("\r\n","");
        strarr.append(TmpStr);
        return sum;
    }
    else
    {
        strarr.append(source);
        return sum;
    }


//    findn=source.contains(ch);
//    if (findn!=-1)
//    {
//        TmpStr=source.mid(0,findn);
//        //TmpStr.Trim();
//        strarr.append(TmpStr);
//        findi=findn+len;
//        sum++;
//    }
//    else
//    {
//        //source.Trim();
//        strarr.append(source);
//        sum++;
//        return sum;
//    }

//    while (findn!=-1)//有发现
//    {
//        findn=source.contains(ch);
//        if (findn!=-1)
//        {
//            TmpStr=source.mid(findi,findn-findi);
//            //TmpStr.Trim();//去除头尾空格
//            strarr.append(TmpStr);

//            findi=findn+len;
//            sum++;
//        }
//        else
//        {
//            TmpStr=source.mid(findi,source.length()-findi);
//            //TmpStr.Trim();
//            strarr.append(TmpStr);
//            sum++;

//        }
//    }

//    return sum;
}
