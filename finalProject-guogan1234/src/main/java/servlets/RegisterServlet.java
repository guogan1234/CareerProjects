package servlets;

import com.google.gson.Gson;
import com.google.gson.JsonObject;
import worker.UserWorker;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet handles register request
 */
public class RegisterServlet extends HttpServlet {
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
        UserWorker userWorker = new UserWorker();
        boolean register = userWorker.registerUser(name, password);
        PrintWriter writer = resp.getWriter();
        writer.write(createResponseJson(name, register));
        writer.flush();
    }

    /**
     * Create response json string
     * @param name username
     * @param register register status
     * @return a json string
     */
    private String createResponseJson(String name, boolean register){
        JsonObject jsonObject = new JsonObject();
        if(register){
            jsonObject.addProperty("name", name);
            jsonObject.addProperty("success", true);
            jsonObject.addProperty("message", "Success Register!");
        }else {
            jsonObject.addProperty("success", false);
            jsonObject.addProperty("message","Username or password incorrect!");
        }
        return jsonObject.toString();
    }
}
