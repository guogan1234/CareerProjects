/**
 * Created by gjj on 2017/4/28.
 */
define(function(require, exports, module) {
    is_token();
    var tp = require('tp');
    var locationId;

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

        var equipmentId ;
        $("#selected-module").text('整机');

        var thisURL = decodeURI(document.URL);
        var params = thisURL.split("&").splice(1,thisURL.split("&").length);
        $.each(params,function (i,v) {
            if(v.indexOf("equipmentId") >= 0 ){
                equipmentId = v.split("=")[1];
                AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + "/components/search/findByEquipmentId",{equipmentId:v.split("=")[1]},function (data_components) {
                    console.log("服务器响应: 依据equipmentId获取components服务成功");
                    var components = [];
                    var sub_components = [];
                    if ('_embedded' in data_components) {
                        var components_len = data_components._embedded.components.length;
                        for (var i = 0; i < components_len; i++) {
                            if (data_components._embedded.components[i].parentId == null) {
                                components.push({
                                    id: extractRowId(data_components._embedded.components[i]),
                                    parentId: data_components._embedded.components[i].parentId,
                                    typeId: data_components._embedded.components[i].typeId,
                                    equipmentId: data_components._embedded.components[i].equipmentId,
                                    deviceId: data_components._embedded.components[i].deviceId,
                                    nameCn: data_components._embedded.components[i].nameCn
                                });
                            } else {
                                sub_components.push({
                                    id: extractRowId(data_components._embedded.components[i]),
                                    parentId: data_components._embedded.components[i].parentId,
                                    equipmentId: data_components._embedded.components[i].equipmentId,
                                    typeId: data_components._embedded.components[i].typeId,
                                    deviceId: data_components._embedded.components[i].deviceId,
                                    nameCn: data_components._embedded.components[i].nameCn
                                });
                            }


                        }
                    }

                    var ul = document.getElementById("mod"); //获取模块列表的ul

                    if (components.length) { //如果components含有元素（一级元素）,根据元素个数来决定li的多少
                        for (var i = 0; i < components.length; i++) {
                            var li = document.createElement("li");
                            var a = document.createElement("a");
                            var ul_children = document.createElement("ul");

                            li.setAttribute("class", "mui-table-view-cell mui-collapse"); //设置li的属性
                            a.setAttribute("class", "mui-navigate-right");
                            a.setAttribute("id", "components_" + components[i].id); //设置a的属性
                            ul_children.setAttribute("class", "mui-table-view mui-table-view-radio");

                            li.appendChild(a);
                            li.appendChild(ul_children);
                            ul.appendChild(li);

                            a.innerHTML = components[i].nameCn;

                            if (sub_components.length) { //如果sub_components含有元素（二级元素）,根据元素个数来决定li的多少
                                console.log("sub_components.length : " + sub_components.length);
                                for (var j = 0; j < sub_components.length; j++) { //循环所有二级元素
                                    if (sub_components[j].parentId == components[i].id) { //如果二级元素属于一级元素的子元素,则应将放入对应的li下
                                        var li_children = document.createElement("li");
                                        var a_children = document.createElement("a");
                                        li_children.setAttribute("class", "mui-table-view-cell");
                                        a_children.setAttribute("class", "mui-navigate-right");
                                        a_children.setAttribute("id", "sub_components_" + sub_components[j].id);

                                        li_children.appendChild(a_children);
                                        ul_children.appendChild(li_children);

                                        a_children.innerHTML = sub_components[j].nameCn;

                                    }
                                }
                            }
                        }

                    }
                });
            } else if( v.indexOf("station") >= 0 ){
                $("#eqipment_station_belong").text(v.split("=")[1]);
            } else if( v.indexOf("line") >= 0 ){
                $("#eqipment_line_belong").text(v.split("=")[1])
            } else if( v.indexOf("equipmentName") >= 0 ){
                $("#equipment_name").text(v.split("=")[1])
            } else if( v.indexOf("locationId") >= 0 ){
                locationId = v.split("=")[1];
            }

        });

        //点击下一页按钮跳转至下一页
        $("#next").click(function(){
            var componentId = $("#selected-module").attr("component-id");
            if (!componentId) {
                mui.alert("至少选择一项故障模块");
                return;
            } else {
                var equipName = $("#selected-module").text();
                var targetUrl = "/skip?path=work/faulttype&line=" + params[0].split("=")[1] + "&equipmentId=" + equipmentId+ "&station=" + params[1].split("=")[1] + "&equipmentName=" + params[2].split("=")[1] + "&componentId=" + componentId+ "&locationId=" + locationId;
                window.location.href = targetUrl;
            }
        });

        mui('#mod').on('tap', 'a', function(e) {
            var sourceid = e.detail.target.id;
            var componentid = handleId(sourceid, '_');
            if (sourceid.substr(0, 3) == "sub") {
                AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + '/components/' + componentid,{},function (data_components) {
                    var parentid = data_components.parentId;
                    var nameCn = data_components.nameCn;
                    AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + '/components/' + parentid,{},function (data) {
                        var p_name = data.nameCn;
                        $("#selected-module").text(p_name + ' > ' + nameCn);
                        $("#selected-module").attr("component-id",componentid);
                    });
                });
            } else {
                if (sourceid.substr(0, 3) == "com") {
                    var componentList = document.getElementById(sourceid).nextSibling.childNodes;
                    mui.each(componentList, function(index, item) {
                        if (item.className.indexOf("mui-selected") != -1) {
                            var componentParent = document.getElementById(sourceid).parentNode;
                            componentParent.className = componentParent.className.replace("mui-active", "");
                            item.className = item.className.replace(" mui-selected", "")
                            var modObject = document.getElementById("mod");
                            mui.trigger(modObject, "tap");
                        }
                    });
                    $("#selected-module").text(e.detail.target.innerText);
                    $("#selected-module").attr("component-id",componentid);
                }
            }
        });
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