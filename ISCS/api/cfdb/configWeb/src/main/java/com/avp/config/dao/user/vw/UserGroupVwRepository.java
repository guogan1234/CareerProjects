package com.avp.config.dao.user.vw;

import com.avp.config.entity.user.vw.UserGroupVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/26.
 */
public interface UserGroupVwRepository extends JpaRepository<UserGroupVw, String>, JpaSpecificationExecutor<UserGroupVw> {
}
