package com.avp.configService.service.rtdb;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Async;
import org.springframework.stereotype.Service;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Created by boris feng on 2018/6/1.
 */
@Service
public class RtdbSyncListener {
    private Logger logger = LoggerFactory.getLogger(RtdbSyncListener.class);

    @Value("${iscs.confdb.rtdb.port:9527}")
    private Integer port;

    @Value("${iscs.confdb.rtdb.sock.timeout:10000}")
    private Integer timeout;

    @Autowired
    private RtdbSyncExecutor rtdbSyncExecutor;

    private ServerSocket server;

    public boolean start() {
        logger.debug("RtdbSyncListener - start");

        try {
            //create assist thread
            Thread job = new Thread(new ListenJob(this), "ConfDb-Listener-" + port);
            job.setDaemon(true);
            job.start();
            logger.debug("listener task run, {}",job.getName());

            return true;
        } catch (Exception e) {
            logger.error("init other err:" + e);
        }

        return false;
    }

    public boolean stop() {
        try {
            if(server != null) {
                server.close();
            }
        } catch (IOException e) {
            e.printStackTrace();
            logger.error(e.getMessage());
            return false;
        }
        return true;
    }

    protected void listen() {
        try {
            server = new ServerSocket(port);
            logger.debug("setting server init ok port:{}", port);

            do {
                try {
                    Socket client = server.accept();
                    logger.debug("begin to connecting:{}", client.getRemoteSocketAddress().toString());
                    logger.debug("getLocalPort:{}", client.getLocalPort());
                    logger.debug("getPort:{}", client.getPort());
                    logger.debug("getInetAddress:{}", client.getInetAddress());
                    logger.debug("getLocalAddress:{}", client.getLocalAddress());
                    logger.debug("getLocalSocketAddress:{}", client.getLocalSocketAddress());

                    client.setSoTimeout(timeout);
                    rtdbSyncExecutor.execSync(client);
                } catch (Exception e) {
                    logger.error(e.getMessage());
                }
            } while (!server.isClosed());
        } catch (IOException e) {
            e.printStackTrace();
            logger.error(e.getMessage());
        }
    }

    class ListenJob implements Runnable {
        private RtdbSyncListener handler;

        @Override
        public void run() {
            if (handler != null) {
                handler.listen();
            }
        }

        public ListenJob(RtdbSyncListener handler) {
            this.handler = handler;
        }
    }
}

