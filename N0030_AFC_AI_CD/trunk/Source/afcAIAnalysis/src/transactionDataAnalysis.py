#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,platform,time,datetime,traceback
import json
import logging,logging.handlers
from DACommon import ConfigData,createDaemon
from DACommon import initLogger,TimeConvert,getPgsqlConn
from DACommon import loadLastStatus,dumpLastStatus
from DACommon import ObjTransCount
import TSDBCMN 
from TSDBCMN import getTsdbConn,ObjTsdbItemBase
from TSDBCMN import fmtStrTimeToRfc3339

logger = logging.getLogger()

TRANSTIMEBEGINTAG = "timeBegin"
gLastInfoMap = {
    TRANSTIMEBEGINTAG : "",
}

def genTsdbQuery_trans(tbName,lineid,beginTime,endTime,maxPerFetch,offset):
    qPattern = '''select {conditions} from {table} ''' 
    qPattern += ''' where LINEID='{lineid}' and time >= {beginTime} and time < {endTime} ''' 
    qPattern += ''' limit {maxPerFetch} offset {offsetNum};'''
    #tbName = "ObjTransaction"
    arrLables = ['TRANSACTIONTYPE','TRANSACTIONSUBTYPE',
                 'TRANSACTIONDATETIME','STATIONID','LINEID','TICKETTYPE','DEVICEID']
    
    qstr = qPattern.format(
        conditions = ",".join(arrLables),
        table = tbName,
        lineid = lineid, # '8'
        beginTime = beginTime,
        endTime = endTime,
        maxPerFetch = maxPerFetch,
        offsetNum = offset
    )
    return qstr    
 
def fillFlowDataBase(qPattern,arrQuery,num_in,num_out,dTmp,refArrResult): 
    if num_in > 0 :
        qstrTmp = qPattern.format(**dTmp)
        arrQuery.append(qstrTmp)
        refArrResult.append(dTmp)
    
    if num_out > 0 : 
        dTmp2 = dict(dTmp)
        dTmp2["direction"] = 1
        dTmp2["passenger_flow"] = num_out            
        qstrTmp = qPattern.format(**dTmp2)
        arrQuery.append(qstrTmp)
        refArrResult.append(dTmp2)

def genPgdbQuery_lineTS(tc,lineid,dRLine,sectionType,refArrResult):
    tbName = "line_time_sharing_passenger_flow"
    #qPattern = '''insert into line_time_sharing_passenger_flow (line_id,direction,passenger_flow,flow_timestamp,section) values (8,0,100,'2017-08-16 14:23:29',10);'''
    arrFields = ["line_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into line_time_sharing_passenger_flow (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []
    
    dRTmp = {}
    if sectionType in dRLine : dRTmp = dRLine[sectionType]
    
    arrTmp = sorted(dRTmp.items(),key=lambda x : x[0])
    #print arrTmp
    for t,dtmp in arrTmp:
        #print t,dtmp
        num_in = dtmp["in"]
        num_out = dtmp["out"]
        dTmp = {
            "line_id" : lineid,
            "direction" : 0,
            "passenger_flow" : num_in,
            "flow_timestamp" : tc.intTime2Str(t),
            "section" : sectionType,
        }
        fillFlowDataBase(qPattern,arrQuery,num_in,num_out,dTmp,refArrResult)
        
    return arrQuery
    
def genPgdbQuery_stationTS(tc,lineid,dRStation,sectionType,refArrResult):
    tbName = "station_time_sharing_passenger_flow"
    #qPattern = '''insert into station_time_sharing_passenger_flow (line_id,station_id,direction,passenger_flow,flow_timestamp,section) values (8,1,0,100,'2017-08-16 14:23:29',10);'''
    arrFields = ["line_id","station_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into station_time_sharing_passenger_flow (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []  

    dRTmp = {}
    if sectionType in dRStation : dRTmp = dRStation[sectionType]
       
    for stationId,stationInfoMap in dRTmp.iteritems() :
        arrStation = sorted(stationInfoMap.items(),key=lambda x : x[0])        
        #print arrStation
        for t,dtmp in arrStation:
            #print t,dtmp
            num_in = dtmp["in"]
            num_out = dtmp["out"]
            dTmp = {
                "line_id" : lineid,
                "station_id" : stationId,
                "direction" : 0,
                "passenger_flow" : num_in,
                "flow_timestamp" : tc.intTime2Str(t),
                "section" : sectionType,
            }
            fillFlowDataBase(qPattern,arrQuery,num_in,num_out,dTmp,refArrResult)
            
    return arrQuery


def genPgdbQuery_ticketTS(tc,dRTicket,sectionType):
    tbName = "ticket_share_passenger_flow"
    #qPattern = '''insert into ticket_share_passenger_flow (ticket_id,direction,passenger_flow,flow_timestamp,section) values (8,0,100,'2017-08-16 14:23:29',10);'''
    arrFields = ["ticket_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into ticket_share_passenger_flow (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []  
    dRTmp = {}
    if sectionType in dRTicket : dRTmp = dRTicket[sectionType]
    for ticketId,dInfo in dRTmp.iteritems() :
        arrTmp = sorted(dInfo.items(),key=lambda x : x[0])
        #print arrTmp
        for t,dtmp in arrTmp:
            #print t,dtmp
            num_in = dtmp["in"]
            num_out = dtmp["out"]
            dTmp = {
                "ticket_id" : ticketId,
                "direction" : 0,
                "passenger_flow" : num_in,
                "flow_timestamp" : tc.intTime2Str(t),
                "section" : sectionType,
            }
            if num_in > 0 :
                qstrTmp = qPattern.format(**dTmp)
                arrQuery.append(qstrTmp)

            dTmp["direction"] = 1
            dTmp["passenger_flow"] = num_out
            if num_out > 0 : 
                qstrTmp = qPattern.format(**dTmp)
                arrQuery.append(qstrTmp)

    return arrQuery
     
    
def genPgdbQuery_lineCumulative(tc,lineid,dRLine,sectionType,refArrResult):
    tbName = "line_cumulative_passenger_flow"
    #qPattern = '''insert into line_cumulative_passenger_flow (line_id,direction,passenger_flow,flow_timestamp,section) values (8,0,100,'2017-08-16 14:23:29',10);'''
    arrFields = ["line_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into line_cumulative_passenger_flow (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []
    
    dRTmp = {}
    if sectionType in dRLine : dRTmp = dRLine[sectionType]
    
    arrTmp = sorted(dRTmp.items(),key=lambda x : x[0])
    #print arrTmp
    num_in,num_out = 0,0
    for t,dtmp in arrTmp:
        #print t,dtmp
        num_in += dtmp["in"]
        num_out += dtmp["out"]
        dTmp = {
            "line_id" : lineid,
            "direction" : 0,
            "passenger_flow" : num_in,
            "flow_timestamp" : tc.intTime2Str(t),
            "section" : sectionType,
        }
        fillFlowDataBase(qPattern,arrQuery,num_in,num_out,dTmp,refArrResult)
        
    return arrQuery

def genPgdbQuery_stationCumulative(tc,lineid,dRStation,sectionType,refArrResult):
    tbName = "station_cumulative_passenger_flow"
    #qPattern = '''insert into station_cumulative_passenger_flow (line_id,station_id,direction,passenger_flow,flow_timestamp,section) values (8,1,0,100,'2017-08-16 14:23:29',10);'''
    arrFields = ["line_id","station_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into station_cumulative_passenger_flow (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern   
   
    arrQuery = []
    
    dRTmp = {}
    if sectionType in dRStation : dRTmp = dRStation[sectionType]    
    
    for stationId,stationInfoMap in dRTmp.iteritems() :
        arrStation = sorted(stationInfoMap.items(),key=lambda x : x[0])
        #print arrStation
        num_in,num_out = 0,0
        for t,dtmp in arrStation:
            #print t,dtmp
            num_in += dtmp["in"]
            num_out += dtmp["out"]
            dTmp = {
                "line_id" : lineid,
                "station_id" : stationId,
                "direction" : 0,
                "passenger_flow" : num_in,
                "flow_timestamp" : tc.intTime2Str(t),
                "section" : sectionType,
            }
            
            fillFlowDataBase(qPattern,arrQuery,num_in,num_out,dTmp,refArrResult)
                     
    return arrQuery 

def genPgdbQuery_ticketCumulative(tc,dRTicket,sectionType):
    tbName = "ticket_cumulative_passenger_flow"
    #qPattern = '''insert into ticket_cumulative_passenger_flow (ticket_id,direction,passenger_flow,flow_timestamp,section) values (8,0,100,'2017-08-16 14:23:29',10);'''
    arrFields = ["ticket_id","direction","passenger_flow","flow_timestamp","section"]
    qPattern = '''insert into ticket_cumulative_passenger_flow (''' + ",".join(arrFields) 
    qPattern += ") values ('{" + "}','{".join(arrFields) + "}');"  
    #print qPattern  
   
    arrQuery = []
    dRTmp = {}
    if sectionType in dRTicket : dRTmp = dRTicket[sectionType]
    for ticket_id,dInfo in dRTmp.iteritems() :
        arrTmp = sorted(dInfo.items(),key=lambda x : x[0])
        #print arrTmp
        num_in,num_out = 0,0
        for t,dtmp in arrTmp:
            #print t,dtmp
            num_in += dtmp["in"]
            num_out += dtmp["out"]
            dTmp = {
                "ticket_id" : ticket_id,
                "direction" : 0,
                "passenger_flow" : num_in,
                "flow_timestamp" : tc.intTime2Str(t),
                "section" : sectionType,
            }
            if num_in > 0 :
                qstrTmp = qPattern.format(**dTmp)
                arrQuery.append(qstrTmp)

            dTmp["direction"] = 1
            dTmp["passenger_flow"] = num_out
            if num_out > 0 : 
                qstrTmp = qPattern.format(**dTmp)
                arrQuery.append(qstrTmp)
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

g_fillTypeMap = {"11":None,"50":None}    
def fillItemData(dResult,tCurrent,item):
    if not tCurrent in dResult : 
        dResult[tCurrent] = {"in" : 0,"out" : 0}  
    #print type(item["time"])
    transType = item['TRANSACTIONTYPE']
    subType = item['TRANSACTIONSUBTYPE']
    #if transType in ["11","50"] :
    if transType in g_fillTypeMap :
        if subType == "1" : # in
            dResult[tCurrent]["in"] += 1
        else : # out
            dResult[tCurrent]["out"] += 1    
    
def fillStatisMap(dataPoints,dRTicket,dRLine,dRStation,tBegin,sectionType):
    tCurrent = tBegin 
    #if not sectionType in dRTicket : dRTicket[sectionType] = {}
    refDRTicket = dRTicket[sectionType]
    #if not sectionType in dRLine : dRLine[sectionType] = {}
    refDRLine = dRLine[sectionType]
    #if not sectionType in dRStation : dRStation[sectionType] = {}
    refDRStation = dRStation[sectionType]    
  
    tStep = 60 * sectionType   
    for item in dataPoints:
        #print item
        curLineTime = item['time']
        tNext = tCurrent + tStep
        if curLineTime >= tNext :
            tCurrent = tNext
            
        # fill line statis data   
        fillItemData(refDRLine,tCurrent,item) 
        
        # fill station statis data
        stationId = item['STATIONID']        
        if not stationId in refDRStation :
            refDRStation[stationId] = {}
        refStationMap = refDRStation[stationId]
        fillItemData(refStationMap,tCurrent,item) 
        
        # fill ticket statis data
        ticketType = item["TICKETTYPE"]
        if not ticketType in refDRTicket:
            refDRTicket[ticketType] = {}
        refDType = refDRTicket[ticketType]
        fillItemData(refDType,tCurrent,item)

    return None 
    
def flusData2pgdb(pgConn,arrQuery):
    for query in arrQuery :
        #print query
        pgConn.execute(query)
    pgConn.commit()
    
    
class ObjTsdbStationTSPItem(ObjTsdbItemBase):
    def init(self):
        self.table = "station_time_sharing_passenger_flow"

class ObjTsdbStationCPItem(ObjTsdbItemBase):
    def init(self):
        self.table = "station_cumulative_passenger_flow"   
        
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
        self.timeFields = ["flow_timestamp"]
        self.timetag = "flow_timestamp"
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

class ObjStationTS(ObjDABase):
    def init(self):   
        self.proName = "ObjStationTS"
        self.json_body = []
        self.tagFields = ["line_id","station_id","section"]
        self.curObjItem = ObjTsdbStationTSPItem    
    
class ObjStationCP(ObjDABase):
    def init(self):   
        self.proName = "ObjStationCP"
        self.json_body = []
        self.tagFields = ["line_id","station_id","section"]
        self.curObjItem = ObjTsdbStationCPItem
        
class ObjLineTS(ObjDABase):
    def init(self):   
        self.proName = "ObjLineTS"
        self.json_body = []
        self.tagFields = ["line_id","section"]
        self.curObjItem = ObjTsdbLineTSPItem

class ObjLineCP(ObjDABase):
    def init(self):
        self.proName = "ObjLineCP"
        self.json_body = []
        self.tagFields = ["line_id","section"]
        self.curObjItem = ObjTsdbLineCPItem
        
  
def transDataStatis(gTC,lineid,tBegin,tEnd,dRTicket):
    cnf = ConfigData()
    tsdbConn = getTsdbConn(cnf)
    t_maxPerFetch = cnf.tsdb_max_per_fetch
 
    dRLine,dRStation = {},{}
    arrSection = [10,30,60]
    for sectionType in arrSection:
        if not sectionType in dRTicket : dRTicket[sectionType] = {}
        if not sectionType in dRLine : dRLine[sectionType] = {}
        if not sectionType in dRStation : dRStation[sectionType] = {}

    beginTime = tBegin * (10 ** 9)
    endTime = tEnd * (10 ** 9)

    dbFile = cnf.cacheFile
    transCountMap={}
    osrTrans = ObjTransCount()
    osrTrans.init(dbFile)
    osrTrans.createTable()
    osrTrans.loadData(transCountMap)
    
    for tbName in ["ObjTransaction","ObjCityCardTransaction"] :
        t_offset = 0
        while True :
            t1 = time.time()
            qstr = genTsdbQuery_trans(tbName,lineid,beginTime,endTime,t_maxPerFetch,t_offset)
            print qstr
            data = tsdbConn.query(qstr,epoch="s")
            #data = tsdbConn.query(qstr)        
            dataPoints = list(data.get_points())
            dataLen= len(dataPoints)   
            
            print "dataPoints len : ",dataLen
            tdiff = time.time() - t1
            print "time diff : ",tdiff,"speed : ",dataLen /(tdiff * 1.0)
            
            for item in dataPoints:
                # caculate trans count
                deviceId = int(item['DEVICEID'])
                if not deviceId in transCountMap :
                    transCountMap[deviceId] = {}
                refMapDevice = transCountMap[deviceId]
                if not tBegin in refMapDevice : 
                    refMapDevice[tBegin] = 0
                refMapDevice[tBegin] += 1

            for sectionType in arrSection:
                fillStatisMap(dataPoints,dRTicket,dRLine,dRStation,tBegin,sectionType)
            
            if dataLen == 0 : break
            t_offset += dataLen

    arrQuery = []    
    arrStationTS,arrSectionCP = [],[]
    arrLineTS,arrLineCP = [],[]
    
    # gen new insert sql    
    for sectionType in arrSection:
        # gen line_time_sharing_passenger_flow 
        arrQuery += genPgdbQuery_lineTS(gTC,lineid,dRLine,sectionType,arrLineTS)
        # gen station_time_sharing_passenger_flow
        arrQuery += genPgdbQuery_stationTS(gTC,lineid,dRStation,sectionType,arrStationTS)
        # gen ticket_share_passenger_flow    
        arrQuery += genPgdbQuery_ticketTS(gTC,dRTicket,sectionType)
        
        # gen line_cumulative_passenger_flow    
        arrQuery += genPgdbQuery_lineCumulative(gTC,lineid,dRLine,sectionType,arrLineCP)
        # gen station_cumulative_passenger_flow
        arrQuery += genPgdbQuery_stationCumulative(gTC,lineid,dRStation,sectionType,arrSectionCP)
        # gen ticket_cumulative_passenger_flow
        arrQuery += genPgdbQuery_ticketCumulative(gTC,dRTicket,sectionType)
    
    #print arrQuery
    
    pgConn = getPgsqlConn(cnf)
    flusData2pgdb(pgConn,arrQuery)
    pgConn.close()
    
    tsdbConn = getTsdbConn(cnf)
    ObjStationTS(gTC,tsdbConn,cnf,arrStationTS).run()
    ObjStationCP(gTC,tsdbConn,cnf,arrSectionCP).run()
    ObjLineTS(gTC,tsdbConn,cnf,arrLineTS).run()
    ObjLineCP(gTC,tsdbConn,cnf,arrLineCP).run()

    #tsdbConn.close()
    osrTrans.dumpData(transCountMap)  
    print "transDataStatis done"
    
def doDataClean(cnf,tBeginStr,tBegin):
    msg = "do doDataClean begin"
    logger.info(msg)
    # gen clear data sql
    arrPgdbQuery = []
    arrTsdbQuery = []
    
    dTableInfo = {
        "station_time_sharing_passenger_flow" : "flow_timestamp",
        "station_cumulative_passenger_flow" : "flow_timestamp",
        "line_time_sharing_passenger_flow" : "flow_timestamp",
        "line_cumulative_passenger_flow" : "flow_timestamp",
        "ticket_share_passenger_flow" : "flow_timestamp",
        "ticket_cumulative_passenger_flow" : "flow_timestamp",
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

    osrTrans = ObjTransCount()
    osrTrans.init(cnf.cacheFile)
    osrTrans.createTable()  
    osrTrans.cleanData(tBegin)
    
    msg = "do doDataClean done"
    logger.info(msg)
    return None
    
def dataAnalysisMain():
    cnf = ConfigData()
    logger.info("transactionDataAnalysis start")
    
    # init 
    gTC = TimeConvert()
    tsdbConn = getTsdbConn(cnf)
    # get line info
        
    arrLineInfo = TSDBCMN.getLineInfo(tsdbConn)
    
    tBeginStr = cnf.strSoureDataTimeBegin
    # load last status data      
    loadLastStatus(cnf.lastStatusFile,gLastInfoMap)
    lastBeginTime = gLastInfoMap[TRANSTIMEBEGINTAG]
    if lastBeginTime :
        tBeginStr = lastBeginTime    
    gtBegin = gTC.strTime2Int(tBeginStr)
    gtEnd = gTC.strTime2Int(str(datetime.date.today())+" 00:00:00")
    
    doDataClean(cnf,tBeginStr,gtBegin)
    
    # data analysis
    daySeconds = 3600 * 24
    dayBegin = gtBegin 
    while True :
        try :
            if dayBegin >= gtEnd : break
            nextDay = dayBegin + daySeconds
            msg = "[%s,%s)" % (gTC.intTime2Str(dayBegin),gTC.intTime2Str(nextDay))
            logger.info(msg)
            dRTicket = {}
            for lineId in arrLineInfo :
                transDataStatis(gTC,lineId,dayBegin,nextDay,dRTicket)
            
            gLastInfoMap[TRANSTIMEBEGINTAG] = gTC.intTime2Str(nextDay)
            dumpLastStatus(cnf.lastStatusFile,gLastInfoMap)
            dayBegin = nextDay
        except :
            print "error occur :"
            print traceback.format_exc()
            logger.error(traceback.format_exc())
            time.sleep(60)
    logger.info("transactionDataAnalysis end")        
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
    createDaemon(dataAnalysisMain,cnf.daemon)  
    
else :
    print "load %s module " % os.path.basename(__file__).split(".")[0]
    
    
    
    
    
    
