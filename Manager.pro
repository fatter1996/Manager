QT       += core gui network texttospeech concurrent multimedia axcontainer sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += resources_big
CONFIG += c++11
TARGET =../bin/ManagerV1.0.0
RC_ICONS = icon.ico
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(GlobalHeaders/GlobalHeaders.pri)
include(SocketPro/SocketPro.pri)
include(BaseDataPro/BaseDataPro.pri)
include(JMShowPro/JMShowPro.pri)
include(VoiceCall/VoiceCall.pri)
include(Repetition/Repetition.pri)
include(JSJ/jsj.pri)
include(testT/testT.pri)
SOURCES += \
    data.cpp \
    main.cpp \
    mainwindow.cpp \
    stationdraw.cpp \
    stationmanager.cpp

HEADERS += \
    Const.h \
    data.h \
    mainwindow.h \
    stationdraw.h \
    stationmanager.h

FORMS += \
    mainwindow.ui \
    stationdraw.ui \
    stationmanager.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibxl64
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibxl64d
else:unix: LIBS += -L$$PWD/lib/ -llibxl64

INCLUDEPATH += $$PWD/include/ffmpeg \
               $$PWD/include/sdl
DEPENDPATH += $$PWD/include/ffmpeg \
              $$PWD/include/sdl

LIBS += $$PWD/lib/ffmpeg/avdevice.lib \
        $$PWD/lib/ffmpeg/avfilter.lib \
        $$PWD/lib/ffmpeg/avformat.lib \
        $$PWD/lib/ffmpeg/avutil.lib \
        $$PWD/lib/ffmpeg/avcodec.lib \
        $$PWD/lib/ffmpeg/postproc.lib \
        $$PWD/lib/ffmpeg/swresample.lib \
        $$PWD/lib/ffmpeg/swscale.lib \
        $$PWD/lib/sdl/SDL2.lib

DISTFILES +=

RESOURCES += \
    image.qrc
