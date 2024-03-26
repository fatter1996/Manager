#include "data.h"
#include "Const.h"
#include<QCoreApplication>
#include "stationmanager.h"
int HexStringToInt(QString str, int length)
{
    int   num[16] = {0};
    int   count = 1;
    int   result = 0;
    QByteArray ba = str.toLatin1();
    char *revstr = ba.data();
    for (int i = length - 1; i >= 0; i--)
    {
        if ((revstr[i] >= '0') && (revstr[i] <= '9'))
            num[i] = revstr[i] - 48; //字符0的ASCII值为48
        else if ((revstr[i] >= 'a') && (revstr[i] <= 'f'))
            num[i] = revstr[i] - 'a' + 10;
        else if ((revstr[i] >= 'A') && (revstr[i] <= 'F'))
            num[i] = revstr[i] - 'A' + 10;
        else
            num[i] = 0;
        result = result + num[i] * count;
        count = count * 16; //十六进制(如果是八进制就在这里乘以8)
    }
    return result;
}
CData::CData(QObject *parent) : QObject(parent)
{
    InitUDPConnect();
    pManager = (CStationManager *)parent;
    InitMenueAndButton("");
    readTrainTypeTXT();
}
CData::~CData()
{

}
bool compareStation(stationStr s1, stationStr s2)
{
    return s1.ChannelID < s2.ChannelID;
}
void CData::ReadStationData(int index, int Num)
{
    QString m_filename;
    QString Tips;
    if (Num == 0)
    {
        m_filename = QString("%1/data/station.txt").arg(QCoreApplication::applicationDirPath());
    }else
    {
        m_filename = QString("%1/data/station%2%3").arg(QCoreApplication::applicationDirPath())
                .arg(QString::number(Num)).arg(".txt");
    }
    QFile tFile(m_filename);
    CStationJSJ *pStation = new CStationJSJ(this);
    Connect(pStation);
    stationStr m_Station;
    m_Station.pStationData = pStation;
    if (!tFile.exists())
    {
        Tips = QString("站场文件读取失败！站%1").arg(  m_filename);
        QMessageBox::information(nullptr, "提示", Tips);
        return;
    }
    if (!tFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream tText(&tFile);
    QString  Getstring{};
    QString m_str;
    while (!tText.atEnd())
    {
        m_str = tText.readLine();
        if (m_str == "####")
        {
            break;
        }
        else if (m_str.left(5) == "##DC")
        {
            CGDDC *pGDDC = new CGDDC;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(13) == "gddc.m_nType=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setType(Getstring.toUInt());
                }
                else if (m_str.left(15) == "gddc.m_strName=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pGDDC->m_strName = Getstring;
                }
                else if (m_str.left(13) == "gddc.m_nCode=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(15) == "gddc.m_nQDCode=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setQDCode(HexStringToInt(Getstring, Getstring.length()));
                }
                else if (m_str.left(13) == "gddc.m_nCxjy=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setCxjy(Getstring.toUInt());
                }
                else if (m_str.left(15) == "gddc.m_nDSCode=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pGDDC-> = Getstring.toUInt();
                }
                else if (m_str.left(17) == "gddc.Module_Code=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pGDDC->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(17) == "gddc.m_nQDMKCode=")
                {
                    //                    Getstring=m_str.mid(17,m_str.indexOf((";"),0)-m_str.indexOf(("="),0)-1);
                    //                    pGDDC->m_nQDMKCode=HexStringToInt(Getstring);
                }
                else if (m_str.left(10) == "gddc.m_nZ=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setZ(Getstring.toUInt());
                }
                else if (m_str.left(11) == "gddc.m_nSX=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(12) == "gddc.m_nJyj=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setJyj(Getstring.toUInt());
                }
                else if (m_str.left(17) == "gddc.oneToMore=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pGDDC->m_nOneToMore = Getstring.toUInt();
                }
                else if (m_str.left(16) == "gddc.m_textRect=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(8) == "gddc.p1=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp1(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gddc.p2=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp2(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gddc.p3=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp3(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gddc.p4=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp4(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gddc.p5=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp5(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gddc.p6=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp6(StringToPoint(Getstring));
                }
                else if (m_str.left(9) == "gddc.p12=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp12(StringToPoint(Getstring));
                }
                else if (m_str.left(9) == "gddc.p34=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp34(StringToPoint(Getstring));
                }
                else if (m_str.left(9) == "gddc.p56=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setp56(StringToPoint(Getstring));

                }
                else if (m_str.left(12) == "gddc.center=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->pCenter = StringToPoint(Getstring);
                }

                else if (m_str.left(10) == "gddc.pZ12=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setpz12(StringToPoint(Getstring));

                }
                else if (m_str.left(10) == "gddc.pZ34=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setpz34(StringToPoint(Getstring));
                }
                else if (m_str.left(10) == "gddc.pZ56=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //AfxMessageBox(pGDDC->m_strName);
                    pGDDC->setpz56(StringToPoint(Getstring));
                }
                else if (m_str.left(15) == "gddc.m_bMainGD=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setIsMainGD(Getstring.toUInt());
                }
                else if (m_str.left(10) == "gddc.m_nQ=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setCQdev(Getstring.toUInt());
                }
                else if (m_str.left(10) == "gddc.m_nD=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setDWdev(Getstring.toUInt());
                }
                else if (m_str.left(10) == "gddc.m_nF=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGDDC->setFWdev(Getstring.toUInt());
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    //DCArray.Add(Getstring.toUInt());
                    pGDDC->GDDCInit(0x55);
                    pStation->DevArray.push_back(pGDDC);
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##XH")
        {
            CXHD *pXHD = new CXHD;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();

                if (m_str.left(12) == "xhd.m_nType=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_nType = Getstring.toUInt();
                }
                else if (m_str.left(14) == "xhd.m_strName=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pXHD->m_strName = Getstring;
                }
                else if (m_str.left(12) == "xhd.m_nCode=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(16) == "xhd.Module_Code=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pXHD->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(11) == "xhd.center=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->pCenter = StringToPoint(Getstring);
                }
                else if (m_str.left(15) == "xhd.m_nXHDType=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->setXHDType(Getstring);
                }
                else if (m_str.left(15) == "xhd.m_textRect=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(10) == "xhd.m_nSX=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(15) == "xhd.SignalType=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->setSignalType(Getstring.toUInt());
                }
                else if (m_str.left(17) == "xhd.D_B_C_Signal=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pXHD->D_B_C_Signal=Getstring.toUInt();
                }
                else if (m_str.left(17) == "xhd.DC_LC_Signal=")
                {
                    Getstring = m_str.mid(17, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pXHD->DC_LC_Signal=Getstring.toUInt();
                }
                else if (m_str.left(13) == "xhd.safeLamp=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->setSafeLamp(Getstring);
                }
                else if (m_str.left(11) == "xhd.isHigh=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    if (Getstring == "FALSE")
                    {
                        pXHD->setIsHigh(false);
                    }
                    else if (Getstring == "TURE")
                    {
                        pXHD->setIsHigh(true);
                    }
                }
                else if (m_str.left(11) == "xhd.isYDSD=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pXHD->setIsYDSD(Getstring.toInt());
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pXHD->XHDInit(0x55);
                    pStation->DevArray.push_back(pXHD);
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##GD")
        {
            CGD *pGD = new CGD;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(11) == "gd.m_nType=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->m_nType = Getstring.toUInt();
                }
                else if (m_str.left(13) == "gd.m_strName=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pGD->m_strName = Getstring;

                }
                else if (m_str.left(11) == "gd.m_nCode=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(15) == "gd.Module_Code=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pGD->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(16) == "gd.GD_Type=GD_QD")
                {
                    pGD->setGDType("GD_QD");
                }
                else if (m_str.left(16) == "gd.GD_Type=JJ_QD")
                {
                    pGD->setGDType("JJ_QD");
                }
                else if (m_str.left(8) == "gd.m_nZ=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setZ(Getstring.toUInt());
                }
                else if (m_str.left(9) == "gd.m_nSX=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(14) == "gd.m_textRect=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(6) == "gd.p1=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setp1(StringToPoint(Getstring));
                }
                else if (m_str.left(6) == "gd.p2=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setp2(StringToPoint(Getstring));
                }
                else if (m_str.left(6) == "gd.p3=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setp3(StringToPoint(Getstring));
                }
                else if (m_str.left(6) == "gd.p4=")
                {
                    Getstring = m_str.mid(6, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setp4(StringToPoint(Getstring));
                }
                else if (m_str.left(7) == "gd.p12=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setp12(StringToPoint(Getstring));
                }
                else if (m_str.left(7) == "gd.p34=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setp34(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gd.pz12=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setpz12(StringToPoint(Getstring));
                }
                else if (m_str.left(8) == "gd.pz34=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->setpz34(StringToPoint(Getstring));
                }
                else if (m_str.left(10) == "gd.center=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pGD->pCenter = StringToPoint(Getstring);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    //GDArray.Add(Getstring.toUInt());
                    pGD->GDInit(0x55);
                    pStation->DevArray.push_back(pGD);
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##TX")
        {
            CText *pText = new CText;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(14) == "txt.m_strName=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pText->m_strName = Getstring;
                }
                else if (m_str.left(12) == "txt.m_nSize=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pText->setTextSize(Getstring.toUInt());
                }
                else if (m_str.left(15) == "txt.m_textRect=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pText->m_textRect = StringToRect(Getstring);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pText->TextInit();
                    pStation->DevArray.push_back(pText);
                    if (pText->getTextSize() > 40)
                        m_Station.StationName = pText->getName();
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##JTX")
        {
            CJTX *pJTX = new CJTX;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(13) == "jtx.JTX_Type=")
                {
                    Getstring = m_str.mid(13, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->setJTXType(Getstring.toUInt());
                }
                else if (m_str.left(15) == "jtx.m_strName=")
                {
                    Getstring = m_str.mid(16, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pJTX->m_strName = Getstring;
                }
                else if (m_str.left(12) == "jtx.m_nType=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->m_nType = Getstring.toUInt();
                }
                else if (m_str.left(7) == "jtx.p1=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->setp1(StringToPoint(Getstring));
                }
                else if (m_str.left(7) == "jtx.p2=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->setp2(StringToPoint(Getstring));
                }
                else if (m_str.left(7) == "jtx.p3=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->setp3(StringToPoint(Getstring));
                }
                else if (m_str.left(7) == "jtx.p4=")
                {
                    Getstring = m_str.mid(7, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->setp4(StringToPoint(Getstring));
                }
                else if (m_str.left(11) == "jtx.center=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pJTX->pCenter = StringToPoint(Getstring);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pStation->DevArray.push_back(pJTX);
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##BS")
        {
            CBS *pBS = new CBS();
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(13) == "bs.m_strName=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pBS->m_strName = Getstring;
                }
                else if (m_str.left(11) == "bs.m_nCode=")
                {
                    Getstring = m_str.mid(11, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBS->m_nCode = HexStringToInt(Getstring, Getstring.length());
                }
                else if (m_str.left(11) == "bs.RelayQD=")
                {
                    Getstring = m_str.mid(12, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    //pBS->RelayQD=Getstring;
                }
                else if (m_str.left(15) == "bs.Module_Code=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->Module_Code=HexStringToInt(Getstring);
                }
                else if (m_str.left(14) == "bs.m_textRect=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBS->m_textRect = StringToRect(Getstring);
                    //AfxMessageBox((""));
                }
                else if (m_str.left(9) == "bs.m_nSX=")
                {
                    Getstring = m_str.mid(9, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBS->m_nSX = Getstring.toUInt();
                }
                else if (m_str.left(8) == "bs.isXN=")
                {
                    Getstring = m_str.mid(8, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBS->isVirtual=Getstring.toUInt();
                }
                else if (m_str.left(10) == "bs.haveJZ=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBS->isHaveJZ = Getstring.toUInt();
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pBS->initBS();
                    pStation->DevArray.push_back(pBS);
                    pStation->v_LZBSArray.push_back(pBS);
                    break;
                }
            }
        }
        else if (m_str.left(9) == "##BuRect")
        {
            CTG *pBuRect = new CTG;
            for (int j = 0;; j++)
            {
                m_str = tText.readLine();
                if (m_str.left(15) == "BuRect.m_nType=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBuRect->setType(Getstring.toUInt());
                }
                else if (m_str.left(17) == "BuRect.m_strName=")
                {
                    Getstring = m_str.mid(18, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 3);
                    pBuRect->setName(Getstring);
                }
                else if (m_str.left(15) == "BuRect.m_nCode=")
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBuRect->setCode(HexStringToInt(Getstring, Getstring.length()));
                }
                else if (m_str.left(10) == "BuRect.p1=")
                {
                    Getstring = m_str.mid(10, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBuRect->setCenterPt(StringToPoint(Getstring));
                }
                else if (m_str.left(18) == "BuRect.m_textRect=")
                {
                    Getstring = m_str.mid(18, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pBuRect->setTextRect(StringToRect(Getstring));
                    //AfxMessageBox((""));
                }
                else if (m_str.left(14) == "BuRect.m_nTZB=")
                {
                    Getstring = m_str.mid(14, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    //pBuRect->m_nTZB=HexStringToInt(Getstring);
                    //pBuRect->BuRInit();
                    //zcArray.Add(pBuRect);
                }
                else if (m_str.left(4) == "ADD_")
                {
                    Getstring = m_str.mid(4, m_str.indexOf((";"), 0) - m_str.indexOf(("_"), 0) - 1);
                    pStation->DevArray.push_back(pBuRect);
                    break;
                }
            }
        }
        else if (m_str.left(5) == "##QD")
        {
            CQD *pQD = new CQD;
            m_str = tText.readLine();
            if (m_str.left(15) == "qd.m_nChildNum=")
            {
                Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                pQD->setChildNum(Getstring.toUInt());
            }
            for (int j = 0; j < pQD->getChildNum(); j++)
            {
                m_str = tText.readLine();
                QString n_str;
                n_str = QString::fromLocal8Bit("qd.m_nChild[%1]=").arg(j);
                if (m_str.left(15) == n_str)
                {
                    Getstring = m_str.mid(15, m_str.indexOf((";"), 0) - m_str.indexOf(("="), 0) - 1);
                    pQD->setChild(j, Getstring.toInt());
                }
            }
            pStation->DevArray.push_back(pQD);
        }
    }
    tFile.close();
    m_Station.StationID = Num;
    m_Station.ChannelID = index;
    ReadLZZDBSData(m_Station.pStationData, Num);
    //AllStation.insert(index, m_Station);
    AllStation.push_back(m_Station);
    qSort(AllStation.begin(), AllStation.end(), compareStation);
}

void CData::ReadLZZDBSData(CStationJSJ *pStation, int Num)
{
    QString m_filename;
    QString xhName;
    CLZZDBS *pLZBZD = nullptr;
    CXHD *pXH = nullptr;
    int zdbsnum;
    if (Num == 0)
        m_filename = GetWorkDirRoot() + "/data/TCCData.ini";
    else
        m_filename = GetWorkDirRoot() + "/data/TCCData" + QString::number(Num) + ".ini";
    QSettings *configIniRead = new QSettings(m_filename, QSettings::IniFormat);
    configIniRead->setIniCodec("UTF-8");
    zdbsnum = configIniRead->value("ZDBSConfig/num").toInt();
    for (int i = 0; i < zdbsnum; i++)
    {
        pLZBZD = new CLZZDBS();
        pLZBZD->Name =  configIniRead->value(QString("ZDBS%1/ZDBSMNNAME").arg(i)).toString();
        xhName = pLZBZD->Name.left(pLZBZD->Name.indexOf("邻站"));
        for (int j = 0; j < pStation->DevArray.size(); j++)
        {
            if (pStation->DevArray[j]->getDevType() == Dev_XH)
            {
                pXH = (CXHD*)pStation->DevArray[j];
                if (pXH->getName() == xhName)
                {
                    pXH->setZDBSJKFlag(true);
                    break;
                }
            }
        }
        pLZBZD->BasePt = configIniRead->value(QString("ZDBS%1/ZDBSMNPOINT").arg(i)).toPoint();
        pLZBZD->m_nZDBSsx = configIniRead->value(QString("ZDBS%1/ZDBSMNSX").arg(i)).toInt();
        pLZBZD->m_nZDBSisNX = configIniRead->value(QString("ZDBS%1/ZDBSMNISNX").arg(i)).toInt();
        pLZBZD->m_nZDBSIDinLS = configIniRead->value(QString("ZDBS%1/ZDBSIDINLS").arg(i)).toInt();
        pLZBZD->InitCoordinate();
        pStation->v_LZBZDArray.push_back(pLZBZD);
        pStation->DevArray.push_back(pLZBZD);
    }
}

#include"JSJ/jsj_services.h"
void CData::InitUDPConnect()
{
    if (m_UDPSock.init( DBSERVICES.LoginSQLData.info.LocalUdpIP, DBSERVICES.LoginSQLData.info.LocalUdpPort))
    {
        connect(&m_UDPSock, SIGNAL(signal_ReceiveUDP(QByteArray, QString, unsigned short)), this, SLOT(AnalyseUdpData(QByteArray, QString, unsigned short)));
    }
    if (m_UDPTrainSock.init( DBSERVICES.LoginSQLData.info.LocalUdpIP,  DBSERVICES.LoginSQLData.info.LocalUdpTrainPort))
    {
        connect(&m_UDPTrainSock, SIGNAL(signal_ReceiveUDP(QByteArray, QString, unsigned short)), this, SLOT(AnalyseUdpTrianData(QByteArray, QString, unsigned short)));
    }
    //    if (m_TCCSock.init( DBSERVICES.LoginSQLData.info.LocalUdpIP,  DBSERVICES.LoginSQLData.info.TCCPort))
    //    {
    //        connect(&m_TCCSock, SIGNAL(signal_ReceiveUDP(QByteArray, QString, unsigned short)), this, SLOT(AnalyTCCData(QByteArray, QString, unsigned short)));
    //    }
}
void CData::InitMenueAndButton(QString FileName)
{
    pMenueButtonGroup = new QButtonGroup();
    QString path = QString("%1/data/operationMenue.ini").arg(QCoreApplication::applicationDirPath());
    QSettings  iniFile(path, QSettings::IniFormat);
    iniFile.setIniCodec("UTF-8");
    int menueNum = 0;
    MangerStr managerStr;
    QStringList strlist;
    iniFile.beginGroup("MenueNum");
    menueNum = iniFile.value(("num"), "").toInt();
    iniFile.endGroup();
    for (int i = 0; i < menueNum; i++)
    {
        managerStr.Init();
        QString str = QString("MenueData%1").arg(i);
        iniFile.beginGroup(str);
        managerStr.menueName = iniFile.value(QString::fromLocal8Bit("name"), "").toString();
        managerStr.MenueButton = new QPushButton();
        managerStr.MenueButton->setCursor(Qt::OpenHandCursor);
        pMenueButtonGroup->addButton(managerStr.MenueButton, i);
        managerStr.pSubGroup = new QButtonGroup();
        InitMenueButton(managerStr.MenueButton, managerStr.menueName);
        strlist = iniFile.value(("subButton"), "").toString().split("|");
        for (int j = 0; j < strlist.size(); j++)
        {
            QPushButton * btn = new QPushButton();
            btn->setCursor(Qt::OpenHandCursor);
            btn->setMaximumSize(161,57);
            btn->setMinimumSize(161,57);
            InitButton(btn, strlist[j]);
            managerStr.pSubGroup->addButton(btn, j);
            btn->setStyleSheet("QPushButton:hover{"
                                                       "border-image: url(:/Pictures/sub_hover.png);"
                                                       "color:rgb(0,255，255);"
                                                       "}"
                                                       "QPushButton{border-image: url(:/Pictures/sub_Normal.png);"
                                                       "color: rgb(255, 255, 255);"
                                                       "font: 16pt bold;}"
                                                       "QPushButton:pressed{border-image: url(:/Pictures/sub_pressr.png);}");
        }
        iniFile.endGroup();
        ManagerMenue.push_back(managerStr);
    }

    connect(pMenueButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(slotUpdateMenue(int)));
}

void CData::InitButton(QPushButton * Btn, QString name)
{
    Btn->setText(name);
    Btn->setStyleSheet("QPushButton{width:154px;height:56px;}"
                       "QPushButton{border-image: url(:/Pictures/button.png);}"
                       "QPushButton{color: rgb(255, 255, 255);}"
                       "QPushButton{font: 16pt bold;}"
                       );
    Btn->setMaximumSize(160, 30);
    Btn->setMinimumSize(160, 30);
}

void CData::InitMenueButton(QPushButton * Btn, QString name)
{
    Btn->setMaximumSize(161,57);
    Btn->setMinimumSize(161,57);
    Btn->setText(name);
    setButtonChecked(Btn, false);
    Btn->setStyleSheet("QPushButton:hover{"
                       "border-image: url(:/Pictures/Menu_hover.png);"
                       "color:rgb(0,255，255);"
                       "}"
                       "QPushButton{border-image: url(:/Pictures/Menu.png);"
                       "color: rgb(255, 255, 255);"
                       "font: 16pt bold;}"
                       "QPushButton:pressed{border-image: url(:/Pictures/menue_check.png);}");


}

void CData::setButtonChecked(QPushButton * Btn, bool isChecked)
{
//    if (isChecked)
//        Btn->setStyleSheet("QPushButton{width:161px;height:57px;}"
//                           "QPushButton{border-image: url(:/Pictures/menue_check.png);}"
//                           "QPushButton{color: rgb(255, 255, 255);}"
//                           "QPushButton{font: 16pt bold;}"
//                           );
//    else
//        Btn->setStyleSheet("QPushButton{width:161px;height:57px;}"
//                           "QPushButton{border-image: url(:/Pictures/Menu.png);}"
//                           "QPushButton{color: rgb(255, 255, 255);}"
//                           "QPushButton{font: 16pt bold;}"
//                           );
}

void CData::clearOtherButtonChecked(QPushButton * button)
{
    setButtonChecked(button, true);
    QVector<MangerStr>::iterator it = ManagerMenue.begin();
    for (; it < ManagerMenue.end(); it++)
    {
        if (it->MenueButton == button)
        {
            pButtonGroup =  it->pSubGroup;
            continue;
        }
        setButtonChecked(it->MenueButton, false);
    }
}

int CData::PackTrainAddMessage(unsigned char *send, int len, CStationJSJ * pStation)
{
    QByteArray array(100, 0);
    int index = 4;
    for (int i = 0; i < 4; i++)
    {
        array[i] = FRAMHEAD;
    }
    array[index++] = 0x77;
    array[index++] = 1;
    memcpy(array.data() + 7, send, len);
    len = 60;
    for (int i = 0; i < 4; i++)
    {
        array[len - i - 1] = FRAMTAIL;
    }

    SendUdpMessage(array, len, pStation);
    return len;
}

void CData::UpdateTrainInfo(int index, unsigned char type, unsigned char cxInfo, QString newTrainNum)
{
    unsigned char nCommand[1024];
    unsigned int nFrameLenth = 0;
    unsigned int nNameLenth = 0;
    unsigned int nCount = 0;
    QByteArray send;
    memset(nCommand, 0, 1024);
    nCommand[0] = 0xAF; //帧头
    nCommand[1] = 0xAF; //帧头
    nCommand[2] = 0xAF; //帧头
    nCommand[3] = 0xAF; //帧头
    if (index < AllTrain.size())
    {
        nNameLenth = AllTrain[index]->TrainNumber.length();
        //nFrameLenth = 4 + 4 + 2 + 1 + 1 + nNameLenth + 1 + 1;
        //帧头4+帧尾4+通信长度2+通信标识1+车次长1+车次内容+控车标识+控车信息

        nCommand[6] = 0x50;
        memcpy(nCommand + 7, &nNameLenth, 1);
        memcpy(&nCommand[8], AllTrain[index]->TrainNumber.toLocal8Bit().data(), nNameLenth);
        nCount = 8 + nNameLenth;
        memcpy(nCommand + nCount, &type, 1);

        nCount++;
        if (cxInfo != 0xff)
        {
            nCommand[nCount] = cxInfo;
            nCount++;
        }
        if (newTrainNum != "")
        {
            nCommand[nCount] = newTrainNum.length();
            nCount++;
            memcpy(&nCommand[nCount], newTrainNum.toLocal8Bit().data(), newTrainNum.length());
            nCount = nCount + newTrainNum.length();
        }
        nFrameLenth = nCount + 4;
        nCommand[nFrameLenth - 1] = 0xFA; //帧尾
        nCommand[nFrameLenth - 2] = 0xFA; //帧尾
        nCommand[nFrameLenth - 3] = 0xFA; //帧尾
        nCommand[nFrameLenth - 4] = 0xFA; //帧尾
        memcpy(nCommand + 4, &nFrameLenth, 2);
        send.resize(nFrameLenth);
        memcpy(send.data(), nCommand, nFrameLenth);
        if (AllTrain[index]->StationIndex < AllStation.size())
            SendUdpMessage(&m_UDPTrainSock, send, send.length(), AllStation[AllTrain[index]->StationIndex].IP_OppTrainAdress, AllStation[AllTrain[index]->StationIndex].IP_OppTrainPort);
    }
}


void CData::SendUdpMessage(QByteArray array, int len, CStationJSJ * pStation)
{
    for (QVector<stationStr>::iterator it = AllStation.begin(); it != AllStation.end(); it++)
    {
        if (pStation == it->pStationData)
        {
            m_UDPSock.send(array, it->IP_OppAdress, it->IP_OppPort, len);
            break;
        }
    }
}

void CData::SendUdpTrainMessage(QByteArray array, int len, CStationJSJ *pStation)
{
    for (QVector<stationStr>::iterator it = AllStation.begin(); it != AllStation.end(); it++)
    {
        if (pStation == it->pStationData)
        {
            m_UDPTrainSock.send(array, it->IP_OppTrainAdress, it->IP_OppTrainPort, len);
            break;
        }
    }
}

int CData::GetId_ByStation(CStationJSJ *pStation)
{
    if(pStation == nullptr)
        return -1;
    for (QVector<stationStr>::iterator it = AllStation.begin(); it != AllStation.end(); it++)
    {
        if (pStation == it->pStationData)
            return it->StationID;
        else
            continue;
    }
}
void CData::SendUdpMessage(CSocketUDP * sock, QByteArray array, int len, QString ip, unsigned short port)
{
    sock->send(array, ip, port, len);
}
void CData::readTrainTypeTXT()
{
    QString m_filename;
    QString Tips;
    m_filename = GetWorkDirRoot() + "data/TrainType.txt";
    QFile tFile(m_filename);
    if (!tFile.exists())
    {
        Tips = QString("车列类型读取失败");
        QMessageBox::information(nullptr, "提示", Tips);
        return;
    }
    if (!tFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream tText(&tFile);
    QString  Getstring{};
    QString m_str;
    while (!tText.atEnd())
    {
        m_str = tText.readLine();
        if (m_str == "####" || m_str == "")
        {
            break;
        }
        if (m_str == "####" || m_str == "")
        {
            break;
        }
        else if (m_str.left(2) == "//")
        {
            continue;
        }
        else if (m_str.indexOf("|") != -1)
        {
            Getstring = m_str;
            QStringList list = m_str.split("|");

            TrainType train;
            train.strType = list[0];
            train.strTypeName = list[1];
            v_TrainType.push_back(train);
        }
    }
    tFile.close();
}
void CData::timePro(long nElapsed)
{
    bool flag_JTGD = false;
    bool flag_DCWZ = false;
    bool flag_WCQDName = false;
    bool flag_DCQDName = false;
    bool flag_XHDName = false;
    bool flag_DCName = false;

    for (auto ement : AllStation)
    {
        ement.pStationData->timePro(nElapsed);
    }

    for (QVector<stationStr>::iterator it = AllStation.begin(); it != AllStation.end();)
    {
        it->comCount--;
        if (it->comCount == 0)
        {
            CStationJSJ* pStationData = it->pStationData;
            it =  AllStation.erase(it);
            pManager->ClearAndUpdate(pStationData);
        }
        else
            it++;
    }
    for (int i = 0; i < AllTrain.size(); i++)
    {
        if (AllTrain[i]->timeCount < 4)
            AllTrain[i]->timeCount++;
        else if (AllTrain[i]->timeCount == 4)
        {
            AllTrain[i]->timeCount++;
            if (AllTrain[i]->StationIndex < AllStation.size())
            {
                if (pManager->pMonitorDlg->row == i)
                    pManager->pMonitorDlg->clearAllView();
                AllStation[AllTrain[i]->StationIndex].pStationData->getQDName(0xffff, AllTrain[i]);
            }
            AllTrain.erase(AllTrain.begin() + i);
            i--;
        }
    }
}

void CData::Time100MS()
{
    //int tm0 = QTime::currentTime().msecsSinceStartOfDay();

    for (QVector<stationStr>::iterator it = AllStation.begin(); it != AllStation.end(); it++)
    {
        CStationJSJ* pStationData = it->pStationData;
        pStationData->TimeProc();
    }
    //    int tm1 = QTime::currentTime().msecsSinceStartOfDay();
    //    qDebug() << tm1 - tm0 << "ms\n";
}

void CData::AnalyseTrainMsg(unsigned char *rec, int channelID, int num)
{
#if 1
    int index = 7;
    bool flag = false;
    QString strTrainName = "";
    CTrain *pTrain = nullptr;
    CTrain m_Train;
    unsigned int nNameLength = 0;
    unsigned int code = 0;
    nNameLength = rec[index++];
    QByteArray temp(nNameLength, 0);
    memcpy(temp.data(), rec + index, nNameLength);
    strTrainName = temp;
    //memcpy(strTrainName.data(), rec + index, nNameLength);
    index = index + nNameLength;
    for (int i = 0; i < AllTrain.size(); i++)
    {
        if ((AllTrain[i]->TrainNumber == strTrainName) && (AllTrain[i]->ChannelIndex == channelID))
        {
            pTrain = AllTrain[i];
            flag = true;
            break;
        }
    }
    if (!flag)
    {
        pTrain = new CTrain();
    }
    if (pTrain == nullptr)
        return;
    unsigned char TrainDirection;
    unsigned char TrainType;
    pTrain->timeCount = 0;
    pTrain->TrainNumber = strTrainName;
    pTrain->ChannelIndex = channelID;
    if (rec[index] == 0xa5)
    {
        pTrain->Train_SX = "左行";
        TrainDirection = LEFT;
    }
    else if (rec[index] == 0x5a)
    {
        pTrain->Train_SX = "右行";
        TrainDirection = RIGHT;
    }
    pTrain->Train_Staion = AllStation[num].StationName;
    pTrain->StationIndex = num;
    index++;
    unsigned int m_devNum = 0; //设备编号
    unsigned int m_devNum1 = 0; //设备编号
    memcpy(&m_devNum, rec + index, 2);
    pTrain->DevCode = m_devNum;
    pTrain->Train_Position = AllStation[num].pStationData->getQDName(m_devNum, pTrain);
    index += 2;
    if (rec[index] == 0xAA || rec[index] == 0xA5)
    {
        pTrain->Train_state = true;
        pTrain->Stop_state = ("运行"); //" 停  车";
    }
    else if (rec[index] == 0x55 || rec[index] == 0x5A)
    {
        pTrain->Train_state = false;
        pTrain->Stop_state = ("停车");//" 发  车";
    }
    index += 1;
    if (rec[index] == 0x5a)
    {
        pTrain->Train_Type = "列车";
        TrainType = LCJL;
    }
    else if (rec[index] == 0x55)
    {
        pTrain->Train_Type = "调车";
        TrainType = DCJL;
    }
    index += 1;
    memcpy(&m_devNum1, rec + index, 2);
    //AllStation[num].pStationData->updateTrainPosition(m_devNum, m_devNum1);
#if 0
    if (m_devNum1 != 0xffff)
    {
        for (int i = 0; i < m; i++)
        {
            //rtc=Array_NOW[i]->GetRuntimeClass();
            rtc = ZCDATAArray[Num].ZCDATAArray[i]->GetRuntimeClass();
            if (rtc->m_lpszClassName == "CGD")
            {
                //gd=(CGDDC *)(Array_NOW[i]);
                gd = (CGDDC *)(ZCDATAArray[Num].ZCDATAArray[i]);
                if (gd->m_nCode == m_devNum1)
                {
                    gd->m_TrainDown = 0;
                    gd->m_TrainNum = "";
                    break;
                }
            }
            if (rtc->m_lpszClassName == "CGDDC")
            {
                //dc=(CGDDC *)(Array_NOW[i]);
                dc = (CGDDC *)(ZCDATAArray[Num].ZCDATAArray[i]);
                if (dc->m_nQDCode == m_devNum1)
                {
                    dc->m_TrainDown = 0;
                    dc->m_TrainNum = "";
                    break;
                }
            }
        }
    }
#endif
    index += 2;
    pTrain->nSpeed = (rec[index] | (rec[index + 1] << 8));
    index += 2;
    pTrain->nQY_Type = rec[index];
    index += 1;
    pTrain->nCX_Info = rec[index];
    index += 1;
    pTrain->nTrainNumType_Count = rec[index];
    if (!flag)
        AllTrain.push_back(pTrain);
#endif
}

void CData::AnalyseBSMsg(unsigned char *rec, int channelID, int index, int len)
{
    if (index < AllStation.size())
    {
        AllStation[index].pStationData->setBSState(rec + 9, len - 9); //自动闭塞通信
    }
}

void CData::Connect(CStationJSJ * pStation)
{
    pManager->Connect(pStation);
}

void CData::AnalyseUdpData(QByteArray array, QString IpAdress, unsigned short port)
{
    unsigned char RecvArray[2048] = {0};
    int nLength = 0;
    int IDNUM;
    if ((array.size() < 6) || (array.size() >= 2048))
    {
        return;   //防止越界
    }
    memset(RecvArray, 0, 2048);
    memcpy(RecvArray, array, array.size()); //转换数据类型
    nLength = array.size();
#if 0   //教师机协议没有帧长
    nLength = (int)(RecvArray[4] | (RecvArray[5] << 8));
    if (nLength != array.size())
    {
        qDebug() << " 接收长度与数据解析长度不符 " << nLength << array.size();
        return;
    }
#endif
    if ((RecvArray[0] == 0xFB) && (RecvArray[nLength - 1] == 0xFC)) //帧头帧尾校验成功
    {
        IDNUM = RecvArray[2];
        int index = -1;
        for (index = 0; index < AllStation.size(); index++)
        {
            if ((IDNUM - 1) == AllStation[index].ChannelID)
            {
                AllStation[index].IP_OppAdress = IpAdress;
                AllStation[index].IP_OppPort = port;
                AllStation[index].comCount = 5;
                ((CStationJSJ*)AllStation[index].pStationData)->StatusChange(RecvArray);
                break;
            }
        }
        if (index == AllStation.size())
        {
            ReadStationData(IDNUM - 1, RecvArray[3] - 1);
            AllStation[AllStation.size() - 1].IP_OppAdress = IpAdress;
            AllStation[AllStation.size() - 1].IP_OppPort = port;
            AllStation[index].comCount = 5;
            pManager->UpdateTreeData();
        }
        else
        {
        }
    }
    else if ((RecvArray[0] == 0xAF) && (RecvArray[1] == 0xAF) && (RecvArray[2] == 0xAF) && (RecvArray[3] == 0xAF)) //联锁车站列车信息帧头验证
    {
        unsigned int nlen_lz = (int)(RecvArray[4] | (RecvArray[5] << 8));
        if (nlen_lz >= 2048)
        {
            return;
        }
        if ((RecvArray[nlen_lz - 1] == 0xFA) && (RecvArray[nlen_lz - 2] == 0xFA) && (RecvArray[nlen_lz - 3] == 0xFA) && (RecvArray[nlen_lz - 4] == 0xFA)) //数据帧尾验证
        {
            int k = RecvArray[6] - 1; //通道  stationID

            int index = -1;
            for (index = 0; index < AllStation.size(); index++)
            {
                if (k == AllStation[index].ChannelID)
                {
                    AnalyseTrainMsg(RecvArray, k, index);
                    break;
                }
            }
        }
    }
}

void CData::AnalyseUdpTrianData(QByteArray array, QString IpAdress, unsigned short port)
{
    unsigned char RecvArray[2048] = {0};
    int nLength = 0;
    int IDNUM;
    if ((array.size() < 6) || (array.size() >= 2048))
    {
        return;   //防止越界
    }
    memset(RecvArray, 0, 2048);
    memcpy(RecvArray, array, array.size()); //转换数据类型
    nLength = array.size();
    if ((RecvArray[0] == 0xAF) && (RecvArray[1] == 0xAF)) //帧头帧尾校验成功
    {
        nLength = (int)(RecvArray[4] | (RecvArray[5] << 8));
        if (nLength != array.size())
        {
            qDebug() << " 接收长度与数据解析长度不符 " << nLength << array.size();
            return;
        }
        if ((RecvArray[nLength - 1] == 0xFA) && (RecvArray[nLength - 2] == 0xFA))
        {
            int k = RecvArray[6] - 1; //通道  stationID
            int index = -1;
            for (index = 0; index < AllStation.size(); index++)
            {
                if (k == AllStation[index].ChannelID)
                {
                    AllStation[index].IP_OppTrainAdress = IpAdress;
                    AllStation[index].IP_OppTrainPort = port;
                    AnalyseTrainMsg(RecvArray, k, index);
                    break;
                }
            }
        }
    }
    /************************************2020.2.12-BJT-邻站模拟配合协议扩展*************************************************/
    else if ((RecvArray[0] == 0xBF) && (RecvArray[1] == 0xBF) && (RecvArray[2] == 0xBF) && (RecvArray[3] == 0xBF)) //邻站状态数据帧头验证
    {
        unsigned int nlen_lz = RecvArray[4];
        if (nlen_lz >= 2048)
        {
            return;
        }
        int k = RecvArray[5] - 1; //通道  stationID
        int index = -1;
        for (index = 0; index < AllStation.size(); index++)
        {
            if (k == AllStation[index].ChannelID)
            {
                AllStation[index].IP_OppTrainAdress = IpAdress;
                AllStation[index].IP_OppTrainPort = port;
                AnalyseBSMsg(RecvArray, k, index, nlen_lz);
                break;
            }
        }
    }
    /************************************2020.2.12-BJT-邻站模拟配合协议扩展*************************************************/
}
void CData::AnalyTCCData(QByteArray data, QString IpAdress, unsigned short port)
{
    unsigned char RecvArray[2048] = {0};
    int nLength = 0;
    int IDNUM;
    if ((data.size() < 6) || (data.size() >= 2048))
    {
        return;   //防止越界
    }
    memset(RecvArray, 0, 2048);
    memcpy(RecvArray, data, data.size()); //转换数据类型
    nLength = data.size();
#if 0   //教师机协议没有帧长
    nLength = (int)(RecvArray[4] | (RecvArray[5] << 8));
    if (nLength != array.size())
    {
        qDebug() << " 接收长度与数据解析长度不符 " << nLength << array.size();
        return;
    }
#endif
    //    if ((RecvArray[0] == 0xEF) && (RecvArray[nLength - 1] == 0xFE)) //帧头帧尾校验成功
    //    {
    //        IDNUM = RecvArray[6];//检测发过来的心跳是否有与之匹配项
    //        int index = -1;
    //        qDebug()<<"StationId:"<<IDNUM;
    //        qDebug()<<"StationIP:"<<IpAdress;
    //        for (index = 0; index <  DBSERVICES.CurBackInfoList.size();index++)
    //        {
    //            if(IDNUM == DBSERVICES.CurBackInfoList[index].StationId)
    //            {
    //                DBSERVICES.CurBackInfoList[index].IPStr = IpAdress;
    //                DBSERVICES.CurBackInfoList[index].Port = port;
    //                break;
    //            }

    //        }
    //    }
}

void CData::TreeViewClick(const QModelIndex & index)
{
    pManager->SetCurrentStation(index.row());
}

void CData::slotUpdateMenue(int index)
{
    QPushButton *pbtn = qobject_cast<QPushButton*> (pMenueButtonGroup->button(index));
    pManager->UpdateButtonLayout(pbtn);
}

