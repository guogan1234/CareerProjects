package com.avp.cdai.web.repository;

import com.avp.cdai.web.entity.RefModule;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

import java.util.List;

/**
 * Created by guo on 2017/9/12.
 */
public interface RefModuleRepository extends CrudRepository<RefModule,Integer> {
    List<RefModule> findAll();

    @Query(value = "select * from ref_module_define",nativeQuery = true)
    List<Object[]> getAll();

    @Query(value = "select a.id,a.module_id,a.module_name,b.id as b_id,b.tag_name,b.tag_desc,b.module_id as b_module_id from ref_module_define a left join ref_module_subtag_define b on a.module_id=b.module_id",nativeQuery = true)
    List<Object[]> cascadeDatasLeft();

    //不支持right join的right关键字，只能用left join实现right join
    @Query(value = "select a.id,a.module_id,a.module_name,b.id as b_id,b.tag_name,b.tag_desc,b.module_id as b_module_id from ref_module_define a right join ref_module_subtag_define on a.module_id=b.module_id",nativeQuery = true)
    List<Object[]> cascadeDatasRight();

    //测试大数据量级联查询的效率
    //select a.id,a.ticket_id,a.direction,a.passenger_flow,a.section,b.id as b_id,b.ticket_id as b_ticket_id,b.family_id,b.ticket_cn_name from ticket_share_passenger_flow a left join ticket_type b on a.ticket_id=b.ticket_id
    //上面SQL语句在pgAdmin执行68*1000ms
    //方法调用执行了185*1000ms(有推送模块的影响)
    //方法调用执行了75*1000ms(无其他模块影响)
    @Query(value = "select a.id,a.ticket_id,a.direction,a.passenger_flow,a.section,b.id as b_id,b.ticket_id as b_ticket_id,b.family_id,b.ticket_cn_name from ticket_share_passenger_flow a left join ticket_type b on a.ticket_id=b.ticket_id",nativeQuery = true)
    List<Object[]> cascadeLargeData();
}
