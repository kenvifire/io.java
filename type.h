//
//  Type.h
//  io.java
//
//  Created by kenvi.zhu on 15/10/28.
//
//

#ifndef Type_h
#define Type_h

struct callbak_args {
    JNIEnv *env;
    jobject callback;
    jmethodID methodId;
    jobject args;
    
};

#endif /* Type_h */
