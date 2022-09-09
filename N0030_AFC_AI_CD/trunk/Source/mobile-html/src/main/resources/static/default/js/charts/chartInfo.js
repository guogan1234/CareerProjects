/**
 * Created by zhoujs on 2017/5/2.
 */
$(function(){
        var lineId = $("#lineId").val();
        var from = $("#from").val();
        var to = $("#to").val();
        var nameEn = $("#lineName").val();
        var nameCn = $("#nameCn").val();
        var param="/workOrderCharts/getLineCharts?statsId=6,10&lineId="+lineId+"&from="+from+"&to="+to;
        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_WORK_ORDER+param,null,function(data){
            var str="";
                str +="<td>"+nameEn+"</td><td>"+data.workOrderChart.alllocationOrEquipmentOrder+"</td><td>" +
                    data.workOrderChart.completelocationOrEquipmentOrder+"</td><td>"+data.workOrderChart.extendDateOrder+"</td><td>" +
                    data.workOrderChart.compleProportion+"</td><td>"+data.workOrderChart.minusMonthProportion+"</td>" +
                    "<td><a href='/skip?path=charts/chartlocations&name="+nameCn+"&lineId="+lineId+"&from="+from
                    +"&to="+to+"&nameCn="+nameEn+"' style='text-decoration:underline;color: blue;' >详情</a></td>";
            $("#lineChartInfo").html(str);
            var comStr = "";
            var lnName=$("#lnName").html().replace("线路站点",'('+data.monthDay+'天)模块');
             $("#lnName").html(lnName);
            comStr +="<td>"+nameEn+"</td><td>"+data.lcChart.faultNumber+"</td><td>" +
                data.lcChart.componentNumber+"</td><td>"+data.lcChart.faultProportion+"</td><td style='color:red'>" +
                data.lcChart.faultCordon+"</td><td>"+data.lcChart.faultMom+"</td>" +
                "<td><a href='/skip?path=charts/chartcomponent&name="+nameCn+"&lineId="+lineId+"&from="+from
                +"&to="+to+"&nameCn="+nameEn+"' style='text-decoration:underline;color: blue;' >详情</a></td>";
            $("#comChartInfo").html(comStr);
        },function (XMLHttpRequest, textStatus, errorThrown) {
            layer.open({
                content: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN,
                btn: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN_BTN
            });
        });
    }
);
