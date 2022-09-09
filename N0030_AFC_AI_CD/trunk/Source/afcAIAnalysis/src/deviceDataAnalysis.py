#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,time,datetime,traceback
import json,random
import logging,logging.handlers
from DACommon import ConfigData,createDaemon
from DACommon import initLogger,TimeConvert,getPgsqlConn
from DACommon import loadLastStatus,dumpLastStatus
from DACommon import ObjParseDeviceInfo,initConsolLogger
from  DACommon import ObjTransCount,ObjDowntimeStatus,ObjFailureStatus
import TSDBCMN 
from dateutil import relativedelta
import tagValueMap

logger = logging.getLogger()
EVENTLOGTAG = "eventLogTimeBegin"
gLastInfoMap = {
    EVENTLOGTAG : "",
}

gWarnMap = tagValueMap.getWarnMap()
gTagModMap = tagValueMap.getTagModMap()
gDowntimeTagName = "NMDSTA" 
gArrFailureMods = [2, 3, 4, 6, 9, 10, 11, 12]
gArrFailureTags = [    
    "METCS1",
    "METCS2",
    "METBNA",
    "METBCG",
    "METCHS",
    "METCST",
    "METCTN",
    "METPLC",
]

#gArrFailureTags = gTagModMap.keys() # for test
gArrFailureTags = [tag for tag in gTagModMap.keys() if not tag in gArrFailureTags]

def calcuteErrorTime(refStatusArr,dataArr,tSectionBegin,totalSecntionTime):
    newDataArr = list(dataArr)
    totalDowntime,totalNormaltime = 0,0
    newTotalSecTime = totalSecntionTime / 60
    lastDownTime = 0    
    lastTime,lastStatus = refStatusArr
    errorCount = 0
    if lastStatus == 0 : # error
        errorCount += 1
        lastDownTime = lastTime
        if (lastDownTime < tSectionBegin) :
            lastDownTime = tSectionBegin
            newDataArr = [(lastDownTime,0)] + newDataArr

    if len(newDataArr) > 0 :
        lastValue = newDataArr[-1]
        if lastValue[1] == 0 :
            newDataArr.append((tSectionBegin+totalSecntionTime,1))

    bAllNormal = True
    for objTrans in newDataArr:
        transTime = objTrans[0]
        tagValue = objTrans[1]
        print transTime,tagValue
        refStatusArr[0] = transTime
        refStatusArr[1] = tagValue
        if tagValue < 0 : continue
        curTransTime = transTime
        if tagValue == 0 : # error status 
            lastDownTime = curTransTime 
            bAllNormal = False
        else : 
            if lastDownTime == 0 : continue
            if (curTransTime >= lastDownTime) and (bAllNormal == False):
                tDiff = curTransTime - lastDownTime
                totalDowntime += tDiff 
                
    if bAllNormal :
        totalDowntime = 0
        totalNormaltime = newTotalSecTime
    else :
        if totalDowntime > totalSecntionTime : 
            totalDowntime = totalSecntionTime
        if totalDowntime < 0 : totalDowntime = 0
        totalDowntime = totalDowntime / 60
        totalNormaltime = newTotalSecTime - totalDowntime
    return totalDowntime,totalNormaltime,errorCount
      
class ObjDowntimeDetail(object):
    # tagName : NMDSTA
    def __init__(self):
        self.dataArr = []
        self.totalDowntime = 0
        self.totalNormaltime = 0
        self.downRate = 0.0
        
    def clear(self):
        del self.dataArr[:]
        
    def getStatusCode(self,tagValue):
        status = 1 # normal 
        if str(tagValue) == '0' : # 停止服务模式 
            status = 0 # error
        return status
        
    def append(self,objTmp):
        lastTagStatus,curTagStatus = None,None
        refArr = self.dataArr
        if len(self.dataArr) > 0 :
            lastObj = self.dataArr[-1]
            lastTagStatus = self.getStatusCode(lastObj[1])
        transTime,tagValue = objTmp
        curTagStatus = self.getStatusCode(tagValue)
        if curTagStatus != lastTagStatus:
            self.dataArr.append((transTime,curTagStatus))    
        return None  
   
    def calcuteDowntime(self,refStatusArr,tSectionBegin,totalSecntionTime): 
        newTotalSecTime = totalSecntionTime / 60
        totalDowntime,totalNormaltime,_  = calcuteErrorTime(refStatusArr,self.dataArr,tSectionBegin,totalSecntionTime) 
        self.totalDowntime = totalDowntime
        self.totalNormaltime = totalNormaltime
        self.downRate = self.totalDowntime / (newTotalSecTime * 1.0)
        return None   
    
def checkTagIsWarn(tagName,tagValue):
    bRet = False
    if tagValue in gWarnMap[tagName]["arrValue"]:
        bRet = True
    return bRet  
      
class ObjFailReasonDetail(object):
    def __init__(self):
        self.tagMap = {}
        self.transCount = 0
        self.modMap = {}
        
    def clear(self):
        for tagName,tmpArr in self.tagMap.iteritems():
            del tmpArr[:]
        self.tagMap.clear()
        self.tagMap = None
        self.modMap.clear()
        self.modMap = None
        
    def getStatusCode(self,tagName,tagValue):
        status = 1 # normal 
        if checkTagIsWarn(tagName,tagValue) : # 报警认为是故障 
            status = 0
        return status
        
    def append(self,tagName,objTmp):
        # get modId by tagName 
        modId = gTagModMap[tagName][0][0]
        if modId in gArrFailureMods :     
            if not tagName in self.tagMap : 
                self.tagMap[tagName] = []
            refArr = self.tagMap[tagName]
            transTime,tagValue = objTmp
            curTagStatus = self.getStatusCode(tagName,tagValue)  
            lastTagStatus,curTagStatus = None,None
            if len(refArr) > 0 :
                lastObj = refArr[-1]
                lastTagStatus = self.getStatusCode(tagName,lastObj[1])
            curTagStatus = self.getStatusCode(tagName,tagValue)
            if curTagStatus != lastTagStatus:
                refArr.append((transTime,curTagStatus))
        return None         

    def calcuteFailureTime(self,gTC,refDeviceIdMap,tSectionBegin,totalSecntionTime): 
        newTotalSecTime = totalSecntionTime / 60
        for tagName in gArrFailureTags : 
            refDeviceIdMap[tagName] = [0,-1]
        
        refTmpMap = refDeviceIdMap
        #print "refTmpMap : ",refTmpMap
        for tagName,tmpArr in self.tagMap.iteritems() :
            refTmpMap = refDeviceIdMap[tagName]
            curTagDownTime,curTagNormalTime,errCount = calcuteErrorTime(refTmpMap,tmpArr,tSectionBegin,totalSecntionTime) 
            # get mod id tagName
            modId = gTagModMap[tagName][0][0]
            if not modId in self.modMap:
                self.modMap[modId] = {
                    "normalTime" : 0.0,
                    "failureTime" : 0.0,
                }
            refCurModMap = self.modMap[modId]
            if errCount > 0 :
                errCount = (errCount+1) * 1.0
                curTagNormalTime = curTagNormalTime / errCount
                curTagDownTime = curTagDownTime / errCount
                
            refCurModMap["normalTime"] += curTagNormalTime 
            refCurModMap["failureTime"] += curTagDownTime 
            
        return None          

def genTsdbQuery_eventlog(tbName,lineId,beginTime,endTime,maxPerFetch,offset):
    qPattern = '''select {conditions} from {table} ''' 
    qPattern += ''' where LINEID='{lineId}' and time >= {beginTime} and time < {endTime} ''' 
    qPattern += ''' limit {maxPerFetch} offset {offsetNum};'''
    #tbName = "ObjTransaction"
    arrLables = ["LINEID","DEVICEID","TRANSACTIONDATETIME","TAGNAME","TAGVALUE"]
    arrLables = [item.upper() for item in arrLables]
    
    qstr = qPattern.format(
        conditions = ",".join(arrLables),
        table = tbName,
        lineId = lineId, # '7'
        beginTime = beginTime,
        endTime = endTime,
        maxPerFetch = maxPerFetch,
        offsetNum = offset
    )
    return qstr    
 
def genPgdbQuery_line(strAnalysisTime,lineId,refDCurLine):
    tbName = "line_failure_analysis"
    #qPattern = '''insert into line_failure_analysis (line_id,tag_name,failure_num,analysis_timestamp) values ('8','PASINT',10,'2017-07-01 00:00:00');'''
    arrFields = ["line_id","tag_name","failure_num","analysis_timestamp"]
    qPattern = '''insert into line_failure_analysis (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []    
    for tagName,dItem in refDCurLine.iteritems(): 
        failureNum = dItem["failure_num"]
        if not failureNum : continue
        dTmp = {
            "line_id" : lineId,
            "tag_name" : tagName,  
            "failure_num" : failureNum,
            "analysis_timestamp" : strAnalysisTime,
        }
        
        qstrTmp = qPattern.format(**dTmp)
        arrQuery.append(qstrTmp)
    return arrQuery

def genPgdbQuery_station(strAnalysisTime,lineId,refDCurLineDetail):
    tbName = "station_failure_analysis"
    #qPattern = '''insert into line_failure_analysis (line_id,station_id,tag_name,failure_num,analysis_timestamp) values ('8','PASINT',10,'2017-07-01 00:00:00');'''
    arrFields = ["line_id","station_id","tag_name","failure_num","analysis_timestamp"]
    qPattern = '''insert into %s (''' % tbName  + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []   
    for stationId,dRStation in refDCurLineDetail.iteritems():
        for tagName,dItem in dRStation.iteritems(): 
            failureNum = dItem["failure_num"]
            if not failureNum : continue
            dTmp = {
                "line_id" : lineId,
                "station_id" : stationId,
                "tag_name" : tagName,  
                "failure_num" : failureNum,
                "analysis_timestamp" : strAnalysisTime,
            }
            
            qstrTmp = qPattern.format(**dTmp)
            arrQuery.append(qstrTmp)
    return arrQuery
    
def genPgdbQuery_model(strAnalysisTime,lineId,refDRModel):
    tbName = "module_failure_analysis"
    #qPattern = '''# insert into module_failure_analysis (module_id,tag_name,failure_num,analysis_timestamp) values (8,'PASINT',10,'2017-07-01 00:00:00');'''
    arrFields = ["line_id","module_id","tag_name","failure_num","analysis_timestamp"]
    qPattern = '''insert into module_failure_analysis (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []    
    for modId,dMod in refDRModel.iteritems():
        for tagName,dItem in dMod.iteritems():
            failureNum = dItem["failure_num"]
            if not failureNum : continue            
            dTmp = {
                "line_id" : lineId,
                "module_id" : modId,
                "tag_name" : tagName,  
                "failure_num" : failureNum,
                "analysis_timestamp" : strAnalysisTime,
            }
            
            qstrTmp = qPattern.format(**dTmp)
            arrQuery.append(qstrTmp)
    return arrQuery 

def genPgdbQuery_model_byStation(strAnalysisTime,lineId,refDRModel):
    tbName = "module_failure_analysis_bystation"
    arrFields = ["line_id","station_id","module_id","tag_name","failure_num","analysis_timestamp"]
    qPattern = '''insert into %s (''' % tbName + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []  
    for stationId,dRStation in refDRModel.iteritems():
        for modId,dMod in dRStation.iteritems():
            for tagName,dItem in dMod.iteritems():
                failureNum = dItem["failure_num"]
                if not failureNum : continue            
                dTmp = {
                    "line_id" : lineId,
                    "station_id" : stationId,
                    "module_id" : modId,
                    "tag_name" : tagName,  
                    "failure_num" : failureNum,
                    "analysis_timestamp" : strAnalysisTime,
                }
                
                qstrTmp = qPattern.format(**dTmp)
                arrQuery.append(qstrTmp)
    return arrQuery 
 
def genPgdbQuery_device(strAnalysisTime,lineId,refDRDevice):
    tbName = "device_failure_analysis"
    #qPattern = '''insert into device_failure_analysis (device_id,failure_num,analysis_timestamp) values (8,10,'2017-07-01 00:00:00');'''
    arrFields = ["line_id","device_id","failure_num","analysis_timestamp"]
    qPattern = '''insert into %s (''' % tbName  + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []  
    for deviceId,dSub in refDRDevice.iteritems():
        dItem = dSub["summary"]
        failureNum = dItem["failure_num"]
        if not failureNum : continue          
        dTmp = {
            "line_id" : lineId,
            "device_id" : deviceId,
            "failure_num" : failureNum,
            "analysis_timestamp" : strAnalysisTime,
        }
        
        qstrTmp = qPattern.format(**dTmp)
        arrQuery.append(qstrTmp)
    return arrQuery

def genPgdbQuery_device_detail(strAnalysisTime,lineId,refDRDevice):
    tbName = "device_failure_analysis_detail"
    arrFields = ["line_id","station_id","device_id","device_type","tag_name","failure_num","analysis_timestamp"]
    qPattern = '''insert into %s (''' % tbName  + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []  
    for deviceId,dSub in refDRDevice.iteritems():
        dTags = dSub["detail"] 
        objDeviceInfo = ObjParseDeviceInfo(deviceId)
        for tagName,dItem in dTags.iteritems():
            failureNum = dItem["failure_num"]
            if not failureNum : continue
            dTmp = {
                "line_id" : lineId,
                "station_id" : objDeviceInfo.getStationId(),
                "device_id" : deviceId,
                "device_type" : objDeviceInfo.getDeviceType(),
                "tag_name" : tagName,
                "failure_num" : failureNum,
                "analysis_timestamp" : strAnalysisTime,
            }
            qstrTmp = qPattern.format(**dTmp)
            arrQuery.append(qstrTmp)
    return arrQuery

def genPgdbQuery_device_downtime(strAnalysisTime,lineId,refDRDevice):
    tbName = "device_downtime_percent_analysis"
    arrFields = ["line_id","station_id","device_id","normal_time_in_minutes","failure_time_in_minutes","failure_rate","analysis_timestamp"]
    qPattern = '''insert into %s (''' % tbName  + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []  
   
    #print "refDRDevice :", refDRDevice
    for deviceId,dSub in refDRDevice.iteritems():
        refObj = dSub["downtime_detail"]
        #print "deviceId :",deviceId
        objDeviceInfo = ObjParseDeviceInfo(deviceId)
        dTmp = {
            "line_id" : lineId,
            "station_id" : objDeviceInfo.getStationId(),
            "device_id" : deviceId,
            "normal_time_in_minutes" : refObj.totalNormaltime,
            "failure_time_in_minutes" : refObj.totalDowntime,
            "failure_rate" : "%.2f" % refObj.downRate,
            "analysis_timestamp" : strAnalysisTime,
        }
        qstrTmp = qPattern.format(**dTmp)
        arrQuery.append(qstrTmp)
        print qstrTmp
    #print arrQuery
    return arrQuery   

def genPgdbQuery_device_failure_reason(strAnalysisTime,lineId,refDRDevice):
    tbName = "failure_reason_analysis"
    arrFields = ["line_id","station_id","device_id","device_type","module_id","normal_time_in_minutes","failure_time_in_minutes","trans_count","analysis_timestamp"]
    qPattern = '''insert into %s (''' % tbName  + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    arrQuery = []  
   
    #print "refDRDevice :", refDRDevice
    for deviceId,dSub in refDRDevice.iteritems():
        #print "deviceId :",deviceId  
        objDeviceInfo = ObjParseDeviceInfo(deviceId)
        refObj = dSub["failure_reason"]        
        for modId,tmpMap in refObj.modMap.iteritems() :
            dTmp = {
                "line_id" : lineId,
                "station_id" : objDeviceInfo.getStationId(),
                "device_id" : deviceId,
                "device_type" : objDeviceInfo.getDeviceType(),
                "module_id" : modId,
                "normal_time_in_minutes" : int(tmpMap["normalTime"]),
                "failure_time_in_minutes" : int(tmpMap["failureTime"]),
                "trans_count" : refObj.transCount,
                #"trans_count" : random.randint(500,5000), # for test
                "analysis_timestamp" : strAnalysisTime,
            }
            qstrTmp = qPattern.format(**dTmp)
            arrQuery.append(qstrTmp)
            print qstrTmp
    #print arrQuery
    return arrQuery       
    
def genPgdbQuery_clear(tbName,tFlag,tBegin):
    retQuery = ""  
    tsFlag = ""
    pattern = "delete from {table} where {timeFlag} >= timestamp '{timeBegin}' ;"
    retQuery = pattern.format(
        table = tbName,
        timeFlag = tFlag,
        timeBegin = tBegin
    )    
    return retQuery
                
def initTagStatisMap(refMap,tagName):
    if not tagName in refMap :
        refMap[tagName] = {
            #"total_num" : 0,
            #"normal_num" : 0,
            "failure_num" : 0,            
        }
    return refMap[tagName]

    
def fillTagStatisData(refDTag,tagName,tagValue):
    #refDTag_line["total_num"] += 1
    #if tagValue in gWarnMap[tagName]["arrValue"] :
    if checkTagIsWarn(tagName,tagValue) :
        refDTag["failure_num"] += 1
    #else: 
    #    refDTag["normal_num"] += 1
    return None
    
def fillDowntimeData(refObj,transTime,tagName,tagValue):
    if tagName == gDowntimeTagName : 
        objTmp = (transTime,tagValue)
        refObj.append(objTmp)
    return None 
    
def fillFailureData(refObj,transTime,tagName,tagValue):
    if tagName in gArrFailureTags : 
        objTmp = (transTime,tagValue)
        refObj.append(tagName,objTmp)
    return None 


def fillLineItemData(gTC,refDCurLine,refDCurLineDetail,refDMod,refDMod_bystation,refDRDevice,dItem):
    transTime = str(dItem["TRANSACTIONDATETIME"])
    tagName = str(dItem["TAGNAME"])
    tagValue = int(dItem["TAGVALUE"])
    deviceId = int(dItem["DEVICEID"])
    staionId = ObjParseDeviceInfo(deviceId).getStationId()
    
    transTime = transTime.split('Z')[0].replace('T',' ')
    transTime = gTC.strTime2Int(transTime)    
    
    if not staionId in refDCurLineDetail :
        refDCurLineDetail[staionId] = {}
    refDCurStation = refDCurLineDetail[staionId]
    
    if not staionId in refDMod_bystation :
        refDMod_bystation[staionId] = {}
    refDModStation = refDMod_bystation[staionId]

    refDTag_line = initTagStatisMap(refDCurLine,tagName)
    refDTag_staion = initTagStatisMap(refDCurStation,tagName)
    #refDTag_line["total_num"] += 1
    if tagName in gWarnMap :
        fillTagStatisData(refDTag_line,tagName,tagValue)
        fillTagStatisData(refDTag_staion,tagName,tagValue)
        
        modId = gWarnMap[tagName]["modName"][0] 
        if not modId in refDMod : refDMod[modId] = {}
        refDTag_mod = initTagStatisMap(refDMod[modId],tagName) 
        fillTagStatisData(refDTag_mod,tagName,tagValue)
        
        if not modId in refDModStation : refDModStation[modId] = {}
        refDTag_mod_station = initTagStatisMap(refDModStation[modId],tagName) 
        fillTagStatisData(refDTag_mod_station,tagName,tagValue)        
                
        if not deviceId in refDRDevice : 
            refDRDevice[deviceId] = {
                "summary" : {"failure_num" : 0},
                "detail" : {},
                "downtime_detail" : ObjDowntimeDetail(), 
                "failure_reason" : ObjFailReasonDetail(), 
            }
            
        refDTag_device_summary = refDRDevice[deviceId]["summary"]
        fillTagStatisData(refDTag_device_summary,tagName,tagValue)
        refDTag_device_detail = refDRDevice[deviceId]["detail"]
        if not tagName in refDTag_device_detail :
            refDTag_device_detail[tagName] = {"failure_num" : 0}
        refDTag_device_detail_tag = refDTag_device_detail[tagName]        
        fillTagStatisData(refDTag_device_detail_tag,tagName,tagValue)
        refDowntimeDetail = refDRDevice[deviceId]["downtime_detail"]        
        fillDowntimeData(refDowntimeDetail,transTime,tagName,tagValue) 
        refFailureReason = refDRDevice[deviceId]["failure_reason"]
        fillFailureData(refFailureReason,transTime,tagName,tagValue) 
    return 0      
    
def flusData2pgdb(pgConn,arrQuery):
    for query in arrQuery :
        #print query
        pgConn.execute(query)
    pgConn.commit()  
    
 
def evnetlogStatis(gTC,lineId,tBegin,tEnd):
    cnf = ConfigData()
    tsdbConn = TSDBCMN.getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch  
    
    dRSummary,dRByStation = {},{}
    dRMSummary,dRMBystation = {},{}
    dRDevice = {}        
    tbName = "ObjEventLog"
    
    totalPointsNum = 0
    arrQuery = []   
    
    daySeconds = 24 * 3600
    curDayTime = tBegin
    while curDayTime < tEnd :        
        beginTime = curDayTime * (10 ** 9)
        nextDayTime = curDayTime + daySeconds
        endTime = nextDayTime * (10 ** 9)
        msg = "[%s,%s)" % (gTC.intTime2Str(curDayTime),gTC.intTime2Str(nextDayTime))
        print lineId,msg
        logger.info(msg)
        t_offset = 0
        while True :
            t1 = time.time()
            qstr = genTsdbQuery_eventlog(tbName,lineId,beginTime,endTime,t_maxPerFetch,t_offset)
            logger.debug(qstr)
            #print qstr
            data = tsdbConn.query(qstr,epoch="s")
            #data = tsdbConn.query(qstr)        
            dataPoints = data.get_points()
            dataLen = 0
            for dItem in dataPoints:
                dataLen += 1
                #print dItem      
                # fill line statis data   
                fillLineItemData(gTC,dRSummary,dRByStation,dRMSummary,dRMBystation,dRDevice,dItem)
                dItem = None
            dataPoints = None
            data = None
            totalPointsNum += dataLen    
            print "dataPoints len : ",dataLen
            tdiff = time.time() - t1
            print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)

            if dataLen == 0 : break
            t_offset += dataLen            
        curDayTime = nextDayTime 
        
    tSectionBegin = tBegin
    totalSecntionTime = tEnd - tBegin
    dbFile = cnf.cacheFile
    
    downtimeMap={}
    osrDown = ObjDowntimeStatus()
    osrDown.init(dbFile)  
    osrDown.createTable()    
    osrDown.loadData(downtimeMap)
    
    failureReasonMap = {}
    osrFailure = ObjFailureStatus()
    osrFailure.init(dbFile)
    osrFailure.createTable()
    osrFailure.loadData(failureReasonMap)
     
    transCountMap={}
    osrTrans = ObjTransCount()
    osrTrans.init(dbFile)    
    osrTrans.createTable()    
    osrTrans.loadData(transCountMap) 

    for deviceId,dSub in dRDevice.iteritems():
        refObj = dSub["downtime_detail"]
        #print "deviceId :",deviceId
        if not deviceId in downtimeMap :
            downtimeMap[deviceId] = [0,-1]
        refTmpMap = downtimeMap[deviceId]
        refObj.calcuteDowntime(refTmpMap,tSectionBegin,totalSecntionTime) 
      
        refObj = dSub["failure_reason"]
        # count device trans data
        if deviceId in transCountMap :
            tCount = 0
            for timeTmp,numTmp in transCountMap[deviceId].iteritems():
                if (timeTmp >= tBegin) and (timeTmp < tEnd) :
                    tCount += numTmp
            refObj.transCount = tCount
        # calcute device failure time
        if not deviceId in failureReasonMap :
            failureReasonMap[deviceId] = {}
        refDeviceIdMap = failureReasonMap[deviceId]
        refObj.calcuteFailureTime(gTC,refDeviceIdMap,tSectionBegin,totalSecntionTime)
        
    # gen new insert sql    
    # gen line_failure_analysis 
    strBeginTime = gTC.intTime2Str(tBegin)
    arrQuery += genPgdbQuery_line(strBeginTime,lineId,dRSummary)
    arrQuery += genPgdbQuery_station(strBeginTime,lineId,dRByStation)
    arrQuery += genPgdbQuery_model(strBeginTime,lineId,dRMSummary)
    arrQuery += genPgdbQuery_model_byStation(strBeginTime,lineId,dRMBystation)
    arrQuery += genPgdbQuery_device(strBeginTime,lineId,dRDevice)
    arrQuery += genPgdbQuery_device_detail(strBeginTime,lineId,dRDevice)
    arrQuery += genPgdbQuery_device_downtime(strBeginTime,lineId,dRDevice)
    arrQuery += genPgdbQuery_device_failure_reason(strBeginTime,lineId,dRDevice)

    #print arrQuery    
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrQuery)
    pgConn.close()
        
    osrDown.dumpData(downtimeMap)
    osrFailure.dumpData(failureReasonMap)
    
    downtimeMap.clear()
    failureReasonMap.clear()
    dRSummary.clear()
    dRByStation.clear()
    dRMSummary.clear()
    dRMBystation.clear()    
    for deviceId,dSub in dRDevice.iteritems():
        for key in ["detail","downtime_detail","failure_reason"] :
            refObj = dSub[key]
            refObj.clear()
            dSub[key] = None
        dSub = None
    downtimeMap = None
    failureReasonMap = None
    dRSummary = None
    dRByStation = None
    dRMSummary = None
    dRMBystation = None
    dRDevice=None
    tsdbConn = None
    print "evnetlogStatis done,totalPointsNum = ",totalPointsNum
    return totalPointsNum
    
def doDataClean(cnf,tBeginStr):
    msg = "do doDataClean begin"
    logger.info(msg)
    # gen clear data sql
    arrPgdbQuery = []
    
    dTableInfo = {
        "line_failure_analysis" : "analysis_timestamp",
        "station_failure_analysis" : "analysis_timestamp",
        "module_failure_analysis" : "analysis_timestamp",
        "module_failure_analysis_bystation" : "analysis_timestamp",
        "device_failure_analysis" : "analysis_timestamp",
        "device_failure_analysis_detail" : "analysis_timestamp",        
        "device_downtime_percent_analysis" : "analysis_timestamp",        
        "failure_reason_analysis" : "analysis_timestamp",        
    }
    for tbName,tField in dTableInfo.iteritems():
        arrPgdbQuery += [genPgdbQuery_clear(tbName,tField,tBeginStr)]
                
    msg = "\n".join(arrPgdbQuery)
    logger.info(msg)
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrPgdbQuery)
    pgConn.close()
    
    if os.path.exists(cnf.cacheFile) : os.remove(cnf.cacheFile)
    
    msg = "do doDataClean done"
    logger.info(msg)  
    
def dataAnalysisMain():
    cnf = ConfigData()
    logger.info("deviceDataAnalysis start")
    
    # init 
    gTC = TimeConvert()
    tsdbConn = TSDBCMN.getTsdbConn(cnf)
    # get line info        
    arrLineInfo = TSDBCMN.getLineInfo(tsdbConn)
    dLineInfo = dict(zip(arrLineInfo,[None] * len(arrLineInfo)))
    logger.info("arrLineInfo : %s" % str(arrLineInfo))
    tsdbConn = None
    tBeginStr = cnf.strSoureDataTimeBegin
    # load last status data     
   
    loadLastStatus(cnf.lastStatusFile,gLastInfoMap)
    lastBeginTime = gLastInfoMap[EVENTLOGTAG]
    if lastBeginTime :
        tBeginStr = lastBeginTime    
    gtBegin = gTC.strTime2Int(tBeginStr)
    beginMonthDay = datetime.date.fromtimestamp(gtBegin).replace(day=1)
    strBeginMonthDay = str(beginMonthDay) + " 00:00:00"
    gtBegin = gTC.strTime2Int(strBeginMonthDay)
    curMonthDay = datetime.date.today().replace(day=1)
    gtEnd = gTC.strTime2Int(str(curMonthDay)+" 00:00:00")
    
    doDataClean(cnf,strBeginMonthDay)    
    totalPointsNum = 0
    
    logger.info("strBeginMonthDay : %s" % strBeginMonthDay)
    # data analysis
    daySeconds = 3600 * 24
    tDayBegin = gtBegin 
    while True :
        try :
            if tDayBegin >= gtEnd : break
            nextMonthDay = beginMonthDay + relativedelta.relativedelta(months=1)
            strNextMonthDay = str(nextMonthDay)+" 00:00:00"
            tNextMonthDay = gTC.strTime2Int(strNextMonthDay)
            msg = "[%s,%s)" % (beginMonthDay,nextMonthDay)
            logger.info(msg)
            for lineId in arrLineInfo :             
                totalPointsNum += evnetlogStatis(gTC,lineId,tDayBegin,tNextMonthDay)
            
            gLastInfoMap[EVENTLOGTAG] = strNextMonthDay
            dumpLastStatus(cnf.lastStatusFile,gLastInfoMap)
            tDayBegin = tNextMonthDay
            beginMonthDay = nextMonthDay
        except :
            errMsg = traceback.format_exc()
            print errMsg
            logger.error(errMsg)
            time.sleep(60)
    gTC.dCache.clear()     
    msg = "totalPointsNum : %d" % totalPointsNum 
    logger.info(msg)
    logger.info("deviceDataAnalysis end")
    return None

if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')      
    if len(sys.argv) < 2 :
        print "usage : %s default.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    print "config file :",fname
    
    cnf = ConfigData(fname)
    cnf.show()
    
    if cnf.logEnable :
        initLogger(cnf.logFilePath,cnf.logFileMaxSize,cnf.logMaxFileNum,cnf.logLevel)
    
    #initConsolLogger()
    createDaemon(dataAnalysisMain,cnf.daemon) 

    raw_input("#" * 30)
else :
    print "load %s module " % os.path.basename(__file__).split(".")[0]

    
    
    
    
    
    
