package hotelapp;

/**
 * An abstract class to provide processing flow
 */
public abstract class AAppWorker {
    abstract Boolean verifyCommand(String[] args);
    abstract Boolean parseData();
    abstract void serve();
    /**
     * start the processing work
     * @param args command line arguments
     */
    public void startWork(String[] args){
        if(verifyCommand(args)){
            Boolean flag = parseData();
            if(flag){
                serve();
            }
        }
    }
}
