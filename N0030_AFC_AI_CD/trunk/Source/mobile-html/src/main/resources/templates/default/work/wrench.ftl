<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>工单列表</title>
    <meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1,user-scalable=no">
    <#include "../common/common.ftl">
    <@header />
    <style type="text/css">
        html, body { background-color: #efeff4; }
        .mui-bar~.mui-content .mui-fullscreen { top: 44px; height: auto; }
        .mui-bar~.mui-pull-top-tips { top: 24px; }
        .mui-pull-top-tips .mui-pull-loading { margin: 0; }
        .mui-pull-top-wrapper .mui-icon,
        .mui-pull-top-wrapper .mui-spinner { margin-top: 7px; }
        .mui-pull-top-canvas canvas { width: 40px; }
        .mui-media-body:before { content: ""; border-style: solid; border-width: 8px 8px 8px 0px; border-color: transparent #FF943E transparent transparent; height: 0px; position: absolute; left: 60px;top: 21px; width: 0px; }
        .mui-bar .mui-title { right: 100px; left: 100px; }
        .mui-icon { font-size: 16px; }
        .mui-bar-nav.mui-bar .mui-icon.iconfont.icon-scan { margin-right: 15px; }
        .mui-table-view-cell.mui-active { background-color: #fff; }
        .mui-table-view-cell:after { position: fixed; }
        .mui-bar .mui-icon { font-size: 24px; position: relative; z-index: 20; padding-top: 10px; height: 44px; padding-bottom: 10px; }
    </style>
</head>

<body>
<#if code ??>
<input type="hidden" value="${code}" id="code" />
<#else>
<input type="hidden" value="123" id="code" />
</#if>
<div class="mui-content">
    <div class="mui-slider mui-fullscreen">
        <div class="mui-slider-group">
            <div id="item1mobile" class="mui-slider-item mui-control-content mui-active">
                <div id="report-data" class="mui-scroll-wrapper">
                    <div class="mui-scroll">
                        <ul id="workorder-table" class="mui-table-view"></ul>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<script id="workorder-item-template" type="text/template">
    <% for(var i in this){ var item=this[i]; %>
    <li class="mui-table-view-cell mui-media" workorder-id="<%=(item.id)%>">
        <div class="mui-pull-left">
            <img class="mui-media-object mui-pull-left <%=(item.type_en)%>" />
        </div>
        <div class="mui-media-body">
            <div class="mui-table">
                <div class="mui-table-cell mems-workorder">
                    <h5 class="mems-workorder-header <%=(item.type_en)%> ">
                        <div class='mui-ellipsis mui-pull-left mui-col-xs-4 wo-type'>
                            <%=(item.type_cn)%>
                        </div>
                        <div class="mui-ellipsis mui-pull-left mui-col-xs-4 wo-type">
                            <div class="arrow-left"></div>
                        </div>
                        <div class='mui-ellipsis mui-pull-left mui-col-xs-4 wo-status status-<%=(item.statusId)%>'>
                            <i id="status_<%=(item.id)%>" class="mui-tab-label mui-pull-right"><%=(item.status)%></i>
                        </div>
                    </h5>
                    <p><span class="mems-workorder-item">设备：</span>
                        <%=(item.line)%>
                        <%=(item.station)%>
                        <%=(item.device_no)%>
                    </p>
                    <p><span class="mems-workorder-item">报修人：</span>
                        <%=(item.creator)%>
                    </p>
                    <p class="mui-ellipsis"><span class="mems-workorder-item">故障现象：</span>
                        <span id="fault_type_<%=(item.id)%>"><%=(item.fault_type)%></span>
                    </p>
                    <p><span class="mems-workorder-item">更新时间：</span>
                        <%=(item.last_update_date)%>
                    </p>
                    <hr />
                    <p><span class="mui-h6 mui-pull-left">查看详情</span></p>
                </div>
            </div>
        </div>
    </li>
    <% } %>
</script>
<@bottom/>
<script src="${PATH}/js/work/ems.js" data-main="${PATH}/js/work/wrench.js" defer="defer"></script>
</body>