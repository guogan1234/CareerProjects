package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.LightingSystem;
import org.springframework.data.repository.CrudRepository;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/12.
 */
public interface LightingSystemRepository extends CrudRepository<LightingSystem,Integer>{
    List<LightingSystem> findByTimestampBetween(Date start, Date end);
}
