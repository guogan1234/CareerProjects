package com.avp.config.service.impl;
import com.avp.config.dao.UserDao;
import com.avp.config.entity.User;
import com.avp.config.service.UserInf;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

@Service
@Transactional(rollbackFor=Exception.class)
public class UserImpl implements UserInf {

    @Autowired
    private UserDao userDao;

    @Override
    public User addUser(User user) throws Exception {
        User user1=userDao.save(user);
       return user1;
    }
}