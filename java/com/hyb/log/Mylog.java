package com.hyb.log;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

/**
 * @author : hyb
 * @date ：2024-02-25 - 19:52
 * @desc :
 */
public class Mylog {
    public static void main(String[] args) {
        Log log = LogFactory.getLog(Mylog.class);
        log.info("start...");
        log.error("end.");
    }
}
