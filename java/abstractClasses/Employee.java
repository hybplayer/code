package abstractClasses;

import java.time.LocalDate;

public class Employee extends Person {
  private double salary;
  private LocalDate hirDate;

  public Employee(String name, double salary, int year, int month, int day) {
    super(name);
    this.salary = salary;
    hirDate = LocalDate.of(year, month, day);
  }

  public double getSalary() { return salary; }

  public LocalDate getHireDay() { return hirDate; }

  public String getDescription() { return "an employee"; }

  public void raiseSalary(double byPercent) {
    double raise = salary * byPercent / 100;
    salary += raise;
  }
}
