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

VolumeItem* jobject2volumeitem(JNIEnv *env, jobject volumeItemDTO) {
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

    std::cout << "jobject2volumeitem: Getting attribute object for FavoriteItemDTO.id...\n";
    const jfieldID idField = env->GetFieldID(volumeItemDTOClass, "id", "()Ljava/lang/String;");
    if(idField == nullptr) {
        std::cout << "jobject2volumeitem: The field object FavoriteItemDTO.id is null.\n";
        return nullptr;
    }
    std::cout << "jobject2volumeitem: Getting the value for FavoriteItemDTO.id...\n";
    const jstring idJString = (jstring) env->GetObjectField(volumeItemDTO, idField);
    if(idJString == nullptr) {
        std::cout << "jobject2volumeitem: The idJString is null.\n";
        return nullptr;
    }
    std::cout << "jobject2volumeitem: FavoriteItemDTO.id = " << jstring2string(env, idJString) << ".\n";

    std::cout << "jobject2volumeitem: Getting attribute object for FavoriteItemDTO.title...\n";
    const jfieldID titleField = env->GetFieldID(volumeItemDTOClass, "title", "()Ljava/lang/String;");
    if(titleField == nullptr) {
        std::cout << "jobject2volumeitem: The field object FavoriteItemDTO.title is null.\n";
        return nullptr;
    }
    const jstring titleJString = (jstring) env->GetObjectField(volumeItemDTO, titleField);
    if(titleJString == nullptr) {
        std::cout << "jobject2volumeitem: The titleJString is null.\n";
        return nullptr;
    }

    std::cout << "jobject2volumeitem: Getting attribute object for FavoriteItemDTO.smallThumbnail...\n";
    const jfieldID smallThumbnailField = env->GetFieldID(volumeItemDTOClass, "smallThumbnail", "()Ljava/lang/String;");
    if(smallThumbnailField == nullptr) {
        std::cout << "jobject2volumeitem: The field object FavoriteItemDTO.smallThumbnail is null.\n";
        return nullptr;
    }
    const jstring smallThumbnailJString = (jstring) env->GetObjectField(volumeItemDTO, smallThumbnailField);
    if(smallThumbnailJString == nullptr) {
        std::cout << "jobject2volumeitem: The smallThumbnailJString is null.\n";
        return nullptr;
    }

    std::cout << "jobject2volumeitem: Getting attribute object for FavoriteItemDTO.thumbnail...\n";
    const jfieldID thumbnailField = env->GetFieldID(volumeItemDTOClass, "thumbnail", "()Ljava/lang/String;");
    if(thumbnailField == nullptr) {
        std::cout << "jobject2volumeitem: The field object FavoriteItemDTO.thumbnail is null.\n";
        return nullptr;
    }
    const jstring thumbnailJString = (jstring) env->GetObjectField(volumeItemDTO, thumbnailField);
    if(thumbnailJString == nullptr) {
        std::cout << "jobject2volumeitem: The thumbnailJString is null.\n";
        return nullptr;
    }

    std::cout << "jobject2volumeitem: Getting attribute object for FavoriteItemDTO.favorite...\n";
    const jfieldID favoriteField = env->GetFieldID(volumeItemDTOClass, "favorite", "()Ljava/lang/Boolean;");
    if(favoriteField == nullptr) {
        std::cout << "jobject2volumeitem: The field object FavoriteItemDTO.favorite is null.\n";
        return nullptr;
    }
    const jboolean favoriteJBoolean = env->GetBooleanField(volumeItemDTO, favoriteField);

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
