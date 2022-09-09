#! /usr/bin/env python
# -*- coding:utf-8 -*-

import sys,logging,subprocess,shlex,time,traceback
from DACommon import ConfigData
from DACommon import getPgsqlConn

logger = logging.getLogger()

def icmpCheck(hostName): 
    bRet = False
    cmd = "ping -c 1 %s" % hostName # only in linux 
    args = shlex.split(cmd)
    try:
        subprocess.check_call(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)
        bRet = True
    except subprocess.CalledProcessError:
        pass
    return bRet

def deviceMonitorMain():
    cnf = ConfigData()
    checkInterval = cnf.deviceMonitorInterval 
    if checkInterval < 30 : checkInterval = 30
    tbName = "core_switch_status"
    queryClean = "delete from %s;" % tbName
    pattern = "insert into %s (ip,name,status) values (%s); "
    
    while True : 
        try :
            arrQuery = []
            arrQuery.append(queryClean)
            for objTmp in cnf.arrSwitchInfo : 
                print objTmp.ip,objTmp.name
                status = 0
                if not icmpCheck(objTmp.ip) : status = 1
                sTmp = "'%s','%s',%d" % (objTmp.ip,objTmp.name,status)
                query = pattern % (tbName,sTmp)
                #print query
                arrQuery.append(query)
                
            pgConn = getPgsqlConn(cnf)            
            for query in arrQuery :
                print query
                pgConn.execute(query)
            pgConn.commit()
            pgConn.close()    
        except :
            logger.error(traceback.format_exc())
            
        time.sleep(checkInterval)
    return None
    
if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')
    if len(sys.argv) < 2 :
        print "usage : %s default.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    cnf = ConfigData(fname)
    cnf.show()
    if cnf.deviceMonitorEnabled :
        deviceMonitorMain()
    else :
        print "module not enabled "
else :
    print "load deviceMonitor module"
    
    
    
