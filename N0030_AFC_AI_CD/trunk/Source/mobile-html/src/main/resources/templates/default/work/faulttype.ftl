<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>移动运维</title>
    <meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1,user-scalable=no">
    <#include "../common/common.ftl">
    <@header />
    <style type="text/css">
        #done.mui-disabled {
            color: darkslategray;
        }

        .block-header {
            padding-top: 20px;
        }

        .block-vgap {
            margin-top: 20px;
        }
    </style>
</head>

<body>
<div class="mui-content mui-scroll-wrapper">
    <div class="mui-scroll">
        <ul class="mui-table-view">
            <h4 class="block-header">已选择模块</h4>
            <li class="mui-table-view-cell">
                <span id="module-name"></span>
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
        <ul id="fault_phenomenon" class="mui-table-view block-vgap">
            <h4 class="block-header">故障现象</h4>
        </ul>
        <div id="textarea" class="mui-input-row mui-hidden" style="margin: 10px 5px;">
            <textarea id="text" rows="5" placeholder="请输入故障现象"></textarea>
        </div>
        <#--<ul class="mui-table-view block-vgap">-->
            <#--<li class="mui-table-view-cell" id="replace-component">-->
                <#--<h4>自修</h4>-->
                <#--<div id="self-repair" class="mui-switch mui-switch-mini">-->
                    <#--<div class="mui-switch-handle"></div>-->
                <#--</div>-->
            <#--</li>-->
        <#--</ul>-->
        <div class="mui-content-padded" style="margin-top: 10%;">
            <button id='next' class="mui-btn mui-btn-block mui-btn-blue">下一步</button>
        </div>
    </div>
</div>
<script src="${PATH}/js/work/faultreason.js" defer="defer"></script>
</body>