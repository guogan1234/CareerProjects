/**
 * Created by gjj on 2017/5/3.
 */

mui.init({
    pullRefresh: {
        container: '#pullrefresh',
        down: {
            callback: mui('.mui-scroll-wrapper').scroll().scrollTo(0, 0, 100)
        },
        up: {
            contentrefresh: '正在加载...',
            callback: mui('.mui-scroll-wrapper').scroll().scrollToBottom(0.0005)
        }
    }
});

$(function () {
    is_token();
    var faultdescid;
    var faultdesc;
    var thisURL = decodeURI(document.URL);
    var params = thisURL.split("&").splice(1,thisURL.split("&").length);
    var locationId;
    var equipmentId ;

    $.each(params,function (i,v) {
        if (v.indexOf("componentId") >= 0) {
            AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + '/components/' + v.split("=")[1],{},function (data_components) {
                var componentTypeId = data_components.typeId;
                var parentId = data_components.parentId;
                var nameCn = data_components.nameCn;
                if( parentId ){
                    AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + '/components/' + parentId,{},function (data) {
                        var p_name = data.nameCn;
                        $("#module-name").text(p_name + ' > ' + nameCn);
                    });
                } else {
                    $("#module-name").text(nameCn);
                }
                AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + '/faultDescriptions/search/findByComponentTypeId',{'componentTypeId':componentTypeId},function (data_faultDescriptions) {
                    var faultDescriptionList = [];
                    if (!('_embedded' in data_faultDescriptions)) {
                        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + '/componentTypes/search/findByParentId',{'parentId':componentTypeId},function (componentTypes) {
                            if ('_embedded' in componentTypes) {
                                /* 循环获取子component下的故障现象 */
                                $.each(componentTypes._embedded.componentTypes, function(index, item) {
                                    var componrentChildId = extractRowId(item);
                                    AJAX_GET("/basic-data-resource/faultDescriptions/search/findByComponentTypeId",{'componentTypeId': componrentChildId},function (data_faultDescriptions) {
                                        if ('_embedded' in data_faultDescriptions) {
                                            var faultTypes_len = data_faultDescriptions._embedded.faultDescriptions.length;
                                            for (var i = 0; i < faultTypes_len; i++) {
                                                faultDescriptionList.push({
                                                    id: extractRowId(data_faultDescriptions._embedded.faultDescriptions[i]),
                                                    nameCn: data_faultDescriptions._embedded.faultDescriptions[i].nameCn,
                                                });
                                            }
                                        }
                                        var html = '';
                                        var faultTypes_len = data_faultDescriptions._embedded.faultDescriptions.length;
                                        for (var i = 0; i < faultTypes_len; i++) {
                                            html+="<li class='mui-table-view-cell'><span>"+data_faultDescriptions._embedded.faultDescriptions[i].nameCn+"</span><div class='mui-switch mui-switch-mini' id='"+extractRowId(data_faultDescriptions._embedded.faultDescriptions[i])+"'> <div class='mui-switch-handle'></div></div></li>";
                                        };
                                        html+="<li class='mui-table-view-cell'><span>其它</span><div class='mui-switch mui-switch-mini' id='faultDesc_-1'> <div class='mui-switch-handle'></div></div></li>";
                                        $("#fault_phenomenon").append(html);
                                        mui('.mui-switch')['switch']();
                                    });
                                });
                            }
                        });
                    } else {
                        var html = '';
                        var faultTypes_len = data_faultDescriptions._embedded.faultDescriptions.length;
                        for (var i = 0; i < faultTypes_len; i++) {
                            html+="<li class='mui-table-view-cell'><span>"+data_faultDescriptions._embedded.faultDescriptions[i].nameCn+"</span><div class='mui-switch mui-switch-mini' id='faultDesc_"+extractRowId(data_faultDescriptions._embedded.faultDescriptions[i])+"'> <div class='mui-switch-handle'></div></div></li>";
                        };
                        html+="<li class='mui-table-view-cell'><span>其它</span><div class='mui-switch mui-switch-mini' id='faultDesc_-1'> <div class='mui-switch-handle'></div></div></li>";
                        $("#fault_phenomenon").append(html);

                        mui('.mui-switch')['switch']();
                    }
                });
            });
        } else if (v.indexOf("station") >= 0) {
            $("#eqipment_station_belong").text(v.split("=")[1]);
        } else if (v.indexOf("line") >= 0) {
            $("#eqipment_line_belong").text(v.split("=")[1])
        } else if (v.indexOf("equipmentName") >= 0) {
            $("#equipment_name").text(v.split("=")[1])
        } else if (v.indexOf("componentName") >= 0) {
            $("#module-name").text(v.split("=")[1])
        } else if( v.indexOf("locationId") >= 0 ){
            locationId = v.split("=")[1];
        } else if( v.indexOf("equipmentId") >= 0 ){
            equipmentId = v.split("=")[1];
        }
    });

    //点击下一页按钮跳转至下一页
    $("#next").click(function(){
        if( faultdescid || faultdesc ){
            if( !faultdesc )faultdesc="";
            var componentId = $("#selected-module").attr("component-id");
            var targetUrl = "/skip?path=work/assignUser&line="+params[0].split("=")[1]+"&station="+params[1].split("=")[1]+"&equipmentName="+params[2].split("=")[1]+"&equipmentId="+equipmentId+"&faultdescid="+faultdescid+"&faultdesc="+faultdesc+ "&locationId=" + locationId;
            window.location.href = targetUrl;
        } else {
            mui.alert("请选择故障现象");
        }
    });
    mui('.mui-content').on('toggle', '#fault_phenomenon div[id^="faultDesc_"]', function(event) {
        var elementid = event.target.id;
        var fList = document.querySelectorAll('#fault_phenomenon div[id^="faultDesc_"]');
        for (var idx = 0; idx < fList.length; idx++) {
            /**对于每一个故障原因**/
            if (fList[idx].id != elementid && fList[idx].classList.contains('mui-active')) {
                fList[idx].classList.remove('mui-active'); //如果id不是active的id，并且包含mui-active
                document.querySelector('#' + fList[idx].id + ' .mui-switch-handle').removeAttribute('style');
            }

            /**对于每一个故障原因**/
            if (fList[idx].id != elementid && fList[idx].classList.contains('mui-active') && faultTypeId != "-1") {
                document.getElementById("fault_append_handle_" + faultTypeId).innerHTML = "";
                fList[idx].classList.remove('mui-active'); //如果id不是active的id，并且包含mui-active
                document.querySelector('#' + fList[idx].id + ' .mui-switch-handle').removeAttribute('style');
            } else if (fList[idx].id != elementid && fList[idx].classList.contains('mui-active') && faultTypeId == "-1") {
                document.getElementById("fault_-1").classList.remove('mui-active'); //如果id不是active的id，并且包含mui-active
                document.querySelector('#fault_-1 .mui-switch-handle').removeAttribute('style');
            }
        }

        if (event.detail.isActive) {
            faultdescid = elementid.split("_")[1];
            /**判断id**/
            if (elementid.indexOf("-1") != -1) {
                document.querySelector("#textarea").setAttribute("class", "mui-input-row");
                return;
            } else {
                document.querySelector("#textarea").setAttribute("class", "mui-input-row mui-hidden");
            }

        } else {
            document.querySelector("#textarea").setAttribute("class", "mui-input-row mui-hidden");
        }
    });

});

function extractRowId(row){
    var url = (typeof(row) == 'object' && '_links' in row) ? row._links.self.href : row;
    var reg = /\/(\w+)$/g;
    var _id = reg.exec(url)[1];
    return _id;
}

function handleId(str, lastindex) {
    var id = null;
    if (str != null) {
        var position_lastindex = str.lastIndexOf(lastindex);
    }
    if (position_lastindex) {
        id = str.substr(position_lastindex + 1);
    }
    return id;
}
