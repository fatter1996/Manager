QT       += core gui sql axcontainer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET =../bin/JSJ
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    arrowlabel.cpp \
    jsj_adp.cpp\
    jsj_jhxfdlg.cpp \
    jsj_multiplandlg.cpp \
    jsj_plannode.cpp \
    jsj_receiveslot.cpp \
    jsj_xsmldlg.cpp \
    cmsgbox.cpp \
    ddml_ddlg.cpp \
    gen_exceloutput.cpp \
    j_logindlg.cpp \
    jsj_addnewonedlg.cpp \
    jsj_ddmldlg.cpp \
    jsj_logindlg.cpp \
    jsj_loginlongdlg.cpp \
    jsj_maindlg.cpp \
    jsj_services.cpp \
    jsj_txservice.cpp \
    jsj_usermanagerdlg.cpp \
    jsj_xsmldlg.cpp \
    main.cpp \
    messagebox.cpp \
    mycustomedit.cpp \
    mytimeedit.cpp
HEADERS += \
    arrowlabel.h \
    jsj_adp.h\
    jsj_jhxfdlg.h \
    jsj_multiplandlg.h \
    jsj_plannode.h \
    jsj_receiveslot.h \
    jsj_xsmldlg.h \
    cmsgbox.h \
    ddml_ddlg.h \
    gen_exceloutput.h \
    j_logindlg.h \
    jsj_addnewonedlg.h \
    jsj_ddmldlg.h \
    jsj_logindlg.h \
    jsj_loginlongdlg.h \
    jsj_maindlg.h \
    jsj_services.h \
    jsj_txservice.h \
    jsj_usermanagerdlg.h \
    jsj_xsmldlg.h \
    messagebox.h \
    minidumper.h \
    mycustomedit.h \
    mytimeedit.h
FORMS += \
    ddml_ddlg.ui \
    j_logindlg.ui \
    jsj_addnewonedlg.ui \
    jsj_adp.ui \
    jsj_ddmldlg.ui \
    jsj_logindlg.ui \
    jsj_loginlongdlg.ui \
    jsj_maindlg.ui \
    jsj_jhxfdlg.ui \
    jsj_multiplandlg.ui \
    jsj_xsmldlg.ui\
    jsj_usermanagerdlg.ui
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
include(testT\testT.pri)
RESOURCES += \
    rc.qrc

DISTFILES += \
    img/调度命令/保存.png \
    img/调度命令/关闭.png \
    img/调度命令/单站阶段计划.png \
    img/调度命令/发送.png \
    img/调度命令/取消命令.png \
    img/调度命令/多站阶段计划.png \
    img/调度命令/效果图.png \
    img/调度命令/背景.png \
    img/调度命令/调度命令.png \
    img/调度命令/限速命令.png \
    rc/用户管理/上一页.png \
    rc/用户管理/下一页.png \
    rc/用户管理/使用记录.png \
    rc/用户管理/全图.png \
    rc/用户管理/删除.png \
    rc/用户管理/密码.png \
    rc/用户管理/导入.png \
    rc/用户管理/导出.png \
    rc/用户管理/新增.png \
    rc/用户管理/检索.png \
    rc/用户管理/用户名.png \
    rc/用户管理/编辑.png \
    rc/用户管理/背景.png \
    rc/用户管理/返回.png \
    rc/用户管理/重置.png
INCLUDEPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibxl64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibxl64d
else:unix: LIBS += -L$$PWD/lib/ -llibxl64


INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
