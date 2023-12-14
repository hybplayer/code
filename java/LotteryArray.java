public class LotteryArray {
  public static void main(String[] args) {
    final int NMAX = 5;
    int[][] odds = new int[NMAX][];
    for (int i = 0; i < NMAX; i++) {
      odds[i] = new int[i + 1];
    }
    int start = 0;
    for (int i = 0; i < odds.length; i++) {
      for (int j = 0; j < odds[i].length; j++) {
        odds[i][j] = start++;
        System.out.printf("%d ", odds[i][j]);
      }
      System.out.println("");
    }
  }
}
