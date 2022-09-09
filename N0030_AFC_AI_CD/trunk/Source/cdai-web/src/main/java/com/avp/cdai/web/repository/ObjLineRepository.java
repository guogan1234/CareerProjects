package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.ObjLine;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

import java.util.List;

/**
 * Created by guo on 2017/8/4.
 */
public interface ObjLineRepository extends CrudRepository<ObjLine,Integer> {
    List<ObjLine> findAll();
    ObjLine findById(@Param("id") Integer id);
    ObjLine findBylineName(@Param("name") String name);
    List<ObjLine> findBylineIdIn(@Param("lineIds") List<Integer> ids);
    ObjLine findByLineId(@Param("lineId")Integer lineId);
}
