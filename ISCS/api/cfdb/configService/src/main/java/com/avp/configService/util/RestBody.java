/**
 * Copyright (c) 2017 Avant-Port All Rights Reserved.
 */
package com.avp.configService.util;

import java.util.Date;

/**
 * Created by boris feng on 2017/5/27.
 */
public class RestBody<T> {
    private Integer code = -1;
    private String message = "unknown";
    private Long timestamp = new Date().getTime();

    public  RestBody(ResponseCode code) {
        this.code = code.value();
        this.message = code.phrase();
    }


    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Integer getCode() {
        return this.code;
    }

    public void setCode(Integer code) {
        this.code = code;
    }

    public Long getTimestamp() {
        return this.timestamp;
    }
}