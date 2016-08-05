#include "AndroidMediaLibrary.h"
#define LOG_TAG "VLC/JNI/AndroidMediaLibrary"
#include "log.h"

AndroidMediaLibrary::AndroidMediaLibrary()
    : p_ml( NewMediaLibrary() )
{
    p_lister = std::make_shared<AndroidDeviceLister>();
//    p_ml->setLogger( new AndroidMediaLibraryLogger );
//    p_ml->setVerbosity(medialibrary::LogLevel::Info);
    p_ml->setDeviceLister(p_lister);
}

AndroidMediaLibrary::~AndroidMediaLibrary()
{
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
//    JNIEnv *env;
//    myVm->GetEnv((void**) &env, VLC_JNI_VERSION);
//    jobjectArray mediaRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(mediaList.size(), m_fields.MediaWrapper.clazz, NULL));
//    int index = -1;
//    jobject item = nullptr;
    for(medialibrary::MediaPtr const& media : mediaList) {
        LOGD("onMediaAdded %s", media->title().c_str());
//        item = mediaToMediaWrapper(env, &m_fields, media);
//        env->SetObjectArrayElement(mediaRefs, ++index, item);
//        env->DeleteLocalRef(item);
    }
//    env->CallStaticVoidMethod(m_fields.MediaLibrary.clazz, m_fields.MediaLibrary.onMediaAddedId, mediaRefs);
//    env->DeleteLocalRef(mediaRefs);
}

void AndroidMediaLibrary::onMediaUpdated( std::vector<medialibrary::MediaPtr> mediaList )
{
//    JNIEnv *env;
//    myVm->GetEnv((void**) &env, VLC_JNI_VERSION);
//    jobjectArray mediaRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(mediaList.size(), m_fields.MediaWrapper.clazz, NULL));
//    int index = -1;
//    jobject item = nullptr;
    for(medialibrary::MediaPtr const& media : mediaList) {
        LOGD("onMediaUpdated %s", media->title().c_str());
//        item = mediaToMediaWrapper(env, &m_fields, media);
//        env->SetObjectArrayElement(mediaRefs, ++index, item);
//        env->DeleteLocalRef(item);
    }
//    env->CallStaticVoidMethod(m_fields.MediaLibrary.clazz, m_fields.MediaLibrary.onMediaUpdatedId, mediaRefs);
//    env->DeleteLocalRef(mediaRefs);
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

}
void AndroidMediaLibrary::onDiscoveryCompleted( const std::string& entryPoint )
{

}
void AndroidMediaLibrary::onParsingStatsUpdated( uint32_t percent)
{

}
