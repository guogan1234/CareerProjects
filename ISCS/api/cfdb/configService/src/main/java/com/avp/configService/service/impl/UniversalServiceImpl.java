package com.avp.configService.service.impl;

import com.avp.configService.dao.UniversalRepository;
import com.avp.configService.entity.vo.CellVo;
import com.avp.configService.entity.vo.UniversalVo;
import com.avp.configService.entity.vo.ZyEntity;
import com.avp.configService.service.UniversalService;
import com.avp.configService.util.ReflectUtils;
import com.avp.configService.util.SpringUtil;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

@Service(value = "UniversalServiceImpl")
@Transactional(rollbackFor = Exception.class)
public class UniversalServiceImpl implements UniversalService {
    @Autowired
    private UniversalRepository universalRepository;

    @Override
    public List<Map<String, Object>> getList(String sql) {
        return universalRepository.findBySql(sql);
    }

    @Override
    public Page<Map<String, Object>> getPage(String sql, Pageable pageable) {
        Page<Map<String, Object>> page = this.universalRepository.findByPageSql(sql, pageable);
        return page;
    }

    @Override
    public ZyEntity getPage(String tableName, int domainId, int currentPage, int size) {
        //System.out.println("getPage:currentPage - " + currentPage + ",size - " + size);
        Pageable pageable = new PageRequest(currentPage, size);
        String sql = "select * from " + tableName;
        if (domainId != -1) {
            sql += " where domain_id=" + domainId;
        }
        Page<Map<String, Object>> page = this.universalRepository.findByPageSq2(sql, pageable);
        ZyEntity zyEntity = new ZyEntity();
        boolean dx = false;
        StringBuilder stringBuilder = new StringBuilder();
        String[] s = tableName.split("\\.");
        String sfix = s[0];
        String[] array = s[1].split("");
        for (int i = 0; i < array.length; i++) {
            if (i == 0) {
                stringBuilder.append(array[i].toUpperCase());
            } else {
                if ("_".equals(array[i])) {
                    dx = true;
                } else if (dx) {
                    stringBuilder.append(array[i].toUpperCase());
                    dx = false;
                } else {
                    stringBuilder.append(array[i]);
                }
            }
        }
        String entityName = sfix + "." + stringBuilder.toString();
        //得到对应的实体字段和数据类型的对应关系
        Map<String, Integer> typeToName = ReflectUtils.getEntityFieldType(stringBuilder.toString());

        List<Map<String, Object>> list = page.getContent();
        List<String> headers = ReflectUtils.getHeaders(typeToName);
        List<List<CellVo>> lists = new ArrayList<>();
        if (list != null && list.size() > 0) {
            for (int i = 0; i < list.size(); i++) {
                Map<String, Object> map2 = list.get(i);
                Iterator it = map2.keySet().iterator();
                List<CellVo> cellVos = new ArrayList<>();
                while (it.hasNext()) {
                    Object key = it.next();
                    int type = ReflectUtils.getType(String.valueOf(key), typeToName);
                    if (type != -1) {
                        CellVo cellVo = new CellVo();
                        cellVo.setName(String.valueOf(key));
                        cellVo.setType(type);
                        cellVo.setValue(map2.get(key));
                        cellVos.add(cellVo);
                    }
                }
                lists.add(cellVos);
            }
        }
        zyEntity.setHeaderList(headers);
        zyEntity.setLists(lists);
        zyEntity.setHasNext(page.hasNext());
        return zyEntity;
    }

    @Override
    public int execSql(String sql) {
        return this.universalRepository.execSql(sql);
    }
}
