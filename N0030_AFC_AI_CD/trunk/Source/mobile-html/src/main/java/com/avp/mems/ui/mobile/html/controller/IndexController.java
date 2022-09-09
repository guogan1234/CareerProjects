package com.avp.mems.ui.mobile.html.controller;

import com.avp.mems.ui.mobile.html.utils.WorkWeChatUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.util.Enumeration;
import java.util.Map;

/**
 * Created by pw on 2017/4/25.
 */
@Controller
public class IndexController {

     Logger logger = LoggerFactory.getLogger(IndexController.class);


    @RequestMapping("/")
    public String index( HttpServletRequest request, HttpServletResponse response,Map<String, Object> map) {
        request(request, map);
        return "index";
    }

    @RequestMapping("/skip")
    public String skip(@RequestParam("path")String path, HttpServletRequest request, HttpServletResponse response, Map<String, Object> map) {
        request(request, map);
        return path;
    }


    @RequestMapping("/{path}/skip")
    private String pathSkip(HttpServletRequest request, HttpServletResponse response, Map<String, Object> map ,
                            @PathVariable("path")String path){
        WorkWeChatUtils.checkUserInfo( request, response);
        request(request, map);
        return path + '/';
    }

    @RequestMapping("/{path}/{fileName}/skip")
    private String pathSkip(HttpServletRequest request, HttpServletResponse response, Map<String, Object> map ,
                            @PathVariable("path")String path,
                            @PathVariable("fileName") String fileName){
        WorkWeChatUtils.checkUserInfo( request, response);
        request(request, map);
        return path + '/' + fileName;
    }


    private void request(HttpServletRequest request, Map<String, Object> map) {
        Enumeration<String> names = request.getParameterNames();
        if(null != names){
            while (names.hasMoreElements()){
                String key = names.nextElement();
                map.put(key,request.getParameter(key));
                logger.info("\n---------------------------------------->Key:" + key + "\t\t Value:" + request.getParameter(key));
            }
        }
    }


}
