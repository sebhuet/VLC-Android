
#include "utils.h"
#define LOG_TAG "VLC/JNI/Utils"
#include "log.h"

jobject
mediaToMediaWrapper(JNIEnv* env, fields *fields, medialibrary::MediaPtr mediaPtr)
{
    //TODO get track, audio & spu track numbers
    jint type;
    switch (mediaPtr->type()) {
    case medialibrary::IMedia::Type::AudioType:
        type = 1; //MediaWrapper.TYPE_AUDIO
        break;
    case medialibrary::IMedia::Type::VideoType:
        type = 0; //MediaWrapper.TYPE_VIDEO
        break;
    default:
        type = -1; //MediaWrapper.TYPE_ALL
        break;
    }
    medialibrary::AlbumTrackPtr p_albumTrack = mediaPtr->albumTrack();
    jstring artist, genre, album, albumArtist, mrl, title, thumbnail;
    if (p_albumTrack) {
        if (p_albumTrack->artist() != nullptr) {
            artist = env->NewStringUTF(p_albumTrack->artist()->name().c_str());
        } else
            artist = nullptr;
        if (p_albumTrack->genre() != nullptr) {
            genre = env->NewStringUTF(p_albumTrack->genre()->name().c_str());
        } else
            genre = nullptr;
        medialibrary::AlbumPtr albumPtr = p_albumTrack->album();
        if (albumPtr!= nullptr) {
            album = env->NewStringUTF(albumPtr->title().c_str());
        } else
            album = nullptr;
        if (albumPtr != nullptr && albumPtr->albumArtist() != nullptr) {
            albumArtist = env->NewStringUTF(albumPtr->albumArtist()->name().c_str());
        } else
            albumArtist = nullptr;
    } else {
        artist = nullptr;
        genre = nullptr;
        album = nullptr;
        albumArtist = nullptr;
    }
    title = env->NewStringUTF(mediaPtr->title().c_str());
    mrl = env->NewStringUTF(mediaPtr->files().at(0)->mrl().c_str());
    thumbnail = env->NewStringUTF(mediaPtr->thumbnail().c_str());
    bool hasVideoTracks = !mediaPtr->videoTracks().empty();
    unsigned int width = hasVideoTracks ? mediaPtr->videoTracks().at(0)->width() : 0;
    unsigned int height = hasVideoTracks ? mediaPtr->videoTracks().at(0)->height() : 0;

    jobject item = env->NewObject(fields->MediaWrapper.clazz, fields->MediaWrapper.initID,
                          mrl,(jlong) mediaPtr->duration(), (jlong) -1, type,
                          title, artist, genre, album,
                          albumArtist, width, height, thumbnail,
                          (jint) -2, (jint) -2, (jint) 0, (jint) 0, (jlong) mediaPtr->files().at(0)->lastModificationDate());
    if (artist != nullptr)
        env->DeleteLocalRef(artist);
    if (genre != nullptr)
        env->DeleteLocalRef(genre);
    if (album != nullptr)
        env->DeleteLocalRef(album);
    if (albumArtist != nullptr)
        env->DeleteLocalRef(albumArtist);
    env->DeleteLocalRef(title);
    env->DeleteLocalRef(mrl);
    env->DeleteLocalRef(thumbnail);
    return item;
}
