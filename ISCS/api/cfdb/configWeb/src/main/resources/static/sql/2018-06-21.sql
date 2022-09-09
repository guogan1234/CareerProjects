create view conf.alarm_info_domain_station_region_type_level_vw as
select i.*,
        t.type_name,
        d.domain_name,
        d.domain_type,
        s.station_label,
        s.station_name,
        r.pro_system_id,
        r.region_label,
        r.region_name,
        l.level_name,
        l.priority,
        l.sound_file,
        l.alarm_act
    from conf.alarm_info_tbl i
    LEFT JOIN conf.alarm_type_tbl t on i.alarm_type = t.type_id
    LEFT JOIN conf.domain_tbl d on i.domain_id = d.domain_id
    LEFT JOIN conf.station_tbl s on i.station_id = s.station_id
    LEFT JOIN conf.region_tbl r on i.region_id = r.region_id
    LEFT JOIN conf.alarm_level_tbl l on i.alarm_level = l.level_id;