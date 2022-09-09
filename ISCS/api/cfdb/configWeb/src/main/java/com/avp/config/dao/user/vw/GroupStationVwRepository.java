package com.avp.config.dao.user.vw;

import com.avp.config.entity.user.vw.GroupStationVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/26.
 */
public interface GroupStationVwRepository extends JpaRepository<GroupStationVw, String>, JpaSpecificationExecutor<GroupStationVw> {
}
