package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.EquipmentData;
import com.avp.cdai.web.entity.EquipmentSubType;
import com.avp.cdai.web.entity.ObjEquipment;
import com.avp.cdai.web.repository.EquipmentSubTypeRepository;
import com.avp.cdai.web.repository.ObjEquipmentRepository;
import com.avp.cdai.web.rest.ResponseBuilder;
import com.avp.cdai.web.rest.ResponseCode;
import com.avp.cdai.web.rest.RestBody;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by guo on 2017/9/4.
 */
@RestController
public class ObjEquipmentController {
    @Autowired
    ObjEquipmentRepository objEquipmentRepository;
    @Autowired
    EquipmentSubTypeRepository equipmentSubTypeRepository;

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "findAllEquipments",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjEquipment>> findAll(){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<ObjEquipment> list = objEquipmentRepository.findAll();
            logger.debug("设备总数量为：{}",list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "findByStationId/{id}",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjEquipment>> findBystationId(@PathVariable("id") Integer stationId){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<ObjEquipment> list = objEquipmentRepository.findByStationId(stationId);
            logger.debug("车站{}包含的设备数量为：{}",stationId,list.size());
            builder.setResultEntity(list,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "stationById/{id}",method = RequestMethod.GET)
    ResponseEntity<RestBody<EquipmentData<String>>> findBystationId2(@PathVariable("id")Integer stationId){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            List<Integer> typeList = objEquipmentRepository.findDistinctTypeByStationId(stationId);
            logger.debug("车站{}有{}种类型设备",stationId,typeList.size());

            List<EquipmentSubType> subTypeList = equipmentSubTypeRepository.findAll();
            Map<String,List<ObjEquipment>> dataMap = new HashMap<String,List<ObjEquipment>>();
            for(Integer type:typeList){
                List<ObjEquipment> list = objEquipmentRepository.findByStationIdAndTypeId(stationId,type);
                String typeName = null;
                for(EquipmentSubType subType:subTypeList){
                    if(type == subType.getEquipmentType())
                        typeName = subType.getShortName();
                }
                if(typeName != null) {
                    dataMap.put(typeName, list);
                }else{
                    logger.debug("找不到对于类型的简称，值为null！");
                }
            }
            EquipmentData data = new EquipmentData();
            data.setEquipmentMap(dataMap);
            builder.setResultEntity(data,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }

    @RequestMapping(value = "findByEquipmentId/{equipmentId}",method = RequestMethod.GET)
    ResponseEntity<RestBody<ObjEquipment>> findByEquimentId(@PathVariable("equipmentId") Integer equipmentId){
        ResponseBuilder builder = ResponseBuilder.createBuilder();
        try {
            ObjEquipment objEquipment = objEquipmentRepository.findByEquipmentId(equipmentId);
            logger.debug("根据设备ID获取的设备名称为：{}",objEquipment.getEquipmentName());
            builder.setResultEntity(objEquipment,ResponseCode.RETRIEVE_SUCCEED);
            return builder.getResponseEntity();
        }catch (Exception e){
            logger.error(e.getMessage());
            builder.setErrorCode(ResponseCode.RETRIEVE_FAILED);
        }
        return builder.getResponseEntity();
    }
}
