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
            height: 25px;
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
            margin-top: 30px;
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
            width: 55px;
            margin-left: 1%;
            margin-top: 50px;
            margin-bottom: 15px;
        }

        p {
            color: gray;
            margin-left: 3%;
            font-size: 15px;
        }

        .col-fault-analysis {
            position: absolute;
            left: 15px;
            width: 100%;
        }

        .weui-cell:before {
            border-top: 0px solid #D9D9D9 !important;
        }

        .div-select {
            position: fixed;
            top: 30px;
            right: 0;
            z-index: 99;
            height: 30px;
            width: 95px;
            background-color: #B2DFEE;
            border-radius: 6px 0 0 6px;
        }

        .label-font {
            color: white;
            font-size: 20px;
            margin-left: 11px;
            margin-bottom: -45px;
            line-height: 25px;
            padding-bottom: -25px;
        }

        #style {
            padding-right: 15px;
            padding-left: 15px;
        }
    </style>
</head>

<body ontouchstart style="background-color:#EAEAEA;" id="style">

<header class='report-header'>
    <h2 class="report-title">????????????</h2>
</header>
<a href="/skip?path=fault/stationMap">
    <div class="div-select">
        <label class="label-font">????????????</label>
    </div>
</a>
<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">

    <div class="col-fault-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
            <a href="/skip?path=fault/typicalFault">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">??????????????????</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>?????????????????????????????????</p>
            <div class="list-bottom"></div>

        </div>
    </div>
</div>
<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">
    <div class="col-fault-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">

            <a href="/skip?path=fault/moduleFailure">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">??????????????????</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>?????????????????????????????????</p>
            <div class="list-bottom"></div>
        </div>
    </div>
</div>

<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">
    <div class="col-fault-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
            <a href="/skip?path=fault/failureDistribution">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">??????????????????</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>?????????????????????????????????</p>
            <div class="list-bottom"></div>
        </div>
    </div>
</div>
<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">
    <div class="col-fault-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
            <a href="/skip?path=fault/rockProbability">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">?????????????????????</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>????????????????????????????????????</p>
            <div class="list-bottom"></div>
        </div>
    </div>
</div>
<div class="weui-cell">
    <img src="/WeChat/image/pf_list1.png" class="list-img">
    <div class="col-fault-analysis">
        <div class="weui-cells weui-cells_form cells-font-size" id="mainForm">
            <a href="/skip?path=fault/failureFactor">
                <div class="weui-cell weui-cell_select div-height">
                    <div class="weui-cell__hd">
                        <label class="weui-label list-font-color" style="width:100%;">??????????????????</label>
                    </div>
                    <div class="weui-cell__bd"></div>
                </div>
            </a>
            <hr class="list-hr">
            <p>????????????????????????????????????</p>
            <div class="list-bottom"></div>
        </div>
    </div>
</div>
<@bottom />

<script src="/common/plugins/jquery-weui/js/jweixin-1.2.0.js"></script>
<script src="/common/plugins/jquery-weui/js/jquery.params.js"></script>

</body>
</html>
