package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.RefModuleSubtag;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

/**
 * Created by guo on 2017/9/12.
 */
public interface RefModuleSubtagRepository extends CrudRepository<RefModuleSubtag,Integer> {
    List<RefModuleSubtag> findAll();
}
