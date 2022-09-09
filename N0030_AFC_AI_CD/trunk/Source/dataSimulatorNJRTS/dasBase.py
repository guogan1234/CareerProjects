
import os,sys,threading,time,datetime
import platform,sched,re

class Singleton(type):
    def __init__(cls, name, bases, dictionary):
        super(Singleton, cls).__init__(name, bases, dictionary)
        cls._instance = None
        cls._rlock = threading.RLock()
    def __call__(cls, *args, **kws):
        with cls._rlock:
            if cls._instance is None:
                cls._instance = super(Singleton, cls).__call__(*args, **kws)
        return cls._instance

class ConfigData(object):
    __metaclass__ = Singleton
    def __init__(self,_fileName):
        self.fileName = _fileName
        self.__docTree = None
        self.dbKeys = ["host","port","user","password","dbName"]
        self.attrArr = []
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
       
    def getSectiontInt(self,path):    
        strTmp = self.getSectiontText(path).strip()
        return (int(strTmp) if strTmp.isdigit() else 0)
    
    def _genDbKeyName(self,dbTag,keyTag):
        keyName = "%s_%s" % (dbTag,keyTag) 
        return keyName
    
    def _fillDbTagData(self,dbTag,keyTag):
        pathBase = "%s/" % dbTag
        keyName = self._genDbKeyName(dbTag,keyTag)        
        self.__dict__[keyName] = self.getSectiontText(pathBase + keyTag)
        self.attrArr.append(keyName)
    
    def getDatabaseConfig(self,dbTag="db2"):
        # get db configure    
        for keyTag in self.dbKeys :
            self._fillDbTagData(dbTag,keyTag) 
    
    def getBaseConfig(self):
        self.daemon = self.getSectiontText("deamon") == "yes"        
        self.pidFilePath = self.getSectiontText("pidFilePath")
        if len(self.pidFilePath) == 0 : self.pidFilePath = "/tmp/.pidOfDataSimulator" 
        self.csvFilesDir = self.getSectiontText("csvFilesDir")        
        self.attrArr += ["daemon","pidFilePath","csvFilesDir"]           
        
        self.strCheckTime = self.getSectiontText("checkTime")
        self.checkInterval = self.getSectiontInt("checkInterval")      
        self.attrArr += ["strCheckTime","checkInterval"] 

        # get db configure 
        self.getDatabaseConfig("pgsql") 
        
        return None  
        
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
    curSystem = platform.system()
    if (curSystem == "Linux") and bDaemon:
        try:
            if os.fork() > 0: os._exit(0)
        except OSError, error:
            print('fork #1 failed: %d (%s)' % (error.errno, error.strerror))
            os._exit(1)
        #os.chdir('/')
        os.setsid()
        os.umask(0)
        try:
            pid = os.fork()
            if pid > 0:
                print('Daemon PID %d' % pid)
                os._exit(0)
        except OSError, error:
            print('fork #2 failed: %d (%s)' % (error.errno, error.strerror))
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
        print(msgTmp)
    mainFunc()
    return None
    

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
        print "%s re-scheduler step : %d" % (self.name,secondsDelay)

    def run(self):
        secondsDelay = self.daySeconds
        startTime = self.startTime
        dTmp = datetime.date.today() + datetime.timedelta(days=1)
        startTime = "%s %s" % (dTmp.strftime("%Y-%m-%d"),startTime)
        inc = (strTime2Int(startTime) - int(time.time()))%(secondsDelay)
        print "%s will start in %d seconds" % (self.name,inc)
        self.schedule.enter(inc,0,self.performTask,(secondsDelay,)) # inc = 0 : right now
        self.schedule.run()       
    
