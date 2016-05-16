package io.java.io;

import io.java.lang.Callback;

/**
 * Created by kenvi on 15/10/28.
 */
public class File {

    //Open(path, flags, mode, function(){});
    public static native void Open(String path, String flags, Integer mode,
                                   Callback callback, Object... args);
    //Read(fd, buffer, offset, length,position, function() {})
    public static native void Read(int fd, Callback callback);

    //Write(fd, data, function(){})
    public static native void Write(int fd, byte[] data, Callback callback);

}
