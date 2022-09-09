package com.avp.config.controller;

import com.avp.config.entity.User;
import com.avp.config.service.UserInf;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.ResponseBody;
import org.springframework.web.bind.annotation.RestController;
@Controller
public class DemoController {
    @Autowired
    private UserInf userInf;

    @RequestMapping("/")
    public String demo() {
        return "1";
    }

    @RequestMapping("/addDemo")
    @ResponseBody
    public String addDemo(){
        try {
            User user = new User();
            user.setAge(111);
            user.setName("2222");
            userInf.addUser(user);
        }catch (Exception e){

        }
        return "请求成功";
    }
}
