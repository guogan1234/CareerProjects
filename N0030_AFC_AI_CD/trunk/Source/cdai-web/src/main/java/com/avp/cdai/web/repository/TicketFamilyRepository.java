package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.TicketFamily;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

/**
 * Created by guo on 2017/12/11.
 */
public interface TicketFamilyRepository extends CrudRepository<TicketFamily,Integer> {
    List<TicketFamily> findAll();
}
