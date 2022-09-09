package com.avp.mems.ui.mobile.html.utils;

import java.net.URLEncoder;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Random;
import java.util.Set;
import java.util.SortedMap;
import java.util.TreeMap;
import java.util.UUID;

import javax.servlet.http.Cookie;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.core.env.Environment;

import com.alibaba.fastjson.JSON;
import com.alibaba.fastjson.JSONObject;

/**
 * Created by pw on 2017/5/28.
 * 参考页面：http://work.weixin.qq.com/api/doc#10028
 * http://work.weixin.qq.com/api/doc#10019
 */
public class WorkWeChatUtils {

    static Logger logger = LoggerFactory.getLogger(WorkWeChatUtils.class);


    static Environment environment;

    public static void setEnvironment(Environment e) {
        environment = e;
    }

    /**
     * 企业微信 检测用户是否存在 以及用户注册
     * @param request
     * @param response
     */
    public static void checkUserInfo(HttpServletRequest request, HttpServletResponse response) {
        boolean login = isLogin(request);
        try {

            logger.info("YYYY ======login======>>" + JSON.toJSONString(login));
            if (login) return;
            String appName = getAppName(request);
            logger.info("YYYY ======appName======>>" + JSON.toJSONString(appName));

//            String accessToken = request.getParameter("accessToken");
            String accessToken = getAccessToken(appName);
            logger.info("YYYY ======accessToken======>>" + JSON.toJSONString(accessToken));

            Map<String, Object> userInfo = getUserInfo(accessToken, request.getParameter("code"));
            logger.info("YYYY ======userInfo======>>" + JSON.toJSONString(userInfo));

            String userId = userInfo.get("UserId").toString();
            logger.info("YYYY ======userId======>>" + JSON.toJSONString(userId));

            String userTicket = userInfo.get("user_ticket").toString();
            logger.info("YYYY ======userTicket======>>" + JSON.toJSONString(userTicket));

            Map<String, Object> user = getUser(accessToken, userId);
            logger.info("YYYY ======user======>>" + JSON.toJSONString(user));

            //TODO  可以添加 注册用户行为 或者登录

            int time = 7 * 24 * 60 * 60;
            setCookie(response, "USER", URLEncoder.encode(JSON.toJSONString(user), "utf-8"), time);
        } catch (Exception e) {
            logger.error("YYYY ======user======>>" + e.getMessage());
        }

    }

    /**
     * 判断用户是否登录
     *    Cookie  OR  Session
     * @param request
     * @return
     */
    public static boolean isLogin(HttpServletRequest request) {
        Cookie user = getCookie(request, "USER");
        if (null != user) {
            logger.error("====>> User :" + user.getValue());
            return true;
        }

//        if(null != getSessionValue(request, "USER")){
//            return true;
//        }

        return false;
    }

    /**
     * 请求连接：https://qyapi.weixin.qq.com/cgi-bin/gettoken
     * 获取：access_token
     * @param appName
     * @return
     */
    public static String getAccessToken(String appName) {
        //"https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=ww958c5f9d94ed052c&corpsecret=fLg_YT6auVuQHJoDfrAw0f672KEq5ooltHbTcPAnuOw"
        String url = "https://qyapi.weixin.qq.com/cgi-bin/gettoken?corpid=" + getCorpId(appName) + "&corpsecret=" + getCorpSecret(appName);
        String result = HttpUtil.sendGet(url);
        if (null != result && result.trim().length() > 0) {
            Map<String, Object> jsonMap = (Map<String, Object>) JSON.parse(result);
            System.out.println(">>>>>>>>>>>jsonMap.get(errcode).toString()>>>>>>>>>>>>["+jsonMap.get("errcode").toString()+"]");
            System.out.println(">>>>>>>>>>>jsonMap.get(access_token).toString()>>>>>>>>>>>>["+jsonMap.get("access_token").toString()+"]");
            if ("0".equals(jsonMap.get("errcode").toString())) return jsonMap.get("access_token").toString();
            logger.error("====>> Error :" + result);
        }
        return null;
    }

    public static Map<String, Object> getHtmlCode(HttpServletRequest request) {
        String appName = getAppName(request);
        StringBuffer url = request.getRequestURL();
        String getUrl = "https://open.weixin.qq.com/connect/oauth2/authorize";
        StringBuffer stringBuffer = new StringBuffer("?appid=" + getCorpId(appName));
        stringBuffer.append("&redirect_uri=" + url);
        stringBuffer.append("&response_type=code&scope=snsapi_privateinfo");
        stringBuffer.append("&agentid=" + getAgentId(appName));
        stringBuffer.append("&state=getHtmlCode");
        stringBuffer.append("E#wechat_redirect");

        String result = HttpUtil.sendGet(getUrl + stringBuffer.toString());
        Map<String, Object> jsonMap = getStringObjectMap(result);

        return jsonMap;
    }

    /**
     * 获取用户信息：https://qyapi.weixin.qq.com/cgi-bin/user/get
     *      DOC:https://work.weixin.qq.com/api/doc#10019
     *
     * @param accessToken
     * @param userId
     * @return
     */
    public static Map<String, Object> getUser(String accessToken, String userId) {
        String url = "https://qyapi.weixin.qq.com/cgi-bin/user/get?access_token=" + accessToken + "&userid=" + userId;
        String result = HttpUtil.sendGet(url);
        Map<String, Object> jsonMap = getStringObjectMap(result);
        System.out.println(">>>>>>>>>>>jsonMap.get(userid).toString()>>>>>>>>>>>>["+jsonMap.get("UserId").toString()+"]");
        return jsonMap;
    }

    /**
     *  通过code获取用户信息:https://qyapi.weixin.qq.com/cgi-bin/user/getuserinfo
     *  DOC:https://work.weixin.qq.com/api/doc#10719
     * @param accessToken
     * @param code
     * @return
     */
    public static Map<String, Object> getUserInfo(String accessToken, String code) {
        String url = "https://qyapi.weixin.qq.com/cgi-bin/user/getuserinfo?access_token=" + accessToken + "&code=" + code;
        System.out.println(">>>>>>>>>>url>>>>>>>>>>>["+url+"]");
        String result = HttpUtil.sendGet(url);
        System.out.println(">>>>>>>>>>getUserInfo>>>>>>>>>>>["+result+"]");
        Map<String, Object> jsonMap = getStringObjectMap(result);
        return jsonMap;
    }

    public static Map<String, Object> getUserDetail(String accessToken, String userTicket) {
        String url = "https://qyapi.weixin.qq.com/cgi-bin/user/getuserdetail?access_token=" + accessToken;
        Map<String,String> map = new HashMap<String,String>();
		map.put("user_ticket", userTicket);
		
		String param = JSONObject.toJSONString(map);
        String result = HttpUtil.sendPost(url, param);
        System.out.println(">>>>>>>>>>getUserDetail>>>>>>>>>>>["+result+"]");
        Map<String, Object> jsonMap = getStringObjectMap(result);
        return jsonMap;
    }
    
    /**
     * js签名
     * @param accessToken
     * @return
     */
    public static Map<Object, Object> getSign(String appName,String signUrl) {
        System.out.println(">>>>>>>>>>>>getSign>>>>>code>>>>>>>>>["+appName+"]");
        System.out.println(">>>>>>>>>>>>getSign>>>>>signUrl>>>>>>>>>["+signUrl+"]");
        String token = getAccessToken(appName);
        System.out.println(">>>>>>>>>>>>getSign>>>>>token>>>>>>>>>["+token+"]");
    	 String url = "https://qyapi.weixin.qq.com/cgi-bin/get_jsapi_ticket?access_token=" + token;
         String result = HttpUtil.sendGet(url);
         System.out.println(">>>>>>>>>>>>result>>>>>>>>>>>>>>["+result+"]");
         Map<String, Object> jsonMap = getStringObjectMap(result);
         String ticket = String.valueOf(jsonMap.get("ticket"));
         System.out.println(">>>>>>>>>>>>ticket>>>>>>>>>>>>>>["+ticket+"]");
         SortedMap<Object, Object> createSign = createSign(ticket, signUrl);
         return createSign;
    }

    /**
     * 根据域名获取 APP NAME
     * @param request
     * @return
     */ //TODO 重要配置 慎重对待
    public static String getAppName(HttpServletRequest request) {
        StringBuffer url = request.getRequestURL();
        if (url.indexOf("localhost") != -1 || url.indexOf("www.avantport.com") != -1
                || url.indexOf("www.ac-mems.com") != -1) {
            return "chengdu";
        } else if (url.indexOf("dev.ac-mems.com") != -1) {
            return "avp";
        }
        return null;
    }

    private static Object getSessionValue(HttpServletRequest request, String key) {
        return request.getSession().getAttribute(key);
    }

    private static Cookie getCookie(HttpServletRequest request, String key) {
        Cookie[] cookies = request.getCookies();
        if (null != cookies && cookies.length > 0) {
            for (int i = 0; i < cookies.length; i++) {
                if (key.equals(cookies[i].getName())) return cookies[i];
            }
        }
        return null;
    }

    private static void setCookie(HttpServletResponse response, String key, String value, int time) {
        Cookie cookie = new Cookie(key, value);
        cookie.setMaxAge(time);
        cookie.setPath("/");
        response.addCookie(cookie);
    }

    private static Map<String, Object> getStringObjectMap(String result) {
        if (null != result && result.trim().length() > 0) {
            Map<String, Object> jsonMap = (Map<String, Object>) JSON.parse(result);
            if ("0".equals(jsonMap.get("errcode").toString())) {
                return jsonMap;
            } else {
                logger.error("====>> Error :" + result);
            }
        }
        return null;
    }

    private static String getCorpId(String appName) {
        return environment.getProperty("conf.wechat." + appName + ".corpid");
    }

    private static String getCorpSecret(String appName) {
        return environment.getProperty("conf.wechat." + appName + ".corpsecret");
    }

    private static String getAgentId(String appName) {
        return environment.getProperty("conf.wechat." + appName + ".agentId");
    }
    
	private static SortedMap<Object, Object> createSign(String ticket, String signUrl) {
        System.out.println(">>>>>>>>>>>>ticket>>>>createSign>>>>>>>>>>["+ticket+"]");
        System.out.println(">>>>>>>>>>>>signUrl>>>>createSign>>>>>>>>>>["+signUrl+"]");
		SortedMap<Object, Object> parameters = new TreeMap<Object, Object>();
		
		String noncestr =UUID.randomUUID().toString().replace("-", "").substring(0, 16);
		
		String timestamp = String.valueOf(System.currentTimeMillis() / 1000);
		
		String string1 = "jsapi_ticket="+ticket+"&noncestr="+noncestr+"&timestamp="+timestamp+"&url="+signUrl;
		
		String signature = Decript.SHA1(string1);
		parameters.put("noncestr", noncestr);
		parameters.put("timestamp", timestamp);
		parameters.put("signature", signature);
		System.out.println(">>>>>>>>>>>>>createSign>>>>>>noncestr>>>>>>"+noncestr);
		System.out.println(">>>>>>>>>>>>>createSign>>>>>>timestamp>>>>>>"+timestamp);
		System.out.println(">>>>>>>>>>>>>createSign>>>>>>signature>>>>>>"+signature);
		return parameters;
	}

}
