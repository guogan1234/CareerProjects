#-------------------------------------------------
#
# Project created by QtCreator 2018-12-28T09:27:18
#
#-------------------------------------------------

QT       -= gui

TARGET = SysPermClient
TEMPLATE = lib

DEFINES += SYSPERMCLIENT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += sysperminterface.cpp \
    cfdbmanager.cpp \
    manager/permuserdefdata.cpp \
    manager/permgroupdefdata.cpp \
    manager/permuserdefmanager.cpp \
    manager/permgroupdefmanager.cpp \
    worker/permworkerimpl.cpp \
    manager/permsecuritydefdata.cpp \
    manager/permsecuritydefmanager.cpp

HEADERS += sysperminterface.h\
        syspermclient_global.h \
    cfdbmanager.h \
    data/permuserdef.h \
    manager/permuserdefdata.h \
    data/permgroupdef.h \
    manager/permgroupdefdata.h \
    manager/permuserdefmanager.h \
    manager/permgroupdefmanager.h \
    worker/permworkerimpl.h \
    data/permdefine.h \
    manager/permsecuritydefdata.h \
    manager/permsecuritydefmanager.h

win32|win64{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib/ -lcfdb_api_cmn
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
