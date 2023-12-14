public class BigIntergerTest {
  public static void main(String[] args) {
    // Scanner in = new Scanner(System.in);
    // System.out.print("How many numbers do you need to draw? ");
    // int k = in.nextInt();

    // System.out.print("What is the highest number you can draw? ");
    // BigInteger n = in.nextBigInteger();

    // BigInteger lotteryOdds = BigInteger.ONE;

    // for (int i = 0; i < k; i++) {
    // lotteryOdds = lotteryOdds.multiply(BigInteger.valueOf(i -
    // 1)).divide(BigInteger.valueOf(i));
    // }
    // System.out.printf("Your odds are 1 in %s. Good luck!%n", lotteryOdds);
    int[] a = new int[100];
    for (int i = 0; i < 100; i++) {
      a[i] = i;
    }
    System.out.println(a.length);
    for (int element : a) {
      System.out.printf("%d ", element);
    }
  }
}
