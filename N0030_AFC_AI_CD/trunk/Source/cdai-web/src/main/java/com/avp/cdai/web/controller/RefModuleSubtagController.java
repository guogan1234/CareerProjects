package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.RefModuleSubtag;
import com.avp.cdai.web.repository.RefModuleSubtagRepository;
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

/**
 * Created by guo on 2017/9/12.
 */
@RestController
public class RefModuleSubtagController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    RefModuleSubtagRepository refModuleSubtagRepository;

    @RequestMapping(value = "/refMouduleSubType",method = RequestMethod.GET)
    ResponseEntity<RestBody<RefModuleSubtag>> getAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<RefModuleSubtag> list = refModuleSubtagRepository.findAll();
            logger.debug("模块子标签总数量为：{}",list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
