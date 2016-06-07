#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "IO_JAVAConfig.h"
#include <dlfcn.h>
#include "io_java.h"
#include <memory.h>
#include <uv.h>
#include "Env.h"
#include "Env.cpp"
#include <JavaVM/jni.h>

using namespace io_java;

int loadLib(JNIEnv* jniEnv, const char* path);

int main (int argc, char *argv[])
{
    Env* env = Env::getOrCreate();
    JavaVM* vm = env->getVM();
    JNIEnv* jniEnv = env->getJNIEnv();
    if(argc < 2) {
        printf("no main class specified.");
        return -1;
    }
    
    
    int loadResult = loadLib(jniEnv,"/Users/kenvi/code/study/io.java/build/Debug/libjavaio.dylib");
    if(loadResult < 0) {
        return loadResult;
    }
    jclass cls = jniEnv->FindClass(argv[1]);
    
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
        return -1;
    }
    
    jmethodID mid = jniEnv->GetStaticMethodID(cls, "main", "([Ljava/lang/String;)V");
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
        return -1;
    }
    jniEnv->CallStaticVoidMethod(cls, mid, NULL);
    if (jniEnv->ExceptionOccurred()) {
        jniEnv->ExceptionDescribe();
        printf("exception");
        return -2;
    }
    uv_loop_t* loop = env->getEventLoop();
    
    uv_run(loop, UV_RUN_ONCE);
   
    
    
    printf("hello world from io.java");
    
    vm->DestroyJavaVM();
    
    
    
    return 0;
}
int loadLib(JNIEnv* jniEnv, const char* path) {
    jclass cls = jniEnv->FindClass("io/java/lang/System");
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
        return -1;
    }
    jmethodID mid = jniEnv->GetStaticMethodID(cls, "load", "()V");
 //   jmethodID mid = jniEnv->GetStaticMethodID(cls, "load", "(Ljava/lang/String;)V");
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
        return -1;
    }
    jniEnv->CallStaticVoidMethod(cls, mid,jniEnv->NewStringUTF(path));
    if (jniEnv->ExceptionOccurred()) {
        printf("exception");
        jniEnv->ExceptionDescribe();
    }
    
    return 0;
    
    
}



