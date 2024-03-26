#ifndef TGLOBAL_H
#define TGLOBAL_H
#include<QMessageBox>
//颜色定义
#define WHITE      qRgb(255, 255, 255)
#define BLUE       qRgb(50, 50, 255)
#define GREEN      qRgb(12, 255, 12)
#define QFYS       qRgb(61, 169, 150)
#define AGREEN     qRgb(12, 150, 12)
#define YELLOW     qRgb(255, 255, 0)
#define BLACK      qRgb(0, 0, 0)
#define JDQD      qRgb(22, 133, 180)// 交大界面区段的颜色
#define cscQD      qRgb(11, 122, 244)// casco界面区段的颜色
#define KBQD      qRgb(85, 120, 182)// kb界面区段的颜色
#define PINK       qRgb(240, 32, 209)// casco界面粉红色
#define THPINK     qRgb(188, 67, 180)// 高铁界面粉红色
#define RED        qRgb(255, 12, 12)
#define THIN_WHITE qRgb(120, 120, 120)//(0,128,192/*134,191,255/*120, 120, 120*/)
#define GRAY       qRgb(120, 120, 120)
#define THIN_GRAY  qRgb(239, 239, 239)
#define THIN_BLUE  qRgb(0,225,255)
#define DEEP_BLUE  qRgb(45,64,128)
#define SWHITE     qRgb(192,192,192) //银白色
#define LGRAY      qRgb(239, 239, 239)//浅灰色
#define QDColour_casco      qRgb(32, 102, 97)
#define SelectColour_casco      qRgb(98, 240, 219)

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
#define XHD_2U     0x0120
#define XHD_BS     0x0121
#define XHD_US     0x0122
#define XHD_LS     0x0123
#define XHD_USU    0x0124
#define XHD_MD     0x0125

#define XH_CZXH    0x0130
#define XH_DZXH    0x0131
#define XH_BZXH    0x0132
#define XH_JTXXH   0x0133

//轨道道岔相关定义
#define GDWIDTH    3
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

#define JKJ   0
#define LSGCS 1
#define CTC   2
#define JCJ   3
#endif // TGLOBAL_H


