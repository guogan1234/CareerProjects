package com.avp.configService.service.rtdb;

import org.springframework.util.StringUtils;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.charset.Charset;
import java.util.Date;

/**
 * Created by boris feng on 2018/6/1.
 */
public class RtdbSyncResponse {

    //payload
    private ByteArrayOutputStream payload = new ByteArrayOutputStream();
    private DataOutputStream output = new DataOutputStream(payload);

    public void writeByte(int value) {
        try {
            output.writeByte(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeShort(int value) {
        try {
            output.writeShort(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeInt(int value) {
        try {
            output.writeInt(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeLong(long value) {
        try {
            output.writeLong(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //length use 1byte
    public void writeSampleAscii(String value) {
        try {
            if(StringUtils.isEmpty(value)) {
                output.writeByte(0);
            } else {
                output.writeByte(value.length());
                output.writeBytes(value);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    //length use 4byte
    public void writeUTF(String value) {
        try {
            if(StringUtils.isEmpty(value)) {
                output.writeInt(0);
            } else {
                byte[] buffer = value.getBytes(Charset.forName("UTF8"));
                output.writeInt(buffer.length);
                output.write(buffer, 0, buffer.length);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeFloat(float value) {
        try {
            output.writeFloat(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeDouble(double value) {
        try {
            output.writeDouble(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void writeBoolean(boolean value) {
        try {
            output.writeBoolean(value);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public int size() {
        return output.size();
    }

    public void writeTo(OutputStream stream) throws IOException {
        payload.writeTo(stream);
    }
}
