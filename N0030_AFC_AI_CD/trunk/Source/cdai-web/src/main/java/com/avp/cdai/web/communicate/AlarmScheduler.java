package com.avp.cdai.web.communicate;

import com.avp.cdai.web.config.ProjectConfig;
import com.avp.cdai.web.model.ReceiveMsg;
import com.avp.cdai.web.model.SendMsg;
import com.avp.cdai.web.model.StatusMsg;
import com.avp.cdai.web.service.AlarmTickService;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.MediaType;
import org.springframework.messaging.core.MessageSendingOperations;
import org.springframework.stereotype.Component;
import org.springframework.web.client.RestTemplate;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.Random;

/**
 * Created by guo on 2017/9/5.
 */
@Component
public class AlarmScheduler {
    @Autowired
    private MessageSendingOperations<String> messagingTemplate;

//    @Autowired
//    WsController wsController;
    private boolean bFirst = false;
    //一次推送量
    private Integer page = 10;
    //调度次数
    private Integer count = 0;
    //发送列表总长度，循环时使用
    private Integer len = 0;

    @Autowired
    AlarmTickService alarmTickService;

    @Autowired
    ProjectConfig projectConfig;

//    @Autowired
//    AlarmSetService alarmSetService;

    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private List<ReceiveMsg> receiveMsgs = null;
    private List<SendMsg> sendMsgs = new ArrayList<SendMsg>();
    private String sendMsgsJson = null;
    private ObjectMapper objectMapper = new ObjectMapper();
    private Random random = new Random();

    //Error(null异常).可以发现属性成员的初始化，在注入的类构造之前
//    private Long delay = Long.parseLong(projectConfig.getPushDelay());//

//    @Scheduled(fixedDelayString = "${cfg.pushDelay}")
    public void Test(){
        count++;
        System.out.println(new Date());
//        messagingTemplate.convertAndSend("/queue/getResponse/0", "{'key':'schedule'}");
        messagingTemplate.convertAndSend("/queue/getResponse/0", "hello!!!");
        if(!bFirst){
            System.out.println("First Scheduled!!!");
            bFirst = true;
            alarmTickService.InitMsgConfig();
            //获取发送列表总长度
            len = alarmTickService.getSendMsgLength();
            logger.debug("len:{}",len);
            //(可能有BUG的地方)若生成报警信息列表时间太长，第一次推送是否会被调用
            Integer start = (count - 1)*page;
            Integer end = count*page;
            List<SendMsg> sendMsg = alarmTickService.getMsg(start,end);
//            messagingTemplate.convertAndSend("/queue/getResponse/0", sendMsg);
            //推送给java应用程序，而不是推送到WebSocket
            RestTemplate restTemplate=new RestTemplate();
//            String url="http://192.168.1.110:4065/status";
            String url = projectConfig.getPushUrl();
            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_JSON_UTF8);
            StatusMsg msg = new StatusMsg();
            msg.setSendMsgList(sendMsg);
            HttpEntity<StatusMsg> entity = new HttpEntity<StatusMsg>(msg);
            try {
            sendMsgsJson = objectMapper.writeValueAsString(msg);
            logger.debug("1-此次推送的数据为：{}", sendMsgsJson);
//            restTemplate.exchange(url, HttpMethod.POST, entity, String.class);
            restTemplate.postForEntity(url,entity,String.class);

//                sendMsgsJson = objectMapper.writeValueAsString(sendMsg);
//                logger.debug("1-此次推送的数据为：{}", sendMsgsJson);
            }catch (Exception e){
                logger.error(e.getMessage());
            }
        }else{
            System.out.println("Not First Scheduled!!!");
            Integer start = (count - 1)*page;
            Integer end = count*page;
            List<SendMsg> sendMsg = alarmTickService.getMsg(start,end);
//            messagingTemplate.convertAndSend("/queue/getResponse/0", sendMsg);
            //推送给java应用程序，而不是推送到WebSocket
            //方法1
            RestTemplate restTemplate=new RestTemplate();
//            String url="http://192.168.1.110:4065/status";
            String url = projectConfig.getPushUrl();
            HttpHeaders headers = new HttpHeaders();
            headers.setContentType(MediaType.APPLICATION_JSON_UTF8);
            StatusMsg msg = new StatusMsg();
            msg.setSendMsgList(sendMsg);
            HttpEntity<StatusMsg> entity = new HttpEntity<StatusMsg>(msg, headers);
            try {
                sendMsgsJson = objectMapper.writeValueAsString(msg);
                logger.debug("2-此次推送的数据为：{}", sendMsgsJson);
//                restTemplate.exchange(url, HttpMethod.POST, entity, String.class);
                restTemplate.postForEntity(url,entity,String.class);
            }catch (Exception e){
                logger.error(e.getMessage());
            }
            if(end > len){//循环控制使用，清空发送数据，重新生成
                logger.debug("一次完整推送完毕！开始下一轮...");
                count = 0;
                alarmTickService.clearSendMsgList();
                alarmTickService.InitMsgConfig();
            }
        }

//        List<ReceiveMsg> list = wsController.getReceiveMsgs();//可以注入控制器，但不正规
//        receiveMsgs = alarmSetService.getReceiveMsgs();
//        if(receiveMsgs == null){
//            System.out.println("list is null!!!");
//            return;
//        }
//        for (ReceiveMsg r : receiveMsgs) {
////            logger.debug("msg.type = {},msg.ids={}",r.getType(),r.getIds());
//            System.out.println("schedule...");
//            System.out.println(r.getType()+";"+r.getIds());
//        }
//        //生成报警信息
//        sendMsgs.clear();
//        getSendMsg();
//        messagingTemplate.convertAndSend("/queue/getResponse/0", sendMsgs);
//        System.out.println("end!");
    }

    private void getSendMsg(){
        for (ReceiveMsg r : receiveMsgs) {
            logger.debug("3:msg.type = {},msg.ids={}",r.getType(),r.getIds());
            Integer type = r.getType();
            List<Integer> ids = r.getIds();
            for(Integer id:ids) {
                //生成随机数
//                int n = (int) (0 + Math.random() * (3 - 0 + 1));
                int n = random.nextInt(3);
                SendMsg s = new SendMsg(type,id,"Test",n);
                sendMsgs.add(s);
            }
        }
        //JSON转换
        try {
            sendMsgsJson = objectMapper.writeValueAsString(sendMsgs);
            logger.debug("sendMsgsJson:{}",sendMsgsJson);
        }
        catch (Exception e){
            logger.error(e.getMessage());
        }
    }
}
