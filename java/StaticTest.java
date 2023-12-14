public class StaticTest {
  public static void main(String[] args) {
    Employee[] staff = new Employee[3];

    staff[0] = new Employee("Tom", 40000);
    staff[1] = new Employee("Dick", 60000);
    staff[2] = new Employee("Harry", 65000);

    for (Employee e : staff) {
      System.out.println("name=" + e.getName() + ",id=" + e.getId() +
                         ",salary=" + e.getSalary());
    }

    int n = Employee.getNextId();
    System.out.println("Next available id=" + n);
  }
}

class Employee {
  private static int nextId = 1;
  private String name;
  private double salary;
  private int id;

  public static int advanceId() {
    int r = nextId;
    nextId++;
    return r;
  }

  public static int getNextId() { return nextId; }

  public static void main(String[] args) {
    // var e = new Employee("Marry", 50000);
    Employee e = new Employee("Marry", 50000);
    System.out.println(e.getName() + " " + e.getSalary());
  }

  public Employee(String n, double s) {
    name = n;
    salary = s;
    id = advanceId();
  }

  public String getName() { return name; }

  public double getSalary() { return salary; }

  public int getId() { return id; }
}