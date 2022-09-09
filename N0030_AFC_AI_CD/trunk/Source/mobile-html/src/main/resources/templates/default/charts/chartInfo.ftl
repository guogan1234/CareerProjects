<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>统计报表</title>
    <style style="text/css">
        body
        {
            background-color: #FFFFFF;
        }
        body,td,th {font-family: Verdana, Arial, Helvetica, sans-serif;font-size: 10px;color: #666666; line-height:45px}
        td{
            text-align:center;
        }
        div em{
            color: #CCCCCC;
        }
         div p{
            margin: 20 0 45;
             color: #CCCCCC;
        }
    </style>
<#include "../common/common.ftl">
<@header />

</head>
<body>
<div class="main">
    <input type="hidden" id="lineId" value="${lineId}" />
    <input type="hidden" id="from" value="${from}" />
    <input type="hidden" id="to" value="${to}" />
    <input type="hidden" id="lineName" value="${nameCn}" />
    <input type="hidden" id="nameCn" value="${name}" />
    <div align="center">
        <font>${name}</font>
    </div>
    <table border="1" width="100%">
        <tr>
            <td>线路</td>
            <td>故障数量</td>
            <td>完成数量</td>
            <td>跨日故障</td>
            <td>响应率</td>
            <td>环比增长率</td>
            <td>详情</td>
        </tr>
        <tr id="lineChartInfo">
        </tr>
    </table>
    <div style="margin-top:15px;">
        <em>备注：</em>
        <p>维修响应率=相应时间达标次数/接报故障总次数*100%</p>
        <p>环比增长率=（本月故障数-上月故障数）/上月故障数*100%</p>
    </div>

    <div align="center" style="margin-top: 50px;">
        <font id="lnName">${name}</font>
    </div>
    <table border="1" width="100%">
        <tr>
            <td>线路</td>
            <td>故障总数</td>
            <td>设备总数</td>
            <td>故障频发率</td>
            <td style="color:red">故障率控制警戒线</td>
            <td>环比增长率</td>
            <td>详情</td>
        </tr>
        <tr id="comChartInfo">
        </tr>
    </table>
    <div style="margin-top:15px;">
        <p>月度故障频发率=当月故障总数/设备总数/月度天数*100%</p>
    </div>
    <div style="margin-top: 25px;">
        <img src="${PATH}/image/u46.jpg">
    </div>
</div>
<@bottom />
</body>
<script src="${PATH}/js/charts/chartInfo.js" defer="defer"></script>
</html>
