#-------------------------------------------------
#
# Project created by QtCreator 2018-08-22T17:22:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AlarmParaConfig
TEMPLATE = app

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
        configdialog.cpp \
    translateservice.cpp \
    configstruct.cpp \
    cfdbservice.cpp \
    colorcopydialog.cpp \
    logindialog.cpp \
    commnstruct.cpp

HEADERS += \
        configdialog.h \
    translateservice.h \
    configstruct.h \
    cfdbservice.h \
    colorcopydialog.h \
    logindialog.h \
    commnstruct.h

FORMS += \
        configdialog.ui \
    colorcopydialog.ui \
    logindialog.ui

INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api_cmn
DEPENDPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api_cmn

INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/parson
DEPENDPATH += $$(ISCS_SRC_HOME)/out/include/parson

target.path += $$(ISCS_SRC_HOME)/out/lib
INSTALLS += target

win32|win64{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lcfdb_api_cmn
}

unix{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lcfdb_api_cmn
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lparson
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib -lsysmgr_api_conf
}
