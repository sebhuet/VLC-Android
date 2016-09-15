#include <stdlib.h>
#include <string>
#include <jni.h>
#include <medialibrary/IDeviceLister.h>
#define LOG_TAG "VLC/JNI/MediaLibrary"
#include "log.h"
#include "utils.h"
#include "AndroidMediaLibrary.h"

static JavaVM *myVm;
fields ml_fields;
bool m_IsInitialized = false;

#define CLASSPATHNAME "org/videolan/medialibrary/Medialibrary"

static inline void throw_IllegalStateException(JNIEnv *env, const char *p_error);
static inline void throw_IllegalArgumentException(JNIEnv *env, const char *p_error);
static AndroidMediaLibrary *MediaLibrary_getInstanceInternal(JNIEnv *env, jobject thiz);
AndroidMediaLibrary *MediaLibrary_getInstance(JNIEnv *env, jobject thiz);


static void
MediaLibrary_setInstance(JNIEnv *env, jobject thiz, AndroidMediaLibrary *p_obj);

void
init(JNIEnv* env, jobject thiz, jstring appPath, jstring libraryPath )
{
    const char *path = env->GetStringUTFChars(appPath, JNI_FALSE);
    const char *libPath = env->GetStringUTFChars(libraryPath, JNI_FALSE);
    const std::string& stringPath(path);
    const std::string& stringLibPath(libPath);
    AndroidMediaLibrary *aml = new  AndroidMediaLibrary(myVm, &ml_fields, thiz);
    MediaLibrary_setInstance(env, thiz, aml);
    aml->initDevices(stringPath, stringLibPath);
    env->ReleaseStringUTFChars(appPath, path);
    env->ReleaseStringUTFChars(libraryPath, libPath);
    m_IsInitialized = true;
}

void release(JNIEnv* env, jobject thiz)
{
    LOGD("/!\\ release medialib. /!\\");
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    delete aml;
    MediaLibrary_setInstance(env, thiz, NULL);
}

void
banFolder(JNIEnv* env, jobject thiz, jstring folderPath )
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    const char *path = env->GetStringUTFChars(folderPath, JNI_FALSE);
    const std::string& stringPath(path);
    aml->banFolder(stringPath);
    env->ReleaseStringUTFChars(folderPath, path);
}

void
discover(JNIEnv* env, jobject thiz, jstring mediaPath )
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    const char *path = env->GetStringUTFChars(mediaPath, JNI_FALSE);
    const std::string& stringPath(path);
    aml->discover(stringPath);
    env->ReleaseStringUTFChars(mediaPath, path);
}

jboolean
isWorking(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    return (jboolean) aml->isWorking();
}
void
setMediaUpdatedCbFlag(JNIEnv* env, jobject thiz, jint flags)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    aml->setMediaUpdatedCbFlag(flags);
}

void
setMediaAddedCbFlag(JNIEnv* env, jobject thiz, jint flags)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    aml->setMediaAddedCbFlag(flags);
}

void
pauseBackgroundOperations(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    aml->pauseBackgroundOperations();
}

void
resumeBackgroundOperations(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    aml->resumeBackgroundOperations();
}

void
reload(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    aml->reload();
}

void
reloadEntryPoint(JNIEnv* env, jobject thiz, jstring entryPoint)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    const char *path = env->GetStringUTFChars(entryPoint, JNI_FALSE);
    const std::string& stringPath(path);
    aml->reload(stringPath);
    env->ReleaseStringUTFChars(entryPoint, path);
}

jboolean
increasePlayCount(JNIEnv* env, jobject thiz, jlong id)
{
    return MediaLibrary_getInstance(env, thiz)->increasePlayCount((int64_t)id);
}

jboolean
updateProgress(JNIEnv* env, jobject thiz, jlong id, jlong time)
{
    return MediaLibrary_getInstance(env, thiz)->updateProgress((int64_t)id, (int64_t) time);
}

jobjectArray
lastMediaPLayed(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    std::vector<medialibrary::MediaPtr> mediaPlayed = aml->lastMediaPlayed();
    jobjectArray mediaRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(mediaPlayed.size(), ml_fields.MediaWrapper.clazz, NULL));
    int index = -1;
    for(medialibrary::MediaPtr const& media : mediaPlayed) {
        jobject item = mediaToMediaWrapper(env, &ml_fields, media);
        env->SetObjectArrayElement(mediaRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return mediaRefs;
}

jobjectArray
getVideos(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    std::vector<medialibrary::MediaPtr> videoFiles = aml->videoFiles();
    jobjectArray videoRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(videoFiles.size(), ml_fields.MediaWrapper.clazz, NULL));
    int index = -1;
    for(medialibrary::MediaPtr const& media : videoFiles) {
        jobject item = mediaToMediaWrapper(env, &ml_fields, media);
        env->SetObjectArrayElement(videoRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return videoRefs;
}

jobjectArray
getAudio(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    std::vector<medialibrary::MediaPtr> audioFiles = aml->audioFiles();
    jobjectArray audioRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(audioFiles.size(), ml_fields.MediaWrapper.clazz, NULL));
    int index = -1;
    for(medialibrary::MediaPtr const& media : audioFiles) {
        jobject item = mediaToMediaWrapper(env, &ml_fields, media);
        env->SetObjectArrayElement(audioRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return audioRefs;
}

jint
getVideoCount(JNIEnv* env, jobject thiz) {
    return MediaLibrary_getInstance(env, thiz)->videoFiles().size();
}

jint
getAudioCount(JNIEnv* env, jobject thiz) {
    return MediaLibrary_getInstance(env, thiz)->audioFiles().size();
}

jobjectArray
getAlbums(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    std::vector<medialibrary::AlbumPtr> albums = aml->albums();
    jobjectArray albumRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(albums.size(), ml_fields.Album.clazz, NULL));
    int index = -1;
    for(medialibrary::AlbumPtr const& album : albums) {
        jobject item = convertAlbumObject(env, &ml_fields, album);
        env->SetObjectArrayElement(albumRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return albumRefs;
}

jobjectArray
getArtists(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    std::vector<medialibrary::ArtistPtr> artists = aml->artists();
    jobjectArray artistRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(artists.size(), ml_fields.Artist.clazz, NULL));
    int index = -1;
    for(medialibrary::ArtistPtr const& artist : artists) {
        jobject item = convertArtistObject(env, &ml_fields, artist);
        env->SetObjectArrayElement(artistRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return artistRefs;
}

jobjectArray
getGenres(JNIEnv* env, jobject thiz)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, thiz);
    std::vector<medialibrary::GenrePtr> genres = aml->genres();
    jobjectArray genreRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(genres.size(), ml_fields.Genre.clazz, NULL));
    int index = -1;
    for(medialibrary::GenrePtr const& genre : genres) {
        jobject item = convertGenreOgbject(env, &ml_fields, genre);
        env->SetObjectArrayElement(genreRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return genreRefs;
}

/*
 * Album methods
 */


jobjectArray
getTracksFromAlbum(JNIEnv* env, jobject thiz, jobject medialibrary, jlong id)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, medialibrary);
    std::vector<medialibrary::MediaPtr> tracks = aml->tracksFromAlbum(id);
    jobjectArray mediaRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(tracks.size(), ml_fields.MediaWrapper.clazz, NULL));
    int index = -1;
    jobject item = nullptr;
    for(medialibrary::MediaPtr const& media : tracks) {
        item = mediaToMediaWrapper(env, &ml_fields, media);
        env->SetObjectArrayElement(mediaRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return mediaRefs;
}

/*
 * Artist methods
 */

jobjectArray
getMediaFromArtist(JNIEnv* env, jobject thiz, jobject medialibrary, jlong id)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, medialibrary);
    std::vector<medialibrary::MediaPtr> mediaList = aml->mediaFromArtist(id);
    jobjectArray mediaRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(mediaList.size(), ml_fields.MediaWrapper.clazz, NULL));
    int index = -1;
    for(medialibrary::MediaPtr const& media : mediaList) {
        jobject item = mediaToMediaWrapper(env, &ml_fields, media);
        env->SetObjectArrayElement(mediaRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return mediaRefs;
}

jobjectArray
getAlbumsFromArtist(JNIEnv* env, jobject thiz, jobject medialibrary, jlong id)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, medialibrary);
    std::vector<medialibrary::AlbumPtr> albums = aml->albumsFromArtist(id);
    jobjectArray albumsRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(albums.size(), ml_fields.Album.clazz, NULL));
    int index = -1;
    for(medialibrary::AlbumPtr const& album : albums) {
        jobject item = convertAlbumObject(env, &ml_fields, album);
        env->SetObjectArrayElement(albumsRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return albumsRefs;
}


/*
 * Genre methods
 */

jobjectArray
getMediaFromGenre(JNIEnv* env, jobject thiz, jobject medialibrary, jlong id)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, medialibrary);
    std::vector<medialibrary::MediaPtr> mediaList = aml->mediaFromGenre(id);
    jobjectArray mediaRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(mediaList.size(), ml_fields.MediaWrapper.clazz, NULL));
    int index = -1;
    for(medialibrary::MediaPtr const& media : mediaList) {
        jobject item = mediaToMediaWrapper(env, &ml_fields, media);
        env->SetObjectArrayElement(mediaRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return mediaRefs;
}

jobjectArray
getAlbumsFromGenre(JNIEnv* env, jobject thiz, jobject medialibrary, jlong id)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, medialibrary);
    std::vector<medialibrary::AlbumPtr> albums = aml->albumsFromGenre(id);
    jobjectArray albumRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(albums.size(), ml_fields.Album.clazz, NULL));
    int index = -1;
    for(medialibrary::AlbumPtr const& album : albums) {
        jobject item = convertAlbumObject(env, &ml_fields, album);
        env->SetObjectArrayElement(albumRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return albumRefs;
}

jobjectArray
getArtistsFromGenre(JNIEnv* env, jobject thiz, jobject medialibrary, jlong id)
{
    AndroidMediaLibrary *aml = MediaLibrary_getInstance(env, medialibrary);
    std::vector<medialibrary::ArtistPtr> artists = aml->artistsFromGenre(id);
    jobjectArray artistsRefs = (jobjectArray) env->NewGlobalRef(env->NewObjectArray(artists.size(), ml_fields.Artist.clazz, NULL));
    int index = -1;
    for(medialibrary::ArtistPtr const& artist : artists) {
        jobject item = convertArtistObject(env, &ml_fields, artist);
        env->SetObjectArrayElement(artistsRefs, ++index, item);
        env->DeleteLocalRef(item);
    }
    return artistsRefs;
}

 /*
  * JNI stuff
  */
static JNINativeMethod methods[] = {
    {"nativeInit", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)init },
    {"nativeRelease", "()V", (void*)release },
    {"nativeDiscover", "(Ljava/lang/String;)V", (void*)discover },
    {"nativeBanFolder", "(Ljava/lang/String;)V", (void*)banFolder },
    {"nativeLastMediaPlayed", "()[Lorg/videolan/medialibrary/media/MediaWrapper;", (void*)lastMediaPLayed },
    {"nativeGetVideos", "()[Lorg/videolan/medialibrary/media/MediaWrapper;", (void*)getVideos },
    {"nativeGetAudio", "()[Lorg/videolan/medialibrary/media/MediaWrapper;", (void*)getAudio },
    {"nativeGetVideoCount", "()I", (void*)getVideoCount },
    {"nativeGetAudioCount", "()I", (void*)getAudioCount },
    {"nativeGetAlbums", "()[Lorg/videolan/medialibrary/media/Album;", (void*)getAlbums },
    {"nativeGetArtists", "()[Lorg/videolan/medialibrary/media/Artist;", (void*)getArtists },
    {"nativeGetGenres", "()[Lorg/videolan/medialibrary/media/Genre;", (void*)getGenres },
    {"nativeIsWorking", "()Z", (void*)isWorking },
    {"nativePauseBackgroundOperations", "()V", (void*)pauseBackgroundOperations },
    {"nativeResumeBackgroundOperations", "()V", (void*)resumeBackgroundOperations },
    {"nativeReload", "()V", (void*)reload },
    {"nativeReload", "(Ljava/lang/String;)V", (void*)reloadEntryPoint },
    {"nativeIncreasePlayCount", "(J)Z", (void*)increasePlayCount },
    {"nativeUpdateProgress", "(JJ)Z", (void*)updateProgress },
    {"nativeSetMediaUpdatedCbFlag", "(I)V", (void*)setMediaUpdatedCbFlag },
    {"nativeSetMediaAddedCbFlag", "(I)V", (void*)setMediaAddedCbFlag },
};

static JNINativeMethod album_methods[] = {
    {"nativeGetTracksFromAlbum", "(Lorg/videolan/medialibrary/Medialibrary;J)[Lorg/videolan/medialibrary/media/MediaWrapper;", (void*)getTracksFromAlbum },
};

static JNINativeMethod artist_methods[] = {
    {"nativeGetMediaFromArtist", "(Lorg/videolan/medialibrary/Medialibrary;J)[Lorg/videolan/medialibrary/media/MediaWrapper;", (void*)getMediaFromArtist },
    {"nativeGetAlbumsFromArtist", "(Lorg/videolan/medialibrary/Medialibrary;J)[Lorg/videolan/medialibrary/media/Album;", (void*)getAlbumsFromArtist },
};

static JNINativeMethod genre_methods[] = {
    {"nativeGetTracksFromGenre", "(Lorg/videolan/medialibrary/Medialibrary;J)[Lorg/videolan/medialibrary/media/MediaWrapper;", (void*)getMediaFromGenre },
    {"nativeGetAlbumsFromGenre", "(Lorg/videolan/medialibrary/Medialibrary;J)[Lorg/videolan/medialibrary/media/Album;", (void*)getAlbumsFromGenre },
    {"nativeGetArtistsFromGenre", "(Lorg/videolan/medialibrary/Medialibrary;J)[Lorg/videolan/medialibrary/media/Artist;", (void*)getArtistsFromGenre },
};

jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNIEnv* env = NULL;
    // Keep a reference on the Java VM.
    myVm = vm;

    if (vm->GetEnv((void**) &env, VLC_JNI_VERSION) != JNI_OK)
        return -1;

#define GET_CLASS(clazz, str, b_globlal) do { \
    (clazz) = env->FindClass((str)); \
    if (!(clazz)) { \
    LOGE("FindClass(%s) failed", (str)); \
    return -1; \
} \
    if (b_globlal) { \
    (clazz) = (jclass) env->NewGlobalRef((clazz)); \
    if (!(clazz)) { \
    LOGE("NewGlobalRef(%s) failed", (str)); \
    return -1; \
} \
} \
} while (0)

#define GET_ID(get, id, clazz, str, args) do { \
    (id) = env->get((clazz), (str), (args)); \
    if (!(id)) { \
    LOGE(#get"(%s) failed", (str)); \
    return -1; \
} \
} while (0)

    GET_CLASS(ml_fields.IllegalStateException.clazz,
              "java/lang/IllegalStateException", true);
    GET_CLASS(ml_fields.IllegalArgumentException.clazz,
              "java/lang/IllegalArgumentException", true);

    GET_CLASS(ml_fields.MediaLibrary.clazz, CLASSPATHNAME, true);
    if (env->RegisterNatives(ml_fields.MediaLibrary.clazz, methods, sizeof(methods) / sizeof(methods[0])) < 0) {
        LOGE("RegisterNatives failed for '%s'", CLASSPATHNAME);
        return -1;
    }

    GET_CLASS(ml_fields.Artist.clazz, "org/videolan/medialibrary/media/Artist", true);
    if (env->RegisterNatives(ml_fields.Artist.clazz, artist_methods, sizeof(artist_methods) / sizeof(artist_methods[0])) < 0) {
        LOGE("RegisterNatives failed for org/videolan/medialibrary/media/Artist");
        return -1;
    }
    GET_ID(GetMethodID,
           ml_fields.Artist.initID,
           ml_fields.Artist.clazz,
           "<init>", "(JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");

    GET_CLASS(ml_fields.Album.clazz, "org/videolan/medialibrary/media/Album", true);
    if (env->RegisterNatives(ml_fields.Album.clazz, album_methods, sizeof(album_methods) / sizeof(album_methods[0])) < 0) {
        LOGE("RegisterNatives failed for 'org/videolan/medialibrary/media/Album");
        return -1;
    }
    GET_ID(GetMethodID,
           ml_fields.Album.initID,
           ml_fields.Album.clazz,
           "<init>", "(JLjava/lang/String;ILjava/lang/String;Ljava/lang/String;JII)V");

    GET_CLASS(ml_fields.Genre.clazz, "org/videolan/medialibrary/media/Genre", true);
    if (env->RegisterNatives(ml_fields.Genre.clazz, genre_methods, sizeof(genre_methods) / sizeof(genre_methods[0])) < 0) {
        LOGE("RegisterNatives failed for org/videolan/medialibrary/media/Genre");
        return -1;
    }
    GET_ID(GetMethodID,
           ml_fields.Genre.initID,
           ml_fields.Genre.clazz,
           "<init>", "(JLjava/lang/String;)V");

    GET_ID(GetFieldID,
           ml_fields.MediaLibrary.instanceID,
           ml_fields.MediaLibrary.clazz,
           "mInstanceID", "J");

    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onMediaAddedId,
           ml_fields.MediaLibrary.clazz,
           "onMediaAdded", "([Lorg/videolan/medialibrary/media/MediaWrapper;)V");
    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onMediaUpdatedId,
           ml_fields.MediaLibrary.clazz,
           "onMediaUpdated", "([Lorg/videolan/medialibrary/media/MediaWrapper;)V");
    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onMediaDeletedId,
           ml_fields.MediaLibrary.clazz,
           "onMediaDeleted", "([J)V");
    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onDiscoveryStartedId,
           ml_fields.MediaLibrary.clazz,
           "onDiscoveryStarted", "(Ljava/lang/String;)V");
    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onDiscoveryProgressId,
           ml_fields.MediaLibrary.clazz,
           "onDiscoveryProgress", "(Ljava/lang/String;)V");
    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onDiscoveryCompletedId,
           ml_fields.MediaLibrary.clazz,
           "onDiscoveryCompleted", "(Ljava/lang/String;)V");
    GET_ID(GetMethodID,
           ml_fields.MediaLibrary.onParsingStatsUpdatedId,
           ml_fields.MediaLibrary.clazz,
           "onParsingStatsUpdated", "(I)V");


    GET_CLASS(ml_fields.MediaWrapper.clazz,
              "org/videolan/medialibrary/media/MediaWrapper", true);

    GET_ID(GetMethodID,
           ml_fields.MediaWrapper.initID,
           ml_fields.MediaWrapper.clazz,
           "<init>", "(JLjava/lang/String;JJILjava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;IILjava/lang/String;IIIIJ)V");

#undef GET_CLASS
#undef GET_ID

    LOGD("JNI interface loaded.");
    return VLC_JNI_VERSION;
}

void JNI_OnUnload(JavaVM* vm, void* reserved)
{
    LOGD("JNI interface unloaded.");
    JNIEnv* env = NULL;

    if (vm->GetEnv((void**) &env, VLC_JNI_VERSION) != JNI_OK)
        return;

    env->DeleteGlobalRef(ml_fields.IllegalArgumentException.clazz);
    env->DeleteGlobalRef(ml_fields.IllegalStateException.clazz);
    env->DeleteGlobalRef(ml_fields.MediaLibrary.clazz);
    env->DeleteGlobalRef(ml_fields.MediaWrapper.clazz);
}


static inline void throw_IllegalStateException(JNIEnv *env, const char *p_error)
{
    env->ThrowNew(ml_fields.IllegalStateException.clazz, p_error);
}

static inline void throw_IllegalArgumentException(JNIEnv *env, const char *p_error)
{
    env->ThrowNew(ml_fields.IllegalArgumentException.clazz, p_error);
}

static AndroidMediaLibrary *
MediaLibrary_getInstanceInternal(JNIEnv *env, jobject thiz)
{
    return (AndroidMediaLibrary*)(intptr_t) env->GetLongField(thiz,
                                                              ml_fields.MediaLibrary.instanceID);
}

AndroidMediaLibrary *
MediaLibrary_getInstance(JNIEnv *env, jobject thiz)
{
    AndroidMediaLibrary *p_obj = MediaLibrary_getInstanceInternal(env, thiz);
    if (!p_obj)
        throw_IllegalStateException(env, "can't get VLCObject instance");
    return p_obj;
}


static void
MediaLibrary_setInstance(JNIEnv *env, jobject thiz, AndroidMediaLibrary *p_obj)
{
    env->SetLongField(thiz,
                      ml_fields.MediaLibrary.instanceID,
                      (jlong)(intptr_t)p_obj);
}
