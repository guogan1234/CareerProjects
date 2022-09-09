#-------------------------------------------------
#
# Project created by QtCreator 2019-03-12T11:14:29
#
#-------------------------------------------------

QT       -= gui

TARGET = sysmgr_server_alarm
TEMPLATE = lib

DEFINES += SYSMGR_SERVER_ALARM_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += sysmgr_server_alarm.cpp \
    Parser/alarmmsgparser.cpp \
    processor/alarmthread.cpp

HEADERS += sysmgr_server_alarm.h\
        sysmgr_server_alarm_global.h \
    Data/devinfo.h \
    Data/alarmconfig.h \
    Parser/alarmmsgparser.h \
    Data/alarmmsg.h \
    processor/alarmthread.h

unix {
    INCLUDEPATH += $$(ISCS_MY_HOME)/src/include/rtdb\
    $$(ISCS_MY_HOME)/src/include/rtdb_api_cli
    LIBS += -L$$(ISCS_MY_HOME)/publish/linux/redhat -lrtdb_api_cli

    target.path = /usr/lib
    INSTALLS += target
}

win32{
    INCLUDEPATH += $$(ISCS_MY_HOME)/src/include/rtdb\
    $$(ISCS_MY_HOME)/src/include/rtdb_api_cli
}
