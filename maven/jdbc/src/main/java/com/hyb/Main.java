package com.hyb;

import java.sql.*;

/**
 * @author : hyb
 * @date ：2024-03-24 - 0:27
 * @desc :
 *///TIP 要<b>运行</b>代码，请按 <shortcut actionId="Run"/> 或
// 点击装订区域中的 <icon src="AllIcons.Actions.Execute"/> 图标。
public class Main {
    public static void main(String[] args) throws SQLException {
// JDBC连接的URL, 不同数据库有不同的格式:
        String JDBC_URL = "jdbc:mysql://localhost:3306/learnjdbc";
        String JDBC_USER = "root";
        String JDBC_PASSWORD = "root";
        try (Connection conn = DriverManager.getConnection(JDBC_URL, JDBC_USER, JDBC_PASSWORD)) {
            try (Statement stmt = conn.createStatement()) {
                try (ResultSet rs = stmt.executeQuery("SELECT id, grade, name, gender FROM students WHERE gender=1")) {
                    while (rs.next()) {
                        long id = rs.getLong(1); // 注意：索引从1开始
                        long grade = rs.getLong(2);
                        String name = rs.getString(3);
                        int gender = rs.getInt(4);
                        System.out.println("id:" + id);
                        System.out.println("name:" + name);
                        System.out.println("grade:" + grade);
                        System.out.println("gender:" + gender);
                        System.out.println("-----------------------------");
                    }
                }
            }
        }
    }
}