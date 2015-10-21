// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "IO_JAVAConfig.h"
#include <dlfcn.h>
#include "io_java.h"
#include <memory.h>
#include <event2/event.h>
#include <Env.h>
#include <Env.cpp>
#include <JavaVM/jni.h>

using namespace io_java;

void cb_func(evutil_socket_t fd, short what, void *arg)
{
    struct event *me = (event*)arg;
    
    printf("cb_func called %d times so far.\n");
    
}

int main (int argc, char *argv[])
{
    Env* env = Env::getOrCreate();
    JavaVM* vm = env->getVM();
    JNIEnv* jniEnv = env->getJNIEnv();
    jclass cls = jniEnv->FindClass("Test");
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
        return -1;
    }
    
    jmethodID mid = jniEnv->GetStaticMethodID(cls, "main", "()V");
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
        return -1;
    }
    jniEnv->CallStaticVoidMethod(cls, mid);
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        return -2;
    }
    struct event_base* loop = env->getEventLoop();
    struct timeval one_sec = { 5, 0 };
    struct event *ev;
    ev = event_new(loop, -1, EV_PERSIST, cb_func, NULL);
    event_add(ev, &one_sec);
    event_base_dispatch(loop);
   
    
    
    printf("hello world from io.java");
    
    vm->DestroyJavaVM();
    
    
    
    return 0;
}



