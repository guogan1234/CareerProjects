<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
    <link rel="stylesheet" href="/common/plugins/jquery-weui/css/jquery-weui.css">
    <style>
        .report-header {
            padding: 5px 0;
            height: 10px;
        }

        .report-title {
            text-align: center;
            font-size: 21px;
            font-weight: 500;
            margin: 0 15%;
        }

        .baidu-class {
            width: 360px;
            height: 530px;
            border: 0px solid gray;
            padding: 50px 50px;

        }
    </style>
</head>

<body ontouchstart>
<header class='report-header'>
    <h2 class="report-title">X号线 车站分布</h2>
</header>
<div class="weui-cells weui-cells_form">
    <div id="baiduMap" class="baidu-class"></div>
    <p id="aa"></p>
</div>
<@bottom />

<script src="/${PATH}/js/fault/stationMap.js?time=New Date()"></script>

</body>
</html>