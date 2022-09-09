package com.avp.configService.communicate;

/**
 * Created by guo on 2018/4/16.
 */
@Deprecated
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

    private static int CharArrayToIntWithSmallEndian(char[] charArray){
        int result = 0;
        int v0,v1,v2,v3;
//        v0 = (charArray[3]&0x000000ff)<<24;
//        v1 = (charArray[2]&0x000000ff)<<16;
//        v2 = (charArray[1]&0x000000ff)<<8;
//        v3 = charArray[0]&0x000000ff;
        int src0,src1,src2,src3;
        src0 = charArray[3];
        src1 = charArray[2];
        src2 = charArray[1];
        src3 = charArray[0];
        v0 = src0<<24;
        v1 = src1<<16;
        v2 = src2<<8;
        v3 = src3;
        System.out.println("char: " + v0 + " " + v1 + " " + v2 + " " + v3);
        result = v0 + v1 + v2 + v3;
        return result;
    }

    public static Integer charArrayToIntWithEndian(char[] charArray){//error
//        return charArrayToIntWithSmallEndian(charArray);
        return CharArrayToIntWithSmallEndian(charArray);
    }

    private static int ByteArrayToIntWithSmallEndian(byte[] array){
        int result = 0;
        int v0,v1,v2,v3;
        v0 = (array[3]&0x000000ff)<<24;
        v1 = (array[2]&0x000000ff)<<16;
        v2 = (array[1]&0x000000ff)<<8;
        v3 = array[0]&0x000000ff;
        result = v0 + v1 + v2 + v3;
        System.out.println("byte: " + v0 + " " + v1 + " " + v2 + " " + v3);
        return result;
    }

    public static int byteArrayToIntWithEndian(byte[] byteArray){
        return ByteArrayToIntWithSmallEndian(byteArray);
    }

    public static Long charArrayToLongWithSmallEndian(char[] charArray){
        Long result = 0L;
        String total = "";
        for(int i = 7;i>=0;i--){
            String str = null;
            str = Integer.toHexString(charArray[i] & 0xff);
            if(str.length() < 2){
                str = "0" + str;
            }
            total += str;
        }
        System.out.println("total(long) - " + total);
        result = Long.parseLong(total,16);
        return result;
    }

    private static long CharArrayToLongWithSmallEndian(char[] charArray){
        long result = 0;
        long v0,v1,v2,v3,v4,v5,v6,v7;
        v0 = (charArray[7]&0xff)<<56;
        v1 = (charArray[6]&0xff)<<48;
        v2 = (charArray[5]&0xff)<<40;
        v3 = (charArray[4]&0xff)<<32;
        v4 = (charArray[3]&0xff)<<24;
        v5 = (charArray[2]&0xff)<<16;
        v6 = (charArray[1]&0xff)<<8;
        v7 = (charArray[0]&0xff);
        result = v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7;
        return result;
    }

    public static Long charArrayToLongWithEndian(char[] charArray){//error
//        return charArrayToLongWithSmallEndian(charArray);
        return CharArrayToLongWithSmallEndian(charArray);
    }

    private static long ByteArrayToLongWithSmallEndian(byte[] array){
        long result = 0;
        long v0,v1,v2,v3,v4,v5,v6,v7;
        v0 = (array[7]&0xff)<<56;
        v1 = (array[6]&0xff)<<48;
        v2 = (array[5]&0xff)<<40;
        v3 = (array[4]&0xff)<<32;
        v4 = (array[3]&0xff)<<24;
        v5 = (array[2]&0xff)<<16;
        v6 = (array[1]&0xff)<<8;
        v7 = (array[0]&0xff);
        result = v0 + v1 + v2 + v3 + v4 + v5 + v6 + v7;
        return result;
    }

    public static long byteArrayToLongWithEndian(byte[] byteArray){
        return 0;
    }
}
