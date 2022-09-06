package servlets;

import com.google.gson.JsonObject;
import data.Review;
import model.ReviewModel;
import model.manager.ReviewManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that adds review
 */
public class AddReviewServlet extends HttpServlet {
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
        String reviewTitle = req.getParameter("reviewTitle");
        String reviewText = req.getParameter("reviewText");
        String reviewRate = req.getParameter("reviewRate");
        String user = req.getParameter("user");
        String dateString = req.getParameter("dateString");
        String reviewId = req.getParameter("reviewId");

        Review review = new Review();
        review.setHotelId(hotelId);
        review.setTitle(reviewTitle);
        review.setReviewText(reviewText);
        review.setAverageRating(Integer.parseInt(reviewRate));
        review.setUserNickname(user);
        review.setDate(dateString);
        review.setReviewId(reviewId);

        ReviewModel model = new ReviewManager();
        int success = model.add(review);
        JsonObject jsonObject = new JsonObject();
        if(success == 1){// update success
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
