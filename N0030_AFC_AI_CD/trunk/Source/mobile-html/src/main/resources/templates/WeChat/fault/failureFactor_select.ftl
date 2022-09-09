<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
    <link rel="stylesheet" href="/common/plugins/jquery-weui/css/jquery-weui.css">
    <style>
        .report-header {
            padding: 5px 0;
        }

        .div-height {
            height: 35px;
            padding: 5px 15px;
        }

        .sbbmdiv-height {
            padding: 5px 15px;
        }

        .report-title {
            text-align: center;
            font-size: 21px;
            font-weight: 500;
            margin: 0 15%;
        }

        .cells-font-size {
            font-size: 18px;
            margin-top: 0.8em;
        }
    </style>
</head>

<body ontouchstart>
<header class='report-header'>
    <h2 class="report-title">设备故障因子条件筛选</h2>
</header>
<div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
    <div class="weui-cell weui-cell_select div-height">
        <div class="weui-cell__hd"><label class="weui-label">线路</label></div>
        <div class="weui-cell__bd">
            <input class="weui-input" type="text" id="line">
        </div>
    </div>
    <div class="weui-cell weui-cell_select div-height">
        <div class="weui-cell__hd"><label class="weui-label">车站</label></div>
        <div class="weui-cell__bd" id="station_div">
            <input class="weui-input" type="text" id='station'/>
        </div>
    </div>
    <div class="weui-cell weui-cell_select div-height">
        <div class="weui-cell__hd"><label class="weui-label">设备类型</label></div>
        <div class="weui-cell__bd">
            <input class="weui-input" type="text" id='deviceType'/>
        </div>
    </div>
    <div class="weui-cell weui-cell_select div-height">
        <div class="weui-cell__hd"><label class="weui-label">设备名称</label></div>
        <div class="weui-cell__bd" id="equipment_div">
            <input class="weui-input" type="text" id='equipment'/>
        </div>
    </div>
    <div class="weui-cell weui-cell_select div-height">
        <div class="weui-cell__hd"><label class="weui-label">模块名称</label></div>
        <div class="weui-cell__bd">
            <input class="weui-input" type="text" id='module'/>
        </div>
    </div>
    <div class="weui-cell weui-cell_select div-height">
        <div class="weui-cell__hd"><label class="weui-label">时间范围</label></div>
        <div class="weui-cell__bd">
            <input class="weui-input" type="text" data-toggle='date' id='time' placeholder='默认为12个月'/>
        </div>
    </div>
    <div class="weui-cell weui-cell_select sbbmdiv-height">
    </div>
    <div class="weui-btn-area">
        <a class="weui-btn weui-btn_primary" href="javascript:btnSubmit()" id="showTooltips">确定</a>
    </div>
</div>
<@bottom />

<script src="/${PATH}/js/fault/failureFactor_select.js?time=New Date()"></script>

</body>
</html>


