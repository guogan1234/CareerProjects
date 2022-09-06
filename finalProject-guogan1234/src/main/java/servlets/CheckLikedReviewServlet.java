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
 * A servlet class that check liked review
 */
public class CheckLikedReviewServlet extends HttpServlet {
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
        String reviewId = req.getParameter("reviewId");

        LikeReviewModel model = new LikeReviewManager();
        int count = model.findByIdAndUser(reviewId, user);
        JsonObject jsonObject = new JsonObject();
        if(count >= 1){
            jsonObject.addProperty("liked", true);
        }else {
            jsonObject.addProperty("liked", false);
        }
        String responseJson = jsonObject.toString();
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
