require(["../config/config"],function(){
    require(["devSelect"],function (devSelect) {
        //初始化下拉框的值
        devSelect.selectAssignment(null,"ZY",$("#proSystem"));
        devSelect.selectAssignment(null,"ST",null);
    });
    require(["select2"],function (select2) {
        //给下拉框加上搜索功能
        $("select").select2({});
    });
    require(["wizardHelp"],function (wizardHelp){
        //点模板
        wizardHelp.wizardInit("pointDioWizard");
        wizardHelp.wizardInit("pointAioWizard");
        wizardHelp.wizardInit("pointAccWizard");
        //特殊信号
        wizardHelp.wizardInit("signalDioWizard");
        wizardHelp.wizardInit("signalAioWizard");
        wizardHelp.wizardInit("signalAccWizard");
        //继承信号实例
        wizardHelp.wizardInit("signalInheritAccWizard");
        wizardHelp.wizardInit("signalInheritAioWizard");
        wizardHelp.wizardInit("signalInheritDioWizard");
        //批量设备实例
        wizardHelp.wizardInit("batchInstanceWizard");
    });
    //设备模版
    require(["bootstrap","deviceTemplate"],function(bootstrap,deviceTemplate){
        //得到设备详情
        deviceTemplate.operateModal();

        $("#devModal").find("[name='proSystemId']").change(function () {
            deviceTemplate.proSystemChange()
        });
        //设备模版保存
        $("#devTemplateSaveModal").click(function () {
            deviceTemplate.save();
        });
        //设备模版修改
        $("#devTemplateUpdateModal").click(function () {
            deviceTemplate.update();
        });

        $("#devTemplateCopyToSaveModal").click(function () {
            deviceTemplate.copy();
        });

        $("#batchInstanceToSaveModal").click(function () {
            deviceTemplate.batchInstance();
        });

        //删除设备模版
        $("#devTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="a"){
                var attrName=$(target).attr("name");
                if(attrName=="delDev"){
                    deviceTemplate.del($(target));
                }
            }
        });
        //查询设备列表
        $("#searchDev").click(function () {
            deviceTemplate.searchDevice();
        });
    });
    //点模版
    require(["bootstrap","pointTemplate","validate"],function(bootstrap,pointTemplate,validate){
        //得到模版类型
        pointTemplate.operateModal();
        validate.initVal($("#pointAioFormModal"));
        validate.initVal($("#pointAccFormModal"));
        pointTemplate.isControl($("#pointDioModal"),$("#pointDio2"),"pointDioWizard",1);
        pointTemplate.isControl($("#pointAioModal"),$("#pointAio3"),"pointAioWizard",2);
        pointTemplate.isLimit($("#pointAioModal"),$("#pointAio2"),"pointAioWizard",1);
        pointTemplate.isLimit($("#pointAccModal"),$("#pointAcc2"),"pointAccWizard",1);

        pointTemplate.limitNum($("#pointAioModal"));
        pointTemplate.limitNum($("#pointAioModal"));

        $("#pointSelect").change(function () {
            var pointSelect=$("#pointSelect").val();
            if (pointSelect==1){//DIG
                $("#devTbl").find("[name='addPoint']").each(function () {
                    $(this).attr("href","#pointDioModal");
                });
            }else if(pointSelect==2){//ANA
                $("#devTbl").find("[name='addPoint']").each(function () {
                    $(this).attr("href","#pointAioModal");
                });
            }else if(pointSelect==3){//ACC
                $("#devTbl").find("[name='addPoint']").each(function () {
                    $(this).attr("href","#pointAccModal");
                });
            }
        });


        $("#savePointDioModal").click(function () {
            pointTemplate.saveDio();
        });

        $("#savePointAioModal").click(function () {
            if (validate.valid($("#pointAioFormModal"))) {
                if (pointTemplate.pointValidate($("#pointAioFormModal"),'pointAioFormModal',true)){
                    pointTemplate.saveAio();
                }
            }
        });

        $("#savePointAccModal").click(function () {
            if (validate.valid($("#pointAccFormModal"))) {
                if (pointTemplate.pointValidate($("#pointAccFormModal"),'pointAccFormModal',true)){
                    pointTemplate.saveAcc();
                }
            }

        });

        $("#updatePointDioModal").click(function () {
            pointTemplate.updateDio();
        });

        $("#updatePointAioModal").click(function () {
            if (validate.valid($("#pointAioFormModal"))) {
                if (pointTemplate.pointValidate($("#pointAioFormModal"),'pointAioFormModal',true)) {
                    pointTemplate.updateAio();
                }
            }
        });

        $("#updatePointAccModal").click(function () {
            if (validate.valid($("#pointAccFormModal"))) {
                if (pointTemplate.pointValidate($("#pointAccFormModal"),'pointAccFormModal',true)){
                    pointTemplate.updateAcc();
                }
            }
        });

        $("#pointCopyToSaveModal").click(function () {
            pointTemplate.copy();
        });

        $("#devTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="span"){
                var attrName=$(target).attr("name");
                if(attrName=="devplus"){
                    var id=$(target).data("id");
                    pointTemplate.searchPoint(id);
                    $(target).attr("name","devMinus");
                    $(target).removeClass("glyphicon-plus").addClass("glyphicon-minus");
                }else if(attrName=="devMinus"){
                    var id=$(target).data("id");
                    $(target).attr("name","devplus");
                    $(target).removeClass("glyphicon-minus").addClass("glyphicon-plus");
                    $("#devTbl tbody").find("tr[data-subid='point_"+id+"_header']").each(function () {
                        $(this).remove();
                    });
                    $("#devTbl tbody").find("tr[data-subid='point_"+id+"']").each(function () {
                        $(this).remove();
                    });
                }
            }
        });

        $("#devTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="a"){
                var attrName=$(target).attr("name");
                if(attrName=="pointDel"){
                    pointTemplate.del($(target));
                }
            }
        });
    });
    //设备实例
    require(["bootstrap","devInstance"],function(bootstrap,devInstance){
        devInstance.operateModal();

        $("#devInstanceSaveModal").click(function () {
            devInstance.saveInstance();
        });
        $("#devInstanceUpdateModal").click(function () {
            devInstance.updateInstance();
        });

        $("#devCopyToSaveModal").click(function () {
            devInstance.copy();
        });

        $("#stationCopyToSaveModal").click(function () {
            devInstance.copyStation();
        });

        $("#instanceSearch").click(function () {
            var devTpLabel=$("#devId").val();
            var stationSelect=$("#stationSelect").val();
            devInstance.searchInstance(devTpLabel,stationSelect);
        });

        $("#devTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="a"){
                var attrName=$(target).attr("name");
                if (attrName=="searchInstance"){
                    var devId=$(target).data('id');
                    $("#devId").val(devId);
                    var proId=$(target).data('zy');
                    $("#proId").val(proId);
                    $("#addDevInstance").removeClass("hide");
                    devInstance.searchInstance(devId,null);
                }
            }
        });

        $("#devInstanceTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="a"){
                var attrName=$(target).attr("name");
                if(attrName=="delDevInstance"){
                    devInstance.delInstance($(target));
                }
            }
        });
    });
    require(["bootstrap","signalInstance"],function(bootstrap,signalInstance){
        signalInstance.operateModal();

        $("#signalSelect").change(function () {
            var signalSelect=$("#signalSelect").val();
            if (signalSelect==1){//DIG
                $("#devInstanceTbl").find("[name='addSignal']").each(function () {
                    $(this).attr("href","#signalDioModal");
                });
            }else if(signalSelect==2){//ANA
                $("#devInstanceTbl").find("[name='addSignal']").each(function () {
                    $(this).attr("href","#signalAioModal");
                });
            }else if(signalSelect==3){//ACC
                $("#devInstanceTbl").find("[name='addSignal']").each(function () {
                    $(this).attr("href","#signalAccModal");
                });
            }
        });

        $("#devInstanceTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="span"){
                var attrName=$(target).attr("name");
                if(attrName=="plusDevInstance"){
                    var id=$(target).data("id");
                    signalInstance.searchSignal(id);
                    $(target).attr("name","minusDevInstance");
                    $(target).removeClass("glyphicon-plus").addClass("glyphicon-minus");
                }else if(attrName=="minusDevInstance"){
                    var id=$(target).data("id");
                    $(target).attr("name","plusDevInstance");
                    $(target).removeClass("glyphicon-minus").addClass("glyphicon-plus");
                    $("#devInstanceTbl tbody").find("tr[data-subid='signal_"+id+"_header']").each(function () {
                        $(this).remove();
                    });
                    $("#devInstanceTbl tbody").find("tr[data-subid='signal_"+id+"']").each(function () {
                        $(this).remove();
                    });
                }
            }
        });

        $("#saveDioSignal").click(function () {
            signalInstance.saveDioSignal();
        });
        $("#saveAioSignal").click(function () {
            signalInstance.saveAioSignal();
        });

        $("#saveAccSignal").click(function () {
            signalInstance.saveAccSignal();
        });

        $("#updateDioSignal").click(function () {
            signalInstance.updateDioSignal();
        });

        $("#updateAioSignal").click(function () {
            signalInstance.updateAioSignal();
        });

        $("#updateAccSignal").click(function () {
            signalInstance.updateAccSignal();
        });

        $("#updateInheritDioSignal").click(function () {
            signalInstance.updateInheritDioSignal();
        });

        $("#updateInheritAioSignal").click(function () {
            signalInstance.updateInheritAioSignal();
        });

        $("#updateInheritAccSignal").click(function () {
            signalInstance.updateInheritAccSignal();
        });

        $("#devInstanceTbl tbody").on('click',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="a"){
                var attrName=$(target).attr("name");
                if(attrName=="signalDel"){
                    signalInstance.del($(target));
                }
            }
        });
    });
});