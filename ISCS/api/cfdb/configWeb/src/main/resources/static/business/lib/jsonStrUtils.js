/**
 * Created by fangxinyuan on 2015/12/2.
 * json字符串特殊字符处理
 * 如' , " < > 空格 enter
 */
var jsonStrUtils=(function(){
    var spaceReg=/^\s$/g;
    var lineBreakReg=/^[\r]$/g;
    var symbolReg=/,\b/gi;
    var singleQuoteReg=/(\b'\b)/g;
    var doubleQuoteReg=/(\b"\b)/g;
    //过滤空格
    var filterSpace=function(str){
        if(stringUtils.isBlank(str)){
            return "";
        }
        return commonDeleteList(str,spaceReg);
    };
    //过滤换行符
    var filterLineBreak=function(str){
        if(stringUtils.isBlank(str)){
            return "";
        }
        return commonDeleteList(str,lineBreakReg);
    };
    //过滤逗号
    var filterSymbol=function(str){
        if(stringUtils.isBlank(str)){
            return "";
        }
       return str.replace(symbolReg,"\\,");
    };
    //过滤单引号(适用单引号在字符中间)
    var filterSingleQuote=function(str){
        if(stringUtils.isBlank(str)){
            return "";
        }
       return str.replace(singleQuoteReg,"\\'");
    };
    //过滤双引号(适用双引号在字符中间)
    var filterDoubleQuote=function(str){
        if(stringUtils.isBlank(str)){
            return "";
        }
      return str.replace(doubleQuoteReg,'\\"');
    };

    //json字符串转object对象
    var jsonStrToObj=function(str){
        if(stringUtils.isBlank(str)){
            return "";
        }
        var index=str.indexOf("{");
        var c=str.substring(index+1,index+2);
        if(c=="'"){//如果是单引号
            str=filterSingleQuote(str);
        }else if(c=='"'){//如果是双引号
            str=filterDoubleQuote(str);
        }

        //过滤空格
        //str=filterSpace(str);
        //过滤换行符
        str=filterLineBreak(str);
        //过滤逗号
        str=filterSymbol(str);

        str=eval("("+str+")");

        if(str!=null){
            return str;
        }
    };


    //公用过滤方法1
    var commonDeleteList=function(str,reg){
        for(var i=0;i<str.length;i++){
            var s=str[i];
            if(s.match(reg)){
                str=stringUtils.deleteCharacter(str,i);
            }
        }
        return str;
    }
    return{
        filterSpace:filterSpace,
        filterLineBreak:filterLineBreak,
        filterSymbol:filterSymbol,
        filterSingleQuote:filterSingleQuote,
        filterDoubleQuote:filterDoubleQuote,
        jsonStrToObj:jsonStrToObj
    };

})();

