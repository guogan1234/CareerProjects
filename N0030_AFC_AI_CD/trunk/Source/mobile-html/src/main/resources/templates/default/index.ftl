<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>MEMS-移动运维</title>
    <#include "common/common.ftl">
    <@header />

</head>
<body>
    <#if code ??>
        <input type="hidden" value="${code}" id="code" />
    <#else>
        <input type="hidden" value="123" id="code" />
    </#if>
    <@bottom />
</body>
<script src="${PATH}/js/index.js"></script>
<script src="http://res.wx.qq.com/open/js/jweixin-1.2.0.js" type="text/javascript"></script>
</html>

