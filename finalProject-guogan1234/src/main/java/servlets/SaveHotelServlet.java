package servlets;

import com.google.gson.JsonObject;
import data.SavedHotel;
import model.SaveHotelModel;
import model.manager.SaveHotelManager;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that save hotel
 */
public class SaveHotelServlet extends HttpServlet {
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String hotelId = req.getParameter("hotelId");
        String hotelName = req.getParameter("hotelName");
        String user = req.getParameter("user");
        String dateString = req.getParameter("dateString");

        SavedHotel savedHotel = new SavedHotel(hotelId, hotelName, user, dateString);
        SaveHotelModel model = new SaveHotelManager();
        int success = model.add(savedHotel);
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
