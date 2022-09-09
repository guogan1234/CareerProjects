package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.TicketFamily;
import com.avp.cdai.web.repository.TicketFamilyRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

/**
 * Created by guo on 2017/12/11.
 */
@RestController
public class TicketFamilyController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @Autowired
    TicketFamilyRepository ticketFamilyRepository;

    @RequestMapping(value = "ticketFamily",method = RequestMethod.GET)
    ResponseEntity<TicketFamily> getAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try{
            List<TicketFamily> list = ticketFamilyRepository.findAll();
            logger.debug("获取的票卡种类数量为：{}",list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
