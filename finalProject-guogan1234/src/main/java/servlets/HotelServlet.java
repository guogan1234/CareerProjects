package servlets;

import worker.HotelWorker;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet handles hotel request
 */
public class HotelServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String city = req.getParameter("city");
        String keyword = req.getParameter("keyword");
        String start = req.getParameter("start");
        String size = req.getParameter("size");

        HotelWorker worker = new HotelWorker();
        String writeData = worker.handleRequest(city, keyword, start, size);
        PrintWriter writer = resp.getWriter();
        writer.write(writeData);
        writer.flush();
    }
}
