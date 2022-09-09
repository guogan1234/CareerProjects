
drop table if exists obj_line;
create table obj_line (  
    id	bigserial primary key,
    line_id	smallint ,
    line_name	varchar(50),
    sync_time	timestamp default current_timestamp   
);

drop table if exists obj_station;
CREATE TABLE obj_station
(
  id bigserial NOT NULL,
  line_id integer,
  station_id integer,
  station_name character varying(50),
  sync_time timestamp without time zone DEFAULT now(),
  longitude double precision, -- 经度
  latitude double precision, -- 纬度
  CONSTRAINT obj_station_pkey PRIMARY KEY (id)
);
create unique index uidx_line_station_id on obj_station(line_id,station_id);

drop table if exists station_time_sharing_passenger_flow;
create table station_time_sharing_passenger_flow (
    id	bigserial primary key,
    line_id	integer ,
    station_id	integer,
    -- station_name varchar(50),
    direction	integer,
    passenger_flow	integer,
    flow_timestamp	timestamp,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists station_cumulative_passenger_flow;
create table station_cumulative_passenger_flow (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    -- station_name	varchar(50),
    direction	integer,
    passenger_flow	integer,
    flow_timestamp	timestamp,
    insert_time	timestamp default current_timestamp,
    section	integer
);


drop table if exists line_time_sharing_passenger_flow;
create table line_time_sharing_passenger_flow (
    id	bigserial primary key,
    line_id	integer ,
    direction	integer ,
    passenger_flow	integer ,
    flow_timestamp	timestamp ,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists line_cumulative_passenger_flow;
create table line_cumulative_passenger_flow (
    id	bigserial primary key,
    line_id	integer ,
    direction	integer ,
    passenger_flow	integer ,
    flow_timestamp	timestamp ,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists ticket_type;
create table ticket_type ( 
    id	bigserial primary key,
    ticket_id	integer,
    family_id integer,
    souvenirflag integer,
    namedsvticketflag integer,
    sounddisplayid integer,
    concessionallampid integer,    
    ticket_cn_name	varchar(50),
    ticket_en_name	varchar(50),
    insert_time	timestamp default current_timestamp
);

drop table if exists ticket_family;
create table ticket_family (
    id	bigserial  primary key,
    family_id integer,
    cn_name	varchar(128),
    en_name	varchar(128),
    update_time	timestamp default current_timestamp 
);

drop table if exists ticket_share_passenger_flow;
create table ticket_share_passenger_flow ( 
    id	bigserial primary key,
    ticket_id	integer ,
    family_id integer,
    -- ticket_name	varchar(50),
    line_id	integer,
    station_id	integer,
    direction	integer ,
    passenger_flow	integer ,
    flow_timestamp	timestamp ,
    insert_time	timestamp default current_timestamp,
    section	integer
);


drop table if exists ticket_cumulative_passenger_flow;
create table ticket_cumulative_passenger_flow ( 
    id	bigserial primary key,
    ticket_id	integer ,
    family_id integer,
    -- ticket_name	varchar(50),
    line_id	integer,
    station_id	integer,
    direction	integer ,
    passenger_flow	integer ,
    flow_timestamp	timestamp ,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists station_time_sharing_predict_flow;
create table station_time_sharing_predict_flow (
    id	bigserial primary key,
    line_id	integer ,
    station_id	integer,
    direction	integer,
    passenger_flow	integer,
    flow_timestamp	timestamp,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists station_cumulative_predict_flow;
create table station_cumulative_predict_flow (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    direction	integer,
    passenger_flow	integer,
    flow_timestamp	timestamp,
    insert_time	timestamp default current_timestamp,
    section	integer
);


drop table if exists line_time_sharing_predict_flow;
create table line_time_sharing_predict_flow (
    id	bigserial primary key,
    line_id	integer ,
    direction	integer ,
    passenger_flow	integer ,
    flow_timestamp	timestamp ,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists line_cumulative_predict_flow;
create table line_cumulative_predict_flow (
    id	bigserial primary key,
    line_id	integer ,
    direction	integer ,
    passenger_flow	integer ,
    flow_timestamp	timestamp ,
    insert_time	timestamp default current_timestamp,
    section	integer
);

drop table if exists ref_module_type;
create table ref_module_type (
    id bigserial primary key,
    module_type_code varchar(50),
    display_wording_cn varchar(50),
    display_wording_en varchar(50),
    insert_time	timestamp default current_timestamp
);

drop table if exists ref_package;
create table ref_package (
    id bigserial primary key,
    package_id integer,
    package_name varchar(50),
    display_name varchar(50),
    package_host varchar(50),
    port integer,
    insert_time	timestamp default current_timestamp
);

drop table if exists equipment_type;
create table equipment_type (
    id bigserial primary key,
    equipment_type_id	integer,
    name varchar(50),
    short_name	varchar(50),
    sync_time	timestamp default current_timestamp
);

drop table if exists equipment_subtype;
create table equipment_subtype (
    id bigserial primary key,
    equipment_subtype_id	integer,
    equipment_type_id	integer,
    name varchar(50),
    short_name	varchar(50),
    sync_time	timestamp default current_timestamp
);

drop table if exists obj_equipment;
create table obj_equipment (
    id bigserial primary key,
    equipment_id	integer,
    equipment_subtype_id	integer,
    equipment_type_id	integer,
    line_id	integer,
    station_id	integer,
    name varchar(50),    
    sync_time	timestamp default current_timestamp
);

drop table if exists line_failure_analysis;
create table line_failure_analysis (
    id bigserial primary key,
    line_id	integer ,
    tag_name varchar(50),
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists station_failure_analysis;
create table station_failure_analysis (
    id bigserial primary key,
    line_id	integer ,
    station_id	integer,
    tag_name varchar(50),
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists ref_module_define;
create table ref_module_define (
    id	bigserial  primary key,
    module_id integer ,
    module_name varchar(50),
    insert_time	timestamp default current_timestamp
);


drop table if exists ref_module_subtag_define;
create table ref_module_subtag_define (
    id	bigserial  primary key,    
    tag_name varchar(50),
    tag_desc varchar(50),
    module_id integer ,
    insert_time	timestamp default current_timestamp
);


drop table if exists module_failure_analysis;
create table module_failure_analysis (
    id	bigserial  primary key,
    line_id	integer ,
    module_id integer ,
    tag_name varchar(50),
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists module_failure_analysis_bystation;
create table module_failure_analysis_bystation (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    module_id integer ,
    tag_name varchar(50),
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists device_failure_analysis;
create table device_failure_analysis (
    id	bigserial  primary key,
    line_id	integer ,
    device_id integer ,
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists device_failure_analysis_detail;
create table device_failure_analysis_detail (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    device_id integer ,
    device_type integer ,
    tag_name varchar(50),
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);
create index idx_dfad_tag_name on device_failure_analysis_detail(tag_name);

drop table if exists power_system_energy_analysis;
create table power_system_energy_analysis (
    id	bigserial  primary key,
    time_table_mode_id integer ,
    time_table_mode_name varchar(50) ,
    consume_number integer ,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);


drop table if exists ventilation_system_energy_analysis;
create table ventilation_system_energy_analysis (
    id	bigserial  primary key,
    time_table_mode_id integer ,
    time_table_mode_name varchar(50) ,
    consume_number integer ,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists air_condition_system_energy_analysis;
create table air_condition_system_energy_analysis (
    id	bigserial  primary key,
    time_table_mode_id integer ,
    time_table_mode_name varchar(50) ,
    consume_number integer ,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists escalator_system_energy_analysis;
create table escalator_system_energy_analysis (
    id	bigserial  primary key,
    time_table_mode_id integer ,
    time_table_mode_name varchar(50) ,
    consume_number integer ,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists lighting_system_energy_analysis;
create table lighting_system_energy_analysis (
    id	bigserial  primary key,
    time_table_mode_id integer ,
    time_table_mode_name varchar(50) ,
    consume_number integer ,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);


drop table if exists core_switch_status;
create table core_switch_status (
    id	bigserial  primary key,
    ip	varchar(20),
    name  varchar(50),   
    status integer ,
    update_time	timestamp default current_timestamp
);

drop table if exists device_downtime_percent_analysis;
create table device_downtime_percent_analysis (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    device_id integer ,
    normal_time_in_minutes integer,
    failure_time_in_minutes integer,
    failure_rate real,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists failure_reason_analysis;
create table failure_reason_analysis (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    device_id integer ,
    device_type integer ,
    module_id integer ,
    normal_time_in_minutes integer,
    failure_time_in_minutes integer,
    trans_count integer,
    failure_num integer,
    analysis_timestamp timestamp,
    insert_time	timestamp default current_timestamp
);

drop table if exists device_maintenance_plan;
create table device_maintenance_plan (
    id	bigserial  primary key,
    plan_id	integer ,
    line_id	integer ,
    station_id	integer,
    device_id integer ,
    device_type integer ,
    module_id integer ,
    next_maintenance_time timestamp,
    decision_basis integer ,
    generate_time timestamp,
    update_time	timestamp default current_timestamp 
);

drop table if exists component_consume_analysis;
create table component_consume_analysis (
    id	bigserial  primary key,
    component_type	integer ,
    component_name	varchar(200) ,
    consumption	integer,    
    analysis_timestamp timestamp,
    update_time	timestamp default current_timestamp 
);

drop table if exists component_consume_predict;
create table component_consume_predict (
    id	bigserial  primary key,
    component_type	integer ,
    component_name	varchar(200) ,
    consumption	integer,    
    analysis_timestamp timestamp,
    update_time	timestamp default current_timestamp 
);

drop table if exists component_destination_analysis;
create table component_destination_analysis (
    id	bigserial  primary key,
    line_id	integer ,
    station_id	integer,
    device_id integer ,
    maintenance_compay	varchar(128),
    maintenance_person	varchar(32),
    consumption	integer,    
    analysis_timestamp timestamp,
    update_time	timestamp default current_timestamp 
);



commit;

