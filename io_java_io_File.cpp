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
#include <assert.h>

using namespace io_java;

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[2014];

static uv_buf_t iov;

struct read_arg{
    jbyteArray buffer;
    int offset;
    int length;
    int position;
};

void on_open(uv_fs_t *req);
void on_read(uv_fs_t *req);
void on_write(uv_fs_t *req);

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



JNIEXPORT void JNICALL Java_io_java_io_File_Open
(JNIEnv *env, jclass clazz, jstring path, jstring flags, jobject mode, jobject callback, jobjectArray args) {

    struct callbak_args* callbackArgs = getCallbackArgs(env, callback);
    
    const char *nativeFileStr = env->GetStringUTFChars(path, NULL);
    
    assert(callbackArgs != NULL);
    open_req.data = (void*)callbackArgs;
    uv_fs_open(uv_default_loop(), &open_req, nativeFileStr , O_RDONLY, 0, on_open);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  
    
}
void on_open(uv_fs_t *req) {
    
    callbak_args * callbackArgs = (struct callbak_args*) req->data;
    JNIEnv * env = callbackArgs->env;
    
    jobjectArray argArray = createArgArray(env, 2);
    if (req->result >= 0) {

        env->SetObjectArrayElement(argArray, 0, NULL);
        jobject intObj = createIntObject(env, req->result);
        env->SetObjectArrayElement(argArray, 1, intObj);
        
    }else {
        
        jstring error = env->NewStringUTF( uv_strerror((int)req->result));
        env->SetObjectArrayElement(argArray, 0, error);

    }
    
     callbackArgs->env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId, argArray);
}

JNIEXPORT void JNICALL Java_io_java_io_File_Read
(JNIEnv *env, jclass clazz, jint fd, jbyteArray jbuffer, jint offset, jint length, jint position, jobject callback) {
    
    struct callbak_args* callbackArgs = getCallbackArgs(env, callback);
    assert(callbackArgs != NULL);
    read_req.data = (void*)callbackArgs;
    struct read_arg* readArg = (struct read_arg*)malloc(sizeof(struct read_arg));
    readArg->buffer = jbuffer;
    readArg->length = length;
    readArg->offset = offset;
    readArg->position = position;
    callbackArgs->args = (void*) readArg;
    
    iov = uv_buf_init(buffer, sizeof(buffer));
    uv_fs_read(uv_default_loop(), &read_req, fd, &iov, 1, 0, on_read);
    
    
}

void on_read(uv_fs_t *req) {
    printf("on read\n");
    
    callbak_args * callbackArgs = (struct callbak_args*) req->data;
    JNIEnv * env = callbackArgs->env;
    struct read_arg* readArg =(struct read_arg*)callbackArgs->args;
    jobjectArray argArray = createArgArray(env, 2);
    
    if(req->result < 0) {
        jstring error = env->NewStringUTF(uv_strerror((int)req->result));
        env->SetObjectArrayElement(argArray, 0, error);
    }
    else if (req->result == 0) {
        uv_fs_t close_req;
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
        env->SetObjectArrayElement(argArray, 0, env->NewStringUTF("file already closed."));
    }else if(req->result > 0) {
        printf("result>0,%ld", req->result);
        iov.len = req->result;
        env->SetByteArrayRegion(readArg->buffer, readArg->offset, readArg->length, (const signed char*)buffer);
        
    }
}

void on_write(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Write error : %s\n",uv_strerror((int)req->result));
    }else {
        uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
    }
}
/*
void cb_func_file(evutil_socket_t fd, short what, void *arg)
{
    struct callbak_args* callbackArgs = (struct callbak_args*) arg;
    
    JNIEnv* env = callbackArgs->env;
    env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId,callbackArgs->args);
}
*/