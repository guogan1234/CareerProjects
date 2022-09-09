#-*- coding:utf-8 -*-

import os,sys,platform
import logging,logging.handlers
import threading
import re,time,json
import ibm_db,tsdbpy,psycopg2
import traceback,string
import datetime,time,sched,re
import sqlite3

curProName = "afcAIAnalysis"
versionInf = "vesion 1.0.1"

_text_characters = "".join(map(chr,range(32,127))) + "\n\r\t\b"
_map_txt_chars = dict(zip(_text_characters,[True]*len(_text_characters)))

logger = logging.getLogger()


def istext(s):
    bret = True
    if not s.isalnum():
        bArr = []
        for chr in s :
            bArr.append(chr in _map_txt_chars)
        bret = all(bArr)   
    return bret

class Singleton(type):
    """
    A singleton metaclass. 
    
    usage :
        class A():
            __metaclass__ = Singleton 
            def __init__(self):
                pass
    """
    def __init__(cls, name, bases, dictionary):
        super(Singleton, cls).__init__(name, bases, dictionary)
        cls._instance = None
        cls._rlock = threading.RLock()
    def __call__(cls, *args, **kws):
        with cls._rlock:
            if cls._instance is None:
                cls._instance = super(Singleton, cls).__call__(*args, **kws)
        return cls._instance

def curProIsRun(pidFilePath,checkTimes=1):
    ret = False
    for _ in xrange(0,checkTimes):
        if os.path.exists(pidFilePath) :
            lastPid = ""
            with open(pidFilePath,"r") as fin:
                lastPid = fin.read().strip()
            if len(lastPid) > 0 :
                try: 
                    with open("/proc/%s/status" % lastPid,"r") as fin :
                        ret = True
                except :
                    pass
        if ret == True : break
        else: time.sleep(1)
    return ret

def createDaemon(mainFunc,bDaemon=True):
    logger = logging.getLogger()
    curSystem = platform.system()
    if (curSystem == "Linux") and bDaemon:
        try:
            if os.fork() > 0: os._exit(0)
        except OSError, error:
            logger.error('fork #1 failed: %d (%s)',error.errno, error.strerror)
            os._exit(1)
        #os.chdir('/')
        os.setsid()
        os.umask(0)
        try:
            pid = os.fork()
            if pid > 0:
                logger.info('Daemon PID %d',pid)
                os._exit(0)
        except OSError, error:
            logger.error('fork #2 failed: %d (%s)',error.errno, error.strerror)
            os._exit(1)
        sys.stdout.flush()
        sys.stderr.flush()
        si = file("/dev/null", 'r')
        so = file("/dev/null", 'a+')
        se = file("/dev/null", 'a+', 0)
        os.dup2(si.fileno(), sys.stdin.fileno())
        os.dup2(so.fileno(), sys.stdout.fileno())
        os.dup2(se.fileno(), sys.stderr.fileno())
    else :
        msgTmp = "platform : {0} ,bDaemon : {1}".format(curSystem,bDaemon)
        logger.info(msgTmp)
    mainFunc()
    return None
    
        
class ObjDataAnalysis(object):
    __metaclass__ = Singleton
    def __init__(self):
        self.sourceData = {}

class ObjSwitchInfo(object):
    def __init__(self,_ip,_name):
        self.ip = _ip 
        self.name = _name 

class ConfigData(object):
    __metaclass__ = Singleton
    def __init__(self,_fileName):
        self.fileName = _fileName
        self.__docTree = None
        self.dbKeys = ["host","port","user","password","dbName"]
        self.attrArr = []
        self.arrSwitchInfo = []
        self.initDoctree()
        self.init()
        self.getBaseConfig()        
    
    def initDoctree(self):
        try:
            import xml.etree.cElementTree as ET
        except ImportError:
            import xml.etree.ElementTree as ET
        if not os.path.exists(self.fileName) : 
            print "file ", self.fileName, " not exists"
            return None
        try:
            self.__docTree = ET.ElementTree(file=self.fileName)
        except Exception,e:
            print "%s is NOT well-formed : %s "%(self.fileName,e)
            return None
    
    def init(self):
        pass
        
    def show(self):    
        for k in self.attrArr :
            print k,":",self.__dict__[k]
            
    def get(self,attrName):
        ret = None
        if attrName in self.__dict__ :
            ret = self.__dict__[attrName]
        return ret
        
    def _getObjBase(self,path):
        retobj = None
        if self.__docTree :
            retobj = self.__docTree.find(path)
        return retobj   
        
    def getSectiontText(self,path):
        retText = ""
        objTmp = self._getObjBase(path)
        if objTmp != None :
            retText = objTmp.text or ""
        return retText.strip()
      
    def getTextAttribute(self,path,attrName):
        retText = ""        
        objTmp = self._getObjBase(path)
        if objTmp != None : 
            retText = objTmp.get(attrName,"")
        return retText.strip()  
       
    def getSectiontInt(self,path):    
        strTmp = self.getSectiontText(path).strip()
        return (int(strTmp) if strTmp.isdigit() else 0)
    
    def _genDbKeyName(self,dbTag,keyTag):
        keyName = "%s_%s" % (dbTag,keyTag) 
        return keyName
    
    def _fillDbTagData(self,dbTag,keyTag):
        pathBase = "databases/%s/" % dbTag
        keyName = self._genDbKeyName(dbTag,keyTag)        
        self.__dict__[keyName] = self.getSectiontText(pathBase + keyTag)
        self.attrArr.append(keyName)
    
    def getDatabaseConfig(self,dbTag="db2"):
        # get db configure    
        for keyTag in self.dbKeys :
            self._fillDbTagData(dbTag,keyTag) 

    def getDbParams(self,dbTag="db2"):
        retArr = []
        for keyTag in self.dbKeys : 
            keyName = self._genDbKeyName(dbTag,keyTag)  
            retArr.append(self.__dict__[keyName])
        return retArr
        
    def getSwitchHostInfo(self,path):        
        if not self.__docTree : 
            return None            
        objTmp = self.__docTree.findall(path)
        if objTmp :
            for objItem in objTmp :
                t_ip = objItem.get("ip","")
                t_name = objItem.get("name","")
                self.arrSwitchInfo.append(ObjSwitchInfo(t_ip,t_name))
        return None    
    
    def getBaseConfig(self):
        self.daemon = self.getSectiontText("deamon") == "yes"
        self.pidFilePath = self.getSectiontText("pidFilePath")
        if len(self.pidFilePath) == 0 : self.pidFilePath = "/tmp/.afcAiPid"
        self.logEnable = self.getTextAttribute("logFile","enable") == "true"
        self.logLevel = self.getSectiontText("logFile/logLevel")
        self.logFilePath = self.getSectiontText("logFile/logFilePath")
        self.logFileMaxSize = self.getSectiontInt("logFile/fileMaxSize")
        self.logMaxFileNum = self.getSectiontInt("logFile/maxFileNum")
        self.attrArr += ["daemon","logEnable","logLevel","logFilePath","logFileMaxSize","logMaxFileNum"]             
        
        self.strCheckTime = self.getSectiontText("checkTime")
        self.checkInterval = self.getSectiontInt("checkInterval")
        self.strSoureDataTimeBegin = self.getSectiontText("sourDataTimeBegin")
        self.lastStatusFile = self.getSectiontText("lastStatusFile")
        self.cacheFile = self.getSectiontText("cacheFile")
        if self.cacheFile == "" : self.cacheFile = "/tmp/afcDACacheFile.db"
        
        self.attrArr += ["strCheckTime","checkInterval"]
        self.attrArr += ["strSoureDataTimeBegin","lastStatusFile","cacheFile"]

        # get db configure
        self.getDatabaseConfig("db2")
        self.getDatabaseConfig("tsdb")
        self.tsdb_max_per_fetch = self.getTextAttribute("databases/tsdb","max_per_fetch")
        self.tsdb_max_per_fetch = int(self.tsdb_max_per_fetch)
        self.tsdb_max_per_commit = self.getTextAttribute("databases/tsdb","max_per_commit")
        self.tsdb_max_per_commit = int(self.tsdb_max_per_commit)  
        self.attrArr += ["tsdb_max_per_fetch","tsdb_max_per_commit"]
        
        self.getDatabaseConfig("pgsql")
        self.getDatabaseConfig("pgsql_stock")
        
        # dataPredict
        self.predictWeeks = self.getSectiontInt("dataPredict/predictWeeks")
        self.attrArr += ["predictWeeks"]

        # device monitor
        attrTmp = self.getTextAttribute("deviceMonitor","enable")
        self.deviceMonitorEnabled = (attrTmp.lower() == "true")
        attrTmp = self.getTextAttribute("deviceMonitor","interval")
        if attrTmp == "" : attrTmp = "0"
        self.deviceMonitorInterval = int(attrTmp)
        self.getSwitchHostInfo("deviceMonitor/coreSwitch/host")
        self.attrArr += ["deviceMonitorEnabled","deviceMonitorInterval"]
        
        return None        
       
def initLogger(logfilePath,maxSize,backupCount,logLevel="info"):
    import logging
    import logging.handlers    
    logger = logging.getLogger()
    dictLoglvel = {        
        "debug":logging.DEBUG,
        "info":logging.INFO,
        "warn":logging.WARNING,
        "error":logging.ERROR,
        "critical":logging.CRITICAL,    
    } 
    try :
        import os
        dirName = os.path.dirname(logfilePath)
        if not os.path.exists(dirName) : os.makedirs(dirName)
        #hdlr = logging.FileHandler(logfilePath)
        hdlr = logging.handlers.RotatingFileHandler(logfilePath,maxBytes=maxSize*1024*1024,backupCount=backupCount)        
        formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(filename)s:%(lineno)d [%(funcName)s] %(message)s')
        #formatter = logging.Formatter('%(asctime)s [%(levelname)s] : %(message)s')       
        hdlr.setFormatter(formatter)
        logger.addHandler(hdlr)
        logger.setLevel(dictLoglvel[logLevel if logLevel in dictLoglvel else "info"]) # default : logging.INFO
    except Exception,e:
        print "initLogger error : %s"%e
        logger = None
    return logger
    
def initConsolLogger():
    logging.basicConfig(
        level=logging.DEBUG, # DEBUG,INFO,WARNING,ERROR,CRITICAL
        format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
        datefmt='%a, %d %b %Y %H:%M:%S'
    )
    return None
    
class TimeConvert(object):
    def __init__(self):
        self.dCache = {}
        self.patternStrTime = re.compile('(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})')
        self.dTimeTable = {}
        self.initTimeTable()
        
    def intTime2Str(self,unixTimeInt):
        if unixTimeInt < 0 : unixTimeInt = 0
        return time.strftime("%Y-%m-%d %H:%M:%S",time.localtime(unixTimeInt))
    
    def initTimeTable(self):
        for i in xrange(3600):
            seconds = i % 60
            minutes = (i / 60) % 60
            strTmp = "%02d:%02d" % (minutes,seconds)
            #print strTmp
            self.dTimeTable[i] = strTmp
        return None
    
    def strTime2Int(self,strTime):
        ret = self.dCache.get(strTime, 0)       
        if ret == 0:
            if not self.patternStrTime.match(strTime) :
                msg = "\"%s\" is not time string format" % strTime
                logger.error(msg)
                ret = int(time.time())
            else :
                #self.dCache.clear()
                if len(self.dCache) > 3600 * 24 : self.dCache.clear()                
                tmpList = self.patternStrTime.search(strTime).groups()
                ret = int(time.mktime(datetime.datetime(* map(int,tmpList)).timetuple()))
                self.dCache[strTime] = ret                           
                for i in xrange(1, 3600 - ret % 3600):
                    time_int = ret + i                
                    j = time_int % 3600
                    #print dTimeTable[j]
                    time_str = strTime[:-5] + self.dTimeTable[j]
                    self.dCache[time_str] = time_int
        else :
            #print "data in cache"
            pass
        return ret       

class ObjDB2Connection():
    def __init__(self,host,port,user,passwd,dbName):
        self.dbCnnInfo = {
            'HOSTNAME':host,
            'DATABASE':dbName,
            'PORT':str(port),
            'UID':user,
            'PWD':passwd,
            'PROTOCOL':'TCPIP',
            'AUTHENTICATION':'SERVER',
        }
        self.cnnStr = ";".join(["%s=%s"%(k,v) for k,v in self.dbCnnInfo.items()])
        self.handle = ibm_db.connect(self.cnnStr,"","")

    def query(self,qstr):
        qResult = []
        try :
            result = ibm_db.exec_immediate(self.handle,qstr)
            while True :
                row = ibm_db.fetch_assoc(result)
                if not row : break
                qResult.append(row)            
        except :
            print qstr
            print  traceback.format_exc()
            self.reInitConn()
        return qResult 

    def query_iter(self,qstr):
        try :
            result = ibm_db.exec_immediate(self.handle,qstr)
            while True :
                row = ibm_db.fetch_assoc(result)
                if not row : break
                yield row            
        except :
            print traceback.format_exc()
            self.reInitConn()
          
        
    def reInitConn(self):
        bret,errMsg = False,""
        try :
            self.close()
            self.handle = ibm_db.connect(self.cnnStr,"","")
            bret = True
            errMsg = "reInitConn ok!"
        except :
            errMsg = "reInitConn fail!"
        print errMsg
        return bret
        
    def close(self):
        bret = True
        try :
            ibm_db.close(self.handle)
        except :
            bret = False
        return bret  
        
def getDb2Conn(cnf):
    host,port,user,passwd,dbName = cnf.getDbParams("db2")    
    db2Conn = ObjDB2Connection(host,port,user,passwd,dbName)   
    return db2Conn


class ObjPGSQLConnection():
    def __init__(self,host,port,user,passwd,dbName):
        self.dbCnnInfo = {
            'host':host,
            'dbname':dbName,
            'port':str(port),
            'user':user,
            'password':passwd,
        }  
        self.handle = None
        self.cursor = None
        
    def conn(self):
        bret = True
        try:
            self.handle = psycopg2.connect(**self.dbCnnInfo)
            self.handle.set_client_encoding("UTF-8")
            #self.handle.set_client_encoding("UTF8")
            self.handle.autocommit = False
            self.cursor = self.handle.cursor()
        except:
            print "I am unable to connect to the database"
            bret = False
        return bret
        
    def execute(self,strQuery):
        bret = True
        try :
            self.cursor.execute(strQuery)
        except :
            print "execute fail!"
            print strQuery
        
        return bret
    
    def query(self,strQuery):
        rows = None
        try :
            self.cursor.execute(strQuery)
            rows = self.cursor.fetchall()
        except :
            print "query fail "
            print strQuery
        return rows
                   
    def commit(self):
        bret = True
        try :
            self.handle.commit()
        except :
            #self.handle.rollback()
            bret = False
        return bret
        
    def close(self):
        bret = True
        try :
            self.handle.close()
        except :
            bret = False
        return bret      
        

def getPgsqlConn(cnf,prefix="pgsql"):
    host,port,user,passwd,dbName = cnf.getDbParams(prefix)
    pgConn = ObjPGSQLConnection(host,port,user,passwd,dbName)
    pgConn.conn()
    return pgConn    


patternStrTime = re.compile('(\d{4})-(\d{2})-(\d{2}) (\d{2}):(\d{2}):(\d{2})')
def strTime2Int(strTime):
    strTime = str(strTime)
    if not patternStrTime.match(strTime) :
        #print "\"%s\" is not time string format" % strTime
        return 0        
    return int(time.mktime(datetime.datetime(* map(int,patternStrTime.search(strTime).groups()) ).timetuple()))    

class ScheduleBase():    
    schedule = sched.scheduler(time.time, time.sleep)
    daySeconds = 3600 * 24
    startTime = "00:00:00"
    name = "ScheduleBase"
        
    def doTaskMain(self):
        pass
        
    def resetStartTime(self,seconds=10):
        d1 = datetime.datetime.now()        
        d1 += datetime.timedelta(seconds=seconds)
        self.startTime = d1.strftime("%H:%M:%S")
        print self.startTime    
        
    def performTask(self,secondsDelay):
        self.schedule.enter(secondsDelay,0,self.performTask,(secondsDelay,)) # re-scheduler
        self.doTaskMain()
        logger.info("%s re-scheduler step : %d",self.name,secondsDelay)
        #print "%s re-scheduler step : %d" % (self.name,secondsDelay)

    def run(self):
        secondsDelay = self.daySeconds
        startTime = self.startTime
        dTmp = datetime.date.today() + datetime.timedelta(days=1)
        startTime = "%s %s" % (dTmp.strftime("%Y-%m-%d"),startTime)
        inc = (strTime2Int(startTime) - int(time.time()))%(secondsDelay)
        logger.info("%s will start in %d seconds",self.name,inc)
        #print "%s will start in %d seconds" % (self.name,inc)
        self.schedule.enter(inc,0,self.performTask,(secondsDelay,)) # inc = 0 : right now
        self.schedule.run()        

def loadLastStatus(lastStatusFile,dLastInfo):
    if os.path.exists(lastStatusFile) :
        with open(lastStatusFile,"r") as fin :
            #self.db2LastStatus = json.load(fin)
            sTmp = fin.read().strip()
            try :
                dLastInfo.update(json.loads(sTmp))
            except :
                pass
    return None
    
def dumpLastStatus(lastStatusFile,dLastInfo):
    print "dLastInfo : ",dLastInfo
    with open(lastStatusFile,"w") as fout :
        fout.write(json.dumps(dLastInfo))
    return None    

class ObjParseDeviceInfo(): 
    def __init__(self,deviceId):
        self.deviceid = int(deviceId)
        
    def bcdDecode(self,a):
        try :
            a = int("%x" % a)
        except :
            logger.warn("bcdDecode error,deviceid :",str(self.deviceid),"a :",str(a))
        return a
        
    def getDeviceType(self):
        a = (self.deviceid & 0xff000000) >> 24
        deviceType = self.bcdDecode(a) % 17
        return deviceType
        
    def getLineId(self):
        lineId = (self.deviceid & 0x00ff0000) >> 16
        lineId = self.bcdDecode(lineId)
        return lineId
    
    def getStationId(self):
        a = (self.deviceid & 0xff000000) >> 24
        c = (self.deviceid & 0x0000ff00) >> 8
        #print a,c
        a = self.bcdDecode(a)
        c = self.bcdDecode(c)        
        staionId = (a / 17) * 100 + c 
        return staionId

class ObjStatusBase(object):
    def __init__(self,):
        self.dbFile = ""
        self.tbName = "" 
        
    def _createTable(self,query):        
        con = sqlite3.connect(self.dbFile) 
        con.execute(query)
        con.commit()
        con.close()
        return None
        
    def _dumpData(self,arrQuery):    
        con = sqlite3.connect(self.dbFile)
        con.execute("delete from %s;" % self.tbName)
        con.commit()         
        for query in arrQuery : 
            con.execute(query)
        con.commit()
        con.close()    
        return None 
        
class ObjTransCount(ObjStatusBase):
    def init(self,dbFile):
        self.dbFile = dbFile
        self.tbName = "deviceTransCount"
        
    def createTable(self):
        query = """
            create table if not exists %s(   
                deviceId integer,
                analysis_timestamp integer,
                count integer
            );
        """ % self.tbName
        self._createTable(query)
        return None
            
    def loadData(self,refMapOut):
        con = sqlite3.connect(self.dbFile) 
        cursor = con.execute("select deviceId,analysis_timestamp,count from %s;" % self.tbName)
        rows = cursor.fetchall()
        #print rows
        for deviceId,analysis_timestamp,count in rows : 
            if not deviceId in refMapOut : 
                refMapOut[deviceId] = {}
            refMapDevice = refMapOut[deviceId]
            if not analysis_timestamp in refMapDevice : 
                refMapDevice[analysis_timestamp] = 0
            refMapDevice[analysis_timestamp] = count
        con.close()
        return None
        
    def dumpData(self,refMapIn):    
        arrQuery = []
        for deviceId,dTmp in refMapIn.iteritems():
            for analysis_timestamp,count in dTmp.iteritems():
                query = "insert into %s values(%d,%d,%d)" 
                query = query % (self.tbName,deviceId,analysis_timestamp,count)
                arrQuery.append(query)
        self._dumpData(arrQuery)
        return None 
    
    def cleanData(self,tBegin):
        con = sqlite3.connect(self.dbFile) 
        cursor = con.execute("delete from %s where analysis_timestamp >= %d;" % (self.tbName,tBegin))
        con.commit()
        return None
        
class ObjDowntimeStatus(ObjStatusBase):
    def init(self,dbFile):
        self.dbFile = dbFile
        self.tbName = "deviceDowntime"        
        
    def createTable(self):
        query = """
            create table if not exists %s(   
                deviceId integer,
                lastTime integer,
                lastStatus integer
            );
        """ % self.tbName
        self._createTable(query)
        return None
                
    def loadData(self,refMapOut):
        con = sqlite3.connect(self.dbFile) 
        cursor = con.execute("select deviceId,lastTime,lastStatus from %s;" % self.tbName)
        rows = cursor.fetchall()
        #print rows
        for deviceId,lastTime,lastStatus in rows : 
            if not deviceId in refMapOut : 
                refMapOut[deviceId] = [0,0]
            refTmp = refMapOut[deviceId]
            refTmp[0] = lastTime
            refTmp[1] = lastStatus
        con.close()
        return None
        
    def dumpData(self,refMapIn):
        arrQuery = []
        for deviceId,(lastTime,lastStatus) in refMapIn.iteritems():
            query = "insert into %s values(%d,%d,%d)" 
            query = query % (self.tbName,deviceId,lastTime,lastStatus)
            arrQuery.append(query)
        self._dumpData(arrQuery)
        return None

class ObjFailureStatus(ObjStatusBase):
    def init(self,dbFile):
        self.dbFile = dbFile
        self.tbName = "deviceFailReason"        
        
    def createTable(self):
        query = """
            create table if not exists %s(   
                deviceId integer,
                tagName varchar(20),
                lastTime integer,
                lastStatus integer
            );
        """ % self.tbName
        self._createTable(query)
        return None
            
    def loadData(self,refMapOut):
        con = sqlite3.connect(self.dbFile) 
        cursor = con.execute("select deviceId,tagName,lastTime,lastStatus from %s;" % self.tbName)
        rows = cursor.fetchall()
        #print rows
        for deviceId,tagName,lastTime,lastStatus in rows : 
            if not deviceId in refMapOut : 
                refMapOut[deviceId] = {}
            refTmp = refMapOut[deviceId]
            if not tagName in refTmp : 
                refTmp[tagName] = [0,0]
            refTmp = refTmp[tagName]
            refTmp[0] = lastTime
            refTmp[1] = lastStatus
        con.close()         
        return None
        
    def dumpData(self,refMapIn):
        arrQuery = []
        for deviceId,dtmp in refMapIn.iteritems():
            for tagName,(lastTime,lastStatus) in dtmp.iteritems():
                query = "insert into %s values(%d,'%s',%d,%d)" 
                query = query % (self.tbName,deviceId,tagName,lastTime,lastStatus)
                arrQuery.append(query)
        self._dumpData(arrQuery)
        return None
       
     

        
