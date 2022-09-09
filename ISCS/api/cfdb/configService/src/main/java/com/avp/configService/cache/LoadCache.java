package com.avp.configService.cache;

import com.avp.configService.ConfigServiceApplication;
import com.avp.configService.dao.UniversalRepository;
import com.avp.configService.service.UserInf;
import com.avp.configService.service.impl.UniversalServiceImpl;
import com.avp.configService.util.SpringUtil;
import net.sf.ehcache.Cache;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.cache.ehcache.EhCacheCacheManager;
import org.springframework.context.ApplicationContext;
import org.springframework.stereotype.Component;


@Component
public class LoadCache implements CommandLineRunner{
    @Autowired
    private UserInf userInf;

    @Autowired
    private EhCacheCacheManager ehCacheCacheManager;

    @Override
    public void run(String... args) throws Exception {
//        userInf.getUsers();
//        Cache cache=ehCacheCacheManager.getCacheManager().getCache("users");
//
//        for(int i=0;i<cache.getKeys().size();i++){
//            System.out.println(cache.getKeys().get(i));
//        }

    }
}
