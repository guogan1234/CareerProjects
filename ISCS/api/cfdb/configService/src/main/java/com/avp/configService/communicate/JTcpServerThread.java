package com.avp.configService.communicate;

import org.springframework.context.ApplicationContext;

import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by guo on 2018/4/12.
 */
@Deprecated
public class JTcpServerThread extends Thread{
    private int PORT = 10000;
    private Integer clientCount = 0;
    private  ApplicationContext applicationContext;

    public  JTcpServerThread(ApplicationContext applicationContext){
        this.applicationContext=applicationContext;
    }

    public void run(){
        try{
            ServerSocket serverSocket = new ServerSocket(PORT);
            System.out.println("server is listening..." + PORT);

            while (!serverSocket.isClosed()){
                Socket socket = serverSocket.accept();
                clientCount++;

                String clientName = "t_" + clientCount;
                TaskThread client = new TaskThread(clientName,socket,applicationContext);
                client.start();
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
