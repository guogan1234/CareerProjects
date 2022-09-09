package com.avp.configService.dao;

import com.avp.configService.entity.User;
import org.springframework.cache.annotation.CacheConfig;
import org.springframework.cache.annotation.Cacheable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.List;
@CacheConfig(cacheNames = "users")
public interface UserDao extends JpaRepository<User,Long>{
    public User save(User user);
    @Cacheable
    public List<User>  findAllBy();
}
