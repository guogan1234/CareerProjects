var interId;
var username;
$(function(){
        WX_auth();
        interId = window.setInterval(intervalUsername,1000);
    // is_token();
    activeTable('wrench');
    });

function intervalUsername(){

    if(null !=JSON.parse(sessionStorage.getItem("USER")).username){
        username =JSON.parse(sessionStorage.getItem("USER")).username;
        selChartInfo();
    }
}

function selChartInfo(){
    window.clearInterval(interId);
    var param ="/locations/getUserLocations?username="+username;
    AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA+param,null,function(data){
            var str="";
            for(var i = 0;i<data.length;i++){
                str += "<li><a href='/skip?path=charts/chartInfo&name="+data[i].nameCn+"&lineId="+data[i].lineid+"&from="+data[i].fromDate
                    +"&to="+data[i].toDate+"&nameCn="+data[i].nameEn+"'>"+data[i].nameCn+"</a></li>"
            }
            $("#lineChart").html(str);

    },function (XMLHttpRequest, textStatus, errorThrown) {
        layer.open({
            content: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN,
            btn: CONSTANTS.MASSAGE.RESULTM.RESULT_NOTN_BTN
        });
    });
}