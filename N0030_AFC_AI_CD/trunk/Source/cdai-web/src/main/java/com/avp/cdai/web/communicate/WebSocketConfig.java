//package com.avp.cdai.web.communicate;
//
//import org.springframework.context.annotation.Configuration;
//import org.springframework.web.servlet.config.annotation.EnableWebMvc;
//import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;
//import org.springframework.web.socket.WebSocketHandler;
//import org.springframework.web.socket.config.annotation.EnableWebSocket;
//import org.springframework.web.socket.config.annotation.WebSocketConfigurer;
//import org.springframework.web.socket.config.annotation.WebSocketHandlerRegistry;
//
///**
// * Created by guo on 2017/8/31.
// */
////class WebSocketConfig extends WebMvcConfigurerAdapter implements WebSocketConfigurer
//@Configuration
//@EnableWebMvc
//@EnableWebSocket
//public class WebSocketConfig extends WebMvcConfigurerAdapter implements WebSocketConfigurer{
//    @Override
//    public void registerWebSocketHandlers(WebSocketHandlerRegistry registry) {
////        registry.addHandler(systemWebSocketHandler(),"/webSocketServer/sockjs").setAllowedOrigins("*").withSockJS();
//          registry.addHandler(systemWebSocketHandler(),"/portfolio").setAllowedOrigins("*").withSockJS();
//    }
//
//    public WebSocketHandler systemWebSocketHandler(){
//        return new SystemWebSocketHandler();
//    }
//}
