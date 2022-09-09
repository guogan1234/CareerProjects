package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.PowerSystem;
import org.springframework.data.repository.CrudRepository;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/10.
 */
public interface PowerSystemRepository extends CrudRepository<PowerSystem,Integer>{
    List<PowerSystem> findAll();
    List<PowerSystem> findByTimestampBetween(Date start,Date end);
}
