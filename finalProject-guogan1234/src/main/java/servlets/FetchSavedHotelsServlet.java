package servlets;

import com.google.gson.Gson;
import data.SavedHotel;
import model.SaveHotelModel;
import model.manager.SaveHotelManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

/**
 * A servlet class that fetch saved hotels
 */
public class FetchSavedHotelsServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String username = req.getParameter("user");

        SaveHotelModel model = new SaveHotelManager();
        List<SavedHotel> savedHotels = model.findByUser(username);
        String responseJson = getSavedHotelsJsonString(savedHotels);
        writeResponseJson(resp, responseJson);
    }

    /**
     * SavedHotel list converts to String
     * @param savedHotels SavedHotel list
     * @return a json string
     */
    private String getSavedHotelsJsonString(List<SavedHotel> savedHotels){
        if(savedHotels == null) return null;
        Gson gson = new Gson();
        return gson.toJson(savedHotels);
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
