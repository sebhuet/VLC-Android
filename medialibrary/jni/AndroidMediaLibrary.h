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
    AndroidMediaLibrary();
    ~AndroidMediaLibrary();

    void initDevices(const std::string& appDirPath, const std::string& libPath);
    void discover(const std::string&);
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
    void onDiscoveryCompleted( const std::string& entryPoint );
    void onParsingStatsUpdated( uint32_t percent);

private:
    JavaVM *myVm;
    fields m_fields;
    medialibrary::IMediaLibrary* p_ml;
    std::shared_ptr<AndroidDeviceLister> p_lister;
};
#endif // ANDROIDMEDIALIBRARY_H
