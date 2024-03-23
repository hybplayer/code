package com.hyb.mylog;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * @author : hyb
 * @date ：2024-03-21 - 12:44
 * @desc :
 */
public class MyLog {
    public static void main(String[] args) {
        Log log = LogFactory.getLog(MyLog.class);
        log.info("start...");
        log.warn("end.");
        log.error("error!");
    }
}
