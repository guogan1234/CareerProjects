package hotelapp;

import data.HotelManager;
import argumentParsing.*;
import data.ThreadSafeHotelManager;
import parsing.ADataParser;
import parsing.DataParser;
import server.WebServer;

/**
 * main framework worker
 */
public class AppWorker extends AAppWorker {
    private static AppWorker singleton = new AppWorker();
    private CommandLineParser commandLineParser;
    private HotelManager hotelManager = new ThreadSafeHotelManager();
    private final int PORT = 8080;

//    private AppWorker(){}
//
//    /**
//     * return a singleton of AppWorker
//     * @return a singleton of AppWorker
//     */
//    public static AppWorker getInstance(){
//        return singleton;
//    }
    public AppWorker(){}

    /**
     * verify command line
     * @param args command line
     * @return true if command line is valid. Otherwise false
     */
    @Override
    public Boolean verifyCommand(String[] args) {
        System.out.println("[AppWorker verifyCommand]");
        CommandLineVerifier verifier = new CommandLineVerifier();
        CommandLineStorer storer = new CommandLineStorer();
        commandLineParser = new CommandLineParser(verifier, storer);
        return commandLineParser.parse(args);
    }

    /**
     * parse data in json files
     * @return true if data parsed. Otherwise false
     */
    @Override
    public Boolean parseData() {
        System.out.println("[AppWorker parseData]");
        if(commandLineParser != null){
            String hotelsPath = commandLineParser.getHotelsPath();
            String reviewsPath = commandLineParser.getReviewsPath();

            ADataParser dataParser = new DataParser(hotelsPath, reviewsPath);
            // get thread num
            int threadNum = 0;
            String threads = commandLineParser.getThreadNum();
            if(threads == null){
                threadNum = 1;
            }else {
                threadNum = Integer.parseInt(threads);
            }
            String outputPath = commandLineParser.getOutputPath();
            if(outputPath == null){
                threadNum = 0;
            }
            dataParser.setThreadNum(threadNum);
            dataParser.parseData(hotelManager);
            // print output file
//            printToOutputFile(outputPath, hotelsPath, reviewsPath);
            return true;
        }
        return false;
    }

    /**
     * provide service when querying
     */
    @Override
    public void serve() {
        System.out.println("[AppWorker serve]");
        WebServer webServer = new WebServer();
        webServer.startServer();
    }

    /**
     * print information to file
     * @param outputPath output file path
     * @param hotelsPath hotels information file path
     * @param reviewsPath reviews information file path
     */
    private void printToOutputFile(String outputPath, String hotelsPath, String reviewsPath){
        if(outputPath == null) return;
        if(reviewsPath == null){
            hotelManager.printHotelsToFile(outputPath);
            return;
        }
        hotelManager.printReviewsToFile(outputPath);
    }
}
