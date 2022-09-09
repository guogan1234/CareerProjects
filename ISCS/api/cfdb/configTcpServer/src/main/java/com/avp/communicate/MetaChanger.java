package com.avp.communicate;

/**
 * Created by guo on 2018/4/16.
 */
public class MetaChanger {

    public static Integer charArrayToIntWithSmallEndian(char[] charArray){
        Integer result = 0;
        String total = "";
        for(int i = 3;i>=0;i--){
            String str = null;
            str = Integer.toHexString(charArray[i] & 0xff);
            if(str.length() < 2){
                str = "0" + str;
            }
            total += str;
        }
        System.out.println("total(int) - " + total);
        result = Integer.parseInt(total,16);
        return result;
    }

    public static Long charArrayToLongWithSmallEndian(char[] charArray){
        Long result = 0L;
        String total = "";
        for(int i = 7;i>=0;i--){
            String str = null;
            str = Integer.toHexString(charArray[i] & 0xff);//此方法只能将字符为0-9，a-z，A-Z的字符转为int值，其他字符全部返回0xfd
            if(str.length() < 2){
                str = "0" + str;
            }
            total += str;
        }
        System.out.println("total(long) - " + total);
        result = Long.parseLong(total,16);
        return result;
    }
}
