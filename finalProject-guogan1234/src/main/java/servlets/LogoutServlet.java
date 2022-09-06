package servlets;

import com.google.gson.JsonObject;
import data.LoginInfo;
import model.LoginInfoModel;
import model.manager.LoginInfoManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * A servlet class that handles logout request
 */
public class LogoutServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String user = req.getParameter("user");
        String loginTime = req.getParameter("loginTime");
        String logoutTime = req.getParameter("logoutTime");

        HttpSession session = req.getSession();
        String loginTimeString = getLoginTime(session);

        LoginInfo loginInfo = new LoginInfo();
        loginInfo.setUsername(user);
        loginInfo.setLoginTime(loginTimeString);
        loginInfo.setLogoutTime(logoutTime);

        String responseJson = getLogoutResponseJson(loginInfo, session);
        writeResponseJson(resp, responseJson);
    }

    /**
     * Get user login time
     * @param session Http Session
     * @return user login time string
     */
    private String getLoginTime(HttpSession session){
        String loginTimeString = null;
        Object object = session.getAttribute("loginTime");
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        if(object != null){
            Date date = (Date)object;
            loginTimeString = dateFormat.format(date);
            System.out.println("[loginTime]:" + loginTimeString);
        }
        return loginTimeString;
    }

    /**
     * Get user logout response json string
     * @param loginInfo user login Info
     * @param session Http Session
     * @return a json string
     */
    private String getLogoutResponseJson(LoginInfo loginInfo, HttpSession session){
        LoginInfoModel model = new LoginInfoManager();
        int success = model.add(loginInfo);
        JsonObject jsonObject = new JsonObject();
        if(success == 1){// update success
            jsonObject.addProperty("success", true);
            session.setAttribute("user", null);
        }else {
            jsonObject.addProperty("success", false);
        }
        return jsonObject.toString();
    }

    /**
     * Write response json string to client
     * @param resp Http Servlet Response
     * @param responseJson response json string
     * @throws ServletException Servlet Exception
     * @throws IOException IO Exception
     */
    private void writeResponseJson(HttpServletResponse resp, String responseJson) throws ServletException, IOException{
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
