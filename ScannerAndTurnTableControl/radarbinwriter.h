#ifndef RADARBINWRITER_H
#define RADARBINWRITER_H

#include <QObject>
#include <QFile>
#include "udphandledefines.h"

class RadarBinWriter : public QObject
{
    Q_OBJECT
public:
    explicit RadarBinWriter(QObject *parent = nullptr);

signals:
    void endWriteRadarBin();
    void signalRefreshStatus(QString msg);
public slots:
    void oneFrameDataSlot(RadarFrameBuffer* frameBuffer);
    void getWriteParamsSlot(const RadarBinParams* writeParams);
    void receive_filepath_msg(QString filepath_);
private:
    QFile file;
    int samples;
    int chirpNum;
    double azimuth;
    double elevation;
    double xAngle;
    double yAngle;
    QString filepath = ".";
    void initObject();
    void initParams();

    void createFilePath();
    void writeRadarFrame(RadarFrameBuffer* frameBuffer);
    void createRefreshMsg(QString msg);
};

#endif // RADARBINWRITER_H
