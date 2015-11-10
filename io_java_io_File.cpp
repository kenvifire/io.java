//
//  io_java_io.File.cpp
//  io.java
//
//  Created by kenvi.zhu on 15/10/28.
//
//

#include <jni.h>
#include <stdio.h>
#include "io_java_io_File.h"
#include <fcntl.h>
#include "uv.h"
#include "type.h"
#include <stdlib.h>
#include "Env.h"

using namespace io_java;



JNIEXPORT void JNICALL Java_io_java_io_File_Read
(JNIEnv * env, jobject file, jobject callback, jobjectArray args){
    jclass thisClass = env->GetObjectClass(file);
    jfieldID fileNameId = env->GetFieldID(thisClass, "fileName", "Ljava/lang/String;");
    
    jstring fileName = (jstring)env->GetObjectField(file, fileNameId);
    const char *nativeFileStr = env->GetStringUTFChars(fileName, NULL);
    int fd = open(nativeFileStr,O_RDONLY);
    Env* ioEnv = Env::getOrCreate();
    
    
    jclass cls = env->FindClass("io/java/lang/Callback");
    
    if (env->ExceptionOccurred()) {
        printf("exception 1");
        env->ExceptionDescribe();
        return -1;
    }
    jmethodID methodId = env->GetMethodID(cls, "apply", "([Ljava/lang/Object;)V");
    if (env->ExceptionOccurred()) {
        printf("exception 2");
        env->ExceptionDescribe();
        return -2;
    }
    
    struct callbak_args * callbackArgs = (struct callbak_args*) malloc(sizeof(struct callbak_args));
   
    
    callbackArgs->env = env;
    callbackArgs->callback = callback;
    callbackArgs->methodId = methodId;
    callbackArgs->args = args;
    
    
  
    
}
/*
void cb_func_file(evutil_socket_t fd, short what, void *arg)
{
    struct callbak_args* callbackArgs = (struct callbak_args*) arg;
    
    JNIEnv* env = callbackArgs->env;
    env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId,callbackArgs->args);
}
*/