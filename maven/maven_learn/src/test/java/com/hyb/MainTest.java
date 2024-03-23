package com.hyb;

import org.junit.Test;
import static org.junit.jupiter.api.Assertions.*;

/**
 * @author : hyb
 * @date ：2024-03-20 - 21:56
 * @desc :
 */
public class MainTest {
    @Test
    public void testshow() {
        Main test = new Main();
        test.show();
    }

    @Test
    public void testFact() {
        assertEquals(1, Main.fact(1));
        assertEquals(2, Main.fact(2));
        assertEquals(6, Main.fact(3));
        assertEquals(3628800, Main.fact(10));
        assertEquals(2432902008176640000L, Main.fact(20));
        System.out.println("over");
    }
}
