package servlets;

import com.google.gson.JsonObject;
import model.LikeReviewModel;
import model.manager.LikeReviewManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that counts liked review
 */
public class CountLikedReviewServlet extends HttpServlet {
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

        LikeReviewModel model = new LikeReviewManager();
        int count = model.findById(reviewId);
        JsonObject jsonObject = new JsonObject();
        jsonObject.addProperty("count", count);
        String responseJson = jsonObject.toString();
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
