package servlets;

import com.google.gson.JsonObject;
import data.LikedReview;
import model.LikeReviewModel;
import model.manager.LikeReviewManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that adds liked review
 */
public class AddLikedReviewServlet extends HttpServlet {
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
        String reviewTitle = req.getParameter("reviewTitle");
        String hotelId = req.getParameter("hotelId");
        String hotelName = req.getParameter("hotelName");
        String user = req.getParameter("user");
        String dateString = req.getParameter("dateString");

        LikedReview likedReview = new LikedReview(reviewId, reviewTitle, hotelId, hotelName, user, dateString);
        LikeReviewModel model = new LikeReviewManager();
        int success = model.add(likedReview);
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
