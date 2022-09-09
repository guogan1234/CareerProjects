package com.avp.mems.ui.mobile.html.controller;

import java.util.HashMap;
import java.util.Map;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.core.env.Environment;
import org.springframework.data.repository.query.Param;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.avp.mems.ui.mobile.html.utils.WorkWeChatUtils;

/**
 * Created by zhoujs on 2017/5/9.
 */
@RestController
public class WXAuthController {
	Logger logger = LoggerFactory.getLogger(this.getClass());

		@Autowired
		Environment	eve;

		@RequestMapping("/getWxUserInfo")
		public Map<String, Object>  getWXUserInfo(@Param("code")String code,@Param("projectName")String projectName){
			System.out.println(">>>>>>>>code>>>>>>>>>>>>>>["+code+"]");
			System.out.println(">>>>>>>>projectName>>>>>>>>>>>>>>["+projectName+"]");
			
			Map<String, Object> retMap = new HashMap<String, Object>();
			
			String token = WorkWeChatUtils.getAccessToken(projectName);
			System.out.println(">>>>>>>>token>>>>>>>>>>>>>>["+token+"]");

			Map<String, Object> userInfoMap = WorkWeChatUtils.getUserInfo(token,code);
			System.out.println(">>>>>>>>user_ticket>>>>>>>>>>>>>>["+userInfoMap.get("user_ticket")+"]");
			
			Map<String, Object> userDetailMap = WorkWeChatUtils.getUserDetail(token, String.valueOf(userInfoMap.get("user_ticket")));

			retMap.put("UserId", userInfoMap.get("UserId"));
			retMap.put("userid", userDetailMap.get("userid"));
			retMap.put("name", userDetailMap.get("name"));
			retMap.put("department", userDetailMap.get("department"));
			retMap.put("position", userDetailMap.get("position"));
			retMap.put("mobile", userDetailMap.get("mobile"));
			retMap.put("email", userDetailMap.get("email"));
			retMap.put("avatar", userDetailMap.get("avatar"));
			retMap.put("wxtoken", token);
			
	        for (Map.Entry<String, Object> entry : retMap.entrySet()) {
	            System.out.println("Key = 【" + entry.getKey() + "】, Value = 【" + entry.getValue()+"】");  
	        }
			return retMap;
		}
		
		@RequestMapping("/getWxSign")
		public Map<Object, Object>  getWXSign(@Param("appName")String appName,@Param("signUrl")String signUrl){
			return WorkWeChatUtils.getSign(appName,signUrl);
		}
}
