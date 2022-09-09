#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T13:57:13
#
#-------------------------------------------------

QT       += widgets

TARGET = tools_universal_searcher
TEMPLATE = lib

DEFINES += UNIVERSALSEARCHER_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += universalsearcher.cpp \
    searcherform.cpp \
    model/activelistmodelbase.cpp \
    manager/domaindata.cpp \
    manager/domainmanager.cpp \
    manager/stationdata.cpp \
    manager/generaldata.cpp \
    manager/stationmanager.cpp \
    manager/prosystemdata.cpp \
    manager/prosystemmanager.cpp \
    manager/devtypedata.cpp \
    manager/devtypemanager.cpp \
    manager/accdata.cpp \
    manager/devdata.cpp \
    manager/devmanager.cpp \
    manager/accmanager.cpp \
    manager/didata.cpp \
    manager/dimanager.cpp \
    manager/aidata.cpp \
    manager/aimanager.cpp \
    ui/activecombobox.cpp \
    model/stationactivemodel.cpp \
    model/domainactivemodel.cpp \
    model/uimodel.cpp \
    model/prosystemactivemodel.cpp \
    model/devtypeactivemodel.cpp \
    ui/activetableview.cpp \
    model/deviceactivemodel.cpp \
    model/pointactivemodel.cpp \
    model/accactivemodel.cpp \
    model/aiactivemodel.cpp \
    model/diactivemodel.cpp \
    searcheasyform.cpp \
    manager/accdefdata.cpp \
    manager/acclimitdata.cpp \
    manager/acctpdata.cpp \
    worker/pointmimedata.cpp

HEADERS += universalsearcher.h\
        universalsearcher_global.h \
    searcherform.h \
    model/activelistmodelbase.h \
    manager/domaindata.h \
    data/domain.h \
    data/searcher.h \
    enum/searchtype.h \
    manager/domainmanager.h \
    manager/stationdata.h \
    manager/generaldata.h \
    manager/stationmanager.h \
    manager/prosystemdata.h \
    manager/prosystemmanager.h \
    manager/devtypedata.h \
    manager/devtypemanager.h \
    manager/accdata.h \
    manager/devdata.h \
    manager/devmanager.h \
    manager/accmanager.h \
    manager/didata.h \
    manager/dimanager.h \
    manager/aidata.h \
    manager/aimanager.h \
    ui/activecombobox.h \
    model/stationactivemodel.h \
    model/domainactivemodel.h \
    model/uimodel.h \
    model/prosystemactivemodel.h \
    model/devtypeactivemodel.h \
    ui/activetableview.h \
    model/deviceactivemodel.h \
    model/pointactivemodel.h \
    model/accactivemodel.h \
    model/aiactivemodel.h \
    model/diactivemodel.h \
    searcheasyform.h \
    data/cfdbtable.h \
    manager/accdefdata.h \
    manager/acclimitdata.h \
    manager/acctpdata.h \
    worker/pointmimedata.h \
    data/mimepoint.h

FORMS += \
    searcherform.ui \
    searcheasyform.ui

#DESTDIR += ./bin

unix {
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib/ -lcfdb_api_cmn
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api \
        ./ui
}

win32|win64{
    LIBS += -L$$(ISCS_SRC_HOME)/out/lib/ -lcfdb_api_cmn
    INCLUDEPATH += $$(ISCS_SRC_HOME)/out/include/cfdb_api \
        ./ui
}

message($$DESTDIR)
installBin.path += $$(ISCS_SRC_HOME)/out/bin
installBin.files +=
#installLib.path +=  $$(ISCS_SRC_HOME)/out/lib
#installLib.files += $$DESTDIR/*
target.path += $$(ISCS_SRC_HOME)/out/lib
installInc.path += $$(ISCS_SRC_HOME)/out/include/tools/universal_searcher
installInc.files += ./universalsearcher_global.h \
    ./searcherform.h \
    ./searcheasyform.h \
    ./model/uimodel.h
#installLib.path +=  $$(ISCS_SRC_HOME)/out/lib
#installLib.files += $$DESTDIR/*

#INSTALLS += installBin installInc installLib
INSTALLS += installBin installInc target
message($$PWD)
