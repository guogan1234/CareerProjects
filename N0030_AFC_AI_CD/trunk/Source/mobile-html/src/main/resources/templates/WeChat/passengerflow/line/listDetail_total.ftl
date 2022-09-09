<!DOCTYPE html>
<#include "../../common/common.ftl">
<html>
<head>
<@header />
    <link rel="stylesheet" href="/common/plugins/vertical-timeline/css/reset.css">
    <link rel="stylesheet" href="/common/plugins/vertical-timeline/css/style.css">
    <style>
        .div-select {
            position: fixed;
            top: 32px;
            right: 0;
            z-index: 99;
            height: 30px;
            width: 60px;
            background-color: #B2DFEE;
            border-radius: 6px 0 0 6px;
        }

        .label-font {
            color: white;
            font-size: 20px;
            margin-left: 11px;
            margin-bottom: -50px;
            line-height: 25px;
            padding-bottom: -25px;
        }

        .listdetail {
            height: 250px;
            margin-top: 5px;
        }

        .listdetail-div {
            min-width: 10px;
            height: 250px;
        }
    </style>
</head>

<body ontouchstart>
<a href="/skip?path=passengerflow/line/select_total">
    <div class="div-select">
        <label class="label-font">筛选</label>
    </div>
</a>
<div>
    <div class="listdetail">
        <div id="container1" class="listdetail-div"></div>
    </div>
    <hr style="background-color:#FFF0F5;height: 1px;border: none;">
    <div class="listdetail">
        <div id="container2" class="listdetail-div"></div>
    </div>
    <hr style="background-color:#FFF0F5;height: 1px;border: none;">
    <div class="listdetail">
        <div id="container3" class="listdetail-div"></div>
    </div>
</div>
<@bottom />

<script src="/${PATH}/js/passengerflow/line/listDetail_total.js?time=New Date()"></script>

</body>
</html>
