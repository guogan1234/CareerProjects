package server;

import org.apache.velocity.app.VelocityEngine;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.servlet.DefaultServlet;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import servlets.*;

import java.nio.file.Paths;

/**
 * A class that provides web services
 */
public class WebServer {
    private final int PORT = 8088;

    /**
     * Start to provide web services
     */
    public void startServer(){
        Server server = new Server(PORT);
        ServletContextHandler context = new ServletContextHandler(ServletContextHandler.SESSIONS);

        String resourceDir = Paths.get(".", "/web").toString();
        context.setResourceBase(resourceDir);
        // add servlets
        context.addServlet(HotelServlet.class, "/hotel");
        context.addServlet(LoginServlet.class, "/login");
//        context.addServlet(LoginServlet.class, "/");//Multiple servlets map
        context.addServlet(RegisterServlet.class, "/register");
        context.addServlet(WelcomeServlet.class, "/welcome");
        context.addServlet(WordIndexServlet.class, "/wordIndex");
        context.addServlet(HotelInfoServlet.class, "/hotelInfo");
        context.addServlet(HotelReviewsServlet.class, "/hotelReviews");
        context.addServlet(ReviewServlet.class, "/review");
        context.addServlet(UpdateReviewServlet.class, "/updateReview");
        context.addServlet(AddReviewServlet.class, "/addReview");
        context.addServlet(DeleteReviewServlet.class, "/deleteReview");
        context.addServlet(SaveHotelServlet.class, "/saveHotel");
        context.addServlet(FetchSavedHotelsServlet.class, "/fetchSavedHotels");
        context.addServlet(DeleteSavedHotelsServlet.class, "/clearSavedHotels");
        context.addServlet(CheckLikedReviewServlet.class, "/checkLikedReview");
        context.addServlet(CountLikedReviewServlet.class, "/countLikedReview");
        context.addServlet(AddLikedReviewServlet.class, "/addLikedReview");
        context.addServlet(LoginInfoServlet.class, "/loginInfo");
        context.addServlet(LogoutServlet.class, "/logout");
        context.addServlet(SaveVisitedLinkServlet.class, "/saveVisitedLink");
        context.addServlet(FetchVisitedLinksServlet.class, "/fetchVisitedLinks");
        context.addServlet(DeleteVisitedLinksServlet.class, "/clearVisitedLinks");

        VelocityEngine velocityEngine = new VelocityEngine();
        velocityEngine.init();

        context.setContextPath("/");
        context.setAttribute("templateEngine", velocityEngine);

        ServletHolder holder = new ServletHolder("default", DefaultServlet.class);
        holder.setInitParameter("dirAllowed", "false");
        context.addServlet(holder, "/");

        server.setHandler(context);
        try {
            server.start();
            server.join();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
