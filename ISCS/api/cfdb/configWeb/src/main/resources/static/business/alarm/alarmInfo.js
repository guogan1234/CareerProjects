require(["../config/config"],function(){
      require(["commonService","bootstrap","validateHelp"],function(commonService, bootstrap,validateHelp){
           var options={
               "formSelector":"alarmInfoFormModal",//表单选择器
               "modalSelector":"alarmInfoModal",//模态框选择器
               "action":"",//请求地址
               "showInfoSelector":"alarmInfoTbl",//显示消息选择器
               "searchSelector":"search",//查询选择器
               "tableSelector":"alarmInfoTbl",//表选择器
               "searchUrl":"/alarmInfo/list",//查询地址
               "saveUrl":"/alarmInfo/saveOrUpdate",//保存地址
               "updateUrl":"/alarmInfo/saveOrUpdate",//修改地址
               "detailUrl":"/alarmInfo/detail",//详情地址
               "delUrl":"/alarmInfo/del",//删除地址
               "saveSelector":"alarmInfoSaveModal",//保存按钮选择器
               "updateSelector":"alarmInfoUpdateModal",//更新按钮选择器
               "labelSelector":"alarmInfoModalLabel"//模态框title选择器
           };
          var validateOptions={
              "formSelector":"alarmInfoFormModal",//表单选择器
              "rules":""
          };

           $(document).ready(function(){
               //初始化参数
               commonService.init(options);
               //初始化模态框
               var grayList=[];
               grayList.push("label");
               grayList.push("second");
               grayList.push("msecond");
               var types=[];
               types.push("ST");
               types.push("DM");
               types.push("RG");
               types.push("AT");
               types.push("AL");
               commonService.initModal(grayList,"",types);
               //初始化校验validate
               validateHelp.validateInit(validateOptions);
               //保存
               $("#alarmInfoSaveModal").click(function () {
                   if(!validateHelp.valid()){
                       return;
                   }
                   //修改地址
                   commonService.save();
               });
               //修改
               $("#alarmInfoUpdateModal").click(function () {
                   commonService.update();
               });
               $("#searchAlarmInfo").click(function () {
                   commonService.search();
               });
               //删除
               $("#alarmInfoTbl tbody").on('click',function(event){
                   var ev= event || window.event;
                   var target = ev.target || ev.srcElement;
                   if(target.nodeName.toLowerCase()=="a"){
                       var attrName=$(target).attr("name");
                       if(attrName=="del"){
                           var id=$(target).data("id");
                           var ids = id.split(",");
                           var data = {};
                           data.label = ids[0];
                           data.second = ids[1];
                           data.msecond = ids[2];
                           // commonService.initAction("/domain/del");
                           commonService.del(data,$(target));
                       }
                   }
               });
          });
      });
});