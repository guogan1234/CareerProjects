package servlets;

import com.google.gson.Gson;
import data.VisitedLink;
import model.VisitedLinkModel;
import model.manager.VisitedLinkManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

/**
 * A servlet class that fetch visited links
 */
public class FetchVisitedLinksServlet extends HttpServlet {
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String user = req.getParameter("user");

        VisitedLinkModel model = new VisitedLinkManager();
        List<VisitedLink> visitedLinks = model.findByUser(user);
        String responseJson = getVisitedLinksJsonString(visitedLinks);
        writeResponseJson(resp, responseJson);
    }

    /**
     * VisitedLink list converts to String
     * @param visitedLinks VisitedLink list
     * @return a json string
     */
    private String getVisitedLinksJsonString(List<VisitedLink> visitedLinks){
        if(visitedLinks == null) return null;
        Gson gson = new Gson();
        return gson.toJson(visitedLinks);
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
