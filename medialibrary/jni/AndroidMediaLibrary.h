#ifndef ANDROIDMEDIALIBRARY_H
#define ANDROIDMEDIALIBRARY_H

#include <vector>
#include <string>
#include <tuple>
#include <stdlib.h>
#include <jni.h>

#include "AndroidDeviceLister.h"
#include "utils.h"

#include <medialibrary/IDeviceLister.h>
#include <medialibrary/IMedia.h>
#include <medialibrary/IMediaLibrary.h>
#include <medialibrary/Types.h>

class AndroidMediaLibrary : public medialibrary::IMediaLibraryCb
{
public:
    AndroidMediaLibrary(JavaVM *vm, fields *ref_fields, jobject thiz);
    ~AndroidMediaLibrary();

    void initDevices(const std::string& appDirPath, const std::string& libPath);
    void banFolder(const std::string& path);
    void discover(const std::string&);
    bool isWorking();
    void setMediaUpdatedCbFlag(int flags);
    void setMediaAddedCbFlag(int flags);
    void pauseBackgroundOperations();
    void resumeBackgroundOperations();
    void reload();
    void reload( const std::string& entryPoint );
    bool increasePlayCount(int64_t mediaId);
    std::vector<medialibrary::MediaPtr> lastMediaPlayed();
    std::vector<medialibrary::MediaPtr> videoFiles( medialibrary::SortingCriteria sort = medialibrary::SortingCriteria::Default, bool desc = false );
    std::vector<medialibrary::MediaPtr> audioFiles( medialibrary::SortingCriteria sort = medialibrary::SortingCriteria::Default, bool desc = false );


    void onMediaAdded( std::vector<medialibrary::MediaPtr> media );
    void onMediaUpdated( std::vector<medialibrary::MediaPtr> media ) ;
    void onMediaDeleted( std::vector<int64_t> ids ) ;

    void onArtistsAdded( std::vector<medialibrary::ArtistPtr> artists ) ;
    void onArtistsModified( std::vector<medialibrary::ArtistPtr> artist );
    void onArtistsDeleted( std::vector<int64_t> ids );

    void onAlbumsAdded( std::vector<medialibrary::AlbumPtr> albums );
    void onAlbumsModified( std::vector<medialibrary::AlbumPtr> albums );
    void onAlbumsDeleted( std::vector<int64_t> ids );
    void onTracksAdded( std::vector<medialibrary::AlbumTrackPtr> tracks );
    void onTracksDeleted( std::vector<int64_t> trackIds );
    void onDiscoveryStarted( const std::string& entryPoint );
    void onDiscoveryProgress( const std::string& entryPoint );
    void onDiscoveryCompleted( const std::string& entryPoint );
    void onParsingStatsUpdated( uint32_t percent);

private:
    JNIEnv *getEnv();
    void DetachCurrentThread();

    JavaVM *myVm;
    jweak weak_thiz;
    fields *p_fields;
    medialibrary::IMediaLibrary* p_ml;
    std::shared_ptr<AndroidDeviceLister> p_lister;
};
#endif // ANDROIDMEDIALIBRARY_H
