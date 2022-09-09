<#macro header>
<title>南京地铁决策支持系统</title>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=no">
<link rel="stylesheet" href="/common/plugins/jquery-weui/lib/weui.css">
<link rel="stylesheet" href="/common/plugins/jquery-weui/css/jquery-weui.css">
<link rel="stylesheet" href="/common/plugins/jquery-weui/demos/css/demos.css">
</#macro>

<#macro bottom>

    <#if code??> <input type="hidden" id="code" value="${code}"> </#if>
    <#if CORPID??> <input type="hidden" id="corpid" value="${CORPID}"> </#if>
    <#if AGENTID??> <input type="hidden" id="agentid" value="${AGENTID}"> </#if>
    <#if CORPSECRET??> <input type="hidden" id="corpsecret" value="${CORPSECRET}"> </#if>
    <#if auth_code??> <input type="hidden" id="authCode" value="${auth_code}"> </#if>

<script src="/common/plugins/jquery-weui/lib/jquery-2.1.4.js"></script>
<script src="/common/plugins/jquery-weui/lib/fastclick.js"></script>
<script>
    $(function () {
        FastClick.attach(document.body);
        //关闭   Highcharts中默认开启了UTC（世界标准时间），由于中国所在时区为+8，所以经过 Highcharts 的处理后会减去8个小时。
        Highcharts.setOptions({global: {useUTC: false}});
    });

</script>
<script src="/common/plugins/jquery-weui/js/moment.min.js"></script>
<script src="/common/plugins/jquery-weui/js/jquery-weui.js"></script>
<script src="/common/plugins/Highcharts-4.2.3/js/highcharts.js"></script>
<script src="/common/plugins/sockjs/sockjs.min.js"></script>
<script src="/common/plugins/stomp/stomp.min.js"></script>
<script type="text/javascript" src="http://api.map.baidu.com/api?v=2.0&ak=uaCTQU12aSNKndsXFi3m7qZG"></script>
<script src="/${PATH}/js/common/ajax-api.js"></script>
<script src="/${PATH}/js/common/common.js"></script>
<script src="/${PATH}/js/common/constants.js"></script>

</#macro>