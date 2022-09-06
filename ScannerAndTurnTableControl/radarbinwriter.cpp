#include "radarbinwriter.h"
#include <QDebug>
#include <QDate>
#include <QCoreApplication>

RadarBinWriter::RadarBinWriter(QObject *parent) : QObject(parent)
{

}

void RadarBinWriter::oneFrameDataSlot(RadarFrameBuffer *frameBuffer)
{
    qDebug("actually get one radar frame -- RadarBinWriter::oneFrameDataSlot");
    QString msg = "actually get one radar frame";
    createRefreshMsg(msg);

    createFilePath();
    writeRadarFrame(frameBuffer);
}

void RadarBinWriter::getWriteParamsSlot(const RadarBinParams *writeParams)
{
    if(!writeParams) return;

    samples = writeParams->samples;
    chirpNum = writeParams->chirpNum;
    azimuth = writeParams->azimuth;
    elevation = writeParams->elevation;
    xAngle = writeParams->xAngle;
    yAngle = writeParams->yAngle;

    qDebug()<<"sanmples"<<samples<<"chirpNum"<<chirpNum;
}

void RadarBinWriter::receive_filepath_msg(QString filepath_)
{
    filepath = filepath_;
}

void RadarBinWriter::initObject()
{
    initParams();
}

void RadarBinWriter::initParams()
{
    samples = 0;
    chirpNum = 0;
    azimuth = 0;
    elevation = 0;
    xAngle = 0;
    yAngle = 0;
}

void RadarBinWriter::createFilePath()
{
//    QString qApp_path = QCoreApplication::applicationDirPath();

    QString path = QString("/bin_s%1_c%2_a%3_e%4.bin").arg(samples).arg(chirpNum).arg(xAngle).arg(yAngle);

    QString qString_dir_t = filepath + path;

    qDebug()<<"file path:"<<qString_dir_t;
    file.setFileName(qString_dir_t);
}

void RadarBinWriter::writeRadarFrame(RadarFrameBuffer *frameBuffer)
{
    qDebug("start write radar frame to file -- RadarBinWriter::writeRadarFrame");
    QString msg = "start write radar frame to file";
    createRefreshMsg(msg);

    if(frameBuffer == nullptr) return;
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qDebug("open radar bin file for writing failed!");
        return;
    }
    qint64 writeRet = -1;
    for(uint32_t i = 0;i < frameBuffer->sensorNum;++i){
        writeRet = file.write((char*)(frameBuffer->sensorBufArr[i].bufPtr),frameBuffer->sensorBufArr->bufSize);
        qDebug()<<"writeRet:"<<i<<writeRet;
    }

    file.close();
    emit endWriteRadarBin();
}

void RadarBinWriter::createRefreshMsg(QString msg)
{
    emit signalRefreshStatus(msg);
}
