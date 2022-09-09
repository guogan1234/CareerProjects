/**
 * Created by fangxinyuan on 2015/12/1.
 *字符问题
 */
var stringUtils=(function(){
    //小数后0位到2位
    var regDecimal=/^\d+[.]\d{1,2}$/;
    
    //整数
    var regNum=/^[1-9]+[0-9]*$/ ;

    //字符串非空
    var isNotBlank=function(str){
        if(str!=null && str!=undefined && str!="" && str!='' && str!="undefined"){
            return true;
        }
        return false;
    };
    //字符串为空
    var isBlank=function(str){
        if(!isNotBlank(str)){
            return true;
        }
        return false;
    };
    //是否为数字
    var isNum=function(str){
    	 if(isBlank(str)){
             return false;
         }
         if(str.match(regNum)){
             return true;
         }
        return false;
    };
    //是否为小数，且保留1到2位小数
    var isDecimal=function(str){
        if(isBlank(str)){
            return false;
        }
        if(isNum(str)){
            return true;
        }
        if(str.match(regDecimal)){
            return true;
        }
        return false;
    }
    //数字比较大小 前面>后面
    var compareNum=function(str1,str2){
        if(isBlank(str1) || isBlank(str2)){
            return false;
        }
        if(!isNum(str1) || !isNum(str2)){
            return false;
        }
        var num1=parseInt(str1);
        var num2=parseInt(str2);
        if(num1>num2){
            return true;
        }
        return false;
    };
    //小数比较大小，前面>后面
    var compareDecimal=function(str1,str2){
        if(isBlank(str1) || isBlank(str2)){
            return false;
        }
        if(!isDecimal(str1) || !isDecimal(str2)){
            return false;
        }
        var decimal1=parseFloat(str1);
        var decimal2=parseFloat(str2);
        if(decimal1>decimal2){
            return true;
        }
        return false;
    };
    //删除字符 i代表第几位
    var deleteCharacter=function(str,i){
        return str.replace(str.substring(i,i+1),"");
    };

    return{
        isNotBlank:isNotBlank,
        isBlank:isBlank,
        isNum:isNum,
        isDecimal:isDecimal,
        compareNum:compareNum,
        compareDecimal:compareDecimal,
        deleteCharacter:deleteCharacter
    };
})();
