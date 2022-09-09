CREATE VIEW perm_spec_group_station_vw AS
SELECT psgsr.relation_id,
    psgsr.group_id,
    psgsr.perm_id,
    psgsr.station_id,
    psgsr.perm_define,
    pf.perm_name,
    pf.perm_type_name,
    pf.perm_type_id,
    pg.group_name,
    pg.group_level,
    pg.group_desc,
    t.station_name,
    t.station_label
   FROM conf.perm_spec_group_station_realation psgsr
     LEFT JOIN conf.perm_func_def pf ON psgsr.perm_id = pf.perm_id
     LEFT JOIN conf.perm_group_def pg ON psgsr.group_id = pg.group_id
     LEFT JOIN conf.station_tbl t ON psgsr.station_id = t.station_id



create view conf.perm_spec_group_station_vw as
select psgsr.*,
        pf.perm_name,
        pf.perm_type_name,
        pf.perm_type_id,
        pg.group_name,
        pg.group_level,
        pg.group_desc,
        t.station_name,
        t.station_label
  from conf.perm_spec_group_station_realation psgsr
  LEFT JOIN conf.perm_func_def pf on psgsr.perm_id = pf.perm_id
  LEFT JOIN conf.perm_group_def pg on psgsr.group_id = pg.group_id
  LEFT JOIN conf.station_tbl t on psgsr.station_id = t.station_id;