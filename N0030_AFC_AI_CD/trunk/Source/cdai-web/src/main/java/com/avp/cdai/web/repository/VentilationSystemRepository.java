package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.VentilationSystem;
import org.springframework.data.repository.CrudRepository;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/12.
 */
public interface VentilationSystemRepository extends CrudRepository<VentilationSystem,Integer>{
    List<VentilationSystem> findByTimestampBetween(Date start,Date end);
}
