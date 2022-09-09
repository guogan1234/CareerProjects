package com.avp.config.dao.user.vw;

import com.avp.config.entity.user.vw.PermSpecGroupStationVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/28.
 */
public interface PermSpecGroupStationVwRepository extends JpaRepository<PermSpecGroupStationVw, String>, JpaSpecificationExecutor<PermSpecGroupStationVw> {
}
