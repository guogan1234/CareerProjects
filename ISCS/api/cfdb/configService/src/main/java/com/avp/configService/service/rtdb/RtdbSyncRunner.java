package com.avp.configService.service.rtdb;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.jdbc.support.rowset.SqlRowSet;
import org.springframework.jdbc.support.rowset.SqlRowSetMetaData;
import org.springframework.stereotype.Service;

import javax.sql.rowset.RowSetMetaDataImpl;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by boris feng on 2018/6/1.
 */
@Service
public class RtdbSyncRunner implements CommandLineRunner {
    private Logger logger = LoggerFactory.getLogger(RtdbSyncRunner.class);

    @Autowired
    RtdbSyncListener rtdbSyncListener;

//    @Autowired
//    RtdbSyncExecutor rtdbSyncExecutor;

    @Override
    public void run(String... args) throws Exception {
        rtdbSyncListener.start();

//        SqlRowSet rowSet = rtdbSyncExecutor.pgsqlForRowSet("obj_user", 0, 0, 100);
//        SqlRowSetMetaData metaData = rowSet.getMetaData();
//        int columnCount = metaData.getColumnCount();
//        Map<String, Integer> columns = new HashMap<>();
//        for (int i = 1; i <= columnCount; i++) {
//            String col = metaData.getColumnName(i);
//            Integer type = Integer.valueOf(metaData.getColumnType(i));
//            String typeName = metaData.getColumnTypeName(i);
//            logger.debug("sql={},{},{}", col, typeName, type);
//            columns.put(col, type);
//        }
    }
}
