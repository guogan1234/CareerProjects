package com.avp.mems.ui.mobile.html.conmunicate;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.core.MessageSendingOperations;
import org.springframework.messaging.handler.annotation.DestinationVariable;
import org.springframework.messaging.handler.annotation.MessageMapping;
import org.springframework.messaging.handler.annotation.SendTo;
import org.springframework.messaging.simp.annotation.SubscribeMapping;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;

/**
 * Created by boris feng on 2017/6/26.
 */
@Controller
@RequestMapping("/webSocket")
@MessageMapping("foo")
public class WsController {
    private final Logger logger = LoggerFactory.getLogger(WsController.class);

    @Autowired
    private MessageSendingOperations<String> messagingTemplate;

//    @Autowired
//    private MyWSockClient myWSockClient;

//    private List<ReceiveMsg> receiveMsgs = null;
    private ObjectMapper objectMapper = new ObjectMapper();

    @MessageMapping("/welcome")
    @SendTo("/topic/getAlarmCount")
    public String sayWelcome(String message) throws Exception{
        System.out.println("sayWelcome: " + message);
        Thread.sleep(10000);
        return "welcome to ws world, " + message;
    }

    public class NotifyEvent {
        public String getEvent() {
            return event;
        }

        public NotifyEvent(String event) {
            this.event = event;
        }

        public void setEvent(String event) {
            this.event = event;
        }

        private String event = "ff";

    }
    // 项目参考
    @SubscribeMapping("/welcomeBy/{uid}")
    @SendTo("/topic/getEvent/{uid}")
    public  NotifyEvent sayWelcomeBy(@DestinationVariable("uid") String uid, String message) throws Exception {
        System.out.println("sayWelcomeTo: " + uid + ", " + message);
        Thread.sleep(5000);
        return new NotifyEvent("welcome " + uid +" to ws world, " + message);
    }

    //@MessageMapping - 客户端发送过来的地址
    //@SendTo - 推送到客户端地址
    @MessageMapping("hello")
    @SendTo("/queue/getResponse/{uid}")
    public void sayHello(String message) {
        System.out.println("sayHello: " + 0 + ", " + message);
//        messagingTemplate.convertAndSend("/queue/getResponse/0", "{'key':'hello'}");
    }

    @MessageMapping("/startService")
    public void startService(String message, java.security.Principal principal) throws Exception {
        logger.debug("接收到页面的参数：" + message);
//    	myWSockClient.sendMessage(WSockUtils.toMessage(message));
        //messagingTemplate.convertAndSend("/queue/getResponse/0", "hello, " + message);
    }

//    private void parseReceiveMsg(String msg){
//        JavaType javaType1 = objectMapper.getTypeFactory().constructParametricType(ArrayList.class, ReceiveMsg.class);
//        try {
//            receiveMsgs = (List<ReceiveMsg>) objectMapper.readValue(msg, javaType1);
//            for (ReceiveMsg r : receiveMsgs) {
//                logger.debug("msg.type = {},msg.ids={}",r.getType(),r.getIds());
//            }
//        }
//        catch (Exception e){
//            logger.error(e.getMessage());
//        }
//    }


}
