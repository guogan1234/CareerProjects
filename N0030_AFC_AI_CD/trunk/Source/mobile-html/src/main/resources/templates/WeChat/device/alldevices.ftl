<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
    <link rel="stylesheet" href="/common/plugins/vertical-timeline/css/reset.css">
    <link rel="stylesheet" href="/common/plugins/vertical-timeline/css/style.css">
    <style>
        .body-class {
            background-color: #EAEAEA;
            transform: rotate(90deg);
            height: 210%;
        }

        .div-iframe {
            margin-left: -117%;
        }

        .iframe-svg1 {
            width: auto;
            height: auto;
            width: 750px;
            height: 480px;
        }

        .iframe-svg2 {
            width: auto;
            height: auto;
            width: 750px;
            height: 378px;
        }

        .iframe-svg3 {
            width: auto;
            height: auto;
            height: 180px;
            width: 750px;
        }
    </style>
</head>

<body ontouchstart class="body-class">

<div class="div-iframe">
    <iframe id="alldevices_gatsvg" class="iframe-svg1" src="../WeChat/image/alldevices_GAT.svg"></iframe>
</div>
<div class="div-iframe">
    <iframe id="alldevices_tvmsvg" class="iframe-svg2" src="../WeChat/image/alldevices_TVM.svg"></iframe>
</div>
<div class="div-iframe" style="height:108px;">
    <iframe id="alldevices_bomsvg" class="iframe-svg3" src="../WeChat/image/alldevices_BOM.svg"></iframe>
</div>
<@bottom />

<script src="/${PATH}/js/common/push.js"></script>
<script src="/${PATH}/js/device/alldevices.js?time=New Date()"></script>

</body>
</html>
