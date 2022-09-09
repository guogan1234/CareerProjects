package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.AirConditionSystem;
import org.springframework.data.repository.CrudRepository;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/12.
 */
public interface AirConditionSystemRepository extends CrudRepository<AirConditionSystem,Integer>{
    List<AirConditionSystem> findByTimestampBetween(Date start,Date end);
}
