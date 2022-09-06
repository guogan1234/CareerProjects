package utils;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Random;

/**
 * A util that salts info
 */
public class SaltUtils {
    private static Random random = new Random(System.currentTimeMillis());
    private static String salt = null;

    /**
     * Returns the hex encoding of a byte array.
     * @param bytes - byte array to encode
     * @param length - desired length of encoding
     * @return hex encoded byte array
     */
    private static String encodeHex(byte[] bytes, int length) {
        BigInteger bigint = new BigInteger(1, bytes);
        String hex = String.format("%0" + length + "X", bigint);

        assert hex.length() == length;
        return hex;
    }

    /**
     * Get the salt used
     * @return the used salt
     */
    public static String getSalt(){
        return salt;
    }

    /**
     * Salt some data
     * @param data a string of data
     * @return salted data
     */
    public static String saltData(String data){
        byte[] saltBytes = new byte[16];
        random.nextBytes(saltBytes);
        salt = encodeHex(saltBytes, 32);
        return saltDataWithSalt(data, salt);
    }

    /**
     * Salt some data with specific salt
     * @param data a string of data
     * @param salt a specific salt
     * @return salted data
     */
    public static String saltDataWithSalt(String data, String salt){
        String result = null;
        String salted = data + salt;

        MessageDigest md = null;
        try {
            md = MessageDigest.getInstance("SHA-256");
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        }
        md.update(salted.getBytes());
        result = encodeHex(md.digest(), 64);
        return result;
    }
}
