#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,json,time,datetime,traceback
import logging,logging.handlers
from DACommon import ConfigData,createDaemon,ScheduleBase
from DACommon import initLogger,curProIsRun,curProName,versionInf
import transactionDataAnalysis as TransAna
import deviceDataAnalysis as DeviceAna
from deviceStatusMonitor import deviceMonitorMain 
import dbdataSync,afcDataPredict,etlInstance
import componentDataAnalysis,componentDataPredict
import resource
import threading

logger = logging.getLogger() 

class ObjTask(ScheduleBase):
    def __init__(self):
        self.name = "ObjTask"  
        
    def doTaskMain(self):
        logger.info("task begin")
        try: 
            # db data sync
            dbdataSync.dataSyncMain()
            
            # data etl
            etlInstance.dataEtlMain()
        
            # data analysis        
            TransAna.dataAnalysisMain()
            DeviceAna.dataAnalysisMain()
            componentDataAnalysis.dataAnalysisMain()
        
            # data predict
            afcDataPredict.dataPredictMain()
            componentDataPredict.dataPredictMain()
            
        except :
            logger.error(traceback.format_exc())
        
        logger.info("task end")
        return None

def afcAnaMain():
    cnf = ConfigData()    
    t = ObjTask()
    t.startTime = cnf.strCheckTime
    #t.daySeconds = cnf.checkInterval # for test
    #t.doTaskMain() # for test
    with open(cnf.pidFilePath,"w") as fout:
        fout.write(str(os.getpid()))
    thrdDeviceMonitor = threading.Thread(target=deviceMonitorMain)
    thrdDeviceMonitor.daemon = True
    thrdDeviceMonitor.start()
    
    t.run()

if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')      

    if len(sys.argv) < 2 :
        print "usage : %s default.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    print "config file :",fname
    
    cnf = ConfigData(fname)

    if (curProIsRun(cnf.pidFilePath)):
        print "another copy of process is already running"
        sys.exit(0)
    
    #cnf.show()
    resource.setrlimit(
        resource.RLIMIT_CORE,
        (resource.RLIM_INFINITY, resource.RLIM_INFINITY)
    )    
    
    if cnf.logEnable :
        initLogger(cnf.logFilePath,cnf.logFileMaxSize,cnf.logMaxFileNum,cnf.logLevel)
    logger.info("{0} started , {1}".format(curProName,versionInf))   
    createDaemon(afcAnaMain,cnf.daemon)  
    
    
    
