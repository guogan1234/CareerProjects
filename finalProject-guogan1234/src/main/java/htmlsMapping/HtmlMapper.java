package htmlsMapping;

import org.apache.velocity.Template;
import org.apache.velocity.VelocityContext;
import org.apache.velocity.app.VelocityEngine;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;

/**
 * A class uses velocity to show html file
 */
public class HtmlMapper {
    public static void showHtml(HttpServletRequest req, HttpServletResponse resp, String fileName, String status) throws IOException {
//        System.out.println("[HtmlMapper]" + fileName);
        resp.setContentType("text/html");
        resp.setStatus(HttpServletResponse.SC_OK);
        VelocityEngine velocityEngine = (VelocityEngine) req.getServletContext().getAttribute("templateEngine");
        VelocityContext context = new VelocityContext();
        Template template = velocityEngine.getTemplate("web/" + fileName + ".html");
        context.put("name", status);

        StringWriter writer = new StringWriter();
        template.merge(context, writer);

        PrintWriter printWriter = resp.getWriter();
        printWriter.println(writer.toString());
    }
}
