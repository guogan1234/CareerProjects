package com.avp.configService.dao;

import org.hibernate.*;
import org.hibernate.transform.Transformers;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.Pageable;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.stereotype.Repository;

import javax.persistence.EntityManager;
import java.util.List;
import java.util.Map;

@Repository
public class UniversalRepository {
    @Autowired
    private EntityManager entityManager;

    public List<Map<String, Object>> findBySql(String sql) {
        Session session = entityManager.unwrap(org.hibernate.Session.class);
        SQLQuery query = session.createSQLQuery(sql);
        query.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP);
        List<Map<String, Object>> result = query.list();
        return result;
    }

    public int execSql(String sql) {
        Session session = entityManager.unwrap(org.hibernate.Session.class);
        SQLQuery query = session.createSQLQuery(sql);
        return query.executeUpdate();
    }

    public Page<Map<String, Object>> findByPageSql(String sql, Pageable pageable) {
        Session session = entityManager.unwrap(org.hibernate.Session.class);
        SQLQuery query = session.createSQLQuery(sql);
        ScrollableResults scroll = query.scroll();
        scroll.last();
        int total = scroll.getRowNumber() + 1;
        query.setFirstResult(pageable.getOffset());
        query.setMaxResults(pageable.getPageSize());
        query.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP);
        Page<Map<String, Object>> page = new PageImpl<>(query.list(), pageable, total);
        return page;
    }

    public Page<Map<String, Object>> findByPageSq2(String sql, Pageable pageable) {
        Session session = entityManager.unwrap(org.hibernate.Session.class);
        SQLQuery query = session.createSQLQuery(sql);
        ScrollableResults scroll = query.scroll();
        scroll.last();
        int total = scroll.getRowNumber() + 1;
        query.setFirstResult(pageable.getOffset());
        query.setMaxResults(pageable.getPageSize());
        query.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP);
        Page<Map<String, Object>> page = new PageImpl<>(query.list(), pageable, total);
        return page;
    }
}
