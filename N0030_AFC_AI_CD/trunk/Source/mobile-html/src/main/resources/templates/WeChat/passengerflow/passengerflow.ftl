<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
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

        .sbbm-font-color {
            color: #63B8FF;
        }

        .list-font-color {
            color: black;
        }

        #mainForm {
            margin-left: 20%;
            margin-right: 8%;
            margin-top: 0;
            border-radius: 6px 6px 6px 6px;
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
            width: 50px;
            margin-left: 2%;
            margin-top: 20px;
        }

        .list-h4 {
            margin-left: 70px;
            margin-top: -56px;
            color: #CDC9C9;
            font-weight: 300;
        }
    </style>
</head>

<body ontouchstart style="background-color:#EAEAEA;">
<header class='report-header'>
    <h2 class="report-title">客流分析预测</h2>
</header>

<img src="/WeChat/image/pf_list1.png" class="list-img">
<h4 class="list-h4">按车站</h4>
<div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
    <div class="weui-cell sbbmdiv-height">
        <div class="weui-cell__hd">
            <label class="weui-label sbbm-font-color">车站</label>
        </div>
        <div class="weui-cell__bd"></div>
    </div>

    <a href="/skip?path=passengerflow/station/listDetail_contrast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">车站分时客流对比</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/station/listDetail_forecast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">车站分时客流预测</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/station/listDetail_total">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">车站累计客流对比</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/station/listDetail_totalForecast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">车站累计客流预测</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">
    <div class="list-bottom"></div>
</div>

<img src="/WeChat/image/pf_list2.png" class="list-img">
<h4 class="list-h4">按线站</h4>
<div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
    <div class="weui-cell sbbmdiv-height">
        <div class="weui-cell__hd">
            <label class="weui-label sbbm-font-color">线路</label>
        </div>
        <div class="weui-cell__bd"></div>
    </div>

    <a href="/skip?path=passengerflow/line/listDetail_contrast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">线路分时客流对比</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/line/listDetail_forecast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">线路分时客流预测</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/line/listDetail_total">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">线路累计客流对比</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/line/listDetail_totalForecast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">线路累计客流预测</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">
    <div class="list-bottom"></div>
</div>

<img src="/WeChat/image/pf_list3.png" class="list-img">
<h4 class="list-h4">按票卡</h4>
<div class="weui-cells weui-cells_form cells-font-size" id="mainForm" style="margin-bottom:10%;">
    <div class="weui-cell sbbmdiv-height">
        <div class="weui-cell__hd">
            <label class="weui-label sbbm-font-color">票卡</label>
        </div>
        <div class="weui-cell__bd"></div>
    </div>

    <a href="/skip?path=passengerflow/ticket/timecontrast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">票卡分时对比</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">

    <a href="/skip?path=passengerflow/ticket/cumulativecontrast">
        <div class="weui-cell weui-cell_select div-height">
            <div class="weui-cell__hd">
                <label class="weui-label list-font-color" style="width:100%;">票卡累计对比</label>
            </div>
            <div class="weui-cell__bd"></div>
        </div>
    </a>
    <hr class="list-hr">
    <div class="list-bottom"></div>
</div>
<@bottom />

<script src="/common/plugins/jquery-weui/js/jweixin-1.2.0.js"></script>
<script src="/common/plugins/jquery-weui/js/jquery.params.js"></script>

<script>
</script>
</body>
</html>
