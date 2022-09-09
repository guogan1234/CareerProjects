package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.RefModule;
import com.avp.cdai.web.repository.RefModuleRepository;
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

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/9/12.
 */
@RestController
public class RefModuleController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    RefModuleRepository refModuleRepository;

    @RequestMapping(value = "/RefModules",method = RequestMethod.GET)
    ResponseEntity<RestBody<RefModule>> getAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<RefModule> list = refModuleRepository.findAll();
            logger.debug("模块总数量为：{}",list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "/allRefModules",method = RequestMethod.GET)
    ResponseEntity<RefModule> getAllRefModules(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            logger.debug("allRefModules...");
            List<Object[]> list = refModuleRepository.getAll();
            logger.debug("模块总数量为：{}",list.size());
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "/cascadeDataLeft",method = RequestMethod.GET)
    ResponseEntity<RefModule> cascadeDataLeft(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            logger.debug("left cascade select...");
            List<Object[]> list = refModuleRepository.cascadeDatasLeft();
            logger.debug("获取的模块left级联查询记录数量为{}",list.size());
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "/cascadeDataRight",method = RequestMethod.GET)
    ResponseEntity<RefModule> cascadeDataRight(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            logger.debug("right cascade select...");
            List<Object[]> list = refModuleRepository.cascadeDatasRight();
            logger.debug("获取的模块right级联查询记录数量为{}",list.size());
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "/selectFromLargeData",method = RequestMethod.GET)
    ResponseEntity<RefModule> selectLargeData(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            Date start = new Date();
            logger.debug("start--{}", start);
            List<Object[]> list = refModuleRepository.cascadeLargeData();
            Date end = new Date();
            logger.debug("end--{}", end);
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
