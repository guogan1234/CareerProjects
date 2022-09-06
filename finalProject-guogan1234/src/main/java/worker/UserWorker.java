package worker;

import data.User;
import model.UserModel;
import model.manager.UserManager;
import utils.SaltUtils;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * A class that handles user info
 */
public class UserWorker {
    private UserModel model;

    /**
     * Constructor
     */
    public UserWorker(){
        model = new UserManager();
    }

    /**
     * Register user info to database
     * @param name username
     * @param password password
     * @return Register status
     */
    public boolean registerUser(String name, String password){
        if(name == null || password == null) return false;
        boolean satisfy = validatePassword(password);//regex
        if(!satisfy) return false;
        boolean duplicate = checkDuplicateName(name);
        if(!duplicate) {
            User user = new User();
            user.setName(name);
            user.setSaltedPassword(SaltUtils.saltData(password));
            user.setSalt(SaltUtils.getSalt());
            int result = model.add(user);
            if(result != 0) return true;
        }
        return false;
    }

    /**
     * Validate user whether can login
     * @param name username
     * @param password password
     * @return the flag whether user can login
     */
    public boolean validateLogin(String name, String password){
        User user = model.findByName(name);
        if(user == null) return false;
        String saltedPassword = user.getSaltedPassword();
        String salt = user.getSalt();
        String matched = SaltUtils.saltDataWithSalt(password, salt);
        if(matched != null){
            return matched.equals(saltedPassword);
        }
        return false;
    }

    /**
     * Check username is duplicate or not
     * @param name username
     * @return the flag whether username is duplicate
     */
    private boolean checkDuplicateName(String name){
        User user = model.findByName(name);
        if(user == null) return false;
        return true;
    }

    /**
     * Validate password
     * @param password password
     * @return the flag whether password is valid
     */
    private boolean validatePassword(String password){
        if(password == null) return false;
        if(password.length() < 5 || password.length() > 10) return false;
        Pattern numPattern = Pattern.compile("\\d+");
        Matcher numMatcher = numPattern.matcher(password);
        boolean numMatched = numMatcher.find();
        if(!numMatched) return false;
        Pattern letterPattern = Pattern.compile("[a-zA-Z]+");
        Matcher letterMatcher = letterPattern.matcher(password);
        boolean letterMatched = letterMatcher.find();
        if(!letterMatched) return false;
        Pattern specialPattern = Pattern.compile("[\\W]+");
        Matcher specialMatcher = specialPattern.matcher(password);
        boolean specialCharacterMatched = specialMatcher.find();
        if(!specialCharacterMatched){
            return false;
        }
        return true;
    }
}
