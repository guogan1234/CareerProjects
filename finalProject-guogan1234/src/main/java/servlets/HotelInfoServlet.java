package servlets;

import data.Hotel;
import model.HotelModel;
import model.manager.HotelDataManager;
import worker.HotelWorker;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

/**
 * A servlet class that fetch hotel info
 */
public class HotelInfoServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String hotelName = req.getParameter("hotelName");
        System.out.println(hotelName);

        HotelModel model = new HotelDataManager();
        Hotel hotel = model.findByName(hotelName);
        String responseJson = HotelWorker.getHotelJsonString(hotel);
        if(responseJson == null) return;
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }
}
