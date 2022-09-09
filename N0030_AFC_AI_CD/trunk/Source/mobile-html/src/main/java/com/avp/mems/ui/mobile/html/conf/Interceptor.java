package com.avp.mems.ui.mobile.html.conf;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.core.env.PropertySource;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.method.HandlerMethod;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.lang.annotation.Annotation;

/**
 * Created by pw on 2016/7/28.
 * 自定义拦截器
 */
public class Interceptor implements HandlerInterceptor {

    private Logger logger = LoggerFactory.getLogger(this.getClass());
    private PropertySource<?> properties;

    public Interceptor(PropertySource<?> properties) {
        this.properties = properties;
    }

    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler)
            throws Exception {
        logger.debug("preHandle...");
        return true;
    }

    //TODO 重要配置 慎重对待
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler,
                           ModelAndView modelAndView) throws Exception {
        HandlerMethod handlerMethod = (HandlerMethod)handler;
        Annotation classAnnotation = handlerMethod.getClass().getAnnotation(RestController.class);
        Annotation methodAnnotation = handlerMethod.getMethod().getAnnotation(ResponseBody.class);
        StringBuffer url = request.getRequestURL();
        int status = response.getStatus();
        if(status < 200 || status > 299) return;
        if(null == classAnnotation && null == methodAnnotation){
        	System.out.println(">>>>>>>>>>>modelAndView>>>>>>>>>>>>>>>【"+modelAndView+"】");
            String path ="WeChat";
            if(url.indexOf("139.129.230.140") != -1 || url.indexOf("www.avantport.com") != -1){
                path = "WeChat";
                //setChengduModelAndViewObject( modelAndView, "chengdu");
            }else if(url.indexOf("dev.ac-mems.com") != -1){
                path = "WeChat";
            }
            if(modelAndView != null)
            {
                String viewName = modelAndView.getViewName();
                modelAndView.addObject("PATH",path);
                modelAndView.setViewName(path+"/"+viewName);
                System.out.println("Result View Name : " + path+"/"+viewName);
                logger.info("Result View Name : " + path+"/"+viewName);
            }
        }
//        System.out.println(">>>MyInterceptor1>>>>>>>请求处理之后进行调用，但是在视图被渲染之前（Controller方法调用之后）");
    }

    public void setChengduModelAndViewObject(ModelAndView modelAndView, String key){
    	System.out.println(">>>>>>>>>>>modelAndView>>>>>>>>>>>>>>>【"+modelAndView+"】");
    	System.out.println(">>>>>>>>>>>key>>>>>>>>>>>>>>>【"+key+"】");
        if(null != key && "chengdu".equals(key)){
            modelAndView.addObject("CORPID",getPropertyValue("conf.wechat." + key + ".corpid"));
            modelAndView.addObject("CORPSECRET",getPropertyValue("conf.wechat." + key + ".corpsecret"));
            modelAndView.addObject("AGENTID",getPropertyValue("conf.wechat." + key + ".agentId").toString());
        }
    }

    private Object getPropertyValue(String key){
        Object property = properties.getProperty(key);
        System.out.println("---------------------------> Key :" + key + "\t\t Value :" + property);
        return property;
    }

    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex)
            throws Exception {
//        System.out.println(">>>MyInterceptor1>>>>>>>在整个请求结束之后被调用，也就是在DispatcherServlet 渲染了对应的视图之后执行（主要是用于进行资源清理工作）");
    }




}
