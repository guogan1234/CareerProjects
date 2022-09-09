<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>工单报修</title>
    <meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1,user-scalable=no">
    <#include "../common/common.ftl">
    <@header />
    <#--<style type="text/css">-->
        <#--html, body { background-color: #efeff4; }-->
        <#--.mui-bar~.mui-content .mui-fullscreen { top: 44px; height: auto; }-->
        <#--.mui-bar~.mui-pull-top-tips { top: 24px; }-->
        <#--.mui-pull-top-tips .mui-pull-loading { margin: 0; }-->
        <#--.mui-pull-top-wrapper .mui-icon,-->
        <#--.mui-pull-top-wrapper .mui-spinner { margin-top: 7px; }-->
        <#--.mui-pull-top-canvas canvas { width: 40px; }-->
        <#--.mui-media-body:before { content: ""; border-style: solid; border-width: 8px 8px 8px 0px; border-color: transparent #FF943E transparent transparent; height: 0px; position: absolute; left: 60px;top: 21px; width: 0px; }-->
        <#--.mui-bar .mui-title { right: 100px; left: 100px; }-->
        <#--.mui-icon { font-size: 16px; }-->
        <#--.mui-bar-nav.mui-bar .mui-icon.iconfont.icon-scan { margin-right: 15px; }-->
        <#--.mui-table-view-cell.mui-active { background-color: #fff; }-->
        <#--.mui-table-view-cell:after { position: fixed; }-->
        <#--.mui-bar .mui-icon { font-size: 24px; position: relative; z-index: 20; padding-top: 10px; height: 44px; padding-bottom: 10px; }-->
    <#--</style>-->
</head>

<body>
<#if code ??>
<input type="hidden" value="${code}" id="code" />
<#else>
<input type="hidden" value="123" id="code" />
</#if>
<@bottom/>
<script src="${PATH}/js/work/ems.js" data-main="${PATH}/js/work/report.js" defer="defer"></script>
</body>