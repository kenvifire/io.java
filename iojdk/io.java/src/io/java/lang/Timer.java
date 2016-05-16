package io.java.lang;


/**
 * Created by kenvi on 15/11/8.
 */
public class Timer {

    public Timer(Callback callback,long timeout,int repeat,Object... args) {
        this.callback = callback;
        this.timeout = timeout;
        this.repeat = repeat;
        this.args = args;
    }


    private long timeout;
    private Callback callback;
    private int repeat;
    private Object[] args;

    public long getTimeout() {
        return timeout;
    }

    public void setTimeout(long timeout) {
        this.timeout = timeout;
    }

    public Callback getCallback() {
        return callback;
    }

    public void setCallback(Callback callback) {
        this.callback = callback;
    }

    public int getRepeat() {
        return repeat;
    }

    public void setRepeat(int repeat) {
        this.repeat = repeat;
    }

    public native int start();

    public native void stop();
}
