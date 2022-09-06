#ifndef TURNWORKER_H
#define TURNWORKER_H

#include <QObject>
#include "ewscomtest.h"
#include "udphandledefines.h"
#include "turntablemessage.h"
typedef enum DIRECTION
{
    POSITIVE,
    NEGATIVE
}DIRECTION;

class TurnWorker : public QObject
{
    Q_OBJECT
public:
    explicit TurnWorker(QObject *parent = nullptr);
    ~TurnWorker();
signals:
    void getOneRadarFrameSignal();
    void writeRadarBinParams(const RadarBinParams* params);
    void signalRefreshStatus(QString msg);
    void signalRefreshOpenStatus(QString msg);
    void tansAnglesMsg(const TurnTableAngle* angles);
public slots:
    void openDeviceSlot();
    void suspendDeviceSlot();
    void closeDeviceSlot();
    void workProcessSlot();
    void movedDoneSlot();
    void endWriteRadarBinSlot();
    void recvSamplingMsg(RadarBinParams* params_);
private:
    EwsComtest ewsCom;
    DIRECTION state = POSITIVE;
    double xStartAngle;
    double yStartAngle;
    double xAdd;
    double yAdd;
    short iDevice;
    short iAxis;
    double fVel;
    short iType;
    double xMaxAngle;
    double yMaxAngle;

    void initObject();
    void initParams();
    void initConnects();

    int moveToLocation(double xAngle,double yAngle);
    void getOneRadarFrame();
    void sendRadarBinParams();
    void movedDoneAfter();
    void createRefreshMsg(QString& msg);

    int doneCount;
    RadarBinParams* params;
    TurnTableAngle* angles;
};

#endif // TURNWORKER_H
