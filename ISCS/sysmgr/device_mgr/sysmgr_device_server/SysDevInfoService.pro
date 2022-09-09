QT -= gui
QT += network

TARGET = sysmgr_device_server
CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    infoserverworker.cpp \
    infoserverworkerimpl.cpp \
    ConfHandle/confdata.cpp \
    ConfHandle/confdataferry.cpp \
    ConfHandle/confhandleworker.cpp \
    ConfHandle/confhandleworkimpl.cpp \
    Servers/baseserverworker.cpp \
    Servers/baseserverimpl.cpp \
    Servers/stationserverimpl.cpp \
    Servers/centerserverimpl.cpp \
    Communicate/threadmanager.cpp \
    Communicate/threadmanagerferry.cpp \
    Communicate/workthreadsocketferry.cpp \
    Communicate/workthread.cpp \
    Communicate/socketwriterworkerimpl.cpp \
    MsgHandle/sendmsgdata.cpp \
    MsgHandle/sendmsgdataferry.cpp \
    MsgHandle/sendmsgworker.cpp \
    MsgHandle/sendmsgworkerimpl.cpp \
    Communicate/datahandlethread.cpp \
    NetHandle/netthreadimpl.cpp \
    NetHandle/networkerimpl.cpp \
    NetHandle/netinfomanager.cpp \
    Service/commandhandlethread.cpp \
    Service/commandhandleworkerimpl.cpp \
    Service/commandmanager.cpp \
    Service/datafinder.cpp \
    Service/serviceserver.cpp \
    Service/servicethread.cpp \
    Service/socketmanager.cpp \
    Parser/devinfoparser.cpp \
    Parser/devinforequestparser.cpp \
    Parser/fulldevinfoparser.cpp \
    ConfHandle/configdata.cpp \
    ConfHandle/configmanager.cpp \
    Servers/configserverimpl.cpp \
    ConfHandle/configworkerimpl.cpp \
    Parser/alarmconfigparser.cpp

HEADERS += \
    infoserverworker.h \
    infoserverworkerimpl.h \
    ConfHandle/confdata.h \
    ConfHandle/confdataferry.h \
    ConfHandle/confhandleworker.h \
    ConfHandle/confhandleworkimpl.h \
    Data/sysconfin.h \
    Data/sysconfout.h \
    Servers/baseserverworker.h \
    Servers/baseserverimpl.h \
    Servers/stationserverimpl.h \
    Servers/centerserverimpl.h \
    Enum/enumtypes.h \
    Communicate/threadmanager.h \
    Communicate/threadmanagerferry.h \
    Communicate/workthreadsocketferry.h \
    Defines/baseserverdefine.h \
    Communicate/workthread.h \
    Communicate/socketwriterworkerimpl.h \
    MsgHandle/sendmsgdata.h \
    Data/sendmsg.h \
    MsgHandle/sendmsgdataferry.h \
    MsgHandle/sendmsgworker.h \
    MsgHandle/sendmsgworkerimpl.h \
    Communicate/datahandlethread.h \
    Data/devinfo.h \
    NetHandle/netthreadimpl.h \
    NetHandle/networkerimpl.h \
    NetHandle/netinfomanager.h \
    Data/netinfo.h \
    Service/commandhandlethread.h \
    Service/commandhandleworkerimpl.h \
    Service/commandmanager.h \
    Service/datafinder.h \
    Service/serviceserver.h \
    Service/servicethread.h \
    Service/socketmanager.h \
    Parser/devinfoparser.h \
    Parser/devinforequestparser.h \
    Parser/fulldevinfoparser.h \
    ConfHandle/configdata.h \
    ConfHandle/configmanager.h \
    Data/alarmconfig.h \
    Servers/configserverimpl.h \
    ConfHandle/configworkerimpl.h \
    Parser/alarmconfigparser.h \
    Data/keyvalue.h

DESTDIR += ./bin

installBin.path += $$(ISCS_SRC_HOME)/out/bin
installBin.files += $$DESTDIR/*
installInc.path += $$(ISCS_SRC_HOME)/out/include
installInc.files +=
installLib.path += $$(ISCS_SRC_HOME)/out/lib
installLib.files +=

unix:!macx: {
    message("this is unix")
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lsysmgr_api_conf -lmxml -lcmn -lsysmgr_server_alarm
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/sysmgr_api
    installInc.path += $$(ISCS_SRC_HOME)/out/include
    installInc.files +=
    installLib.path += $$(ISCS_SRC_HOME)/out/lib
    installLib.files +=
    installBin.path += $$(ISCS_SRC_HOME)/out/bin
    installBin.files += $$DESTDIR/*
}

win32: {
    message("this is win32")
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lsysmgr_api_conf -lmxml -lcmn -lsysmgr_server_alarm
#    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lconf_parser
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/sysmgr_api   
}
#INSTALLS += installBin
INSTALLS += installBin installInc installLib

#DEPENDPATH += /usr/local/include/conf_parser

message($${INCLUDEPATH})
message($${LIBS})
