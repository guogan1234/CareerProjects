package com.avp.cdai.web.controller;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

import java.util.HashMap;
import java.util.Map;

/**
 * Created by guo on 2017/9/28.
 */
@RestController
public class UpdateStationController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    RestTemplate restTemplate = new RestTemplate();

    @RequestMapping(value = "stationLocation",method = RequestMethod.GET)
    private void updateLocation(){
//        String url = "http://api.map.baidu.com/place/v2/search?q={q}&tag={tag}&region={region}&output={output}&ak={ak}";
        String url = "http://api.map.baidu.com/place/v2/search";
        Map<String, Object> uriVariables = new HashMap<String, Object>();
        uriVariables.put("q", "鼓楼");
        uriVariables.put("tag", "交通设施");
        uriVariables.put("region","南京");
        uriVariables.put("output","json");
        uriVariables.put("ak","d9x68euLWXdN8WMBhjO0aMyB");

        String result = restTemplate.getForObject(url, String.class,  uriVariables);
        logger.debug("返回数据：{}",result);
    }
}
