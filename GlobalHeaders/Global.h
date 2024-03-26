#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtMath>
#include <QObject>
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>
#include <QDir>
#include <QtDebug>
#include <QTimer>
#include <QSettings>
#include <QSignalMapper>
#include <QtSql/QSqlDatabase>
#include <QStandardItemModel>
#include <QLabel>
#include "QMessageBox"
#include "Windows.h"
#pragma execution_character_set("utf-8")
//软件版本号
#define VERSION  "计算机联锁监控机软件_V1.0.0"
#define COMPANY  "兰州安信铁路科技有限公司"
//方向
#define LEFT      0x5A
#define RIGHT     0xA5

//吸起落下
#define UP        0x55
#define DOWN      0xAA

//客户区坐标偏移量
#define Off_Global 0

//颜色定义
#define SkyBlue QColor(85,120,182)

//区段状态定义
#define GD_WHITE   0x0001
#define GD_BLUE    0x0002
#define GD_GREEN   0x0003
#define GD_RED     0x0004

#define GD_QD      0x000A
#define JJ_QD      0x000B
#define QD         0x000C

//信号机相关定义
#define JZ_XHJ     0x0100
#define DCJL_XHJ   0x0101
#define JZFS_XHJ   0x0102
#define SXCZ_XHJ   0x0103
#define YG_XHJ     0x0104
#define CZ_XHJ     0x0105
#define DC_XHJ     0x0106
#define DCFS_XHJ   0x0107
#define FCJL_XHJ   0x0108
#define JLFS_XHJ   0x0109
#define XHP_XHJ    0x010A

//功能按钮功能代码
#define Fun_FCZK        0x0600
#define Fun_XYDZS       0x0601
#define Fun_SYDZS       0x0602
#define Fun_ZQX         0x0603
#define Fun_ZRJ         0x0604
#define Fun_QGJ         0x0605
#define Fun_DCZD        0x0606
#define Fun_DCZF        0x0607
#define Fun_CLEAR       0x0608
#define Fun_DCDS        0x0609
#define Fun_DCDJ        0x060A
#define Fun_ANFS        0x060B
#define Fun_ANJF        0x060C
#define Fun_DCFS        0x060D
#define Fun_DCJF        0x060E
#define Fun_DD          0x060F
#define Fun_MD          0x0610
#define Fun_SDJS        0x0611
#define Fun_FZMENU      0x0612
#define Fun_MARK        0x0613
#define Fun_FLBL        0x0614
#define Fun_QRWCZY      0x0615
#define Fun_QJZJS       0x0616
#define Fun_OPENLJJC    0x0617
#define Fun_CLOSELJJC   0x0618
#define Fun_PDJS        0x0619
#define Fun_CQFLBL      0x061A
#define Fun_DWFLBL      0x061B
#define Fun_FWFLBL      0x061C
#define Fun_XHANDOWN    0x061D
#define Fun_TGANDOWN    0x061E
#define Fun_GDFS        0x061F
#define Fun_GDJF        0x0620
#define Fun_GZTZ        0x0621
#define Fun_GFAN        0x0622
#define Fun_ZFZ         0x0623
#define Fun_FCFZ        0x0624
#define Fun_JCFZ        0x0625

#define XHD_DS     0x0110
#define XHD_HD     0x0111
#define XHD_AD     0x0112
#define XHD_BD     0x0113
#define XHD_LD     0x0114
#define XHD_UD     0x0115
#define XHD_UU     0x0116
#define XHD_LL     0x0117
#define XHD_YD     0x0118
#define XHD_LU     0x0119
#define XHD_2U     0x011A
#define XHD_2L     0x011B
#define XHD_BS     0x011C
#define XHD_US     0x011D
#define XHD_LS     0x011E
#define XHD_USU    0x011F
#define XHD_MD     0x0120
#define XHD_YB     0x0121

#define LCAN       0x0126
#define DCAN       0x0127
#define YDAN       0x0128
#define TGAN       0x0129
#define GDDCAN     0x012A
#define QDAN       0x012B
#define ZDBSAN     0x012C
#define BZDBSAN    0x012D

#define ClearAllGZ 0x012F

#define XH_CZXH    0x0130
#define XH_DZXH    0x0131
#define XH_BZXH    0x0132
#define XH_JTXXH   0x0133

//设备类型相关定义
#define Dev_XH    0x0150
#define Dev_DC    0x0151
#define Dev_GD    0x0152
#define Dev_QD    0x0153
#define Dev_JTX   0x0154
#define Dev_TEXT  0x0155
#define Dev_YDQ   0x0156
#define Dev_DCQD  0x0157
#define Dev_WCQD  0x0158
#define Dev_TG    0x0159
#define Dev_ZDBS  0x015A
#define Dev_LZZDBS  0x015B
#define Dev_LZBZD  0x015C
//菜单相关定义
#define QDGZ     0x1601
#define QDZY     0x1602
#define QDFLBL   0x1603
#define DCCQFLBL 0x1604
#define DCDWFLBL 0x1605
#define DCFWFLBL 0x1606
#define DCWZ     0x1607
#define YDQGZ    0x1608
#define LEUGZ    0x1609
#define ZCRESET  0x160A

//轨道道岔相关定义
#define GDWIDTH    2
#define JYJ12      8            //岔前绝缘节
#define JYJ34      16           //定位绝缘节
#define JYJ56      32           //反位绝缘节
#define DCDW       0x0200       //定位
#define DCFW       0x0201       //反位
#define DCSK       0x0202       //四开

//文本相关定义
#define STATIONNAMETEXT   0x0300
#define DCQDNAMETEXT      0x0301
#define TEXT              0x0302

//应答器相关定义
#define YDQ_Q            0x0400
#define YDQ_FQ           0x0401
#define YDQ_DW           0x0402
#define YDQ_JZ           0x0403
#define YDQ_CZ           0x0404
#define YDQ_FCZ          0x0405
#define YDQ_FJZ          0x0406
#define YDQ_DC           0x0407
#define YDQ_DD           0x0408
#define YDQ_ZJ           0x0409
#define YDQ_RL           0x040A
#define YDQ_ZX_R         0x040B
#define YDQ_YG_R         0x040C
#define YDQ_ZC_3_2       0x040D
#define YDQ_YG_3_2       0x040E

#define YDQ_WY     0x0420
#define YDQ_YY     0x0421

//低频码相关定义
#define DMH_H    0x0500
#define DMH_HB   0x0501
#define DMH_HU   0x0502
#define DMH_UU   0x0503
#define DMH_UUS  0x0504   //双黄闪码
#define DMH_U2   0x0505   //黄2码
#define DMH_U2S  0x0506   //黄2闪码
#define DMH_U    0x0507
#define DMH_LU   0x0508
#define DMH_L    0x0509
#define DMH_L2   0x050A
#define DMH_L3   0x050B
#define DMH_L4   0x050C
#define DMH_L5   0x050D
#define DMH_JC   0x050E
#define DMH_SP   0x050F  //机车信号设备的载频锁定或切换
#define DMH_WM   0x0510

//功能按钮功能代码
#define Fun_FCZK        0x0600
#define Fun_XYDZS       0x0601
#define Fun_SYDZS       0x0602
#define Fun_ZQX         0x0603
#define Fun_ZRJ         0x0604
#define Fun_QGJ         0x0605
#define Fun_DCZD        0x0606
#define Fun_DCZF        0x0607
#define Fun_CLEAR       0x0608
#define Fun_DCDS        0x0609
#define Fun_DCDJ        0x060A
#define Fun_ANFS        0x060B
#define Fun_ANJF        0x060C
#define Fun_DCFS        0x060D
#define Fun_DCJF        0x060E
#define Fun_DD          0x060F
#define Fun_MD          0x0610
#define Fun_SDJS        0x0611
#define Fun_FZMENU      0x0612
#define Fun_MARK        0x0613
#define Fun_FLBL        0x0614
#define Fun_QRWCZY      0x0615
#define Fun_QJZJS       0x0616
#define Fun_OPENLJJC    0x0617
#define Fun_CLOSELJJC   0x0618
#define Fun_PDJS        0x0619
#define Fun_CQFLBL      0x061A
#define Fun_DWFLBL      0x061B
#define Fun_FWFLBL      0x061C
#define Fun_XHANDOWN    0x061D
#define Fun_TGANDOWN    0x061E
#define Fun_GDFS        0x061F
#define Fun_GDJF        0x0620
#define Fun_GZTZ        0x0621
#define Fun_GFAN        0x0622
#define Fun_ZFZ         0x0623
#define Fun_FCFZ        0x0624
#define Fun_JCFZ        0x0625

#define Arrow_Red       0x0701
#define Arrow_Yellow    0x0702
#define Arrow_Green     0x0703
#define Arrow_Black     0x0704
#define Arrow_White     0x0705
#define Arrow_Gray      0x0706

#define BSD_Black            0x0801
#define BSD_Red              0x0802
#define BSD_Green            0x0803
#define BSD_White            0x0804
#define BSD_Yellow           0x0805
#define BSD_Gray             0x0806
#define BSD_RedFlash         0x0807
#define BSD_GreenFlash       0x0808
#define BSD_WhiteFlash       0x0809
#define BSD_YellowFlash      0x080A

#define ARROW_LEFT     71
#define ARROW_RIGHT    72
#define FAULT_BUTTON   63
#define RECOVER_BUTTON 62
#define BLOCK_BUTTON   61

#define  STATE_YELLOW  0x01
#define  STATE_GREEN   0x02
#define  STATE_RED     0x04
#define  STATE_GRAY    0x00

#define ALL     2

#endif // GLOBAL_H
