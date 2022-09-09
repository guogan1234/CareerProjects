#! /usr/bin/env python
# -*- coding:utf-8 -*-

import os,sys,psycopg2,csv,datetime,resource,traceback
from dateutil.relativedelta import relativedelta
from dasBase import ConfigData,curProIsRun,createDaemon,ScheduleBase,strTime2Int

gIgnoreFields = ["id","insert_time"]

arrTables_day_history = [
    ('station_time_sharing_passenger_flow' , "flow_timestamp"),
    #('station_time_sharing_predict_flow' , "flow_timestamp"),
    ('station_cumulative_passenger_flow' , "flow_timestamp"),
    #('station_cumulative_predict_flow' , "flow_timestamp"),
    ('line_time_sharing_passenger_flow' , "flow_timestamp"),
    #('line_time_sharing_predict_flow' , "flow_timestamp"),
    ('line_cumulative_passenger_flow' , "flow_timestamp"),
    #('line_cumulative_predict_flow' , "flow_timestamp"),
    ('ticket_share_passenger_flow' , "flow_timestamp"),
    ('ticket_cumulative_passenger_flow' , "flow_timestamp"),
    
    ('power_system_energy_analysis',"analysis_timestamp"),
    ('ventilation_system_energy_analysis',"analysis_timestamp"),
    ('air_condition_system_energy_analysis',"analysis_timestamp"),
    ('escalator_system_energy_analysis',"analysis_timestamp"),
    ('lighting_system_energy_analysis',"analysis_timestamp"),
]

arrTables_day_predict = [    
    ('station_time_sharing_predict_flow' , "flow_timestamp"),   
    ('station_cumulative_predict_flow' , "flow_timestamp"),   
    ('line_time_sharing_predict_flow' , "flow_timestamp"),   
    ('line_cumulative_predict_flow' , "flow_timestamp"),  
]

arrTables_month = [
    ('line_failure_analysis',"analysis_timestamp"),
    ('module_failure_analysis',"analysis_timestamp"),
    ('device_failure_analysis',"analysis_timestamp"),
    ('device_failure_analysis_detail',"analysis_timestamp"),
]
    
def getDtRange(csvFileName,timeFiled):
    dtMin,dtMax = None,None    
    arrColumns = None
    with open(csvFileName) as fin:
        for line in csv.reader(fin):
            if arrColumns == None : 
                arrColumns = list(line)
                continue
            dtmp = dict(zip(arrColumns,line))
            s1 = dtmp[timeFiled]
            dtTmp = datetime.datetime.strptime(s1,"%Y-%m-%d %H:%M:%S")
            if not dtMin : dtMin = dtTmp
            dtMin = min(dtMin,dtTmp)
            if not dtMax : dtMax = dtTmp
            dtMax = max(dtMax,dtTmp)
            #print line

    print "dtMin :",dtMin," dtMax :",dtMax
    return dtMin,dtMax    

def getTimeDiff_days_history(csvFileName,timeFiled):     
    dtMin,dtMax = getDtRange(csvFileName,timeFiled)
    if dtMax :
        yesterday = datetime.date.today() - datetime.timedelta(days=1)
        #today = datetime.date.today()
        diff = yesterday - dtMax.date() 
        tDiffValue = diff.days
    return tDiffValue
    
def getTimeDiff_days_predict(csvFileName,timeFiled):
    dtMin,dtMax = getDtRange(csvFileName,timeFiled)
    if dtMin :
        today = datetime.date.today()
        diff = today - dtMin.date() 
        tDiffValue = diff.days
    return tDiffValue 

def getTimeDiff_months(csvFileName,timeFiled):
    dtMin,dtMax = getDtRange(csvFileName,timeFiled)
    if dtMax :
        today = datetime.date.today()
        #diff = today - dtMax.date() 
        diff = (today.year * 12 + today.month) - (dtMax.year * 12 + dtMax.month) - 1
        if diff < 0 : diff = 0
        tDiffValue = diff
    return tDiffValue   

def flushData2DB(csvFileName,tbName,tDiffValue,timeFiled,dbConn):
    diffMonths,diffDays = tDiffValue    
    pattern = '''insert into {tbName}({columns}) values ({values});'''
    loop = True
    chunkSize = 2000
    cur = dbConn.cursor()
    query = "delete from %s;" % tbName 
    cur.execute(query) 

    arrColumns,newColumns = None,[]
    i = 0
    with open(csvFileName,"rb") as fin:
        for line in csv.reader(fin):
            if arrColumns == None : 
                arrColumns = list(line)
                for field in arrColumns :
                    if not field in gIgnoreFields :
                        newColumns.append(field)
                continue
            #print line
            line = [unicode(cell, 'utf-8') for cell in line]
            #print line
            tMap = dict(zip(arrColumns,line))
            if i < 3 :
                for k,v in tMap.iteritems():
                    if (len(v) == 0) and (k in newColumns):
                        newColumns.remove(k)
            s1 = tMap[timeFiled]
            dtTmp = datetime.datetime.strptime(s1,"%Y-%m-%d %H:%M:%S")
            newDate = dtTmp + relativedelta(months=diffMonths,days=diffDays)
            tMap[timeFiled] = str(newDate)
            newLine = []
            for k in newColumns :
                item = "'%s'" % tMap[k]
               
                newLine.append(item.encode("utf-8"))
            query = pattern.format(
                tbName = tbName,
                columns = ",".join(newColumns),
                values = ",".join(newLine),
            )
            print query
            #raw_input()                
            cur.execute(query) 
            i += 1
            if (i % 1000) == 0 : 
                print i
                dbConn.commit()  
        dbConn.commit()

    print "%s task done" % tbName
    
def csv2Pgdb(csvDir,dbConn):    
    try :
        # dispose history data
        for tbName,timeFiled in arrTables_day_history :    
            fileName = "%s/%s.csv" % (csvDir,tbName)
            tDiffValue = getTimeDiff_days_history(fileName,timeFiled)        
            flushData2DB(fileName,tbName,(0,tDiffValue),timeFiled,dbConn)
            
        # dispose predict data    
        for tbName,timeFiled in arrTables_day_predict :    
            fileName = "%s/%s.csv" % (csvDir,tbName)
            tDiffValue = getTimeDiff_days_predict(fileName,timeFiled)        
            flushData2DB(fileName,tbName,(0,tDiffValue),timeFiled,dbConn)
        
        # dispose month data 
        for tbName,timeFiled in arrTables_month :
            fileName = "%s/%s.csv" % (csvDir,tbName)
            tDiffValue = getTimeDiff_months(fileName,timeFiled)  
            flushData2DB(fileName,tbName,(tDiffValue,0),timeFiled,dbConn)
    except :
        print traceback.format_exc()
    return None     
    

    
