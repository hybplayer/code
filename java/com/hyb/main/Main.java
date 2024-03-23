package com.hyb.main;

import java.io.*;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.Scanner;
import java.util.concurrent.Callable;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
//import java.util.logging.Handler;

/**
 * @author : hyb
 * @date ：2024-03-03 - 10:43
 * @desc :
 */
public class Main {
    public static void main(String[] args) throws InterruptedException, IOException {
        ServerSocket serverSocket = new ServerSocket(8080);
        System.out.println("server is running");
        while (true)
        {
            Socket sock = serverSocket.accept();
            System.out.println("connect from " + sock.getRemoteSocketAddress());
            Thread thread = new Handler(sock);
            thread.start();
        }
    }
}

class Handler extends Thread{
    Socket sock;
    public Handler(Socket sock)
    {
        this.sock = sock;
    }

    @Override
    public void run() {
        try (InputStream input = this.sock.getInputStream()){
            try(OutputStream output = this.sock.getOutputStream())
            {
                handle(input, output);
            }
        } catch (Exception e) {
            try {
                this.sock.close();
            }catch (IOException ioe)
            {
                ioe.printStackTrace();
            }
        }
    }

    private void handle(InputStream input, OutputStream output) throws IOException{
        BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(output, StandardCharsets.UTF_8));
        BufferedReader reader = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8));
        writer.write("hello\n");
        writer.flush();
        while (true)
        {
            String s = reader.readLine();
            if(s.equals("bye"))
            {
                writer.write("bye\n");
                writer.flush();
                break;
            }
            writer.write("ok:" + s +"\n");
            writer.flush();
        }
    }
}