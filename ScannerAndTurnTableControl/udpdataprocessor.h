#ifndef UDPDATAPROCESS_H
#define UDPDATAPROCESS_H

#include <QObject>
#include <QMetaType>
#include "workerqueue.h"
#include "udphandledefines.h"

class UdpDataProcessor : public QObject
{
    Q_OBJECT
public:
    explicit UdpDataProcessor(WorkerQueue * workerQPtr,QString dataProtoStr="CaptureOnly", QObject *parent = nullptr);
    ~UdpDataProcessor();
    void GetOneRadarFrame();
private:
    uint16_t GetUint16FromArr(uint8_t *arr, uint32_t index, bool bigEndianFlg = false);
    uint32_t GetUint32FromArr(uint8_t *arr, uint32_t index, bool bigEndianFlg = false);
    bool CheckSensorTimestamp(uint32_t * timeStampArr,uint32_t * sensorIdArr,uint32_t sensorNum);
signals:
    void oneFrameDataReady(RadarFrameBuffer *frameBuffer);
public slots:

private:
    WorkerQueue * workerQPtr_;      //由外部维护
    //维护的一帧雷达数据结果（多个Sensor）
    uint8_t * sensorBufPtrArr_[SENSOR_NUM];  //顺序:sensor0\sensor1\sensor2\sensor3
    uint32_t timestampArr_[SENSOR_NUM];//顺序:sensor0\sensor1\sensor2\sensor3
    uint32_t bufSizeArr_[SENSOR_NUM];//顺序:sensor0\sensor1\sensor2\sensor3
    uint32_t sensorIdArr_[SENSOR_NUM];//顺序:sensor0\sensor1\sensor2\sensor3
    RadarFrameBuffer frameBuffer_;

    QString dataProtoStr_ = "CaptureOnly";//[Options]:CaptureOnly/AntData/
    int32_t dataProtoId_ = 0;
};

#endif // UDPDATAPROCESS_H
