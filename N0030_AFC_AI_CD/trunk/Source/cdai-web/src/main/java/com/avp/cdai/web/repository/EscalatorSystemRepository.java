package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.EscalatorSystem;
import org.springframework.data.repository.CrudRepository;

import java.util.Date;
import java.util.List;

/**
 * Created by guo on 2017/10/12.
 */
public interface EscalatorSystemRepository extends CrudRepository<EscalatorSystem,Integer>{
    List<EscalatorSystem> findByTimestampBetween(Date start,Date end);
}
