package com.hyb.string;

/**
 * @author : hyb
 * @date ：2024-01-20 - 21:17
 * @desc :
 */
public class string {
    public static void main(String[] args) {
        String name = "abc";
        String name2 = "abc";
        System.out.println(name == name2);
        int num = 123;
        String numStr = String.valueOf(num);
        System.out.println(numStr);
        System.out.println(String.valueOf(new Animal()));

        StringBuffer stringBuffer = new StringBuffer();
        System.out.println(stringBuffer.capacity());
        stringBuffer.append("A");
        System.out.println(stringBuffer);
    }
}

class Animal{

    @Override
    public String toString() {
        return "hello Animal";
    }

}
