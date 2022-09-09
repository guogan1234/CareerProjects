#-------------------------------------------------
#
# Project created by QtCreator 2019-01-03T09:17:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = SysmgrPermClient
TEMPLATE = lib

DEFINES += SYSMGRPERMCLIENT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += sysmgrpermclient.cpp \
    sysmgrperminterface.cpp \
    module/cfdbmanager.cpp \
    worker/permworkerimpl.cpp \
    manager/permuserdefdata.cpp \
    manager/permuserdefmanager.cpp \
    manager/permgroupdefdata.cpp \
    manager/permgroupdefmanager.cpp \
    manager/permsecuritydefdata.cpp \
    manager/permsecuritydefmanager.cpp \
    manager/permtypedefdata.cpp \
    manager/permtypedefmanager.cpp \
    manager/permfuncdefdata.cpp \
    manager/permfuncdefmanager.cpp \
    manager/permusergroupdefdata.cpp \
    manager/permusergroupdefmanager.cpp \
    manager/permgrouprelationdata.cpp \
    manager/permgrouprelationmanager.cpp \
    manager/permspecgroupstationdata.cpp \
    manager/permspecgroupstationmanager.cpp \
    manager/permspecgroupregiondata.cpp \
    manager/permspecgroupregionmanager.cpp

HEADERS += sysmgrpermclient.h\
        sysmgrpermclient_global.h \
    sysmgrperminterface.h \
    data/permdefine.h \
    module/cfdbmanager.h \
    worker/permworkerimpl.h \
    manager/permuserdefdata.h \
    manager/permuserdefmanager.h \
    manager/permgroupdefdata.h \
    manager/permgroupdefmanager.h \
    manager/permsecuritydefdata.h \
    manager/permsecuritydefmanager.h \
    define/sysmgrpermdefine.h \
    manager/permtypedefdata.h \
    manager/permtypedefmanager.h \
    manager/permfuncdefdata.h \
    manager/permfuncdefmanager.h \
    manager/permusergroupdefdata.h \
    manager/permusergroupdefmanager.h \
    manager/permgrouprelationdata.h \
    manager/permgrouprelationmanager.h \
    manager/permspecgroupstationdata.h \
    manager/permspecgroupstationmanager.h \
    manager/permspecgroupregiondata.h \
    manager/permspecgroupregionmanager.h \
    data/perm_define.h

win32|win64{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib/ -lcfdb_api_cmn
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
