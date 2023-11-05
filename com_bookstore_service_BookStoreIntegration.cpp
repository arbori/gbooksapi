#include "com_bookstore_service_BookStoreIntegration.h"
#include "gbooks.h"

std::string jstring2string(JNIEnv *env, jstring jstr) {
    if (!jstr) {
        return "";
    }

    const jclass stringClass = env->GetObjectClass(jstr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray) env->CallObjectMethod(jstr, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t) env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char *)pBytes, length);

    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);

    return ret;
}

std::string search_callback(std::string content, int code) {
    if(code == 0) {
        return content;
    }
    else {
        return gbooks_error_message(code);
    }
} 

JNIEXPORT jstring JNICALL Java_com_bookstore_service_BookStoreIntegration_search(JNIEnv *env, jobject jobj, jstring jquery, jint startIndex, jint maxResults) {
    std::string query = jstring2string(env, jquery);

    std::string content = gbooks_search(query, startIndex, maxResults, search_callback);

    return env->NewStringUTF(content.c_str());
}
