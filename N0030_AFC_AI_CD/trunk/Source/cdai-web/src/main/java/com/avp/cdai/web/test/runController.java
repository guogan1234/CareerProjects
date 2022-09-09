package com.avp.cdai.web.test;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

/**
 * Created by guo on 2017/10/20.
 */
@RestController
public class runController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "run",method = RequestMethod.GET)
    private void doRun(){
        while (true) {
            logger.debug("doRun...");
        }
    }

//    @PostConstruct
    private void doRun2(){
        while (true){
            logger.debug("doRun2...");
        }
    }
}
