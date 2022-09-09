package com.avp;

import com.avp.communicate.JTcpServerThread;

/**
 * Hello world!
 *
 */
public class App
{
    public static void main( String[] args )
    {
        System.out.println( "Hello World!" );
        JTcpServerThread serverThread = new JTcpServerThread();
        serverThread.start();

        System.out.println("springBoot App start...");
    }
}
