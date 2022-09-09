#! /usr/bin/env python
# -*- coding:utf-8 -*-

# db2 to pgsql and tsdb
import sys,binascii,logging
from DACommon import istext,ConfigData
from DACommon import getDb2Conn,getPgsqlConn
from TSDBCMN import getTsdbConn,ObjTsdbItemBase,fmtStrTimeToRfc3339

logger = logging.getLogger()

class ObjTsdbLineItem(ObjTsdbItemBase):
    def init(self):
        self.table = "ObjLine"
        
class ObjTsdbStationItem(ObjTsdbItemBase):
    def init(self):
        self.table = "ObjStation"

class ObjTsdbTicketTypeItem(ObjTsdbItemBase):
    def init(self):
        self.table = "RefTicketType"

class ObjTsdbTicketFamilyItem(ObjTsdbItemBase):
    def init(self):
        self.table = "REFTICKETFAMILY"
        
class ObjDsBase():
    def __init__(self,tsdbConn,dataArr):
        self.proName = "ObjDsBase"
        self.tsdbConn = tsdbConn
        self.dbName = tsdbConn.dbName
        self.maxNumPerCommit = 1000
        self.dataArr = dataArr
        self.init() 
    
    def fillItemData(self,refItem,objTmp):
        for kName,value in objTmp.iteritems():
            #value = str(value)
            #if not istext(value) :
            #    print "'%s' is not text" % value
            #    value = "0x" + binascii.b2a_hex(value)
            if kName in self.tagFields :
                refItem.tags[kName] = value
            else:
                refItem.fields[kName] = value
        for kName in self.timeFields :
            vtmp = refItem.fields[kName]
            refItem.fields[kName] = fmtStrTimeToRfc3339(vtmp)
       
    def run(self):
        print "[%s] run" % self.proName
        self.tsdbConn.createDb(self.dbName)
        self.doDataWrite()
        print "[%s] task done" % self.proName

        
class ObjDsLine(ObjDsBase):
    def init(self):   
        self.proName = "ObjDsLine"
        self.json_body = []
        self.tagFields = ["LINEID"]
        self.timeFields = []#["OPENINGDATE","CLOSINGDATE"]       
        
    def doDataWrite(self):
        self.tsdbConn.query("drop table %s;" % ObjTsdbLineItem().table)
        for i,objTmp in enumerate(self.dataArr):
            otItem = ObjTsdbLineItem()           
            self.fillItemData(otItem,objTmp)
            tMap = otItem.toDict()
            print "toDict : ",tMap
            self.json_body.append(tMap)
            if (i+1) % self.maxNumPerCommit == 0 :
                self.tsdbConn.write(self.json_body)
                self.json_body = []
                print "commit %d" % i 
            
        self.tsdbConn.write(self.json_body)
        self.json_body = [] 

 
class ObjDsStation(ObjDsBase):
    def init(self):   
        self.proName = "ObjDsStation"
        self.json_body = []
        self.tagFields = ["LINEID","STATIONID"]
        self.timeFields = []#["OPENINGDATE","CLOSINGDATE"]       
        
    def doDataWrite(self):
        self.tsdbConn.query("drop table %s;" % ObjTsdbStationItem().table)
        for i,objTmp in enumerate(self.dataArr):
            otItem = ObjTsdbStationItem()           
            self.fillItemData(otItem,objTmp)
            tMap = otItem.toDict()
            print "toDict : ",tMap
            self.json_body.append(tMap)
            if (i+1) % self.maxNumPerCommit == 0 :
                self.tsdbConn.write(self.json_body)
                self.json_body = []
                print "commit %d" % i 
            
        self.tsdbConn.write(self.json_body)
        self.json_body = [] 

class ObjDsTicketType(ObjDsBase):
    def init(self):   
        self.proName = "ObjDsTicketType"
        self.json_body = []
        self.tagFields = ["TICKETTYPE"]
        self.timeFields = []#["OPENINGDATE","CLOSINGDATE"]       
        
    def doDataWrite(self):
        self.tsdbConn.query("drop table %s;" % ObjTsdbTicketTypeItem().table)
        for i,objTmp in enumerate(self.dataArr):
            otItem = ObjTsdbTicketTypeItem()           
            self.fillItemData(otItem,objTmp)
            tMap = otItem.toDict()
            print "toDict : ",tMap
            self.json_body.append(tMap)
            if (i+1) % self.maxNumPerCommit == 0 :
                self.tsdbConn.write(self.json_body)
                self.json_body = []
                print "commit %d" % i 
            
        self.tsdbConn.write(self.json_body)
        self.json_body = [] 

class ObjDsTicketFamily(ObjDsBase):
    def init(self):   
        self.proName = "ObjDsTicketFamily"
        self.json_body = []
        self.tagFields = ["TICKETFAMILY"]
        self.timeFields = []
        
    def doDataWrite(self):
        self.tsdbConn.query("drop table %s;" % ObjTsdbTicketFamilyItem().table)
        for i,objTmp in enumerate(self.dataArr):
            otItem = ObjTsdbTicketFamilyItem()
            self.fillItemData(otItem,objTmp)
            tMap = otItem.toDict()
            print "toDict : ",tMap
            self.json_body.append(tMap)
            if (i+1) % self.maxNumPerCommit == 0 :
                self.tsdbConn.write(self.json_body)
                self.json_body = []
                print "commit %d" % i 
            
        self.tsdbConn.write(self.json_body)
        self.json_body = [] 
        
def lineSync_pgdb(result,pgConn):
    arrQuery = []
    arrQuery.append("delete from obj_line;")
    qPattern = "insert into obj_line (line_id,line_name) values (%d,'%s');"
    for objTmp in result:
        print objTmp        
        tid,tname = objTmp['LINEID'],objTmp['LINECNNAME']
        qtmp = qPattern % (tid,tname.encode("utf-8"))
        #qtmp = qPattern % (tid,str(tid))
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()

    return None

        
def lineSync(db2Conn,pgConn,tsdbConn):
    #query = "select * from ObjLine;"
    arrHeader = ["lineid","linecnname"]
    query = "select %s from ObjLine;" % (",".join(arrHeader))
    result = db2Conn.query(query)
    ObjDsLine(tsdbConn,result).run()

    #lineSync_pgdb(result,pgConn)
    msg = "lineSync done"
    logger.info(msg)
    return None

def stationSync_pgdb(result,pgConn):    
    arrQuery = []
    #arrQuery.append("delete from obj_station;")
    # insert into obj_station(line_id,station_id,station_name) values (8,115,'caochangmen') on conflict(line_id,station_id) do update set station_name = excluded.station_name;

    qPattern = "insert into obj_station (line_id,station_id,station_name) values (%d,%d,'%s') "
    qPattern += " on conflict(line_id,station_id) do update set station_name = excluded.station_name,sync_time=now();"
    
    for objTmp in result:
        print objTmp        
        lid,sid,sname = objTmp['LINEID'],objTmp['STATIONID'],objTmp['STATIONCNNAME']
        qtmp = qPattern % (lid,sid,sname.encode("utf-8"))
        #qtmp = qPattern % (tid,str(tid))
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()

    return None    
    
def stationSync(db2Conn,pgConn,tsdbConn):
    #query = "select * from ObjStation;"
    arrHeader = ["LineId","StationId","StationCnName"]
    query = "select %s from ObjStation;" % (",".join(arrHeader))
    result = db2Conn.query(query)    
    ObjDsStation(tsdbConn,result).run()
    #stationSync_pgdb(result,pgConn)    
    msg = "stationSync done"
    logger.info(msg)
    return None

def ticketSync(db2Conn,pgConn,tsdbConn):
    #query = "select * from REFTICKETTYPE;"
    # TICKETTYPE, TICKETFAMILY, SOUVENIRFLAG, NAMEDSVTICKETFLAG, SOUNDDISPLAYID, CONCESSIONALLAMPID, TICKETTYPECNNAME, TICKETTYPEENNAME
   
    #arrHeader = ["TicketType","TicketTypeCnName"]
    arrHeader = ['TICKETTYPE', 'TICKETFAMILY', 'SOUVENIRFLAG', 'NAMEDSVTICKETFLAG', 'SOUNDDISPLAYID', 'CONCESSIONALLAMPID', 'TICKETTYPECNNAME', 'TICKETTYPEENNAME']
   
    query = "select %s from RefTicketType;" % (",".join(arrHeader))
    result = db2Conn.query(query)
    
    ObjDsTicketType(tsdbConn,result).run()
    
    arrQuery = []
    arrQuery.append("delete from ticket_type;")
    #qPattern = "insert into ticket_type (ticket_id,ticket_name) values (%d,'%s');"
    qPattern = "insert into ticket_type (ticket_id,family_id,souvenirflag,namedsvticketflag,sounddisplayid,concessionallampid,ticket_cn_name,ticket_en_name) "
    qPattern += "values (%d,%d,%d,%d,%d,%d,'%s','%s');"
    for objTmp in result:
        print objTmp        
        tid,tcnname,tenname = objTmp['TICKETTYPE'],objTmp['TICKETTYPECNNAME'],objTmp['TICKETTYPEENNAME']
        family_id = objTmp['TICKETFAMILY']
        souvenirflag = objTmp['SOUVENIRFLAG']        
        namedsvticketflag = objTmp['NAMEDSVTICKETFLAG'] 
        sounddisplayid = objTmp['SOUNDDISPLAYID'] 
        concessionallampid = objTmp['CONCESSIONALLAMPID'] 
        
        qtmp = qPattern % (tid,family_id,souvenirflag,namedsvticketflag,sounddisplayid,concessionallampid,tcnname.encode("utf-8"),tenname)
        #qtmp = qPattern % (tid,str(tid))
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
    
    msg = "ticketSync done"
    logger.info(msg)
    return None
    
def ticketFamilySync(db2Conn,pgConn,tsdbConn):
    #query = "select * from REFTICKETFAMILY;"
    arrHeader = ['TICKETFAMILY', 'CNNAME', 'ENNAME']
   
    query = "select %s from REFTICKETFAMILY;" % (",".join(arrHeader))
    result = db2Conn.query(query)
    
    ObjDsTicketFamily(tsdbConn,result).run()
    
    arrQuery = []
    arrQuery.append("delete from ticket_family;")
    qPattern = "insert into ticket_family (family_id,cn_name,en_name) values (%d,'%s','%s');"    
    for objTmp in result:
        print objTmp      
        family_id = objTmp['TICKETFAMILY']
        cn_name = objTmp['CNNAME']        
        en_name = objTmp['ENNAME']         
        qtmp = qPattern % (family_id,cn_name.encode("utf-8"),en_name)
        #qtmp = qPattern % (tid,str(tid))
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
    
    msg = "ticketSync done"
    logger.info(msg)
    return None
    
def moduleTypeSync(db2Conn,pgConn):
    query = "select * from REFMODULETYPE;" 
    result = db2Conn.query(query)
    
    arrQuery = []
    arrQuery.append("delete from ref_module_type;")
    qPattern = "insert into ref_module_type (module_type_code,display_wording_cn,display_wording_en) values ('%s','%s','%s');"
    for objTmp in result:
        print objTmp        
        tid,cnName,enName = objTmp['MODULETYPECODE'],objTmp['DISPLAYWORDING_CN'],objTmp['DISPLAYWORDING_EN']
        qtmp = qPattern % (tid,cnName.encode("utf-8"),enName)
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
    
    msg = "moduleTypeSync done"
    logger.info(msg)
    return None    
    
def refPackageSync(db2Conn,pgConn):
    query = "select * from REFPACKAGE;" 
    result = db2Conn.query(query)
    
    arrQuery = []
    arrQuery.append("delete from ref_package;")
    qPattern = "insert into ref_package (package_id,package_name,display_name,package_host,port) values (%d,'%s','%s','%s',%d);"
    for objTmp in result:
        print objTmp        
        tid,enName,cnName = objTmp['PACKAGEID'],objTmp['PACKAGENAME'],objTmp['DISPLAYNAME']
        host,port = objTmp['PACKAGEHOST'],objTmp['PORT']
        qtmp = qPattern % (tid,enName,cnName.encode("utf-8"),host,port)
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
    
    msg = "refPackageSync done"
    logger.info(msg)
    return None  

def refEquipmentTypeSync(db2Conn,pgConn):
    query = "select * from REFEQUIPMENTTYPE;" 
    result = db2Conn.query(query)
    
    arrQuery = []
    arrQuery.append("delete from equipment_type;")
    qPattern = "insert into equipment_type (equipment_type_id,name,short_name) values (%d,'%s','%s');"
    for objTmp in result:
        print objTmp        
        tid,cnName,shortName = objTmp['EQUIPMENTTYPEID'],objTmp['DISPLAYNAME_CN'],objTmp['SHORTNAME']        
        qtmp = qPattern % (tid,cnName.encode("utf-8"),shortName)
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
    
    msg = "refEquipmentTypeSync done"
    logger.info(msg)
    return None  

def refEquipmentSubTypeSync(db2Conn,pgConn):
    query = "select * from REFEQUIPMENTSUBTYPE;" 
    result = db2Conn.query(query)
    
    arrQuery = []
    arrQuery.append("delete from equipment_subtype;")
    qPattern = "insert into equipment_subtype (equipment_subtype_id,equipment_type_id,name,short_name) values (%d,%d,'%s','%s');"
    for objTmp in result:
        print objTmp        
        subId,tid = objTmp['EQUIPMENTSUBTYPEID'],objTmp['EQUIPMENTTYPEID']
        cnName,shortName = objTmp['DISPLAYNAME_CN'],objTmp['SHORTNAME']        
        qtmp = qPattern % (subId,tid,cnName.encode("utf-8"),shortName)
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
    
    msg = "refEquipmentSubTypeSync done"
    logger.info(msg)
    return None  

def objEquipmentSync(db2Conn,pgConn):
    query = "select * from OBJEQUIPMENT;" 
    result = db2Conn.query(query)
    
    arrQuery = []
    arrQuery.append("delete from obj_equipment;")
    qPattern = "insert into obj_equipment (equipment_id,equipment_subtype_id,equipment_type_id,line_id,station_id,name) values (%d,%d,%d,%d,%d,'%s');"
    for objTmp in result:
        print objTmp 
        eqId,subTid,tid = objTmp['EQUIPMENTID'],objTmp['EQUIPMENTSUBTYPEID'],objTmp['EQUIPMENTTYPEID']
        line_id,station_id,name = objTmp['LINEID'],objTmp['STATIONID'],objTmp['MACHINENAME']    
        qtmp = qPattern % (eqId,subTid,tid,line_id,station_id,name)
        arrQuery.append(qtmp)
    
    for query in arrQuery :
        print query
        pgConn.execute(query)
    pgConn.commit()
      
    msg = "objEquipmentSync done"
    logger.info(msg)
    return None 
    
def dataSyncMain():
    logger.info("dataSyncMain begin")
    cnf = ConfigData()
    db2Conn = getDb2Conn(cnf)
    pgConn = getPgsqlConn(cnf)
    tsdbConn = getTsdbConn(cnf)

    lineSync(db2Conn,pgConn,tsdbConn)
    stationSync(db2Conn,pgConn,tsdbConn)
    ticketSync(db2Conn,pgConn,tsdbConn)
    ticketFamilySync(db2Conn,pgConn,tsdbConn)
    moduleTypeSync(db2Conn,pgConn)
    refPackageSync(db2Conn,pgConn)
    refEquipmentTypeSync(db2Conn,pgConn)
    refEquipmentSubTypeSync(db2Conn,pgConn)
    objEquipmentSync(db2Conn,pgConn)
    
    pgConn.close()    
    db2Conn.close()
    logger.info("dataSyncMain end")
    
if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')
    if len(sys.argv) < 2 :
        print "usage : %s default.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    cnf = ConfigData(fname)
    cnf.show()
    dataSyncMain()
    
else :
    print "load dbdataSync module"
    
    
    
