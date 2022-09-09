package com.avp.cdai.web.pushWechat.service;

import com.avp.cdai.web.config.ProjectConfig;
import com.avp.cdai.web.pushWechat.model.*;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;

import java.util.List;

/**
 * Created by guo on 2017/10/20.
 */
@Component
public class pushService {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private ObjectMapper objectMapper = new ObjectMapper();

    @Autowired
    ProjectConfig projectConfig;

    //测试的企业微信应用
//    private final String TOKEN_URL = "https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=ww958c5f9d94ed052c&corpsecret=lYfbaql8MNwY2j6TGNaLy9w8Id7kT_auOsYYDazlCkU";
    private final String PUSH_URL = "https://qyapi.weixin.qq.com/cgi-bin/message/send?access_token=";
//    private final Integer AGENTID = 1000011;
    //
    private String TOKEN_URL = null;
    private String agentId_str = null;
    private Integer AGENTID = 0;

    @Autowired
    AlarmService alarmService;
    @Autowired
    ConnectService connectService;

//    @Scheduled(fixedDelayString = "${cfg.pushWechatDelay}")
    public void pushToWechat(){
        //初始化常量
        TOKEN_URL = projectConfig.getAlarmPushUrl();
        agentId_str = projectConfig.getAlarmAgentId();
        AGENTID = Integer.parseInt(agentId_str);

        //获取访问Token
//        logger.debug("pushToWechat...str--{}",agentId_str);
        String token = getToken();
        if(token == null){
            logger.error("未获取到正确的访问Token！");
        }

        List<PushMsgModel> alarmMsgs = alarmService.getPushList();
        if(alarmMsgs == null){
            logger.error("获取的报警信息列表为null！");
        }
        logger.debug("获取的报警数量为：{}",alarmMsgs.size());
        for(PushMsgModel pushMsg:alarmMsgs){
            String msg = pushMsg.getMsg();

            ContentText text = new ContentText();
            text.setTitle(pushMsg.getTag());
            text.setDescription(msg);
            text.setUrl(pushMsg.getUrl());

            WechatMsgModel model = new WechatMsgModel();
            model.setTouser("@all");
            model.setMsgtype("textcard");
            model.setAgentid(AGENTID);
            model.setTextcard(text);
            //推送消息
            String url2 = PUSH_URL + token;
            logger.debug("push_url:{}",url2);
            RestTemplate restTemplate1 = new RestTemplate();
            HttpEntity<WechatMsgModel> entity = new HttpEntity<WechatMsgModel>(model);
            try {
                String sendMsgsJson = objectMapper.writeValueAsString(model);
                logger.debug("1-此次推送的数据为：{}", sendMsgsJson);
                ResponseEntity<String> res = restTemplate1.postForEntity(url2,entity,String.class);
                logger.debug("res:{}",res);
            }catch (Exception e){
                logger.error(e.getMessage());
            }
        }
    }

//    @Scheduled(fixedDelayString = "${cfg.pushConnectDelay}")
    private void pushConnect(){
        //初始化常量
        TOKEN_URL = projectConfig.getAlarmPushUrl();
        agentId_str = projectConfig.getAlarmAgentId();
        AGENTID = Integer.parseInt(agentId_str);

        //获取访问Token
        logger.debug("pushConnect...str--{}",agentId_str);
        String token = getToken();

        //推送报警文本卡片信息
        List<String> alarmDatas = connectService.getConnAlarms();
        for (String str:alarmDatas){
            ConnectText text = new ConnectText();
            text.setContent(str);

            WechatMsgModel model = new WechatMsgModel();
            model.setTouser("@all");
            model.setMsgtype("text");
            model.setAgentid(AGENTID);
            model.setText(text);
            //推送消息
            String url2 = PUSH_URL + token;
            logger.debug("网络报警 -- push_url:{}",url2);
            RestTemplate restTemplate1 = new RestTemplate();
            HttpEntity<WechatMsgModel> entity = new HttpEntity<WechatMsgModel>(model);
            try {
                String sendMsgsJson = objectMapper.writeValueAsString(model);
                logger.debug("1-此次推送的数据为：{}", sendMsgsJson);
                ResponseEntity<String> res = restTemplate1.postForEntity(url2,entity,String.class);
                logger.debug("res:{}",res);
            }catch (Exception e){
                logger.error(e.getMessage());
            }
        }
    }

    private String getToken(){
        String token = null;
        RestTemplate restTemplate=new RestTemplate();
        TokenModel model = restTemplate.getForObject(TOKEN_URL,TokenModel.class);
        token = model.getAccess_token();
        logger.debug("token:{}",token);

        return token;
    }
}
