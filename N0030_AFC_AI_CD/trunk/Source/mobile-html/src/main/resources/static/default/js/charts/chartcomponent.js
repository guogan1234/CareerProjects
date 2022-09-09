/**
 * Created by zhoujs on 2017/5/2.
 */
$(function(){
        var lineId = $("#lineId").val();
        var from = $("#from").val();
        var to = $("#to").val();
        var param="/workOrderCharts/getWorkOrderComCharts?statsId=6,10&lineId="+lineId+"&from="+from+"&to="+to;
        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_WORK_ORDER+param,null,function(data){
            var str="";
            var lnName=$("#lnName").html().replace("线路站点",'('+data.monthDay+'天)模块');
            $("#lnName").html(lnName);
            var locationComs = data.locationCom;
            if(locationComs.length>0){
            for(var i = 0;i<locationComs.length;i++){
                    str +="<tr><td>"+locationComs[i].locationName+"</td><td>"+locationComs[i].faultNumber+"</td><td>" +
                        locationComs[i].componentNumber+"</td><td>"+locationComs[i].faultProportion+"</td><td style='color: red;'>" +
                        locationComs[i].faultCordon+"</td><td>"+locationComs[i].faultMom+"</td>" +
                        "</tr>";
            }
            $("#comChartInfo").html(str);
            }else{
                $("#comChartInfo").html("<tr><td colspan='6'>暂无数据</td></tr>");
            }
        },function (XMLHttpRequest, textStatus, errorThrown) {
            layer.open({
                content: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN,
                btn: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN_BTN
            });
        });
    }
);
