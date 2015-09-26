//
//  io_java.h
//  io.java
//
//  Created by kenvi.zhu on 15/9/26.
//
//

#ifndef io_java_h
#define io_java_h

#include <stdio.h>
#include <jni.h>

/*
 * Pointers to the needed JNI invocation API, initialized by LoadJavaVM.
 */
typedef jint (JNICALL *CreateJavaVM_t)(JavaVM **pvm, void **env, void *args);
typedef jint (JNICALL *GetDefaultJavaVMInitArgs_t)(void *args);
typedef jint (JNICALL *GetCreatedJavaVMs_t)(JavaVM **vmBuf, jsize bufLen, jsize *nVMs);

typedef struct {
    CreateJavaVM_t CreateJavaVM;
    GetDefaultJavaVMInitArgs_t GetDefaultJavaVMInitArgs;
    GetCreatedJavaVMs_t GetCreatedJavaVMs;
} InvocationFunctions;

#endif /* io_java_h */
