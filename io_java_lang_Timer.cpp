#include <jni.h>
#include "Env.h"
#include "io_java_lang_Timer.h"
#include <stdlib.h>
#include "type.h"
#include "uv.h"

using namespace io_java;

void OnTimeout(uv_timer_t* handle);

JNIEXPORT jint JNICALL Java_io_java_lang_Timer_start(JNIEnv * env, jobject thisObj){
    Env* jenv = Env::getOrCreate();
    uv_loop_t* loop = jenv->getEventLoop();
    
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
    
    jclass thisCls = env->GetObjectClass(thisObj);
    if (env->ExceptionOccurred()) {
        printf("exception 3");
        env->ExceptionDescribe();
        return -3;
    }
    
    jfieldID argsId = env->GetFieldID(thisCls, "args", "[Ljava/lang/Object;");
    
    if (env->ExceptionOccurred()) {
        printf("exception 6");
        env->ExceptionDescribe();
        return -6;
    }
    jobject args= env->GetObjectField(thisCls, argsId);
    if (env->ExceptionOccurred()) {
        printf("exception 7");
        env->ExceptionDescribe();
        return -7;
    }
    jfieldID callbackId = env->GetFieldID(thisCls, "callback", "Lio/java/lang/Callback;");
    if (env->ExceptionOccurred()) {
        printf("exception 4");
        env->ExceptionDescribe();
        return -4;
    }
    jobject callback = env->GetObjectField(thisObj, callbackId);
    if (env->ExceptionOccurred()) {
        printf("exception 5");
        env->ExceptionDescribe();
        return -5;
    }
   
    
  //  jobjectArray* args = reinterpret_cast(jobjectArray*)(&argsObj);
    
    
    
    struct callbak_args * callbackArgs = (struct callbak_args*) malloc(sizeof(struct callbak_args));
    
    callbackArgs->env = env;
    callbackArgs->callback = callback;
    callbackArgs->methodId = methodId;
    callbackArgs->args = args;
    
    uv_timer_t timer_req;
    uv_timer_init(loop, &timer_req);
    timer_req.data = (void*)callbackArgs;
    uv_timer_start(&timer_req, OnTimeout, 10, 1);
    
    uv_run(loop, UV_RUN_DEFAULT);
  }
void OnTimeout(uv_timer_t* handle) {
    
    callbak_args * callbackArgs = (struct callbak_args*) handle->data;
    callbackArgs->env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId,callbackArgs->args);
}




