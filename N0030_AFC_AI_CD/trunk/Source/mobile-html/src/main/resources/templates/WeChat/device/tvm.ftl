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
            height: 510px;
        }

        .iframe-svg {
            width: auto;
            height: auto;
            width: 700px;
            height: 510px;
        }
    </style>
</head>

<body ontouchstart class="body-div">

<iframe id="tvmsvg" class="iframe-svg" src="../WeChat/image/tvm.svg"></iframe>
<@bottom />

<script src="/${PATH}/js/common/push.js"></script>
<script src="/${PATH}/js/device/tvm.js?time=New Date()"></script>

</body>
</html>
