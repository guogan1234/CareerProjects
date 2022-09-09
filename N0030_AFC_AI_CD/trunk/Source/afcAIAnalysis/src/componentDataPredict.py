#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,platform,time,datetime,traceback
import json,math,logging,random
from dateutil import relativedelta
from DACommon import ConfigData,ScheduleBase
from DACommon import TimeConvert,getPgsqlConn
from DACommon import createDaemon,initLogger
from DACommon import loadLastStatus,dumpLastStatus
import TSDBCMN
from TSDBCMN import getTsdbConn,ObjTsdbItemBase
from TSDBCMN import fmtStrTimeToRfc3339
import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures

logger = logging.getLogger()
gPredictTag = "predictArr"

COMPONETPREDICTTIME = "componetLastPredictTime"

gLastInfoMap = {
    COMPONETPREDICTTIME : "",   
} 

def genTsdbQueryFlowBase(tbName,beginTime,endTime,maxPerFetch,offset):   
    qPattern = '''select {conditions} from {table} where  '''     
    qPattern += ''' time >= {beginTime} and time < {endTime} '''         
    qPattern += ''' limit {maxPerFetch} offset {offsetNum};'''
    
    dtmp = {
        "conditions" : "*",
        "table" : tbName,        
        "beginTime" : beginTime,
        "endTime" : endTime,
        "maxPerFetch" : maxPerFetch,
        "offsetNum" : offset
    }

    qstr = qPattern.format(**dtmp)
    return qstr   
    

def genTsdbQuery_consume(tbName,beginTime,endTime,maxPerFetch,offset):
    qstr = genTsdbQueryFlowBase(tbName,beginTime,endTime,maxPerFetch,offset)
    return qstr   

def genPgInsert_consumePredict(tbName,component_type,component_name,predictNum,nextMonthDay): 
    arrFields = ["component_type","component_name","consumption","analysis_timestamp"]    
    qPattern = '''insert into  %s (''' % tbName + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   

    dTmp = {
        "component_type" : component_type,
        "component_name" : component_name,
        "consumption" : predictNum,
        "analysis_timestamp" : str(nextMonthDay),
    }            
    qstrTmp = qPattern.format(**dTmp)
    return qstrTmp
    
def genTsdbQuery_clear(tbName,tBegin):
    retQuery = ""  
    tsFlag = ""
    pattern = "delete from {table} where time >= {timeBegin}s ;"       
    retQuery = pattern.format(
        table = tbName,
        timeBegin = tBegin # timestamp in seconds
    )    
    return retQuery    
    
def fillStatisData(dataPoints,refMap):
    for item in dataPoints:
        #print item
        ctype = item["component_type"]
        if not ctype in refMap : 
            refMap[ctype] = {
                "component_name" : item["component_name"],
                "consumptionArr" : []
            } 
        refArr = refMap[ctype]["consumptionArr"]
        refArr.append(item["consumption"])
    return None 
    
def flusData2pgdb(pgConn,arrInsert):
    for query in arrInsert :
        print query
        pgConn.execute(query)
    pgConn.commit()
    

def fillConsumeData(gTC,tBegin,tEnd,dSourceData):
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
    
    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)
    
    arrTables = [
        "component_consume_analysis",     
    ]
    
    for tbName in arrTables : 
        if not tbName in dSourceData : dSourceData[tbName] = {}
        refConsumeMap = dSourceData[tbName]
        t_offset = 0
        while True :
            t1 = time.time()
            qstr = genTsdbQuery_consume(tbName,beginTime,endTime,t_maxPerFetch,t_offset)
            print qstr
            data = tsdbConn.query(qstr,epoch="s")
            #data = tsdbConn.query(qstr)        
            dataPoints = list(data.get_points())
            dataLen= len(dataPoints)
            fillStatisData(dataPoints,refConsumeMap)
            print "dataPoints len : ",dataLen
            tdiff = time.time() - t1
            print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
            
            if dataLen == 0 : break
            t_offset += dataLen
   
    print "get line sourcde data ok"
    
def getCurMonth(gTC):
    curMonthDay = datetime.date.today().replace(day=1)
    strCurMonthDay = str(curMonthDay) + " 00:00:00"
    tCurMonthDay = gTC.strTime2Int(strCurMonthDay)
    return curMonthDay,tCurMonthDay

def getNextMonth(gTC,curMonthDay,num=1):
    nextMonthDay = curMonthDay + relativedelta.relativedelta(months=num)
    strNextMonthDay = str(nextMonthDay)+" 00:00:00"
    tNextMonthDay = gTC.strTime2Int(strNextMonthDay)
    return nextMonthDay,tNextMonthDay

def flushComponetPredictData(cnf,gTC,dPredictData,dTbConvert):
    '''flush data to database'''
    msg = "flushComponetPredictData begin"
    logger.info(msg) 
    arrInsert = []
    # cur Month 
    curMonthDay,tCurMonthDay = getCurMonth(gTC)
    
    for tbName,dTbData in dPredictData.iteritems():
        print tbName,dTbData 
        newTbName = dTbConvert[tbName]
        for component_type,dTmp in dTbData.iteritems():
            component_name = dTmp["component_name"]
            refPredictArr = dTmp[gPredictTag] 
            for i,predictNum in enumerate(refPredictArr) :
                nextMonthDay,_ = getNextMonth(gTC,curMonthDay,i+1)
                query = genPgInsert_consumePredict(newTbName,component_type,component_name,predictNum,nextMonthDay)
                arrInsert.append(query)
                
    pgConn = getPgsqlConn(cnf)
    #print "\n".join(arrInsert)
    flusData2pgdb(pgConn,arrInsert)
    pgConn.close()
    
    msg = "flushComponetPredictData end"
    logger.info(msg)
    

def doPredicDataClean(cnf):
    msg = "do doPredicDataClean begin"
    logger.info(msg)
    arrPgdbQuery = [
        "delete from component_consume_predict ;", 
    ]        
    print "\n".join(arrPgdbQuery)
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrPgdbQuery)
    pgConn.close()
 
    msg = "do doPredicDataClean done"
    logger.info(msg)
   

def doPredict(arrSourceData,arrPredict,nPredictNum=1):
    if len(arrSourceData) == 0 : return None
    #arrSourceData = [15, 33, 30, 29, 34, 33, 40, 38, 31, 29, 28]
    #print arrSourceData
    N = len(arrSourceData)+1
    arrX = range(1,N)
    arrY = arrSourceData

    X = np.array(arrX).reshape(-1,1)
    y = np.array(arrY).reshape(-1,1)

    qf = PolynomialFeatures(degree=3) 
    qModel = LinearRegression() 
    qModel.fit(qf.fit_transform(X), y) 

    #X_predict = np.linspace(100, 200, 100) 
    X_predict = np.array(range(1,N+2+nPredictNum)).reshape(-1,1)
    X_predict_result = qModel.predict(qf.transform(X_predict.reshape(X_predict.shape[0], 1)))
    #X_predict_result = X_predict_result.reshape(-1,1)
    #print "X_predict_result : ",X_predict_result
    arrPData = X_predict_result[-(2+nPredictNum):][0:nPredictNum]
    #print "arrSourceData : ",arrSourceData,"X_predict_result : ",X_predict_result,arrPData
    for pData in arrPData:
        pData = pData[0]
        srcLen = len(arrSourceData)
        avgData = sum(arrSourceData) / (srcLen * 1.0)
        #print pData,avgData
        #if pData <= 0 : pData = avgData
        fillData = arrSourceData[-1*random.randint(1,srcLen)]
        #fillData = arrSourceData[-1]
        if pData <= 0 : pData = fillData # arrSourceData[-1]
        if pData >= 3 * avgData : pData = fillData #arrSourceData[-1*random.randint(1,srcLen)]
        #if math.fabs(pData-avgData) > avgData * 0.5 : pData = avgData 
        pData = int(pData)
        #pData = int((pData / 10) * 10)
        arrPredict.append(pData)
    return None

def doPredictDetail(dTbData): 
    for component_type,dTmp in dTbData.iteritems():        
        consumptionArr = dTmp["consumptionArr"]        
        if not gPredictTag in dTmp : dTmp[gPredictTag] = []
        refPredictArr = dTmp[gPredictTag]         
        # do predict
        doPredict(consumptionArr,refPredictArr,6)
        print "component_type : ",component_type
        print "consumptionArr : ",consumptionArr
        print "refPredictArr : ",refPredictArr
        print "*" * 30
    return None    
 
def predictComponetData(cnf,gTC,dTbConvert):
    msg = "predictComponetData begin"
    logger.info(msg)
    tBeginStr = cnf.strSoureDataTimeBegin 
    gtBegin = gTC.strTime2Int(tBeginStr)         
    beginMonthDay = datetime.date.fromtimestamp(gtBegin).replace(day=1)
    strBeginMonthDay = str(beginMonthDay) + " 00:00:00"
    gtBegin = gTC.strTime2Int(strBeginMonthDay)
    curMonthDay = datetime.date.today().replace(day=1)
    strCurMonthDay = str(curMonthDay)+" 00:00:00"
    gtEnd = gTC.strTime2Int(strCurMonthDay) 
    
    # data predict
    dSourceData = {}
    
    tCurMonthDay = gtBegin   
    while True :
        try :
            if tCurMonthDay >= gtEnd : break 
            nextMonthDay = beginMonthDay + relativedelta.relativedelta(months=1)
            strNextMonthDay = str(nextMonthDay)+" 00:00:00"
            tNextMonthDay = gTC.strTime2Int(strNextMonthDay)
            
            # do predict
            fillConsumeData(gTC,tCurMonthDay,tNextMonthDay,dSourceData)
            #print "dSourceData : ",dSourceData                            

            tCurMonthDay = tNextMonthDay
            beginMonthDay = nextMonthDay                  
        except :
            logger.error("error occur :")
            logger.error(traceback.format_exc())
            print traceback.format_exc()
            time.sleep(60) 
     
    for tbName,dTbData in dSourceData.iteritems():
        print tbName,dTbData
        doPredictDetail(dTbData)

    flushComponetPredictData(cnf,gTC,dSourceData,dTbConvert)
    
    # dump last status
    gLastInfoMap[COMPONETPREDICTTIME] = strCurMonthDay
    dumpLastStatus(cnf.lastStatusFile,gLastInfoMap)
    
    msg = "predictComponetData end"
    logger.info(msg)

def dataPredictMain():
    logger.info("dataPredictMain begin")
    cnf = ConfigData()
    gTC = TimeConvert()
    
    dTbConvert = {
        "component_consume_analysis" : "component_consume_predict",
    } 
    
    loadLastStatus(cnf.lastStatusFile,gLastInfoMap)
    strLastPredictTime = gLastInfoMap[COMPONETPREDICTTIME]
    curMonthDay,tCurMonthDay = getCurMonth(gTC)
    tLastPredictTime = 0
    print "strLastPredictTime :",strLastPredictTime
    if len(strLastPredictTime) > 0 :
        tLastPredictTime = gTC.strTime2Int(strLastPredictTime) 
    print tCurMonthDay,curMonthDay,tLastPredictTime
    if tCurMonthDay > tLastPredictTime :
        doPredicDataClean(cnf)     
        predictComponetData(cnf,gTC,dTbConvert)
    else :
        print "already predicted,lastPredictTime :",strLastPredictTime
    
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


