package com.hyb.cls;

import com.sun.xml.internal.ws.api.ha.StickyFeature;

/**
 * @author : hyb
 * @date ：2024-02-26 - 20:49
 * @desc :
 */
public class Cls {
    public static void main(String[] args) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
//        Class s1 = String.class;
//        String t = "hello";
//        Class s2 = t.getClass();
//        Class s3 = Class.forName("java.lang.String");
//        System.out.println(s1 == s2);
//        System.out.println(s2 == s3);
//        printObjectInfo("abc");
//        printObjectInfo(2);
        Class cls = String.class;
        String s = (String)cls.newInstance();

    }

    static void printObjectInfo(Object obj)
    {
        Class cls = obj.getClass();
        System.out.println(cls.toString());
    }
}
