package com.avp.configService.service.rtdb;

import org.springframework.util.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.support.rowset.SqlRowSet;
import org.springframework.jdbc.support.rowset.SqlRowSetMetaData;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

import java.io.BufferedInputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.nio.charset.Charset;
import java.sql.Types;
import java.util.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by boris feng on 2018/6/1.
 */
@Service
public class RtdbSyncExecutor {
    static int RTDB_SYNC_HEADER_MIN_SIZE = 16;
    private Logger logger = LoggerFactory.getLogger(RtdbSyncExecutor.class);

    @Value("${iscs.confdb.rtdb.page-size:3000}")
    private Integer pageSize;

    @Value("${iscs.confdb.rtdb.sync-size:300}")
    private Integer syncSize;//not used, reserved

    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Async
    public void execSync(Socket client) {
        try {
            DataInputStream input = new DataInputStream(new BufferedInputStream(client.getInputStream()));
            DataOutputStream output = new DataOutputStream(client.getOutputStream());

            int length = input.readInt();
            logger.debug("command lenght = {}", length);
            if (length > RTDB_SYNC_HEADER_MIN_SIZE) {
                //header
                int msgLength = length;
                int domainId = input.readInt();
                logger.debug("domainId = {}", domainId);

                int optType = input.readUnsignedByte();
                logger.debug("optType = {}", optType);

                long lastQueryTime = input.readLong();
                logger.debug("lastQueryTime = {}/{}", lastQueryTime, new Date(lastQueryTime));

                String tableName = readAsciiChars(input);
                logger.debug("tableName = {}", tableName);

                //body -- empty

                //tail
                int crc16 = input.readUnsignedShort();

                // now only support full load, opt=0
                // todo: opt=1
                int pageNumber = 0;
                Map<String, Integer> columns = new HashMap<>();
                do {
                    SqlRowSet rowSet = pgsqlForRowSet(tableName, domainId, pageNumber, pageSize);
                    if (pageNumber == 0 && rowSet.isBeforeFirst()) {
                        SqlRowSetMetaData metaData = rowSet.getMetaData();
                        int columnCount = metaData.getColumnCount();
                        for (int i = 1; i <= columnCount; i++) {
                            columns.put(metaData.getColumnName(i), metaData.getColumnType(i));
                        }
                    }

                    //todo: use assistant thread to build & send response by fifo(RtdbSyncSender)
                    // 表内记录数量过大，容易造成sql查询效率低，所以错开查询和发送的线程，提高性能
                    // build the response
                    int sendCount = sendRespPayload(output, rowSet, columns, optType, tableName);
                    if (sendCount > 0) {
                        logger.debug("send pageNumber={}, sendCount={}", pageNumber, sendCount);
                    } else {
                        logger.debug("load table({}) done!!!", tableName);
                        break;
                    }
                    pageNumber++;
                } while (!client.isClosed());

            } else {
                // request error
                logger.error("illegal request length = {}", length);
                client.close();
            }
        } catch (IOException e) {
            logger.error("io socket err = {}", e.getMessage());
            closeClient(client);
        } catch (Exception e) {
            logger.error("sql err = {}", e.getMessage());
            closeClient(client);
        }
    }

    protected int sendRespPayload(DataOutputStream output, SqlRowSet rowSet, Map<String, Integer> columns, int optType, String tableName)
            throws Exception {
        int rowCount = 0;
        RtdbSyncResponse resp = new RtdbSyncResponse();

        resp.writeByte(optType);
        resp.writeSampleAscii(tableName);
        resp.writeByte(columns.size());
        for (String filed : columns.keySet()) {
            resp.writeSampleAscii(filed);
            resp.writeByte(columns.get(filed));
        }
        while (rowSet.next()) {
            for (String filed : columns.keySet()) {
                switch (columns.get(filed)) {
                    case Types.BIGINT://long
                        resp.writeLong(rowSet.getLong(filed));
                        break;
                    case Types.INTEGER:
                        resp.writeInt(rowSet.getInt(filed));
                        break;
                    case Types.SMALLINT://short
                        resp.writeShort(rowSet.getShort(filed));
                        break;
                    case Types.TINYINT://byte
                        resp.writeByte(rowSet.getByte(filed));
                        break;
                    case Types.CHAR:
                        String value = rowSet.getString(filed);
                        resp.writeByte((byte) (StringUtils.isEmpty(value) ? 0 : value.charAt(0)));
                        break;
                    case Types.FLOAT://float
                    case Types.REAL:
                        resp.writeFloat(rowSet.getFloat(filed));
                        break;
                    case Types.DOUBLE://double
                        resp.writeDouble(rowSet.getDouble(filed));
                        break;
                    case Types.TIMESTAMP:
                        Date date = rowSet.getDate(filed);
                        resp.writeLong(date != null ? date.getTime() : 0L);
                        break;
                    case Types.DATE:
                        Date d = rowSet.getDate(filed);
                        resp.writeLong(d!=null ? d.getTime() :0L);
                        break;
                    case Types.VARCHAR: //string/text
                        resp.writeUTF(rowSet.getString(filed));
                        break;
                    case Types.BIT://bool
                        resp.writeByte(rowSet.getBoolean(filed) ? 1 : 0);
                        break;
                    default:
                        logger.error("undefine sql type({},{})", filed, columns.get(filed));
                        throw new Exception(String.format("undefined sql type (%d,%d)", filed, columns.get(filed)));
                }
            }
            rowCount++;
        }


        int length = resp.size() + 8;
        output.writeInt(length);
        output.writeShort(rowCount);
        resp.writeTo(output);
        output.writeShort(0); //crc16
        output.flush();

        return rowCount;
    }

    protected SqlRowSet pgsqlForRowSet(String tableName, int domainId, int pageNumber, int pageSize) {
        if(domainId == -1) {//table without column domain_id
            String sql = String.format("SELECT * FROM %s OFFSET ? LIMIT ?", tableName);
            return jdbcTemplate.queryForRowSet(sql, pageNumber * pageSize, pageSize);
        } else {
            String sql = String.format("SELECT * FROM %s WHERE domain_id = ? OFFSET ? LIMIT ?", tableName);
            return jdbcTemplate.queryForRowSet(sql, domainId, pageNumber * pageSize, pageSize);
        }
    }

    protected String readAsciiChars(DataInputStream input) throws IOException {
        int length = input.readUnsignedByte();
        byte result[] = new byte[length];
        input.read(result);
        return new String(result, Charset.forName("ASCII"));//Arrays.toString(result);
    }

    protected void closeClient(Socket client) {
        try {
            client.close();
        } catch (IOException e) {
            logger.error(e.getMessage());
        } catch (Exception e) {
            logger.error(e.getMessage());
        }
    }
}
