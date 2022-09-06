package servlets;

import com.google.gson.JsonObject;
import model.VisitedLinkModel;
import model.manager.VisitedLinkManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that deletes visited links
 */
public class DeleteVisitedLinksServlet extends HttpServlet {
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

        VisitedLinkModel model = new VisitedLinkManager();
        int success = model.deleteByUser(user);
        JsonObject jsonObject = new JsonObject();
        if(success >= 0){
            jsonObject.addProperty("success", true);
        }else {
            jsonObject.addProperty("success", false);
        }
        String responseJson = jsonObject.toString();
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
