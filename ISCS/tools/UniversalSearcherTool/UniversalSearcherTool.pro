#-------------------------------------------------
#
# Project created by QtCreator 2018-10-11T16:00:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UniversalSearcherTool
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
        widget.cpp \
    testdeleteform.cpp

HEADERS += \
        widget.h \
    testdeleteform.h

FORMS += \
        widget.ui \
    testdeleteform.ui

#unix:!macx: LIBS += -L$$PWD/libs/UniversalSearcher/lib/ -ltools_universal_searcher

#INCLUDEPATH += $$PWD/libs/UniversalSearcher/include
#DEPENDPATH += $$PWD/libs/UniversalSearcher/include

#INCLUDEPATH += $$PWD/../../out/include/cfdb_api

unix {
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib/ -lcfdb_api_cmn -ltools_universal_searcher
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api \
        $$(ISCS_SRC_HOME)/out/include/tools/universal_searcher
}

win32|win64{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib/ -lcfdb_api_cmn -ltools_universal_searcher
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api \
        $$(ISCS_SRC_HOME)/out/include/tools/universal_searcher
}
