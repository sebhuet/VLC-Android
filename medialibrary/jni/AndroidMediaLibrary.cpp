#include "AndroidMediaLibrary.h"
#define LOG_TAG "VLC/JNI/AndroidMediaLibrary"
#include "log.h"

#define FLAG_MEDIA_UPDATED_AUDIO 1 << 0
#define FLAG_MEDIA_UPDATED_VIDEO 1 << 1
#define FLAG_MEDIA_ADDED_AUDIO   1 << 2
#define FLAG_MEDIA_ADDED_VIDEO   1 << 3


std::string mainStorage = "";
bool discoveryEnded = false;
uint32_t m_nbDiscovery = 0, m_progress = 0, m_mediaAddedType = 0, m_mediaUpdatedType = 0;

AndroidMediaLibrary::AndroidMediaLibrary(JavaVM *vm, fields *ref_fields, jobject thiz)
    : p_ml( NewMediaLibrary() ), myVm ( vm ), p_fields (ref_fields)
{
    p_lister = std::make_shared<AndroidDeviceLister>();
//    p_ml->setLogger( new AndroidMediaLibraryLogger );
//    p_ml->setVerbosity(medialibrary::LogLevel::Info);
    p_ml->setDeviceLister(p_lister);
    JNIEnv *env = getEnv();
    if (env == NULL)
        return;
    weak_thiz = env->NewWeakGlobalRef(thiz);
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
AndroidMediaLibrary::banFolder(const std::string& path)
{
    p_ml->banFolder(path);
}

void
AndroidMediaLibrary::discover(const std::string& libraryPath)
{
    p_ml->discover(libraryPath);
}

bool
AndroidMediaLibrary::isWorking()
{
    return m_nbDiscovery > 0 && m_progress < 100;
}

void
AndroidMediaLibrary::pauseBackgroundOperations()
{
    p_ml->pauseBackgroundOperations();
}

void
AndroidMediaLibrary::setMediaUpdatedCbFlag(int flags)
{
    m_mediaUpdatedType = flags;
}

void
AndroidMediaLibrary::setMediaAddedCbFlag(int flags)
{
    m_mediaAddedType = flags;
}

void
AndroidMediaLibrary::resumeBackgroundOperations()
{
    p_ml->resumeBackgroundOperations();
}

void
AndroidMediaLibrary::reload()
{
    p_ml->reload();
}

void
AndroidMediaLibrary::reload( const std::string& entryPoint )
{
    p_ml->reload(entryPoint);
}

bool
AndroidMediaLibrary::increasePlayCount(int64_t mediaId)
{
    return p_ml->media(mediaId)->increasePlayCount();
}

std::vector<medialibrary::MediaPtr>
AndroidMediaLibrary::lastMediaPlayed() {
    try {
    return p_ml->lastMediaPlayed();
    } catch (const std::exception& e) {
        LOGE("fail %s", e.what());
        std::vector<medialibrary::MediaPtr> vector;
        return vector;
    }
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
    if (m_mediaAddedType & FLAG_MEDIA_ADDED_AUDIO || m_mediaAddedType & FLAG_MEDIA_ADDED_VIDEO) {
        JNIEnv *env = getEnv();
        if (env == NULL /*|| env->IsSameObject(weak_thiz, NULL)*/)
            return;
        jobjectArray mediaRefs = (jobjectArray) env->NewObjectArray(mediaList.size(), p_fields->MediaWrapper.clazz, NULL);
        int index = -1;
        for (medialibrary::MediaPtr const& media : mediaList) {
            medialibrary::IMedia::Type type = media->type();
            if (!(type == medialibrary::IMedia::Type::AudioType && m_mediaAddedType & FLAG_MEDIA_ADDED_AUDIO ||
                    type == medialibrary::IMedia::Type::VideoType && m_mediaAddedType & FLAG_MEDIA_ADDED_VIDEO))
                continue;
            jobject item = mediaToMediaWrapper(env, p_fields, media);
            env->SetObjectArrayElement(mediaRefs, ++index, item);
            env->DeleteLocalRef(item);
        }
        env->CallVoidMethod(weak_thiz, p_fields->MediaLibrary.onMediaAddedId, mediaRefs);
        env->DeleteLocalRef(mediaRefs);
    }
}

void AndroidMediaLibrary::onMediaUpdated( std::vector<medialibrary::MediaPtr> mediaList )
{
    if (m_mediaUpdatedType & FLAG_MEDIA_UPDATED_AUDIO || m_mediaUpdatedType & FLAG_MEDIA_UPDATED_VIDEO) {
        JNIEnv *env = getEnv();
        if (env == NULL /*|| env->IsSameObject(weak_thiz, NULL)*/)
            return;
        jobjectArray mediaRefs = (jobjectArray) env->NewObjectArray(mediaList.size(), p_fields->MediaWrapper.clazz, NULL);
        int index = -1;
        for (medialibrary::MediaPtr const& media : mediaList) {
            medialibrary::IMedia::Type type = media->type();
            if (!(type == medialibrary::IMedia::Type::AudioType && m_mediaUpdatedType & FLAG_MEDIA_UPDATED_AUDIO ||
                    type == medialibrary::IMedia::Type::VideoType && m_mediaUpdatedType & FLAG_MEDIA_UPDATED_VIDEO))
                continue;
            jobject item = mediaToMediaWrapper(env, p_fields, media);
            env->SetObjectArrayElement(mediaRefs, ++index, item);
            env->DeleteLocalRef(item);
        }
        if (index > -1)
            env->CallVoidMethod(weak_thiz, p_fields->MediaLibrary.onMediaUpdatedId, mediaRefs);
        env->DeleteLocalRef(mediaRefs);
    }
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
    ++m_nbDiscovery;
    JNIEnv *env = getEnv();
    if (env == NULL || env->IsSameObject(weak_thiz, NULL))
        return;
    if (mainStorage.empty()) {
        discoveryEnded = false;
        mainStorage = entryPoint;
    }
    jstring ep = env->NewStringUTF(entryPoint.c_str());
    env->CallVoidMethod(weak_thiz, p_fields->MediaLibrary.onDiscoveryStartedId, ep);
    env->DeleteLocalRef(ep);
}

void AndroidMediaLibrary::onDiscoveryProgress( const std::string& entryPoint )
{
    JNIEnv *env = getEnv();
    if (env == NULL || env->IsSameObject(weak_thiz, NULL))
        return;
    jstring ep = env->NewStringUTF(entryPoint.c_str());
    env->CallVoidMethod(weak_thiz, p_fields->MediaLibrary.onDiscoveryProgressId, ep);
    env->DeleteLocalRef(ep);

}

void AndroidMediaLibrary::onDiscoveryCompleted( const std::string& entryPoint )
{
    --m_nbDiscovery;
    JNIEnv *env = getEnv();
    if (env == NULL || env->IsSameObject(weak_thiz, NULL))
        return;
    if (!entryPoint.compare(mainStorage)) {
        discoveryEnded = true;
        mainStorage.clear();
    }
    jstring ep = env->NewStringUTF(entryPoint.c_str());
    env->CallVoidMethod(weak_thiz, p_fields->MediaLibrary.onDiscoveryCompletedId, ep);
    env->DeleteLocalRef(ep);
}

void AndroidMediaLibrary::onParsingStatsUpdated( uint32_t percent)
{
    m_progress = percent;
    if (!discoveryEnded)
        return;
    JNIEnv *env = getEnv();
    if (env == NULL || env->IsSameObject(weak_thiz, NULL))
        return;
    jint progress = percent;
    env->CallVoidMethod(weak_thiz, p_fields->MediaLibrary.onParsingStatsUpdatedId, progress);
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
