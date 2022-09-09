package com.avp.cdai.web.controller;

import com.avp.cdai.web.entity.TicketType;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.io.File;

/**
 * Created by guo on 2017/12/12.
 */
@RestController
public class TestController {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    @RequestMapping(value = "TestDefaultNewClass",method = RequestMethod.GET)
    private void TestDefaultNewClass(){
        TicketType obj = new TicketType();
        logger.debug("TestDefaultNewClass!!!");
    }

    private void readFile(){
        String fileName = null;
        fileName = createfile();
        writefile(fileName);
        read(fileName);
    }

    private String createfile(){
        String str = null;
        try {
            File f = new File("f://testFile.txt");
            boolean b = f.mkdir();
            if (!b) {
                logger.error("create file failed!");
            }
            str = f.getAbsolutePath();
            String str2 = f.getCanonicalPath();
        }catch (Exception e){
            logger.error(e.getMessage());
        }
        return str;
    }

    private void writefile(String filename){

    }

    private void read(String filename){}
}
