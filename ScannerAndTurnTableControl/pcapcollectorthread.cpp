#include "pcapcollectorthread.h"
#include <QDebug>
#include "SocketData.h"

QMutex udpMutex;
QList<UdpRawData*> rawDataList;

static ulong packetNum = 0;


PcapCollectorThread* PcapCollectorThread::self;

PcapCollectorThread::PcapCollectorThread(std::string fillter, int netDevicesCnt, QString dataProtoStr, QObject* parent)
    : QThread(parent),dataProtoStr_(dataProtoStr)
{
    if("CaptureOnly"==dataProtoStr_)
    {
        dataProtoId_=0;
    }
    else if("AntData"==dataProtoStr_)
    {
        dataProtoId_=1;
    }

    fillterRole = fillter;
    netDevicesID = netDevicesCnt;
    self = this;
    init();
    timer_ = new QTimer(this);
//    connect(timer_, SIGNAL(timeout()), this, SLOT(HandleTimer()));
    timer_->start(1000);
}

void PcapCollectorThread::init()
{
    pcap_if_t* alldevs;
    pcap_if_t* d;
    int i = 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    u_int netmask;
    struct bpf_program fcode;

    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        exit(1);
    }

    for (d = alldevs; d; d = d->next)
    {
        printf("%d. %s", ++i, d->name);
        if (d->description)
        {
            printf("  %s",d->description);
            qDebug() << ( d->description)<<(d->name);
        }
        else
            qDebug()<<(" (No description available)\n");
    }


    if (i == 0)
    {
        printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
        return;
    }

    /* Check if the user specified a valid adapter */
    if (netDevicesID < 1 || netDevicesID > i)
    {
        printf("\nAdapter number out of range.\n");
        pcap_freealldevs(alldevs);
    }
    for (d = alldevs, i = 0; i < netDevicesID - 1; d = d->next, i++);

    if ((adhandle = pcap_open_live(d->name,	// name of the device
                                   65536,			// portion of the packet to capture.
                                   // 65536 grants that the whole packet will be captured on all the MACs.
                                   1,				// promiscuous mode (nonzero means promiscuous)
                                   1000,			// read timeout
                                   errbuf			// error buffer
                                   )) == NULL)
    {
        fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n");
        pcap_freealldevs(alldevs);
        return;
    }

    if (pcap_datalink(adhandle) != DLT_EN10MB)
    {
        fprintf(stderr, "\nThis program works only on Ethernet networks.\n");
        pcap_freealldevs(alldevs);
        return;
    }

    if (d->addresses != NULL)
        netmask = ((struct sockaddr_in*)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    else
        netmask = 0xffffff;

    if (pcap_compile(adhandle, &fcode, fillterRole.data(), 1, netmask) < 0)
    {
        fprintf(stderr, "\nUnable to compile the packet filter. Check the syntax.\n");
        pcap_freealldevs(alldevs);
        return;
    }
    if (pcap_setfilter(adhandle, &fcode) < 0)
    {
        fprintf(stderr, "\nError setting the filter.\n");
        pcap_freealldevs(alldevs);
        return;
    }
    printf("\nlistening on %s...\n", d->description);
    pcap_freealldevs(alldevs);
    //    pcap_loop(adhandle, 0, receiveDataHandle, NULL);
}

void PcapCollectorThread::HandleTimer()
{
//    qDebug()<<"packetNum = "<<packetNum;
    qDebug()<<"rawDataList.size() = "<<rawDataList.size();
}

void PcapCollectorThread::startNetwork()
{
    if(!runningFlg_)
    {
        mutex_.lock();
        runningFlg_ = true;
        mutex_.unlock();
        this->start();          //开启线程
    }
}

void PcapCollectorThread::stopNetwork()
{
    mutex_.lock();
    runningFlg_ = false;
    mutex_.unlock();
}

void PcapCollectorThread::run()
{
    int ret;
    int framePacketCounter = 0;
    bool fonudHeader=false;

    switch (dataProtoId_) {
    case (0):
        while(runningFlg_)
        {
            mutex_.lock();
            while((ret = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
            {
                if(ret == 0)
                    /* Timeout elapsed */
                    continue;

                if(header->caplen>UDP_HEADER_LEN)
                {
    //                qDebug()<<"[TEST pkt_data] :[0]="<<(int)pkt_data[UDP_HEADER_LEN+UDP_PID_FLG_OFFSET]<<",[1]="<<(int)pkt_data[UDP_HEADER_LEN+UDP_PID_FLG_OFFSET+1];
                    quint64 dataSize = header->len-UDP_HEADER_LEN;
                    if(dataSize == 696 || dataSize == 1472)
                    {
                        char* rawData = new char[dataSize];
                        memcpy(rawData,&(pkt_data[UDP_HEADER_LEN]),dataSize);
        //                qDebug()<<"[TEST data] :[0]="<<(int)rawData[UDP_PID_FLG_OFFSET]<<",[1]="<<(int)rawData[UDP_PID_FLG_OFFSET+1];


                        UdpRawData* udpRawData = new UdpRawData();
                        udpRawData->rawData = rawData;
                        udpRawData->rawDataSize = header->len-UDP_HEADER_LEN;
                        udpMutex.lock();
                        rawDataList.append(udpRawData);
        //                qDebug()<<"rawDataList.size() = "<<rawDataList.size();
                        udpMutex.unlock();
                    }
                }
            }
            mutex_.unlock();
        }
        break;
    case(1):
        while(runningFlg_)
        {
            mutex_.lock();
            while((ret = pcap_next_ex(adhandle, &header, &pkt_data)) >= 0)
            {
                if(ret == 0)
                    /* Timeout elapsed */
                    continue;

                if(header->caplen>UDP_HEADER_LEN)
                {
    //                qDebug()<<"[TEST pkt_data] :[0]="<<(int)pkt_data[UDP_HEADER_LEN+UDP_PID_FLG_OFFSET]<<",[1]="<<(int)pkt_data[UDP_HEADER_LEN+UDP_PID_FLG_OFFSET+1];
                    quint64 dataSize = header->len-UDP_HEADER_LEN;
                    if(dataSize == 1472 || dataSize == 128)//112
                    {
                        char* rawData = new char[dataSize];
                        memcpy(rawData,&(pkt_data[UDP_HEADER_LEN]),dataSize);
                        qDebug()<<"[TEST data] :[0]="<<(int)rawData[UDP_PID_FLG_OFFSET]<<",[1]="<<(int)rawData[UDP_PID_FLG_OFFSET+1];


                        UdpRawData* udpRawData = new UdpRawData();
                        udpRawData->rawData = rawData;
                        udpRawData->rawDataSize = header->len-UDP_HEADER_LEN;
                        udpMutex.lock();
                        rawDataList.append(udpRawData);
        //                qDebug()<<"rawDataList.size() = "<<rawDataList.size();
                        udpMutex.unlock();
                    }

                }
            }
            mutex_.unlock();
        }
        break;
    default:
        break;
    }



}
