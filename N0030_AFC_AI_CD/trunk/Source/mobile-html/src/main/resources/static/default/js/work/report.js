/**
 * Created by gjj on 2017/4/27.
 */
define(function(require, exports, module) {

    var interId;
    var username;

    $(function () {
        if($("#code").val() !="123"){
            WX_auth();
            interId = window.setInterval(intervalUsername,1000);
        } else {
            AJAX_USER_LOGIN("admin","admin");
            interId = window.setInterval(intervalUsername,1000);
        };

    });

    function intervalUsername(){

        if(null !=JSON.parse(sessionStorage.getItem("USER")).username){
            username =JSON.parse(sessionStorage.getItem("USER")).username;
            window.clearInterval(interId);
            loadEquipmentData();
        }
    };

    function loadEquipmentData(){
        AJAX_GET_URL(CONSTANTS.CONF.RESOURCE_BASIC_DATA + "/equipments/getTreeLineStationEquipment", {projectId:CONSTANTS.BASE.PROJECT_ID}, function (data) {
            show_equipment_picker(data); //显示插件
        });
    }

    /**
     * show_equipment_picker 显示插件
     **/
    function show_equipment_picker(equipmentData) {
        var equipment_line_station_selectpicker = new mui.PopPicker({
            layer: 3
        });
        equipment_line_station_selectpicker.cancel.addEventListener('tap', function(event) {
            equipment_line_station_selectpicker.hide();
        }, false);
        equipment_line_station_selectpicker.setData(equipmentData);

        equipment_line_station_selectpicker.show(function(items) {
            var item = items;
            if( items[2] && items[2].text ){
                var locationId = items[0].id
                var line = items[0].text;
                var station = items[1].text;
                var equipmentName = items[2].text;
                var equipmentId = items[2].id;
                var targetUrl = "/skip?path=work/equipmentselect&line="+line+"&station="+station+"&equipmentName="+equipmentName+"&equipmentId="+equipmentId+"&locationId="+locationId;
                window.location.href = targetUrl;
            } else {
                mui.alert("您未选择设备");
            }
            wrapper.closeWaiting();
        });

    }

    /**
     * 编译模板，将数据添加至document
     * @param {Array} _record
     * @param {boolean} _append
     */
    function appendToTable(_record, _append) {
        tp.bind({
            template: 'workorder-item-template',
            element: 'workorder-table',
            model: _record,
            append: _append
        });
    }
})