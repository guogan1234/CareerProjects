package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.CoreSwitchStatus;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

/**
 * Created by guo on 2017/10/20.
 */
public interface CoreSwitchRepository extends CrudRepository<CoreSwitchStatus,Integer>{
    @Query(value = "select * from core_switch_status order by update_time desc",nativeQuery = true)
    List<CoreSwitchStatus> getLatestData();
}
