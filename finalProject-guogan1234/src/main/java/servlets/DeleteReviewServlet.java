package servlets;

import com.google.gson.JsonObject;
import model.ReviewModel;
import model.manager.ReviewManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that deletes liked review
 */
public class DeleteReviewServlet extends HttpServlet {
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
        int success = model.deleteById(reviewId);
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
