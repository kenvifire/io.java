/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_java_io_File */

#ifndef _Included_io_java_io_File
#define _Included_io_java_io_File
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     io_java_io_File
 * Method:    Open
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/Integer;Lio/java/lang/Callback;[Ljava/lang/Object;)V
 */
JNIEXPORT void JNICALL Java_io_java_io_File_Open
  (JNIEnv *, jclass, jstring, jstring, jobject, jobject, jobjectArray);

/*
 * Class:     io_java_io_File
 * Method:    Read
 * Signature: (ILio/java/lang/Callback;)V
 */
JNIEXPORT void JNICALL Java_io_java_io_File_Read
  (JNIEnv *, jclass, jint, jobject);

/*
 * Class:     io_java_io_File
 * Method:    Write
 * Signature: (I[BLio/java/lang/Callback;)V
 */
JNIEXPORT void JNICALL Java_io_java_io_File_Write
  (JNIEnv *, jclass, jint, jbyteArray, jobject);

#ifdef __cplusplus
}
#endif
#endif