/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class me_rjdj_zengarden_ZGGraph */

#ifndef _Included_me_rjdj_zengarden_ZGGraph
#define _Included_me_rjdj_zengarden_ZGGraph
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     me_rjdj_zengarden_ZGGraph
 * Method:    attach
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_me_rjdj_zengarden_ZGGraph_attach
  (JNIEnv *, jobject, jlong);

/*
 * Class:     me_rjdj_zengarden_ZGGraph
 * Method:    unattach
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_me_rjdj_zengarden_ZGGraph_unattach
  (JNIEnv *, jobject, jlong);

/*
 * Class:     me_rjdj_zengarden_ZGGraph
 * Method:    isAttached
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_me_rjdj_zengarden_ZGGraph_isAttached
  (JNIEnv *, jobject, jlong);

/*
 * Class:     me_rjdj_zengarden_ZGGraph
 * Method:    addObject
 * Signature: (Ljava/lang/String;IIJ)J
 */
JNIEXPORT jlong JNICALL Java_me_rjdj_zengarden_ZGGraph_addObject
  (JNIEnv *, jobject, jstring, jint, jint, jlong);

/*
 * Class:     me_rjdj_zengarden_ZGGraph
 * Method:    addConnection
 * Signature: (JIJIJ)V
 */
JNIEXPORT void JNICALL Java_me_rjdj_zengarden_ZGGraph_addConnection
  (JNIEnv *, jobject, jlong, jint, jlong, jint, jlong);

/*
 * Class:     me_rjdj_zengarden_ZGGraph
 * Method:    removeConnection
 * Signature: (JIJIJ)V
 */
JNIEXPORT void JNICALL Java_me_rjdj_zengarden_ZGGraph_removeConnection
  (JNIEnv *, jobject, jlong, jint, jlong, jint, jlong);

#ifdef __cplusplus
}
#endif
#endif
