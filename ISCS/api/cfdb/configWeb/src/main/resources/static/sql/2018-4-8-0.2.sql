
alter table conf.acc_limit_tbl drop column acc_tp_label;
alter table conf.acc_limit_tbl add point_tp_label character varying(32);

alter table conf.acc_def_tbl drop column acc_tp_label;
alter table conf.acc_def_tbl add point_tp_label character varying(32);