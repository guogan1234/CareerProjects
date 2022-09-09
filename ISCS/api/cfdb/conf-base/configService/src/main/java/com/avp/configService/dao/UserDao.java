package com.avp.configService.dao;

import com.avp.configService.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;

import java.util.List;

public interface UserDao extends JpaRepository<User,Long>{
    public User save(User user);
    @Query(value = "select * from demo_user d where d.age=112",nativeQuery = true)
    public List<User> findAll();
}
