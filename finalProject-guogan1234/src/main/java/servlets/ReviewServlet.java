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

/**
 * A servlet class that fetch review
 */
public class ReviewServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String reviewId = req.getParameter("reviewId");
        ReviewModel model = new ReviewManager();
        Review review = model.findById(reviewId);
        String responseJson = ReviewWorker.getReviewJsonString(review);
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
