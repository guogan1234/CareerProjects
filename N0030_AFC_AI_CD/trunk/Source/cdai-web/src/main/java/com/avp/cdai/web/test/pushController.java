package com.avp.cdai.web.test;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.http.HttpEntity;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.client.RestTemplate;

/**
 * Created by guo on 2017/10/19.
 */
@RestController
public class pushController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private ObjectMapper objectMapper = new ObjectMapper();

    @RequestMapping(value = "testPush",method = RequestMethod.POST)
    private void testPush(){
        RestTemplate restTemplate=new RestTemplate();
        String url="https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=wwe0a3883554c3531c&corpsecret=NWQwulktiBCUwuRTz-PvwammFa13fFuxdJblOpm1FuM";
        tokenModel model = restTemplate.getForObject(url,tokenModel.class);
        String str = model.getAccess_token();
        logger.debug("str:{}",str);

        //生成发送消息
        msgSub sub = new msgSub();
        sub.setContent("hello boy!!!详情：http://www.baidu.com");
        msgModel model1 = new msgModel();
        model1.setTouser("@all");
        model1.setMsgtype("text");
        model1.setAgentid(1000002);
        model1.setText(sub);
        //推送消息
        String url2 = "https://qyapi.weixin.qq.com/cgi-bin/message/send?access_token=" + str;
        logger.debug("url2:{}",url2);
//        restTemplate.postForEntity(url2,model1,String.class);
        RestTemplate restTemplate1 = new RestTemplate();
        HttpEntity<msgModel> entity = new HttpEntity<msgModel>(model1);
//        restTemplate1.postForEntity(url2,entity,String.class);
        try {
            String sendMsgsJson = objectMapper.writeValueAsString(model1);
            logger.debug("1-此次推送的数据为：{}", sendMsgsJson);
            ResponseEntity<String> res = restTemplate1.postForEntity(url2,entity,String.class);
            logger.debug("res:{}",res);
        }catch (Exception e){
            logger.error(e.getMessage());
        }
        return;
    }
}
