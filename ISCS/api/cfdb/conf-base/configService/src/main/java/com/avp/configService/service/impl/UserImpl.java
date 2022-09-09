package com.avp.configService.service.impl;
import com.avp.configService.dao.UserDao;
import com.avp.configService.entity.User;
import com.avp.configService.service.UserInf;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
@Transactional(rollbackFor=Exception.class)
public class UserImpl implements UserInf {

    @Autowired
    private UserDao userDao;

    @Override
    public User addUser(User user){
        User user1=userDao.save(user);
       return user1;
    }

    @Override
    public List<User> getUsers() {
        return this.userDao.findAll();
    }


}