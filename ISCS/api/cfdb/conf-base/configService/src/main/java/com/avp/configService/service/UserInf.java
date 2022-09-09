package com.avp.configService.service;

import com.avp.configService.entity.User;

import java.util.List;

public interface UserInf {

    public User addUser(User user);

    public List<User> getUsers();
}
