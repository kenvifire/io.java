//
//  Env.hpp
//  io.java
//
//  Created by kenvi.zhu on 15/10/14.
//
//

#ifndef Env_h
#define Env_h

#include <stdio.h>
#include <string>
#include <jni.h>
#include "uv.h"
#include <stdlib.h>
#include "type.h"

using namespace std;

// create callback args
struct callbak_args* getCallbackArgs(JNIEnv *env, jobject callback);

// create a java.lang.Integer object
jobject createIntObject(JNIEnv *env, int i);

// create a java array
jobjectArray createArgArray(JNIEnv *env, int size);



namespace io_java {
    class Env{
    private:
        uv_loop_t* event_loop_;
        JavaVM* vm_;
        JNIEnv* jnienv_;
        static Env* env_;
        
        
        
    public:
        Env(JavaVM* vm, JNIEnv* env, uv_loop_t* loop);
        uv_loop_t* getEventLoop();
        static Env* getOrCreate();
        JavaVM* getVM();
        JNIEnv* getJNIEnv();
        
        void dispose();
        
        
        
    };
    
    
   
    
}

#endif /* Env_hpp */
