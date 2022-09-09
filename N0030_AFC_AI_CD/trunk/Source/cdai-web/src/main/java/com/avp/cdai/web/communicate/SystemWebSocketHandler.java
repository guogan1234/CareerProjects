//package com.avp.cdai.web.communicate;
//
//import com.fasterxml.jackson.databind.ObjectMapper;
//import org.slf4j.Logger;
//import org.slf4j.LoggerFactory;
//import org.springframework.web.socket.*;
//
//import java.util.ArrayList;
//import java.util.Date;
//import java.util.List;
//
///**
// * Created by guo on 2017/8/31.
// */
//public class SystemWebSocketHandler implements WebSocketHandler {
//    private Logger log = LoggerFactory.getLogger(SystemWebSocketHandler.class);
//
//    private static final List<WebSocketSession> users = new ArrayList<WebSocketSession>();
//
//    private ObjectMapper objectMapper = new ObjectMapper();
//
//    @Override
//    public void afterConnectionEstablished(WebSocketSession webSocketSession) throws Exception {
//        System.out.println("ConnectionEstablished");
//        log.debug("ConnectionEstablished");
//        users.add(webSocketSession);
//
//        AlarmDataGetter alarmDataGetter = new AlarmDataGetter();
//        alarmDataGetter.GetAlarmData();
//        String logStr = alarmDataGetter.getAlarmData2().toString();
//        log.debug(logStr);
//        String str2 = objectMapper.writeValueAsString(alarmDataGetter.getAlarmData());
//        log.debug(str2);
//        webSocketSession.sendMessage(new TextMessage(str2));
////        webSocketSession.sendMessage(new TextMessage("connect"));
////        webSocketSession.sendMessage(new TextMessage("new_msg"));
//    }
//
//    @Override
//    public void handleMessage(WebSocketSession webSocketSession, WebSocketMessage<?> webSocketMessage) throws Exception {
//        System.out.println("handleMessage:" + webSocketMessage.toString());
//        log.debug("handleMessage:" + webSocketMessage.toString());
//        //sendMessageToUsers();
//        webSocketSession.sendMessage(new TextMessage(new Date() + ""));
//    }
//
//    @Override
//    public void handleTransportError(WebSocketSession webSocketSession, Throwable throwable) throws Exception {
//        if(webSocketSession.isOpen()){
//            webSocketSession.close();
//        }
//        users.remove(webSocketSession);
//
//        log.debug("handleTransportError:" + throwable.getMessage());
//    }
//
//    @Override
//    public void afterConnectionClosed(WebSocketSession webSocketSession, CloseStatus closeStatus) throws Exception {
//        users.remove(webSocketSession);
//        log.debug("afterConnectionClosed:" + closeStatus.getReason());
//    }
//
//    @Override
//    public boolean supportsPartialMessages() {
//        return false;
//    }
//}
