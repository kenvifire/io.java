#include <jni.h>
#include "Env.h"
#include "io_java_lang_System.h"
#include <event2/event.h>
#include <stdlib.h>
using namespace io_java;

void cb_func(evutil_socket_t fd, short what, void *arg);

struct callbak_args {
    JNIEnv *env;
    jobject callback;
    jmethodID methodId;
    jobject args;
    
};

JNIEXPORT jint JNICALL Java_io_java_lang_System_setTimeout
  (JNIEnv * env, jclass clz, jobject callback, jint millions, jobjectArray args){
      Env* jenv = Env::getOrCreate();
      struct event_base* loop = jenv->getEventLoop();
      int sec = millions / 1000;
      int msec = millions % 1000;
      struct timeval one_sec = { sec, msec };
      struct event *ev;
      
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
      
   
      ev = event_new(loop, -1, EV_PERSIST, cb_func,callbackArgs);
      event_add(ev, &one_sec);
     
      event_base_dispatch(loop);
      
  }

void cb_func(evutil_socket_t fd, short what, void *arg)
{
    struct callbak_args* callbackArgs = (struct callbak_args*) arg;
    
    JNIEnv* env = callbackArgs->env;
    env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId,callbackArgs->args);
}

/*
 * Class:     io_java_lang_System
 * Method:    setRepeate
 * Signature: (Lio/java/lang/Callback;I[Ljava/lang/Object;)I
 */
JNIEXPORT jint JNICALL Java_io_java_lang_System_setRepeate
  (JNIEnv * env, jclass clz, jobject obj, jint millions, jobjectArray args){

  }

