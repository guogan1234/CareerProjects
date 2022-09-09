<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>报表</title>
    <style style="text/css">
        body,td,th {font-family: Verdana, Arial, Helvetica, sans-serif;font-size: 12px;color: #666666; line-height:45px}
        td{
            text-align:center;
        }
        div em{
            margin-left:35px;
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
    <div align="center">
        <font id="lnName">${name}</font>
    </div>

    <table border="1" width="100%">
        <tr>
            <td>模块名称</td>
            <td>故障总数</td>
            <td>设备总数</td>
            <td>故障频率</td>
            <td style='color: red;'>故障率警戒控制线</td>
            <td>环比增长率</td>
        </tr>
        <tbody id="comChartInfo">
        </tbody>
    </table>
    <div style="margin-top:15px;">
        <p>月度故障频发率=当月故障总数/设备总数/月度天数*100%</p>
    </div>
</div>
<@bottom />
</body>
<script src="${PATH}/js/charts/chartcomponent.js" defer="defer"></script>
</html>
