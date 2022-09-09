package com.avp.configService.controller;

import com.avp.configService.entity.User;
import com.avp.configService.service.UserInf;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
public class UserController {
    @Autowired
    private UserInf userInf;

    @RequestMapping("/")
    public String getUser(){
        List<User> list=this.userInf.getUsers();
        if(list!=null && list.size()>0) {
            for (int i = 0; i < list.size(); i++) {
                System.out.println(list.get(i).getName());
            }
        }
        return "1111";
    }
}
