#! /usr/bin/env python
# -*- coding:utf-8 -*-

import sys,logging 
from DACommon import ConfigData
import etlInstance_db2
import etlInstance_pgdb

logger = logging.getLogger()
  
def dataEtlMain():
    logger.info("dataEtlMain begin")
    cnf = ConfigData()
    etlInstance_db2.etlUseDb2(cnf)
    etlInstance_pgdb.etlUsePgdb(cnf)
    logger.info("dataEtlMain end")
    return None
    
if __name__ == "__main__":
    reload(sys)
    sys.setdefaultencoding('utf-8')
    if len(sys.argv) < 2 :
        print "usage : %s etl.xml" % sys.argv[0]
        sys.exit(-1)
    fname = sys.argv[1]
    cnf = ConfigData(fname)
    cnf.show()
    dataEtlMain()
else :
    print "load etlInstance module"
    
    
