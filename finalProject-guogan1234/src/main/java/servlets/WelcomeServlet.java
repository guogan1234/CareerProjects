package servlets;

import htmlsMapping.HtmlMapper;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

/**
 * A servlet handles welcome request
 */
public class WelcomeServlet extends HttpServlet {
    /**
     * Handle get request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        HttpSession session = req.getSession();
        String name = (String) session.getAttribute("user");
        if(name == null){
            HtmlMapper.showHtml(req, resp, "index", "");
        }else {
            HtmlMapper.showHtml(req, resp, "index", name);
        }
    }
}
