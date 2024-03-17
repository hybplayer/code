package junit_test.src;

/**
 * @author : hyb
 * @date ：2024-03-10 - 15:57
 * @desc :
 */
public class Factorial {
    public static long fact(long n) {
        long r = 1;
        for (long i = 1; i <= n; i++) {
            r = r * i;
        }
        return r;
    }
}
