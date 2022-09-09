<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
    <link rel="stylesheet" href="/common/plugins/vertical-timeline/css/reset.css">
    <link rel="stylesheet" href="/common/plugins/vertical-timeline/css/style.css">
    <style>
        .body-div {
            background-color: #EAEAEA;
            transform: rotate(90deg);
            height: 75%;
        }

        .iframe-svg {
            width: auto;
            height: auto;
            width: 180%;
            height: 100%;
            margin-left: -15%;
        }
    </style>
</head>

<body ontouchstart class="body-div">

<iframe id="stationssvg" class="iframe-svg" src="../WeChat/image/stations.svg"></iframe>
<@bottom />

<script src="/${PATH}/js/common/push.js"></script>
<script src="/${PATH}/js/device/stations.js?time=New Date()"></script>

</body>
</html>
