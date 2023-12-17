#include "com_bookstore_service_BookStoreIntegration.h"
#include "gbooks.h"
#include "volumeitem.h"

#include <iostream>

std::string jstring2string(JNIEnv *env, jstring jstr) {
    if (!jstr) {
        return "";
    }

    jboolean isCopy = false;

    const char *inCStr = env->GetStringUTFChars(jstr, &isCopy);
    if (NULL == inCStr) return std::string("");

    return std::string(inCStr);
}

jstring get_string(JNIEnv *env, jclass clazz, jobject object, const std::string& method_name) {
    const jmethodID get_attribute = env->GetMethodID(clazz, method_name.c_str(), "()Ljava/lang/String;");
    if(get_attribute == nullptr) {
        std::cout << "error - The get string attribute is null.\n";
        return nullptr;
    }
    const jstring valueJString = (jstring) env->CallObjectMethod(object, get_attribute, env->NewStringUTF("UTF-8"));
    if(valueJString == nullptr) {
        std::cout << "error - The value jstring is null.\n";
        return nullptr;
    }

    return valueJString;
}

jboolean get_boolean(JNIEnv *env, jclass clazz, jobject object, const std::string& method_name) {
    const jmethodID get_attribute = env->GetMethodID(clazz, method_name.c_str(), "()Ljava/lang/Boolean;");
    if(get_attribute == nullptr) {
        std::cout << "error - The get boolean attribute is null.\n";
        return false;
    }
    return (jboolean) env->CallBooleanMethod(object, get_attribute);
}

VolumeItem* jobject2volumeitem(JNIEnv *env, jobject volumeItemDTO) {
    std::cout << "Calling jobject2volumeitem(JNIEnv *env, jobject volumeItemDTO)\n";

    if (!volumeItemDTO) {
        return nullptr;
    }

    VolumeItem* vi = new VolumeItem();

    std::cout << "jobject2volumeitem: Getting class object...\n";
    const jclass volumeItemDTOClass = env->GetObjectClass(volumeItemDTO);
    if(volumeItemDTOClass == nullptr) {
        std::cout << "jobject2volumeitem: The object volumeItemDTOClass is null.\n";
        return nullptr;
    }
    
    const jstring idJString = get_string(env, volumeItemDTOClass, volumeItemDTO, "getId");
    if(idJString == nullptr) {
        std::cout << "jobject2volumeitem: The idJString is null.\n";
        return nullptr;
    }

    const jstring titleJString = get_string(env, volumeItemDTOClass, volumeItemDTO, "getTitle");
    if(idJString == nullptr) {
        std::cout << "jobject2volumeitem: The titleJString is null.\n";
        return nullptr;
    }

    const jstring smallThumbnailJString = get_string(env, volumeItemDTOClass, volumeItemDTO, "getSmallThumbnail");
    const jstring thumbnailJString = get_string(env, volumeItemDTOClass, volumeItemDTO, "getThumbnail");
    const jboolean favoriteJBoolean = get_boolean(env, volumeItemDTOClass, volumeItemDTO, "getFavorite");

    vi->id = jstring2string(env, idJString);
    vi->title = jstring2string(env, titleJString);
    vi->smallThumbnail = jstring2string(env, smallThumbnailJString);
    vi->thumbnail = jstring2string(env, thumbnailJString);
    vi->favorite = favoriteJBoolean;

    return vi;
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

JNIEXPORT jlong JNICALL Java_com_bookstore_service_BookStoreIntegration_startFavoriteRepository(JNIEnv *env, jobject jobj, jstring jsFilePath) {
    std::string filePath = jstring2string(env, jsFilePath);

    long handler = reinterpret_cast<long>(new FavoriteRepository(filePath));

    std::cout << "Java_com_bookstore_service_BookStoreIntegration_startFavoriteRepository with handler " << handler << " in the C++\n";

    return reinterpret_cast<jlong>(handler);
}

JNIEXPORT void JNICALL Java_com_bookstore_service_BookStoreIntegration_closeFavoriteRepository(JNIEnv *env, jobject jobj, jlong favoriteHandle) {
    FavoriteRepository* favoriteRepository = reinterpret_cast<FavoriteRepository*>(favoriteHandle);

    delete favoriteRepository;
}

JNIEXPORT void JNICALL Java_com_bookstore_service_BookStoreIntegration_updateFavorite(JNIEnv *env, jobject jobj, jlong favoriteHandle, jobject volumeItemDTO) {
    std::cout << "Java_com_bookstore_service_BookStoreIntegration_updateFavorite in the C++ recovering handler " << favoriteHandle << "...\n";

    FavoriteRepository* favoriteRepository = reinterpret_cast<FavoriteRepository*>(favoriteHandle);

    VolumeItem* volumeItem = jobject2volumeitem(env, volumeItemDTO);

    if(volumeItem != nullptr) {
        favoriteRepository->updateFavorite(volumeItem);
    }
}

JNIEXPORT jboolean JNICALL Java_com_bookstore_service_BookStoreIntegration_isFavorite(JNIEnv *env, jobject jobj, jlong favoriteHandle, jstring jsid) {
    std::cout << "Java_com_bookstore_service_BookStoreIntegration_isFavorite in the C++ recovering handler " << favoriteHandle << "...\n";

    if(jsid == nullptr) {
        return false;
    }

    FavoriteRepository* favoriteRepository = reinterpret_cast<FavoriteRepository*>(favoriteHandle);

    std::string id = jstring2string(env, jsid);

    return favoriteRepository->isFavorite(id);
}
