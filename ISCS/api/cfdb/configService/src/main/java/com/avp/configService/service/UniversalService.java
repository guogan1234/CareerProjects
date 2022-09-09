package com.avp.configService.service;

import com.avp.configService.entity.vo.UniversalVo;
import com.avp.configService.entity.vo.ZyEntity;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;

import java.util.List;
import java.util.Map;

public interface UniversalService {
    public List<Map<String,Object>> getList(String sql);

    public Page<Map<String, Object>> getPage(String sql, Pageable pageable);

    public ZyEntity getPage(String tableName, int domainId, int currentPage, int size);

    public int execSql(String sql);
}
