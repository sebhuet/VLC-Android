
#include "utils.h"
#include <medialibrary/IAlbumTrack.h>
#include <medialibrary/IVideoTrack.h>
#include <medialibrary/IFile.h>
#include <medialibrary/IMedia.h>
#include <medialibrary/IArtist.h>
#include <medialibrary/IGenre.h>
#include <medialibrary/IAlbum.h>
#define LOG_TAG "VLC/JNI/Utils"
#include "log.h"

jobject
mediaToMediaWrapper(JNIEnv* env, fields *fields, medialibrary::MediaPtr const& mediaPtr)
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
    jstring artist = NULL, genre = NULL, album = NULL, albumArtist = NULL, mrl = NULL, title = NULL, thumbnail = NULL;
    if (p_albumTrack) {
        if (p_albumTrack->artist() != NULL)
            artist = env->NewStringUTF(p_albumTrack->artist()->name().c_str());
        if (p_albumTrack->genre() != NULL)
            genre = env->NewStringUTF(p_albumTrack->genre()->name().c_str());
        medialibrary::AlbumPtr albumPtr = p_albumTrack->album();
        if (albumPtr!= NULL) {
            album = env->NewStringUTF(albumPtr->title().c_str());
            if (albumPtr->albumArtist() != NULL)
                albumArtist = env->NewStringUTF(albumPtr->albumArtist()->name().c_str());
        }
    }
    title = mediaPtr->title().empty() ? NULL : env->NewStringUTF(mediaPtr->title().c_str());
    mrl = mediaPtr->files().at(0)->mrl().empty() ? NULL : env->NewStringUTF(mediaPtr->files().at(0)->mrl().c_str());
    thumbnail = mediaPtr->thumbnail().empty() ? NULL : env->NewStringUTF(mediaPtr->thumbnail().c_str());
    bool hasVideoTracks = !mediaPtr->videoTracks().empty();
    unsigned int width = hasVideoTracks ? mediaPtr->videoTracks().at(0)->width() : 0;
    unsigned int height = hasVideoTracks ? mediaPtr->videoTracks().at(0)->height() : 0;
    int64_t duration = mediaPtr->duration();
    int64_t progress = duration* mediaPtr->progress();

    jobject item = env->NewObject(fields->MediaWrapper.clazz, fields->MediaWrapper.initID,
                          (jlong) mediaPtr->id(), mrl,(jlong) progress, (jlong) duration, type,
                          title, artist, genre, album,
                          albumArtist, width, height, thumbnail,
                          (jint) -2, (jint) -2, (jint) 0, (jint) 0, (jlong) mediaPtr->files().at(0)->lastModificationDate());
    if (artist != NULL)
        env->DeleteLocalRef(artist);
    if (genre != NULL)
        env->DeleteLocalRef(genre);
    if (album != NULL)
        env->DeleteLocalRef(album);
    if (albumArtist != NULL)
        env->DeleteLocalRef(albumArtist);
    if (title != NULL)
        env->DeleteLocalRef(title);
    if (mrl != NULL)
        env->DeleteLocalRef(mrl);
    if (thumbnail != NULL)
        env->DeleteLocalRef(thumbnail);
    return item;
}

jobject
convertAlbumObject(JNIEnv* env, fields *fields, medialibrary::AlbumPtr const& albumPtr)
{
    jstring title = env->NewStringUTF(albumPtr->title().c_str());
    jstring artworkMrl = env->NewStringUTF(albumPtr->artworkMrl().c_str());
    jlong albumArtistId = albumPtr->albumArtist() != nullptr ? albumPtr->albumArtist()->id() : 0;
    jobject item = env->NewObject(fields->Album.clazz, fields->Album.initID,
                          (jlong) albumPtr->id(), title, albumPtr->releaseYear(), artworkMrl, albumArtistId, (jint) albumPtr->nbTracks(), (jint) albumPtr->duration());
    env->DeleteLocalRef(title);
    env->DeleteLocalRef(artworkMrl);
    return item;
}

jobject
convertArtistObject(JNIEnv* env, fields *fields, medialibrary::ArtistPtr const& artistPtr)
{
    jstring name = env->NewStringUTF(artistPtr->name().c_str());
    jstring artworkMrl = env->NewStringUTF(artistPtr->artworkMrl().c_str());
    jstring shortBio = env->NewStringUTF(artistPtr->shortBio().c_str());
    jstring musicBrainzId = env->NewStringUTF(artistPtr->musicBrainzId().c_str());
    jobject item = env->NewObject(fields->Artist.clazz, fields->Artist.initID,
                          (jlong) artistPtr->id(), name, shortBio, artworkMrl, musicBrainzId);
    env->DeleteLocalRef(name);
    env->DeleteLocalRef(artworkMrl);
    env->DeleteLocalRef(shortBio);
    env->DeleteLocalRef(musicBrainzId);
    return item;
}

jobject
convertGenreOgbject(JNIEnv* env, fields *fields, medialibrary::GenrePtr const& genrePtr)
{
    jstring name = env->NewStringUTF(genrePtr->name().c_str());
    jobject item = env->NewObject(fields->Genre.clazz, fields->Genre.initID,
                          (jlong) genrePtr->id(), name);
    env->DeleteLocalRef(name);
    return item;
}
