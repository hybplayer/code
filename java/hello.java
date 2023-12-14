import java.io.IOException;
import java.nio.file.Paths;
import java.util.Scanner;

public class hello {
    /**
     * hello world
     * 
     * @param args null
     */
    public static void main(String[] args) throws IOException {
        /**
         * true true
         */
        // String s = "hyb";
        // String t = "hyb";
        // System.out.println(s.equals(t));
        // System.out.println(s == t);

        // String ch = "jk";
        // StringBuilder builder = new StringBuilder();
        // builder.append(ch);
        // String complString = builder.toString();
        // System.out.println(complString);

        // Scanner in = new Scanner(System.in);
        // System.out.println("What is your name");
        // String name = in.nextLine();
        // System.out.println("hello " + name);

        String dir = System.getProperty("user.dir");
        Scanner in = new Scanner(Paths.get(dir, "test.txt"), "UTF-8");
        System.out.println(in.nextLine());
        in.close();

    };
}
