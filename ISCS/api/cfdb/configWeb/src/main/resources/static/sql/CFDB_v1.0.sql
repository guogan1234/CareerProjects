create table conf."sys_db_node"
(
    "node_name" character varying(40) collate pg_catalog."default" not null,
    "node_type" integer,
    "service_name" character varying(16) collate pg_catalog."default",
    "user_name" character varying(16) collate pg_catalog."default",
    "user_password" character varying(16) collate pg_catalog."default",
    "cur_id" integer,
    "db_name" character varying(16) collate pg_catalog."default",
    "is_switch" integer,
    "host_prior" integer,
    "direct_access" integer,
    "sergrp_id" integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf."sys_db_node"
    owner to postgres;

create table conf."sys_domain_process_info"
(
    "proc_id" integer not null,
    "domain_id" integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf."sys_domain_process_info"
    owner to postgres;

-- table: conf.acc_def_tbl

-- drop table conf.acc_def_tbl;

create table conf.acc_def_tbl
(
    acc_def_label character varying(64) collate pg_catalog."default" not null,
    acc_base1 integer,
    acc_base2 integer,
    acc_base3 integer,
    acc_base4 integer,
    acc_coeff1 integer,
    acc_coeff2 integer,
    acc_coeff3 integer,
    acc_coeff4 integer,
    channel_num integer,
    channel_tag1 character varying(64) collate pg_catalog."default",
    channel_tag2 character varying(64) collate pg_catalog."default",
    channel_tag3 character varying(64) collate pg_catalog."default",
    channel_tag4 character varying(64) collate pg_catalog."default",
    dev_type integer,
    domain_id integer,
    dot_no1 integer,
    dot_no2 integer,
    dot_no3 integer,
    dot_no4 integer,
    location_id integer,
    pro_system_id integer,
    dev_type_label character varying(16) collate pg_catalog."default",
    operate_time timestamp without time zone,
    point_tp_label character varying(32) collate pg_catalog."default",
    constraint acc_def_tbl_pkey primary key (acc_def_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.acc_def_tbl
    owner to postgres;

-- table: conf.acc_limit_tbl

-- drop table conf.acc_limit_tbl;

create table conf.acc_limit_tbl
(
    acc_limit_label character varying(128) collate pg_catalog."default" not null,
    alarm_delay_time integer,
    alarm_level integer,
    domain_id integer,
    is_del_on_ack integer,
    limit_low1 integer,
    limit_low2 integer,
    limit_low3 integer,
    limit_num integer,
    limit_up1 integer,
    limit_up2 integer,
    limit_up3 integer,
    location_id integer,
    pic_name character varying(64) collate pg_catalog."default",
    pro_system_id integer,
    region_id integer,
    is_replace_alarm boolean,
    operate_time timestamp without time zone,
    point_tp_label character varying(32) collate pg_catalog."default",
    constraint acc_limit_tbl_pkey primary key (acc_limit_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.acc_limit_tbl
    owner to postgres;

-- table: conf.acc_tbl

-- drop table conf.acc_tbl;

create table conf.acc_tbl
(
    acc_label character varying(128) collate pg_catalog."default" not null,
    acc_name character varying(128) collate pg_catalog."default",
    acc_order integer,
    acc_status integer,
    acc_value integer,
    camera_preset character varying(64) collate pg_catalog."default",
    camera_tag character varying(64) collate pg_catalog."default",
    change_time bigint,
    dev_label character varying(96) collate pg_catalog."default",
    dev_type_label character varying(16) collate pg_catalog."default",
    domain_id integer,
    is_limit boolean,
    is_sample boolean,
    location_id integer,
    point_tp_label character varying(32) collate pg_catalog."default",
    pro_system_id integer,
    region_id integer,
    sample_deadband integer,
    unit_id integer,
    update_time bigint,
    po_integer_tp_label character varying(32) collate pg_catalog."default",
    operate_time timestamp without time zone,
    constraint acc_tbl_pkey primary key (acc_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.acc_tbl
    owner to postgres;

-- table: conf.acc_tp_tbl

-- drop table conf.acc_tp_tbl;

create table conf.acc_tp_tbl
(
    acc_tp_label character varying(32) collate pg_catalog."default" not null,
    acc_base1 integer,
    acc_base2 integer,
    acc_base3 integer,
    acc_base4 integer,
    acc_coeff1 integer,
    acc_coeff2 integer,
    acc_coeff3 integer,
    acc_coeff4 integer,
    acc_tp_name character varying(64) collate pg_catalog."default",
    acc_tp_order integer,
    alarm_delay_time integer,
    alarm_level integer,
    dev_tp_label character varying(32) collate pg_catalog."default",
    is_del_on_ack integer,
    is_limit boolean,
    is_replace_alarm boolean,
    is_sample boolean,
    limit_low1 integer,
    limit_low2 integer,
    limit_low3 integer,
    limit_num integer,
    limit_up1 integer,
    limit_up2 integer,
    limit_up3 integer,
    operate_time timestamp without time zone,
    sample_deadbande integer,
    status integer,
    unit_id integer,
    constraint acc_tp_tbl_pkey primary key (acc_tp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.acc_tp_tbl
    owner to postgres;

-- table: conf.ai_limit_tbl

-- drop table conf.ai_limit_tbl;

create table conf.ai_limit_tbl
(
    ai_limit_label character varying(128) collate pg_catalog."default" not null,
    alarm_delay_time integer,
    alarm_level integer,
    domain_id integer,
    is_del_on_ack integer,
    limit_low1 real,
    limit_low2 real,
    limit_low3 real,
    limit_num integer,
    limit_up1 real,
    limit_up2 real,
    limit_up3 real,
    location_id integer,
    pic_name character varying(64) collate pg_catalog."default",
    point_tp_label character varying(32) collate pg_catalog."default",
    pro_system_id integer,
    is_replace_alarm boolean,
    operate_time timestamp without time zone,
    constraint ai_limit_tbl_pkey primary key (ai_limit_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ai_limit_tbl
    owner to postgres;	
	
-- table: conf.ai_tbl

-- drop table conf.ai_tbl;

create table conf.ai_tbl
(
    ai_label character varying(128) collate pg_catalog."default" not null,
    ai_name character varying(128) collate pg_catalog."default",
    ai_order integer,
    ai_status integer,
    ai_value real,
    camera_preset character varying(64) collate pg_catalog."default",
    camera_tag character varying(64) collate pg_catalog."default",
    change_time bigint,
    is_control boolean,
    ctrl_trans_grp_label character varying(32) collate pg_catalog."default",
    dev_label character varying(96) collate pg_catalog."default",
    dev_type character varying(16) collate pg_catalog."default",
    domain_id integer,
    is_limit boolean,
    location_id integer,
    point_tp_label character varying(32) collate pg_catalog."default",
    point_type integer,
    pro_system_id integer,
    region_id integer,
    is_sample boolean,
    sample_deadband real,
    unit_id integer,
    update_time bigint,
    operate_time timestamp without time zone,
    constraint ai_tbl_pkey primary key (ai_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ai_tbl
    owner to postgres;	
	
-- table: conf.aio_def_tbl

-- drop table conf.aio_def_tbl;

create table conf.aio_def_tbl
(
    aio_def_label character varying(64) collate pg_catalog."default" not null,
    aio_base1 real,
    aio_base2 real,
    aio_base3 real,
    aio_base4 real,
    aio_coeff1 real,
    aio_coeff2 real,
    aio_coeff3 real,
    aio_coeff4 real,
    aio_deadband1 real,
    aio_deadband2 real,
    aio_deadband3 real,
    aio_deadband4 real,
    aio_percent1 integer,
    aio_percent2 integer,
    aio_percent3 integer,
    aio_percent4 integer,
    aio_type1 integer,
    aio_type2 integer,
    aio_type3 integer,
    aio_type4 integer,
    aio_zeroband1 real,
    aio_zeroband2 real,
    aio_zeroband3 real,
    aio_zeroband4 real,
    bottom_code1 integer,
    bottom_code2 integer,
    bottom_code3 integer,
    bottom_code4 integer,
    bottom_scale1 real,
    bottom_scale2 real,
    bottom_scale3 real,
    bottom_scale4 real,
    channel_tag1 character varying(64) collate pg_catalog."default",
    channel_tag2 character varying(64) collate pg_catalog."default",
    channel_tag3 character varying(64) collate pg_catalog."default",
    channel_tag4 character varying(64) collate pg_catalog."default",
    dev_type integer,
    domain_id integer,
    dot_no1 integer,
    dot_no2 integer,
    dot_no3 integer,
    dot_no4 integer,
    is_filter1 boolean,
    is_filter2 boolean,
    is_filter3 boolean,
    is_filter4 boolean,
    location_id integer,
    point_tp_label character varying(32) collate pg_catalog."default",
    pro_system_id integer,
    top_code1 integer,
    top_code2 integer,
    top_code3 integer,
    top_code4 integer,
    top_scale1 real,
    top_scale2 real,
    top_scale3 real,
    top_scale4 real,
    is_valid1 boolean,
    is_valid2 boolean,
    is_valid3 boolean,
    is_valid4 boolean,
    operate_time timestamp without time zone,
    dev_type_label character varying(16) collate pg_catalog."default",
    constraint aio_def_tbl_pkey primary key (aio_def_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.aio_def_tbl
    owner to postgres;

-- table: conf.aio_tp_tbl

-- drop table conf.aio_tp_tbl;

create table conf.aio_tp_tbl
(
    aio_tp_label character varying(32) collate pg_catalog."default" not null,
    aio_base1 real,
    aio_base2 real,
    aio_base3 real,
    aio_base4 real,
    aio_coeff1 real,
    aio_coeff2 real,
    aio_coeff3 real,
    aio_coeff4 real,
    aio_deadband1 real,
    aio_deadband2 real,
    aio_deadband3 real,
    aio_deadband4 real,
    aio_percent1 integer,
    aio_percent2 integer,
    aio_percent3 integer,
    aio_percent4 integer,
    aio_tp_name character varying(64) collate pg_catalog."default",
    aio_tp_order integer,
    aio_type1 integer,
    aio_type2 integer,
    aio_type3 integer,
    aio_type4 integer,
    aio_zeroband1 real,
    aio_zeroband2 real,
    aio_zeroband3 real,
    aio_zeroband4 real,
    alarm_delay_time integer,
    alarm_level integer,
    bottom_code1 integer,
    bottom_code2 integer,
    bottom_code3 integer,
    bottom_code4 integer,
    bottom_scale1 real,
    bottom_scale2 real,
    bottom_scale3 real,
    bottom_scale4 real,
    ctrl_base real,
    ctrl_bottom_code integer,
    ctrl_bottom_scale real,
    ctrl_coeff real,
    ctrl_data_type integer,
    ctrl_format integer,
    ctrl_time_out integer,
    ctrl_tolerance real,
    ctrl_top_code integer,
    ctrl_top_scale real,
    ctrl_wait_act integer,
    dev_tp_label character varying(32) collate pg_catalog."default",
    formula_tp_label character varying(32) collate pg_catalog."default",
    is_control boolean,
    is_del_on_ack integer,
    is_filter1 boolean,
    is_filter2 boolean,
    is_filter3 boolean,
    is_filter4 boolean,
    is_limit boolean,
    is_replace_alarm boolean,
    is_sample boolean,
    is_valid1 boolean,
    is_valid2 boolean,
    is_valid3 boolean,
    is_valid4 boolean,
    limit_low1 real,
    limit_low2 real,
    limit_low3 real,
    limit_num integer,
    limit_up1 real,
    limit_up2 real,
    limit_up3 real,
    operate_time timestamp without time zone,
    point_type integer,
    resv_time integer,
    sample_deadbande real,
    status integer,
    top_code1 integer,
    top_code2 integer,
    top_code3 integer,
    top_code4 integer,
    top_scale1 real,
    top_scale2 real,
    top_scale3 real,
    top_scale4 real,
    unit_id integer,
    constraint aio_tp_tbl_pkey primary key (aio_tp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.aio_tp_tbl
    owner to postgres;

-- table: conf.ao_tbl

-- drop table conf.ao_tbl;

create table conf.ao_tbl
(
    ao_label character varying(128) collate pg_catalog."default" not null,
    ao_base real,
    ao_coeff real,
    ao_format integer,
    ao_no integer,
    ao_tolerance real,
    bottom_code integer,
    bottom_scale real,
    data_type integer,
    exec_timeout integer,
    formula_label character varying(32) collate pg_catalog."default",
    resv_time integer,
    top_code integer,
    top_scale real,
    wait_act integer,
    operate_time timestamp without time zone,
    constraint ao_tbl_pkey primary key (ao_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ao_tbl
    owner to postgres;

-- table: conf.ctrl_trans_grp_tbl

-- drop table conf.ctrl_trans_grp_tbl;

create table conf.ctrl_trans_grp_tbl
(
    ctrl_trans_grp_label character varying(32) collate pg_catalog."default" not null,
    ctrl_trans_grp_name character varying(64) collate pg_catalog."default" not null,
    ctrl_trans_grp_plabel character varying(32) collate pg_catalog."default",
    domain_id integer not null,
    ex_cmp_label character varying(160) collate pg_catalog."default",
    ex_value integer not null,
    pro_system_id integer not null,
    region_id integer not null,
    station_id integer not null,
    trans_state integer not null,
    trans_timeout integer,
    constraint ctrl_trans_grp_tbl_pkey primary key (ctrl_trans_grp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ctrl_trans_grp_tbl
    owner to postgres;

-- table: conf.dev_node_tbl

-- drop table conf.dev_node_tbl;

create table conf.dev_node_tbl
(
    tag_name character varying(32) collate pg_catalog."default" not null,
    domain_id integer,
    location_id integer,
    node_no1 integer,
    node_no10 integer,
    node_no10status integer,
    node_no1status integer,
    node_no2 integer,
    node_no2status integer,
    node_no3 integer,
    node_no3status integer,
    node_no4 integer,
    node_no4status integer,
    node_no5 integer,
    node_no5status integer,
    node_no6 integer,
    node_no6status integer,
    node_no7 integer,
    node_no7status integer,
    node_no8 integer,
    node_no8status integer,
    node_no9 integer,
    node_no9status integer,
    region_id integer,
    system_id integer,
    topology_color integer,
    voltage_id integer,
    constraint dev_node_tbl_pkey primary key (tag_name)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dev_node_tbl
    owner to postgres;

-- table: conf.dev_tbl

-- drop table conf.dev_tbl;

create table conf.dev_tbl
(
    dev_label character varying(96) collate pg_catalog."default" not null,
    ctrl_trans_grp_label character varying(32) collate pg_catalog."default",
    dev_name character varying(32) collate pg_catalog."default",
    dev_status integer,
    dev_tp_label character varying(32) collate pg_catalog."default",
    dev_type_label character varying(16) collate pg_catalog."default",
    domain_id integer,
    is_summary integer,
    pro_system_id integer,
    region_id integer,
    rtu_id integer,
    station_id integer,
    summary_status integer,
    summary_value integer,
    operate_time timestamp without time zone,
    constraint dev_tbl_pkey primary key (dev_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dev_tbl
    owner to postgres;

-- table: conf.dev_token_info_tbl

-- drop table conf.dev_token_info_tbl;

create table conf.dev_token_info_tbl
(
    token_id integer not null,
    dev_label character varying(96) collate pg_catalog."default" not null,
    description character varying(128) collate pg_catalog."default",
    domain_id integer,
    host_name character varying(64) collate pg_catalog."default",
    op_time integer,
    station_id integer,
    user_grp_id integer,
    user_id integer,
    constraint dev_token_info_tbl_pkey primary key (token_id, dev_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dev_token_info_tbl
    owner to postgres;

-- table: conf.dev_tp_tbl

-- drop table conf.dev_tp_tbl;

create table conf.dev_tp_tbl
(
    dev_tp_label character varying(32) collate pg_catalog."default" not null,
    dev_tp_name character varying(64) collate pg_catalog."default",
    dev_type_label character varying(16) collate pg_catalog."default",
    operate_time timestamp without time zone,
    pro_system_id integer,
    status integer,
    constraint dev_tp_tbl_pkey primary key (dev_tp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dev_tp_tbl
    owner to postgres;

-- table: conf.dev_type_tbl

-- drop table conf.dev_type_tbl;

create table conf.dev_type_tbl
(
    dev_type_label character varying(16) collate pg_catalog."default" not null,
    dev_type_name character varying(32) collate pg_catalog."default",
    pro_system_id integer,
    constraint dev_type_tbl_pkey primary key (dev_type_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dev_type_tbl
    owner to postgres;

-- table: conf.di_tbl

-- drop table conf.di_tbl;

create table conf.di_tbl
(
    di_label character varying(128) collate pg_catalog."default" not null,
    alarm_delay_time integer,
    alarm_level integer,
    bit_delay_time integer,
    bit_num integer,
    camera_preset character varying(64) collate pg_catalog."default",
    camera_tag character varying(64) collate pg_catalog."default",
    change_time1 bigint,
    change_time2 bigint,
    change_time3 bigint,
    change_time4 bigint,
    change_time5 bigint,
    ctrl_trans_grp_label character varying(32) collate pg_catalog."default",
    dev_label character varying(96) collate pg_catalog."default",
    dev_type integer,
    di_name character varying(128) collate pg_catalog."default",
    di_order integer,
    di_status integer,
    di_status1 integer,
    di_status2 integer,
    di_status3 integer,
    di_status4 integer,
    di_status5 integer,
    di_text_label character varying(32) collate pg_catalog."default",
    di_value integer,
    di_value1 integer,
    di_value2 integer,
    di_value3 integer,
    di_value4 integer,
    di_value5 integer,
    domain_id integer,
    is_del_on_ack integer,
    is_replace_alarm boolean,
    is_sample boolean,
    is_summary integer,
    location_id integer,
    pic_name character varying(64) collate pg_catalog."default",
    point_tp_label character varying(32) collate pg_catalog."default",
    point_type integer,
    pro_system_id integer,
    region_id integer,
    summary_status integer,
    summary_value integer,
    update_time bigint,
    is_control boolean,
    dev_type_label character varying(255) collate pg_catalog."default",
    operate_time timestamp without time zone,
    constraint di_tbl_pkey primary key (di_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.di_tbl
    owner to postgres;
	
-- table: conf.di_text_tbl

-- drop table conf.di_text_tbl;

create table conf.di_text_tbl
(
    di_text_label character varying(32) collate pg_catalog."default" not null,
    alarm_mode integer,
    di_text character varying(32) collate pg_catalog."default",
    di_value integer,
    pro_system_id integer,
    constraint di_text_tbl_pkey primary key (di_text_label,di_value)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.di_text_tbl
    owner to postgres;
	
-- table: conf.dio_def_tbl

-- drop table conf.dio_def_tbl;

create table conf.dio_def_tbl
(
    dio_def_label character varying(64) collate pg_catalog."default" not null,
    bit_num integer,
    channel_tag1 character varying(64) collate pg_catalog."default",
    channel_tag2 character varying(1) collate pg_catalog."default",
    channel_tag3 character varying(64) collate pg_catalog."default",
    channel_tag4 character varying(64) collate pg_catalog."default",
    dev_type_label character varying(15) collate pg_catalog."default",
    dio_polarity2 integer,
    dio_polarity3 integer,
    dio_polarity4 integer,
    disturb_time1 integer,
    disturb_time2 integer,
    disturb_time3 integer,
    disturb_time4 integer,
    domain_id integer,
    dot1_no1 integer,
    dot1_no2 integer,
    dot1_no3 integer,
    dot1_no4 integer,
    dot1_no5 integer,
    dot2_no1 integer,
    dot2_no2 integer,
    dot2_no3 integer,
    dot2_no4 integer,
    dot2_no5 integer,
    dot3_no1 integer,
    dot3_no2 integer,
    dot3_no3 integer,
    dot3_no4 integer,
    dot3_no5 integer,
    dot4_no1 integer,
    dot4_no2 integer,
    dot4_no3 integer,
    dot4_no4 integer,
    dot4_no5 integer,
    filter_disturb1 boolean,
    filter_disturb2 boolean,
    filter_disturb3 boolean,
    filter_disturb4 boolean,
    filter_err1 boolean,
    filter_err2 boolean,
    filter_err3 boolean,
    filter_err4 boolean,
    location_id integer,
    operate_time timestamp without time zone,
    point_tp_label character varying(32) collate pg_catalog."default",
    polarity1 integer,
    pro_system_id integer,
    constraint dio_def_tbl_pkey primary key (dio_def_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dio_def_tbl
    owner to postgres;

-- table: conf.dio_tp_tbl

-- drop table conf.dio_tp_tbl;

create table conf.dio_tp_tbl
(
    dio_tp_label character varying(32) collate pg_catalog."default" not null,
    alarm_delay_time integer,
    alarm_level integer,
    bit_delay_time integer,
    bit_num integer,
    ctrl_grp_label character varying(32) collate pg_catalog."default",
    ctrl_timeout integer,
    ctrl_wait_act integer,
    dev_tp_label character varying(32) collate pg_catalog."default",
    di_text_label character varying(32) collate pg_catalog."default",
    dio_tp_name character varying(64) collate pg_catalog."default",
    dio_tp_order integer,
    disturb_time1 integer,
    disturb_time2 integer,
    disturb_time3 integer,
    disturb_time4 integer,
    do_num integer,
    filter_disturb1 boolean,
    filter_disturb2 boolean,
    filter_disturb3 boolean,
    filter_disturb4 boolean,
    filter_err1 boolean,
    filter_err2 boolean,
    filter_err3 boolean,
    filter_err4 boolean,
    formula_tp_label character varying(32) collate pg_catalog."default",
    is_control boolean,
    is_del_on_ack integer,
    is_replace_alarm boolean,
    is_sample boolean,
    is_summary boolean,
    operate_time timestamp without time zone,
    point_type character varying(255) collate pg_catalog."default",
    polarity1 integer,
    polarity2 integer,
    polarity3 integer,
    polarity4 integer,
    pulse_duration integer,
    resv_time integer,
    constraint dio_tp_tbl_pkey primary key (dio_tp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.dio_tp_tbl
    owner to postgres;

-- table: conf.do_grp_tbl

-- drop table conf.do_grp_tbl;

create table conf.do_grp_tbl
(
    do_order integer not null,
    do_grp_label character varying(32) collate pg_catalog."default" not null,
    do_allow integer,
    do_index integer,
    do_name character varying(32) collate pg_catalog."default",
    do_number integer,
    do_type integer,
    do_value integer,
    pro_system_id integer,
    constraint do_grp_tbl_pkey primary key (do_order, do_grp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.do_grp_tbl
    owner to postgres;

-- table: conf.do_tbl

-- drop table conf.do_tbl;

create table conf.do_tbl
(
    do_label character varying(128) collate pg_catalog."default" not null,
    do_grp_label character varying(32) collate pg_catalog."default",
    do_no1 integer,
    do_no2 integer,
    do_no3 integer,
    do_no4 integer,
    do_no5 integer,
    do_num integer,
    exec_timeout integer,
    formula_label character varying(32) collate pg_catalog."default",
    pulse_duration integer,
    resv_time integer,
    wait_act integer,
    operate_time timestamp without time zone,
    constraint do_tbl_pkey primary key (do_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.do_tbl
    owner to postgres;

-- table: conf.domain_tbl

-- drop table conf.domain_tbl;

create table conf.domain_tbl
(
    domain_id integer not null,
    domain_label character varying(16) collate pg_catalog."default",
    domain_name character varying(32) collate pg_catalog."default",
    domain_type integer,
    constraint domain_tbl_pkey primary key (domain_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.domain_tbl
    owner to postgres;
-- table: conf.formula_grp_tbl

-- drop table conf.formula_grp_tbl;

create table conf.formula_grp_tbl
(
    formula_grp_id integer not null,
    formula_grp_name character varying(32) collate pg_catalog."default",
    constraint formula_grp_tbl_pkey primary key (formula_grp_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.formula_grp_tbl
    owner to postgres;
	
-- table: conf.formula_param_tbl

-- drop table conf.formula_param_tbl;

create table conf.formula_param_tbl
(
    formula_param_order integer not null,
    formula_label character varying(32) collate pg_catalog."default" not null,
    domain_id integer,
    formula_param_label character varying(160) collate pg_catalog."default",
    constraint formula_param_tbl_pkey primary key (formula_param_order, formula_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.formula_param_tbl
    owner to postgres;

-- table: conf.formula_tbl

-- drop table conf.formula_tbl;

create table conf.formula_tbl
(
    formula_label character varying(32) collate pg_catalog."default" not null,
    formula_type integer,
    trigger_mode integer,
    description character varying(64) collate pg_catalog."default",
    cycle_time integer,
    param_num integer,
    formula_tp_label character varying(32) collate pg_catalog."default",
    result_label character varying(128) collate pg_catalog."default",
    exec_order integer,
    formula_grp_id integer,
    domain_id integer,
    constraint formula_tbl_pkey primary key (formula_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.formula_tbl
    owner to postgres;
	
-- table: conf.formula_tp_tbl

-- drop table conf.formula_tp_tbl;

create table conf.formula_tp_tbl
(
    formula_tp_label character varying(32) collate pg_catalog."default" not null,
    formula_text character varying(4000) collate pg_catalog."default",
    formula_tp_name character varying(64) collate pg_catalog."default",
    constraint formula_tp_tbl_pkey primary key (formula_tp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.formula_tp_tbl
    owner to postgres;

-- table: conf.node_conn_tbl

-- drop table conf.node_conn_tbl;

create table conf.node_conn_tbl
(
    tag_name character varying(32) collate pg_catalog."default" not null,
    node_no integer not null,
    para1 integer,
    para2 integer,
    target_nodeno integer not null,
    target_tagname character varying(32) collate pg_catalog."default" not null,
    constraint node_conn_tbl_pkey primary key (tag_name, node_no)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.node_conn_tbl
    owner to postgres;

-- table: conf.point_op_info_tbl

-- drop table conf.point_op_info_tbl;

create table conf.point_op_info_tbl
(
    point_label character varying(128) collate pg_catalog."default" not null,
    op_type integer not null,
    description character varying(128) collate pg_catalog."default",
    display_text character varying(32) collate pg_catalog."default",
    domain_id integer,
    force_value real,
    host_name character varying(64) collate pg_catalog."default",
    op_time integer,
    station_id integer,
    user_grp_id integer,
    user_id integer,
    constraint point_op_info_tbl_pkey primary key (point_label, op_type)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.point_op_info_tbl
    owner to postgres;

-- table: conf.pro_system_tbl

-- drop table conf.pro_system_tbl;

create table conf.pro_system_tbl
(
    pro_system_id integer not null,
    pro_system_label character varying(16) collate pg_catalog."default",
    pro_system_name character varying(32) collate pg_catalog."default",
    constraint pro_system_tbl_pkey primary key (pro_system_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pro_system_tbl
    owner to postgres;

-- table: conf.region_tbl

-- drop table conf.region_tbl;

create table conf.region_tbl
(
    region_id integer not null,
    pro_system_id integer,
    region_label character varying(16) collate pg_catalog."default",
    region_name character varying(32) collate pg_catalog."default",
    constraint region_tbl_pkey primary key (region_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.region_tbl
    owner to postgres;

-- table: conf.station_tbl

-- drop table conf.station_tbl;

create table conf.station_tbl
(
    station_id integer not null,
    station_label character varying(16) collate pg_catalog."default",
    station_name character varying(32) collate pg_catalog."default",
    station_pid integer,
    domain_id integer,
    constraint station_tbl_pkey primary key (station_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.station_tbl
    owner to postgres;

-- table: conf.sys_node_info

-- drop table conf.sys_node_info;

create table conf.sys_node_info
(
    node_id integer not null,
    actual_run_domain_id integer,
    cpu_used integer,
    last_report_time timestamp without time zone,
    location_id integer,
    mem_used integer,
    net1_status integer,
    net2_status integer,
    net3_status integer,
    net4_status integer,
    net_status integer,
    nic1_address character varying(16) collate pg_catalog."default",
    nic1_mask character varying(16) collate pg_catalog."default",
    nic1_name character varying(64) collate pg_catalog."default",
    nic2_address character varying(16) collate pg_catalog."default",
    nic2_mask character varying(16) collate pg_catalog."default",
    nic2_name character varying(64) collate pg_catalog."default",
    nic3_address character varying(16) collate pg_catalog."default",
    nic3_mask character varying(16) collate pg_catalog."default",
    nic3_name character varying(64) collate pg_catalog."default",
    nic4_address character varying(16) collate pg_catalog."default",
    nic4_mask character varying(16) collate pg_catalog."default",
    nic4_name character varying(64) collate pg_catalog."default",
    nic_num integer,
    node_dev character varying(64) collate pg_catalog."default",
    node_name character varying(40) collate pg_catalog."default",
    node_name_cn character varying(64) collate pg_catalog."default",
    node_os character varying(40) collate pg_catalog."default",
    node_status integer,
    node_type integer,
    start_order integer,
    constraint sys_node_info_pkey primary key (node_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.sys_node_info
    owner to postgres;

-- table: conf.sys_process_info

-- drop table conf.sys_process_info;

create table conf.sys_process_info
(
    proc_id integer not null,
    auto_run integer,
    command_name character varying(255) collate pg_catalog."default",
    command_para character varying(255) collate pg_catalog."default",
    cycle_period integer,
    cycle_start integer,
    description character varying(255) collate pg_catalog."default",
    duty_run integer,
    file_path character varying(255) collate pg_catalog."default",
    instance_num integer,
    not_used integer,
    occupy_context integer,
    offline_run integer,
    proc_alias character varying(255) collate pg_catalog."default",
    proc_type integer,
    report_type integer,
    run_order integer,
    run_style integer,
    standby_run integer,
    start_type integer,
    constraint sys_process_info_pkey primary key (proc_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.sys_process_info
    owner to postgres;

-- table: conf.table_ctrl_tbl

-- drop table conf.table_ctrl_tbl;

create table conf.table_ctrl_tbl
(
    table_name character varying(32) collate pg_catalog."default" not null,
    field_name character varying(64) collate pg_catalog."default" not null,
    field_type integer,
    is_archive boolean,
    description character varying(64) collate pg_catalog."default",
    constraint table_ctrl_tbl_pkey primary key (table_name, field_name)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.table_ctrl_tbl
    owner to postgres;

-- table: conf.unit_tbl

-- drop table conf.unit_tbl;

create table conf.unit_tbl
(
    unit_id integer not null,
    unit_name character varying(32) collate pg_catalog."default" not null,
    constraint unit_tbl_pkey primary key (unit_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.unit_tbl
    owner to postgres;

create table conf.alarm_act_tbl
(
    act_id integer not null,
    act_name character varying(32) collate pg_catalog."default" not null,
    constraint alarm_act_tbl_pkey primary key (act_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.alarm_act_tbl
    owner to postgres;

-- table: conf.alarm_info_tbl

-- drop table conf.alarm_info_tbl;

create table conf.alarm_info_tbl
(
    label character varying(128) collate pg_catalog."default" not null,
    second integer not null,
    msecond integer not null,
    alarm_type integer,
    domain_id integer,
    station_id integer,
    region_id integer,
    dev_type_label character varying(16) collate pg_catalog."default",
    ctrl_trans_grp_label character varying(32) collate pg_catalog."default",
    is_restrain boolean,
    pic_name character varying(64) collate pg_catalog."default",
    is_recovery boolean,
    is_del_on_ack integer,
    is_replace_alarm boolean,
    alarm_level integer,
    content character varying(256) collate pg_catalog."default",
    is_acked boolean,
    constraint alarm_info_tbl_pkey primary key (label, second, msecond)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.alarm_info_tbl
    owner to postgres;	
	
-- table: conf.alarm_level_tbl

-- drop table conf.alarm_level_tbl;

create table conf.alarm_level_tbl
(
    level_id integer not null,
    level_name character varying(32) collate pg_catalog."default",
    priority integer,
    color character varying(8) collate pg_catalog."default",
    sound_file character varying(64) collate pg_catalog."default",
    alarm_act character varying(128),
    constraint alarm_level_tbl_pkey primary key (level_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.alarm_level_tbl
    owner to postgres;

-- table: conf.alarm_source_tbl

-- drop table conf.alarm_source_tbl;

create table conf.alarm_source_tbl
(
    source_id integer not null,
    source_name character varying(32) collate pg_catalog."default",
    alarm_type integer,
    alarm_level integer,
    is_replace_alarm boolean,
    is_del_on_ack integer,
    constraint alarm_source_tbl_pkey primary key (source_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.alarm_source_tbl
    owner to postgres;

-- table: conf.alarm_type_tbl

-- drop table conf.alarm_type_tbl;

create table conf.alarm_type_tbl
(
    type_id integer not null,
    type_name character varying(32) collate pg_catalog."default",
    constraint alarm_type_tbl_pkey primary key (type_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.alarm_type_tbl
    owner to postgres;

-- table: conf.bas_tt_content_tbl

-- drop table conf.bas_tt_content_tbl;

create table conf.bas_tt_content_tbl
(
    tt_label character varying(64) collate pg_catalog."default" not null,
    "order" integer not null,
    mode_info integer,
    station_id integer,
    domain_id integer,
    constraint bas_tt_content_tbl_pkey primary key (tt_label, "order")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_content_tbl
    owner to postgres;

-- table: conf.bas_tt_date_sche_tbl

-- drop table conf.bas_tt_date_sche_tbl;

create table conf.bas_tt_date_sche_tbl
(
    station_id integer not null,
    tt_label character varying(64) collate pg_catalog."default",
    domain_id integer,
    date date,
    constraint bas_tt_date_sche_tbl_pkey primary key (station_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_date_sche_tbl
    owner to postgres;

-- table: conf.bas_tt_def_tbl

-- drop table conf.bas_tt_def_tbl;

create table conf.bas_tt_def_tbl
(
    label character varying(64) collate pg_catalog."default" not null,
    name character varying(128) collate pg_catalog."default",
    id integer,
    version integer,
    sequence integer,
    length integer,
    station_id integer,
    domain_id integer,
    constraint bas_tt_def_tbl_pkey primary key (label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_def_tbl
    owner to postgres;

-- table: conf.bas_tt_info_tbl

-- drop table conf.bas_tt_info_tbl;

create table conf.bas_tt_info_tbl
(
    station_id integer not null,
    rtu_id integer,
    max_seq_no integer,
    max_ver_no integer,
    download_timeout integer,
    read_timeout integer,
    sche_download_time integer,
    id_seq_plabel character varying(160) collate pg_catalog."default",
    ver_plabel character varying(160) collate pg_catalog."default",
    download_status integer,
    read_status integer,
    cur_tt_label character varying(64) collate pg_catalog."default",
    cur_tt_id integer,
    cur_tt_name character varying(128) collate pg_catalog."default",
    cur_tt_ver integer,
    cur_tt_seq integer,
    cur_tt_length integer,
    domain_id integer,
    constraint bas_tt_info_tbl_pkey primary key (station_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_info_tbl
    owner to postgres;

comment on column conf.bas_tt_info_tbl.sche_download_time
    is '从0点开始的秒数';

comment on column conf.bas_tt_info_tbl.download_status
    is '0 成功  1 失败 2 进行中';

comment on column conf.bas_tt_info_tbl.read_status
    is '0 成功  1 失败 2 进行中';

-- table: conf.bas_tt_mode_tbl

-- drop table conf.bas_tt_mode_tbl;

create table conf.bas_tt_mode_tbl
(
    label character varying(32) collate pg_catalog."default" not null default null::character varying,
    name character varying(64) collate pg_catalog."default" default null::character varying,
    station_id integer,
    domain_id integer,
    subsys_lable character varying(32) collate pg_catalog."default",
    mode_no integer,
    constraint bas_tt_mode_tbl_pkey primary key (label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_mode_tbl
    owner to postgres;

-- table: conf.bas_tt_run_content_tbl

-- drop table conf.bas_tt_run_content_tbl;

create table conf.bas_tt_run_content_tbl
(
    tt_label character varying(64) collate pg_catalog."default" not null,
    "order" integer not null,
    mode_info integer,
    station_id integer,
    domain_id integer,
    constraint bas_tt_run_content_tbl_pkey primary key (tt_label, "order")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_run_content_tbl
    owner to postgres;

-- table: conf.bas_tt_subsys_tbl

-- drop table conf.bas_tt_subsys_tbl;

create table conf.bas_tt_subsys_tbl
(
    label character varying(32) collate pg_catalog."default" not null,
    name character varying(64) collate pg_catalog."default",
    station_id integer,
    domain_id integer,
    constraint bas_tt_subsys_tbl_pkey primary key (label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_subsys_tbl
    owner to postgres;

-- table: conf.bas_tt_weekly_sche_tbl

-- drop table conf.bas_tt_weekly_sche_tbl;

create table conf.bas_tt_weekly_sche_tbl
(
    station_id integer not null,
    sun_tt_label character varying(64) collate pg_catalog."default",
    "mon_ tt_label" character varying(64) collate pg_catalog."default",
    "tues_ tt_label" character varying(64) collate pg_catalog."default",
    "wed_ tt_label" character varying(64) collate pg_catalog."default",
    "thur_ tt_label" character varying(64) collate pg_catalog."default",
    "fri_ tt_label" character varying(64) collate pg_catalog."default",
    "sat_ tt_label" character varying(64) collate pg_catalog."default",
    domain_id integer,
    constraint bas_tt_weekly_sche_tbl_pkey primary key (station_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.bas_tt_weekly_sche_tbl
    owner to postgres;

-- table: conf.calc_ana

-- drop table conf.calc_ana;

create table conf.calc_ana
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "description" character varying(100) collate pg_catalog."default" not null,
    "value" integer,
    "status" integer,
    "if_limit" integer,
    "region_id" integer not null,
    "if_sample" integer,
    "sample_deadband" integer,
    "domain_id" integer not null,
    "location_id" integer not null,
    "name" character varying(32) collate pg_catalog."default" not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.calc_ana
    owner to postgres;	
	
-- table: conf.calc_dig

-- drop table conf.calc_dig;

create table conf.calc_dig
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "description" character varying(100) collate pg_catalog."default",
    "value" integer,
    "status" integer,
    "state_text_name" character varying(40) collate pg_catalog."default",
    "region_id" integer not null,
    "alarm_priority" integer,
    "pic_name" character varying(64) collate pg_catalog."default",
    "alarm_delay_time" integer,
    "if_water_alm" integer,
    "if_ack_on_rtn" integer,
    "del_act_on_ack" integer,
    "if_never_alm_on_rtn" integer,
    "if_sample" integer,
    "domain_id" integer not null,
    "location_id" integer not null,
    "name" character varying(32) collate pg_catalog."default" not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.calc_dig
    owner to postgres;

-- table: conf.calc_limit_info

-- drop table conf.calc_limit_info;

create table conf.calc_limit_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null default null::character varying,
    "record_app" integer not null,
    "limit_type" integer,
    "limit_num" integer,
    "limit_up1" integer,
    "limit_low1" integer,
    "limit_up2" integer,
    "limit_low2" integer,
    "limit_up3" integer,
    "limit_low3" integer,
    "cross_percent" integer,
    "return_percent" integer,
    "cross_time" date,
    "alarm_priority" integer,
    "pic_name" character varying(64) collate pg_catalog."default" default null::character varying,
    "alarm_delay_time" integer,
    "if_water_alm" integer,
    "if_ack_on_rtn" integer,
    "del_act_on_ack" integer,
    "if_never_alm_on_rtn" integer,
    "point_tp_name" character varying(64) collate pg_catalog."default" default null::character varying,
    "domain_id" integer not null,
    "location_id" integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.calc_limit_info
    owner to postgres;

-- table: conf.cctv_camera_tbl

-- drop table conf.cctv_camera_tbl;

create table conf.cctv_camera_tbl
(
    camera_labe character varying(64) collate pg_catalog."default" not null,
    camera_no integer,
    camera_type integer,
    description character varying(100) collate pg_catalog."default",
    camera_ip character varying(32) collate pg_catalog."default",
    domain_id integer,
    station_id integer,
    constraint cctv_camera_tbl_pkey primary key (camera_labe)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.cctv_camera_tbl
    owner to postgres;

-- table: conf.cctv_monitor_tbl

-- drop table conf.cctv_monitor_tbl;

create table conf.cctv_monitor_tbl
(
    monitor_label character varying(64) collate pg_catalog."default" not null,
    monitor_no integer,
    "monitor_type " integer,
    "description " character varying(100) collate pg_catalog."default",
    div_mode integer,
    separator_label character varying(64) collate pg_catalog."default",
    separator_label4 character varying(64) collate pg_catalog."default",
    separator_label9 character varying(64) collate pg_catalog."default",
    domain_id integer,
    station_id integer,
    "video_source_key_id_tag " character varying(100) collate pg_catalog."default",
    video_source_type integer,
    constraint cctv_monitor_tbl_pkey primary key (monitor_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.cctv_monitor_tbl
    owner to postgres;

-- table: conf.cctv_orientation_tbl

-- drop table conf.cctv_orientation_tbl;

create table conf.cctv_orientation_tbl
(
    orientation_label character varying(64) collate pg_catalog."default" not null,
    orientation_id integer,
    description character varying(100) collate pg_catalog."default",
    camera_label character varying(64) collate pg_catalog."default",
    domain_id integer,
    station_id integer,
    constraint cctv_orientation_tbl_pkey primary key (orientation_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.cctv_orientation_tbl
    owner to postgres;

-- table: conf.cctv_separator_info

-- drop table conf.cctv_separator_info;

create table conf.cctv_separator_info
(
    separator_label character varying(64) collate pg_catalog."default" not null,
    separator_id integer,
    "separator_type " integer,
    "description " character varying(100) collate pg_catalog."default",
    "domain_id " integer,
    "station_id " integer,
    "video_tag1 " character varying(64) collate pg_catalog."default",
    "video_tag2 " character varying(64) collate pg_catalog."default",
    "video_tag3 " character varying(64) collate pg_catalog."default",
    "video_tag4 " character varying(64) collate pg_catalog."default",
    "video_tag5 " character varying(64) collate pg_catalog."default",
    "video_tag6 " character varying(64) collate pg_catalog."default",
    "video_tag7 " character varying(64) collate pg_catalog."default",
    "video_tag8 " character varying(64) collate pg_catalog."default",
    "video_tag9 " character varying(64) collate pg_catalog."default",
    "video_type1 " integer,
    "video_type2 " integer,
    "video_type3 " integer,
    "video_type4 " integer,
    "video_type5 " integer,
    "video_type6 " integer,
    "video_type7 " integer,
    "video_type8 " integer,
    "video_type9 " integer,
    "if_full_sequence " integer,
    "full_sequence_key_id_tag " character varying(64) collate pg_catalog."default",
    "sequence_order " integer,
    constraint cctv_separator_info_pkey primary key (separator_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.cctv_separator_info
    owner to postgres;

-- table: conf.cctv_sequence_tbl

-- drop table conf.cctv_sequence_tbl;

create table conf.cctv_sequence_tbl
(
    sequence_label character varying(64) collate pg_catalog."default" not null default null::character varying,
    sequence_id integer,
    sequence_type integer,
    description character varying(100) collate pg_catalog."default" default null::character varying,
    operator_id integer,
    operator_node_name character varying(64) collate pg_catalog."default" default null::character varying,
    operate_time integer,
    request_id integer,
    camera_num integer,
    camera_showtime integer,
    camera_tag1 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag2 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag3 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag4 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag5 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag6 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag7 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag8 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag9 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag10 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag11 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag12 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag13 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag14 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag15 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag16 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag17 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag18 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag19 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag20 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag21 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag22 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag23 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag24 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag25 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag26 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag27 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag28 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag29 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag30 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag31 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag32 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag33 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag34 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag35 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag36 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag37 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag38 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag39 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag40 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag41 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag42 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag43 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag44 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag45 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag46 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag47 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag48 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag49 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag50 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag51 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag52 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag53 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag54 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag55 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag56 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag57 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag58 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag59 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag60 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag61 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag62 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag63 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag64 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag65 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag66 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag67 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag68 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag69 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag70 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag71 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag72 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag73 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag74 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag75 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag76 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag77 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag78 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag79 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag80 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag81 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag82 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag83 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag84 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag85 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag86 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag87 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag88 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag89 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag90 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag91 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag92 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag93 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag94 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag95 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag96 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag97 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag98 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag99 character varying(64) collate pg_catalog."default" default null::character varying,
    camera_tag100 character varying(64) collate pg_catalog."default" default null::character varying,
    domain_id integer,
    station_id integer,
    constraint cctv_sequence_tbl_pkey primary key (sequence_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.cctv_sequence_tbl
    owner to postgres;
-- table: conf.color_strategy

-- drop table conf.color_strategy;

create table conf.color_strategy
(
    strategy_name character varying(64) collate pg_catalog."default" not null,
    strategy_value integer not null,
    line_color_a character varying(64) collate pg_catalog."default",
    line_color_b character varying(64) collate pg_catalog."default",
    fill_color_a character varying(64) collate pg_catalog."default",
    fill_color_b character varying(64) collate pg_catalog."default",
    icon_plane integer,
    "if_twinkle" integer,
    twinkle_frequency integer,
    "show_text" character varying(64) collate pg_catalog."default",
    "if_hide" integer,
    constraint color_strategy_pkey primary key (strategy_name)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.color_strategy
    owner to postgres;
	
-- table: conf.fes_ai_tbl

-- drop table conf.fes_ai_tbl;

create table conf.fes_ai_tbl
(
    ai_label character varying(64) collate pg_catalog."default" not null,
    "app_id" integer,
    fes_ai_id integer,
    domain_id integer,
    ai_no integer,
    ai_desc character varying(64) collate pg_catalog."default",
    "app_table" character varying(64) collate pg_catalog."default",
    "app_labelname" character varying(64) collate pg_catalog."default",
    "app_valuename" character varying(64) collate pg_catalog."default",
    ai_value integer,
    status integer,
    coeff real,
    "offset" real,
    deadband real,
    zeroband real,
    "up_limit" real,
    "down_limit" real,
    "raw_code" integer,
    "change_time" bigint,
    "update_time" bigint,
    "valid" integer,
    para1 integer,
    para2 integer,
    constraint fes_ai_tbl_pkey primary key (ai_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_ai_tbl
    owner to postgres;
	
-- table: conf.fes_channal

-- drop table conf.fes_channal;

create table conf.fes_channal
(
    tag_name character varying(255) collate pg_catalog."default",
    channal_id integer,
    channel_desc character varying(64) collate pg_catalog."default",
    domain_id integer,
    location_id integer,
    g_tag character varying(64) collate pg_catalog."default",
    r_tag character varying(64) collate pg_catalog."default",
    chan_type integer,
    app_id integer,
    prot_type integer,
    ip_addr character varying(16) collate pg_catalog."default",
    port integer,
    listen_port integer,
    com_no integer,
    baud_rate integer,
    data_bit integer,
    stop_bit integer,
    verify_type integer,
    chan_status integer,
    para1 integer,
    para2 integer,
    para3 integer,
    para4 integer,
    para5 integer,
    para6 integer,
    para7 integer,
    para8 integer,
    para9 integer,
    para10 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_channal
    owner to postgres;

comment on column conf.fes_channal.chan_type
    is '0：无效
1：串口
2：tcp客户端
3：tcp服务端
4：udp
';

-- table: conf.fes_di_tbl

-- drop table conf.fes_di_tbl;

create table conf.fes_di_tbl
(
    "di_label" character varying(64) collate pg_catalog."default" not null,
    "app_id" integer,
    fes_di_id integer,
    domain_id integer,
    "di_no" integer,
    di_desc character varying(64) collate pg_catalog."default",
    "app_table" character varying(64) collate pg_catalog."default",
    "app_labelname" character varying(64) collate pg_catalog."default",
    "app_valuename" character varying(64) collate pg_catalog."default",
    "di_value" integer,
    status integer,
    "if_invert" integer,
    "raw_code" integer,
    "change_time" bigint,
    "update_time" bigint,
    "disturb_time" integer,
    "if_filter" integer,
    para1 integer,
    para2 integer,
    constraint fes_di_tbl_pkey primary key ("di_label")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_di_tbl
    owner to postgres;
	
-- table: conf.fes_group

-- drop table conf.fes_group;

create table conf.fes_group
(
    tag_name character varying(255) collate pg_catalog."default",
    group_name character varying(255) collate pg_catalog."default",
    domain_id integer,
    location_id integer,
    app_id integer,
    group_desc character varying(64) collate pg_catalog."default",
    run_mode integer,
    para1 integer,
    para2 integer,
    para3 integer,
    para4 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_group
    owner to postgres;

comment on column conf.fes_group.run_mode
    is '0：主备
1：集群
';

-- table: conf.fes_pi_tbl

-- drop table conf.fes_pi_tbl;

create table conf.fes_pi_tbl
(
    pi_label character varying(64) collate pg_catalog."default" not null,
    "app_id" integer,
    fes_pi_id integer,
    domain_id integer,
    pi_no integer,
    pi_desc character varying(64) collate pg_catalog."default",
    "app_table" character varying(64) collate pg_catalog."default",
    "app_labelname" character varying(64) collate pg_catalog."default",
    "app_valuename" character varying(64) collate pg_catalog."default",
    pi_value integer,
    status integer,
    coeff real,
    "offset" real,
    "raw_code" integer,
    para1 integer,
    para2 integer,
    "change_time" bigint,
    "update_time" bigint,
    constraint fes_pi_tbl_pkey primary key (pi_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_pi_tbl
    owner to postgres;
	
-- table: conf.fes_prot

-- drop table conf.fes_prot;

create table conf.fes_prot
(
    prot_id integer,
    prot_type integer,
    prot_name character varying(16) collate pg_catalog."default",
    prot_desc character varying(64) collate pg_catalog."default",
    para_file character varying(16) collate pg_catalog."default",
    para1 integer,
    para2 integer,
    para3 integer,
    para4 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_prot
    owner to postgres;
	
-- table: conf.fes_rtu

-- drop table conf.fes_rtu;

create table conf.fes_rtu
(
    tag_name character varying(255) collate pg_catalog."default",
    rtu_id integer,
    domain_id integer,
    location_id integer,
    app_id integer,
    rtu_desc character varying(64) collate pg_catalog."default",
    g_tag character varying(64) collate pg_catalog."default",
    rtu_type integer,
    rtu_addr integer,
    dig_block integer,
    ana_block integer,
    acc_block integer,
    ctrl_block integer,
    para1 integer,
    para2 integer,
    para3 integer,
    para4 integer,
    para5 integer,
    para6 integer,
    para7 integer,
    para8 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.fes_rtu
    owner to postgres;

comment on column conf.fes_rtu.rtu_type
    is '0：接收
1：转发
2：接收+转发
';

comment on column conf.fes_rtu.dig_block
    is '0：不封锁
1：封锁
';

comment on column conf.fes_rtu.ana_block
    is '0：不封锁
1：封锁
';

comment on column conf.fes_rtu.acc_block
    is '0：不封锁
1：封锁
';

comment on column conf.fes_rtu.ctrl_block
    is '0：不封锁
1：封锁
';

-- table: conf.formula_info

-- drop table conf.formula_info;

create table conf.formula_info
(
    "formula_name" character varying(64) collate pg_catalog."default" not null,
    "time_slice" integer,
    "formula_str" character varying(4000) collate pg_catalog."default",
    "groups" integer not null,
    "category" integer not null,
    "compute_order" integer,
    "output_app_key_id_tag" character varying(100) collate pg_catalog."default" not null,
    "domain_id" integer not null,
    "input_operand_num" integer not null,
    "description" character varying(100) collate pg_catalog."default"
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.formula_info
    owner to postgres;	
	
-- table: conf.formula_para_def

-- drop table conf.formula_para_def;

create table conf.formula_para_def
(
    "formula_tag" character varying(64) collate pg_catalog."default" not null,
    "input_app_key_id_tag" character varying(100) collate pg_catalog."default" not null,
    "para_id" integer not null,
    "domain_id" integer not null,
    "input_statics_info" character varying(64) collate pg_catalog."default"
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.formula_para_def
    owner to postgres;

	
-- table: conf."grp_conresult"

-- drop table conf."grp_conresult";

create table conf."grp_conresult"
(
    "grp_id" integer,
    "yk_tagname" character varying(64) collate pg_catalog."default",
    "yk_sort" integer,
    "yk_value" integer,
    "action_type" integer,
    "yk_condition" integer,
    "time" integer,
    "resulttype" integer,
    "result" integer,
    para1 integer,
    para2 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf."grp_conresult"
    owner to postgres;

-- table: conf.grp_control

-- drop table conf.grp_control;

create table conf.grp_control
(
    "grp_id" integer,
    "yk_tagname" character varying(64) collate pg_catalog."default",
    "yk_sort" integer,
    "yk_value" integer,
    "action_type" integer,
    "yk_condition" character varying(64) collate pg_catalog."default",
    "yk_condition1" integer,
    "interval" integer,
    "icount" integer,
    para1 integer,
    para2 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.grp_control
    owner to postgres;

-- table: conf.hmi_type_info

-- drop table conf.hmi_type_info;

create table conf.hmi_type_info
(
    hmi_id integer not null,
    hmi_name character varying(64) collate pg_catalog."default" not null,
    hmi_desc character varying(128) collate pg_catalog."default" not null,
    hmi_define character varying(64) collate pg_catalog."default" not null,
    constraint hmi_type_info_pkey primary key (hmi_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.hmi_type_info
    owner to postgres;

-- table: conf.linkage_action

-- drop table conf.linkage_action;

create table conf.linkage_action
(
    "linkage_id" integer not null,
    action_id integer not null,
    "des" character varying(64) collate pg_catalog."default",
    "actionobj" character varying(64) collate pg_catalog."default" not null,
    "actionvalue" integer not null,
    "sync" integer not null,
    "if_checked" integer not null,
    "before_conditon" integer not null,
    "after_conditon" integer not null,
    "result" integer not null,
    "fail_deal" character varying(64) collate pg_catalog."default" not null,
    "retry_num" integer not null,
    "delay" integer not null,
    "command" character varying(64) collate pg_catalog."default" not null,
    "time" date not null,
    constraint linkage_action_pkey primary key ("linkage_id", action_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_action
    owner to postgres;
	
-- table: conf.linkage_action_after_conditio

-- drop table conf.linkage_action_after_conditio;

create table conf.linkage_action_after_conditio
(
    action_id integer not null,
    "after_condition_id" integer not null,
    "desc" character varying(64) collate pg_catalog."default",
    "furmula_id" character varying(64) collate pg_catalog."default" not null,
    "furmula_value" integer not null,
    constraint linkage_action_after_conditio_pkey primary key (action_id, "after_condition_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_action_after_conditio
    owner to postgres;

-- table: conf.linkage_action_before_conditio

-- drop table conf.linkage_action_before_conditio;

create table conf.linkage_action_before_conditio
(
    action_id integer not null,
    "before_condition_id" integer not null,
    "desc" character varying(64) collate pg_catalog."default",
    "furmula_id" character varying(64) collate pg_catalog."default" not null,
    "furmula_value" integer not null,
    constraint linkage_action_before_conditio_pkey primary key (action_id, "before_condition_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_action_before_conditio
    owner to postgres;

-- table: conf.linkage_action_record

-- drop table conf.linkage_action_record;

create table conf.linkage_action_record
(
    "linkage_id" integer not null,
    action_id integer not null,
    "des" character varying(64) collate pg_catalog."default",
    "actionobj" character varying(64) collate pg_catalog."default" not null,
    "actionvalue" integer not null,
    "sync" integer not null,
    "before_conditon" integer not null,
    "after_conditon" integer not null,
    result integer not null,
    "retry_num" integer not null,
    delay integer not null,
    "time" date not null,
    constraint linkage_action_record_pkey primary key ("linkage_id", action_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_action_record
    owner to postgres;
	
-- table: conf.linkage_condition

-- drop table conf.linkage_condition;

create table conf.linkage_condition
(
    "linkage_id" integer not null,
    condition_id integer not null,
    "desc" character varying(64) collate pg_catalog."default",
    "furmula_id" character varying(64) collate pg_catalog."default" not null,
    "furmula_value" integer not null,
    constraint linkage_condition_pkey primary key ("linkage_id", condition_id)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_condition
    owner to postgres;

-- table: conf.linkage_event

-- drop table conf.linkage_event;

create table conf.linkage_event
(
    "linkage_id" integer not null,
    "desc" character varying(64) collate pg_catalog."default",
    domain_id integer not null,
    "linkage_type" integer,
    action_type integer not null,
    "conditon_tab" integer not null,
    "linkage_trig" integer not null,
    "cur_step" integer not null,
    "status" character varying(64) collate pg_catalog."default" not null,
    "result" integer not null,
    "usr_id" character varying(64) collate pg_catalog."default" not null,
    "time" date not null,
    "result_time" date not null,
    constraint linkage_event_pkey primary key ("linkage_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_event
    owner to postgres;

-- table: conf.linkage_record

-- drop table conf.linkage_record;

create table conf.linkage_record
(
    "linkage_id" integer not null,
    "desc" character varying(64) collate pg_catalog."default",
    domain_id integer not null,
    "linkage_type" integer not null,
    action_type integer not null,
    "conditon_tab" integer not null,
    step_num integer not null,
    result integer not null,
    "usr_id" character varying(64) collate pg_catalog."default" not null,
    "time" date not null,
    "result_time" date not null,
    constraint linkage_record_pkey primary key ("linkage_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_record
    owner to postgres;

-- table: conf.linkage_trig

-- drop table conf.linkage_trig;

create table conf.linkage_trig
(
    "linkage_id" integer not null,
    "trip_id" integer not null,
    "desc" character varying(64) collate pg_catalog."default",
    "trigobj" character varying(64) collate pg_catalog."default" not null,
    "trigobj_type" integer not null,
    "trigprop" character varying(64) collate pg_catalog."default" not null,
    "time" date not null,
    constraint linkage_trig_pkey primary key ("linkage_id", "trip_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.linkage_trig
    owner to postgres;

-- table: conf.mq_exchange_domain

-- drop table conf.mq_exchange_domain;

create table conf.mq_exchange_domain
(
    id integer not null,
    src_domain_id integer not null,
    dest_domain_id integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.mq_exchange_domain
    owner to postgres;

-- table: conf.mq_host

-- drop table conf.mq_host;

create table conf.mq_host
(
    id integer not null,
    host_name character varying(50) collate pg_catalog."default" not null,
    transport_connector_protocol character varying(10) collate pg_catalog."default" not null,
    transport_connector_ip character varying(50) collate pg_catalog."default" not null,
    transport_connector_port integer not null,
    transport_heartbeat integer not null,
    exchage_connector_protocol character varying(10) collate pg_catalog."default" not null,
    exchage_connector_ip character varying(50) collate pg_catalog."default" not null,
    exchage_connector_port integer not null,
    exchage_heartbeat integer not null,
    sentinel_protocol character varying(10) collate pg_catalog."default" not null,
    sentinel_ip character varying(50) collate pg_catalog."default" not null,
    sentinel_port integer not null,
    sentinel_heartbeat integer not null,
    sync_connector_protocol character varying(10) collate pg_catalog."default" not null,
    sync_connector_ip character varying(50) collate pg_catalog."default" not null,
    sync_connector_port integer not null,
    sync_connector_heartbeat integer not null,
    queue_max_messages integer not null,
    dlq_max_messages integer not null,
    sergrp_id integer not null,
    domain_id integer not null,
    realm_id integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.mq_host
    owner to postgres;
	
-- table: conf.mq_realm

-- drop table conf.mq_realm;

create table conf.mq_realm
(
    id integer not null,
    "user" character varying(50) collate pg_catalog."default" not null,
    passwd character varying(200) collate pg_catalog."default" not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.mq_realm
    owner to postgres;

-- table: conf.nms_mng_node_info

-- drop table conf.nms_mng_node_info;

create table conf.nms_mng_node_info
(
    "node_name" character varying(40) collate pg_catalog."default" not null,
    "location_id" integer not null,
    "description" character varying(64) collate pg_catalog."default" not null,
    "sergrp_id" integer,
    "domain" character varying(64) collate pg_catalog."default" not null,
    "node_type" integer,
    "is_used" integer,
    "nic_num" integer,
    "netmask" character varying(16) collate pg_catalog."default",
    "nic1_name" character varying(64) collate pg_catalog."default",
    "nic1_address" character varying(16) collate pg_catalog."default",
    "nic2_name" character varying(64) collate pg_catalog."default",
    "nic2_address" character varying(16) collate pg_catalog."default"
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_mng_node_info
    owner to postgres;

-- table: conf.nms_node_app_info

-- drop table conf.nms_node_app_info;

create table conf.nms_node_app_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "node_name" character varying(40) collate pg_catalog."default" not null,
    "description" character varying(100) collate pg_catalog."default" not null,
    "location_id" integer not null,
    "app_priority" integer,
    "app_status" character varying(40) collate pg_catalog."default",
    "sergrp_id" integer,
    "domain_id" integer not null,
    "domain" integer not null,
    "app_name" character varying(64) collate pg_catalog."default" not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_node_app_info
    owner to postgres;

-- table: conf.nms_node_disk_info

-- drop table conf.nms_node_disk_info;

create table conf.nms_node_disk_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "node_name" character varying(40) collate pg_catalog."default" not null,
    "disk_index" integer not null,
    "disk_name" character varying(80) collate pg_catalog."default",
    "disk_size" integer,
    "disk_usage" integer,
    "disk_io" integer,
    "domain_id" integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_node_disk_info
    owner to postgres;

-- table: conf.nms_node_info

-- drop table conf.nms_node_info;

create table conf.nms_node_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "node_name" character varying(40) collate pg_catalog."default" not null,
    "location_id" integer not null,
    "description" character varying(100) collate pg_catalog."default" not null,
    "cpu_num" integer,
    "cpu_usage" integer,
    "mem_count" integer,
    "mem_usage" integer,
    "nic1_status" integer,
    "nic2_status" integer,
    "domain_id" integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_node_info
    owner to postgres;
	
-- table: conf.nms_node_proc_info

-- drop table conf.nms_node_proc_info;

create table conf.nms_node_proc_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "node_name" character varying(40) collate pg_catalog."default" not null,
    "proc_index" integer not null,
    "proc_id" integer,
    "proc_name" character varying(32) collate pg_catalog."default",
    "proc_instance" character varying(40) collate pg_catalog."default",
    "proc_status" character varying(32) collate pg_catalog."default",
    "domain_id" integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_node_proc_info
    owner to postgres;

-- table: conf.nms_switch_info

-- drop table conf.nms_switch_info;

create table conf.nms_switch_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "description" character varying(100) collate pg_catalog."default" not null,
    "location_id" integer not null,
    "type " integer not null,
    "ip_address" character varying(16) collate pg_catalog."default" not null,
    "netmask" character varying(16) collate pg_catalog."default",
    "runtime" character varying(64) collate pg_catalog."default",
    "cup_usage" integer,
    "mem_size" integer,
    "mem_used" integer,
    "temperature " integer,
    "domain_id" integer not null
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_switch_info
    owner to postgres;
	
-- table: conf.nms_switch_port_info

-- drop table conf.nms_switch_port_info;

create table conf.nms_switch_port_info
(
    "tag_name" character varying(64) collate pg_catalog."default" not null,
    "switch_name" character varying(64) collate pg_catalog."default" not null,
    "description" character varying(100) collate pg_catalog."default" not null,
    "location_id" integer not null,
    "port_no" integer not null,
    "status" integer,
    "in_bcpkts" integer,
    "in_unbcpkts" integer,
    "in_discards" integer,
    "out_bcpkts" integer,
    "out_unbcpkts" integer,
    "out_discards" integer,
    "domain_id" integer not null,
    "in_rate" integer,
    "out_rate" integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.nms_switch_port_info
    owner to postgres;
	
-- table: conf.pa_audio_grp

-- drop table conf.pa_audio_grp;

create table conf.pa_audio_grp
(
    grp_label character varying(64) collate pg_catalog."default" not null,
    grp_id integer,
    description character varying(100) collate pg_catalog."default",
    area_seq character varying(800) collate pg_catalog."default",
    domain_id integer,
    station_id integer,
    sta_seq character varying(200) collate pg_catalog."default",
    audio_id integer,
    audio_priority integer,
    constraint pa_audio_grp_pkey primary key (grp_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pa_audio_grp
    owner to postgres;
	
-- table: conf.pa_audio_tbl

-- drop table conf.pa_audio_tbl;

create table conf.pa_audio_tbl
(
    audio_label character varying(64) collate pg_catalog."default" not null,
    audio_no integer,
    audio_type integer,
    description character varying(100) collate pg_catalog."default",
    audio_text character varying(800) collate pg_catalog."default",
    audio_priority integer,
    domain_id integer,
    station_id integer,
    audio_length integer,
    constraint pa_audio_tbl_pkey primary key (audio_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pa_audio_tbl
    owner to postgres;	
-- table: conf.pa_loop_tbl

-- drop table conf.pa_loop_tbl;

create table conf.pa_loop_tbl
(
    loop_label character varying(64) collate pg_catalog."default",
    loop_id integer,
    description character varying(100) collate pg_catalog."default",
    audio_num integer,
    audio_id1 integer,
    audio_id2 integer,
    audio_id3 integer,
    audio_id4 integer,
    audio_id5 integer,
    audio_id6 integer,
    audio_id7 integer,
    audio_id8 integer,
    audio_id9 integer,
    audio_id10 integer,
    audio_id11 integer,
    audio_id12 integer,
    audio_id13 integer,
    audio_id14 integer,
    audio_id15 integer,
    audio_id16 integer,
    audio_type1 integer,
    audio_type2 integer,
    audio_type3 integer,
    audio_type4 integer,
    audio_type5 integer,
    audio_type6 integer,
    audio_type7 integer,
    audio_type8 integer,
    audio_type9 integer,
    audio_type10 integer,
    audio_type11 integer,
    audio_type12 integer,
    audio_type13 integer,
    audio_type14 integer,
    audio_type15 integer,
    audio_type16 integer,
    domain_id integer,
    station_id integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pa_loop_tbl
    owner to postgres;	

-- table: conf.pa_shot_tbl

-- drop table conf.pa_shot_tbl;

create table conf.pa_shot_tbl
(
    shot_label character varying(64) collate pg_catalog."default" not null,
    shot_id integer,
    description character varying(100) collate pg_catalog."default",
    audio_tts_id integer,
    audio_tts_type integer,
    domain_id integer,
    station_id integer,
    constraint pa_shot_tbl_pkey primary key (shot_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pa_shot_tbl
    owner to postgres;	

-- table: conf.pa_time_tbl

-- drop table conf.pa_time_tbl;

create table conf.pa_time_tbl
(
    time_label character varying(64) collate pg_catalog."default" not null,
    time_id integer,
    description character varying(100) collate pg_catalog."default",
    time_status integer,
    audio_tts_rpt_id integer,
    audio_tts_rpt_type integer,
    start_time integer,
    stop_time integer,
    interval_time integer,
    week character varying(32) collate pg_catalog."default",
    rpt_mode integer,
    area_seq character varying(800) collate pg_catalog."default",
    domain_id integer,
    station_id integer,
    operator_id integer,
    operator_priority integer,
    operator_node_name character varying(64) collate pg_catalog."default",
    operate_time integer,
    request_id integer,
    linked_pis_status integer,
    start_date integer,
    stop_date integer,
    sta_seq character varying(200) collate pg_catalog."default",
    constraint pa_time_tbl_pkey primary key (time_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pa_time_tbl
    owner to postgres;
	
-- table: conf.pa_tts_tbl

-- drop table conf.pa_tts_tbl;

create table conf.pa_tts_tbl
(
    tts_label character varying(64) collate pg_catalog."default" not null,
    tts_id integer,
    tts_type integer,
    description character varying(100) collate pg_catalog."default",
    tts_text character varying(800) collate pg_catalog."default",
    tts_priority integer,
    domain_id integer,
    station_id integer,
    tts_length integer,
    constraint pa_tts_tbl_pkey primary key (tts_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pa_tts_tbl
    owner to postgres;	
	
-- table: conf."perm_department_def "

-- drop table conf."perm_department_def ";

create table conf."perm_department_def "
(
    "dept_id" integer not null,
    "dept_name" character varying(64) collate pg_catalog."default" not null,
    "delong_dept_id" integer,
    "dept_desc" character varying(100) collate pg_catalog."default",
    constraint "perm_department_def _pkey" primary key ("dept_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf."perm_department_def "
    owner to postgres;	
-- table: conf.perm_func_def

-- drop table conf.perm_func_def;

create table conf.perm_func_def
(
    "perm_id" integer not null,
    "perm_name" character varying(64) collate pg_catalog."default" not null,
    "perm_type_name" character varying(32) collate pg_catalog."default" not null,
    "perm_define" character varying(32) collate pg_catalog."default" not null,
    constraint perm_func_def_pkey primary key ("perm_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_func_def
    owner to postgres;	

-- table: conf.perm_group_def

-- drop table conf.perm_group_def;

create table conf.perm_group_def
(
    "group_id" integer not null,
    "group_name" character varying(64) collate pg_catalog."default" not null,
    "group_level" integer,
    "group_desc" character varying(100) collate pg_catalog."default",
    "group_pic1" character varying(64) collate pg_catalog."default",
    "group_pic2" character varying(64) collate pg_catalog."default",
    "group_pic3" character varying(64) collate pg_catalog."default",
    constraint perm_group_def_pkey primary key ("group_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_group_def
    owner to postgres;	
	

-- table: conf.perm_group_realation

-- drop table conf.perm_group_realation;

create table conf.perm_group_realation
(
    "relation_id" character varying(32) collate pg_catalog."default" not null,
    "group_id" integer not null,
    "perm_id" integer not null,
    "perm_define" character varying(32) collate pg_catalog."default" not null,
    constraint perm_group_realation_pkey primary key ("relation_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_group_realation
    owner to postgres;	
	
-- table: conf.perm_group_station_def

-- drop table conf.perm_group_station_def;

create table conf.perm_group_station_def
(
    "relation_id" character varying(32) collate pg_catalog."default" not null,
    "group_id" integer not null,
    "station_id" integer not null,
    constraint perm_group_station_def_pkey primary key ("relation_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_group_station_def
    owner to postgres;

-- table: conf.perm_security_def

-- drop table conf.perm_security_def;

create table conf.perm_security_def
(
    "perm_security_id" integer not null,
    "perm_password_min_length" integer,
    "perm_password_min_letr" integer,
    "perm_password_min_num" integer,
    "perm_account_max_lock" integer,
    "perm_account_lock_timer" integer,
    "perm_account_invalid_timer" integer,
    constraint perm_security_def_pkey primary key ("perm_security_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_security_def
    owner to postgres;
	
-- table: conf.perm_spec_group_region_realation

-- drop table conf.perm_spec_group_region_realation;

create table conf.perm_spec_group_region_realation
(
    "relation_id" character varying(32) collate pg_catalog."default" not null,
    "group_id" integer not null,
    "perm_id" integer not null,
    "region_id" integer not null,
    "perm_define" character varying(32) collate pg_catalog."default" not null,
    constraint perm_spec_group_region_realation_pkey primary key ("relation_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_spec_group_region_realation
    owner to postgres;

-- table: conf.perm_spec_group_station_realation

-- drop table conf.perm_spec_group_station_realation;

create table conf.perm_spec_group_station_realation
(
    "relation_id" character varying(32) collate pg_catalog."default" not null,
    "group_id" integer not null,
    "perm_id" integer not null,
    "station_id" integer not null,
    "perm_define" character varying(32) collate pg_catalog."default" not null,
    constraint perm_spec_group_station_realation_pkey primary key ("relation_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_spec_group_station_realation
    owner to postgres;

-- table: conf.perm_type_def

-- drop table conf.perm_type_def;

create table conf.perm_type_def
(
    "perm_type_id" integer not null,
    "perm_type_name" character varying(32) collate pg_catalog."default" not null,
    constraint perm_type_def_pkey primary key ("perm_type_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_type_def
    owner to postgres;

-- table: conf.perm_user_def

-- drop table conf.perm_user_def;

create table conf.perm_user_def
(
    "user_id" integer not null,
    "user_name" character varying(64) collate pg_catalog."default" not null,
    "user_alias" character varying(64) collate pg_catalog."default",
    "user_password" character varying(64) collate pg_catalog."default" not null,
    "user_desc" character varying(100) collate pg_catalog."default",
    "user_if_lock" integer,
    "user_lock_time" date,
    "user_create_date" date,
    "user_expire_date" date,
    constraint perm_user_def_pkey primary key ("user_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_user_def
    owner to postgres;

-- table: conf.perm_user_department_def

-- drop table conf.perm_user_department_def;

create table conf.perm_user_department_def
(
    "relation_id" character varying(32) collate pg_catalog."default" not null,
    "user_id" integer not null,
    "dept_id" integer not null,
    constraint perm_user_department_def_pkey primary key ("relation_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_user_department_def
    owner to postgres;

-- table: conf.perm_user_group_def

-- drop table conf.perm_user_group_def;

create table conf.perm_user_group_def
(
    "relation_id" character varying(32) collate pg_catalog."default" not null,
    "user_id" integer not null,
    "group_id" integer not null,
    "if_lead" integer,
    constraint perm_user_group_def_pkey primary key ("relation_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.perm_user_group_def
    owner to postgres;

-- table: conf.pis_custom_tbl

-- drop table conf.pis_custom_tbl;

create table conf.pis_custom_tbl
(
    custom_label character varying(64) collate pg_catalog."default" not null,
    custom_id integer,
    custom_type integer,
    description character varying(100) collate pg_catalog."default",
    custom_text character varying(800) collate pg_catalog."default",
    custom_priority integer,
    domain_id integer,
    station_id integer,
    custom_length integer,
    constraint pis_custom_tbl_pkey primary key (custom_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pis_custom_tbl
    owner to postgres;

-- table: conf.pis_display_tbl

-- drop table conf.pis_display_tbl;

create table conf.pis_display_tbl
(
    display_label character varying(64) collate pg_catalog."default" not null,
    display_id integer,
    description character varying(100) collate pg_catalog."default",
    display_status integer,
    msg_id integer,
    msg_type integer,
    msg_text character varying(800) collate pg_catalog."default",
    start_time integer,
    stop_time integer,
    interval_time integer,
    area_seq character varying(800) collate pg_catalog."default",
    destination integer,
    domain_id integer,
    station_id integer,
    operator_id integer,
    operator_priority integer,
    operator_node_name character varying(64) collate pg_catalog."default",
    operate_time integer,
    request_id integer,
    verifier_id integer,
    verifier_priority integer,
    sta_seq character varying(200) collate pg_catalog."default",
    constraint pis_display_tbl_pkey primary key (display_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pis_display_tbl
    owner to postgres;

-- table: conf.pis_expect_tbl

-- drop table conf.pis_expect_tbl;

create table conf.pis_expect_tbl
(
    expect_label character varying(64) collate pg_catalog."default" not null,
    expect_no integer,
    expect_type integer,
    description character varying(100) collate pg_catalog."default",
    expect_text character varying(800) collate pg_catalog."default",
    expect_priority integer,
    domain_id integer,
    station_id integer,
    expect_length integer,
    constraint pis_expect_tbl_pkey primary key (expect_label)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.pis_expect_tbl
    owner to postgres;



-- table: conf."prog_conresult"

-- drop table conf."prog_conresult";

create table conf."prog_conresult"
(
    "prog_id" integer,
    "grp_id" integer,
    "yk_tagname" character varying(64) collate pg_catalog."default",
    "yk_sort" integer,
    "yk_value" integer,
    "action_type" integer,
    "yk_condition" integer,
    "time" integer,
    "resulttype" integer,
    "result" integer,
    para1 integer,
    para2 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf."prog_conresult"
    owner to postgres;

-- table: conf.prog_control

-- drop table conf.prog_control;

create table conf.prog_control
(
    "prog_id" integer,
    "grp_id" integer,
    "yk_tagname" character varying(64) collate pg_catalog."default",
    "yk_sort" integer,
    "yk_value" integer,
    "action_type" integer,
    "yk_condition" character varying(64) collate pg_catalog."default",
    "yk_condition1" integer,
    "interval" integer,
    "icount" integer,
    para1 integer,
    para2 integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.prog_control
    owner to postgres;

-- table: conf.sys_menu

-- drop table conf.sys_menu;

create table conf.sys_menu
(
    display_value character varying(64) collate pg_catalog."default" not null,
    actual_value integer not null,
    menu_name character varying(64) collate pg_catalog."default" not null,
    macro character varying(64) collate pg_catalog."default" not null,
    constraint sys_menu_pkey primary key (actual_value, menu_name)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.sys_menu
    owner to postgres;

-- table: conf.zf_ai_tbl

-- drop table conf.zf_ai_tbl;

create table conf.zf_ai_tbl
(
    "domain_id" integer not null,
    "label_name" character varying(64) collate pg_catalog."default",
    "desc" character varying(64) collate pg_catalog."default",
    "location_id" integer,
    "zf_rtu" character varying(64) collate pg_catalog."default",
    "zf_type" integer,
    "zf_value" real,
    "zf_status" integer,
    "zf_no" integer,
    "zf_source_label" character varying(64) collate pg_catalog."default",
    "para1" integer,
    "para2" integer,
    "para3" integer,
    "para4" integer,
    "para5" integer,
    "para6" integer,
    "para7" integer,
    "para8" integer,
    constraint zf_ai_tbl_pkey primary key ("domain_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.zf_ai_tbl
    owner to postgres;

comment on column conf.zf_ai_tbl."zf_type"
    is '0 float
1 int
2 其他1
3 其他2
4 其他3
';

-- table: conf.zf_ctrl_tbl

-- drop table conf.zf_ctrl_tbl;

create table conf.zf_ctrl_tbl
(
    "domain_id" integer not null,
    "tag_name" character varying(64) collate pg_catalog."default",
    "location_id" integer,
    "zf_rtu" character varying(64) collate pg_catalog."default",
    "zf_type" integer,
    "zf_value" integer,
    "zf_status" integer,
    "zf_no" integer,
    "zf_source_tag" character varying(64) collate pg_catalog."default",
    "para1" integer,
    "para2" integer,
    "para3" integer,
    "para4" integer,
    "para5" integer,
    "para6" integer,
    "para7" integer,
    "para8" integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.zf_ctrl_tbl
    owner to postgres;

-- table: conf.zf_di_tbl

-- drop table conf.zf_di_tbl;

create table conf.zf_di_tbl
(
    "domain_id" integer not null,
    "label_name" character varying(64) collate pg_catalog."default",
    "desc_dig" character varying(64) collate pg_catalog."default",
    "location_id" integer,
    "zf_rtu" character varying(64) collate pg_catalog."default",
    "zf_type" integer,
    "zf_value" integer,
    "zf_status" integer,
    "zf_no" integer,
    "zf_source_label" character varying(64) collate pg_catalog."default",
    "para1" integer,
    "para2" integer,
    "para3" integer,
    "para4" integer,
    "para5" integer,
    "para6" integer,
    "para7" integer,
    "para8" integer,
    constraint zf_di_tbl_pkey primary key ("domain_id")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.zf_di_tbl
    owner to postgres;

comment on column conf.zf_di_tbl."zf_type"
    is '0 单位
1 双位
2 三位
3 四位
4 五位
';

-- table: conf.zf_pi_tbl

-- drop table conf.zf_pi_tbl;

create table conf.zf_pi_tbl
(
    "domain_id" integer not null,
    "label_name" character varying(64) collate pg_catalog."default",
    "location_id" integer,
    "zf_rtu" character varying(64) collate pg_catalog."default",
    "zf_type" integer,
    "zf_value" integer,
    "zf_status" integer,
    "zf_no" integer,
    "zf_source_label" character varying(64) collate pg_catalog."default",
    "para1" integer,
    "para2" integer,
    "para3" integer,
    "para4" integer,
    "para5" integer,
    "para6" integer,
    "para7" integer,
    "para8" integer
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.zf_pi_tbl
    owner to postgres;	
	
	
	
-- table: conf.ats_actual_schedule

-- drop table conf.ats_actual_schedule;

create table conf.ats_actual_schedule
(
    "tag_name" character varying(64) collate pg_catalog."default" not null default null::character varying,
    "schedule_data" character varying(64) collate pg_catalog."default" not null default null::character varying,
    direction integer not null,
    station character varying(64) collate pg_catalog."default" not null default null::character varying,
    "arrive_time" date not null,
    depart_time date not null,
    "serial_no" integer not null,
    constraint ats_actual_schedule_pkey primary key ("tag_name")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ats_actual_schedule
    owner to postgres;	
	
-- table: conf.ats_online_schedule

-- drop table conf.ats_online_schedule;

create table conf.ats_online_schedule
(
    "tag_name" character varying(64) collate pg_catalog."default" not null default null::character varying,
    "schedule_data" character varying(64) collate pg_catalog."default" not null default null::character varying,
    direction integer not null,
    station character varying(64) collate pg_catalog."default" not null default null::character varying,
    "arrive_time" date not null,
    depart_time date not null,
    "serial_no" integer not null,
    constraint ats_online_schedule_pkey primary key ("tag_name")
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ats_online_schedule
    owner to postgres;	
	
-- table: conf.ats_train_info

-- drop table conf.ats_train_info;

create table conf.ats_train_info
(
    pvid integer not null,
    tid integer not null,
    direction integer not null,
    section character varying(64) collate pg_catalog."default" not null,
    "cur_station" character varying(64) collate pg_catalog."default",
    "next_station" character varying(64) collate pg_catalog."default",
    "destination" integer,
    speed integer not null,
    "if_hold" integer not null,
    "if_skip" integer not null,
    "if_eb" integer not null,
    "if_instorage" integer not null,
    fire_alarm_status integer,
    block_alarm_status integer,
    constraint ats_train_info_pkey primary key (tid)
)
with (
    oids = false
)
tablespace pg_default;

alter table conf.ats_train_info
    owner to postgres;	
	
		
-- table: public.sys_user

-- drop table public.sys_user;

create table public.sys_user
(
    id bigint not null,
    password character varying(255) collate pg_catalog."default",
    username character varying(255) collate pg_catalog."default",
    constraint sys_user_pkey primary key (id)
)
with (
    oids = false
)
tablespace pg_default;

alter table public.sys_user
    owner to postgres;
insert into sys_user values(1,'admin','admin@123.com');	
	