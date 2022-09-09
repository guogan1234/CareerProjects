#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,platform,time,datetime,traceback
import json,math,logging
from DACommon import ConfigData,ScheduleBase
from DACommon import TimeConvert,getPgsqlConn
from DACommon import createDaemon,initLogger
import TSDBCMN
from TSDBCMN import getTsdbConn,ObjTsdbItemBase
from TSDBCMN import fmtStrTimeToRfc3339
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures

logger = logging.getLogger()

def genTsdbQueryFlowBase(dParams,beginTime,endTime,maxPerFetch,offset):    
    fType = dParams["fType"] # fType : line , station
    qPattern = '''select {conditions} from {table} where line_id='{lineid}' ''' 
    if fType == "station" :
        qPattern += ''' and station_id='{stationid}' '''        
    qPattern += ''' and time >= {beginTime} and time < {endTime} '''         
    qPattern += ''' limit {maxPerFetch} offset {offsetNum};'''
    
    dtmp = {
        "conditions" : "*",
        "table" : dParams["tbName"],
        "lineid" : dParams["lineid"], # '8'
        "beginTime" : beginTime,
        "endTime" : endTime,
        "maxPerFetch" : maxPerFetch,
        "offsetNum" : offset
    }
    if fType == "station" :
        dtmp["stationid"] = dParams["stationid"]
    
    qstr = qPattern.format(**dtmp)
    return qstr   
    

def genTsdbQuery_lineFlow(tbName,lineid,beginTime,endTime,maxPerFetch,offset):
    dParams = {
        "fType" : "line",
        "tbName" : tbName,
        "lineid" : lineid,
    }
    qstr = genTsdbQueryFlowBase(dParams,beginTime,endTime,maxPerFetch,offset)
    return qstr   

def genTsdbQuery_stationFlow(tbName,lineid,stationid,beginTime,endTime,maxPerFetch,offset):
    dParams = {
        "fType" : "station",
        "tbName" : tbName,
        "lineid" : lineid,
        "stationid" : stationid,
    }
    qstr = genTsdbQueryFlowBase(dParams,beginTime,endTime,maxPerFetch,offset)    
    return qstr  
    
def getQPattern_line(tbName):
    arrFields = ["line_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into  %s (''' % tbName + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern
    return qPattern

def addQuery_line(arrRet,qPattern,refTmpArr,lineId,direction,tmpDate,sectionType,newDay,nWeeks):
    #print "dDataDetail[tag] :",dDataDetail[tag],
    if len(refTmpArr) == 0 : return
    numFlow = refTmpArr[nWeeks-1]
    #print numFlow
    #for numFlow in refTmpArr:
    if numFlow <= 0 : return           
    dTmp = {
        "line_id" : lineId,
        "direction" : direction,
        "passenger_flow" : numFlow,
        "flow_timestamp" : str(tmpDate),
        "section" : sectionType,
    }            
    qstrTmp = qPattern.format(**dTmp)
    arrRet.append(qstrTmp)
        
def genPgInsert_line(dataTmp,tbName,lineId,sectionType,newDay,nWeeks):  
    tbName1 = "line_time_sharing_predict_flow"
    tbName2 = "line_cumulative_predict_flow"
    
    qPattern1 = getQPattern_line(tbName1)
    qPattern2 = getQPattern_line(tbName2)
   
    directionMap = {"in":0,"out":1}
    arrRet = []
    arrTmp = sorted(dataTmp.items(),key=lambda x:x[0])
    for (section,dDataDetail) in arrTmp:
        #print str(section),":",dDataDetail
        tmpDate = "%s %s" % (newDay , section)
        for tag,direction in  directionMap.iteritems(): 
            refTmpArr = dDataDetail[tag]
            #print "refTmpArr :",refTmpArr
            addQuery_line(arrRet,qPattern1,refTmpArr[0],lineId,direction,tmpDate,sectionType,newDay,nWeeks)
            addQuery_line(arrRet,qPattern2,refTmpArr[1],lineId,direction,tmpDate,sectionType,newDay,nWeeks)
            
    return arrRet
    
def getQPattern_station(tbName):
    arrFields = ["line_id","station_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into  %s (''' % tbName + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
    return qPattern

def addQuery_station(arrRet,qPattern,refTmpArr,lineId,stationId,direction,tmpDate,sectionType):
    for numFlow in refTmpArr:
        if numFlow <= 0 : continue 
        dTmp = {
            "line_id" : lineId,
            "station_id" : stationId,
            "direction" : direction,
            "passenger_flow" : numFlow,
            "flow_timestamp" : str(tmpDate),
            "section" : sectionType,
        }            
        qstrTmp = qPattern.format(**dTmp)
        arrRet.append(qstrTmp)    
    
def genPgInsert_station(dataTmp,tbName,lineId,stationId,sectionType,newDay):    
    tbName1 = "station_time_sharing_predict_flow"
    tbName2 = "station_cumulative_predict_flow"
    
    qPattern1 = getQPattern_station(tbName1)
    qPattern2 = getQPattern_station(tbName2)
   
    directionMap = {"in":0,"out":1}
    arrRet = []
    arrTmp = sorted(dataTmp.items(),key=lambda x:x[0])
    for (section,dDataDetail) in arrTmp:
        #print str(section),":",dDataDetail
        tmpDate = "%s %s" % (newDay , section)
        for tag,direction in  directionMap.iteritems(): 
            refTmpArr = dDataDetail[tag]
            addQuery_station(arrRet,qPattern1,refTmpArr[0],lineId,stationId,direction,tmpDate,sectionType)
            addQuery_station(arrRet,qPattern2,refTmpArr[1],lineId,stationId,direction,tmpDate,sectionType)
                
    return arrRet
        
def genTsdbQuery_clear(tbName,tBegin):
    retQuery = ""  
    tsFlag = ""
    pattern = "delete from {table} where time >= {timeBegin}s ;"       
    retQuery = pattern.format(
        table = tbName,
        timeBegin = tBegin # timestamp in seconds
    )    
    return retQuery
    
def fillSectionItemData(refDRWeekday,tCurDate,item):
    #strCurTime = tCurDate.time().strftime("%H:%M:%S")
    curSection = tCurDate.time()
    if not curSection in refDRWeekday : 
        refDRWeekday[curSection] = {"in" : [],"out" : []}  
    refDRSection = refDRWeekday[curSection] 
        
    #print type(item["time"])
    direction = int(item["direction"])
    #print "direction : ",direction
    passenger_flow = item["passenger_flow"]
    if direction == 0 :
        refDRSection["in"].append(passenger_flow)
    elif direction == 1 :
        refDRSection["out"].append(passenger_flow)   
    
def fillStatisMap(dataPoints,refFlowMap):
    for item in dataPoints:
        #print item
        tCurrent = item["time"]
        sectionType = item["section"]
        if not sectionType in refFlowMap : refFlowMap[sectionType] = {}
        refSecMap = refFlowMap[sectionType]    
                    
        # fill line statis data  
        tCurDate = datetime.datetime.fromtimestamp(tCurrent)
        curWeekday = tCurDate.isoweekday()
        if not curWeekday in refSecMap : refSecMap[curWeekday] = {}
        refDRWeekday = refSecMap[curWeekday] 
        fillSectionItemData(refDRWeekday,tCurDate,item)
                        
    return None 
    
def flusData2pgdb(pgConn,arrInsert):
    for query in arrInsert :
        #print query
        pgConn.execute(query)
    pgConn.commit()
    

def fillLineFlowData(gTC,lineid,tBegin,tEnd,dSourceData):
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
    
    if not lineid in dSourceData : dSourceData[lineid] = {}
    refLineDataMap = dSourceData[lineid]
     
    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)
    
    arrTables = [
        #"line_cumulative_passenger_flow",
        "line_time_sharing_passenger_flow",       
    ]
    
    for tbName in arrTables : 
        if not tbName in refLineDataMap : refLineDataMap[tbName] = {}
        refFlowMap = refLineDataMap[tbName]
        t_offset = 0
        while True :
            t1 = time.time()
            qstr = genTsdbQuery_lineFlow(tbName,lineid,beginTime,endTime,t_maxPerFetch,t_offset)
            print qstr
            data = tsdbConn.query(qstr,epoch="s")
            #data = tsdbConn.query(qstr)        
            dataPoints = list(data.get_points())
            dataLen= len(dataPoints)
            fillStatisMap(dataPoints,refFlowMap)
            print "dataPoints len : ",dataLen
            tdiff = time.time() - t1
            print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
            
            if dataLen == 0 : break
            t_offset += dataLen
   
    print "get line sourcde data ok"
    
    
def fillStationFlowData(gTC,lineid,stationId,tBegin,tEnd,dSourceData):
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
    
    if not lineid in dSourceData : dSourceData[lineid] = {}
    refLineDataMap = dSourceData[lineid]
    if not stationId in refLineDataMap : refLineDataMap[stationId] = {}
    refStationDataMap = refLineDataMap[stationId]

    
    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)
    
    arrTables = [
        #"line_cumulative_passenger_flow",
        #"line_time_sharing_passenger_flow",
        "station_cumulative_passenger_flow",
        "station_time_sharing_passenger_flow"
    ]
    
    for tbName in arrTables : 
        if not tbName in refStationDataMap : refStationDataMap[tbName] = {}
        refFlowMap = refStationDataMap[tbName]
        t_offset = 0
        while True :
            t1 = time.time()
            qstr = genTsdbQuery_stationFlow(tbName,lineid,stationId,beginTime,endTime,t_maxPerFetch,t_offset)
            print qstr
            data = tsdbConn.query(qstr,epoch="s")
            #data = tsdbConn.query(qstr)        
            dataPoints = list(data.get_points())
            dataLen = len(dataPoints)
            fillStatisMap(dataPoints,refFlowMap)
            print "dataPoints len : ",dataLen
            tdiff = time.time() - t1
            print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
            
            if dataLen == 0 : break
            t_offset += dataLen
   
    print "get sourcde data ok"    

def fillStationFlowData2(gTC,lineid,tBegin,tEnd,dSourceData):
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
    
    if not lineid in dSourceData : dSourceData[lineid] = {}
    refLineDataMap = dSourceData[lineid]
    
    #if not stationId in refLineDataMap : refLineDataMap[stationId] = {}
    #refStationDataMap = refLineDataMap[stationId]

    
    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)
    
    arrTables = [
        #"line_cumulative_passenger_flow",
        #"line_time_sharing_passenger_flow",
     #   "station_cumulative_passenger_flow",
        "station_time_sharing_passenger_flow"
    ]
    
    arrPoints = []
    for tbName in arrTables : 
        #if not tbName in refStationDataMap : refStationDataMap[tbName] = {}
        #refFlowMap = refStationDataMap[tbName]
        t_offset = 0
        while True :
            t1 = time.time()
            qstr = genTsdbQuery_lineFlow(tbName,lineid,beginTime,endTime,t_maxPerFetch,t_offset)
            #qstr = genTsdbQuery_stationFlow(tbName,lineid,stationId,beginTime,endTime,t_maxPerFetch,t_offset)
            print qstr
            data = tsdbConn.query(qstr,epoch="s")
            #data = tsdbConn.query(qstr)        
            dataPoints = list(data.get_points())
            dataLen = len(dataPoints)
            #fillStatisMap(dataPoints,refFlowMap)
            print "dataPoints len : ",dataLen
            tdiff = time.time() - t1
            print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
            
            if dataLen == 0 : break
            arrPoints += dataPoints
            t_offset += dataLen    
   
        for item in arrPoints:
            #print item
            tCurrent = item["time"]
            sectionType = item["section"]
            stationId = item["station_id"]
            
            if not stationId in refLineDataMap : refLineDataMap[stationId] = {}
            refStationDataMap = refLineDataMap[stationId]
            
            if not tbName in refStationDataMap : refStationDataMap[tbName] = {}
            refFlowMap = refStationDataMap[tbName]
            
            if not sectionType in refFlowMap : refFlowMap[sectionType] = {}
            refSecMap = refFlowMap[sectionType]    
                        
            # fill line statis data  
            tCurDate = datetime.datetime.fromtimestamp(tCurrent)
            curWeekday = tCurDate.isoweekday()
            if not curWeekday in refSecMap : refSecMap[curWeekday] = {}
            refDRWeekday = refSecMap[curWeekday] 
            fillSectionItemData(refDRWeekday,tCurDate,item)       

    print "get sourcde data ok"        
    
def showPredictData(dPredictData):
    print "predict data :"
    
    for lineId,dLineData in dPredictData.iteritems():
        for stationId,dStationData in dLineData.iteritems():
            for tbName,dTbData in dStationData.iteritems():
                print tbName ,           
                for sectionType,dSecData in dTbData.iteritems():
                    print sectionType,               
                    for weekday,dWeekdayData in dSecData.iteritems():
                        print weekday                    
                        arrTmp = sorted(dWeekdayData.items(),key=lambda x:x[0])
                        for (section,dDataDetail) in arrTmp:
                            print str(section),":",dDataDetail
    return None

def flushLinePredictData(cnf,dPredictData):
    '''flush data to database'''
    msg = "flushLinePredictData begin"
    logger.info(msg) 
    arrInsert = []
    curDay = datetime.date.today()  
    for lineId,dLineData in dPredictData.iteritems():
        for tbName,dTbData in dLineData.iteritems():
            print tbName ,           
            for sectionType,dSecData in dTbData.iteritems():
                print sectionType,
                for num in range(1,7 * cnf.predictWeeks + 1):
                    newDay = curDay + datetime.timedelta(days=num)
                    tWeekday = newDay.isoweekday()
                    if tWeekday in dSecData :
                        dataTmp = dSecData[tWeekday]
                        #print tbName,sectionType,newDay,dataTmp
                        print tbName,sectionType,newDay,dataTmp
                        arrInsert += genPgInsert_line(dataTmp,tbName,lineId,sectionType,newDay,cnf.predictWeeks)
                    
    pgConn = getPgsqlConn(cnf)
    #print "\n".join(arrInsert)
    flusData2pgdb(pgConn,arrInsert)
    pgConn.close()
    
    msg = "flushLinePredictData end"
    logger.info(msg)
    
def flushStationPredictData(cnf,dPredictData):
    msg = "flushStationPredictData begin" 
    logger.info(msg)
   # flush data to database
    arrInsert = []
    curDay = datetime.date.today()  
    for lineId,dLineData in dPredictData.iteritems():
        for stationId,dStationData in dLineData.iteritems():
            for tbName,dTbData in dStationData.iteritems():
                print tbName ,           
                for sectionType,dSecData in dTbData.iteritems():
                    print sectionType,
                    for num in range(1,8):
                        newDay = curDay + datetime.timedelta(days=num)
                        tWeekday = newDay.isoweekday()
                        if tWeekday in dSecData :
                            dataTmp = dSecData[tWeekday]
                            #print tbName,sectionType,newDay,dataTmp
                            print tbName,sectionType,newDay,dataTmp
                            arrInsert += genPgInsert_station(dataTmp,tbName,lineId,stationId,sectionType,newDay)
    pgConn = getPgsqlConn(cnf)
    #print "\n".join(arrInsert)
    flusData2pgdb(pgConn,arrInsert)
    pgConn.close()
    
    msg = "flushStationPredictData done" 
    logger.info(msg)
    
def doPredicDataClean(cnf):
    msg = "do doPredicDataClean begin"
    logger.info(msg)
    arrPgdbQuery = [
        "delete from station_time_sharing_predict_flow ;",
        "delete from station_cumulative_predict_flow ;",
        "delete from line_time_sharing_predict_flow ;",
        "delete from line_cumulative_predict_flow ;",    
    ]        
    print "\n".join(arrPgdbQuery)
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrPgdbQuery)
    pgConn.close()
 
    msg = "do doPredicDataClean done"
    logger.info(msg)
   

def doPredict(arrSourceData,arrPredict,nWeeks=1):
    if len(arrSourceData) == 0 : return None
    #arrSourceData = [15, 33, 30, 29, 34, 33, 40, 38, 31, 29, 28]
    #print arrSourceData
    N = len(arrSourceData)+1
    arrX = range(1,N)
    arrY = arrSourceData

    X = np.array(arrX).reshape(-1,1)
    y = np.array(arrY).reshape(-1,1)

    qf = PolynomialFeatures(degree=6) 
    qModel = LinearRegression() 
    qModel.fit(qf.fit_transform(X), y) 

    #X_predict = np.linspace(100, 200, 100) 
    X_predict = np.array(range(1,N+2+nWeeks)).reshape(-1,1)
    X_predict_result = qModel.predict(qf.transform(X_predict.reshape(X_predict.shape[0], 1)))
    #X_predict_result = X_predict_result.reshape(-1,1)
    #print "X_predict_result : ",X_predict_result
    arrPData = X_predict_result[-(2+nWeeks):][0:nWeeks]
    #print "arrSourceData : ",arrSourceData,"X_predict_result : ",X_predict_result,arrPData
    tMax = max(arrSourceData)
    for pData in arrPData:
        pData = pData[0]
        avgData = sum(arrSourceData) / (len(arrSourceData) * 1.0)
        #print pData,avgData
        #if pData <= 0 : pData = avgData
        if pData <= 0 : pData = arrSourceData[-1]
        #if math.fabs(pData-avgData) > avgData * 0.8 : pData = avgData 
        #if pData > tMax + avgData : pData = avgData 
        if pData > tMax * 1.1  : pData = avgData 
        pData = int(pData)
        #pData = int((pData / 10) * 10)
        arrPredict[0].append(pData) # predict data
    return None

def doPredictDetail(dTbData,refDTbPData,nWeeks):   
    for sectionType,dSecData in dTbData.iteritems():
        print sectionType,
        if not sectionType in refDTbPData : refDTbPData[sectionType] = {}
        refSectionPMap = refDTbPData[sectionType]
        for weekday,dWeekdayData in dSecData.iteritems():
            print weekday
            if not weekday in refSectionPMap : refSectionPMap[weekday] = {}            
            refWeekdayPMap = refSectionPMap[weekday] 
            arrTmp = sorted(dWeekdayData.items(),key=lambda x:x[0])
            dPredictAcc = {}
            t = 0
            accArr = [0] * nWeeks
            for (section,dDataDetail) in arrTmp:
                #print str(section),":",dDataDetail
                if not section in dPredictAcc : 
                    dPredictAcc[section] = [0] * nWeeks
                if not section in refWeekdayPMap : 
                    refWeekdayPMap[section] = {"in" : [[],[]],"out":[[],[]]}
                refSecDetailPMap = refWeekdayPMap[section]
                for tag in ["in","out"]:
                    doPredict(dDataDetail[tag],refSecDetailPMap[tag],nWeeks)
                    refTagArr = refSecDetailPMap[tag][0]
                    if len(refTagArr) == 0 : continue
                   # print dPredictAcc[section]
                    #print refTagArr
                    for i in range(nWeeks):
                        #print "i=",i
                        dPredictAcc[section][i] = dPredictAcc[section][i] + refTagArr[i]
                        t+=refTagArr[i]
                        accArr[i] += refTagArr[i]
                    #print "dPredictAcc[",section,"] :",dPredictAcc[section],"t=",t
                    #refSecDetailPMap[tag][1] += dPredictAcc[section]                    
                    refSecDetailPMap[tag][1] += list(accArr)
                    #print "refSecDetailPMap[tag][1] :",refSecDetailPMap[tag][1]
    return None    
 
def predictLineRealteData(cnf,gTC,dTbConvert,arrLineInfo):
    msg = "predictLineRealteData begin"
    logger.info(msg)
    tBeginStr = cnf.strSoureDataTimeBegin 
    gtBegin = gTC.strTime2Int(tBeginStr)         
    
    # data predict
    dSourceData = {}        
    dPredictData = {}
    
    daySeconds = 3600 * 24
    dayBegin = gtBegin 
    while  time.time() - dayBegin > daySeconds   :
        try :
            if (time.time() - dayBegin) < daySeconds :
                break
            nextDay = dayBegin + daySeconds

            for lineId in arrLineInfo :
                fillLineFlowData(gTC,lineId,dayBegin,nextDay,dSourceData)
            #print "dSourceData : ",dSourceData                            
            
            dayBegin = nextDay
        except :
            logger.error("error occur :")
            logger.error(traceback.format_exc())
            time.sleep(60) 
            
    for lineId,dLineData in dSourceData.iteritems():
        print lineId,
        if not lineId in dPredictData : dPredictData[lineId] = {}
        refLinePMap=dPredictData[lineId]
        for tbName,dTbData in dLineData.iteritems():
            print tbName ,
            tbName = dTbConvert[tbName]                
            if not tbName in refLinePMap : refLinePMap[tbName] = {}
            refDTbPData = refLinePMap[tbName]
            doPredictDetail(dTbData,refDTbPData,cnf.predictWeeks)
            
    #showPredictData(dPredictData)  
    flushLinePredictData(cnf,dPredictData)
    msg = "predictLineRealteData end"
    logger.info(msg)

def predictStationRealteData(cnf,gTC,dTbConvert,dStationInfo):
    msg = "predictStationRealteData begin"
    logger.info(msg)
    tBeginStr = cnf.strSoureDataTimeBegin 
    gtBegin = gTC.strTime2Int(tBeginStr)         
    
    # data predict
    dSourceData = {}        
    dPredictData = {}
    
    daySeconds = 3600 * 24
    dayBegin = gtBegin 
    while  time.time() - dayBegin > daySeconds   :
        try :
            if (time.time() - dayBegin) < daySeconds :
                time.sleep(60 * 30)
                continue
            nextDay = dayBegin + daySeconds
            
            for lineId,arrStation in dStationInfo.iteritems():
                #for stationId in arrStation : 
                    #fillStationFlowData(gTC,lineId,stationId,dayBegin,nextDay,dSourceData)
                fillStationFlowData2(gTC,lineId,dayBegin,nextDay,dSourceData)
            #print dSourceData                            
            
            dayBegin = nextDay
        except :
            logger.error("error occur :")
            logger.error(traceback.format_exc())
            time.sleep(60) 
       
    for lineId,dLineData in dSourceData.iteritems():
        print lineId,
        if not lineId in dPredictData : dPredictData[lineId] = {}
        refLinePMap=dPredictData[lineId]
        
        for stationId,dStationData in dLineData.iteritems():
            if not stationId in refLinePMap : refLinePMap[stationId] = {}
            refStationMap = refLinePMap[stationId]
            for tbName,dTbData in dStationData.iteritems():
                print tbName ,
                tbName = dTbConvert[tbName]                
                if not tbName in refStationMap : refStationMap[tbName] = {}
                refDTbPData = refStationMap[tbName]
                doPredictDetail(dTbData,refDTbPData,cnf.predictWeeks)
    
    #showPredictData(dPredictData)
    flushStationPredictData(cnf,dPredictData)
    msg = "predictStationRealteData end"
    logger.info(msg)

def dataPredictMain():
    logger.info("dataPredictMain begin")
    cnf = ConfigData()
    # init 
    gTC = TimeConvert()
    
    dTbConvert = {
        "line_cumulative_passenger_flow" : "line_cumulative_predict_flow",
        "line_time_sharing_passenger_flow" : "line_time_sharing_predict_flow",
        "station_cumulative_passenger_flow" : "station_cumulative_predict_flow",
        "station_time_sharing_passenger_flow" : "station_time_sharing_predict_flow",
    } 
    doPredicDataClean(cnf)
    tsdbConn = getTsdbConn(cnf)
    arrLineInfo = TSDBCMN.getLineInfo(tsdbConn)
    dStationInfo = TSDBCMN.getStationInfo(tsdbConn)
    predictLineRealteData(cnf,gTC,dTbConvert,arrLineInfo)
    predictStationRealteData(cnf,gTC,dTbConvert,dStationInfo)
    logger.info("dataPredictMain end")
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
    createDaemon(dataPredictMain,cnf.daemon)
else :
    print "load afcDataPredict module"


