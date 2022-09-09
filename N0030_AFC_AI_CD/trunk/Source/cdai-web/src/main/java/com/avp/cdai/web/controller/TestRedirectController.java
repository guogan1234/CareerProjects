package com.avp.cdai.web.controller;

import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

/**
 * Created by guo on 2017/9/5.
 */
@RestController
public class TestRedirectController {
    @RequestMapping(value = "/Redirect",method = RequestMethod.GET)
    public String test(){
        System.out.println("Redirct...");
        return "redirect:/TestRedirect";
//        return new ModelAndView("redirect:/TestRedirect");
    }
}
