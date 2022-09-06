QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    collector.cpp \
    ewscomtest.cpp \
    ewsthread.cpp \
    main.cpp \
    mainwindow.cpp \
    mainworker.cpp \
    pcapcollectorthread.cpp \
    radarbinwriter.cpp \
    turnworker.cpp \
    udpdataprocessor.cpp \
    udphandlethread.cpp \
    workerqueue.cpp

HEADERS += \
    Include/Packet32.h \
    Include/Win32-Extensions.h \
    Include/bittypes.h \
    Include/ip6_misc.h \
    Include/pcap-bpf.h \
    Include/pcap-namedb.h \
    Include/pcap-stdinc.h \
    Include/pcap.h \
    Include/pcap/bluetooth.h \
    Include/pcap/bpf.h \
    Include/pcap/namedb.h \
    Include/pcap/pcap.h \
    Include/pcap/sll.h \
    Include/pcap/usb.h \
    Include/pcap/vlan.h \
    Include/remote-ext.h \
    Interface.h \
    SocketData.h \
    collector.h \
    ewscomtest.h \
    ewsthread.h \
    mainwindow.h \
    mainworker.h \
    pcapcollectorthread.h \
    radarbinwriter.h \
    turntablemessage.h \
    turnworker.h \
    udpdataprocessor.h \
    udphandledefines.h \
    udphandlethread.h \
    workerqueue.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Lib_pcap/Packet.lib \
    Lib_pcap/libpacket.a \
    Lib_pcap/libwpcap.a \
    Lib_pcap/wpcap.lib \
    Lib_pcap/x64/Packet.lib \
    Lib_pcap/x64/wpcap.lib \
    Lib/EwsCom.lib \
    lib/EwsCom.dll



message($$PWD)
unix|win32: LIBS += -L$$PWD/Lib_pcap/x64/ -lPacket
#unix|win32: LIBS += -LC:/Users/GPAL/Desktop/ScannerAndTurnTableControl/Lib_pcap/x64/ -lPacket

INCLUDEPATH += $$PWD/lib/x64
INCLUDEPATH += $$PWD/Include
INCLUDEPATH += $$PWD/Include/pcap
#DEPENDPATH += $$PWD/lib/x64

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/x64/Packet.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/x64/libPacket.a
#win32:!win32-g++: PRE_TARGETDEPS += C:/Users/GPAL/Desktop/ScannerAndTurnTableControl/Lib_pcap/x64/Packet.lib
#else:unix|win32-g++: PRE_TARGETDEPS += C:/Users/GPAL/Desktop/ScannerAndTurnTableControl/Lib_pcap/x64/libPacket.a

unix|win32: LIBS += -L$$PWD/Lib_pcap/x64/ -lwpcap
#unix|win32: LIBS += -LC:/Users/GPAL/Desktop/ScannerAndTurnTableControl/Lib_pcap/x64/ -lwpcap

INCLUDEPATH += $$PWD/Lib_pcap/x64
DEPENDPATH += $$PWD/Lib_pcap/x64
#INCLUDEPATH += C:/Users/GPAL/Desktop/ScannerAndTurnTableControl/Lib_pcap/x64
#DEPENDPATH += C:/Users/GPAL/Desktop/ScannerAndTurnTableControl/Lib_pcap/x64

LIBS += -L$$PWD/lib/ -lEwsCom
