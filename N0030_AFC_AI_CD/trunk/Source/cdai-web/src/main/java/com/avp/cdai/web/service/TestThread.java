package com.avp.cdai.web.service;

/**
 * Created by guo on 2017/9/12.
 */
public class TestThread extends Thread {
    public void run(){
        System.out.println("run...");
    }

    public void run2(){
        while(true){
            System.out.println("run2...");
        }
    }
}
