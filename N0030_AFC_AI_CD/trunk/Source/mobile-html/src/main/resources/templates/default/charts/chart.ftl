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
    body,td,th {font-family: Verdana, Arial, Helvetica, sans-serif;font-size: 18px;line-height:45px}
    ul{
        list-style:none;
        margin:0px;
        width:100%;

    }
    ul li {
        height:50px;
        width:100%;
        line-height:50px;
        list-style:none;
        border-bottom: 2px solid #666666;
    }
    ul li a{
        height:50px;
        line-height:50px;
        margin: 0px;
        text-decoration:none;
        color:#666666;
    }
</style>
<#include "../common/common.ftl">
<@header />

</head>
<body>
<div class="main" align="center">
    <ul id="lineChart">

    </ul>
</div>
<@bottom />
</body>
<script src="${PATH}/js/charts/chart.js" defer="defer"></script>
</html>
