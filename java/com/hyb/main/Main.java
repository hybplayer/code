package com.hyb.main;

import com.sun.xml.internal.ws.api.ha.StickyFeature;
import javafx.util.Pair;

import java.io.*;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
import java.lang.reflect.*;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;

/**
 * @author : hyb
 * @date ：2024-03-03 - 10:43
 * @desc :
 */
public class Main {
    public static void main(String[] args) throws IOException {
        String regex = "20\\d\\d";
        System.out.println("2019".matches(regex));
        System.out.println("2100".matches(regex));
    }
}