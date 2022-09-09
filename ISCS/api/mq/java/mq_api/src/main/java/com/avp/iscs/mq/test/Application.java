package com.avp.iscs.mq.test;

import com.avp.iscs.mq.client.*;

//@Configuration
public class Application {
    public static void main(String[] args) {
    	try{
        RedisMQ.initialize("");
        
        String squeue = "queue://domain:test_java_queue_send_receive";
        String queueMsg = "test_java_queue_send_receive: one to one message";
        if(args.length == 0){
	        //  Queue Message Send / Receive
	        System.out.println("test_java_queue_send_receive...");
	        
	        int iloopcount = 100;
	    	String recvmsg = "";
	        for(int i = 0; i < iloopcount; i++) {
	        	RedisMQ.send("", squeue, queueMsg + " #" + i, 5);
	        	System.out.println("Sent Message: " + queueMsg + " #" + i);
	    		recvmsg = RedisMQ.receive("", squeue, 5);
	        	System.out.println("Received Message: " + " #" + i + ": " + recvmsg);
	        }
        }
        else if( (args.length == 1) && (args[0].compareToIgnoreCase("send") ==0) ){
	        int iloopcount = 40000;

	        System.out.println("test_java_queue_send...");
	    	String recvmsg = "";
	        for(int i = 0; i < iloopcount; i++) {
	        	RedisMQ.send("", squeue, queueMsg + " #" + i, 5);
	        	System.out.println("Sent Message: " + queueMsg + " #" + i);
	        }
        }
        else if( (args.length == 1) && (args[0].compareToIgnoreCase("recv") ==0) ){
	        int iloopcount = 40000;

	        System.out.println("test_java_queue_receive...");
	    	String recvmsg = "";
	        for(int i = 0; i < iloopcount; i++) {
	        	recvmsg = RedisMQ.receive("", squeue, 5);
	        	System.out.println("Received Message: " + " #" + i + ": " + recvmsg);
	        }
        }
        
        RedisMQ.release();
    	}catch(Exception ex){
    		System.out.println("Exception catched: " + ex.getMessage());
    	}
    }
}

