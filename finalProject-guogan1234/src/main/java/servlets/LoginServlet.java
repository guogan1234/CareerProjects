package servlets;

import com.google.gson.JsonObject;
import htmlsMapping.HtmlMapper;
import worker.UserWorker;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Date;

/**
 * A servlet handles login request
 */
public class LoginServlet extends HttpServlet {
    /**
     * Handle get request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        boolean loginStatus = isLogin(req);
        if(loginStatus) return;
        HtmlMapper.showHtml(req, resp, "login", "Sign in");
    }

    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String name = req.getParameter("name");
        String password = req.getParameter("password");
        HttpSession session = req.getSession();
        boolean hasLogin = checkHasLogin(session, name);
        if(hasLogin){
            String responseJson = createHasLoginResponseJson(name);
            writeResponseJson(resp, responseJson);
            return;
        }
        userLogin(req, resp);
    }

    /**
     * Handle user login
     * @param req Http Servlet Request
     * @param resp Http Servlet Response
     * @throws ServletException Servlet Exception
     * @throws IOException IO Exception
     */
    private void userLogin(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException{
        String name = req.getParameter("name");
        String password = req.getParameter("password");
        UserWorker userWorker = new UserWorker();
        boolean login = userWorker.validateLogin(name, password);
        String responseData = null;
        if(login){// success login
            HttpSession session = req.getSession();
            session.setAttribute("status", "login");
            session.setAttribute("user", name);
            Date now = new Date();
            session.setAttribute("loginTime", now);
//            HtmlMapper.showHtml(req, resp, "index", name);
            responseData = createResponseJson(name, true);
        }else {// login failed
//            HtmlMapper.showHtml(req, resp, "login", "Sign in");
            responseData = createResponseJson(name, false);
        }
        writeResponseJson(resp, responseData);
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

    /**
     * Check user has logined
     * @param session Http Session
     * @param username username
     * @return user login status
     */
    private boolean checkHasLogin(HttpSession session, String username){
        if(username == null) return false;
        Object object = session.getAttribute("user");
        if(object != null){
            String name = (String)object;
            if(username.equals(name)) return true;
        }
        return false;
    }

    /**
     * Create response json string when user has logined
     * @param name username
     * @return a json string
     */
    private String createHasLoginResponseJson(String name){
        JsonObject jsonObject = new JsonObject();
        jsonObject.addProperty("name", name);
        jsonObject.addProperty("success", true);
        jsonObject.addProperty("message", "You already have logined!");
        return jsonObject.toString();
    }

    /**
     * Create response json string
     * @param name username
     * @param login user login status
     * @return a json string
     */
    private String createResponseJson(String name, boolean login){
        JsonObject jsonObject = new JsonObject();
        if(login){
            jsonObject.addProperty("name", name);
            jsonObject.addProperty("success", true);
            jsonObject.addProperty("message", "Success Login!");
        }else {
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("message","Username or password incorrect!");
        }
        return jsonObject.toString();
    }

    /**
     * Get user login status
     * @param req Http Servlet Request
     * @return user login status
     */
    private boolean isLogin(HttpServletRequest req){
        HttpSession session = req.getSession();
        Object object = session.getAttribute("status");
        if(object == null) return false;
        return true;
    }
}
