package com.avp.mems.ui.mobile.html.conf;

import com.avp.mems.ui.mobile.html.utils.WorkWeChatUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.core.env.Environment;
import org.springframework.stereotype.Component;

/**
 * Created by pw on 2016/7/25.
 * Spring Boot 启动完毕后加载执行事件
 */
@Component
public class StartupRunner implements CommandLineRunner {

    @Autowired
    Environment environment;

    @Override
    public void run(String... args) throws Exception {
        WorkWeChatUtils.setEnvironment(environment);
    }


}
