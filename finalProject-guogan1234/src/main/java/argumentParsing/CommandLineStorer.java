package argumentParsing;

import java.util.HashMap;

/**
 * a class store command line
 */
public class CommandLineStorer {
    private HashMap<String,String> parameters;
    private final String HOTEL_COMMAND = "-hotels";
    private final String REVIEW_COMMAND = "-reviews";
    private final String OUTPUT_COMMAND = "-output";
    private final String THREAD_COMMAND = "-threads";

    /**
     * store command line
     * @param args
     */
    public void store(String[] args) {
        parameters = new HashMap<>();

        int len = args.length;
        for(int i = 0; i < len && i+1 < len; i++){
            if(i % 2 == 0){
                parameters.put(args[i] , args[i+1]);
            }
        }

        for(String key: parameters.keySet()){
            System.out.println("#:" + key + "," + parameters.get(key));
        }
    }

    /**
     * get element from a key
     * @param key a key string to find element
     * @return a string of the returned element
     */
    public String get(String key) {
        return parameters.get(key);
    }

    /**
     * get a json file path which contains hotel information
     * @return a string of a json file path
     */
    public String getHotelPath(){
        return get(HOTEL_COMMAND);
    }

    /**
     * get a directory path which contains reviews information
     * @return a string of the specific directory path
     */
    public String getReviewsPath(){
        return get(REVIEW_COMMAND);
    }

    /**
     * get output print path
     * @return a string of directory path
     */
    public String getOutPutPath(){
        return get(OUTPUT_COMMAND);
    }

    /**
     * get thread number string
     * @return a string of thread number
     */
    public String getThreadNum(){
        return get(THREAD_COMMAND);
    }
}
