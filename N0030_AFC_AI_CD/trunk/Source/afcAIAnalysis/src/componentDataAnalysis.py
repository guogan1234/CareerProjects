#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,platform,time,datetime,traceback
import json
from dateutil import relativedelta
import logging,logging.handlers
from DACommon import ConfigData,createDaemon
from DACommon import initLogger,TimeConvert,getPgsqlConn
from DACommon import loadLastStatus,dumpLastStatus
from DACommon import ObjParseDeviceInfo
import TSDBCMN 
from TSDBCMN import getTsdbConn,ObjTsdbItemBase
from TSDBCMN import fmtStrTimeToRfc3339

logger = logging.getLogger()

COMPONETTIME = "componetTimeBegin"

gLastInfoMap = {
    COMPONETTIME : "",   
} 

def genTsdbQuery_consume(tbName,beginTime,endTime,maxPerFetch,offset):
    qPattern = '''select {conditions} from {table} ''' 
    qPattern += ''' where time >= {beginTime} and time < {endTime} ''' 
    qPattern += ''' limit {maxPerFetch} offset {offsetNum};'''
    #tbName = "ObjTransaction"
    arrLables = ['component_serial_no','component_type','component_name','consuming_datetime']
    
    qstr = qPattern.format(
        conditions = ",".join(arrLables),
        table = tbName,
        beginTime = beginTime,
        endTime = endTime,
        maxPerFetch = maxPerFetch,
        offsetNum = offset
    )
    return qstr   

def genTsdbQuery_install(tbName,beginTime,endTime,maxPerFetch,offset):
    qPattern = '''select {conditions} from {table} ''' 
    qPattern += ''' where time >= {beginTime} and time < {endTime} ''' 
    qPattern += ''' limit {maxPerFetch} offset {offsetNum};'''
    #tbName = "ObjTransaction"
    arrLables = ['component_serial_no','component_type','component_name',
                 'equipment_id','install_person','install_datetime']
    
    qstr = qPattern.format(
        conditions = ",".join(arrLables),
        table = tbName,
        beginTime = beginTime,
        endTime = endTime,
        maxPerFetch = maxPerFetch,
        offsetNum = offset
    )
    return qstr   
 
def genPgdbQuery_consuming(tc,dRConsume,tAna,refArrResult):
    tbName = "component_consume_analysis"
    arrFields = ["component_type","component_name","consumption","analysis_timestamp"]
    qPattern = '''insert into component_consume_analysis (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []   
    strAnaTime = tc.intTime2Str(tAna)
    for componetType,dtmp in dRConsume.iteritems():
        #print t,dtmp

        dTmp = {
            "component_type" : componetType,
            "component_name" : dtmp["name"],
            "consumption" : dtmp["consumption"],
            "analysis_timestamp" : strAnaTime,
        }

        qstrTmp = qPattern.format(**dTmp)
        arrQuery.append(qstrTmp) 
        refArrResult.append(dTmp)        
      
    return arrQuery
    
def genPgdbQuery_install(tc,dRInstall,tAna,refArrResult):
    tbName = "component_destination_analysis"   
    
    arrFields = ["line_id","station_id","device_id","maintenance_compay",
        "maintenance_person","consumption","analysis_timestamp"]
    qPattern = '''insert into component_destination_analysis (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []   
    strAnaTime = tc.intTime2Str(tAna)
    for equipment_id,dEq in dRInstall.iteritems():
        #print t,dtmp
        # get lineid station_id from equipment_id
        objDevice = ObjParseDeviceInfo(equipment_id)
        line_id = objDevice.getLineId()
        station_id = objDevice.getStationId()
        for person,dPerson in dEq.iteritems():
            compay = dPerson["maintenance_compay"]
            dTmp = {
                "line_id" : line_id,
                "station_id" : station_id,
                "device_id" : equipment_id,
                "maintenance_compay" : compay,
                "maintenance_person" : person,
                "consumption" : dPerson["consumption"],
                "analysis_timestamp" : strAnaTime,
            }

        qstrTmp = qPattern.format(**dTmp)
        arrQuery.append(qstrTmp) 
        refArrResult.append(dTmp)        
      
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
        
def genTsdbQuery_clear(tbName,tBegin):
    retQuery = ""  
    tsFlag = ""
    pattern = "delete from {table} where time >= {timeBegin}s ;"       
    retQuery = pattern.format(
        table = tbName,
        timeBegin = tBegin # timestamp in seconds
    )    
    return retQuery
    
def fillConsumeMap(dataPoints,dRComponent):
    for item in dataPoints:
        print item
        #curLineTime = item['time']
        component_type = item["component_type"]
        component_name = item["component_name"]
        if not component_type in dRComponent :
            dRComponent[component_type] = {
                "name" : component_name,
                "consumption" : 0,
            }
        refMap = dRComponent[component_type]
        refMap["consumption"] += 1
                       
    return None 

def fillInstallMap(dataPoints,dRInstall):
    for item in dataPoints:
        print item
        #curLineTime = item['time']
        equipment_id = item["equipment_id"]
        install_person = item["install_person"]
        if not equipment_id in dRInstall :
            dRInstall[equipment_id] = {}

        refEqMap = dRInstall[equipment_id]
        if not install_person in refEqMap :
            refEqMap[install_person] = {
                "maintenance_compay" : "",
                "consumption" : 0
            }
        refPerson = refEqMap[install_person]
        refPerson["consumption"] += 1
                       
    return None 
    
def flusData2pgdb(pgConn,arrQuery):
    for query in arrQuery :
        #print query
        pgConn.execute(query)
    pgConn.commit()
        
class ObjTsdbLineTSPItem(ObjTsdbItemBase):
    def init(self):
        self.table = "line_time_sharing_passenger_flow"

class ObjTsdbLineCPItem(ObjTsdbItemBase):
    def init(self):
        self.table = "line_cumulative_passenger_flow"
        
class ObjDABase():
    def __init__(self,gTC,tsdbConn,cnf,arrData):
        self.proName = "ObjDABase"
        self.tsdbConn = tsdbConn
        self.dbName = tsdbConn.dbName
        self.maxNumPerCommit = cnf.tsdb_max_per_commit
        self.tConvert = gTC
        self.arrData = arrData
        self.timeFields = ["analysis_timestamp"]
        self.timetag = "analysis_timestamp"
        self.init()

    def init(self):
        pass
        
    def updateTime(self,refObj,curLineTime):
        #print "curLineTime : ",curLineTime
        tDay,tTime = curLineTime.split(" ")
        curTimeInt = self.tConvert.strTime2Int(curLineTime)
        refObj.time = curTimeInt * (10**9) 
        #print refObj.time
    
    def fillItemData(self,refItem,objTmp):
        for kName,value in objTmp.iteritems():
            #value = str(value)
            if kName in self.tagFields :
                refItem.tags[kName] = value
            else:
                refItem.fields[kName] = value
        curLineTime = refItem.fields[self.timetag]
        for kName in self.timeFields :
            vtmp = refItem.fields[kName]
            refItem.fields[kName] = fmtStrTimeToRfc3339(vtmp)
        self.updateTime(refItem,curLineTime)

    def doDataWrite(self):
        nCount = 0
        for i,objTmp in enumerate(self.arrData):
            otItem = self.curObjItem()            
            self.fillItemData(otItem,objTmp)
            tMap = otItem.toDict()
            #print "toDict : ",tMap
            self.json_body.append(tMap)
            if (i+1) % self.maxNumPerCommit == 0 :
                self.tsdbConn.write(self.json_body)
                print self.json_body[-1]
                self.json_body = []
                print "commit %d" % i
            nCount = i

        self.tsdbConn.write(self.json_body)
        self.json_body = []
        
    def run(self):
        print "[%s] run" % self.proName
        self.tsdbConn.createDb(self.dbName)
        self.doDataWrite()
        print "[%s] task done" % self.proName

class ObjTsdbConsumeItem(ObjTsdbItemBase):
    def init(self):
        self.table = "component_consume_analysis"
        
class ObjDAConsume(ObjDABase):
    def init(self):   
        self.proName = "ObjDAConsume"
        self.json_body = []
        self.tagFields = ["component_type",]
        self.curObjItem = ObjTsdbConsumeItem    
        
def consumeDataStatis(gTC,tBegin,tEnd):
    dRConsume = {}
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)
    
    tbName = "component_consuming"

    t_offset = 0
    while True :
        t1 = time.time()
        qstr = genTsdbQuery_consume(tbName,beginTime,endTime,t_maxPerFetch,t_offset)
        print qstr
        data = tsdbConn.query(qstr,epoch="s")
        #data = tsdbConn.query(qstr)        
        dataPoints = list(data.get_points())
        dataLen= len(dataPoints)   
        
        print "dataPoints len : ",dataLen
        tdiff = time.time() - t1
        print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
        
        fillConsumeMap(dataPoints,dRConsume)
        
        if dataLen == 0 : break
        t_offset += dataLen

    arrQuery,arrTsdbData = [],[]  
    
    # gen new insert sql    
    arrQuery += genPgdbQuery_consuming(gTC,dRConsume,tBegin,arrTsdbData)
    
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrQuery)
    pgConn.close()
     
    ObjDAConsume(gTC,tsdbConn,cnf,arrTsdbData).run()
    
    print "consumeDataStatis done"
    
def installDataStatis(gTC,tBegin,tEnd):
    dRInstall = {}
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)
    
    tbName = "component_install"

    t_offset = 0
    while True :
        t1 = time.time()
        qstr = genTsdbQuery_install(tbName,beginTime,endTime,t_maxPerFetch,t_offset)
        print qstr
        data = tsdbConn.query(qstr,epoch="s")    
        dataPoints = list(data.get_points())
        dataLen= len(dataPoints)   
        
        print "dataPoints len : ",dataLen
        tdiff = time.time() - t1
        print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
       
        fillInstallMap(dataPoints,dRInstall)
        
        if dataLen == 0 : break
        t_offset += dataLen

    arrQuery,arrTsdbData = [],[]  
    
    # gen new insert sql    
    arrQuery += genPgdbQuery_install(gTC,dRInstall,tBegin,arrTsdbData)
    
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrQuery)
    pgConn.close()
    
    print "installDataStatis done"    
    
def doDataClean(cnf,tBeginStr,tBegin):
    msg = "do doDataClean begin"
    logger.info(msg)
    # gen clear data sql
    arrPgdbQuery = []
    arrTsdbQuery = []
    
    dTableInfo = {
        "component_consume_analysis" : "analysis_timestamp",
        "component_destination_analysis" : "analysis_timestamp",
    }
    for tbName,tField in dTableInfo.iteritems():
        arrPgdbQuery += [genPgdbQuery_clear(tbName,tField,tBeginStr)]
        arrTsdbQuery += [genTsdbQuery_clear(tbName,tBegin)]
        
    print "\n".join(arrPgdbQuery)
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrPgdbQuery)
    pgConn.close()
    
    print "\n".join(arrTsdbQuery)
    tsdbConn = getTsdbConn(cnf)
    for item in arrTsdbQuery :
        tsdbConn.query(item)

    msg = "do doDataClean done"
    logger.info(msg)
    return None
    
def dataAnalysisMain():
    cnf = ConfigData()
    logger.info("componetDataAnalysis start")
        
    # init 
    gTC = TimeConvert()
    tBeginStr = cnf.strSoureDataTimeBegin
    # load last status data     
   
    loadLastStatus(cnf.lastStatusFile,gLastInfoMap)
    lastBeginTime = gLastInfoMap[COMPONETTIME]
    if lastBeginTime :
        tBeginStr = lastBeginTime    
    gtBegin = gTC.strTime2Int(tBeginStr)
    beginMonthDay = datetime.date.fromtimestamp(gtBegin).replace(day=1)
    strBeginMonthDay = str(beginMonthDay) + " 00:00:00"
    gtBegin = gTC.strTime2Int(strBeginMonthDay)
    curMonthDay = datetime.date.today().replace(day=1)
    gtEnd = gTC.strTime2Int(str(curMonthDay)+" 00:00:00")
    
    doDataClean(cnf,strBeginMonthDay,gtBegin)
    
    totalPointsNum = 0
    dResult = {}
    # data analysis
    tCurMonthDay = gtBegin        
    
    while True :
        try :
            if tCurMonthDay >= gtEnd : break
            nextMonthDay = beginMonthDay + relativedelta.relativedelta(months=1)
            strNextMonthDay = str(nextMonthDay)+" 00:00:00"
            tNextMonthDay = gTC.strTime2Int(strNextMonthDay)
            
            consumeDataStatis(gTC,tCurMonthDay,tNextMonthDay)
            installDataStatis(gTC,tCurMonthDay,tNextMonthDay)

            gLastInfoMap[COMPONETTIME] = strNextMonthDay
            dumpLastStatus(cnf.lastStatusFile,gLastInfoMap)
            tCurMonthDay = tNextMonthDay
            beginMonthDay = nextMonthDay            
            
        except :
            print "error occur :"
            print traceback.format_exc()
            logger.error(traceback.format_exc())
            time.sleep(60)
    logger.info("componetDataAnalysis end")        
    return None

if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')      

    if len(sys.argv) < 2 :
        print "usage : %s componet.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    print "config file :",fname
    
    cnf = ConfigData(fname)
    cnf.show()
    if cnf.logEnable :
        initLogger(cnf.logFilePath,cnf.logFileMaxSize,cnf.logMaxFileNum,cnf.logLevel)
    createDaemon(dataAnalysisMain,cnf.daemon)  
    
else :
    print "load %s module " % os.path.basename(__file__).split(".")[0]
    
    
    
    
    
    
