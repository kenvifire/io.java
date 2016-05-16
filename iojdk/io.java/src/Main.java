import io.java.io.Net;
import io.java.lang.*;

import java.lang.System;

/**
 * Created by kenvi on 15/10/21.
 */
public class Main {
    public static void main(String[] args){
        System.load("/Users/kenvi/code/study/io.java/Debug/liblibiojava.a");
//        Timer timer = new Timer(new Callback() {
//            @Override
//            public void apply(Object... args) {
//                System.out.println("test");
//            }
//        },10000,1);
//        timer.start();

//        File.Open("/tmp/test.dat",
//                "rw", null, new Callback() {
//                    @Override
//                    public void apply(Object... args) {
//                        String error = (String) args[0];
//                        if(error == null) {
//                            Integer fd = (Integer) args[1];
//                            System.out.println("open file success, fd is " + fd);
//                            int bufLen = 1024;
//                            String str = new String("test write");
//
//                            byte[] data = str.getBytes();
//                            File.Write(fd, data, new Callback() {
//                                @Override
//                                public void apply(Object... args) {
//                                    String error = (String)args[0];
//                                    if(error != null) {
//                                        System.out.println(error);
//                                    }
//                                }
//                            });
//                        }else {
//                            System.out.println("open error:" + error);
//                        }
//                    }
//                });

//        File.Read(40, new Callback() {
//            @Override
//            public void apply(Object... args) {
//                System.out.println("========");
//                String error = (String) args[0];
//                if(error != null) {
//                    System.out.println(error);
//                }
//            }
//        });
        Net.connect("127.0.0.1", 8080, new Callback() {
            @Override
            public void apply(Object... args) {

            }
        });


    }
}
