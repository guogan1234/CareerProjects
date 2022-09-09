package com.avp.configService;

import com.avp.configService.communicate.JTcpServerThread;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cache.annotation.EnableCaching;
import org.springframework.context.ApplicationContext;
import org.springframework.transaction.annotation.EnableTransactionManagement;

/**
 * 启动类
 * 开启事务
 */
@SpringBootApplication
@EnableTransactionManagement
@EnableCaching
public class ConfigServiceApplication {
	public static void main(String[] args) {
		System.out.println("SpringBoot app start...");
		ApplicationContext applicationContext=SpringApplication.run(ConfigServiceApplication.class, args);
		JTcpServerThread serverThread =new JTcpServerThread(applicationContext);
		serverThread.start();
	}
}
