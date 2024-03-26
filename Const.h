#ifndef CONST_H
#define CONST_H

#define FRAMHEAD 0xEF
#define FRAMTAIL 0xFE

#define  DCDWGZ      0x0011
#define  DCFWGZ      0x0012
#define  DCJCGZ      0x0013
#define  DCDLGZ      0x0014
#define  DWFLBL      0x0015
#define  FWFLBL      0x0016
#define  CQFLBL      0x0017
#define  DWFLBLHF    0x0018
#define  FWFLBLHF    0x0019
#define  CQFLBLHF    0x001a
//故障强制扳动码*/
#define  DCQZDW      0x3305
#define  DCQZFW      0x3306

#define  QDGZM       0x0021
#define  QDFLBL      0x0022
#define  QDFLBLFLAG  0x0023
#define  QDFLBLFLAGHF  0x0024

#define  DS1GZM      0x0031
#define  DS2GZM      0x0032
#define  GDTDGZ      0x0033
#define  LSTDGZ      0x3307
#define  LSTDHF      0x3308
#define  ZDBSSJGZ    0x0051  //自动闭塞双接故障
#define  ZDBSSJHF    0x0052  //自动闭塞双接恢复
#define  BZDBSGZ     0x0061  //半自动闭塞故障 20190515
#define  BZDBSHF     0x0062  //半自动闭塞恢复
#define  JZGZ       0x0063
#define  JZGZHF     0x0064
#define  JZDifferent       0x0065
#define  JZDifferentHF     0x0066

#define  DSGZM_L     0x0035
#define  DSGZM_U     0x0037
#define  DSGZM_B     0x0039

#define  DSGZM_LHF     0x0036
#define  DSGZM_UHF     0x0038
#define  DSGZM_BHF     0x003A

#define DSGZM_YB      0x003D
#define DSGZM_YBHF    0x003E

//故障取消功能码*/
#define  DCGZQX      0x3301
#define  QDGZQX      0x3302
#define  XHGZQX      0x3303
#define  GDTDQX      0x3304
/*故障强制扳动码*/
#define  DCQZDW      0x3305
#define  DCQZFW      0x3306
/**/
//各设备的故障信息
#define attr_XH_JZDS      0x01
#define attr_XH_DSL       0x02
#define attr_XH_DSU       0x04
#define attr_XH_DSB       0x08
#define attr_XH_DSYB      0x10
#define attr_XH_SJGZ      0x20
#define attr_XH_BZDGZ     0x40
#define attr_XH_JZGZ      0x80
#define attr_XH_JZBYZ     0x100

#define attr_DC_DWGZ      0x01
#define attr_DC_FWGZ      0x02
#define attr_DC_JCGZ      0x04
#define attr_DC_DLGZ      0x08
#define attr_DC_QDGZ      0x10
#define attr_DC_FLBLGZ    0x20
#define attr_DC_DWFLBL_   0x40
#define attr_DC_FWFLBL_   0x80
#define attr_DC_CQFLBL_   0x100

#define attr_QD_QDGZ      0x200
#define attr_QD_FLBLGZ    0x400
#define attr_QD_FLBL_     0x800
#endif // CONST_H
