#ifndef PCAPCOLLECTORTHREAD_H
#define PCAPCOLLECTORTHREAD_H

#include <QThread>
#include <string>
#include <QTimer>
#include <QMutex>
#include "pcap.h"
#include "udphandledefines.h"

class PcapCollectorThread : public QThread
{
    Q_OBJECT
public:
    explicit PcapCollectorThread(std::string fillter, int netDevicesCnt, QString dataProtoStr = "CaptureOnly", QObject* parent = nullptr);
    static PcapCollectorThread* self;
public slots:
    void HandleTimer();
    void startNetwork();
    void stopNetwork();
protected:
    void run();
private:
    void init();
private:
//    std::string fillterRole="src port 12346 and src host 127.0.0.1";
    std::string fillterRole="udp and src host 192.168.1.200";
    int netDevicesID;
    pcap_t* adhandle;
    struct pcap_pkthdr *header;
    const u_char *pkt_data;
    QTimer *timer_;

    QMutex mutex_;
    bool runningFlg_ = false;

    QString dataProtoStr_ = "CaptureOnly";//[Options]:CaptureOnly/AntData/
    int32_t dataProtoId_ = 0;
};

#endif // PCAPCOLLECTORTHREAD_H
