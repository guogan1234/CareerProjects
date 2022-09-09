/**
 * Created by pw on 2017/4/25.
 */
var userId="";
var userName="";
$(function () {
    if($("#code").val() !="123"){
        WX_auth();
    }
    if(null !=userName&&userName.trim().length>0){
        AJAX_USER_LOGIN(userName,CONSTANTS.BASE.USERPW);
    }
    // is_token();
    activeTable('wrench');
});


