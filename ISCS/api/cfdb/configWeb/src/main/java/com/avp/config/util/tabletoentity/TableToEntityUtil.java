package com.avp.config.util.tabletoentity;

import java.util.ArrayList;
import java.util.List;


/**
 * 
* @ClassName: TableToEntity
* @Description: 数据库表转实体类
* @author Cookie
* @date 2018年3月27日 上午8:50:45
* @company 上海艾文普信息技术有限公司
*
 */
public class TableToEntityUtil {
	
	/**
	 *1.字段之间属性一个空格隔开 不同字段逗号隔开 
	 *2.int:1,String:2,boolean:3,long:4
	 *3.非String型可不用写长度
	 */
	public static void main(String[] args) throws Exception {
		String str = "ai_label 标签 2 128,"
				+ "ai_name 名称 2 128,"
				+ "point_type 点类型 1,"
				+ "point_tp_label 点模板标签 2 32,"
				+ "dev_label 所属设备 2 96,"
				+ "dev_type 设备类型 2 16,"
				+ "ai_order 顺序号 1,"
				+ "domain_id 所属域 1,"
				+ "location_id 所属位置 1,"
				+ "pro_system_id 所属子系统 1,"
				+ "region_id 所属责任区 1,"
				+ "ai_value 值 float,"
				+ "ai_status 状态 1,"
				+ "";
				
		
		TableToEntityUtil tableToEntity = new TableToEntityUtil();
		List<TablePo> tPos = new ArrayList<>();
		tPos = tableToEntity.parseString(str);
		tableToEntity.tableToEntity("di_tbl","离散输入点表输出点表", tPos);
	}
	
	
	
	
	
	
	//------------------------- Cookie 自定义方法开始 ----------------------------
	
	/**
	 * 
	 * @Title: tableToEntity
	 * @Description: 自定义转换方法
	 * @author Cookie
	 * @date 2018年3月27日 上午9:07:17
	 * @company 上海艾文普信息技术有限公司
	 */
	public void tableToEntity(String tableName,String explain,List<TablePo> param) {
		System.out.println("import javax.persistence.Column;\r\n" + 
				"import javax.persistence.Entity;\r\n" + 
				"import javax.persistence.Id;\r\n" + 
				"import javax.persistence.Table;\r\n");
		System.out.println("/**\r\n * "+explain+"\r\n *\r\n */");
		System.out.println("@Entity\r\n@Table(name = \""+tableName+"\",schema = \"conf\")");
		System.out.println("public class "+clumFormat(tableName, 1)+" {");
		for(int i = 0;i<param.size();i++) {
			TablePo po = param.get(i);
			String str1 = "//"+po.getcName()+"\r\n";
			if(i==0) { str1 = str1 + "@Id\r\n";}
			String str2 = null;
			//判断字段是否是String类型
			if(po.getType().equals("String")||po.getType().equals("string")) {
				po.setType("String");
				str2 = "@Column(name = "+"\""+po.geteName()+"\""+",length = "+po.getLength()+")\r\n";
			}else {
				str2 = "@Column(name = "+"\""+po.geteName()+"\""+")\r\n";
			}
			String str3 = "private " + po.getType() + " " + clumFormat(po.geteName()) + ";\r\n";
			System.out.println(str1+str2+str3);
		}
		System.out.println("}");
	}
	
	/**
	 * 
	 * @Title: clumFormat
	 * @Description: 将数据库下划线字段转换为Java格式字段(首字母第一个首字母不大写)
	 * @author Cookie
	 * @date 2018年3月27日 上午9:25:28
	 * @company 上海艾文普信息技术有限公司
	 * @param param
	 * @return
	 */
	public String clumFormat(String param) {
		String[] strs = param.split("_");
		String result = null;
		for(int i = 0;i<strs.length;i++) {
			if(i==0) { result = strs[i]; continue;}
			//首字母大写
			char fir = strs[i].charAt(0);//获取字符串的第一个字符
			String tempStr = strs[i].substring(1);
			tempStr = String.valueOf(fir).toUpperCase()+strs[i].substring(1);
			result = result +tempStr;
		}
		return result;
	}

	/**
	 * 
	 * @Title: clumFormat
	 * @Description: 将数据库下划线字段转换为Java格式字段(所有首字母大写)
	 * @author Cookie
	 * @date 2018年3月27日 上午9:25:28
	 * @company 上海艾文普信息技术有限公司
	 * @param param
	 * @return
	 */
	public String clumFormat(String param,int max) {
		String[] strs = param.split("_");
		String result = "";
		for(int i = 0;i<strs.length;i++) {
			//首字母大写
			char fir = strs[i].charAt(0);//获取字符串的第一个字符
			String tempStr = strs[i].substring(1);
			tempStr = String.valueOf(fir).toUpperCase()+strs[i].substring(1);
			result = result +tempStr;
		}
		return result;
	}
	
	/**
	 * 
	 * @Title: parseString
	 * @Description: 将固定格式字符串解析成对象集合
	 * @author Cookie
	 * @date 2018年3月27日 上午10:14:09
	 * @company 上海艾文普信息技术有限公司
	 * @param param
	 * @return
	 * @throws Exception 
	 */
	public List<TablePo> parseString(String param) throws Exception{
		List<TablePo> tPos = new ArrayList<>();
		String[] strs = param.split(",");
		for(int i = 0;i<strs.length;i++) {
			String[] strs2 = strs[i].split(" ");
			TablePo tPo = null;
			String sType = "";
			switch (strs2[2].trim()) {
			case "1":
				sType = "int";
				break;
			case "2":
				sType = "String";
				break;
			case "3":
				sType = "boolean";
				break;
			case "4":
				sType = "long";
				break;
			default:
				sType = strs2[2].trim();
				break;
			}
			try {
				tPo = new TablePo(strs2[0].trim(), strs2[1].trim(), sType, strs2.length>3?strs2[3].trim():"0");
			} catch (Exception e) {
				System.out.println("==========>错误序号为:"+i+"===========>"+strs[i]);
				throw new Exception("字符串转对象错误");
			}
			tPos.add(tPo);
		}
		return tPos;
	}
}
