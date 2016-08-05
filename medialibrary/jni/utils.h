/*****************************************************************************
 * utils.h
 *****************************************************************************
 * Copyright © 2016 VLC authors and VideoLAN
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_MEDIALIB_UTILS_H
#define VLC_MEDIALIB_UTILS_H
#include <jni.h>
#include <medialibrary/Types.h>
#include <medialibrary/IAlbumTrack.h>
#include <medialibrary/IVideoTrack.h>
#include <medialibrary/IFile.h>
#include <medialibrary/IMedia.h>
#include <medialibrary/IArtist.h>
#include <medialibrary/IGenre.h>
#include <medialibrary/IAlbum.h>

#define VLC_JNI_VERSION JNI_VERSION_1_2

struct fields {
    jint SDK_INT;
    struct IllegalStateException {
        jclass clazz;
    } IllegalStateException;
    struct IllegalArgumentException {
        jclass clazz;
    } IllegalArgumentException;
    struct MediaLibrary {
        jclass clazz;
        jfieldID instanceID;
        jmethodID onMediaAddedId;
        jmethodID onMediaUpdatedId;
    } MediaLibrary;
    struct MediaWrapper {
        jclass clazz;
        jmethodID initID;
    } MediaWrapper;
};

jobject
mediaToMediaWrapper(JNIEnv*, fields*, medialibrary::MediaPtr);

#endif //VLC_MEDIALIB_UTILS_H
