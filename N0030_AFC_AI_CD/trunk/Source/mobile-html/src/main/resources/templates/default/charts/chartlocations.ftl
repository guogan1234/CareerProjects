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
        <font>${name}详情</font>
    </div>

    <table border="1" width="100%">
        <tr>
            <td>线路</td>
            <td>故障数量</td>
            <td>完成数量</td>
            <td>跨日故障</td>
            <td>响应率</td>
            <td>环比增长率</td>
        </tr>
        <tbody id="lineChartInfo">
        </tbody>
    </table>
    <div style="margin-top:15px;">
        <em>备注：</em>
        <p>维修响应率=相应时间达标次数/接报故障总次数*100%</p>
        <p>环比增长率=（本月故障数-上月故障数）/上月故障数*100%</p>
    </div>
</div>
<@bottom />
</body>
<script src="${PATH}/js/charts/chartlocations.js" defer="defer"></script>
</html>
