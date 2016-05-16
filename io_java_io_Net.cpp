//
//  io_java_io_Net.cpp
//  io.java
//
//  Created by kenvi.zhu on 16/5/16.
//
//

#include <jni.h>
#include <stdio.h>
#include "io_java_io_Net.h"
#include <fcntl.h>
#include "uv.h"
#include "type.h"
#include <stdlib.h>
#include "Env.h"
#include <assert.h>

using namespace io_java;


void on_connect(uv_connect_t* req, int status);

JNIEXPORT void JNICALL Java_io_java_io_Net_connect
(JNIEnv * env, jclass clazz, jstring addr, jint port, jobject callback) {
    printf("connect");
    struct callbak_args* callbackArg =  getCallbackArgs(env, callback);
    uv_tcp_t* socket = (uv_tcp_t*) malloc(sizeof(uv_tcp_t));
    uv_tcp_init(uv_default_loop(), socket);
    
    uv_connect_t* connect = (uv_connect_t*)malloc(sizeof(uv_connect_t));
    
    struct sockaddr_in dest;
    const char* addrStr = env->GetStringUTFChars(addr, NULL);
    uv_ip4_addr(addrStr, port, &dest);
    
    printf("before connect");
    uv_tcp_connect(connect, socket, (const struct sockaddr*)&dest, on_connect);
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    printf("after connet");
    
    
    
    
    
}

void on_connect(uv_connect_t* req, int status) {
    printf("on_connect");
    if (status < 0) {
        printf("connect error: %s", uv_err_name(status));
    }else {
        printf("connect success");
    }
}



