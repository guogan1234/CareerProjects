#-*- coding:utf-8 -*-

import os
import time
import tsdbpy
import traceback,string
from datetime import datetime

    
class ObjTSDBConnection():
    def __init__(self,host,port,user,passwd,dbName):
        self.host = host
        self.port = port
        self.user = user
        self.passwd = passwd
        self.dbName = dbName
        self.handle = tsdbpy.TSDBClient(
            self.host,self.port,self.user,
            self.passwd,self.dbName
        )
        
    def createDb(self,dbName):
        try :
            self.handle.create_database(dbName)
        except :
            print "create database error"
    
    def write(self,dataArr):
        try :
            self.handle.write_points(dataArr)
        except tsdbpy.exceptions.TSDBServerError:
            print "TSDBServerError occur,do reInitConn"
            self.reInitConn()
            
    def query(self,qstr,epoch=None):
        qResult = None
        try :
            qResult = self.handle.query(qstr,None,epoch)
        except :
            print "query error : ",qstr
        return qResult        
        
    def reInitConn(self):
        bret,errMsg = False,""
        try :
            self.handle = tsdbpy.TSDBClient(
                self.host,self.port,self.user,
                self.passwd,self.dbName
            )
            bret = True
            errMsg = "reInitConn ok!"
        except :
            errMsg = "reInitConn fail!"
        print errMsg
        return bret

def getTsdbConn(cnf):
    host,port,user,passwd,dbName = cnf.getDbParams("tsdb")
    #host,port,user,passwd,dbName = '127.0.0.1',8086,'root','root','example'
    client = ObjTSDBConnection(host,port,user,passwd,dbName)
    return client     

class ObjTsdbItemBase():
    def __init__(self):
        self.table = ""
        self.tags = {}
        self.time = "" 
        self.fields = {}
        self.init()
        
    def init(self):
        pass
        
    def toDict(self):
        dtmp = {
            "table" : self.table,
            "tags" : self.tags,            
            "fields" : self.fields,
        }
        if self.time :
            dtmp["time"] = self.time
        return dtmp      
        
def fmtStrTimeToRfc3339(strTmp):
    arrTmp = strTmp.split(" ")
    if len(arrTmp) == 1 : arrTmp.append("00:00:00")
    retStr = "%sT%sZ" % (arrTmp[0],arrTmp[1])
    return retStr    
    
def getLineInfo(tsdbConn):
    # get line info
    arrLineInfo = []
    data = tsdbConn.query("select * from ObjLine;")
    for item in list(data.get_points()) :
        #print item
        arrLineInfo.append(int(item['LINEID']))
    
    arrLineInfo = sorted(arrLineInfo)
    print arrLineInfo
    return arrLineInfo
    
def getStationInfo(tsdbConn): 
    # get station info
    dStationInfo = {}
    data = tsdbConn.query("select * from ObjStation;")
    for item in list(data.get_points()) :
        #print item
        lineId,stationId = int(item['LINEID']),int(item['STATIONID'])
        if not lineId in dStationInfo : dStationInfo[lineId] = []
        dStationInfo[lineId].append(stationId)            
    print dStationInfo
    #print sorted(dStationInfo.items(),key=lambda x:x[0])
    #print dStationInfo.keys()
    return dStationInfo
