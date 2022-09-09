create view conf.user_group_vw as
select ug.*,
        u.user_name,
        g.group_name from conf.perm_user_group_def ug  LEFT JOIN
   conf.perm_user_def u on u.user_id = ug.user_id
  LEFT JOIN conf.perm_group_def g on ug.group_id = g.group_id;


create view conf.group_station_vw as
SELECT gs.*,
        g.group_name,
        s.station_name
  from conf.perm_group_station_def gs
  LEFT JOIN conf.perm_group_def g on gs.group_id = g.group_id
  LEFT JOIN conf.station_tbl s on gs.station_id = s.station_id;