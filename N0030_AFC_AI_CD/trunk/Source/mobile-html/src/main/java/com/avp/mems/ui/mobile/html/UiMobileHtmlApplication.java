package com.avp.mems.ui.mobile.html;

import com.avp.mems.ui.mobile.html.conf.Interceptor;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.cloud.netflix.zuul.EnableZuulProxy;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.support.PropertySourcesPlaceholderConfigurer;
import org.springframework.core.env.PropertySources;
import org.springframework.web.servlet.config.annotation.InterceptorRegistry;
import org.springframework.web.servlet.config.annotation.ResourceHandlerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;


/**
 * Created by pw on 2017/4/25.
 */
//@EnableOAuth2Sso
@Configuration
//@Controller
//@EnableConfigurationProperties
@EnableZuulProxy
@SpringBootApplication
public class UiMobileHtmlApplication extends WebMvcConfigurerAdapter {

    @Qualifier("propertySourcesPlaceholderConfigurer")
    @Autowired
    PropertySourcesPlaceholderConfigurer placeholderConfigurer;

    public static void main(String[] args) {
        SpringApplication.run(UiMobileHtmlApplication.class, args);
    }

    @Override
    public void addInterceptors(InterceptorRegistry registry) {
        PropertySources propertySources = placeholderConfigurer.getAppliedPropertySources();
        System.out.println("--------------------------->"+propertySources.get("localProperties").getProperty("server.port"));
        registry.addInterceptor(new Interceptor(propertySources.get("localProperties")));
        super.addInterceptors(registry);
    }

    @Override
    public void addResourceHandlers(ResourceHandlerRegistry registry) {
        registry
                .addResourceHandler("/**")
                .addResourceLocations("classpath:/static/");
//                .addResourceLocations("classpath:/static/**");
        super.addResourceHandlers(registry);
    }

//
//    @Autowired
//    @Qualifier("defaultConversionService")
//    public void setGenericConversionService(GenericConversionService genericConversionService) {
//        //StringToBadComponentPK bcConverter = new StringToBadComponentPK();
//        //genericConversionService.addConverter(bcConverter);
//    }
//
//    @Bean
//    public static PropertySourcesPlaceholderConfigurer properties() {
//        PropertySourcesPlaceholderConfigurer propertySourcesPlaceholderConfigurer = new PropertySourcesPlaceholderConfigurer();
//        YamlPropertiesFactoryBean yaml = new YamlPropertiesFactoryBean();
//        yaml.setResources(new ClassPathResource("application.yml"));
//        propertySourcesPlaceholderConfigurer.setProperties(yaml.getObject());
//        return propertySourcesPlaceholderConfigurer;
//    }


}
