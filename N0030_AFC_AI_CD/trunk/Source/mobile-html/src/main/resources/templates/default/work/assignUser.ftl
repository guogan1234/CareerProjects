<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>移动运维</title>
    <meta name="viewport" content="width=device-width, initial-scale=1,maximum-scale=1,user-scalable=no">
    <#include "../common/common.ftl">
    <@header />
    <style>
        html, body { height: 100%; overflow: hidden; }
        #list { display: none; }
    </style>
</head>

<body>
<div class="mui-content">
    <div id='list' class="mui-indexed-list">
        <div class="mui-indexed-list-search mui-input-row mui-search">
            <input id="test" type="search" class="mui-input-clear mui-indexed-list-search-input" placeholder="搜索">
        </div>
        <script type="text/template" id="assign-user-template">
            <% for(var i in this) {var user = this[i] %>
            <li data-uid="<%=user._links.self.href %>" data-value="<%=user.fullName %>" data-tags="<%=user.fullName %>" class="mui-table-view-cell mui-indexed-list-item mui-radio mui-left"><%=user.fullName %></li>
            <% } %>
        </script>
        <div class="mui-indexed-list-alert"></div>
        <div class="mui-indexed-list-inner">
            <ul class="mui-table-view" id="assign-user-table">
                <li data-group="热" class="mui-table-view-divider mui-indexed-list-group">最近选择的人</li>
                <li data-value="1" data-tags="ShangHai" class="mui-table-view-cell mui-indexed-list-item mui-checkbox mui-left"><input type="checkbox" />张三</li>
                <li data-group="S" class="mui-table-view-divider mui-indexed-list-group">联系人</li>
                <li data-value="1" data-tags="ShangHai" class="mui-table-view-cell mui-indexed-list-item mui-checkbox mui-left"><input type="checkbox" />李四</li>
                <li data-value="2" data-tags="ShengZhen" class="mui-table-view-cell mui-indexed-list-item mui-checkbox mui-left"><input type="checkbox" />王五</li>
                <li data-value="2" data-tags="ShengZhen" class="mui-table-view-cell mui-indexed-list-item mui-checkbox mui-left"><input type="checkbox" />赵六</li>
                <li data-value="2" data-tags="ShengZhen" class="mui-table-view-cell mui-indexed-list-item mui-checkbox mui-left"><input type="checkbox" />冯七</li>
                <li data-value="2" data-tags="ShengZhen" class="mui-table-view-cell mui-indexed-list-item mui-checkbox mui-left"><input type="checkbox" />马九</li>
            </ul>
        </div>
    </div>
</div>
<script src="${PATH}/js/work/ems.js" data-main="${PATH}/js/work/assignUser.js" defer="defer"></script>
</body>