package io.java.lang;

/**
 * Created by kenvi on 15/10/18.
 */
public class System {
    public static native int setTimeout(Callback callback, int millions, Object ... args);

    public static native int setRepeate(Callback callback, int millions, Object ... args);
}
