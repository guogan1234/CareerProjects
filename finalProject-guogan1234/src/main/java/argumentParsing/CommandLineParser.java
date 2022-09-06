package argumentParsing;

/**
 * a class that parse command line
 */
public class CommandLineParser {
    private CommandLineVerifier verifier;
    private CommandLineStorer storer;

    /**
     * constructor
     * @param verifier a Verifier class
     * @param storer a Storer class
     */
    public CommandLineParser(CommandLineVerifier verifier, CommandLineStorer storer){
        this.verifier = verifier;
        this.storer = storer;
    }

    /**
     * parse command line
     * @param args command line
     * @return return true if command line is valid. Otherwise, return false.
     */
    public boolean parse(String[] args){
        boolean flag = verify(args);
        if(!flag){
            gracefulNotice();
            return false;
        }
        store(args);
        return true;
    }

    /**
     * verify command line
     * @param args command line
     * @return return true if command line is valid. Otherwise, return false.
     */
    boolean verify(String[] args) {
        return verifier.verify(args);
    }

    /**
     * store command line
     * @param args command line
     */
    void store(String[] args) {
        storer.store(args);
    }

    /**
     * gracefully notice users if their command line is invalid
     */
    void gracefulNotice() {
        verifier.gracefulNotice();
    }

    /**
     * get json file path which contains hotel information
     * @return a string of file path
     */
    public String getHotelsPath(){
        if(storer != null){
            return storer.getHotelPath();
        }
        return null;
    }

    /**
     * get directory path which contains reviews information
     * @return a string of directory path
     */
    public String getReviewsPath(){
        if(storer != null){
            return storer.getReviewsPath();
        }
        return null;
    }

    /**
     * get output print path
     * @return a string of directory path
     */
    public String getOutputPath(){
        if(storer != null){
            return storer.getOutPutPath();
        }
        return null;
    }

    /**
     * get thread number string
     * @return a string of thread number
     */
    public String getThreadNum(){
        if(storer != null){
            return storer.getThreadNum();
        }
        return null;
    }
}
