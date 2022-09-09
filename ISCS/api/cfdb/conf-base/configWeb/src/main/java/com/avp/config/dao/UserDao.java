package com.avp.config.dao;

import com.avp.config.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.repository.Repository;

public interface UserDao extends JpaRepository<User,Long>{
    public User save(User user);
}
