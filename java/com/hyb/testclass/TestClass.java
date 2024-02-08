package com.hyb.testclass;

/**
 * @author : hyb
 * @date ：2024-01-24 - 20:21
 * @desc :
 */
public class TestClass {
    public static void main(String[] args) {
        Person p1 = new Student();
        Person p2 = new Person();
        p1.print();
        p2.print();

        Income[] incomes = new Income[]{
                new Income(3000),
                new Salary(7500),
                new StateCouncilSpecialAllowance(15000)
        };
        System.out.println(totalTax(incomes));
    }

    public static double totalTax(Income... incomes)
    {
        double total = 0;
        for(Income income : incomes)
        {
            total = total + income.getTax();
        }
        return total;
    }
}

class Person {
    public void print() {
        System.out.println("Person-print");
    }
}

class Student extends Person {
    @Override
    public void print() {
        System.out.println("Student-print");
    }
}

class Income {
    protected double income;

    public Income(double income) {
        this.income = income;
    }

    public double getTax() {
        return income * 0.1;
    }
}

class Salary extends Income {
    public Salary(double income) {
        super(income);
    }

    @Override
    public double getTax() {
        if (income <= 5000) {
            return 0;
        }
        return (income - 5000) * 0.2;
    }
}

class StateCouncilSpecialAllowance extends Income {
    public StateCouncilSpecialAllowance(double income) {
        super(income);
    }

    @Override
    public double getTax() {
        return 0;
    }
}