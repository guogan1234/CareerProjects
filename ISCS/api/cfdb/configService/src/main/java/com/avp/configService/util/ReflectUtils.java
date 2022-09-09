package com.avp.configService.util;

import com.avp.configService.communicate.TransType;

import java.lang.reflect.Field;
import java.sql.Timestamp;
import java.util.*;

public class ReflectUtils {

    public static int getType(String fieldName,Map<String,Integer> map){
        int type=-1;
        if(map.containsKey(fieldName)){
            type=map.get(fieldName);
        }else{
            System.out.println(fieldName);
        }
        return type;
    }

    public static  List<String> getHeaders(Map<String,Integer> map){
            List<String> list=new ArrayList<>();
            Iterator iterator=map.keySet().iterator();
            while (iterator.hasNext()){
                Object key=iterator.next();
                list.add(String.valueOf(key));
            }
            return list;
    }

    public static  List<String> getColumns(Map<String,Object> map){
        List<String> list=new ArrayList<>();
        Iterator iterator=map.keySet().iterator();
        while (iterator.hasNext()){
            Object key=iterator.next();
            list.add(String.valueOf(key));
        }
        return list;
    }



    public static Map<String,Integer> getEntityFieldType(String entityName){
            Map<String,Integer> map=new HashMap<>();
        try {
            Class c= Class.forName("com.avp.configService.entity."+entityName);
            Field[] fields = c.getDeclaredFields();
            if(fields.length>0){
                for(int i=0;i<fields.length;i++){
                    Class classes=fields[i].getType();
                    String fieldName=fields[i].getName();
                    StringBuilder stringBuilder=new StringBuilder();
                    for(int j=0;j<fieldName.length();j++){
                        char c1 = fieldName.charAt(j);
                       if(Character.isUpperCase(c1)){
                         stringBuilder.append("_"+String.valueOf(c1).toLowerCase());
                       }else{
                           stringBuilder.append(c1);
                       }
                    }
                    map.put(stringBuilder.toString(),getType(classes));
                }
            }
        } catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return map;
    }

    public static int getType(Class c){
        int type=0;
        if(c==String.class){
            type=TransType.TRANS_STRING;
        }else if(c==Integer.class){
            type=TransType.TRANS_INT;
        }else if(c==Long.class){
            type=TransType.TRANS_LONG;
        }else if(c==Float.class){
            type=TransType.TRANS_FLOAT;
        }else if(c==Double.class){
            type=TransType.TRANS_DOUBLE;
        }else if(c==Short.class){
            type=TransType.TRANS_SHORT;
        }else if(c==Boolean.class){
            type=TransType.TRANS_BOOL;
        }else if(c== Timestamp.class){
            type=TransType.TRANS_TIMESTAMP;
        }
        return type;
    }

    public static  void  main(String args[]){
        //ReflectUtils.getHeaders(ReflectUtils.getEntityFieldType("DevTbl"));
    }



}
