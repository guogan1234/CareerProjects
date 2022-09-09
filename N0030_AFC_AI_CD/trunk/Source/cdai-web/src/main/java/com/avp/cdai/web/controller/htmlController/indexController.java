package com.avp.cdai.web.controller.htmlController;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

/**
 * Created by guo on 2017/11/23.
 */
@Controller
public class indexController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    public static final String TITLE_INDEX = "首页";


    @RequestMapping(value = {"/#","/"})
    public ModelAndView index(HttpSession session, HttpServletRequest request) {
        ModelAndView model = new ModelAndView("cdai-pages/index");
        try {
            model.addObject("cdai_title", TITLE_INDEX);
        } catch (Exception e) {
            logger.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }
}
