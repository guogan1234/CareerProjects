package com.avp.communicate;

import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by guo on 2018/4/12.
 */
public class JTcpServerThread extends Thread{
    private int PORT = 10000;
    private Integer clientCount = 0;

    public void run(){
        try{
            ServerSocket serverSocket = new ServerSocket(PORT);

            while (!serverSocket.isClosed()){
                Socket socket = serverSocket.accept();
                clientCount++;

                String clientName = "t_" + clientCount;
                TaskThread client = new TaskThread(clientName,socket);
                client.start();
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}
