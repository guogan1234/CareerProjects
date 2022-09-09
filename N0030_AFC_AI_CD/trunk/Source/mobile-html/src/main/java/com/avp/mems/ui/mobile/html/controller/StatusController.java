package com.avp.mems.ui.mobile.html.controller;

import com.avp.mems.ui.mobile.html.model.SendMsg;
import com.avp.mems.ui.mobile.html.model.StatusMsg;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.messaging.core.MessageSendingOperations;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * Created by guo on 2017/9/21.
 */
@RestController
public class StatusController {
    private final Logger logger = LoggerFactory.getLogger(StatusController.class);

    @Autowired
    private MessageSendingOperations<String> messagingTemplate;

    @RequestMapping(value = "status",method = RequestMethod.POST)
     public void getStatusFromRequest(@RequestBody StatusMsg msg){
        logger.debug("getStatusFromRequest...{}", msg.getSendMsgList().size());

        List<SendMsg> temp = msg.getSendMsgList();
        messagingTemplate.convertAndSend("/queue/getResponse/1", temp);
    }

    @RequestMapping(value = "status2",method = RequestMethod.POST)
    String getStatus2(@RequestBody String string){
        logger.debug("getStatus2...");

        return null;
    }
}
