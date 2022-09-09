package com.avp.cdai.web.controller;

import com.avp.cdai.web.service.TestThread;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

/**
 * Created by guo on 2017/9/4.
 */
@RestController
public class TestTimerController {
    @RequestMapping(value = "/TestRedirect",method = RequestMethod.GET)
    public void test_dmxy(){
        System.out.println("Test Redirect OK!");
    }

    @RequestMapping(value = "/random",method = RequestMethod.GET)
    private void random(){
        for(int i = 0;i<10;i++){
            int n = (int)(5+Math.random()*(10-5+1));
            System.out.println("Random:"+n);
        }
    }

    @RequestMapping(value = "/gotoHtml",method = RequestMethod.GET)
    private String goHtml(){
        System.out.println("gotoHtml...");
        return "/templates/Test";
    }

    //通过浏览器URL发送的请求，都是GET请求，不能触发不是GET的请求
    @RequestMapping(value = "/TestThreadRun",method = RequestMethod.POST)
    private void TestThreadRun(){
        TestThread t = new TestThread();
//        t.start();
        t.run2();
    }
}
