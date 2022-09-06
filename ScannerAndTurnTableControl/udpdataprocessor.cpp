#include "udpdataprocessor.h"
#include <QDebug>

UdpDataProcessor::UdpDataProcessor(WorkerQueue * workerQPtr,QString dataProtoStr, QObject *parent)
    :QObject(parent),workerQPtr_(workerQPtr),dataProtoStr_(dataProtoStr)
{
    if("CaptureOnly"==dataProtoStr_)
    {
        dataProtoId_=0;
    }
    else if("AntData"==dataProtoStr_)
    {
        dataProtoId_=1;
    }
    switch (dataProtoId_) {
    case (0):
        for(int i=0;i<SENSOR_NUM;++i)
        {
            sensorBufPtrArr_[i] = new uint8_t[MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR];
            sensorIdArr_[i] = static_cast<uint32_t>(i);
        }
        break;
    case(1):
        sensorBufPtrArr_[0] = new uint8_t[MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR];
        break;
    default:
        break;
    }

    qRegisterMetaType<RadarFrameBuffer>("RadarFrameBuffer");
}

UdpDataProcessor::~UdpDataProcessor()
{
    switch (dataProtoId_) {
    case (0):
        for(int i=0;i<SENSOR_NUM;++i)
        {
            delete [] sensorBufPtrArr_[i];
        }
        break;
    case(1):
        delete [] sensorBufPtrArr_[0];
        break;
    default:
        break;
    }
}

void UdpDataProcessor::GetOneRadarFrame()
{
    bool getOneFrameFlg = false;
    bool errorOccurFlg = false;

    switch (dataProtoId_) {
    case (0):
        memset(timestampArr_,0,sizeof(uint32_t)*SENSOR_NUM);
        while(!getOneFrameFlg)
        {
            int cur_pid = -1, cur_fid = -1, last_pid = -1, last_fid = -1;
            uint8_t *bufPtr;
            uint32_t bufSize;
            errorOccurFlg = false;
            if(!workerQPtr_->IsEmpty())
            {
                //取一个packet
                PacketBuffer firstPacketBuf = workerQPtr_->GetNextPacket();
                qDebug()<<"[UdpDataProcessor::GetOneRadarFrame]:WorkerQ size="<<workerQPtr_->GetPacketNum();
                bufPtr = firstPacketBuf.bufPtr;
                bufSize =firstPacketBuf.bufSize;
                //            qDebug()<<"bufPtr Header:"<<bufPtr[0]<<" "<<bufPtr[1]<<" "<<bufPtr[2]<<" "<<bufPtr[3];
                //寻找帧开头
                if(bufPtr[UDP_START_FLG_OFFSET] == UDP_START_FLG)
                {
                    //                qDebug()<<"Found Frame Start Packet!";
                    uint32_t sensor_id = 0, total_packet_num = 0,data_len = 0,frame_offset = 0,timestamp_in_ms = 0;

                    //获取UDP数据包头信息
                    sensor_id = bufPtr[UDP_IDX_FLG_OFFSET];
                    cur_fid =GetUint16FromArr(bufPtr,UDP_FID_FLG_OFFSET);
                    cur_pid =GetUint16FromArr(bufPtr,UDP_PID_FLG_OFFSET);
                    //qDebug()<<"cur_fid:"<<cur_fid<<",cur_pid ="<<cur_pid;
                    last_fid = cur_fid;
                    last_pid = cur_pid;

                    total_packet_num = GetUint16FromArr(bufPtr,UDP_PNUM_FLG_OFFSET);
                    data_len = GetUint32FromArr(bufPtr,UDP_DLEN_FLG_OFFSET);
                    frame_offset = GetUint32FromArr(bufPtr,UDP_FOFFSET_FLG_OFFSET);
                    timestamp_in_ms = GetUint32FromArr(bufPtr,UDP_TSTAMP_FLG_OFFSET);
                    if((data_len+UDP_DATA_OFFSET) < bufSize)
                    {
                        qDebug()<<"[ERROR]:UDP Packet has not enough data!!!";
                        //异常处理————舍弃该帧数据
                        timestampArr_[sensor_id] = 0;
                        memset(sensorBufPtrArr_[sensor_id],0,MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR);
                        continue;
                    }

                    qDebug()<<"sensor_id="<<sensor_id<<"curFID = " << cur_fid << ",curPID= "<<cur_pid
                           <<"total_packet_num = " << total_packet_num << ",data_len= "<<data_len
                          <<"frame_offset = " << frame_offset << ",timestamp_in_ms= "<<timestamp_in_ms;

                    //开始缓存该Sensor的一帧数据
                    memset(sensorBufPtrArr_[sensor_id],0,MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR);
                    memcpy(&(sensorBufPtrArr_[sensor_id][frame_offset]),&(bufPtr[UDP_DATA_OFFSET]),data_len);
                    workerQPtr_->Free(firstPacketBuf);
                    for(uint32_t recvPacketIdx=1;recvPacketIdx<total_packet_num;)
                    {
                        if(!workerQPtr_->IsEmpty())
                        {
                            // 尝试取一个新的packet */
                            PacketBuffer newPacketBuf = workerQPtr_->GetNextPacket();
                            bufPtr = newPacketBuf.bufPtr;
                            bufSize =newPacketBuf.bufSize;

                            last_fid = cur_fid;
                            last_pid = cur_pid;
                            //                        qDebug()<<"cur_fid:"<<cur_fid<<",cur_pid ="<<cur_pid;
                            cur_fid =GetUint16FromArr(bufPtr,UDP_FID_FLG_OFFSET);
                            cur_pid =GetUint16FromArr(bufPtr,UDP_PID_FLG_OFFSET);
                            // 检查PID、FID */
                            if(cur_fid!=last_fid || cur_pid - last_pid != 1)
                            {
                                qDebug()<<"[Warn]:There is Not nough packet in a Frame!---"
                                       <<last_fid<<","<<last_pid<<"-->"<<cur_fid<<","<<cur_pid;
                                // 舍弃该帧数据
                                timestampArr_[sensor_id] = 0;
                                memset(sensorBufPtrArr_[sensor_id],0,MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR);
                                //TODO:将Header Packet再添加回m_radar0DataListBuf，重新开始处理
                                errorOccurFlg = true;
                                break;
                            }
                            // 将packet添加到帧数据缓冲区 */
                            data_len = GetUint32FromArr(bufPtr,UDP_DLEN_FLG_OFFSET);
                            frame_offset = GetUint32FromArr(bufPtr,UDP_FOFFSET_FLG_OFFSET);
                            memcpy(&(sensorBufPtrArr_[sensor_id][frame_offset]),&(bufPtr[UDP_DATA_OFFSET]),data_len);
                            workerQPtr_->Free(newPacketBuf);
                            recvPacketIdx++;
                        }
                    }//for
                    if(!errorOccurFlg)//组包成功，记录sensor时间戳
                    {
                        timestampArr_[sensor_id] = timestamp_in_ms;
                        bufSizeArr_[sensor_id] = frame_offset+data_len;
                        qDebug()<<"Got Radar Sensor["<<sensor_id<<"]Frame Data!";
                    }
                }//if(bufPtr[UDP_START_FLG_OFFSET] == UDP_START_FLG)
                else
                {
                    // 释放堆缓冲区内存 */
                    workerQPtr_->Free(firstPacketBuf);
                }
                // 判断是否保存了一帧4个Sensor的数据*/
                if(CheckSensorTimestamp(timestampArr_,sensorIdArr_,SENSOR_NUM))
                {
                    getOneFrameFlg = true;
                }
            }//if(!workerQ->IsEmpty())
            else
            {
                //            usleep(1000);
                //TODO:延时
            }
        }//while(!getOneFrameFlg)

        frameBuffer_.sensorNum = SENSOR_NUM;
        for(uint32_t sensor_id=0;sensor_id<SENSOR_NUM;++sensor_id)
        {
            frameBuffer_.sensorBufArr[sensor_id].bufPtr = sensorBufPtrArr_[sensor_id];
            frameBuffer_.sensorBufArr[sensor_id].bufSize = bufSizeArr_[sensor_id];
            frameBuffer_.sensorBufArr[sensor_id].sensorId = sensor_id;
            frameBuffer_.sensorBufArr[sensor_id].timestamp = timestampArr_[sensor_id];
        }

        emit oneFrameDataReady(&frameBuffer_);
        break;
    case(1):
        while(!getOneFrameFlg)
        {
            int cur_pid = -1, cur_fid = -1, last_pid = -1, last_fid = -1;
            uint8_t *bufPtr;
            uint32_t bufSize;
            errorOccurFlg = false;
            if(!workerQPtr_->IsEmpty())
            {
                //取一个packet
                PacketBuffer firstPacketBuf = workerQPtr_->GetNextPacket();
    //            qDebug()<<"[UdpDataProcessor::GetOneRadarFrame]:WorkerQ size="<<workerQPtr_->GetPacketNum();
                bufPtr = firstPacketBuf.bufPtr;
                bufSize =firstPacketBuf.bufSize;
//                qDebug()<<"bufPtr Header:"<<bufPtr[0]<<" "<<bufPtr[1]<<" "<<bufPtr[2]<<" "<<bufPtr[3];
                //寻找帧开头
                if(bufPtr[UDP_START_FLG_OFFSET] == UDP_START_FLG)
                {
                    qDebug()<<"Found Frame Start Packet!";
                    uint32_t total_packet_num = 0,data_len = 0,frame_offset = 0;
                    //获取UDP数据包头信息
                    cur_fid =GetUint16FromArr(bufPtr,UDP_FID_FLG_OFFSET);
                    cur_pid =GetUint16FromArr(bufPtr,UDP_PID_FLG_OFFSET);
                    qDebug()<<"cur_fid:"<<cur_fid<<",cur_pid ="<<cur_pid;
                    last_fid = cur_fid;
                    last_pid = cur_pid;

                    total_packet_num = GetUint16FromArr(bufPtr,UDP_PNUM_FLG_OFFSET);
                    data_len = GetUint32FromArr(bufPtr,UDP_DLEN_FLG_OFFSET);
                    frame_offset = GetUint32FromArr(bufPtr,UDP_FOFFSET_FLG_OFFSET);

                    if((data_len+UDP_DATA_OFFSET) < bufSize)
                    {
                        qDebug()<<"[ERROR]:UDP Packet has not enough data!!!";
                        //异常处理————舍弃该帧数据
                        memset(sensorBufPtrArr_[0],0,MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR);
                        continue;
                    }

                    qDebug()<<"curFID = " << cur_fid << ",curPID= "<<cur_pid
                           <<"total_packet_num = " << total_packet_num << ",data_len= "<<data_len
                          <<"frame_offset = " << frame_offset;

                    //开始缓存该Sensor的一帧数据
                    memset(sensorBufPtrArr_[0],0,MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR);
                    memcpy(&(sensorBufPtrArr_[0][frame_offset]),&(bufPtr[UDP_DATA_OFFSET]),data_len);
                    workerQPtr_->Free(firstPacketBuf);
                    for(uint32_t recvPacketIdx=1;recvPacketIdx<total_packet_num;)
                    {
                        if(!workerQPtr_->IsEmpty())
                        {
                            // 尝试取一个新的packet */
                            PacketBuffer newPacketBuf = workerQPtr_->GetNextPacket();
                            bufPtr = newPacketBuf.bufPtr;
                            bufSize =newPacketBuf.bufSize;

                            last_fid = cur_fid;
                            last_pid = cur_pid;
                            //                        qDebug()<<"cur_fid:"<<cur_fid<<",cur_pid ="<<cur_pid;
                            cur_fid =GetUint16FromArr(bufPtr,UDP_FID_FLG_OFFSET);
                            cur_pid =GetUint16FromArr(bufPtr,UDP_PID_FLG_OFFSET);
                            // 检查PID、FID */
                            if(cur_fid!=last_fid || cur_pid - last_pid != 1)
                            {
                                qDebug()<<"[Warn]:There is Not nough packet in a Frame!---"
                                       <<last_fid<<","<<last_pid<<"-->"<<cur_fid<<","<<cur_pid;
                                // 舍弃该帧数据
                                memset(sensorBufPtrArr_[0],0,MAX_UDP_PACKET_LEN * MAX_PACKET_NUM_PER_SENSOR);
                                //TODO:将Header Packet再添加回m_radar0DataListBuf，重新开始处理
                                errorOccurFlg = true;
                                break;
                            }
                            // 将packet添加到帧数据缓冲区 */
                            data_len = GetUint32FromArr(bufPtr,UDP_DLEN_FLG_OFFSET);
                            frame_offset = GetUint32FromArr(bufPtr,UDP_FOFFSET_FLG_OFFSET);
                            memcpy(&(sensorBufPtrArr_[0][frame_offset]),&(bufPtr[UDP_DATA_OFFSET]),data_len);
                            workerQPtr_->Free(newPacketBuf);
                            recvPacketIdx++;
                        }
                    }//for
                    if(!errorOccurFlg)//组包成功，记录sensor时间戳
                    {
                        bufSizeArr_[0] = frame_offset+data_len;
                        getOneFrameFlg = true;
                        qDebug()<<"Got One Frame Radar Antenna Data!";
                    }
                }//if(bufPtr[UDP_START_FLG_OFFSET] == UDP_START_FLG)
                else
                {
                    // 释放堆缓冲区内存 */
                    workerQPtr_->Free(firstPacketBuf);
                }
            }//if(!workerQ->IsEmpty())
            else
            {
                //            usleep(1000);
                //TODO:延时
            }
        }//while(!getOneFrameFlg)

        frameBuffer_.sensorNum = 1;
        for(uint32_t sensor_id=0;sensor_id<SENSOR_NUM;++sensor_id)
        {
            frameBuffer_.sensorBufArr[sensor_id].bufPtr = sensorBufPtrArr_[sensor_id];
            frameBuffer_.sensorBufArr[sensor_id].bufSize = bufSizeArr_[sensor_id];
            frameBuffer_.sensorBufArr[sensor_id].sensorId = sensor_id;
            frameBuffer_.sensorBufArr[sensor_id].timestamp = 0;//timestampArr_[sensor_id];
        }

        emit oneFrameDataReady(&frameBuffer_);
        break;
    default:
        break;
    }
}

uint16_t UdpDataProcessor::GetUint16FromArr(uint8_t *arr, uint32_t index, bool bigEndianFlg)
{
    if (bigEndianFlg)
    {
        uint16_t ret = arr[index] * (0xFF + 1) + arr[index+1];
        return ret;
    }
    uint16_t ret = arr[index] + arr[index+1] * (0xFF + 1);
    return ret;
}

uint32_t UdpDataProcessor::GetUint32FromArr(uint8_t *arr, uint32_t index, bool bigEndianFlg)
{
    if (bigEndianFlg)
    {
        uint32_t ret = arr[index] * (0xFFFFFF + 1) + arr[index+1] * (0xFFFF + 1) +
                arr[index+2] * (0xFF + 1) + arr[index+3];
        return ret;
    }
    uint32_t ret = arr[index] + arr[index+1] * (0xFF + 1) +
            arr[index+2] * (0xFFFF + 1) + arr[index+3] * (0xFFFFFF + 1);
    return ret;
}

bool UdpDataProcessor::CheckSensorTimestamp(uint32_t * timeStampArr,uint32_t * sensorIdArr,uint32_t sensorNum)
{
    uint32_t last_timestamp = 0,cur_timestamp = 0;
    for(uint32_t i=0;i<sensorNum;++i)
    {
        uint32_t sensorId = sensorIdArr[i];
        cur_timestamp = timeStampArr[sensorId];
        if(0 == cur_timestamp)
        {
            return false;
        }
        if(0==i)
        {
            last_timestamp = cur_timestamp;
        }
        else
        {
            if(abs(int(cur_timestamp-last_timestamp))>MAX_DELTA_T_IN_MS_THRES)
            {
                return false;
            }
        }
    }
    return true;
}


