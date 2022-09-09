package com.avp.communicate;

import com.avp.entity.Device;
import com.avp.service.DeviceService;

import java.io.*;
import java.net.Socket;
import java.util.List;

/**
 * Created by guo on 2018/4/12.
 */
public class TaskThread extends Thread{
    private String name;
    private Socket socket;

    TaskThread(String name, Socket socket){
        this.name = name;
        this.socket = socket;
    }

    public void run(){
        System.out.println("[client] " + name);

//        test_base();
//        test();
        doTask();
    }

    private List<Device> readAndGetList(){
        List<Device> list = null;

        try {
            //获取输入流(字节流)，并读取客户端信息
            InputStream inp = socket.getInputStream();
            //把字节流转换成字符流
            InputStreamReader isr = new InputStreamReader(inp);
            //为字符流增加缓冲区
            BufferedReader bfr = new BufferedReader(isr);
            //
            char[] msg_head = new char[2];
            bfr.read(msg_head);
            if (msg_head[0] == 0x7e && msg_head[1] == 0x7e) {
                System.out.println("all catch!!!");
            }
            char[] msg_time = new char[8];
            bfr.read(msg_time);
            Long time_secs = MetaChanger.charArrayToLongWithSmallEndian(msg_time);
            System.out.println("msg_time: " + time_secs);
            char[] msg_table_len = new char[4];
            bfr.read(msg_table_len);
            Integer t_len = MetaChanger.charArrayToIntWithSmallEndian(msg_table_len);
            System.out.println("t_len: " + t_len);
            char[] msg_table_name = new char[20];
            bfr.read(msg_table_name);
            System.out.println("table_name: " + msg_table_name);
            char[] msg_total_len = new char[4];
            bfr.read(msg_total_len);
            Integer total_len = MetaChanger.charArrayToIntWithSmallEndian(msg_total_len);
            System.out.println("total_len: " + total_len);
        }catch (Exception e){
            e.printStackTrace();
        }
        return list;
    }

    private int writeMsgHead(PrintWriter pw){
        System.out.println("writeMsgHead...");

        char[] start = {0x7e,0x7e};
        pw.print(start);
        long time = 1L;
        pw.print(time);//Error - only send one char
//        int operate_id = 1;
//        pw.print(operate_id);
//        char[] tableName = new char[20];
//        tableName = "tableName".toCharArray();
//        pw.print(tableName);
//        int total = 1;
//        pw.print(total);

        pw.flush();
        return -1;
    }

    private int writeMessageHead(DataOutputStream dos){
        System.out.println("writeMessageHead...");

        try {
            byte[] start = {0x7e, 0x7e};
            dos.write(start);
            long time = 1;
            dos.writeLong(time);
            int operateId = 1;
            dos.writeInt(operateId);
//            byte[] tableName = new byte[20];
//            tableName = "tableName".getBytes();
//            dos.write(tableName);
            String tableName = "tableName";
            int tableNameLen = tableName.length();
            dos.writeInt(tableNameLen);
            dos.writeBytes(tableName);
            int total = 1;
            dos.writeInt(total);

            dos.flush();
        }catch (Exception e){

        }
        return -1;
    }

    private int writeMsgBody(DataOutputStream pw,List<Device> list){
        try {
            for (int i = 0; i < 1; i++) {
                byte bNext = 0x01;
                if (i == 0) {
                    bNext = 0x00;
                }
                pw.write(bNext);
                int objectCount = 1;
                pw.writeInt(objectCount);
                int objectSum = 0;
                String label = list.get(i).getCtrl_trans_grp_label();
                int label_len = label.length();
                int type = 1;
                pw.writeInt(type);
                objectSum += 4;
                pw.writeInt(label_len);
                objectSum += 4;
                pw.writeBytes(label);
                objectSum += label_len;

                int status = list.get(i).getDev_status();
                int type_2 = 2;
                pw.writeInt(type_2);
                objectSum += 4;
                pw.writeInt(status);
                objectSum += 4;
            }
            pw.flush();
        }catch (Exception e){
            e.printStackTrace();
        }
        return -1;
    }

    private int writeMsgTail(DataOutputStream pw){
        System.out.println("writeMsgTail...");

        return -1;
    }

    private void doTask(){
        try{
//            List<Device> list = readAndGetList();
            List<Device> list = DeviceService.getDevice();
            OutputStream os = socket.getOutputStream();
//            PrintWriter pw = new PrintWriter(os);//Error - write long only send one char
            DataOutputStream pw = new DataOutputStream(os);

//            writeMsgHead(pw);
//            writeMessageHead(pw);
            writeMsgBody(pw,list);
//            writeMsgTail(pw);
        }catch (Exception e){
            e.printStackTrace();
        }
    }

    private void test_base(){
        Integer i = Integer.parseInt("07000000",16);
        System.out.println(i);
        Integer i_2 = Integer.parseInt("00000007",16);
        System.out.println(i_2);
        Integer i_3 = Integer.parseInt("01000107",16);
        System.out.println(i_3);
    }

    private void test(){
        try {
            //获取输入流(字节流)，并读取客户端信息
            InputStream inp = socket.getInputStream();
            //把字节流转换成字符流
            InputStreamReader isr = new InputStreamReader(inp);
            //为字符流增加缓冲区
            BufferedReader bfr = new BufferedReader(isr);
            //3.
            char[] msg_head = new char[2];
            bfr.read(msg_head);
            if(msg_head[0] == 0x7e && msg_head[1] == 0x7e){
                System.out.println("all catch!!!");
            }
            char[] msg_time = new char[8];
            bfr.read(msg_time);
            Long time_secs = MetaChanger.charArrayToLongWithSmallEndian(msg_time);
            System.out.println("msg_time: " + time_secs);
            char[] msg_table_len = new char[4];
            bfr.read(msg_table_len);
            System.out.println("msg_table_len: " + msg_table_len);
            System.out.println(msg_table_len);
            for(int i = 0;i<4;i++){
                System.out.println("char: ");
                System.out.println(msg_table_len[i]);
                System.out.println(Integer.toHexString(msg_table_len[i]));
            }

//            int len = (int)msg_table_len;//Error:compile failed
//            int table_len = Integer.parseInt(String.valueOf(msg_table_len));
            Integer t_len = MetaChanger.charArrayToIntWithSmallEndian(msg_table_len);
            System.out.println("t_len: " + t_len);
//            int table_len = Integer.parseInt(new String(msg_table_len));//Error:
//            System.out.println("table_len: " + table_len);
            char[] msg_table_name = new char[20];
            bfr.read(msg_table_name);
            System.out.println("table_name: " + msg_table_name);
            char[] msg_total_len = new char[4];
            bfr.read(msg_total_len);
            Integer total_len = MetaChanger.charArrayToIntWithSmallEndian(msg_total_len);
//            int total_len = Integer.parseInt(String.valueOf(msg_total_len));//Error
            System.out.println("total_len: " + total_len);

//            //2.字符流读取数据
//            for(int i = 0;i<100;i++){
//                char[] buf = new char[1];
//                bfr.read(buf);
//                System.out.println("1#" + (buf[0] & 0xffff));
//                System.out.println("2#" + Integer.toHexString(buf[0]));
//                System.out.println("3#" + Integer.toHexString(buf[0] & 0xffff));
//                System.out.println("3#" + Integer.toHexString(buf[0] & 0xff));
//                if(buf[0] == 0x7e){
//                    System.out.println("catch it!");
//                }
//            }
//            String info = null;
//            while ((info = bfr.readLine()) != null) {//循环读取数据
//                System.out.println(name + "：" + info);
//            }

//            //1.字节流读取数据
//            BufferedInputStream binp = new BufferedInputStream(inp);
//            for(int i = 0;i<100;i++){
//                byte[] buf = new byte[1];
//                binp.read(buf);
//                System.out.println("# " + Integer.toHexString(buf[0]));
//            }
            socket.shutdownInput();//关闭输入流

            System.out.println("[client] " + name + " [Begin closed!]");
            //关闭资源
//            bfr.close();
//            isr.close();
            inp.close();
            socket.close();
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }
}
