require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"doGrpFormModal",//表单选择器
            "modalSelector":"doGrpModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"doGrpTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"doGrpTbl",//表选择器
            "searchUrl":"/doGrp/list",//查询地址
            "saveUrl":"/doGrp/saveOrUpdate",//保存地址
            "updateUrl":"/doGrp/saveOrUpdate",//修改地址
            "detailUrl":"/doGrp/detail",//详情地址
            "delUrl":"/doGrp/del",//删除地址
            "saveSelector":"doGrpSaveModal",//保存按钮选择器
            "updateSelector":"doGrpUpdateModal",//更新按钮选择器
            "labelSelector":"doGrpModalLabel",//模态框title选择器
            "autoAssignWhenAdd":true
        };

        var validateOptions={
            "formSelector":"doGrpFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("doGrpLabel");
            grayList.push("doOrder");
            //加载需要的下拉框
            var types=[];
            types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#doGrpSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#doGrpUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchDoGrp").click(function () {
                commonService.search();
            });
            //删除
            $("#doGrpTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var ids=id.split(",");
                        var data={"doGrpLabel":ids[0],"doOrder":ids[1]};
                        commonService.del(data,$(target));
                    }
                }
            });
            $("#configSaveOrUpdateModal").click(function(){
                var  doGrpLabel=$("#hid_doGrpLabel").val();
                var doOrder=$("#hid_doOrder").val();
                var h=[];
                $("#configFormModal").find("input:checkbox").each(function () {
                    var v=$(this).prop("checked");
                    if (v){
                        h.push(1);
                    }else{
                        h.push(0);
                    }
                });
                h.reverse();
                var data={"doGrpLabel":doGrpLabel,"doOrder":doOrder,"allows":h.join("")};
                commonService.saveB("configModal","/doGrp/saveDoGrpConfig",data);
            });
            $("#configModal").on('show.bs.modal',function (event) {
                $.fn.modal.Constructor.prototype.enforceFocus = function () { };
                var b = $(event.relatedTarget)
                var modal = $(this);
                var operate=b.data("operate");
                var types=[];
               // modalHelp.modalInit("",types,$("#configFormModal"));
                if (operate=="config"){
                    var doNumber=b.data("donumber");//分量值
                    var doValue=b.data("dovalue");//目标值
                    var doAllow=b.data("doallow");//允许值
                    doAllow=Number(doAllow).toString(2);
                    var id=b.data("id");
                    var ids=id.split(",");
                    $("#hid_doGrpLabel").val(ids[0]);
                    $("#hid_doOrder").val(ids[1]);
                    doNumber=Math.pow(2,Number(doNumber));
                    if (doAllow.length<Number(doNumber)){
                        var d="";
                        for (var i=0;i<Number(doNumber)-doAllow.length;i++){
                            d=d+"0";
                        }
                        doAllow=d+doAllow;
                    }
                    //console.log(doAllow+":"+doNumber);
                    var html=[];
                    for(var i=0;i<Number(doNumber);i++){
                        html.push('<tr>');
                        html.push('<td>'+i+'</td>');
                        if(doAllow.charAt(Number(doNumber)-i-1)=="1"){
                            html.push('<td><input type="checkbox" checked="checked" /></td>');
                        }else{
                            html.push('<td><input type="checkbox" /></td>');
                        }
                        html.push('<td>'+doValue+'</td>')
                        html.push('</tr>');
                    }
                    $("#configTbl tbody").html(html.join(""));
                }
            });
        });
    });
});