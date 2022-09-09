<#macro header>
    <meta name="front-end technicist" content="yinxin,lijun">
    <meta name="editor" content="zhulin">
    <meta name="designer" content="once">
    <meta name="pmid" content="14588">
    <meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=no">
    <#--<meta name="viewport" content="width=device-width, initial-scale=1">-->
    <meta name="format-detection" content="telephone=no">
    <meta name="format-detection" content="address=no">
    <meta name="applicable-device" content="mobile">

    <link href="/common/plugins/mui/css/iconfont.css" rel="stylesheet" />
    <link href="/common/plugins/mui/css/mui.min.css" rel="stylesheet" />
    <link href="/common/plugins/mui/css/default.css" rel="stylesheet" />
    <link href="/common/plugins/mui/css/font-awesome.min.css" rel="stylesheet" />
    <!--弹出窗口所需要的样式文件-->
    <link href="/common/plugins/mui/css/mui.picker.css" rel="stylesheet" />
    <link href="/common/plugins/mui/css/mui.picker.min.css" rel="stylesheet" />
    <link href="/common/plugins/mui/css/mui.poppicker.css" rel="stylesheet" />
    <link type="text/css" rel="stylesheet" href="/common/plugins/mzui/css/mzui.min.css">
    <script src="/common/plugins/mzui/js/mzui.min.js"></script>
    <script src="/common/plugins/jQuery/jquery-3.2.1.min.js"></script>
    <script src="/common/plugins/layer.mobile-v2.0/layer_mobile/layer.js"></script>
    <script src="/common/plugins/mui/js/mui.min.js"></script>
    <script src="/common/plugins/mui/js/mui.picker.min.js"></script>
    <#--<script src="/common/plugins/mui/js/mui.indexedlist.js"></script>-->
    <script src="/common/plugins/mui/js/mui.poppicker.js"></script>
    <script src="/common/plugins/mui/js/mui.pullToRefresh.js"></script>
    <script src="/common/plugins/mui/js/mui.pullToRefresh.material.js"></script>
</#macro>

<#macro bottom>
    <div class="with-nav-bottom">
        <#if code ??>
            <input type="hidden" value="${code}" id="code" />
        <#else>
            <input type="hidden" value="" id="code" />
        </#if>
    <#--<nav id="nav-list" class="nav justified dock-bottom outline">
        <a id="empty" href="/skip?path=user/empty"><i class="icon-2x icon-star-empty"></i></a>
            <a id="wrench" href="/skip?path=work/report"><i class="icon-2x icon-wrench"></i></a>
            <a id="chart" href="/skip?path=charts/chart"><i class="icon-2x icon-bar-chart"></i></a>
        </nav>-->
    </div>
</#macro>