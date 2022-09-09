<!DOCTYPE html>
<html>
<head>
		<meta charset="utf-8">
		<title>故障模块</title>
		<meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1,user-scalable=no">
		<meta name="apple-mobile-web-app-capable" content="yes">
		<meta name="apple-mobile-web-app-status-bar-style" content="black">
		<#include "../common/common.ftl">
        <@header />
		<style type="text/css">
			#done.mui-disabled {
				color: darkslategray;
			}
			.mems-left-title{
				font-size: 5px;
			}
			.block-header {
				padding-top: 20px;
				padding-left: 10px;
			}

			.block-vgap {
				margin-top: 20px;
			}
		</style>
	</head>
	<body>
		<div class="mui-content mui-scroll-wrapper">
			<div class="mui-scroll">
				<!--已选择模块-->
				<ul class="mui-table-view">
					<h4 class="block-header">已选择模块</h4>
					<li class="mui-table-view-cell">
						<span id="selected-module" component-id=""></span>
					</li>
				</ul>
				<ul class="mui-table-view block-vgap">
					<h4 class="block-header">设备信息</h4>
					<li class="mui-table-view-cell">
						<span>设备名称 :</span>
						<span id="equipment_name"></span>
					</li>
					<li class="mui-table-view-cell">
						<span>所属线路 :</span>
						<span id="eqipment_line_belong"></span>
					</li>
					<li class="mui-table-view-cell">
						<span>所属车站 :</span>
						<span id="eqipment_station_belong"></span>
					</li>
				</ul>
				<ul id = "mod" class="mui-table-view block-vgap">
					<h4 class="block-header">模块列表</h4>
				</ul>
				<div class="mui-content-padded" style="margin-top: 10%;">
                    <button id='next' class="mui-btn mui-btn-block mui-btn-blue">下一步</button>
                </div>
			</div>
		</div>
		<div id="pullrefresh">
		</div>
		<script src="${PATH}/js/work/ems.js" data-main="${PATH}/js/work/equipmentselect.js" defer="defer"></script>
	</body>