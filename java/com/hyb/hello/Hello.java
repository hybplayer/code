package com.hyb.hello;

import java.util.Arrays;
import java.util.Scanner;

public class Hello {
    public static void main(String[] args) {
        Person p = new Person();
        String[] fullname = new String[]{"Homer", "Simpson"};
        p.setName(fullname); // 传入fullname数组
        System.out.println(p.getName()); // "Homer Simpson"
        fullname[0] = "Bart"; // fullname数组的第一个元素修改为"Bart"
        System.out.println(p.getName()); // "Homer Simpson"还是"Bart Simpson"?

        Person1 p1 = new Person1();
        String bob = "Bob";
        p1.setName(bob); // 传入bob变量
        System.out.println(p1.getName()); // "Bob"
        bob = "Alice"; // bob改名为Alice
        System.out.println(p1.getName()); // "Bob"还是"Alice"?

        String n1 = "aaa";
        System.out.println(n1.hashCode());
        String n2 = n1;
        System.out.println(n1 == n2);
        n1 = "bbb";
        System.out.println(n2);
        System.out.println(n2.hashCode());
        swap(n1, n2);
        System.out.println(n2.hashCode());
        System.out.println(n2);
        n1 = "ccc";
        System.out.println(n2);
    }

    public static void swap(String s1, String s2) {
        System.out.println(s2.hashCode());
        s2 = s1;
        System.out.println(s2.hashCode());
    }
}


class Person {
    private String[] name;

    public String getName() {
        return this.name[0] + " " + this.name[1];
    }

    public void setName(String[] name) {
        this.name = name;
    }
}

class Person1 {
    private String name;

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }
}