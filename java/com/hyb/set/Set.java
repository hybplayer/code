package com.hyb.set;

import com.hyb.student.*;

import java.util.*;

/**
 * @author : hyb
 * @date ：2024-01-20 - 22:45
 * @desc :
 */
public class Set {
    public static void main(String[] args) {
        Collection<Student> collection = new ArrayList<>();

        Student student1 = new Student("小红", 18);
        Student student2 = new Student("小明", 19);
        Student student3 = new Student("小辉", 18);

        collection.add(student1);	//添加方法
        collection.add(student2);
        collection.add(student3);

        //for循环的形式
        for(Iterator<Student> it = collection.iterator(); it.hasNext();){
            Student student = it.next();
            System.out.println(student);
        }

        //while循环的形式
        Iterator<Student> it = collection.iterator();
        while(it.hasNext()){
            Student student = it.next();
            System.out.println(student);
        }

        List list = new ArrayList();
        list.add(1);
    }
}
