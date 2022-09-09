#-------------------------------------------------
#
# Project created by QtCreator 2018-06-28T19:32:49
#
#-------------------------------------------------

QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sysmgr_device_viewer
TEMPLATE = app

CONFIG +=c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    widget.cpp \
    ConfHandle/confdata.cpp \
    ConfHandle/confdataferry.cpp \
    ConfHandle/confhandleworker.cpp \
    ConfHandle/confhandleworkimpl.cpp \
    nodemanager.cpp \
    Servers/confserverworker.cpp \
    Servers/confserverworkerimpl.cpp \
    Servers/uiserverworker.cpp \
    Servers/uiserverworkerimpl.cpp \
    ui/nodestreemodel.cpp \
    Servers/communicateserverworker.cpp \
    Servers/communicateserverworkerimpl.cpp \
    Communicate/communicatethread.cpp \
    Communicate/communicateworkerimpl.cpp \
    Communicate/communicatesocketwriter.cpp \
    Communicate/socketmanagerimpl.cpp \
    Parser/devinforequestparser.cpp \
    Parser/devinfoparser.cpp \
    Communicate/recvdataworkerimpl.cpp \
    ui/processtablemodel.cpp \
    Parser/fulldevinfoparser.cpp \
    ui/cputablemodel.cpp \
    ui/memorytablemodel.cpp \
    ui/disktablemodel.cpp \
    ui/nettablemodel.cpp \
    Common/bytecaster.cpp \
    ConfHandle/configdata.cpp \
    ConfHandle/configmanager.cpp \
    Parser/alarmconfigparser.cpp \
    config/configworker.cpp

HEADERS += \
    widget.h \
    ConfHandle/confdata.h \
    ConfHandle/confdataferry.h \
    ConfHandle/confhandleworker.h \
    ConfHandle/confhandleworkimpl.h \
    Data/devinfo.h \
    Data/netinfo.h \
    Data/sysconfin.h \
    Data/sysconfout.h \
    nodemanager.h \
    Servers/confserverworker.h \
    Servers/confserverworkerimpl.h \
    Servers/uiserverworker.h \
    Servers/uiserverworkerimpl.h \
    ui/nodestreemodel.h \
    Servers/communicateserverworker.h \
    Servers/communicateserverworkerimpl.h \
    Communicate/communicatethread.h \
    Communicate/communicateworkerimpl.h \
    Communicate/communicatesocketwriter.h \
    Communicate/socketmanagerimpl.h \
    Data/devinforequest.h \
    Parser/devinforequestparser.h \
    Parser/devinfoparser.h \
    Communicate/recvdataworkerimpl.h \
    ui/processtablemodel.h \
    Parser/fulldevinfoparser.h \
    Data/fulldevinfo.h \
    ui/cputablemodel.h \
    ui/memorytablemodel.h \
    ui/disktablemodel.h \
    ui/nettablemodel.h \
    Common/bytecaster.h \
    ConfHandle/configdata.h \
    ConfHandle/configmanager.h \
    Parser/alarmconfigparser.h \
    config/configworker.h \
    Data/keyvalue.h

FORMS += \
        widget.ui

DESTDIR += $$PWD/bin

installBin.path += $$(ISCS_SRC_HOME)/out/bin
installBin.files += $$DESTDIR/*
installInc.path +=
installLib.path +=

INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/sysmgr_api
DEPENDPATH += $$(ISCS_SRC_HOME)/out/include/sysmgr_api

win32|win64{
    message("ISCS_SRC_HOME is $$(ISCS_SRC_HOME)")
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lsysmgr_api_conf
#    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lconf_parser
}

unix{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lsysmgr_api_conf -lmxml -lcmn
}

INSTALLS += installBin installInc installLib
