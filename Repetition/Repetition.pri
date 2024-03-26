include("PullFlow/PullFlow.pri")
include("FileRecord/FileRecord.pri")
include("TCPSocket/TCPSocket.pri")
include("TextAnalysis/TextAnalysis.pri")


SOURCES += \
    $$PWD/GlobalData.cpp \
    $$PWD/RepetitionWidget.cpp \
    $$PWD/TextDisplay.cpp

HEADERS += \
    $$PWD/GlobalData.h \
    $$PWD/RepetitionWidget.h \
    $$PWD/TextDisplay.h

FORMS += \
    $$PWD/RepetitionWidget.ui \
    $$PWD/TextDisplay.ui



