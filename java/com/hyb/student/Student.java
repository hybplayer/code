package com.hyb.student;

import java.util.Objects;

/**
 * @author : hyb
 * @date ：2024-01-20 - 19:53
 * @desc :
 */

public class Student {
    String name;
    {
        System.out.println("构造代码块开始执行");
    }

    @Override
    public String toString() {
        return "Student{" +
                "name='" + name + '\'' +
                ", sid=" + sid +
                '}';
    }

    long sid;

    public String getName() {
        return name;
    }

    public long getSid() {
        return sid;
    }

    public void setName(String name) {
        this.name = name;
    }

    public void setSid(long sid) {
        this.sid = sid;
    }

    public Student(String name, long sid) {
        this.name = name;
        this.sid = sid;
        System.out.println("构造函数开始执行");
    }

    public Student() {
        System.out.println("构造函数开始执行");
    }

    public void eat() {
        System.out.println("学生吃饭");
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof Student)) return false;
        Student student = (Student) o;
        return getSid() == student.getSid() && Objects.equals(getName(), student.getName());
    }

    @Override
    public int hashCode() {
        return Objects.hash(getName(), getSid());
    }
}
