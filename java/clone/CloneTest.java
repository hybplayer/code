package clone;

public class CloneTest {
  public static void main(String[] args) {
    Employee original = new Employee("John Q. public", 50000);
    original.setHireDay(2000, 1, 1);
    try {
      Employee copy = original.clone();
      copy.raiseSalary(10);
      copy.setHireDay(2002, 12, 31);
      System.out.println("original=" + original);
      System.out.println("copy=" + copy);
    } catch (CloneNotSupportedException exception) {
    } finally {
    }
  }
}
