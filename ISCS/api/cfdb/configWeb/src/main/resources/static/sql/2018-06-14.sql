create view conf.alarm_source_act_type_vw as
  SELECT s.*,
          l.level_name,
          l.alarm_act,
          t.type_name
  from conf.alarm_source_tbl s
  LEFT JOIN conf.alarm_level_tbl l on s.alarm_level = l.level_id
    LEFT JOIN  conf.alarm_type_tbl t on s.alarm_type = t.type_id;