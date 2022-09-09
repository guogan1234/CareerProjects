
-- ref_module_define
insert into ref_module_define(module_id,module_name) values (1,'总体状态');
insert into ref_module_define(module_id,module_name) values (2,'纸币找零');
insert into ref_module_define(module_id,module_name) values (3,'纸币接收');
insert into ref_module_define(module_id,module_name) values (4,'硬币模块');
insert into ref_module_define(module_id,module_name) values (5,'系统事件');
insert into ref_module_define(module_id,module_name) values (6,'通道控制');
insert into ref_module_define(module_id,module_name) values (7,'数据存储');
insert into ref_module_define(module_id,module_name) values (8,'模式事件');
insert into ref_module_define(module_id,module_name) values (9,'回收模块');
insert into ref_module_define(module_id,module_name) values (10,'发卡模块');
insert into ref_module_define(module_id,module_name) values (11,'读写器2');
insert into ref_module_define(module_id,module_name) values (12,'读写器1');
insert into ref_module_define(module_id,module_name) values (13,'打印机');
insert into ref_module_define(module_id,module_name) values (14,'参数事件');
insert into ref_module_define(module_id,module_name) values (15,'UPS');

-- ref_module_subtag_define
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METSER','总体状态（模块、设备、车站总体）',1);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METBCG','纸币找零综合状态',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGCOM','纸币找零通信状态',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGSTA','纸币找零故障',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGCT1','找零钱箱1状态',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGT1O','找零钱箱1抽出',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGT1I','找零钱箱1推入',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGCT2','找零钱箱2状态',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGT2O','找零钱箱2抽出',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGT2I','找零钱箱2推入',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGCT3','找零废币箱状态',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGT3O','找零废币箱抽出',2);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BCGT3I','找零废币箱推入',2);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METBNA','纸币接收模块综合状态',3);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BNACOM','纸币接收通信状态',3);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NNVAST','纸币接收钱箱状态',3);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NVAUEX','纸币钱箱抽出',3);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NVAUIN','纸币钱箱推入',3);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BNAJAM','纸币模块卡币',3);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BNASTA','纸币模块故障',3);
	
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CHGSTA','硬币找零模块找零',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NHO1ST','1元备用找零钱箱状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CHSSTA','硬币模块状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('HO1AEX','1元备用找零钱箱抽出',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('HO1AIN','1元备用找零钱箱推入',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('HO1ERR','1元备用找零钱箱错误',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NHO2ST','5角备用找零钱箱状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('HO2ERR','5角备用找零钱箱错误',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('HO2AEX','5角备用找零钱箱抽出',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('HO2AIN','5角备用找零钱箱推入',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NDR1ST','1元循环找零钱箱总状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NDR2ST','5角循环找零钱箱总状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METCHS','硬币模块综合状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CHSCOM','硬币模块通信状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('DR1STA','硬币回收钱箱1状态',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('DR1AEX','硬币回收钱箱1抽出',4);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('DR1AIN','硬币回收钱箱1推入',4);
	
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BUSDAY','运营时间事件',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('COMSTA','通信状态',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NLOGON','登录事件',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('TIMDIS','时间同步',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NACTAI','招援事件',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('ACCCOM','和ACC通讯',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('TEMOFR','温度',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('INTRST','登录超时',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('BREAKS','轮班处理',5);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('DORPOS','维护门事件',5);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('PASINT','乘客入侵',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('AILSTA','扇门模式',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('SENFAL','传感器事件',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('PLCSTA','扇门控制器',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('FLAPST','扇门状态',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('FLAFOR','扇门冲撞',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('PLCCOM','通道控制通信状态',6);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METPLC','通道控制综合',6);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('DAT1US','系统存储空间',7);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('DAT2US','数据存储空间',7);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METEMM','ECU存储综合',7);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NMDSTA','系统模式事件',8);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('SERSTA','设备服务状态',8);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('AILDIR','通道方向事件',8);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CINJAM','卡票事件',9);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CINCS1','票箱1',9);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CINCS2','票箱2',9);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CINCS3','票箱3',9);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CINSTA','回收模块工作状态',9);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METCTN','回收模块综合状态',9);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CSTSTA','发卡模块工作状态',10);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METCST','发卡模块综合状态',10);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CSTCOM','发卡模块通信状态',10);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CSTCS1','票箱1',10);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CSTCS2','票箱2',10);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CSTCS3','票箱3',10);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS2KEY','读写器2密钥事件',11);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS2EOD','读写器2参数激活',11);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS2STA','读写器2状态',11);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS2DWN','读写器2参数下载',11);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS2COM','读写器2通信状态',11);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METCS2','读写器2综合状态',11);
	
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS1KEY','读写器1密钥事件',12);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS1EOD','读写器1参数激活',12);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS1STA','读写器1状态',12);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS1DWN','读写器1参数下载',12);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('CS1COM','读写器1通信状态',12);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METCS1','读写器1综合状态',12);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METRPU','打印机状态',13);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('RPUMET','打印机综合状态',13);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METEOD','参数综合状态',14);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('NEDFAI','参数生效',14);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('EODDWN','参数下载',14);

insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('METUPS','UPS状态',15);
insert into ref_module_subtag_define(tag_name,tag_desc,module_id) values ('UPSMET','UPS综合状态',15);

-- obj_line
INSERT INTO obj_line (line_id, line_name, sync_time) VALUES (4, '十号线', '2017-08-17 17:12:02');
INSERT INTO obj_line (line_id, line_name, sync_time) VALUES (6, '宁天城际', '2017-08-17 17:12:02');
INSERT INTO obj_line (line_id, line_name, sync_time) VALUES (7, '三号线', '2017-08-17 17:12:02');
INSERT INTO obj_line (line_id, line_name, sync_time) VALUES (8, 'X号线', '2017-08-17 17:12:02');
INSERT INTO obj_line (line_id, line_name, sync_time) VALUES (1, '一号线', '2017-08-17 17:12:02');

-- obj_station
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 114, '龙江', '2017-08-17 17:12:04', 118.746385, 32.06369);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 115, '草场门', '2017-08-17 17:12:04', 118.745954, 32.062848);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 116, '云南路', '2017-08-17 17:12:04', 118.781166, 32.065189);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 11, '鼓楼', '2017-08-17 17:12:04', 118.789995, 32.06507);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 98, '鸡鸣寺', '2017-08-17 17:12:04', 118.804167, 32.063699);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 117, '九华山', '2017-08-17 17:12:04', 118.812338, 32.063688);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 118, '岗子村', '2017-08-17 17:12:04', 118.822102, 32.071958);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 119, '蒋王庙', '2017-08-17 17:12:04', 118.837644, 32.083553);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 120, '王家湾', '2017-08-17 17:12:04', 118.84452, 32.091105);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 121, '聚宝山', '2017-08-17 17:12:04', 118.871195, 32.099163);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 122, '徐庄', '2017-08-17 17:12:04', 118.895646, 32.090637);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 34, '金马路', '2017-08-17 17:12:04', 118.912541, 32.077876);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 123, '汇通路', '2017-08-17 17:12:04', 118.937907, 32.08436);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 124, '灵山', '2017-08-17 17:12:04', 118.950408, 32.083714);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 125, '东流', '2017-08-17 17:12:04', 118.971221, 32.08295);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 126, '孟北', '2017-08-17 17:12:04', 118.994038, 32.09);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 127, '西岗桦墅', '2017-08-17 17:12:04', 119.003201, 32.104645);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (8, 128, '仙林湖', '2017-08-17 17:12:04', 118.999339, 32.131236);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 5, '安德门', '2017-08-17 17:12:04', 118.768408, 31.997111);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 4, '小行', '2017-08-17 17:12:04', 118.750965, 31.988061);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 3, '中胜', '2017-08-17 17:12:04', 118.740078, 31.993863);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 2, '元通', '2017-08-17 17:12:04', 118.728019, 32.001636);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 1, '奥体中心', '2017-08-17 17:12:04', 118.724648, 32.014641);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 60, '浦口大道', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 101, '夫子庙', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 102, '武定门', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 103, '雨花门', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 104, '卡子门', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 105, '大明路', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 106, '明城大道', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 44, '南京南站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 107, '宏运大道', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 108, '胜太西路', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 109, '天元西路', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 110, '九龙湖', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 111, '诚信大道', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 112, '东大九龙湖校区', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 113, '秣周东路', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 24, '白湖亭', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 11, '象峰站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 12, '秀山站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 13, '罗汉山站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 14, '福州火车站站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 15, '斗门站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 16, '树兜站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 17, '屏山站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 18, '东街口站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 19, '南门兜站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 20, '茶亭站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 21, '达道站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 22, '上藤站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 23, '三叉街站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 25, '葫芦阵站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 26, '黄山站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 27, '排下站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 28, '城门站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 29, '三角埕站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 30, '胪雷站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (1, 31, '福州火车南站站', '2017-08-17 17:12:04', null, null);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 56, '梦都大街', '2017-08-17 17:12:04', 118.728496, 32.021622);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 57, '绿博园', '2017-08-17 17:12:04', 118.723864, 32.030042);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 58, '江心洲', '2017-08-17 17:12:04', 118.710073, 32.038541);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 59, '临江', '2017-08-17 17:12:04', 118.671513, 32.063605);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 61, '南京工业大学', '2017-08-17 17:12:04', 118.654624, 32.072823);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 62, '龙华路', '2017-08-17 17:12:04', 118.641906, 32.07041);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 63, '文德路', '2017-08-17 17:12:04', 118.633406, 32.063193);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (4, 64, '雨山路', '2017-08-17 17:12:04', 118.62186, 32.050221);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 72, '泰山新村', '2017-08-17 17:12:04', 118.722553, 32.150572);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 73, '泰冯路', '2017-08-17 17:12:04', 118.724986, 32.159761);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 74, '高新开发区', '2017-08-17 17:12:04', 118.726222, 32.183669);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 75, '信息工程大学', '2017-08-17 17:12:04', 118.733642, 32.208535);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 76, '卸甲甸', '2017-08-17 17:12:04', 118.737302, 32.220426);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 77, '大厂', '2017-08-17 17:12:04', 118.746646, 32.234617);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 90, '星火路', '2017-08-17 17:12:04', 118.703324, 32.16376);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 78, '葛塘', '2017-08-17 17:12:04', 118.760248, 32.250244);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 79, '长芦', '2017-08-17 17:12:04', 118.783179, 32.279629);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 80, '化工园', '2017-08-17 17:12:04', 118.792126, 32.291846);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 81, '六合开发区', '2017-08-17 17:12:04', 118.811502, 32.311537);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 82, '龙池', '2017-08-17 17:12:04', 118.826863, 32.325218);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 83, '雄州', '2017-08-17 17:12:04', 118.843307, 32.340535);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 84, '凤凰山公园', '2017-08-17 17:12:04', 118.851749, 32.352723);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 85, '方州广场', '2017-08-17 17:12:04', 118.856496, 32.364841);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 86, '沈桥', '2017-08-17 17:12:04', 118.898164, 32.405959);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 87, '八百桥', '2017-08-17 17:12:04', 118.939642, 32.433298);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (6, 88, '金牛湖', '2017-08-17 17:12:04', 118.970654, 32.470861);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 89, '林场', '2017-08-17 17:12:04', 118.67778, 32.169947);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 91, '东大成贤学院', '2017-08-17 17:12:04', 118.714479, 32.162498);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 73, '泰冯路', '2017-08-17 17:12:04', 118.724986, 32.159761);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 92, '天润城', '2017-08-17 17:12:04', 118.740599, 32.156536);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 93, '柳洲东路', '2017-08-17 17:12:04', 118.752849, 32.145776);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 94, '上元门', '2017-08-17 17:12:04', 118.776209, 32.121777);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 95, '五塘广场', '2017-08-17 17:12:04', 118.784105, 32.116682);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 96, '小市', '2017-08-17 17:12:04', 118.791834, 32.101609);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 14, '南京站', '2017-08-17 17:12:04', 118.802629, 32.094062);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 97, '南京林业大学-新庄', '2017-08-17 17:12:04', 118.816707, 32.083033);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 98, '鸡鸣寺', '2017-08-17 17:12:04', 118.804167, 32.063699);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 99, '浮桥', '2017-08-17 17:12:04', 118.802703, 32.055384);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 26, '大行宫', '2017-08-17 17:12:04', 118.801217, 32.047738);
INSERT INTO obj_station (line_id, station_id, station_name, sync_time, longitude, latitude) VALUES (7, 100, '常府街', '2017-08-17 17:12:04', 118.798819, 32.04028);



commit;





