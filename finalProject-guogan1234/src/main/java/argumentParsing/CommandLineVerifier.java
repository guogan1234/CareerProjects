package argumentParsing;

/**
 * a class that verifies command line
 */
public class CommandLineVerifier {
    /**
     * verify the command line
     * @param args command line
     * @return true if the command line is valid. Otherwise, false
     */
    public boolean verify(String[] args) {
//        if(args.length != 4) return false;
//        if(!args[0].contains("-hotels") && !args[2].contains("-hotels")){
//            return false;
//        }
//        if(!args[0].contains("-reviews") && !args[2].contains("-reviews")){
//            return false;
//        }
        if(args.length % 2 != 0) return false;
        System.out.println("Success, congratulations!");
        return true;
    }

    /**
     * gracefully notice users when they input an invalid command line
     */
    public void gracefulNotice() {
        System.out.println("Invalid parameters, parameters example like this:");
        System.out.println("-hotels jsonFilePath -reviews reviewsDirectoryPath");
    }
}
