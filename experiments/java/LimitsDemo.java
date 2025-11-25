import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class LimitsDemo {
    // Custom constant names
    public static final int MAX_INT = Integer.MAX_VALUE;
    public static final int MIN_INT = Integer.MIN_VALUE;
    public static final double INF = Double.POSITIVE_INFINITY;
    public static final double NEG_INF = Double.NEGATIVE_INFINITY;
    public static final double QUIET_NAN = Double.NaN;

    public static void main(String[] args) {
        System.out.println("Java Numeric Limits");
        System.out.println("-------------------");
        System.out.println("INT_MAX: " + MAX_INT);
        System.out.println("INT_MIN: " + MIN_INT);
        System.out.println("Infinity: " + INF);
        System.out.println("Negative Infinity: " + NEG_INF);
        System.out.println("NaN (Not a Number): " + QUIET_NAN);

        // Demonstrate checks
        System.out.println("Is INF infinite? " + Double.isInfinite(INF));
        System.out.println("Is QUIET_NAN NaN? " + Double.isNaN(QUIET_NAN));
    }
}
