package com.avp.cdai.web.controller.htmlController;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.servlet.ModelAndView;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.security.Principal;

/**
 * Created by len on 2017/9/19.
 */
@Controller
public class EnergyController {

    private static final Logger LOGGER = LoggerFactory.getLogger(EnergyController.class);

    public static final String TITLE_ENERGYSAVING= "能耗分析节能";


    @RequestMapping(value = {"/energy/energySaving"})
    public ModelAndView energySaving(HttpSession session, HttpServletRequest request, Principal user) {
        ModelAndView model = new ModelAndView("cdai-pages/energy/energySaving");
        try {
            model.addObject("cdai_title", TITLE_ENERGYSAVING);
        } catch (Exception e) {
            LOGGER.error(e.getMessage());
            e.printStackTrace();
        }
        return model;
    }

}
