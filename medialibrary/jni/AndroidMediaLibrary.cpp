#include "AndroidMediaLibrary.h"
#define LOG_TAG "VLC/JNI/AndroidMediaLibrary"
#include "log.h"

AndroidMediaLibrary::AndroidMediaLibrary(JavaVM *vm, fields *ref_fields)
    : p_ml( NewMediaLibrary() ), myVm ( vm ), p_fields (ref_fields)
{
    p_lister = std::make_shared<AndroidDeviceLister>();
//    p_ml->setLogger( new AndroidMediaLibraryLogger );
//    p_ml->setVerbosity(medialibrary::LogLevel::Info);
    p_ml->setDeviceLister(p_lister);
}

AndroidMediaLibrary::~AndroidMediaLibrary()
{
    LOGD("AndroidMediaLibrary delete");
    delete p_ml;
}

void
AndroidMediaLibrary::initDevices(const std::string& appDirPath, const std::string& libpath)
{
    p_lister->addDevice(libpath, libpath, false);
    p_ml->initialize(appDirPath + "/vlc_media.db", appDirPath + "/thumbs", this);
}

void
AndroidMediaLibrary::discover(const std::string& libraryPath)
{
    p_ml->discover(libraryPath);
}

std::vector<medialibrary::MediaPtr>
AndroidMediaLibrary::videoFiles( medialibrary::SortingCriteria sort, bool desc )
{
    try {
    return p_ml->videoFiles(sort, desc);
    } catch (const std::exception& e) {
        LOGE("fail %s", e.what());
        std::vector<medialibrary::MediaPtr> vector;
        return vector;
    }
}

std::vector<medialibrary::MediaPtr>
AndroidMediaLibrary::audioFiles( medialibrary::SortingCriteria sort, bool desc )
{
    try {
    return p_ml->audioFiles(sort, desc);
    } catch (const std::exception& e) {
        LOGE("fail %s", e.what());
        std::vector<medialibrary::MediaPtr> vector;
        return vector;
    }
}

void
AndroidMediaLibrary::onMediaAdded( std::vector<medialibrary::MediaPtr> mediaList )
{
    JNIEnv *env = getEnv();
    if (env == NULL)
        return;
    jobjectArray mediaRefs = (jobjectArray) env->NewObjectArray(mediaList.size(), p_fields->MediaWrapper.clazz, NULL);
    int index = -1;
    for (medialibrary::MediaPtr const& media : mediaList) {
        jobject item = mediaToMediaWrapper(env, p_fields, media);
        env->SetObjectArrayElement(mediaRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    env->CallStaticVoidMethod(p_fields->MediaLibrary.clazz, p_fields->MediaLibrary.onMediaAddedId, mediaRefs);
    env->DeleteLocalRef(mediaRefs);
}

void AndroidMediaLibrary::onMediaUpdated( std::vector<medialibrary::MediaPtr> mediaList )
{
    JNIEnv *env = getEnv();
    if (env == NULL)
        return;
    jobjectArray mediaRefs = (jobjectArray) env->NewObjectArray(mediaList.size(), p_fields->MediaWrapper.clazz, NULL);
    int index = -1;
    for (medialibrary::MediaPtr const& media : mediaList) {
        jobject item = mediaToMediaWrapper(env, p_fields, media);

        env->SetObjectArrayElement(mediaRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    env->CallStaticVoidMethod(p_fields->MediaLibrary.clazz, p_fields->MediaLibrary.onMediaUpdatedId, mediaRefs);
    env->DeleteLocalRef(mediaRefs);
}

void AndroidMediaLibrary::onMediaDeleted( std::vector<int64_t> ids )
{

}

void AndroidMediaLibrary::onArtistsAdded( std::vector<medialibrary::ArtistPtr> artists )
{

}
void AndroidMediaLibrary::onArtistsModified( std::vector<medialibrary::ArtistPtr> artist )
{

}
void AndroidMediaLibrary::onArtistsDeleted( std::vector<int64_t> ids )
{

}

void AndroidMediaLibrary::onAlbumsAdded( std::vector<medialibrary::AlbumPtr> albums )
{

}
void AndroidMediaLibrary::onAlbumsModified( std::vector<medialibrary::AlbumPtr> albums )
{

}
void AndroidMediaLibrary::onAlbumsDeleted( std::vector<int64_t> ids )
{

}
void AndroidMediaLibrary::onTracksAdded( std::vector<medialibrary::AlbumTrackPtr> tracks )
{

}
void AndroidMediaLibrary::onTracksDeleted( std::vector<int64_t> trackIds )
{

}
void AndroidMediaLibrary::onDiscoveryStarted( const std::string& entryPoint )
{
    JNIEnv *env = getEnv();
    if (env == NULL)
        return;
    jstring ep = env->NewStringUTF(entryPoint.c_str());
    env->CallStaticVoidMethod(p_fields->MediaLibrary.clazz, p_fields->MediaLibrary.onDiscoveryStartedId, ep);
    env->DeleteLocalRef(ep);
}
void AndroidMediaLibrary::onDiscoveryCompleted( const std::string& entryPoint )
{
    JNIEnv *env = getEnv();
    if (env == NULL)
        return;
    jstring ep = env->NewStringUTF(entryPoint.c_str());
    env->CallStaticVoidMethod(p_fields->MediaLibrary.clazz, p_fields->MediaLibrary.onDiscoveryCompletedId, ep);
    env->DeleteLocalRef(ep);
}
void AndroidMediaLibrary::onParsingStatsUpdated( uint32_t percent)
{
    JNIEnv *env = getEnv();
    if (env == NULL)
        return;
    jint progress = percent;
    env->CallStaticVoidMethod(p_fields->MediaLibrary.clazz, p_fields->MediaLibrary.onParsingStatsUpdatedId, progress);
}

JNIEnv *
AndroidMediaLibrary::getEnv() {
    JNIEnv *env = nullptr;
    switch (myVm->GetEnv((void**)(&env), VLC_JNI_VERSION))
    {
    case JNI_OK:
        break;
    case JNI_EDETACHED:
        myVm->AttachCurrentThread(&env, NULL);
        break;
    default:
        LOGE("failed to get env");
    }
    return env;
}

void
AndroidMediaLibrary::DetachCurrentThread() {
    myVm->DetachCurrentThread();
}
