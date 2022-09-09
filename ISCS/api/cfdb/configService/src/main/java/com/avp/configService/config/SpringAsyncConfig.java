package com.avp.configService.config;

import org.springframework.beans.factory.annotation.Value;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.task.AsyncTaskExecutor;
import org.springframework.scheduling.annotation.EnableAsync;
import org.springframework.scheduling.concurrent.ThreadPoolTaskExecutor;

/**
 * Created by boris feng on 2018/6/1.
 */
@Configuration
@EnableAsync
public class SpringAsyncConfig {

    @Value("${iscs.confdb.threadpool.core-size:10}")
    private int corePoolSize;

    @Value("${iscs.confdb.threadpool.max-size:200}")
    private int maxPoolSize;

    @Value("${iscs.confdb.threadpool.queue-capacity:50}")
    private int queueCapacity;


    @Bean
    public AsyncTaskExecutor taskExecutor() {
        ThreadPoolTaskExecutor executor = new ThreadPoolTaskExecutor();
        executor.setCorePoolSize(corePoolSize);
        executor.setMaxPoolSize(maxPoolSize);
        executor.setQueueCapacity(queueCapacity);
        executor.setThreadNamePrefix("ConfDb-");
        executor.initialize();
        return executor;
    }
}