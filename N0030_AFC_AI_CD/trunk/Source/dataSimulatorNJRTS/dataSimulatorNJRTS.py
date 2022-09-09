#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,psycopg2,resource
#from csvFile2Pgdb import csv2Pgdb
from csvFile2Pgdb_nopd import csv2Pgdb
from dasBase import ConfigData,curProIsRun,createDaemon,ScheduleBase
    
class ObjTask(ScheduleBase):
    def __init__(self):
        self.name = "ObjTask"  
        
    def doTaskMain(self):
        dbConn = None
        cnf = ConfigData()   
        dbCnnInfo = {
            'host' : cnf.pgsql_host,
            'dbname' : cnf.pgsql_dbName,
            'port' : str(cnf.pgsql_port),
            'user' : cnf.pgsql_user,
            'password' : cnf.pgsql_password,
        }
        print dbCnnInfo
        try:    
            dbConn = psycopg2.connect(**dbCnnInfo)
            dbConn.autocommit = False
            csvDir = cnf.csvFilesDir
        except :
            print "I am unable to connect to the database"        
        if dbConn : 
            csv2Pgdb(csvDir,dbConn)
            print "%s : import ok" % csvDir 
     
        return None

def main():
    cnf = ConfigData()   
    t = ObjTask()
    t.startTime = cnf.strCheckTime
    #t.daySeconds = cnf.checkInterval # for test
    #t.doTaskMain() # for test
    with open(cnf.pidFilePath,"w") as fout:
        fout.write(str(os.getpid()))
    t.run()
    
if __name__ == "__main__":    
    if len(sys.argv) < 2 :
        print "usaage : %s confFile" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    print "config file :",fname    
    cnf = ConfigData(fname)   

    if (curProIsRun(cnf.pidFilePath)):
        print "another copy of process is already running,pid : ",
        with open(cnf.pidFilePath) as fin : print fin.read() 
        sys.exit(0)
    #cnf.show()
    resource.setrlimit(
        resource.RLIMIT_CORE,
        (resource.RLIM_INFINITY, resource.RLIM_INFINITY)
    )
    createDaemon(main,cnf.daemon)  
    
    
