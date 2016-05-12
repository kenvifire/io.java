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

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[2014];

static uv_buf_t iov;

void on_open(uv_fs_t *req);
void on_read(uv_fs_t *req);
void on_write(uv_fs_t *req);

JNIEXPORT void JNICALL Java_io_java_io_File_Read
(JNIEnv * env, jobject file, jobject callback, jobjectArray args){
    jclass thisClass = env->GetObjectClass(file);
    jfieldID fileNameId = env->GetFieldID(thisClass, "fileName", "Ljava/lang/String;");
    
    jstring fileName = (jstring)env->GetObjectField(file, fileNameId);
    const char *nativeFileStr = env->GetStringUTFChars(fileName, NULL);
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
   
    uv_fs_open(uv_default_loop(), &open_req, nativeFileStr , O_RDONLY, 0, on_open);
  
    
}
void on_open(uv_fs_t *req) {
    if (req->result >= 0) {
        iov = uv_buf_init(buffer, sizeof(buffer));
        uv_fs_read(uv_default_loop(), &read_req, req->result, &iov, 1, -1, on_read);
    }else {
        fprintf(stderr, "error opening file : %s\n", uv_strerror((int)req->result));
    }
    
}

void on_read(uv_fs_t *req) {
    if(req->result < 0) {
        fprintf(stderr, "Read error: %s\n",uv_strerror(req->result));
    }
    else if (req->result == 0) {
        uv_fs_t close_req;
        uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
    }else if(req->result > 0) {
        iov.len = req->result;
        uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
        
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