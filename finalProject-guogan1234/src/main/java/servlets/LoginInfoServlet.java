package servlets;

import com.google.gson.Gson;
import data.LoginInfo;
import model.LoginInfoModel;
import model.manager.LoginInfoManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that handle login info
 */
public class LoginInfoServlet extends HttpServlet {
    /**
     * Handle get request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String user = req.getParameter("user");

        LoginInfoModel model = new LoginInfoManager();
        LoginInfo info = model.findByUserOrderByLoginTime(user);
        String responseJson = getLoginInfoJson(info);
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }

    /**
     * LoginInfo converts to String
     * @param info login info
     * @return a json string
     */
    private String getLoginInfoJson(LoginInfo info){
        Gson gson = new Gson();
        return gson.toJson(info);
    }
}
