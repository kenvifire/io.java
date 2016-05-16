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
    int fd;
    int offset;
    int length;
    int position;
};

void on_open(uv_fs_t *req);
void on_read(uv_fs_t *req);
void on_write(uv_fs_t *req);





JNIEXPORT void JNICALL Java_io_java_io_File_Open
(JNIEnv *env, jclass clazz, jstring path, jstring flags, jobject mode, jobject callback, jobjectArray args) {

    struct callbak_args* callbackArgs = getCallbackArgs(env, callback);
    
    const char *nativeFileStr = env->GetStringUTFChars(path, NULL);
    
    assert(callbackArgs != NULL);
    open_req.data = (void*)callbackArgs;
    uv_fs_open(uv_default_loop(), &open_req, nativeFileStr , O_WRONLY, 0, on_open);
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
(JNIEnv *env, jclass clazz, jint fd, jobject callback){

    struct callbak_args* callbackArgs = getCallbackArgs(env, callback);
    assert(callbackArgs != NULL);
    
    struct read_arg* readArgs = (struct read_arg*)malloc(sizeof(struct read_arg));
    readArgs->fd = fd;
    callbackArgs->args = (void*) readArgs;
    
    read_req.data = (void*)callbackArgs;
    
    iov = uv_buf_init(buffer, sizeof(buffer));
    uv_fs_read(uv_default_loop(), &read_req, fd, &iov, 1, -1, on_read);
    
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    
    
}

/*
 * Class:     io_java_io_File
 * Method:    Write
 * Signature: (I[BLio/java/lang/Callback;)V
 */
JNIEXPORT void JNICALL Java_io_java_io_File_Write
(JNIEnv *env, jclass clazz, jint fd, jbyteArray jdata, jobject callback){
    struct callbak_args* callbackArgs = getCallbackArgs(env, callback);
    assert(callbackArgs != NULL);
    
    iov.len = env->GetArrayLength(jdata);
    jbyte* data = env->GetByteArrayElements(jdata, NULL);
    iov.base = (char*)data;
    write_req.data = (void*) callbackArgs;
    
    printf("open file: %d\n", fd);
    uv_fs_write(uv_default_loop(), &write_req, fd, &iov, 1, -1, on_write);
    
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);

    
}

void on_read(uv_fs_t *req) {
    
    struct callbak_args * callbackArgs = (struct callbak_args*) req->data;
    JNIEnv * env = callbackArgs->env;
    struct read_arg* readArg = (struct read_arg*) callbackArgs->args;
    int fd = readArg->fd;
    
    jobjectArray argArray = createArgArray(env, 2);
    
    if (req->result < 0) {
        
        jstring error = env->NewStringUTF(uv_strerror((int)req->result));
        env->SetObjectArrayElement(argArray, 0, error);
        
    }else if (req->result == 0) {
        
        uv_fs_t close_req;
        uv_fs_close(uv_default_loop(), &close_req, fd , NULL);
        jstring error = env->NewStringUTF("file already closed.");
        env->SetObjectArrayElement(argArray, 0, error);
        
    }else {
        
        iov.len = req->result;
        jbyteArray data = env->NewByteArray(iov.len);
        env->SetByteArrayRegion(data, 0, iov.len, (jbyte *)buffer);
        env->SetObjectArrayElement(argArray, 1, data);
        
    }
    callbackArgs->env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId, argArray);
  

}

void on_write(uv_fs_t *req) {
    printf("on write func");
    
    struct callbak_args * callbackArgs = (struct callbak_args*) req->data;
    JNIEnv * env = callbackArgs->env;

    jobjectArray argArray = createArgArray(env, 2);
    
    if (req->result < 0) {
        printf(" write< 0");

        jstring error = env->NewStringUTF(uv_strerror((int)req->result));
        env->SetObjectArrayElement(argArray, 0, error);
    }else {
        printf(" write> 0:%ld", req->result);
    }
    callbackArgs->env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId, argArray);
}
/*
void cb_func_file(evutil_socket_t fd, short what, void *arg)
{
    struct callbak_args* callbackArgs = (struct callbak_args*) arg;
    
    JNIEnv* env = callbackArgs->env;
    env->CallVoidMethod(callbackArgs->callback, callbackArgs->methodId);
}
*/