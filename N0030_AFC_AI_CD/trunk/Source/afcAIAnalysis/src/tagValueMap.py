#! /usr/bin/env python
#-*- coding : utf-8 -*-

#import sys

#reload(sys)
#sys.setdefaultencoding('utf-8')  
import os

gWarnMap = {
    "BNACOM" : ['1'],
    "NNVAST" : ['2','3'],
    "NVAUEX" : ['1'],
    "NVAUIN" : ['1'],
    "BNAJAM" : ['1'],
    "BNASTA" : ['1'],
}

dEventLevel = {
    0 : "normal",
    1 : "warn",
    2 : "alert",
    3 : "exit service",
}

class ObjTag():
    def __init__(self,value,level):
        self.value = value
        self.level = level

gEventTagMap = {
    (1,"GlobalStatus") : { # 总体状态
        "METSER" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,0),
            ObjTag(3,0),
            ObjTag(4,0),
        ],
    },
    (2,"BanknotesChange") : { # 纸币找零
        "METBCG" : [ 
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
            ObjTag(4,0),        
        ],
        "BCGCOM" : [
            ObjTag(0,0),
            ObjTag(1,3),
            ObjTag(2,4),
        ],
        "BCGSTA" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BCGCT1" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
            ObjTag(4,0),
        ],
        "BCGT1O" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BCGT1I" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BCGCT2" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
            ObjTag(4,0),
        ],
        "BCGT2O" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BCGT2I" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BCGCT3" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
            ObjTag(4,0),
        ],
        "BCGT3O" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BCGT3I" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],        
    },    
    (3,"BanknotesReceiving") : { # 纸币接收
        "METBNA" : [
             ObjTag(0,0),
             ObjTag(1,0),
             ObjTag(2,2),
             ObjTag(3,3),
        ],
        "BNACOM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "NNVAST" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
        ],
        "NVAUEX" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "NVAUIN" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BNAJAM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "BNASTA" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
    },
    (4,"CoinModule") : { # 硬币模块
        "CHGSTA" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "NHO1ST" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
            ObjTag(4,0),
        ],
        "CHSSTA" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
         "HO1AEX" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "HO1AIN" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "HO1ERR" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "NHO2ST" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
            ObjTag(4,0),
        ],
        "HO2ERR" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "HO2AEX" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "HO2AIN" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "NDR1ST" : [
            ObjTag(0,0),
            ObjTag(1,3),
            ObjTag(2,0),
        ],
        "NDR2ST" : [
            ObjTag(0,0),
            ObjTag(1,3),
            ObjTag(2,0),
        ],
        "METCHS" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
        "CHSCOM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "DR1STA" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,3),
        ],
        "DR1AEX" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "DR1AIN" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],

    },
    (5,"SystemEvent") : { # 系统事件
        "BUSDAY" : [
            ObjTag(0,0),
            ObjTag(1,0),
        ],
        "COMSTA" : [
            ObjTag(0,0),
            ObjTag(1,3),# bit0 = 1
            ObjTag(2,3),# bit1 = 1
            ObjTag(4,3),# bit2 = 1
        ],
        "NLOGON" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,0),
            ObjTag(3,0),
            ObjTag(4,0),
        ],
        "TIMDIS" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "NACTAI" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "ACCCOM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "TEMOFR" : [
            ObjTag(0,0),
            ObjTag(1,2),
        ],
        "INTRST" : [
            ObjTag(0,0),
            ObjTag(1,2),
        ],
        "BREAKS" : [
            ObjTag(0,0),
            ObjTag(1,0),
        ],
        "DORPOS" : [
            ObjTag(0,0),
            ObjTag(1,2),
        ],
    },
    (6,"ChannelControl") : { #  通道控制
        "PASINT" : [
            ObjTag(0,0),
            ObjTag(1,2),
        ],
        "AILSTA" : [
            ObjTag(0,0),
            ObjTag(1,0),
        ],
        "SENFAL" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "PLCSTA" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "FLAPST" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "FLAFOR" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "PLCCOM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "METPLC" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
    },
    (7,"DataStore") : { # 数据存储
        "DAT1US" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
        ],
        "DAT2US" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
        ],
        "METEMM" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
    },
    (8,"ModeEvent") : { # 模式事件
        "NMDSTA" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,0),
            ObjTag(3,0),
            ObjTag(4,2),
            ObjTag(5,2),
            ObjTag(6,2),
            ObjTag(7,2),
            ObjTag(8,2),
            ObjTag(9,2),
            ObjTag(255,0),
        ],
        "SERSTA" : [
           ObjTag(0,0), 
           ObjTag(1,1), # bit1 = 1
           ObjTag(2,1), # bit2 = 1
           ObjTag(4,1), # bit3 = 1
           ObjTag(8,2), # bit4 = 1
           ObjTag(16,2), # bit5 = 1
           ObjTag(32,2), # bit6 = 1
           ObjTag(64,2), # bit7 = 1
           ObjTag(128,2), # bit8 = 1
           ObjTag(256,2), # bit9 = 1
           ObjTag(512,2), # bit10 = 1
           ObjTag(1024,2), # bit11 = 1
        ],
        "AILDIR" : [
           ObjTag(1,0),
           ObjTag(2,0),
           ObjTag(3,0),
        ],
    },
    (9,"RecoveryModule") : { # 回收模块
        "CINJAM" : [
           ObjTag(0,0), 
           ObjTag(1,3), 
        ],
        "CINCS1" : [
           ObjTag(0,0), 
           ObjTag(1,2), 
           ObjTag(2,3), 
           ObjTag(3,0), 
        ],
        "CINCS2" : [
            ObjTag(0,0),
            ObjTag(1,2), 
            ObjTag(2,3), 
            ObjTag(3,0),
        ],
        "CINCS3" : [
            ObjTag(0,0),
            ObjTag(1,2), 
            ObjTag(2,3), 
            ObjTag(3,0),
        ],
        "CINSTA" : [
            ObjTag(0,0),
            ObjTag(1,3), 
        ],
        "METCTN" : [
            ObjTag(0,0),
            ObjTag(1,0), 
            ObjTag(2,2), 
            ObjTag(3,3),
        ],
    },
    (10,"CardIssuerMode") : { # 发卡模块
        "CSTSTA" : [
            ObjTag(0,1),
            ObjTag(1,2),
        ],
        "METCST" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
        "CSTCOM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "CSTCS1" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,0),
        ],
        "CSTCS2" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,0),
        ],

        "CSTCS3" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,3),
            ObjTag(3,0),
        ],

    },
    (11,"Reader2") : { # 读写器2 
        "CS2KEY" : [
            ObjTag(0,0), 
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1 
            ObjTag(4,3), # bit2 = 1 
            ObjTag(8,3), # bit3 = 1 
        ],
        "CS2EOD" : [
            ObjTag(0,0), 
            ObjTag(1,3), # bit0 = 1
            ObjTag(2,3), # bit1 = 1
        ],
        "CS2STA" : [
            ObjTag(0,0), 
            ObjTag(1,3), # bit0 = 1
        ],
        "CS2DWN" : [
            ObjTag(0,0),
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1 
            ObjTag(4,3), # bit2 = 1 
        ],
        "CS2COM" : [
            ObjTag(0,0),
            ObjTag(1,3),  
        ],
        "METCS2" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
    },
    (12,"Reader1") : { # 读写器1
        "CS1KEY" : [
            ObjTag(0,0),
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1 
            ObjTag(4,3), # bit2 = 1 
            ObjTag(8,3), # bit3 = 1 
        ],
        "CS1EOD" : [
            ObjTag(0,0),
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1                 
        ],
        "CS1STA" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "CS1DWN" : [
            ObjTag(0,0),
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1 
            ObjTag(4,3), # bit2 = 1 
        ],
        "CS1COM" : [
            ObjTag(0,0),
            ObjTag(1,3),
        ],
        "METCS1" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
    },
    (13,"Printer") : { # 打印机
        "METRPU" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,2),
            ObjTag(3,2),
            ObjTag(4,2),
        ],
        "RPUMET" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
    },
    (14,"ParameterEvent") : { # 参数事件
        "METEOD" : [
            ObjTag(0,0),      
            ObjTag(1,0),      
            ObjTag(2,2),      
            ObjTag(3,3),      
        ],

        "NEDFAI" : [ 
            ObjTag(0,0), 
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1 
            ObjTag(4,3), # bit2 = 1 
            ObjTag(8,3), # bit3 = 1 
        ],

        "EODDWN" : [ 
            ObjTag(0,0), 
            ObjTag(1,3), # bit0 = 1 
            ObjTag(2,3), # bit1 = 1 
            ObjTag(4,3), # bit2 = 1 
            ObjTag(8,3), # bit3 = 1 
        ],
    },
    (15,"UPS") : {
        "METUPS" : [
            ObjTag(0,0),
            ObjTag(1,2),
            ObjTag(2,2),
            ObjTag(3,2),
        ],
        "UPSMET" : [
            ObjTag(0,0),
            ObjTag(1,0),
            ObjTag(2,2),
            ObjTag(3,3),
        ],
    },
}

def getTagModMap():
    dTagMod = {}
    for modName,modMap in gEventTagMap.iteritems():
        for tagName,arrTagValue in modMap.iteritems():
            if not tagName in dTagMod : 
                dTagMod[tagName] = []
            dTagMod[tagName].append(modName)
   
    #for k,v in dTagMod.iteritems():
    #    print k,v
    return dTagMod

def getWarnMap():
    dWarn = {}
    for modName,modMap in gEventTagMap.iteritems():
        for tagName,arrTagValue in modMap.iteritems():
            if not tagName in dWarn : 
                dWarn[tagName] = {"modName" : modName,"arrValue" : []}
            refArrValue = dWarn[tagName]["arrValue"]
            for objItem in arrTagValue :
                if objItem.level in [3,]:
                    refArrValue.append(objItem.value)                    
    #print dWarn
    #for k,v in dWarn.iteritems():
    #    print k,v
        
    return dWarn

if __name__ == "__main__" :
    getTagModMap()
    print getWarnMap()

else :
    print "load %s module" % os.path.basename(__file__).split(".")[0]




