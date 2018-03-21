/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_breadwallet_core_ethereum_BREthereumLightNode */

#ifndef _Included_com_breadwallet_core_ethereum_BREthereumLightNode
#define _Included_com_breadwallet_core_ethereum_BREthereumLightNode
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniCreateEthereumLightNodeLES
 * Signature: (Lcom/breadwallet/core/ethereum/BREthereumLightNode/Client;J)J
 */
JNIEXPORT jlong JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniCreateEthereumLightNodeLES
  (JNIEnv *, jclass, jobject, jlong);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniCreateEthereumLightNodeJSON_RPC
 * Signature: (Lcom/breadwallet/core/ethereum/BREthereumLightNode/Client;J)J
 */
JNIEXPORT jlong JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniCreateEthereumLightNodeJSON_1RPC
  (JNIEnv *, jclass, jobject, jlong);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniCreateEthereumLightNodeAccount
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniCreateEthereumLightNodeAccount
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniCreateEthereumLightNodeWallet
 * Signature: (JJ)J
 */
JNIEXPORT jlong JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniCreateEthereumLightNodeWallet
  (JNIEnv *, jobject, jlong, jlong);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniGetAccountPrimaryAddress
 * Signature: (J)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniGetAccountPrimaryAddress
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniForceWalletBalanceUpdate
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniForceWalletBalanceUpdate
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniForceWalletTransactionUpdate
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniForceWalletTransactionUpdate
  (JNIEnv *, jobject, jlong);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    jniAnnounceTransaction
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_jniAnnounceTransaction
  (JNIEnv *, jobject, jstring, jstring, jstring, jstring, jstring, jstring, jstring, jstring);

/*
 * Class:     com_breadwallet_core_ethereum_BREthereumLightNode
 * Method:    initializeNative
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_breadwallet_core_ethereum_BREthereumLightNode_initializeNative
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif