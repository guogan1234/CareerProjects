QT += core network
QT -= gui

CONFIG += c++11

TARGET = sysmgr_device_agent
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    servers/appserverworker.cpp \
    servers/appserverworkerimpl.cpp \
    deviceInfo/devinfodata.cpp \
    deviceInfo/devinfodataferry.cpp \
    deviceInfo/devinfomanager.cpp \
    servers/collectserverworker.cpp \
    servers/collectserverworkerimpl.cpp \
    collectInfo/collectthread.cpp \
    collectInfo/collectworker.cpp \
    collectInfo/collectworkerimplwin.cpp \
    collectInfo/collectworkerimplunix.cpp \
    servers/communicateserverworker.cpp \
    servers/communicateserverworkerimpl.cpp \
    commandHandle/commandhandlethread.cpp \
    commandHandle/commandhandleworker.cpp \
    commandManager/commanddata.cpp \
    commandManager/commanddataferry.cpp \
    commandManager/commanddatamanager.cpp \
    commandHandle/commandhandleworkerimpl.cpp \
    communicate/communicatethread.cpp \
    communicate/communicatesocket.cpp \
    parser/commandmsgparser.cpp \
    parser/devinfoparser.cpp \
    conf/confdata.cpp \
    conf/confmanager.cpp \
    collectInfo/cpuusage.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    servers/appserverworker.h \
    servers/appserverworkerimpl.h \
    deviceInfo/devinfodata.h \
    deviceInfo/devinfodataferry.h \
    deviceInfo/devinfomanager.h \
    data/devinfo.h \
    servers/collectserverworker.h \
    servers/collectserverworkerimpl.h \
    collectInfo/collectthread.h \
    collectInfo/collectworker.h \
    collectInfo/collectworkerimplwin.h \
    collectInfo/collectworkerimplunix.h \
    servers/communicateserverworker.h \
    servers/communicateserverworkerimpl.h \
    commandHandle/commandhandlethread.h \
    commandHandle/commandhandleworker.h \
    data/commandmsg.h \
    commandManager/commanddata.h \
    commandManager/commanddataferry.h \
    commandManager/commanddatamanager.h \
    commandHandle/commandhandleworkerimpl.h \
    communicate/communicatethread.h \
    communicate/communicatesocket.h \
    parser/commandmsgparser.h \
    data/sendmsg.h \
    parser/devinfoparser.h \
    conf/confdata.h \
    conf/confmanager.h \
    collectInfo/cpuusage.h

DESTDIR += ./bin

target.path += $$(ISCS_SRC_HOME)/out/bin
headers.path = $$(ISCS_SRC_HOME)/out/include/plugins/hmi
headers.files =
INSTALLS += target

INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/sysmgr_api \
DEPENDPATH += $$(ISCS_SRC_HOME)/out/include/sysmgr_api

win32|win64{
    message("ISCS_SRC_HOME is $$(ISCS_SRC_HOME)")
    LIBS += ./lib/psapi.lib
}

unix{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lsysmgr_api_conf -lmxml -lcmn
}


#INSTALLS += installInc installLib installBin
