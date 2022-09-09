package com.avp.cdai.web.controller;

import com.avp.cdai.web.model.ReceiveMsg;
import com.avp.cdai.web.model.SendMsg;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import com.avp.cdai.web.service.AlarmSetService;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * Created by guo on 2017/9/27.
 */
@RestController
public class StatusByIdsController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private ObjectMapper objectMapper = new ObjectMapper();

    @Autowired
    AlarmSetService alarmSetService;

    @RequestMapping(value = "statusByIds",method = RequestMethod.POST)
    ResponseEntity<SendMsg> getStatusByIds(@RequestBody List<ReceiveMsg> idMsgs){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        logger.debug("获取的ids集合数量：{}",idMsgs.size());
        //清空历史数据
//        alarmSetService.clearSendMsgs();
        //存储接收数据，发送获取数据
        alarmSetService.setReceiveMsgs(idMsgs);
        alarmSetService.getSendMsg();
        List<SendMsg> sendMsgs = alarmSetService.getSendMsgs();
        try {
            String sendMsgsJson = objectMapper.writeValueAsString(sendMsgs);
            logger.debug("此次注册的返回推送的数据为：{}", sendMsgsJson);
//            alarmSetService.clearSendMsgs();//java返回的集合数据为引用的方式返回
            builder.setResultEntity(sendMsgs, ResponseCode.RETRIEVE_SUCCEED);
            alarmSetService.clearSendMsgs();//调用OK。猜测--java调用set方法会将数据复制一份，保存在类内部
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        alarmSetService.clearSendMsgs();
        return builder.getResponseEntity();
    }
}
