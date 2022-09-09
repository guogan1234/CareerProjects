define(['jquery'],function($){
     var showPopUp=function (selectorId,type,operate,msg) {
         if (type=="success"){
            $("#"+selectorId).before(success(operate,msg));
         }else if (type=="fail"){
             $("#"+selectorId).before(fail(operate,msg));
         }
     };

     var success=function (operate,msg) {
         var h=[];
         h.push('<div class="alert alert-success fade in">');
         h.push('<button class="close" data-dismiss="alert">');
         h.push('x </button>');
         h.push('<i class="fa-fw fa fa-check"></i>');
         h.push('<strong>'+operate+'成功</strong>'+msg+'');
         h.push('</div>');
         return h.join("");
     };
     
     var fail=function (operate,msg) {
         var h=[];
         h.push('<div class="alert alert-danger fade in">');
         h.push('<button class="close" data-dismiss="alert">');
         h.push('x </button>');
         h.push('<i class="fa-fw fa fa-check"></i>');
         h.push('<strong>'+operate+'出错</strong>:'+msg+'');
         h.push('</div>');
         return h.join("");
     };
     
     var close=function () {
       $(".alert").each(function () {
          $(this).remove();
       });
     };
     
     return {
         "showPopUp":showPopUp,
         "close":close
     }
});