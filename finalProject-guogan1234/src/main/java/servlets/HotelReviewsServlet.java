package servlets;

import data.Review;
import model.ReviewModel;
import model.manager.ReviewManager;
import worker.ReviewWorker;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

/**
 * A servlet class that fetch hotel reviews
 */
public class HotelReviewsServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String hotelId = req.getParameter("hotelId");
        String start = req.getParameter("start");
        String size = req.getParameter("size");

        ReviewModel model = new ReviewManager();
        List<Review> reviews = model.findByIdOrderByDateDesc(hotelId, start, size);
        String responseJson = ReviewWorker.getReviewsJsonString(reviews);
        if(responseJson == null) return;
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
