<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
    <style>
        .report-header {
            padding: 5px 0;
            padding-top: 50px;
            padding-top: 5px;
        }

        .div-height {
            height: 35px;
            padding: 5px 5px;

        }

        .report-title {
            text-align: center;
            font-size: 21px;
            font-weight: 500;
            margin: 0 15%;
        }

        .cells-font-size {
            font-size: 18px;
            margin-top: 50px;
            margin-buttom: 50px;
        }

        .list-font-color {
            color: black;
        }

        #mainForm {
            margin-left: 20%;
            margin-right: 15%;
            margin-top: 10px;
            margin-buttom: 30px;
            border-radius: 6px 6px 6px 6px;
            width: 70%;
        }

        .list-hr {
            width: 90%;
            margin-left: 5%;
            border: 1px solid #EAEAEA;
        }

        .list-bottom {
            margin-bottom: 10%;
        }

        .list-img {
            width: 55px;
            margin-left: 1%;
            margin-top: 80px;
            margin-bottom: 10px;
        }

        p {
            color: gray;
            margin-left: 3%;
            font-size: 15px;
        }

        .col-energy-analysis {
            position: absolute;
            left: 15px;
            width: 100%;
            margin-top: 10%;
        }

        .weui-cell:before {
            border-top: 0px solid #D9D9D9 !important;
        }

        #style {
            padding-right: 15px;
            padding-left: 15px;
        }

    </style>
</head>

<body ontouchstart style="background-color:#EAEAEA;" id="style">

<header class='report-header'>
    <h2 class="report-title">耗材分析与预测</h2>
</header>

<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">

    <div class="col-energy-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
            <a href="/skip?path=consumptive/consumptionAnalysis">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">消耗分析</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>根据历史数据分析配件消耗情况</p>
            <div class="list-bottom"></div>

        </div>
    </div>
</div>
<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">
    <div class="col-energy-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">

            <a href="/skip?path=consumptive/consumptionForecast">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">消耗预测</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>根据历史数据预测配件消耗情况</p>
            <div class="list-bottom"></div>
        </div>
    </div>
</div>

<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">
    <div class="col-energy-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
            <a href="/skip?path=consumptive/consumablesTrend">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">耗材去向分析</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>根据历史数据分析耗材去向情况</p>
            <div class="list-bottom"></div>
        </div>
    </div>
</div>
<@bottom />

<script src="/common/plugins/jquery-weui/js/jweixin-1.2.0.js"></script>
<script src="/common/plugins/jquery-weui/js/jquery.params.js"></script>

<script>
</script>
</body>
</html>
