//
//  Env.cpp
//  io.java
//
//  Created by kenvi.zhu on 15/10/14.
//
//

#include "Env.h"

using namespace io_java;



Env::Env(JavaVM* vm, JNIEnv* env, uv_loop_t* loop)
    :
    event_loop_(loop),
    vm_(vm),
    jnienv_(env){
    
}

uv_loop_t* Env::getEventLoop(){
    return event_loop_;
}

Env* Env::env_;

Env* Env::getOrCreate(){
    
    if(env_ == NULL) {
        JavaVM *jvm;       /* denotes a Java VM */
        JNIEnv *env;       /* pointer to native method interface */
        JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
        JavaVMOption* options = new JavaVMOption[1];
        options[0].optionString = "-Djava.class.path=/Users/kenvi/code/study/io.java/test";
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
       
        uv_loop_t *loop = uv_default_loop();
        
        env_ = new Env(jvm,env,loop);
        
    }
    return env_;
    
}

JavaVM* Env::getVM(){
    return vm_;
}

JNIEnv* Env::getJNIEnv() {
    return jnienv_;
}

void Env::dispose(){
    vm_->DestroyJavaVM();
}

jobjectArray createArgArray(JNIEnv *env, int size) {
    jclass classObject = env->FindClass("java/lang/Object");
    jobjectArray argArray = env->NewObjectArray(size, classObject, NULL);
    return argArray;
}

jobject createIntObject(JNIEnv *env, int i) {
    jclass classInteger = env->FindClass("java/lang/Integer");
    jmethodID intConstructId = env->GetMethodID(classInteger, "<init>", "(I)V");
    jobject intObj = env->NewObject(classInteger, intConstructId, i);
    return intObj;
}

struct callbak_args* getCallbackArgs(JNIEnv *env, jobject callback) {
    jclass cls = env->FindClass("io/java/lang/Callback");
    
    if (env->ExceptionOccurred()) {
        printf("exception 1");
        env->ExceptionDescribe();
        return NULL;
    }
    jmethodID methodId = env->GetMethodID(cls, "apply", "([Ljava/lang/Object;)V");
    if (env->ExceptionOccurred()) {
        printf("exception 2");
        env->ExceptionDescribe();
        return NULL;
    }
    struct callbak_args * callbackArgs = (struct callbak_args*) malloc(sizeof(struct callbak_args));
    callbackArgs->env = env;
    callbackArgs->methodId = methodId;
    callbackArgs->callback = callback;
    return callbackArgs;
    
}




    