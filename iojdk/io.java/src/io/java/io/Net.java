package io.java.io;

import io.java.lang.Callback;

/**
 * Created by kenvi on 16/5/15.
 */
public class Net {
    public native static void connect(String addr, int port, Callback callback);
}
