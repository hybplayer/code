package com.hyb.hello;

import java.util.Arrays;
import java.util.Scanner;

public class hello {
    public static void main(String[] args) {
        System.out.println("hello world");
        show1();
        int result = getResult();
        System.out.println(result);
        int[] arr = new int[5];
        for (int i = 0; i < arr.length; i++) {
            arr[i] = i;
            System.out.print(arr[i] + " ");
        }
        System.out.println("");
        method("hello", "world", "hyb");
        System.out.println("");
        swap(arr[0], arr[1]);
        System.out.println(Arrays.toString(arr));
        swapArray(arr);
        System.out.println(Arrays.toString(arr));
    }

    public static void show1() {
        System.out.println("show1");
    }

    public static int getResult() {
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += i;
        }
        return sum;
    }

    public static void method(String... args) {
        for (int i = 0; i < args.length; i++) {
            System.out.print(args[i] + " ");
        }
    }

    public static void swap(int a, int b) {
        int tmp = a;
        a = b;
        b = tmp;
    }

    public static void swapArray(int[] arr) {
        int tmp = arr[0];
        arr[0] = arr[1];
        arr[1] = tmp;
    }
}
