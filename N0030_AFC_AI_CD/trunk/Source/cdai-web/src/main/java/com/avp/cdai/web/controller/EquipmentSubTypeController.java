package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.EquipmentSubType;
import com.avp.cdai.web.repository.EquipmentSubTypeRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import com.avp.cdai.web.rest.RestBody;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;
import java.util.Timer;

/**
 * Created by guo on 2017/9/4.
 */
@RestController
public class EquipmentSubTypeController {
    @Autowired
    EquipmentSubTypeRepository equipmentSubTypeRepository;

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private Timer timer = null;
    private Timer timer2 = new Timer();

    @RequestMapping(value = "AllEquipmentSubType",method = RequestMethod.GET)
    ResponseEntity<RestBody<EquipmentSubType>> findAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<EquipmentSubType> list = equipmentSubTypeRepository.findAll();
            logger.debug("设备子类型数量为：{}", list.size());
            builder.setResultEntity(list, ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

//    //测试代码，测试定时器在Controller中使用
//    public EquipmentSubTypeController(){
//        timer = new Timer();
//        timer.schedule(new TimerTask() {
//            @Override
//            public void run() {
//                logger.debug("timer...");
//            }
//        },1000);
//    }
//
//    @RequestMapping(value = "/timer",method = RequestMethod.GET)
//    void timerWork(){
//        timer = new Timer();
//        timer.schedule(new TimerTask() {
//            @Override
//            public void run() {
//                System.out.println("timer...");
//                logger.debug("timer...");
//            }
//        },1000);
//    }
//
//    @RequestMapping(value = "/timer2",method = RequestMethod.GET)
//    void timer2Work(){
//        timer2.schedule(new TimerTask() {
//            @Override
//            public void run() {
//                System.out.println("timer2...");
//                logger.debug("timer2...");
//            }
//        },1000);
//    }
}
