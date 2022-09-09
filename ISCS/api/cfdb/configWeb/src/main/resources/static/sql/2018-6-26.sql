	 CREATE OR REPLACE VIEW conf.perm_group_realation_vw AS
	 select gr.relation_id,gr.group_id,g.group_name,gr.perm_id,f.perm_name,gr.perm_define from conf.perm_group_realation gr
	 left join conf.perm_group_def g on gr.group_id=g.group_id
	 left join conf.perm_func_def f on gr.perm_id=f.perm_id and f.perm_type_name!='特殊权限'