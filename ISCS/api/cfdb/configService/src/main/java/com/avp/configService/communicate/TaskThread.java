package com.avp.configService.communicate;

import com.avp.configService.entity.vo.CellVo;
import com.avp.configService.entity.vo.ZyEntity;
import com.avp.configService.service.UniversalService;
import com.avp.configService.util.SpringUtil;
import org.springframework.context.ApplicationContext;

import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.util.List;
import java.nio.charset.Charset;

/**
 * Created by guo on 2018/4/12.
 */
@Deprecated
public class TaskThread extends Thread{
    private String name;
    private Socket socket;

    TaskThread(String name, Socket socket,ApplicationContext applicationContext){
        this.name = name;
        this.socket = socket;
        SpringUtil.setApplicationContext(applicationContext);
    }

    public void run(){
        System.out.println("[client] " + name);

//        test_base();
//        test();
        doTask();
    }

    private int writeMsgHead(PrintWriter pw){
        System.out.println("writeMsgHead...");

        char[] start = {0x7e,0x7e};
        pw.print(start);
        long time = 1L;
        pw.print(time);//Error - only send one char

        pw.flush();
        return -1;
    }

    private int recvMsgHead(DataInputStream bfr,GetDataParam param){
        try {
            int ret = 0;

//            char[] msg_head = new char[2];
            byte[] msg_head = new byte[2];
            while (true) {
                ret = bfr.read(msg_head);
                if(ret == 2){
                    break;
                }
            }
            if (msg_head[0] == 0x7e && msg_head[1] == 0x7e) {
                System.out.println("all catch!!!");
            }

            byte[] msg_time = new byte[8];
            bfr.read(msg_time);
            Long time_secs = MetaChanger.byteArrayToLongWithEndian(msg_time);
            System.out.println("msg_time: " + time_secs);

            byte[] domainId = new byte[4];
            bfr.read(domainId);
            Integer domain = MetaChanger.byteArrayToIntWithEndian(domainId);
            System.out.println("domain: " + domain);

            byte[] operateId = new byte[4];
            bfr.read(operateId);
            Integer operate = MetaChanger.byteArrayToIntWithEndian(operateId);
            System.out.println("operate: " + operate);

            byte[] msg_table_len = new byte[4];
            bfr.read(msg_table_len);
            Integer t_len = MetaChanger.byteArrayToIntWithEndian(msg_table_len);
            System.out.println("t_len: " + t_len);

            if(t_len <= 0){
                return -1;
            }
            byte[] msg_table_name = new byte[t_len];
            bfr.read(msg_table_name);
//            String tableName = String.valueOf(msg_table_name);
            String tableName = new String(msg_table_name);
            System.out.println("table_name: " + msg_table_name);
            System.out.println("tableName: " + tableName);

            byte[] msg_total_len = new byte[4];
            bfr.read(msg_total_len);
            Integer total_len = MetaChanger.byteArrayToIntWithEndian(msg_total_len);
            System.out.println("total_len: " + total_len);

            //
            param.setTimestamp(time_secs);
            param.setDomainId(domain);
            param.setOperateId(operate);
            param.setTableNameLen(t_len);
            param.setTableName(tableName);

            System.out.println("Param:time_secs " + time_secs);
            System.out.println("Param:domain " + domain);
            System.out.println("Param:operate " + operate);
            System.out.println("Param:t_len " + t_len);
            System.out.println("Param:tableName " + tableName);
        }catch (Exception e){
            e.printStackTrace();
        }
        return 0;
    }

    private int recvMsgBody(DataInputStream bfr,GetDataParam param){
        try {
            byte[] msg_page = new byte[4];
            bfr.read(msg_page);
            Integer page = MetaChanger.byteArrayToIntWithEndian(msg_page);
            System.out.println("page: " + page);

            byte[] msg_page_size = new byte[4];
            bfr.read(msg_page_size);
            Integer pageSize = MetaChanger.byteArrayToIntWithEndian(msg_page_size);
            System.out.println("pageSize: " + pageSize);

            param.setPage(page);
            param.setPageSize(pageSize);
            System.out.println("Param:page " + page);
            System.out.println("Param:pageSize " + pageSize);
        }catch (Exception e){
            e.printStackTrace();
        }
        return 0;
    }

    private int recvMsgTail(DataInputStream bfr){
        try {
            byte[] crc = new byte[2];
            bfr.read(crc);
            if(crc[0] == 0x7e && crc[1] == 0x7e){
                System.out.println("Recv:crc all catched!");
            }

            byte[] end = new byte[2];
            bfr.read(end);
            if(end[0] == 0x0d && end[1] == 0x0a){
                System.out.println("Recv:ends all catched!");
            }
        }catch (Exception e){
            e.printStackTrace();
        }
        return 0;
    }

    private void testRecvChars(BufferedReader bfr){
        try {
            char[] buf = new char[100];
            bfr.read(buf);
            for(int i = 0;i<100;i++){
                System.out.println("# - " + Integer.toHexString(buf[i] & 0xff));
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    private GetDataParam recvPieceMsg(DataInputStream bfr){
//        testRecvChars(bfr);
        GetDataParam dataParam = new GetDataParam();
        dataParam.setObjectNum(1);
        recvMsgHead(bfr,dataParam);
        recvMsgBody(bfr,dataParam);
        recvMsgTail(bfr);
        return dataParam;
    }

    private int writeMessageHead(DataOutputStream dos,GetDataParam param){
        try {
            byte[] start = {0x7e, 0x7e};
            dos.write(start);
            long time = param.getTimestamp();
            dos.writeLong(time);
            int domain = param.getDomainId();
            dos.writeInt(domain);
            int operateId = param.getOperateId();
            dos.writeInt(operateId);
            String tableName = param.getTableName();
            int tableNameLen = tableName.length();
            dos.writeInt(tableNameLen);
            dos.writeBytes(tableName);
            //total is send in body

            dos.flush();

            System.out.println("Write head: - time " + time);
            System.out.println("Write head: - domain " + domain);
            System.out.println("Write head: - operateId " + operateId);
            System.out.println("Write head: - tableNameLen " + tableNameLen);
            System.out.println("Write head: - tableName " + tableName);
        }catch (Exception e){
            e.printStackTrace();
        }
        return -1;
    }

    private int writeCellSize(CellVo cellVo){
        int len = 0;
        try {
            int type = cellVo.getType();
            len += 4;
            System.out.println("writeCellSize:type - " + cellVo.getType());
            System.out.println("writeCellSize:name - " + cellVo.getName());
            switch (type) {
                case 0://String
                {
                    len += 4;
                    String value = (String) cellVo.getValue();
                    if (value == null) {
                        value = " ";
                        byte[] bytes = value.getBytes("UTF8");
                        int bytesLen = bytes.length;
                        len += bytesLen;
                    } else {
                        byte[] bytes = value.getBytes("UTF8");
                        int bytesLen = bytes.length;
                        len += bytesLen;
                    }
                }
                break;
                case 1://bool
                    len += 1;
                    break;
                case 2://short
                    len += 2;
                    break;
                case 3://int
                    len += 4;
                    break;
                case 4://long
                    len += 8;
                    break;
                case 5://float
                    len += 4;
                    break;
                case 6://double
                    len += 8;
                    break;
                case 7:
                    break;
                default: {
                    System.out.println("TransType not matched!");
                }
            }
            //写入表字段名称
            String name = cellVo.getName();
            len += 4;
            len += name.length();
        }
        catch (Exception e){
            e.printStackTrace();
        }
        return len;
    }

    private int writeMsgListObject(DataOutputStream os,Object obj,GetDataParam param){
        try {
            if(obj == null){//
                int total = 0;
                os.writeInt(total);
                byte[] exitFlag = new byte[1];
                exitFlag[0] = 0x01;
                os.write(exitFlag);
                byte[] nextFlag = new byte[1];
                nextFlag[0] = 0x00;
                os.write(nextFlag);
                int objectNum = 0;
                os.writeInt(objectNum);
                int objectSize = 0;
                os.writeInt(objectSize);

                return 0;
            }

            int total = 0;
            List listObj = (List) obj;
            byte[] exitFlag = new byte[1];
            exitFlag[0] = 0x00;
            boolean bExit = param.isExitFlag();
            if(bExit){
                exitFlag[0] = 0x01;
            }
            total += 1;

            byte[] nextFlag = new byte[1];
            boolean bNext = param.isNextFlag();
            if (bNext) {//下一片标志。true：还有下一片对象数据包
                nextFlag[0] = 0x01;
            } else {
                nextFlag[0] = 0x00;
            }
            total += 1;

            int objectNum = param.getObjectNum();
            total += 4;

            int objectSize = 0;
            total += 4;

            String tableName = param.getTableName();
            int listSize = listObj.size();
            for(int i = 0;i<listSize;i++){
                CellVo cellVo = (CellVo) listObj.get(i);
                int len = writeCellSize(cellVo);
                objectSize += len;
                objectSize += 1;//property next flag
            }
            total += objectNum * objectSize;
            total += 4;//tail
            os.writeInt(total);
            os.write(exitFlag);
            os.write(nextFlag);
            os.writeInt(objectNum);
            os.writeInt(objectSize);
            System.out.println("Write body:total - " + total);
            System.out.println("Write body:objectNum - " + objectNum);
            System.out.println("Write body:objectSize - " + objectSize);

            for(int i = 0;i<listSize;i++){
                CellVo cellVo = (CellVo) listObj.get(i);
                int type = cellVo.getType();
                os.writeInt(type);
                System.out.println(i + " Write body:type - " + type);
                System.out.println(i + " Write body:name - " + cellVo.getName());
                switch (type){
                    case 0://string
                    {
                        String value = " ";
                        int strLen = 1;
                        Object object = cellVo.getValue();
                        if(object != null){
                            value = (String)object;
                            strLen = value.length();
                        }
						byte[] buffer = value.getBytes(Charset.forName("UTF8"));
                        os.writeInt(buffer.length);
                        os.write(buffer, 0, buffer.length);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:string len - " + strLen);
                        System.out.println(i + " Write body:string value - " + value);
                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    case 1://bool
                    {
                        byte[] b = new byte[1];
                        b[0] = 0x00;
                        Object object = cellVo.getValue();
                        if(object != null){
                            boolean value = (boolean)object;
                            if(value){
                                b[0] = 0x01;
                            }
                        }
                        os.write(b);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    case 2://short
                    {
                        short value = 0;
                        Object object = cellVo.getValue();
                        if(object != null){
                            value = (short)object;
                        }
                        os.writeShort(value);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:value - " + value);
                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    case 3://int
                    {
                        int value = 0;
                        Object object = cellVo.getValue();
                        if(object != null){
                            value = (int)object;
                        }
                        os.writeInt(value);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:value - " + value);
                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    case 4://long
                    {
                        long value = 0;
                        Object object = cellVo.getValue();
                        if(object != null){
                            System.out.println(object.getClass()==BigInteger.class);
                            if(object.getClass()==BigInteger.class){
                                BigInteger bigInteger=(BigInteger)object;
                                value=bigInteger.longValue();
                            }else{
                                value = (long) object;
                            }
                        }
                        os.writeLong(value);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:value - " + value);
                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    case 5://float
                    {
                        float value = 0;
                        Object object = cellVo.getValue();
                        if(object != null){
                            value = (float) object;
                        }
                        os.writeFloat(value);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:value - " + value);
                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    case 6://double
                    {
                        double value = 0;
                        Object object = cellVo.getValue();
                        if(object != null){
                            value = (double)object;
                        }
                        os.writeDouble(value);

                        String fieldName = cellVo.getName();
                        int fieldLen = fieldName.length();
                        os.writeInt(fieldLen);
                        os.writeBytes(fieldName);

                        System.out.println(i + " Write body:value - " + value);
                        System.out.println(i + " Write body:field len - " + fieldLen);
                        System.out.println(i + " Write body:field name - " + fieldName);
                    }
                        break;
                    default:
                    {
                        System.out.println("CellVo type not matched!");
                    }
                }

                byte[] bPropertyNext = new byte[1];
                bPropertyNext[0] = 0x01;
                System.out.println(i + " write body:bPropertyNext 0x01.");
                if(i == listSize -1){//last property
                    bPropertyNext[0] = 0x00;
                    System.out.println(i + " write body:bPropertyNext 0x00.");
                }
                os.write(bPropertyNext);
            }

//            long userId = user.getId();
//            objectSize += 8;
//            int type_1 = TransType.TRANS_LOOG;
//            objectSize += 4;
//
//            String name = user.getName();
//            objectSize += name.length();
//            int type_2 = TransType.TRANS_STRING;
//            objectSize += 4;
//            int strLen_2 = name.length();
//            objectSize += 4;
//
//            int age = user.getAge();
//            objectSize += 4;
//            int type_3 = TransType.TRANS_INT;
//            objectSize += 4;

//            os.writeInt(type_1);
//            os.writeLong(userId);
//
//            os.writeInt(type_2);
//            os.writeInt(strLen_2);
//            os.writeBytes(name);
//
//            os.writeInt(type_3);
//            os.writeInt(age);

            os.flush();
        }catch (Exception e){
            e.printStackTrace();
        }
        return 0;
    }

    private int writeMessageBody(DataOutputStream os,Object obj,GetDataParam param){
        try {
            writeMsgListObject(os,obj,param);
        }catch (Exception e){
            e.printStackTrace();
            return 0;
        }
        return 0;
    }

    private int writeMessageTail(DataOutputStream os){
        try{
            byte[] crc = new byte[2];
            crc[0] = 0x7e;
            crc[1] = 0x7e;
            os.write(crc);

            byte[] end = new byte[2];
            end[0] = 0x0d;
            end[1] = 0x0a;
            os.write(end);

            os.flush();
            System.out.println("Write tail.");
        }catch (Exception e){
            e.printStackTrace();
        }
        return 0;
    }

    private int writePieceData(DataOutputStream os,Object obj,GetDataParam param){
        writeMessageHead(os,param);
        writeMessageBody(os,obj,param);
        writeMessageTail(os);
        return 0;
    }

    private int writeListDatas(DataOutputStream os,List list,GetDataParam param,boolean bExitFlag){
        System.out.println("list size - " + list.size());
        if(list.size() == 0){
            param.setNextFlag(false);
            param.setExitFlag(false);
            writePieceData(os,null,param);
        }
        for(int i = 0;i < list.size();i++){
            System.out.println("list.." + i);
            if(i == list.size() -1){//一页的最后一条记录
                param.setNextFlag(false);
                if(!bExitFlag){
                    param.setExitFlag(true);
                }
            }else {
                param.setNextFlag(true);
            }
            Object obj = list.get(i);
            writePieceData(os,obj,param);
        }
        return 0;
    }

    private void doTask(){
        try{
            //获取输入流(字节流)，并读取客户端信息
            InputStream inp = socket.getInputStream();
//            //把字节流转换成字符流
//            InputStreamReader isr = new InputStreamReader(inp);
//            //为字符流增加缓冲区
//            BufferedReader bfr = new BufferedReader(isr);
            DataInputStream bfr = new DataInputStream(inp);

            OutputStream os = socket.getOutputStream();
            DataOutputStream pw = new DataOutputStream(os);

            GetDataController controller = new GetDataController();
            boolean runFlag = true;
            while (runFlag){
                GetDataParam dataParam = null;
                dataParam = recvPieceMsg(bfr);//接收一个操作命令

                controller.setPage(dataParam.getPage());
                controller.setPageSize(dataParam.getPageSize());

                List list = null;
                UniversalService universalService = SpringUtil.getBean(UniversalService.class);
                System.out.println("getPage - " + dataParam.getPage());
                System.out.println("getPage(size) - " + dataParam.getPageSize());
                ZyEntity zyEntity = null;
                zyEntity = universalService.getPage(dataParam.getTableName(),dataParam.getDomainId(),dataParam.getPage(),dataParam.getPageSize());
                list = zyEntity.getLists();

                dataParam.setExitFlag(false);//7fef72dc08d0
                if(list == null || (list.size() == 0)){
                    runFlag = false;
                }
                else {
                    if (zyEntity.isHasNext() == false) {
                        //dataParam.setExitFlag(true);
                        runFlag = false;
                    }
                }

                writeListDatas(pw,list,dataParam,runFlag);
            }
        }catch (Exception e){
            e.printStackTrace();
            return;
        }
    }
}
