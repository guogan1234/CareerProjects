#ifndef UDPHANDLEDEFINES_H
#define UDPHANDLEDEFINES_H

#include <QObject>

#define SENSOR_NUM                    4
#define MAX_UDP_PACKET_LEN            2048
#define MAX_PACKET_NUM_PER_SENSOR     4096

#define UDP_START_FLG           0xBE
#define UDP_END_FLG             0xEF
#define UDP_VER_FLG             0x1

#define UDP_END_FLG_OFFSET          0
#define UDP_START_FLG_OFFSET        1
#define UDP_IDX_FLG_OFFSET          2
#define UDP_VER_FLG_OFFSET          3
#define UDP_FID_FLG_OFFSET          4
#define UDP_PID_FLG_OFFSET          8
#define UDP_PNUM_FLG_OFFSET         10
#define UDP_DLEN_FLG_OFFSET         12
#define UDP_FOFFSET_FLG_OFFSET      16
#define UDP_TSTAMP_FLG_OFFSET       20
#define UDP_DATA_OFFSET             24

#define UDP_HEADER_LEN              42 //For PCAP

#define MAX_DELTA_T_IN_MS_THRES     50

struct PacketBuffer
{
    uint8_t *bufPtr = nullptr;
    uint32_t bufSize = 0;
};

struct SensorDataBuffer
{
    uint8_t * bufPtr;
    uint32_t bufSize;
    uint32_t sensorId;
    uint32_t timestamp;
};

struct RadarFrameBuffer
{
    SensorDataBuffer sensorBufArr[SENSOR_NUM];
    uint32_t sensorNum;
};

struct RadarBinParams{
    int samples;
    int chirpNum;
    int speed;
    double azimuth;
    double elevation;
    double xAngle;
    double yAngle;
};

#endif // UDPHANDLEDEFINES_H
