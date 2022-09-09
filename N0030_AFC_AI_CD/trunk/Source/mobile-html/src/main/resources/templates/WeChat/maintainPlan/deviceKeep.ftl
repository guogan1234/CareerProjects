<!DOCTYPE html>
<#include "../common/common.ftl">
<html>
<head>
<@header />
    <link rel="stylesheet" href="/common/plugins/jquery-weui/css/jquery-weui.css">
    <style>
        .img-class {
            width: 100%;
            height: 50%;
        }

        .h5-class {
            text-align: center;
        }

        .hr-class {
            background-color: #FDF5E6;
            height: 1px;
            border: none;
        }

        .details-class {
            position: relative;
        }

        .summary-class {
            position: absolute;
            top: -25px;
            left: 5px;
        }

        .summary-class2 {
            position: absolute;
            top: -25px;
            left: 15px;
        }

        table.table-class {
            width: 100%;
            font-family: verdana, arial, sans-serif;
            font-size: 11px;
            color: #333333;
            border-width: 1px;
            border-color: #FFFAFA;
            border-collapse: collapse;
        }

        table.table-class th {
            border-width: 1px;
            padding: 8px;
            border-style: solid;
            border-color: #FFFAFA;
        }

        table.table-class td {
            text-align: center;
            border-width: 1px;
            padding: 8px;
            border-style: solid;
            border-color: #FFFAFA;
        }

        .oddrowcolor {
            background-color: #D1EEEE;
        }

        .evenrowcolor {
            background-color: #F0F8FF;
        }
    </style>
</head>

<body ontouchstart>

<img src="../WeChat/image/maintain.png" class="img-class"/>

<div>
    <h5 class="h5-class">1号线</h5>
    <hr class="hr-class"/>
    <details open class="details-class">
        <summary class="summary-class">
        </summary>

        <h6 class="h5-class">1号线-车站1</h6>
        <hr class="hr-class"/>
        <details open class="details-class">
            <summary class="summary-class2">
            </summary>
            <table class="table-class">
                <tr>
                    <th>设备</th>
                    <th>保养计划</th>
                </tr>
                <tr>
                    <td>设备1</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备2</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备3</td>
                    <td>已生成</td>
                </tr>
            </table>
        </details>
        <h6 class="h5-class">1号线-车站2</h6>
        <hr class="hr-class"/>
        <details open class="details-class">
            <summary class="summary-class2">
            </summary>
            <table class="table-class">
                <tr>
                    <th>设备</th>
                    <th>保养计划</th>
                </tr>
                <tr>
                    <td>设备1</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备2</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备3</td>
                    <td>已生成</td>
                </tr>
            </table>
        </details>
    </details>
</div>
<div>
    <h5 class="h5-class">2号线</h5>
    <hr class="hr-class"/>
    <details open class="details-class">
        <summary class="summary-class">
        </summary>

        <h6 class="h5-class">2号线-车站1</h6>
        <hr class="hr-class"/>
        <details open class="details-class">
            <summary class="summary-class2">
            </summary>
            <table class="table-class">
                <tr>
                    <th>设备</th>
                    <th>保养计划</th>
                </tr>
                <tr>
                    <td>设备1</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备2</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备3</td>
                    <td>已生成</td>
                </tr>
            </table>
        </details>
        <h6 class="h5-class">2号线-车站2</h6>
        <hr class="hr-class"/>
        <details open class="details-class">
            <summary class="summary-class2">
            </summary>
            <table class="table-class">
                <tr>
                    <th>设备</th>
                    <th>保养计划</th>
                </tr>
                <tr>
                    <td>设备1</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备2</td>
                    <td>已生成</td>
                </tr>
                <tr>
                    <td>设备3</td>
                    <td>已生成</td>
                </tr>
            </table>
        </details>
    </details>
</div>
<@bottom />

<script src="/${PATH}/js/maintainPlan/deviceKeep.js?time=New Date()"></script>

</body>
</html>