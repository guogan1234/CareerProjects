require(["../config/config"],function(){
      require(["commonService","bootstrap","validateHelp"],function(commonService, bootstrap,validateHelp){
           var options={
               "formSelector":"alarmSourceFormModal",//表单选择器
               "modalSelector":"alarmSourceModal",//模态框选择器
               "action":"",//请求地址
               "showInfoSelector":"alarmSourceTbl",//显示消息选择器
               "searchSelector":"search",//查询选择器
               "tableSelector":"alarmSourceTbl",//表选择器
               "searchUrl":"/alarmSource/list",//查询地址
               "saveUrl":"/alarmSource/saveOrUpdate",//保存地址
               "updateUrl":"/alarmSource/saveOrUpdate",//修改地址
               "detailUrl":"/alarmSource/detail",//详情地址
               "delUrl":"/alarmSource/del",//删除地址
               "saveSelector":"alarmSourceSaveModal",//保存按钮选择器
               "updateSelector":"alarmSourceUpdateModal",//更新按钮选择器
               "labelSelector":"alarmSourceModalLabel"//模态框title选择器
           };
          var validateOptions={
              "formSelector":"alarmSourceFormModal",//表单选择器
              "rules":""
          };
           $(document).ready(function(){
               //初始化参数
               commonService.init(options);
               //初始化模态框
               var grayList=[];
               grayList.push("sourceId");
               grayList.push("sourceName");
               var types=[];
               types.push("AL");
               types.push("AT");
               commonService.initModal(grayList,"",types);
               //初始化校验validate
               validateHelp.validateInit(validateOptions);
               //保存
               $("#alarmSourceSaveModal").click(function () {
                   if(!validateHelp.valid()){
                       return;
                   }
                   //修改地址
                   commonService.save();
               });
               //修改
               $("#alarmSourceUpdateModal").click(function () {
                   commonService.update();
               });
               $("#searchAlarmSource").click(function () {
                   commonService.search();
               });
               //删除
               $("#alarmSourceTbl tbody").on('click',function(event){
                   var ev= event || window.event;
                   var target = ev.target || ev.srcElement;
                   if(target.nodeName.toLowerCase()=="a"){
                       var attrName=$(target).attr("name");
                       if(attrName=="del"){
                           var id=$(target).data("id");
                           var data={"sourceId":id};
                           // commonService.initAction("/domain/del");
                           commonService.del(data,$(target));
                       }
                   }
               });
          });
      });
});