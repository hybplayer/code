package interfaces;

public class Employee implements Comparable<Employee> {
  private String name;
  private double salary;

  public Employee(String name, double salary) {
    this.name = name;
    this.salary = salary;
  }

  public String getName() { return name; }

  public double getSalary() { return salary; }

  public void raiseSalary(double byPercnet) {
    double raise = salary * byPercnet / 100;
    salary += raise;
  }

  public int compareTo(Employee other) {
    return Double.compare(salary, other.salary);
  }
}
