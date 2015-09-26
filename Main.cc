// A simple program that computes the square root of a number
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include "IO_JAVAConfig.h"
#include <dlfcn.h>
#include "io_java.h"
#include <memory.h>


int main (int argc, char *argv[])
{
//    void *libjvm;
//    libjvm = dlopen("/Users/kenvi/code/study/jdk9/build/macosx-x86_64-normal-server-fastdebug/jdk/lib/server/libjvm.dylib", RTLD_NOW + RTLD_GLOBAL);
//    
////    libjvm = dlopen("/System/Library/Frameworks/JavaVM.framework/Versions/1.6.0/Libraries/libserver.dylib", RTLD_NOW + RTLD_GLOBAL);
//    
//    if(libjvm == NULL) {
//        return -1;
//    }
    JavaVM *jvm;       /* denotes a Java VM */
    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    JavaVMOption* options = new JavaVMOption[1];
    options[0].optionString = "-Djava.class.path=/usr/lib/java";
    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 1;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = false;
    /* load and initialize a Java VM, return a JNI interface
     * pointer in env */
    JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
    delete options;
    /* invoke the Main.test method using the JNI */
//    jclass cls = env->FindClass("Main");
//    jmethodID mid = env->GetStaticMethodID(cls, "test", "(I)V");
//    env->CallStaticVoidMethod(cls, mid, 100);
    /* We are done. */
    jvm->DestroyJavaVM();
//    
    
    printf("hello world from io.java");
    
    
    
    
    return 0;
}
