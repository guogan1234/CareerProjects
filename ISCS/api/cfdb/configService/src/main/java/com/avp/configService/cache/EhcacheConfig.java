package com.avp.configService.cache;

import net.sf.ehcache.CacheManager;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.cache.ehcache.EhCacheCacheManager;
import org.springframework.cache.ehcache.EhCacheManagerFactoryBean;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.io.ClassPathResource;

@Configuration
@EnableCaching
public class EhcacheConfig {

    @Bean(name="ehCacheCacheManager")
    public EhCacheCacheManager ehCacheCacheManager(CacheManager cm){
        EhCacheCacheManager ehCacheCacheManager = new EhCacheCacheManager();
        ehCacheCacheManager.setTransactionAware(true);
        ehCacheCacheManager.setCacheManager(cm);
        return ehCacheCacheManager;
    }

    @Bean(name="ehCacheManagerFactoryBean")
    public EhCacheManagerFactoryBean ehCacheManagerFactoryBean(){
        String src = "ehcache.xml";
        EhCacheManagerFactoryBean ehFactoryBean =
                new EhCacheManagerFactoryBean();
        ehFactoryBean.setConfigLocation(new ClassPathResource(src));
        return ehFactoryBean;
    }
}
