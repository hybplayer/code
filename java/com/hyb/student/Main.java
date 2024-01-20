package com.hyb.student;

/**
 * @author : hyb
 * @date ：2024-01-20 - 19:58
 * @desc :
 */
public class Main {
    public static void main(String[] args) {
        Student s1 = new Student("小明", 1L);
        System.out.println(s1);

        Father father = new Son();
        father.method();

        Student s2 = new Student("小明", 1L);
        System.out.println(s2.hashCode());
        System.out.println(s1.hashCode());
        System.out.println(s1.equals(s2));
    }
}


class Father {
    public void method(){
        System.out.println("父类method方法");
    }
}
//子类继承父类
class Son extends Father {
    //重写父类的方法
    public void method(){
        System.out.println("子类的method方法");
    }
}