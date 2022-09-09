#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,platform,time,binascii,json,logging,traceback
from DACommon import ConfigData,TimeConvert,ObjParseDeviceInfo
from DACommon import istext,getPgsqlConn,initConsolLogger
from TSDBCMN import getTsdbConn,ObjTsdbItemBase
from TSDBCMN import fmtStrTimeToRfc3339

logger = logging.getLogger()

'''
数据抽取程序

实现pgdb数据库到tsdb的抽取

'''
        
class ObjTsdbConsumeItem(ObjTsdbItemBase):
    def init(self):
        self.table = "component_consuming"      

class ObjTsdbInstallItem(ObjTsdbItemBase):
    def init(self):
        self.table = "component_install"      
        
class ObjEtlBase():
    def __init__(self,cnf):
        self.proName = "ObjEtlBase"
        self.curTableName = "EtlBase"
        self.tsdbConn = getTsdbConn(cnf)
        self.pgdbConn = getPgsqlConn(cnf,"pgsql_stock")
        self.db2LastStatusFile = cnf.lastStatusFile
        self.dLastStatus = {"etl":{}}
        self.db2LastStatus = {}
        self.dbName = self.tsdbConn.dbName
        self.maxNumPerCommit = cnf.tsdb_max_per_commit
        self.idCloumn = ""
        self.tConvert = TimeConvert()
        self.init()

    def init(self):
        pass
        
    def resetTableIndex(self):
        # self.db2LastStatus["ObjCityCardTransaction_id"] = 0
        # self.db2LastStatus["ObjTransaction_id"] = 0
        # self.db2LastStatus["ObjEventLog_id"] = 0  
        for k,_ in self.db2LastStatus.iteritems() :
            self.db2LastStatus[k] = 0            

    def loadLastStatus(self):
        logger.info("loadLastStatus begin")
        if os.path.exists(self.db2LastStatusFile) :
            with open(self.db2LastStatusFile,"r") as fin :
                #self.db2LastStatus = json.load(fin)
                sTmp = fin.read().strip()
                try :
                    self.dLastStatus.update(json.loads(sTmp))
                    self.db2LastStatus.update(self.dLastStatus["etl"])
                except :
                    self.resetTableIndex()
        else :
            self.resetTableIndex()
        logger.info("dLastStatus : %s" % str(self.dLastStatus))
        logger.info("loadLastStatus end")
        return None
        
    def dumpLastStatus(self):
        self.dLastStatus["etl"].update(self.db2LastStatus)
        print "self.db2LastStatusFile : ",self.db2LastStatusFile
        with open(self.db2LastStatusFile,"w") as fout :
            fout.write(json.dumps(self.dLastStatus))
        return None
    
    def genQueryBase(self):
        query = "select %s from %s " % (",".join(self.pgFields),self.curTableName)   
        logger.debug(str(self.db2LastStatus))
        query += "where %s > %s order by %s " % (self.idCloumn,self.db2LastStatus[self.curTableName],self.idCloumn)
        #query += " fetch first 10000 rows only"
        query += " limit 10000 "
        logger.debug(query)
        return query
        
    def updateTime_use_id(self,refObj,curLineTime):
        #print "curLineTime : ",curLineTime
        tDay,tTime = curLineTime.split(" ")
        v = 0
        if self.idCloumn : v = int(refObj.fields[self.idCloumn])
        v = long(v)%(10**9)
        
        curTimeInt = self.tConvert.strTime2Int(curLineTime)
        refObj.time = curTimeInt * (10**9) + v
        #print refObj.time
     
    def updateTime(self,refObj,curLineTime):
        #self.updateTime_use_time(refObj,curLineTime)
        self.updateTime_use_id(refObj,curLineTime)
    
    def fillItemData(self,refItem,objTmp):
        for kName,value in objTmp.iteritems():
            value = str(value)
            #if not istext(value) :
                #print "'%s' is not text" % value
            #    value = "0x" + binascii.b2a_hex(value)
            if kName in self.tagFields :
                refItem.tags[kName] = value
            else:
                refItem.fields[kName] = value
        curLineTime = refItem.fields[self.timetag]
        for kName in self.timeFields :
            vtmp = refItem.fields[kName]
            refItem.fields[kName] = fmtStrTimeToRfc3339(vtmp)
        self.updateTime(refItem,curLineTime)

    def run(self):
        msg = "%s run" % self.proName
        logger.info(msg)
        self.tsdbConn.createDb(self.dbName)
        self.loadLastStatus()
        tNum = 0
        while True :
            nCount = self.doDataWrite()
            logger.debug("nCount = %d" % nCount)
            if nCount > 0 : 
                self.dumpLastStatus()
                tNum += nCount
            if nCount == 0 : break
        print self.db2LastStatus
        logger.info("db2LastStatus : %s" % str(self.db2LastStatus))
        logger.info("tNum = %d" % tNum)
        msg = "%s task done" % self.proName
        logger.info(msg)

class ObjEtlConsume(ObjEtlBase):
    def init(self):   
        self.proName = "ObjEtlConsume"
        self.curTableName = "component_consuming"
        self.db2LastStatus[self.curTableName] = 0
        self.idCloumn = "id"
        self.json_body = []
        self.tagFields = ["component_serial_no",]
        self.timeFields = ["consuming_datetime",]       
        self.timetag = "consuming_datetime"        
        self.pgFields = [        
            "id" ,
            "component_serial_no",
            "component_type" ,
            "component_name" ,
            "manufacture" ,
            "product_type" ,
            "status" ,
            "stock_id" ,
            "stockout_id" ,
            "stockout_person",
            "consuming_person" ,
            "consuming_datetime" ,
            "remark" 
        ]
        
    def doDataWrite(self):
        nCount = 0
        try :
            query = self.genQueryBase()
            result = self.pgdbConn.query(query)            

            for i,objTmp in enumerate(result):
                otItem = ObjTsdbConsumeItem()
                print "objTmp :",objTmp
                objTmp = dict(zip(self.pgFields,objTmp))
                print "objTmp :",objTmp
                self.db2LastStatus[self.curTableName] = int(objTmp[self.idCloumn])
                self.fillItemData(otItem,objTmp)
                
                tMap = otItem.toDict()
                #print "toDict : ",tMap
                self.json_body.append(tMap)
                if (i+1) % self.maxNumPerCommit == 0 :
                    self.tsdbConn.write(self.json_body)
                    self.json_body = []
                    print "commit %d" % i 
                nCount += 1
                
            self.tsdbConn.write(self.json_body)
            self.json_body = [] 
        except :
            nCount = -1
            logger.error(traceback.format_exc())  
        
        return nCount
        
class ObjEtlInstall(ObjEtlBase):
    def init(self):   
        self.proName = "ObjEtlInstall"
        self.curTableName = "component_install"
        self.db2LastStatus[self.curTableName] = 0
        self.idCloumn = "id"
        self.json_body = []
        self.tagFields = ["component_serial_no",]
        self.timeFields = ["install_datetime",]       
        self.timetag = "install_datetime"
        self.pgFields = [        
            "id" ,
            "component_serial_no",
            "component_type" ,
            "component_name" ,
            "manufacture" ,
            "product_type" ,
            "status" ,            
            "equipment_id" ,
            "install_person" ,
            "install_datetime",
        ]
        
    def doDataWrite(self):
        nCount = 0
        try :
            query = self.genQueryBase()
            result = self.pgdbConn.query(query)            
            #t_curTime = None
            for i,objTmp in enumerate(result):
                otItem = ObjTsdbInstallItem()
                print "objTmp :",objTmp
                objTmp = dict(zip(self.pgFields,objTmp))
                print "objTmp :",objTmp
                self.db2LastStatus[self.curTableName] = int(objTmp[self.idCloumn])
                self.fillItemData(otItem,objTmp)
                
                tMap = otItem.toDict()
                #print "toDict : ",tMap
                self.json_body.append(tMap)
                if (i+1) % self.maxNumPerCommit == 0 :
                    self.tsdbConn.write(self.json_body)
                    self.json_body = []
                    print "commit %d" % i 
                nCount += 1
                           
            self.tsdbConn.write(self.json_body)
            self.json_body = [] 
        except :
            nCount = -1
            logger.error(traceback.format_exc())  
        
        return nCount

        
def etlUsePgdb(cnf):
    logger.info("task begin")

    arrEtl = []
    
    arrEtl.append(ObjEtlConsume(cnf))
    arrEtl.append(ObjEtlInstall(cnf))
    
    for objEtl in arrEtl :
        objEtl.run()
    logger.info("task end")
  
if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')
    if len(sys.argv) < 2 :
        print "usage : %s default.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    cnf = ConfigData(fname)
    cnf.show()

    initConsolLogger()
    etlUsePgdb(cnf)
else :
    print "load etlInstance_db2 module"
    
    
  
    
