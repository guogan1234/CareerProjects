/**
 * Created by zhoujs on 2017/5/2.
 */
$(function(){
        var lineId = $("#lineId").val();
        var from = $("#from").val();
        var to = $("#to").val();
        var param="/workOrderCharts/getLocationCharts?statsId=6,10&lineId="+lineId+"&from="+from+"&to="+to;
        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_WORK_ORDER+param,null,function(data){
            var str="";
            for(var i = 0;i<data.length;i++){
                if(i==0){
                    str +="<tr><td>"+$("#lineName").val()+"</td><td>"+data[i].alllocationOrEquipmentOrder+"</td><td>" +
                        data[i].completelocationOrEquipmentOrder+"</td><td>"+data[i].extendDateOrder+"</td><td>" +
                        data[i].compleProportion+"</td><td>"+data[i].minusMonthProportion+"</td>" +
                        "</tr>";
                }else{
                    str +="<tr><td>"+data[i].licationEquipment+"</td><td>"+data[i].alllocationOrEquipmentOrder+"</td><td>" +
                        data[i].completelocationOrEquipmentOrder+"</td><td>"+data[i].extendDateOrder+"</td><td>" +
                        data[i].compleProportion+"</td><td>"+data[i].minusMonthProportion+"</td>" +
                        "</tr>";
                }

            }
            $("#lineChartInfo").html(str);
        },function (XMLHttpRequest, textStatus, errorThrown) {
            layer.open({
                content: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN,
                btn: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN_BTN
            });
        });
    }
);
