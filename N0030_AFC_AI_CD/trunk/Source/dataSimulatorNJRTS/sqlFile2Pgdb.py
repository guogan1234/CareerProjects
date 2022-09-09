#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,psycopg2,traceback
from dasBase import ConfigData

arrFiles = [
    'afccd_pgsql_init.sql',
    'afccd_pgsql_fill_data.sql',
    'equipment_subtype.sql',
    'objEquipment.sql',
    'obj_station.sql',
    'ref_package.sql',
    'equipment_type.sql',
    'obj_line.sql',
    'ref_module_type.sql',
    'ticket_type.sql',
]   
   
def sqlFile2Pgdb(fname,dbConn):
    cur = dbConn.cursor()
    with open(fname,"r") as fin :
        cur.execute(fin.read())        
    dbConn.commit()     
          
def doTask(fname,sqlFile): 
    cnf = ConfigData(fname)  

    dbCnnInfo = {
        'host' : cnf.pgsql_host,
        'dbname' : cnf.pgsql_dbName,
        'port' : str(cnf.pgsql_port),
        'user' : cnf.pgsql_user,
        'password' : cnf.pgsql_password,
    }
    print dbCnnInfo
 
    dbConn = None
    try:
        dbConn = psycopg2.connect(**dbCnnInfo)
        dbConn.autocommit = False
    except:
        print "I am unable to connect to the database"
        
    if dbConn : 
        sqlFile2Pgdb(sqlFile,dbConn)
        print "%s : import ok" % sqlFile 
    return None

if __name__ == "__main__":    
    if len(sys.argv) < 3 :
        print "usaage : %s confFile sqlFileDir" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    sqlDir = sys.argv[2]
    for item in arrFiles :
        sqlFile = item
        try :
            sqlFile = os.path.join(sqlDir,item)
            doTask(fname,sqlFile)
        except :
            print "import %s fail" % sqlFile
            print traceback.format_exc()
    
    
    
    
   

    
    
    
    
