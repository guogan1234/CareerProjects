CREATE TABLE conf.acc_def_tbl
(
    acc_def_label character varying(64) COLLATE pg_catalog."default" NOT NULL,
    acc_base1 integer,
    acc_base2 integer,
    acc_base3 integer,
    acc_base4 integer,
    acc_coeff1 integer,
    acc_coeff2 integer,
    acc_coeff3 integer,
    acc_coeff4 integer,
    acc_tp_label character varying(32) COLLATE pg_catalog."default",
    channel_num integer,
    channel_tag1 character varying(64) COLLATE pg_catalog."default",
    channel_tag2 character varying(64) COLLATE pg_catalog."default",
    channel_tag3 character varying(64) COLLATE pg_catalog."default",
    channel_tag4 character varying(64) COLLATE pg_catalog."default",
    domain_id integer,
    dot_no1 integer,
    dot_no2 integer,
    dot_no3 integer,
    dot_no4 integer,
    location_id integer,
    pro_system_id integer,
    dev_type_label character varying(16) COLLATE pg_catalog."default",
    operate_time timestamp without time zone,
    CONSTRAINT acc_def_tbl_pkey PRIMARY KEY (acc_def_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.acc_def_tbl
    OWNER to postgres;

CREATE TABLE conf.acc_limit_tbl
(
    acc_limit_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    acc_tp_label character varying(32) COLLATE pg_catalog."default",
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
    pic_name character varying(64) COLLATE pg_catalog."default",
    pro_system_id integer,
    region_id integer,
    is_replace_alarm boolean,
    operate_time timestamp without time zone,
    CONSTRAINT acc_limit_tbl_pkey PRIMARY KEY (acc_limit_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.acc_limit_tbl
    OWNER to postgres;

CREATE TABLE conf.acc_tbl
(
    acc_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    acc_name character varying(128) COLLATE pg_catalog."default",
    acc_order integer,
    acc_status integer,
    acc_value integer,
    camera_preset character varying(64) COLLATE pg_catalog."default",
    camera_tag character varying(64) COLLATE pg_catalog."default",
    change_time bigint,
    dev_label character varying(96) COLLATE pg_catalog."default",
    dev_type_label character varying(16) COLLATE pg_catalog."default",
    domain_id integer,
    is_limit boolean,
    is_sample boolean,
    location_id integer,
    point_tp_label character varying(32) COLLATE pg_catalog."default",
    pro_system_id integer,
    region_id integer,
    sample_deadband integer,
    unit_id integer,
    update_time bigint,
    po_integer_tp_label character varying(32) COLLATE pg_catalog."default",
    operate_time timestamp without time zone,
    CONSTRAINT acc_tbl_pkey PRIMARY KEY (acc_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.acc_tbl
    OWNER to postgres;
CREATE TABLE conf.acc_tp_tbl
(
    acc_tp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    acc_base1 integer,
    acc_base2 integer,
    acc_base3 integer,
    acc_base4 integer,
    acc_coeff1 integer,
    acc_coeff2 integer,
    acc_coeff3 integer,
    acc_coeff4 integer,
    acc_tp_name character varying(64) COLLATE pg_catalog."default",
    acc_tp_order integer,
    alarm_delay_time integer,
    alarm_level integer,
    dev_tp_label character varying(32) COLLATE pg_catalog."default",
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
    CONSTRAINT acc_tp_tbl_pkey PRIMARY KEY (acc_tp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.acc_tp_tbl
    OWNER to postgres;

CREATE TABLE conf.ai_limit_tbl
(
    ai_limit_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
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
    pic_name character varying(64) COLLATE pg_catalog."default",
    point_tp_label character varying(32) COLLATE pg_catalog."default",
    pro_system_id integer,
    is_replace_alarm boolean,
    operate_time timestamp without time zone,
    CONSTRAINT ai_limit_tbl_pkey PRIMARY KEY (ai_limit_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.ai_limit_tbl
    OWNER to postgres;

CREATE TABLE conf.ai_tbl
(
    ai_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    ai_name character varying(128) COLLATE pg_catalog."default",
    ai_order integer,
    ai_status integer,
    ai_value real,
    camera_preset character varying(64) COLLATE pg_catalog."default",
    camera_tag character varying(64) COLLATE pg_catalog."default",
    change_time bigint,
    is_control boolean,
    ctrl_trans_grp_label character varying(32) COLLATE pg_catalog."default",
    dev_label character varying(96) COLLATE pg_catalog."default",
    dev_type character varying(16) COLLATE pg_catalog."default",
    domain_id integer,
    is_limit boolean,
    location_id integer,
    point_tp_label character varying(32) COLLATE pg_catalog."default",
    point_type integer,
    pro_system_id integer,
    region_id integer,
    is_sample boolean,
    sample_deadband real,
    unit_id integer,
    update_time bigint,
    operate_time timestamp without time zone,
    CONSTRAINT ai_tbl_pkey PRIMARY KEY (ai_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.ai_tbl
    OWNER to postgres;

CREATE TABLE conf.aio_def_tbl
(
    aio_def_label character varying(64) COLLATE pg_catalog."default" NOT NULL,
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
    channel_tag1 character varying(64) COLLATE pg_catalog."default",
    channel_tag2 character varying(64) COLLATE pg_catalog."default",
    channel_tag3 character varying(64) COLLATE pg_catalog."default",
    channel_tag4 character varying(64) COLLATE pg_catalog."default",
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
    point_tp_label character varying(32) COLLATE pg_catalog."default",
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
    dev_type_label character varying(16) COLLATE pg_catalog."default",
    CONSTRAINT aio_def_tbl_pkey PRIMARY KEY (aio_def_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.aio_def_tbl
    OWNER to postgres;

CREATE TABLE conf.aio_tp_tbl
(
    aio_tp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
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
    aio_tp_name character varying(64) COLLATE pg_catalog."default",
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
    dev_tp_label character varying(32) COLLATE pg_catalog."default",
    formula_tp_label character varying(32) COLLATE pg_catalog."default",
    is_control boolean,
    id_del_on_ack integer,
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
    po_integer_type integer,
    CONSTRAINT aio_tp_tbl_pkey PRIMARY KEY (aio_tp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.aio_tp_tbl
    OWNER to postgres;

CREATE TABLE conf.alarm_act_def_tbl
(
    act_id integer NOT NULL,
    act_def character varying(64) COLLATE pg_catalog."default",
    description character varying(32) COLLATE pg_catalog."default",
    tag_name character varying(16) COLLATE pg_catalog."default",
    CONSTRAINT alarm_act_def_tbl_pkey PRIMARY KEY (act_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.alarm_act_def_tbl
    OWNER to postgres;

CREATE TABLE conf.ao_tbl
(
    ao_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    ao_base real,
    ao_coeff real,
    ao_format integer,
    ao_no integer,
    ao_tolerance real,
    bottom_code integer,
    bottom_scale real,
    data_type integer,
    exec_timeout integer,
    formula_label character varying(32) COLLATE pg_catalog."default",
    resv_time integer,
    top_code integer,
    top_scale real,
    wait_act integer,
    operate_time timestamp without time zone,
    CONSTRAINT ao_tbl_pkey PRIMARY KEY (ao_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.ao_tbl
    OWNER to postgres;

CREATE TABLE conf.ctrl_trans_grp_tbl
(
    ctrl_trans_grp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    ctrl_trans_grp_name character varying(64) COLLATE pg_catalog."default" NOT NULL,
    ctrl_trans_grp_plabel character varying(32) COLLATE pg_catalog."default",
    domain_id integer NOT NULL,
    ex_cmp_label character varying(160) COLLATE pg_catalog."default",
    ex_value integer NOT NULL,
    pro_system_id integer NOT NULL,
    region_id integer NOT NULL,
    station_id integer NOT NULL,
    trans_state integer NOT NULL,
    trans_timeout integer,
    CONSTRAINT ctrl_trans_grp_tbl_pkey PRIMARY KEY (ctrl_trans_grp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.ctrl_trans_grp_tbl
    OWNER to postgres;

CREATE TABLE conf.dev_node_tbl
(
    tag_name character varying(32) COLLATE pg_catalog."default" NOT NULL,
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
    CONSTRAINT dev_node_tbl_pkey PRIMARY KEY (tag_name)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dev_node_tbl
    OWNER to postgres;

CREATE TABLE conf.dev_tbl
(
    dev_label character varying(96) COLLATE pg_catalog."default" NOT NULL,
    ctrl_trans_grp_label character varying(32) COLLATE pg_catalog."default",
    dev_name character varying(32) COLLATE pg_catalog."default",
    dev_status integer,
    dev_tp_label character varying(32) COLLATE pg_catalog."default",
    dev_type_label character varying(16) COLLATE pg_catalog."default",
    domain_id integer,
    is_summary integer,
    pro_system_id integer,
    region_id integer,
    rtu_id integer,
    station_id integer,
    summary_status integer,
    summary_value integer,
    operate_time timestamp without time zone,
    CONSTRAINT dev_tbl_pkey PRIMARY KEY (dev_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dev_tbl
    OWNER to postgres;

CREATE TABLE conf.dev_token_info_tbl
(
    token_id integer NOT NULL,
    dev_label character varying(96) COLLATE pg_catalog."default" NOT NULL,
    description character varying(128) COLLATE pg_catalog."default",
    domain_id integer,
    host_name character varying(64) COLLATE pg_catalog."default",
    op_time integer,
    station_id integer,
    user_grp_id integer,
    user_id integer,
    CONSTRAINT dev_token_info_tbl_pkey PRIMARY KEY (token_id, dev_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dev_token_info_tbl
    OWNER to postgres;

CREATE TABLE conf.dev_tp_tbl
(
    dev_tp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    dev_tp_name character varying(64) COLLATE pg_catalog."default",
    dev_type_label character varying(16) COLLATE pg_catalog."default",
    operate_time timestamp without time zone,
    pro_system_id integer,
    status integer,
    CONSTRAINT dev_tp_tbl_pkey PRIMARY KEY (dev_tp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dev_tp_tbl
    OWNER to postgres;

CREATE TABLE conf.dev_type_tbl
(
    dev_type_label character varying(16) COLLATE pg_catalog."default" NOT NULL,
    dev_type_name character varying(32) COLLATE pg_catalog."default",
    pro_system_id integer,
    CONSTRAINT dev_type_tbl_pkey PRIMARY KEY (dev_type_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dev_type_tbl
    OWNER to postgres;

CREATE TABLE conf.di_tbl
(
    di_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    alarm_delay_time integer,
    alarm_level integer,
    bit_delay_time integer,
    bit_num integer,
    camera_preset character varying(64) COLLATE pg_catalog."default",
    camera_tag character varying(64) COLLATE pg_catalog."default",
    change_time1 bigint,
    change_time2 bigint,
    change_time3 bigint,
    change_time4 bigint,
    change_time5 bigint,
    ctrl_trans_grp_label character varying(32) COLLATE pg_catalog."default",
    dev_label character varying(96) COLLATE pg_catalog."default",
    di_name character varying(128) COLLATE pg_catalog."default",
    di_order integer,
    di_status integer,
    di_status1 integer,
    di_status2 integer,
    di_status3 integer,
    di_status4 integer,
    di_status5 integer,
    di_text_label character varying(32) COLLATE pg_catalog."default",
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
    pic_name character varying(64) COLLATE pg_catalog."default",
    point_tp_label character varying(32) COLLATE pg_catalog."default",
    point_type integer,
    pro_system_id integer,
    region_id integer,
    summary_status integer,
    summary_value integer,
    update_time bigint,
    po_integer_tp_label character varying(32) COLLATE pg_catalog."default",
    po_integer_type integer,
    is_control boolean,
    dev_type_label character varying(255) COLLATE pg_catalog."default",
    operate_time timestamp without time zone,
    CONSTRAINT di_tbl_pkey PRIMARY KEY (di_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.di_tbl
    OWNER to postgres;

CREATE TABLE conf.di_text_tbl
(
    di_text_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    alarm_mode integer,
    di_text character varying(32) COLLATE pg_catalog."default",
    di_value integer,
    pro_system_id integer,
    CONSTRAINT di_text_tbl_pkey PRIMARY KEY (di_text_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.di_text_tbl
    OWNER to postgres;

CREATE TABLE conf.dio_def_tbl
(
    dio_def_label character varying(64) COLLATE pg_catalog."default" NOT NULL,
    bit_num integer,
    channel_tag1 character varying(64) COLLATE pg_catalog."default",
    channel_tag2 character varying(1) COLLATE pg_catalog."default",
    channel_tag3 character varying(64) COLLATE pg_catalog."default",
    channel_tag4 character varying(64) COLLATE pg_catalog."default",
    dev_type_label character varying(15) COLLATE pg_catalog."default",
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
    point_tp_label character varying(32) COLLATE pg_catalog."default",
    polarity1 integer,
    pro_system_id integer,
    CONSTRAINT dio_def_tbl_pkey PRIMARY KEY (dio_def_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dio_def_tbl
    OWNER to postgres;

CREATE TABLE conf.dio_tp_tbl
(
    dio_tp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    alarm_delay_time integer,
    alarm_level integer,
    bit_delay_time integer,
    bit_num integer,
    ctrl_grp_label character varying(32) COLLATE pg_catalog."default",
    ctrl_timeout integer,
    ctrl_wait_act integer,
    dev_tp_label character varying(32) COLLATE pg_catalog."default",
    di_text_label character varying(32) COLLATE pg_catalog."default",
    dio_tp_name character varying(64) COLLATE pg_catalog."default",
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
    formula_tp_label character varying(32) COLLATE pg_catalog."default",
    is_control boolean,
    is_del_on_ack integer,
    is_replace_alarm boolean,
    is_sample boolean,
    is_summary boolean,
    operate_time timestamp without time zone,
    point_type character varying(255) COLLATE pg_catalog."default",
    polarity1 integer,
    polarity2 integer,
    polarity3 integer,
    polarity4 integer,
    pulse_duration integer,
    resv_time integer,
    status integer,
    po_integer_type character varying(255) COLLATE pg_catalog."default",
    CONSTRAINT dio_tp_tbl_pkey PRIMARY KEY (dio_tp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.dio_tp_tbl
    OWNER to postgres;

CREATE TABLE conf.do_grp_tbl
(
    do_order integer NOT NULL,
    do_grp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    do_allow integer,
    do_index integer,
    do_name character varying(32) COLLATE pg_catalog."default",
    do_number integer,
    do_type integer,
    do_value integer,
    pro_system_id integer,
    CONSTRAINT do_grp_tbl_pkey PRIMARY KEY (do_order, do_grp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.do_grp_tbl
    OWNER to postgres;


CREATE TABLE conf.do_tbl
(
    do_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    do_grp_label character varying(32) COLLATE pg_catalog."default",
    do_no1 integer,
    do_no2 integer,
    do_no3 integer,
    do_no4 integer,
    do_no5 integer,
    do_num integer,
    exec_timeout integer,
    formula_label character varying(32) COLLATE pg_catalog."default",
    pulse_duration integer,
    resv_time integer,
    wait_act integer,
    operate_time timestamp without time zone,
    CONSTRAINT do_tbl_pkey PRIMARY KEY (do_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.do_tbl
    OWNER to postgres;

CREATE TABLE conf.domain_tbl
(
    domain_id integer NOT NULL,
    domain_label character varying(16) COLLATE pg_catalog."default",
    domain_name character varying(32) COLLATE pg_catalog."default",
    domain_type integer,
    CONSTRAINT domain_tbl_pkey PRIMARY KEY (domain_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.domain_tbl
    OWNER to postgres;

CREATE TABLE conf.formula_grp_tbl
(
    formula_grp_id integer NOT NULL,
    formula_grp_name character varying(32) COLLATE pg_catalog."default",
    CONSTRAINT formula_grp_tbl_pkey PRIMARY KEY (formula_grp_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.formula_grp_tbl
    OWNER to postgres;

CREATE TABLE conf.formula_param_tbl
(
    formula_param_order integer NOT NULL,
    formula_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    domain_id integer,
    formula_param_label character varying(160) COLLATE pg_catalog."default",
    CONSTRAINT formula_param_tbl_pkey PRIMARY KEY (formula_param_order, formula_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.formula_param_tbl
    OWNER to postgres;


CREATE TABLE conf.formula_tp_tbl
(
    formula_tp_label character varying(32) COLLATE pg_catalog."default" NOT NULL,
    formula_text character varying(4000) COLLATE pg_catalog."default",
    formula_tp_name character varying(64) COLLATE pg_catalog."default",
    CONSTRAINT formula_tp_tbl_pkey PRIMARY KEY (formula_tp_label)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.formula_tp_tbl
    OWNER to postgres;

CREATE TABLE conf.node_conn_tbl
(
    tag_name character varying(32) COLLATE pg_catalog."default" NOT NULL,
    node_no integer NOT NULL,
    para1 integer,
    para2 integer,
    target_nodeno integer NOT NULL,
    target_tagname character varying(32) COLLATE pg_catalog."default" NOT NULL,
    CONSTRAINT node_conn_tbl_pkey PRIMARY KEY (tag_name, node_no)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.node_conn_tbl
    OWNER to postgres;

CREATE TABLE conf.point_op_info_tbl
(
    point_label character varying(128) COLLATE pg_catalog."default" NOT NULL,
    op_type integer NOT NULL,
    description character varying(128) COLLATE pg_catalog."default",
    display_text character varying(32) COLLATE pg_catalog."default",
    domain_id integer,
    force_value real,
    host_name character varying(64) COLLATE pg_catalog."default",
    op_time integer,
    station_id integer,
    user_grp_id integer,
    user_id integer,
    CONSTRAINT point_op_info_tbl_pkey PRIMARY KEY (point_label, op_type)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.point_op_info_tbl
    OWNER to postgres;

CREATE TABLE conf.pro_system_tbl
(
    pro_system_id integer NOT NULL,
    pro_system_label character varying(16) COLLATE pg_catalog."default",
    pro_system_name character varying(32) COLLATE pg_catalog."default",
    CONSTRAINT pro_system_tbl_pkey PRIMARY KEY (pro_system_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.pro_system_tbl
    OWNER to postgres;

CREATE TABLE conf.region_tbl
(
    region_id integer NOT NULL,
    pro_system_id integer,
    region_label character varying(16) COLLATE pg_catalog."default",
    region_name character varying(32) COLLATE pg_catalog."default",
    CONSTRAINT region_tbl_pkey PRIMARY KEY (region_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.region_tbl
    OWNER to postgres;

CREATE TABLE conf.station_tbl
(
    station_tbl integer NOT NULL,
    station_label character varying(16) COLLATE pg_catalog."default",
    station_name character varying(32) COLLATE pg_catalog."default",
    station_pid integer,
    CONSTRAINT station_tbl_pkey PRIMARY KEY (station_tbl)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.station_tbl
    OWNER to postgres;

CREATE TABLE conf.unit_tbl
(
    unit_id integer NOT NULL,
    unit_name character varying(32) COLLATE pg_catalog."default" NOT NULL,
    CONSTRAINT unit_tbl_pkey PRIMARY KEY (unit_id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE conf.unit_tbl
    OWNER to postgres;

CREATE TABLE public.sys_user
(
    id bigint NOT NULL,
    password character varying(255) COLLATE pg_catalog."default",
    username character varying(255) COLLATE pg_catalog."default",
    CONSTRAINT sys_user_pkey PRIMARY KEY (id)
)
WITH (
    OIDS = FALSE
)
TABLESPACE pg_default;

ALTER TABLE public.sys_user
    OWNER to postgres;