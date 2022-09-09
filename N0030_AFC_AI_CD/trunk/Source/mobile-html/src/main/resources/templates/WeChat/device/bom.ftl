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
            height: 410px;
        }

        .iframe-svg {
            width: auto;
            height: auto;
            width: 750px;
            height: 410px;
        }
    </style>
</head>

<body ontouchstart class="body-div">

<iframe id="bomsvg" class="iframe-svg" src="../WeChat/image/bom.svg"></iframe>
<@bottom />

<script src="/${PATH}/js/common/push.js"></script>
<script src="/${PATH}/js/device/bom.js?time=New Date()"></script>

</body>
</html>
