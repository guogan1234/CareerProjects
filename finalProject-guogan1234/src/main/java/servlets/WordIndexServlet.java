package servlets;

import com.google.gson.Gson;
import data.WordIndex;
import model.WordIndexModel;
import model.manager.WordIndexManager;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.List;

/**
 * A servlet handles word index request
 */
public class WordIndexServlet extends HttpServlet {
    /**
     * Handle post request
     * @param req http request
     * @param resp http response
     * @throws ServletException servlet exception
     * @throws IOException io exception
     */
    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String keyword = req.getParameter("keyword");
        String start = req.getParameter("start");
        String size = req.getParameter("size");

        WordIndexModel model = new WordIndexManager();
        List<WordIndex> wordIndices = model.findReviewsByWord(keyword, start, size);
        String responseJson = createResponseJson(wordIndices);
        PrintWriter writer = resp.getWriter();
        writer.write(responseJson);
        writer.flush();
    }

    /**
     * WordIndex list converts to String
     * @param wordIndices WordIndex list
     * @return a json string
     */
    private String createResponseJson(List<WordIndex> wordIndices){
        Gson gson = new Gson();
        return gson.toJson(wordIndices);
    }
}
