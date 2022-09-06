#ifndef MAINWORKER_H
#define MAINWORKER_H

#include <QObject>
#include <QThread>
//#include "collector.h"
#include "pcapcollectorthread.h"
#include "udphandlethread.h"
#include "radarbinwriter.h"
#include "turnworker.h"
#include "ewscomtest.h"
#include "turntablemessage.h"
#include "udphandledefines.h"
class MainWorker : public QObject
{
    Q_OBJECT
public:
    explicit MainWorker(QObject *parent = nullptr);
    virtual ~MainWorker();

signals:
    void startNetwork();
    void openDeviceSignal();
    void workProcessSignal();
    void signalRefreshMsg(QString msg);
    void sendAnglesMsg(const TurnTableAngle* angles);
    void suspendDeviceSignal();
    void closeDeviceSignal();
    void sendSamplingMsg(RadarBinParams* params);
    void send_filepath_msg_to_writer(QString filepath);
public slots:
    void connectSlot();
    void mainw_startWork();
    void mainw_suspendWork();
    void mainw_closeWork();
    void turningWork();
    void getOneRadarFrameSlot();
    void slotRefreshMsg(QString msg);
    void receAnglesMsg(const TurnTableAngle* angles);
    void mainw_sampling_params(RadarBinParams* params);
    void receive_save_filepath_msg(QString filepath);
private:
//    Collector collector;
    PcapCollectorThread * pcapThread;
    QThread collectThread;
    UdpHandleThread* udpHandler;
    WorkerQueue workerQueue;
    RadarBinWriter binWriter;
    QThread writerThread;

    TurnWorker turnWorker;
    QThread turnThread;
//    EwsComtest ewsCom;
//    QThread ewsComThread;

    void initObject();
    void initParams();
    void initThreads();
    void initConnects();

    void connectNetwork();
    void getOneRadarFrame();

    double xStartAngle;
    double yStartAngle;
    double xAdd;
    double yAdd;
    double fVel;

    void connectNetworkByPcap();
    void openDevice();
    void suspendDevice();
    void closeDevice();
    void workProcess();
    void turnToLocation(double xAngle,double yAngle);
};

#endif // MAINWORKER_H
